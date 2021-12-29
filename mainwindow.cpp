#include "mainwindow.h"
#include <QDebug>
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // initialize state
    videoStopped = true;
    isImage = false;
    modelLoaded = false;

    // initialize UI
    ui->tabs->setCurrentIndex(0);
    ui->videoView->setScene(&scene);
    scene.addItem(&pixmap);
    ui->lineSx->setValidator(new QIntValidator(100, 4000, this));
    ui->lineSy->setValidator(new QIntValidator(100, 4000, this));
    ui->lineConfidence->setValidator(new QDoubleValidator(0.01, 0.99, 2, this));

    // connect signals to slots
    QObject::connect(ui->startBtn, &QPushButton::clicked, this,
                     &MainWindow::startPressed);
    QObject::connect(ui->loadBtn, &QPushButton::clicked, this, &MainWindow::loadPressed);
    QObject::connect(ui->browseVideoBtn, &QPushButton::clicked, this,
                     &MainWindow::browseVideoPressed);
    QObject::connect(ui->browseImageBtn, &QPushButton::clicked, this,
                     &MainWindow::browseImagePressed);
    QObject::connect(ui->pbBrowseBtn, &QPushButton::clicked, this,
                     &MainWindow::browsePbPressed);
    QObject::connect(ui->pbtxtBrowseBtn, &QPushButton::clicked, this,
                     &MainWindow::browsePbtxtPressed);
    QObject::connect(ui->classesBrowseBtn, &QPushButton::clicked, this,
                     &MainWindow::browseClassesPressed);
}

MainWindow::~MainWindow() { delete ui; }

// load network
void MainWindow::loadPressed() {
    if (!videoStopped) {
        // set video to be stoped, change "stop/start" button
        videoStopped = true;
        ui->startBtn->setText("Start");
        return;
    }

    using namespace cv;
    using namespace dnn;
    using namespace std;

    QElapsedTimer timer;  // model loading timer
    timer.start();

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

    // Use OpenCL. Too broke for CUDA, yet still better than running on CPU
    tfNetwork.setPreferableTarget(DNN_TARGET_OPENCL);

    // Set parameters from UI
    inWidth = ui->lineSx->text().toInt();
    inHeight = ui->lineSy->text().toInt();
    confidenceThreshold = ui->lineConfidence->text().toFloat();
    // set parameters that don't usually need to be changed
    meanVal = 127.5;       // 255 divided by 2
    inScaleFactor = 1.0f;  // used to be "1.0f / meanVal" for older version of openCV

    // model succesfully loaded
    modelLoaded = true;

    // get load time from timer, display
    qint64 elapsed = timer.elapsed();
    QMessageBox::information(this, "Info",
                             QString("Network is loaded in %1 ms").arg(elapsed));
}

// start/stop video/image processing
void MainWindow::startPressed() {
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

    using namespace cv;
    using namespace dnn;
    using namespace std;

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

    // keep processing video until users presses "stop" or no more frames to process
    while (!videoStopped && (video.isOpened() | isImage)) {
        // if image, will be done after one loop, otherwise load current frame from video
        if (isImage)
            videoStopped = true;
        else
            video >> image;

        // turn to blob for processing
        Mat inputBlob = blobFromImage(image, inScaleFactor, Size(inWidth, inHeight),
                                      Scalar(meanVal, meanVal, meanVal), true, false);
        // set input, run model, get results
        tfNetwork.setInput(inputBlob);
        Mat result = tfNetwork.forward();
        Mat detections(result.size[2], result.size[3], CV_32F, result.ptr<float>());

        // draw detections where confidence is over threshold
        for (int i = 0; i < detections.rows; i++) {
            float confidence = detections.at<float>(i, 2);

            if (confidence > confidenceThreshold) {
                // get class and bounds
                int objectClass = (int)(detections.at<float>(i, 1));
                int left = static_cast<int>(detections.at<float>(i, 3) * image.cols);
                int top = static_cast<int>(detections.at<float>(i, 4) * image.rows);
                int right = static_cast<int>(detections.at<float>(i, 5) * image.cols);
                int bottom = static_cast<int>(detections.at<float>(i, 6) * image.rows);

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
            }
        }

        // turn image to pixmap for displaying
        pixmap.setPixmap(QPixmap::fromImage(
            QImage(image.data, image.cols, image.rows, image.step, QImage::Format_RGB888)
                .rgbSwapped()));
        ui->videoView->fitInView(&pixmap, Qt::KeepAspectRatio);

        // maintains some responsiveness, should ideally run on a separate thread
        qApp->processEvents();
    }

    // release video when done
    if (!isImage) video.release();
}

// close, request that camera/video is closed before exiting
void MainWindow::closeEvent(QCloseEvent *event) {
    if (!videoStopped) {
        QMessageBox::warning(this, "Warning",
                             "First make sure you stop the video/camera!");
        event->ignore();
    } else {
        event->accept();
    }
}

// file selection`

void MainWindow::browseVideoPressed() {
    QString fname = QFileDialog::getOpenFileName(this, "Open Video", QString(),
                                                 "Videos (*.avi *.mp4 *.mov)");
    if (QFile::exists(fname)) ui->videoEdit->setText(fname);
}

void MainWindow::browseImagePressed() {
    QString fname = QFileDialog::getOpenFileName(
        this, "Open Image", QString(),
        "Images (*.bmp *.dib *.jpeg *.jpg *.jpe *.jp2 *.png *.webp *.pbm *.pgm *.ppm "
        "*.pxm *.pnm *.sr *.ras *.tiff *.tif *.exr *.hdr *.pic)");
    if (QFile::exists(fname)) ui->imageEdit->setText(fname);
}

void MainWindow::browsePbPressed() {
    QString fname =
        QFileDialog::getOpenFileName(this, "Open Model", QString(), "PB files (*.pb)");
    if (QFile::exists(fname)) ui->pbFileEdit->setText(fname);
}

void MainWindow::browsePbtxtPressed() {
    QString fname = QFileDialog::getOpenFileName(this, "Open Config", QString(),
                                                 "PBTXT files (*.pbtxt)");
    if (QFile::exists(fname)) ui->pbtxtFileEdit->setText(fname);
}

void MainWindow::browseClassesPressed() {
    QString fname = QFileDialog::getOpenFileName(this, "Open Classes", QString(),
                                                 "CSV files (*.csv)");
    if (QFile::exists(fname)) ui->classesFileEdit->setText(fname);
}
