OpenCV DNN implementation on a QT application
========================

#Summary
This is an example of how to implement a deep-learning model (TensorFlow model, in this example) on a QT application using openCV.
#Environment
Built on Ubuntu 20.04.3 LTS using:
- QT 6.1.1
- CMake 3.16.3
- GCC compiler

Aditional libraries used for development:
- OpenCV 4.5.4 , included in repository.
- Python 3 , used to run script to convert model.
- ssd_mobilenet_v1_coco , model being implemented.

#Development process
A minimal QT widget project was used as a base for development, adding the path to the OpenCV build directory and linking it on the CMakeList.
From here, a basic UI was made:
- ```QGraphicsView``` to display the output ```QGraphicsScene```.
- ```QTabWidget``` to separate the controls from the settings/model selection.
- ```QRadioButton``` was used to select the desired input.
- ```QSpinBox``` was used to select the camera index.
- ```QLineEdit``` was used to store file paths and numeric inputs.
- ```QPushButton``` was used for all buttons.
- Corresponding layout components.

Having a basic UI, th next step was to connect buttons to their respective functions in the MainWindow class. In QT, this is done with signals and slots.
The corresponding functions are declared as private slots, and are connected to their button's ```clicked()``` signals:
- mainwindow.h:
```c++
private slots:
    void startPressed();
```
- mainwindow.cpp:
```c++
QObject::connect(ui->startBtn, &QPushButton::clicked, this,
                     &MainWindow::startPressed);
```

Outside of loading and starting/stopping the model, the remaining buttons are for selecting files.
For this, we use a ```QFileDialog```:
```c++
void MainWindow::browseVideoPressed() {
    QString fname = QFileDialog::getOpenFileName(this, "Open Video", QString(),
                                                 "Videos (*.avi *.mp4 *.mov)");
    if (QFile::exists(fname)) ui->videoEdit->setText(fname);
}
```

Before implementing the load and start functions, some state variables and OpenCV specific variables are neccesary.
mainwindow.h:
```c++
private:
    Ui::MainWindow *ui;

    cv::dnn::Net tfNetwork;

    QGraphicsScene scene;
    QGraphicsPixmapItem pixmap;
    QMap<int,QString> classNames;

    bool videoStopped;
    bool isImage;
    bool modelLoaded;

    int inWidth;
    int inHeight;
    float meanVal;
    float inScaleFactor;
    float confidenceThreshold;

```
- The three state flags are for: Stopping the processing of the video input, processing a single image instead of a video, and keeping track of a valid model being loaded.

- ```QGraphicsView``` displays a ```QGraphicsScene```, to which we assing a ```QGraphicsPixmapItem``` that we can write to.
- ```QMap``` is used to map class ids to their labels.

Now ```loadPressed()``` can be implemented:
- We want the video proccessing to stop if a model is being loaded:
```c++
if (!videoStopped) {
    // set video to be stoped, change "stop/start" button
    videoStopped = true;
    ui->startBtn->setText("Start");
    return;
}
```
- A timer is used to keep track of model loading:
```c++
QElapsedTimer timer;  // model loading timer
timer.start();
```
- The class CSV file is loaded and parsed:
```c++
// load classes CSV file, and parse
QFile labelsFile(ui->classesFileEdit->text());
if (labelsFile.open(QFile::ReadOnly | QFile::Text)) {
    while (!labelsFile.atEnd())  // Parse CSV for labels
    {
        QString line = labelsFile.readLine();
        classNames[line.split(',')[0].trimmed().toInt()] =
            line.split(',')[1].trimmed();
    }
    labelsFile.close();
} else {
    QMessageBox::warning(this, "Warning",
                         "Make sure a valid class list is being loaded.");
    return;
}
```
- The tensor flow model is loaded:
```c++
// Load tensorflow model
try {
    tfNetwork = readNetFromTensorflow(ui->pbFileEdit->text().toStdString(),
                                      ui->pbtxtFileEdit->text().toStdString());
} catch (cv::Exception &e) {
    QMessageBox::warning(this, "Exception",
                         (QString::fromStdString(e.msg) +
                          "\nMake sure a valid model is being loaded."));
    return;
}
```
- The preffered target is set to openCL, in order to use a GPU if available:
```c++
tfNetwork.setPreferableTarget(DNN_TARGET_OPENCL);
```
- Input parameters are read and set:
```c++
// Set parameters from UI
inWidth = ui->lineSx->text().toInt();
inHeight = ui->lineSy->text().toInt();
confidenceThreshold = ui->lineConfidence->text().toFloat();
// set parameters that don't usually need to be changed
meanVal = 127.5;       // 255 divided by 2
inScaleFactor = 1.0f;  // used to be "1.0f / meanVal" for older version of openCV
```
- Once succesfully loaded, set the state, and display elapsed time:
```c++
//model succesfully loaded
modelLoaded = true;

// get load time from timer, display
qint64 elapsed = timer.elapsed();
QMessageBox::information(this, "Info",
                         QString("Network is loaded in %1 ms").arg(elapsed));
```
Now ```startPressed()``` can be implemented:
- Once more, we want the video proccessing to stop if already running, and to not start if a model isn't loaded:
```c++
if (!modelLoaded) {
    QMessageBox::warning(this, "Warning", "Load a model first.");
    return;
}
if (!videoStopped) {
    // stop processing video if already active.
    videoStopped = true;
    ui->startBtn->setText("Start");
    return;
}
```
- Depending on if an image is to be processed, or a video, we set the apropriate state, load the corresponding input, and try to catch any inadequate input:
```c++
VideoCapture video;
Mat image;
// what to process:
if (ui->imageRadio->isChecked()) {
    // single image: set state, load image
    isImage = true;
    image = imread(ui->imageEdit->text().toStdString(), IMREAD_COLOR);
    if (image.empty()) {
        QMessageBox::warning(this, "Warning",
                             "Make sure a valid image is being loaded.");
        return;
    }
} else {
    // video: set state, check if from camera or file
    isImage = false;
    if (ui->cameraRadio->isChecked()) {
        // camera input, check if valid camera selected, update "start/stop" button
        if (!video.open(ui->cameraSpin->value())) {
            QMessageBox::warning(this, "Warning",
                                 "Make sure a valid camera Id is used.");
            return;
        }
        ui->startBtn->setText("Stop");
    } else {
        // video file, check if valid file selected, update "start/stop" button
        if (!video.open(ui->videoEdit->text().toStdString())) {
            QMessageBox::warning(this, "Warning",
                                 "Make sure a valid video is being loaded.");
            return;
        }
        ui->startBtn->setText("Stop");
    }
}
// input is valid, can start processing
videoStopped = false;
```
- Now the loop which will repeat for every frame, unless it's a single image:
```c++
// keep processing video until users presses "stop" or no more frames to process
while (!videoStopped && (video.isOpened() | isImage)) {
    // if image, will be done after one loop, otherwise load current frame from video
    if (isImage)
        videoStopped = true;
    else
        video >> image;

    //Rest of image processing...
}
```
- Within the loop, the image is turned into a blob, set as input, run through the model, and results extracted:
```c++
// turn to blob for processing
Mat inputBlob = blobFromImage(image, inScaleFactor, Size(inWidth, inHeight),
                              Scalar(meanVal, meanVal, meanVal), true, false);
// set input, run model, get results
tfNetwork.setInput(inputBlob);
Mat result = tfNetwork.forward();
Mat detections(result.size[2], result.size[3], CV_32F, result.ptr<float>());
```
- Now loop over the detections:
```c++
// draw detections where confidence is over threshold
for (int i = 0; i < detections.rows; i++) {
    //confidence threshold check and bounding box rendering...
}
```
- Check confidence threshold, get class and bounds:
```c++
float confidence = detections.at<float>(i, 2);

if (confidence > confidenceThreshold) {
    // get class and bounds
    int objectClass = (int)(detections.at<float>(i, 1));
    int left = static_cast<int>(detections.at<float>(i, 3) * image.cols);
    int top = static_cast<int>(detections.at<float>(i, 4) * image.rows);
    int right = static_cast<int>(detections.at<float>(i, 5) * image.cols);
    int bottom = static_cast<int>(detections.at<float>(i, 6) * image.rows);

    //bounding box rendering...
}
```
- Render bounding box and label:
```c++
// draw bounding rectangle and label with class and confidence
rectangle(image, Point(left, top), Point(right, bottom),
          Scalar(0, 255, 0));
String label = (classNames[objectClass] + " " +
                QString::number(confidence * 100) + "%")
                   .toStdString();
int baseLine = 0;
Size labelSize =
    getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.5, 2, &baseLine);
top = max(top, labelSize.height);
rectangle(image, Point(left, top - labelSize.height),
          Point(left + labelSize.width, top + baseLine),
          Scalar(255, 255, 255), FILLED);
putText(image, label, Point(left, top), FONT_HERSHEY_SIMPLEX, 0.5,
        Scalar(0, 0, 0));
```
- After rendering all detections, display the image, then process QT events before ending the loop:
```c++
// turn image to pixmap for displaying
pixmap.setPixmap(QPixmap::fromImage(
    QImage(image.data, image.cols, image.rows, image.step, QImage::Format_RGB888)
        .rgbSwapped()));
ui->videoView->fitInView(&pixmap, Qt::KeepAspectRatio);

// maintains some responsiveness, should ideally run on a separate thread
qApp->processEvents();
```
- Once the loop is exited through ```videoStopped``` being set to true, release the video:
```c++
// release video when done
if (!isImage) video.release();
```


#Usage
To load a TensorFlow model, it must first be converted to a ```*.pb``` and ```*.pbtxt```. For this purpose, OpenCV comes with a python script that does this conversion.
It is included as ```/ssd_mobilenet_v1_coco/tf_text_graph_ssd.py```.
Run the script with the following parameters:
```tf_text_graph_ssd.py --input frozen_inference_graph.pb --output frozen_inference_graph.pbtxt --config pipeline.config```
The CSV file with the corresponding labels is already included as```/ssd_mobilenet_v1_coco/class-names.csv```.

At this point, the application can be run, switching to the "settings" tab to select the weight files and labels CSV.
Switching back to the "controls" tab, an input can be selected, "load model" will load the model, and "start" will start the video/image proccessing.