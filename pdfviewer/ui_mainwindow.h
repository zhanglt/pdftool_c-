/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.13
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "include/lineedit/CustomLineEdit.h"
#include "include/slider/CustomSlider.h"
#include "qpdfview.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionOpen;
    QAction *actionQuit;
    QAction *actionAbout;
    QAction *actionAbout_Qt;
    QAction *actionZoom_In;
    QAction *actionZoom_Out;
    QAction *actionPrevious_Page;
    QAction *actionNext_Page;
    QAction *actionContinuous;
    QWidget *centralWidget;
    QGridLayout *gridLayout_2;
    QWidget *widget;
    QPdfView *pdfView;
    QTabWidget *tabWidget;
    QWidget *tab;
    QPushButton *btnSelectOutput;
    QComboBox *cBoxFont;
    QLineEdit *lineEditRotate;
    QLabel *labInput;
    CustomLineEdit *lineEditColor;
    CustomLineEdit *lineEditOutput;
    QLabel *labOutput_4;
    QLineEdit *lineEditOpacity;
    QLabel *labOutput_3;
    QPushButton *btnSelectInput;
    QLabel *labOutput_2;
    QLabel *label;
    QPushButton *btnColorSelect;
    QPushButton *btnAddWater;
    QLabel *label_3;
    CustomLineEdit *lineEditWaterText;
    QLabel *labWater;
    QLabel *labOutput;
    CustomLineEdit *lineEditInput;
    CustomSlider *sliderOpacity;
    CustomSlider *sliderRotate;
    QPushButton *btnExportPDF;
    QComboBox *cBoxResolution;
    QWidget *tab_transform;
    CustomLineEdit *lineEditImageFile;
    QLabel *label_i2p;
    QPushButton *btnSelectImageFile;
    QPushButton *btnTransform;
    QLabel *label_i2p_2;
    QLabel *label_i2p_3;
    QPushButton *btnTransformBat;
    QPushButton *btnSelectImageDir;
    QLabel *label_i2p_4;
    CustomLineEdit *lineEditImageDir;
    QTextEdit *textEditLog;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuHelp;
    QMenu *menuView;
    QToolBar *mainToolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->setWindowModality(Qt::NonModal);
        MainWindow->resize(818, 631);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setStyleSheet(QString::fromUtf8("background-color: rgb(243, 243, 243);"));
        MainWindow->setUnifiedTitleAndToolBarOnMac(true);
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/images/fileopen.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpen->setIcon(icon);
        actionQuit = new QAction(MainWindow);
        actionQuit->setObjectName(QString::fromUtf8("actionQuit"));
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        actionAbout_Qt = new QAction(MainWindow);
        actionAbout_Qt->setObjectName(QString::fromUtf8("actionAbout_Qt"));
        actionZoom_In = new QAction(MainWindow);
        actionZoom_In->setObjectName(QString::fromUtf8("actionZoom_In"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/images/zoom-in-24.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionZoom_In->setIcon(icon1);
        actionZoom_Out = new QAction(MainWindow);
        actionZoom_Out->setObjectName(QString::fromUtf8("actionZoom_Out"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icons/images/zoom-out-24.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionZoom_Out->setIcon(icon2);
        actionPrevious_Page = new QAction(MainWindow);
        actionPrevious_Page->setObjectName(QString::fromUtf8("actionPrevious_Page"));
        actionNext_Page = new QAction(MainWindow);
        actionNext_Page->setObjectName(QString::fromUtf8("actionNext_Page"));
        actionContinuous = new QAction(MainWindow);
        actionContinuous->setObjectName(QString::fromUtf8("actionContinuous"));
        actionContinuous->setCheckable(true);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout_2 = new QGridLayout(centralWidget);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        widget = new QWidget(centralWidget);
        widget->setObjectName(QString::fromUtf8("widget"));
        pdfView = new QPdfView(widget);
        pdfView->setObjectName(QString::fromUtf8("pdfView"));
        pdfView->setGeometry(QRect(453, 9, 361, 508));
        pdfView->setAutoFillBackground(false);
        pdfView->setStyleSheet(QString::fromUtf8(""));
        tabWidget = new QTabWidget(widget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(0, 0, 451, 519));
        tabWidget->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        tab->setStyleSheet(QString::fromUtf8(""));
        btnSelectOutput = new QPushButton(tab);
        btnSelectOutput->setObjectName(QString::fromUtf8("btnSelectOutput"));
        btnSelectOutput->setGeometry(QRect(370, 110, 61, 31));
        btnSelectOutput->setStyleSheet(QString::fromUtf8(""));
        cBoxFont = new QComboBox(tab);
        cBoxFont->addItem(QString());
        cBoxFont->addItem(QString());
        cBoxFont->addItem(QString());
        cBoxFont->addItem(QString());
        cBoxFont->setObjectName(QString::fromUtf8("cBoxFont"));
        cBoxFont->setGeometry(QRect(184, 156, 71, 25));
        cBoxFont->setStyleSheet(QString::fromUtf8("color: rgb(107, 107, 107);\n"
"font: 11pt \"\347\255\211\347\272\277\";"));
        lineEditRotate = new QLineEdit(tab);
        lineEditRotate->setObjectName(QString::fromUtf8("lineEditRotate"));
        lineEditRotate->setEnabled(false);
        lineEditRotate->setGeometry(QRect(370, 200, 61, 20));
        lineEditRotate->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
        lineEditRotate->setReadOnly(true);
        labInput = new QLabel(tab);
        labInput->setObjectName(QString::fromUtf8("labInput"));
        labInput->setGeometry(QRect(12, 62, 81, 20));
        QFont font;
        font.setFamily(QString::fromUtf8("\347\255\211\347\272\277"));
        font.setPointSize(12);
        font.setBold(false);
        font.setItalic(false);
        labInput->setFont(font);
        labInput->setStyleSheet(QString::fromUtf8("\n"
"font: 12pt \"\347\255\211\347\272\277\";"));
        lineEditColor = new CustomLineEdit(tab);
        lineEditColor->setObjectName(QString::fromUtf8("lineEditColor"));
        lineEditColor->setGeometry(QRect(309, 156, 61, 25));
        lineEditColor->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
        lineEditColor->setReadOnly(true);
        lineEditOutput = new CustomLineEdit(tab);
        lineEditOutput->setObjectName(QString::fromUtf8("lineEditOutput"));
        lineEditOutput->setGeometry(QRect(90, 110, 281, 31));
        lineEditOutput->setStyleSheet(QString::fromUtf8("color: rgb(103, 103, 103);"));
        labOutput_4 = new QLabel(tab);
        labOutput_4->setObjectName(QString::fromUtf8("labOutput_4"));
        labOutput_4->setGeometry(QRect(9, 240, 81, 30));
        labOutput_4->setFont(font);
        labOutput_4->setStyleSheet(QString::fromUtf8("\n"
"font: 12pt \"\347\255\211\347\272\277\";"));
        lineEditOpacity = new QLineEdit(tab);
        lineEditOpacity->setObjectName(QString::fromUtf8("lineEditOpacity"));
        lineEditOpacity->setEnabled(false);
        lineEditOpacity->setGeometry(QRect(370, 242, 61, 20));
        lineEditOpacity->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
        lineEditOpacity->setReadOnly(true);
        labOutput_3 = new QLabel(tab);
        labOutput_3->setObjectName(QString::fromUtf8("labOutput_3"));
        labOutput_3->setGeometry(QRect(9, 190, 81, 30));
        labOutput_3->setFont(font);
        labOutput_3->setStyleSheet(QString::fromUtf8("\n"
"font: 12pt \"\347\255\211\347\272\277\";"));
        btnSelectInput = new QPushButton(tab);
        btnSelectInput->setObjectName(QString::fromUtf8("btnSelectInput"));
        btnSelectInput->setGeometry(QRect(370, 60, 61, 31));
        btnSelectInput->setStyleSheet(QString::fromUtf8(""));
        labOutput_2 = new QLabel(tab);
        labOutput_2->setObjectName(QString::fromUtf8("labOutput_2"));
        labOutput_2->setGeometry(QRect(9, 150, 81, 30));
        labOutput_2->setFont(font);
        labOutput_2->setStyleSheet(QString::fromUtf8("\n"
"font: 12pt \"\347\255\211\347\272\277\";"));
        label = new QLabel(tab);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(142, 156, 41, 22));
        label->setStyleSheet(QString::fromUtf8("color: rgb(121, 121, 121);\n"
"font: 12pt \"\347\255\211\347\272\277\";"));
        btnColorSelect = new QPushButton(tab);
        btnColorSelect->setObjectName(QString::fromUtf8("btnColorSelect"));
        btnColorSelect->setGeometry(QRect(370, 156, 61, 25));
        btnColorSelect->setStyleSheet(QString::fromUtf8(""));
        btnAddWater = new QPushButton(tab);
        btnAddWater->setObjectName(QString::fromUtf8("btnAddWater"));
        btnAddWater->setGeometry(QRect(60, 320, 101, 31));
        btnAddWater->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"background-color: rgb(237, 237, 237);\n"
"}\n"
"QPushButton:hover{\n"
"background:qlineargradient(spread:pad,x1:0,y1:0,x2:0,y2:1,stop:0 #e8e8e8,stop:1 #e8e8e8);\n"
"}\n"
"QPushButton:pressed{\n"
"background:qlineargradient(spread:pad,x1:0,y1:0,x2:0,y2:1,stop:0 #e0e0e0,stop:1 #e0e0e0);\n"
"}"));
        label_3 = new QLabel(tab);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(262, 157, 41, 25));
        label_3->setStyleSheet(QString::fromUtf8("color: rgb(121, 121, 121);\n"
"font: 12pt \"\347\255\211\347\272\277\";"));
        lineEditWaterText = new CustomLineEdit(tab);
        lineEditWaterText->setObjectName(QString::fromUtf8("lineEditWaterText"));
        lineEditWaterText->setGeometry(QRect(90, 12, 341, 31));
        lineEditWaterText->setStyleSheet(QString::fromUtf8("color: rgb(103, 103, 103);"));
        labWater = new QLabel(tab);
        labWater->setObjectName(QString::fromUtf8("labWater"));
        labWater->setGeometry(QRect(12, 12, 81, 30));
        labWater->setFont(font);
        labWater->setStyleSheet(QString::fromUtf8("\n"
"font: 12pt \"\347\255\211\347\272\277\";"));
        labOutput = new QLabel(tab);
        labOutput->setObjectName(QString::fromUtf8("labOutput"));
        labOutput->setGeometry(QRect(12, 109, 81, 30));
        labOutput->setFont(font);
        labOutput->setStyleSheet(QString::fromUtf8("\n"
"font: 12pt \"\347\255\211\347\272\277\";"));
        lineEditInput = new CustomLineEdit(tab);
        lineEditInput->setObjectName(QString::fromUtf8("lineEditInput"));
        lineEditInput->setGeometry(QRect(90, 60, 281, 31));
        lineEditInput->setStyleSheet(QString::fromUtf8("color: rgb(103, 103, 103);"));
        sliderOpacity = new CustomSlider(tab);
        sliderOpacity->setObjectName(QString::fromUtf8("sliderOpacity"));
        sliderOpacity->setGeometry(QRect(90, 242, 271, 22));
        sliderOpacity->setSingleStep(0);
        sliderOpacity->setValue(20);
        sliderOpacity->setSliderPosition(20);
        sliderOpacity->setOrientation(Qt::Horizontal);
        sliderRotate = new CustomSlider(tab);
        sliderRotate->setObjectName(QString::fromUtf8("sliderRotate"));
        sliderRotate->setGeometry(QRect(90, 200, 271, 22));
        sliderRotate->setMinimum(-180);
        sliderRotate->setMaximum(180);
        sliderRotate->setSingleStep(0);
        sliderRotate->setValue(45);
        sliderRotate->setSliderPosition(45);
        sliderRotate->setOrientation(Qt::Horizontal);
        btnExportPDF = new QPushButton(tab);
        btnExportPDF->setObjectName(QString::fromUtf8("btnExportPDF"));
        btnExportPDF->setGeometry(QRect(240, 320, 101, 31));
        btnExportPDF->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"background-color: rgb(237, 237, 237);\n"
"}\n"
"QPushButton:hover{\n"
"background:qlineargradient(spread:pad,x1:0,y1:0,x2:0,y2:1,stop:0 #e8e8e8,stop:1 #e8e8e8);\n"
"}\n"
"QPushButton:pressed{\n"
"background:qlineargradient(spread:pad,x1:0,y1:0,x2:0,y2:1,stop:0 #e0e0e0,stop:1 #e0e0e0);\n"
"}"));
        cBoxResolution = new QComboBox(tab);
        cBoxResolution->addItem(QString());
        cBoxResolution->addItem(QString());
        cBoxResolution->addItem(QString());
        cBoxResolution->addItem(QString());
        cBoxResolution->addItem(QString());
        cBoxResolution->addItem(QString());
        cBoxResolution->setObjectName(QString::fromUtf8("cBoxResolution"));
        cBoxResolution->setGeometry(QRect(90, 156, 51, 25));
        cBoxResolution->setStyleSheet(QString::fromUtf8("color: rgb(107, 107, 107);\n"
"font: 11pt \"\347\255\211\347\272\277\";"));
        tabWidget->addTab(tab, QString());
        labOutput->raise();
        labWater->raise();
        btnSelectOutput->raise();
        lineEditRotate->raise();
        labInput->raise();
        lineEditOutput->raise();
        labOutput_4->raise();
        lineEditOpacity->raise();
        labOutput_3->raise();
        btnSelectInput->raise();
        labOutput_2->raise();
        label->raise();
        btnColorSelect->raise();
        btnAddWater->raise();
        label_3->raise();
        lineEditWaterText->raise();
        lineEditInput->raise();
        sliderOpacity->raise();
        sliderRotate->raise();
        btnExportPDF->raise();
        cBoxResolution->raise();
        lineEditColor->raise();
        cBoxFont->raise();
        tab_transform = new QWidget();
        tab_transform->setObjectName(QString::fromUtf8("tab_transform"));
        lineEditImageFile = new CustomLineEdit(tab_transform);
        lineEditImageFile->setObjectName(QString::fromUtf8("lineEditImageFile"));
        lineEditImageFile->setGeometry(QRect(80, 40, 211, 31));
        label_i2p = new QLabel(tab_transform);
        label_i2p->setObjectName(QString::fromUtf8("label_i2p"));
        label_i2p->setGeometry(QRect(10, 40, 71, 16));
        btnSelectImageFile = new QPushButton(tab_transform);
        btnSelectImageFile->setObjectName(QString::fromUtf8("btnSelectImageFile"));
        btnSelectImageFile->setGeometry(QRect(291, 40, 75, 31));
        btnTransform = new QPushButton(tab_transform);
        btnTransform->setObjectName(QString::fromUtf8("btnTransform"));
        btnTransform->setGeometry(QRect(370, 40, 75, 31));
        label_i2p_2 = new QLabel(tab_transform);
        label_i2p_2->setObjectName(QString::fromUtf8("label_i2p_2"));
        label_i2p_2->setGeometry(QRect(10, 10, 71, 16));
        label_i2p_3 = new QLabel(tab_transform);
        label_i2p_3->setObjectName(QString::fromUtf8("label_i2p_3"));
        label_i2p_3->setGeometry(QRect(10, 80, 271, 16));
        btnTransformBat = new QPushButton(tab_transform);
        btnTransformBat->setObjectName(QString::fromUtf8("btnTransformBat"));
        btnTransformBat->setGeometry(QRect(370, 110, 75, 31));
        btnSelectImageDir = new QPushButton(tab_transform);
        btnSelectImageDir->setObjectName(QString::fromUtf8("btnSelectImageDir"));
        btnSelectImageDir->setGeometry(QRect(291, 110, 75, 31));
        label_i2p_4 = new QLabel(tab_transform);
        label_i2p_4->setObjectName(QString::fromUtf8("label_i2p_4"));
        label_i2p_4->setGeometry(QRect(10, 120, 71, 16));
        lineEditImageDir = new CustomLineEdit(tab_transform);
        lineEditImageDir->setObjectName(QString::fromUtf8("lineEditImageDir"));
        lineEditImageDir->setGeometry(QRect(80, 110, 211, 31));
        tabWidget->addTab(tab_transform, QString());
        label_i2p->raise();
        lineEditImageFile->raise();
        btnSelectImageFile->raise();
        btnTransform->raise();
        label_i2p_2->raise();
        label_i2p_3->raise();
        btnTransformBat->raise();
        btnSelectImageDir->raise();
        label_i2p_4->raise();
        lineEditImageDir->raise();
        textEditLog = new QTextEdit(widget);
        textEditLog->setObjectName(QString::fromUtf8("textEditLog"));
        textEditLog->setGeometry(QRect(0, 520, 815, 51));
        textEditLog->setStyleSheet(QString::fromUtf8("border-color: rgb(255, 255, 255);"));

        gridLayout_2->addWidget(widget, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 818, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        menuView = new QMenu(menuBar);
        menuView->setObjectName(QString::fromUtf8("menuView"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        mainToolBar->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        mainToolBar->setMovable(false);
        mainToolBar->setFloatable(false);
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuView->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionQuit);
        menuHelp->addAction(actionAbout);
        menuHelp->addAction(actionAbout_Qt);
        menuView->addAction(actionZoom_In);
        menuView->addAction(actionZoom_Out);
        menuView->addAction(actionPrevious_Page);
        menuView->addAction(actionNext_Page);
        menuView->addSeparator();
        menuView->addAction(actionContinuous);
        mainToolBar->addAction(actionOpen);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionZoom_Out);
        mainToolBar->addAction(actionZoom_In);
        mainToolBar->addSeparator();

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "PDF Viewer", nullptr));
        actionOpen->setText(QCoreApplication::translate("MainWindow", "\346\211\223\345\274\200...", nullptr));
#if QT_CONFIG(tooltip)
        actionOpen->setToolTip(QCoreApplication::translate("MainWindow", "\346\211\223\345\274\200\346\226\207\344\273\266", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(shortcut)
        actionOpen->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+O", nullptr));
#endif // QT_CONFIG(shortcut)
        actionQuit->setText(QCoreApplication::translate("MainWindow", "\351\200\200\345\207\272", nullptr));
#if QT_CONFIG(tooltip)
        actionQuit->setToolTip(QCoreApplication::translate("MainWindow", "\345\205\263\351\227\255\345\272\224\347\224\250", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(shortcut)
        actionQuit->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Q", nullptr));
#endif // QT_CONFIG(shortcut)
        actionAbout->setText(QCoreApplication::translate("MainWindow", "\345\205\263\344\272\216...", nullptr));
        actionAbout_Qt->setText(QCoreApplication::translate("MainWindow", "\345\205\263\344\272\216 Qt", nullptr));
#if QT_CONFIG(tooltip)
        actionAbout_Qt->setToolTip(QCoreApplication::translate("MainWindow", "\345\205\263\344\272\216 Qt", nullptr));
#endif // QT_CONFIG(tooltip)
        actionZoom_In->setText(QCoreApplication::translate("MainWindow", "\346\224\276\345\244\247", nullptr));
#if QT_CONFIG(shortcut)
        actionZoom_In->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+=", nullptr));
#endif // QT_CONFIG(shortcut)
        actionZoom_Out->setText(QCoreApplication::translate("MainWindow", "\347\274\251\345\260\217", nullptr));
#if QT_CONFIG(tooltip)
        actionZoom_Out->setToolTip(QCoreApplication::translate("MainWindow", "\347\274\251\345\260\217", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(shortcut)
        actionZoom_Out->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+-", nullptr));
#endif // QT_CONFIG(shortcut)
        actionPrevious_Page->setText(QCoreApplication::translate("MainWindow", "\344\270\212\344\270\200\351\241\265", nullptr));
#if QT_CONFIG(tooltip)
        actionPrevious_Page->setToolTip(QCoreApplication::translate("MainWindow", "\344\270\212\344\270\200\351\241\265", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(shortcut)
        actionPrevious_Page->setShortcut(QCoreApplication::translate("MainWindow", "PgUp", nullptr));
#endif // QT_CONFIG(shortcut)
        actionNext_Page->setText(QCoreApplication::translate("MainWindow", "\344\270\213\344\270\200\351\241\265", nullptr));
#if QT_CONFIG(tooltip)
        actionNext_Page->setToolTip(QCoreApplication::translate("MainWindow", "\344\270\213\344\270\200\351\241\265", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(shortcut)
        actionNext_Page->setShortcut(QCoreApplication::translate("MainWindow", "PgDown", nullptr));
#endif // QT_CONFIG(shortcut)
        actionContinuous->setText(QCoreApplication::translate("MainWindow", "\346\273\232\345\212\250\346\230\276\347\244\272", nullptr));
#if QT_CONFIG(tooltip)
        actionContinuous->setToolTip(QCoreApplication::translate("MainWindow", "\346\273\232\345\212\250\346\230\276\347\244\272", nullptr));
#endif // QT_CONFIG(tooltip)
        btnSelectOutput->setText(QCoreApplication::translate("MainWindow", "\351\200\211\346\213\251\347\233\256\345\275\225", nullptr));
        cBoxFont->setItemText(0, QCoreApplication::translate("MainWindow", "\346\226\260\345\256\213\344\275\223", nullptr));
        cBoxFont->setItemText(1, QCoreApplication::translate("MainWindow", "\346\245\267\344\275\223", nullptr));
        cBoxFont->setItemText(2, QCoreApplication::translate("MainWindow", "\344\273\277\345\256\213", nullptr));
        cBoxFont->setItemText(3, QCoreApplication::translate("MainWindow", "\351\273\221\344\275\223", nullptr));

        lineEditRotate->setText(QCoreApplication::translate("MainWindow", "45", nullptr));
        labInput->setText(QCoreApplication::translate("MainWindow", "\346\226\207\344\273\266\350\276\223\345\205\245\357\274\232", nullptr));
#if QT_CONFIG(tooltip)
        lineEditColor->setToolTip(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" color:#0055ff;\">\350\256\276\347\275\256\346\260\264\345\215\260\346\226\207\346\234\254\347\232\204\351\242\234\350\211\262</span></p></body></html>", nullptr));
#endif // QT_CONFIG(tooltip)
        lineEditColor->setText(QCoreApplication::translate("MainWindow", "#000000", nullptr));
        lineEditOutput->setText(QString());
        lineEditOutput->setPlaceholderText(QCoreApplication::translate("MainWindow", "\345\217\257\344\273\245\346\213\226\345\212\250\347\233\256\345\275\225\345\210\260\350\257\245\346\226\207\346\234\254\346\241\206\344\270\255", nullptr));
        labOutput_4->setText(QCoreApplication::translate("MainWindow", "\344\270\215\351\200\217\346\230\216\345\272\246\357\274\232", nullptr));
        lineEditOpacity->setText(QCoreApplication::translate("MainWindow", "20", nullptr));
        labOutput_3->setText(QCoreApplication::translate("MainWindow", "\346\226\207\346\234\254\346\227\213\350\275\254\357\274\232", nullptr));
        btnSelectInput->setText(QCoreApplication::translate("MainWindow", "\351\200\211\346\213\251\347\233\256\345\275\225", nullptr));
        labOutput_2->setText(QCoreApplication::translate("MainWindow", "\345\210\206 \350\276\250 \347\216\207\357\274\232", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "\345\255\227\344\275\223:", nullptr));
        btnColorSelect->setText(QCoreApplication::translate("MainWindow", "...", nullptr));
#if QT_CONFIG(tooltip)
        btnAddWater->setToolTip(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" color:#0055ff;\">\344\270\272\346\272\220PDF\346\226\207\344\273\266\345\242\236\345\212\240\346\260\264\345\215\260\346\226\207\346\234\254</span></p></body></html>", nullptr));
#endif // QT_CONFIG(tooltip)
        btnAddWater->setText(QCoreApplication::translate("MainWindow", "\345\242\236\345\212\240\346\260\264\345\215\260", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "\351\242\234\350\211\262:", nullptr));
        lineEditWaterText->setText(QCoreApplication::translate("MainWindow", "\350\201\224\351\200\232\346\225\260\345\255\227\347\247\221\346\212\200\346\234\211\351\231\220\345\205\254\345\217\270\346\200\273\351\203\250xx\351\241\271\347\233\256\344\270\223\347\224\250\346\226\207\346\241\243", nullptr));
        labWater->setText(QCoreApplication::translate("MainWindow", "\346\260\264\345\215\260\346\226\207\346\234\254\357\274\232", nullptr));
        labOutput->setText(QCoreApplication::translate("MainWindow", "\350\276\223\345\207\272\347\233\256\345\275\225\357\274\232", nullptr));
        lineEditInput->setText(QString());
        lineEditInput->setPlaceholderText(QCoreApplication::translate("MainWindow", "\345\217\257\344\273\245\346\213\226\345\212\250\347\233\256\345\275\225\345\210\260\350\257\245\346\226\207\346\234\254\346\241\206\344\270\255", nullptr));
#if QT_CONFIG(tooltip)
        btnExportPDF->setToolTip(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:10pt; color:#0055ff;\">\345\260\206\345\242\236\345\212\240\350\277\207\346\260\264\345\215\260\347\232\204PDF\345\257\274\345\207\272\344\270\272\345\233\276\347\211\207PDF</span></p></body></html>", nullptr));
#endif // QT_CONFIG(tooltip)
        btnExportPDF->setText(QCoreApplication::translate("MainWindow", "\345\257\274\345\207\272PDF", nullptr));
        cBoxResolution->setItemText(0, QCoreApplication::translate("MainWindow", "100", nullptr));
        cBoxResolution->setItemText(1, QCoreApplication::translate("MainWindow", "300", nullptr));
        cBoxResolution->setItemText(2, QCoreApplication::translate("MainWindow", "150", nullptr));
        cBoxResolution->setItemText(3, QCoreApplication::translate("MainWindow", "120", nullptr));
        cBoxResolution->setItemText(4, QCoreApplication::translate("MainWindow", "90", nullptr));
        cBoxResolution->setItemText(5, QCoreApplication::translate("MainWindow", "72", nullptr));

#if QT_CONFIG(tooltip)
        cBoxResolution->setToolTip(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" color:#0055ff;\">\345\275\261\345\223\215\345\257\274\345\207\272pdf\347\232\204\346\270\205\346\231\260\345\272\246\357\274\214 \346\225\260\345\200\274\350\266\212\351\253\230\346\270\205\346\231\260\345\272\246\350\266\212\351\253\230\357\274\214\345\257\274\345\207\272\346\226\207\344\273\266\350\266\212\345\244\247\343\200\202</span></p></body></html>", nullptr));
#endif // QT_CONFIG(tooltip)
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("MainWindow", "\346\260\264\345\215\260\346\223\215\344\275\234", nullptr));
        label_i2p->setText(QCoreApplication::translate("MainWindow", "\345\233\276\347\211\207\346\226\207\344\273\266\357\274\232", nullptr));
        btnSelectImageFile->setText(QCoreApplication::translate("MainWindow", "\351\200\211\346\213\251", nullptr));
        btnTransform->setText(QCoreApplication::translate("MainWindow", "\350\275\254\346\215\242", nullptr));
        label_i2p_2->setText(QCoreApplication::translate("MainWindow", "\345\233\276\347\211\207\350\275\254PDF:", nullptr));
        label_i2p_3->setText(QCoreApplication::translate("MainWindow", "\345\233\276\347\211\207\346\211\271\351\207\217\350\275\254PDF:", nullptr));
        btnTransformBat->setText(QCoreApplication::translate("MainWindow", "\350\275\254\346\215\242", nullptr));
        btnSelectImageDir->setText(QCoreApplication::translate("MainWindow", "\351\200\211\346\213\251", nullptr));
        label_i2p_4->setText(QCoreApplication::translate("MainWindow", "\345\233\276\347\211\207\347\233\256\345\275\225\357\274\232", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_transform), QCoreApplication::translate("MainWindow", "\350\275\254\346\215\242\345\267\245\345\205\267", nullptr));
        menuFile->setTitle(QCoreApplication::translate("MainWindow", "\346\226\207\344\273\266", nullptr));
        menuHelp->setTitle(QCoreApplication::translate("MainWindow", "\345\270\256\345\212\251", nullptr));
        menuView->setTitle(QCoreApplication::translate("MainWindow", "\350\247\206\345\233\276", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
