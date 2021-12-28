#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //initialize UI
    videoStopped = true;
    isImage = false;
    modelLoaded = false;
    ui->tabs->setCurrentIndex(0);
    ui->videoView->setScene(&scene);
    scene.addItem(&pixmap);
    ui->lineSx->setValidator( new QIntValidator(100, 4000, this) );
    ui->lineSy->setValidator( new QIntValidator(100, 4000, this) );
    ui->lineConfidence->setValidator( new QDoubleValidator(0.01, 0.99, 2, this) );

    //connect signals to slots
    QObject::connect(ui->startBtn, &QPushButton::clicked, this, &MainWindow::startPressed);
    QObject::connect(ui->loadBtn, &QPushButton::clicked, this, &MainWindow::loadPressed);
    QObject::connect(ui->browseVideoBtn, &QPushButton::clicked, this, &MainWindow::browseVideoPressed);
    QObject::connect(ui->browseImageBtn, &QPushButton::clicked, this, &MainWindow::browseImagePressed);
    QObject::connect(ui->pbBrowseBtn, &QPushButton::clicked, this, &MainWindow::browsePbPressed);
    QObject::connect(ui->pbtxtBrowseBtn, &QPushButton::clicked, this, &MainWindow::browsePbtxtPressed);
    QObject::connect(ui->classesBrowseBtn, &QPushButton::clicked, this, &MainWindow::browseClassesPressed);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadPressed()
{
    if(!videoStopped)
    {
        videoStopped = true;
        ui->startBtn->setText("Start");
        return;
    }

    using namespace cv;
    using namespace dnn;
    using namespace std;

    QElapsedTimer timer;//model loading timer
    timer.start();

    QFile labelsFile(ui->classesFileEdit->text());
    if(labelsFile.open(QFile::ReadOnly | QFile::Text))
    {
        while(!labelsFile.atEnd())//Parse CSV for labels
        {
            QString line = labelsFile.readLine();
            classNames[line.split(',')[0].trimmed().toInt()] = line.split(',')[1].trimmed();
        }
        labelsFile.close();
    }
    else
    {
        QMessageBox::warning(this,
                             "Warning",
                             "Make sure a valid class list is being loaded.");
        return;
    }

    try
    {
        tfNetwork = readNetFromTensorflow(ui->pbFileEdit->text().toStdString(),
                                      ui->pbtxtFileEdit->text().toStdString());
    }
    catch ( cv::Exception & e )
    {
        QMessageBox::warning(this,
                             "Exception",
                             (QString::fromStdString(e.msg) + "\nMake sure a valid model is being loaded."));
        return;
    }
    //tfNetwork.setPreferableTarget(DNN_TARGET_OPENCL);!!!

    // see the following for more on this parameters
    // https://www.tensorflow.org/tutorials/image_retraining
    inWidth = ui->lineSx->text().toInt();
    inHeight = ui->lineSy->text().toInt();
    meanVal = 127.5; // 255 divided by 2
    inScaleFactor = 1.0f;// used to be "1.0f / meanVal" for older version of openCV
    confidenceThreshold = ui->lineConfidence->text().toFloat();

    modelLoaded = true;

    qint64 elapsed = timer.elapsed();
    QMessageBox::information(this,
                             "Info",
                             QString("Network is loaded in %1 ms").arg(elapsed));
}

void MainWindow::startPressed()
{
    if(!modelLoaded)
    {
        QMessageBox::warning(this,
                             "Warning",
                             "Load a model first.");
        return;
    }

    if(!videoStopped)
    {
        videoStopped = true;
        ui->startBtn->setText("Start");
        return;
    }

    using namespace cv;
    using namespace dnn;
    using namespace std;

    VideoCapture video;
    Mat image;

    if(ui->cameraRadio->isChecked())
    {
        if(!video.open(ui->cameraSpin->value()))
        {
            QMessageBox::warning(this,
                                 "Warning",
                                 "Make sure a valid camera Id is used.");
            return;
        }

        ui->startBtn->setText("Stop");
        isImage = false;
    }
    else if(ui->videoRadio->isChecked())
    {
        if(!video.open(ui->videoEdit->text().toStdString()))
        {
            QMessageBox::warning(this,
                                 "Warning",
                                 "Make sure a valid video is being loaded.");
            return;
        }

        ui->startBtn->setText("Stop");
        isImage = false;
    }
    else
    {
        isImage = true;
        image = imread(ui->imageEdit->text().toStdString(),IMREAD_COLOR);

        if(image.empty())
        {
            QMessageBox::warning(this,
                                 "Warning",
                                 "Make sure a valid image is being loaded.");
            return;
        }
    }

    videoStopped = false;

    while(!videoStopped && (video.isOpened() | isImage))
    {
        if(isImage)
            videoStopped = true;
        else
           video >> image;

        Mat inputBlob = blobFromImage(image,
                                      inScaleFactor,
                                      Size(inWidth, inHeight),
                                      Scalar(meanVal, meanVal, meanVal),
                                      true,
                                      false);
        tfNetwork.setInput(inputBlob);
        Mat result = tfNetwork.forward();
        Mat detections(result.size[2], result.size[3], CV_32F, result.ptr<float>());
        for(int i=0; i<detections.rows; i++)
        {
            float confidence = detections.at<float>(i, 2);
            if(confidence > confidenceThreshold)
            {
                using namespace cv;
                int objectClass = (int)(detections.at<float>(i, 1));

                int left = static_cast<int>(detections.at<float>(i, 3) * image.cols);
                int top = static_cast<int>(detections.at<float>(i, 4) * image.rows);
                int right = static_cast<int>(detections.at<float>(i, 5) * image.cols);
                int bottom = static_cast<int>(detections.at<float>(i, 6) * image.rows);

                rectangle(image, Point(left, top), Point(right, bottom), Scalar(0, 255, 0));
                String label = (classNames[objectClass] + " " + QString::number(confidence*100) + "%").toStdString();
                int baseLine = 1;
                Size labelSize = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.5, 2, &baseLine);
                top = max(top, labelSize.height);
                rectangle(image, Point(left, top - labelSize.height),
                          Point(left + labelSize.width, top + baseLine),
                          Scalar(255, 255, 255), FILLED);
                putText(image, label, Point(left, top),
                        FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0,0,0));
            }
        }

        pixmap.setPixmap(QPixmap::fromImage(QImage(image.data,
                                                   image.cols,
                                                   image.rows,
                                                   image.step,
                                                   QImage::Format_RGB888).rgbSwapped()));
        ui->videoView->fitInView(&pixmap, Qt::KeepAspectRatio);

        qApp->processEvents();
    }

    if(!isImage)
        video.release();
}

//close camera before exiting !!!

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(!videoStopped)
    {
        QMessageBox::warning(this,
                             "Warning",
                             "First make sure you stop the video/camera!");
        event->ignore();
    }
    else
    {
        event->accept();
    }
}

//file selection

void MainWindow::browseVideoPressed()
{
    QString fname = QFileDialog::getOpenFileName(this, "Open Video", QString(), "Videos (*.avi *.mp4 *.mov)");
    if(QFile::exists(fname))
        ui->videoEdit->setText(fname);
}

void MainWindow::browseImagePressed()
{
    QString fname = QFileDialog::getOpenFileName(this, "Open Image", QString(), "Images (*.bmp *.dib *.jpeg *.jpg *.jpe *.jp2 *.png *.webp *.pbm *.pgm *.ppm *.pxm *.pnm *.sr *.ras *.tiff *.tif *.exr *.hdr *.pic)");
    if(QFile::exists(fname))
        ui->imageEdit->setText(fname);
}

void MainWindow::browsePbPressed()
{
    QString fname = QFileDialog::getOpenFileName(this, "Open Model", QString(), "PB files (*.pb)");
    if(QFile::exists(fname))
        ui->pbFileEdit->setText(fname);
}

void MainWindow::browsePbtxtPressed()
{
    QString fname = QFileDialog::getOpenFileName(this, "Open Config", QString(), "PBTXT files (*.pbtxt)");
    if(QFile::exists(fname))
        ui->pbtxtFileEdit->setText(fname);
}

void MainWindow::browseClassesPressed()
{
    QString fname = QFileDialog::getOpenFileName(this, "Open Classes", QString(), "CSV files (*.csv)");
    if(QFile::exists(fname))
        ui->classesFileEdit->setText(fname);
}
