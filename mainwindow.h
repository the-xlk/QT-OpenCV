#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QDebug>
#include <QFile>
#include <QElapsedTimer>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QCloseEvent>
#include <QFileDialog>
#include <opencv2/opencv.hpp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void closeEvent(QCloseEvent *event);

private slots:
    void startPressed();
    void loadPressed();
    void browseVideoPressed();
    void browseImagePressed();
    void browsePbPressed();
    void browsePbtxtPressed();
    void browseClassesPressed();

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

};

#endif // MAINWINDOW_H
