/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.1.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QTabWidget *tabs;
    QWidget *controlsTab;
    QGridLayout *gridLayout_3;
    QHBoxLayout *horizontalLayout_2;
    QRadioButton *videoRadio;
    QLineEdit *videoEdit;
    QPushButton *browseVideoBtn;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *loadBtn;
    QPushButton *startBtn;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout;
    QRadioButton *cameraRadio;
    QSpinBox *cameraSpin;
    QLabel *label_4;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_7;
    QRadioButton *imageRadio;
    QLineEdit *imageEdit;
    QPushButton *browseImageBtn;
    QWidget *settingsTab;
    QGridLayout *gridLayout_4;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label;
    QLineEdit *pbFileEdit;
    QPushButton *pbBrowseBtn;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_3;
    QLineEdit *classesFileEdit;
    QPushButton *classesBrowseBtn;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_2;
    QLineEdit *pbtxtFileEdit;
    QPushButton *pbtxtBrowseBtn;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_5;
    QLineEdit *lineSx;
    QLabel *label_6;
    QLineEdit *lineSy;
    QLabel *label_7;
    QLineEdit *lineConfidence;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_2;
    QGraphicsView *videoView;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(615, 414);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        tabs = new QTabWidget(centralwidget);
        tabs->setObjectName(QString::fromUtf8("tabs"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(tabs->sizePolicy().hasHeightForWidth());
        tabs->setSizePolicy(sizePolicy);
        controlsTab = new QWidget();
        controlsTab->setObjectName(QString::fromUtf8("controlsTab"));
        gridLayout_3 = new QGridLayout(controlsTab);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        videoRadio = new QRadioButton(controlsTab);
        videoRadio->setObjectName(QString::fromUtf8("videoRadio"));

        horizontalLayout_2->addWidget(videoRadio);

        videoEdit = new QLineEdit(controlsTab);
        videoEdit->setObjectName(QString::fromUtf8("videoEdit"));

        horizontalLayout_2->addWidget(videoEdit);

        browseVideoBtn = new QPushButton(controlsTab);
        browseVideoBtn->setObjectName(QString::fromUtf8("browseVideoBtn"));

        horizontalLayout_2->addWidget(browseVideoBtn);


        gridLayout_3->addLayout(horizontalLayout_2, 1, 0, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_3);

        loadBtn = new QPushButton(controlsTab);
        loadBtn->setObjectName(QString::fromUtf8("loadBtn"));

        horizontalLayout_4->addWidget(loadBtn);

        startBtn = new QPushButton(controlsTab);
        startBtn->setObjectName(QString::fromUtf8("startBtn"));

        horizontalLayout_4->addWidget(startBtn);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_2);


        gridLayout_3->addLayout(horizontalLayout_4, 3, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        cameraRadio = new QRadioButton(controlsTab);
        cameraRadio->setObjectName(QString::fromUtf8("cameraRadio"));
        cameraRadio->setChecked(true);

        horizontalLayout->addWidget(cameraRadio);

        cameraSpin = new QSpinBox(controlsTab);
        cameraSpin->setObjectName(QString::fromUtf8("cameraSpin"));

        horizontalLayout->addWidget(cameraSpin);

        label_4 = new QLabel(controlsTab);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        horizontalLayout->addWidget(label_4);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        gridLayout_3->addLayout(horizontalLayout, 0, 0, 1, 1);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        imageRadio = new QRadioButton(controlsTab);
        imageRadio->setObjectName(QString::fromUtf8("imageRadio"));

        horizontalLayout_7->addWidget(imageRadio);

        imageEdit = new QLineEdit(controlsTab);
        imageEdit->setObjectName(QString::fromUtf8("imageEdit"));

        horizontalLayout_7->addWidget(imageEdit);

        browseImageBtn = new QPushButton(controlsTab);
        browseImageBtn->setObjectName(QString::fromUtf8("browseImageBtn"));

        horizontalLayout_7->addWidget(browseImageBtn);


        gridLayout_3->addLayout(horizontalLayout_7, 2, 0, 1, 1);

        tabs->addTab(controlsTab, QString());
        settingsTab = new QWidget();
        settingsTab->setObjectName(QString::fromUtf8("settingsTab"));
        gridLayout_4 = new QGridLayout(settingsTab);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label = new QLabel(settingsTab);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_3->addWidget(label);

        pbFileEdit = new QLineEdit(settingsTab);
        pbFileEdit->setObjectName(QString::fromUtf8("pbFileEdit"));

        horizontalLayout_3->addWidget(pbFileEdit);

        pbBrowseBtn = new QPushButton(settingsTab);
        pbBrowseBtn->setObjectName(QString::fromUtf8("pbBrowseBtn"));

        horizontalLayout_3->addWidget(pbBrowseBtn);


        gridLayout_4->addLayout(horizontalLayout_3, 0, 0, 1, 1);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        label_3 = new QLabel(settingsTab);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_6->addWidget(label_3);

        classesFileEdit = new QLineEdit(settingsTab);
        classesFileEdit->setObjectName(QString::fromUtf8("classesFileEdit"));

        horizontalLayout_6->addWidget(classesFileEdit);

        classesBrowseBtn = new QPushButton(settingsTab);
        classesBrowseBtn->setObjectName(QString::fromUtf8("classesBrowseBtn"));

        horizontalLayout_6->addWidget(classesBrowseBtn);


        gridLayout_4->addLayout(horizontalLayout_6, 2, 0, 1, 1);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label_2 = new QLabel(settingsTab);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_5->addWidget(label_2);

        pbtxtFileEdit = new QLineEdit(settingsTab);
        pbtxtFileEdit->setObjectName(QString::fromUtf8("pbtxtFileEdit"));

        horizontalLayout_5->addWidget(pbtxtFileEdit);

        pbtxtBrowseBtn = new QPushButton(settingsTab);
        pbtxtBrowseBtn->setObjectName(QString::fromUtf8("pbtxtBrowseBtn"));

        horizontalLayout_5->addWidget(pbtxtBrowseBtn);


        gridLayout_4->addLayout(horizontalLayout_5, 1, 0, 1, 1);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        label_5 = new QLabel(settingsTab);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        horizontalLayout_8->addWidget(label_5);

        lineSx = new QLineEdit(settingsTab);
        lineSx->setObjectName(QString::fromUtf8("lineSx"));

        horizontalLayout_8->addWidget(lineSx);

        label_6 = new QLabel(settingsTab);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        horizontalLayout_8->addWidget(label_6);

        lineSy = new QLineEdit(settingsTab);
        lineSy->setObjectName(QString::fromUtf8("lineSy"));

        horizontalLayout_8->addWidget(lineSy);

        label_7 = new QLabel(settingsTab);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        horizontalLayout_8->addWidget(label_7);

        lineConfidence = new QLineEdit(settingsTab);
        lineConfidence->setObjectName(QString::fromUtf8("lineConfidence"));

        horizontalLayout_8->addWidget(lineConfidence);


        gridLayout_4->addLayout(horizontalLayout_8, 3, 0, 1, 1);

        tabs->addTab(settingsTab, QString());

        gridLayout->addWidget(tabs, 1, 0, 1, 1);

        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout_2 = new QGridLayout(groupBox);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        videoView = new QGraphicsView(groupBox);
        videoView->setObjectName(QString::fromUtf8("videoView"));

        gridLayout_2->addWidget(videoView, 1, 0, 1, 1);


        gridLayout->addWidget(groupBox, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        tabs->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        videoRadio->setText(QCoreApplication::translate("MainWindow", "Video", nullptr));
        browseVideoBtn->setText(QCoreApplication::translate("MainWindow", "Browse", nullptr));
        loadBtn->setText(QCoreApplication::translate("MainWindow", "Load model", nullptr));
        startBtn->setText(QCoreApplication::translate("MainWindow", "Start", nullptr));
        cameraRadio->setText(QCoreApplication::translate("MainWindow", "Camera", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "Camera Id in case of multile cameras.", nullptr));
        imageRadio->setText(QCoreApplication::translate("MainWindow", "Image", nullptr));
        browseImageBtn->setText(QCoreApplication::translate("MainWindow", "Browse", nullptr));
        tabs->setTabText(tabs->indexOf(controlsTab), QCoreApplication::translate("MainWindow", "Controls", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "TensorFlow PB File :", nullptr));
        pbFileEdit->setText(QString());
        pbBrowseBtn->setText(QCoreApplication::translate("MainWindow", "Browse", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Class Names :", nullptr));
        classesFileEdit->setText(QString());
        classesBrowseBtn->setText(QCoreApplication::translate("MainWindow", "Browse", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "TensorFlow PBTXT File :", nullptr));
        pbtxtFileEdit->setText(QString());
        pbtxtBrowseBtn->setText(QCoreApplication::translate("MainWindow", "Browse", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "Imput size x", nullptr));
        lineSx->setText(QCoreApplication::translate("MainWindow", "300", nullptr));
        lineSx->setPlaceholderText(QCoreApplication::translate("MainWindow", "300", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "Input size y", nullptr));
        lineSy->setText(QCoreApplication::translate("MainWindow", "300", nullptr));
        lineSy->setPlaceholderText(QCoreApplication::translate("MainWindow", "300", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "Confidence threshold", nullptr));
        lineConfidence->setText(QCoreApplication::translate("MainWindow", "0.5", nullptr));
        lineConfidence->setPlaceholderText(QCoreApplication::translate("MainWindow", "0.5", nullptr));
        tabs->setTabText(tabs->indexOf(settingsTab), QCoreApplication::translate("MainWindow", "Settings", nullptr));
        groupBox->setTitle(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
