#include <iostream>

#include "qpainter.h"
#pragma execution_character_set("utf-8")

#include <windows.h>

#include <QDebug>
#include <QFileDialog>
#include <QFutureWatcher>
#include <QMessageBox>
#include <QPdfBookmarkModel>
#include <QPdfDocument>
#include <QPdfPageNavigation>
#include <QProgressDialog>
#include <QStandardItemModel>
#include <QTableWidget>
#include <QtConcurrent/QtConcurrent>
#include <QtMath>
#include <QtSvg/QSvgGenerator>

#include "QStandardItem"
#include "function.h"
#include "include/QProgressIndicator.h"
#include "include/mark/mark.h"
#include "mainwindow.h"
#include "pageselector.h"
#include "qColordialog.h"
#include "ui_mainwindow.h"
#include "zoomselector.h"
QWidget *horizontalLayoutWidget;
QHBoxLayout *horizontalLayout;
const qreal zoomMultiplier = qSqrt(2.0);

Q_LOGGING_CATEGORY(lcExample, "qt.examples.pdfviewer")

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)  // 使用父类构造函数初始化
      ,
      m_zoomSelector(new ZoomSelector(this))  // 创建 Ui::MainWindow 实例
      ,
      ui(new Ui::MainWindow)  // 创建 ZoomSelector 实例并设置父对象
      ,
      m_pageSelector(
          new PageSelector(this))  // 创建 PageSelector 实例并设置父对象
      ,
      m_document(new QPdfDocument(this))  // 创建 QPdfDocument 实例并设置父对象
      ,
      m_title(new QLabel(this)) {
  ui->setupUi(this);  // 使用 Ui::MainWindow 实例的 setupUi 函数初始化界面

  m_zoomSelector->setMaximumWidth(
      150);  // 设置 ZoomSelector 控件的最大宽度为 150
  ui->mainToolBar->insertWidget(
      ui->actionZoom_In, m_zoomSelector);  // 在主工具栏中插入 ZoomSelector 控件

  m_pageSelector->setMaximumWidth(
      150);  // 设置 PageSelector 控件的最大宽度为 150
  ui->mainToolBar->addWidget(
      m_pageSelector);  // 在主工具栏中添加 PageSelector 控件
  ui->mainToolBar->addSeparator();  //分割线
  QWidget *spacer = new QWidget();
  spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
  ui->mainToolBar->addWidget(spacer);

  m_title->setText("泛生态业务线支撑工具-仅限内部使用");
  m_title->setStyleSheet(
      QString::fromUtf8("color: rgb(0, 160, 235);font: 14pt ;"));
  ui->mainToolBar->addWidget(m_title);

  // 设置 PageSelector 控件的页面导航功能为 pdfView 控件的页面导航功能
  m_pageSelector->setPageNavigation(ui->pdfView->pageNavigation());

  // 将 ZoomSelector 控件的信号与 pdfView 控件的相应槽函数连接起来
  connect(m_zoomSelector, &ZoomSelector::zoomModeChanged, ui->pdfView,
          &QPdfView::setZoomMode);
  connect(m_zoomSelector, &ZoomSelector::zoomFactorChanged, ui->pdfView,
          &QPdfView::setZoomFactor);
  m_zoomSelector->reset();  // 调用 ZoomSelector 控件的 reset 函数

  // 创建 QPdfBookmarkModel 实例并设置父对象
  // QPdfBookmarkModel *bookmarkModel = new QPdfBookmarkModel(this);
  // bookmarkModel->setDocument(m_document);  // 设置 QPdfBookmarkModel
  // 实例的文档
  // 设置 bookmarkView 控件的模型为 QPdfBookmarkModel 实例
  // ui->bookmarkView->setModel(bookmarkModel);
  // 将 bookmarkView 控件的 activated 信号连接到当前对象的 bookmarkSelected
  // 槽函数
  // connect(ui->bookmarkView, SIGNAL(activated(QModelIndex)), this,
  // SLOT(bookmarkSelected(QModelIndex)));

  ui->tabWidget->setTabEnabled(1, true);

  ui->pdfView->setDocument(
      m_document);  // 设置 pdfView 控件的文档为 QPdfDocument 实例

  // 将 pdfView 控件的 zoomFactorChanged 信号与 ZoomSelector
  // 控件的相应槽函数连接起来
  connect(ui->pdfView, &QPdfView::zoomFactorChanged, m_zoomSelector,
          &ZoomSelector::setZoomFactor);

  qRegisterMetaType<wMap>("wMap");

  // 设置线程池的大小
  threadPool.setMaxThreadCount(10);

  // 当lineEditWaterText文本变动且失去焦点时候出发viewWatermark()函数用来更新文档预览
  connect(ui->lineEditWaterText, &CustomTextEdit::dataChanged, this,
          &MainWindow::viewWatermark);
  // 拖动滑块改变数值，后同步更新预览
  connect(ui->sliderOpacity, &QSlider::sliderReleased, this,
          &MainWindow::viewWatermark);
  connect(ui->sliderRotate, &QSlider::sliderReleased, this,
          &MainWindow::viewWatermark);

  connect(ui->sliderFontsize, &QSlider::sliderReleased, this,
          &MainWindow::viewWatermark);

  // 更新slider后同步更新
  // 数值显示框，这个功能可以合并到MainWindow::viewWatermark中？
  connect(ui->sliderRotate, &QSlider::valueChanged, this, [=]() {
    ui->lineEditRotate->setText(QString::number(ui->sliderRotate->value()));
  });
  connect(ui->sliderOpacity, &QSlider::valueChanged, this, [=]() {
    ui->lineEditOpacity->setText(QString::number(ui->sliderOpacity->value()));
  });
  connect(ui->sliderFontsize, &QSlider::valueChanged, this, [=]() {
    ui->lineEdit_fs->setText(QString::number(ui->sliderFontsize->value()));
  });
  // connect(m_slider, &QSlider::valueChanged, this, &MyWidget::valueChanged);
  // 检测输出目录lineedit ，保证其值必须是一个正确的文件目录
  connect(ui->lineEditOutput, &CustomLineEdit::dataChanged, this, [=]() {
    QFileInfo dir(ui->lineEditOutput->text());
    if (!dir.isDir()) {
      ui->lineEditOutput->setText("");
      QMessageBox::information(nullptr, "警告！",
                               "选择、拖入或者输入一个存在的正确目录");
    }
  });

  connect(ui->tableWidgetMerge, &mytable::selectRow, this, [=]() {
    QString file = ui->tableWidgetMerge->getCurrFile();
    if (!file.isEmpty()) {
      this->open(QUrl::fromLocalFile(file));
    }
  });

  //以下设置QPdfView的背景色。
  QPalette palette = ui->pdfView->palette();
  QColor color(199, 199, 199);
  palette.setBrush(QPalette::Dark, color);
  ui->pdfView->setPalette(palette);
  ui->btnExportPDF->setEnabled(false);
  initTable();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::open(const QUrl &docLocation) {
  if (docLocation.isLocalFile()) {
    m_document->load(docLocation.toLocalFile());
    const auto documentTitle =
        m_document->metaData(QPdfDocument::Title).toString();
    setWindowTitle(!documentTitle.isEmpty() ? documentTitle : "PDF浏览器");
  } else {
    // qCDebug(lcExample) << docLocation << "is not a valid local file";
    QMessageBox::critical(this, tr("文件打开错误"),
                          tr("%1 无效的本地文件").arg(docLocation.toString()));
  }
  // qCDebug(lcExample) << docLocation;
}

void MainWindow::bookmarkSelected(const QModelIndex &index) {
  if (!index.isValid()) return;

  const int page = index.data(QPdfBookmarkModel::PageNumberRole).toInt();
  ui->pdfView->pageNavigation()->setCurrentPage(page);
}

void MainWindow::on_actionOpen_triggered() {
  QUrl toOpen = QFileDialog::getOpenFileUrl(
      this, tr("选择打开PDF文件"),
      QUrl("file:///" + ui->lineEditOutput->text()), "文档 (*.pdf)");

  if (toOpen.isValid()) open(toOpen);
  emit m_zoomSelector->zoomModeChanged(QPdfView::FitToWidth);
}

void MainWindow::on_actionQuit_triggered() { QApplication::quit(); }

void MainWindow::on_actionAbout_triggered() {
  QMessageBox::about(this, tr("关于pdf处理"),
                     tr("数科-泛生态业务线工具-仅限内部使用"));
}

void MainWindow::on_actionAbout_Qt_triggered() { QMessageBox::aboutQt(this); }

void MainWindow::on_actionZoom_In_triggered() {
  ui->pdfView->setZoomFactor(ui->pdfView->zoomFactor() * zoomMultiplier);
}

void MainWindow::on_actionZoom_Out_triggered() {
  ui->pdfView->setZoomFactor(ui->pdfView->zoomFactor() / zoomMultiplier);
}

void MainWindow::on_actionPrevious_Page_triggered() {
  ui->pdfView->pageNavigation()->goToPreviousPage();
}

void MainWindow::on_actionNext_Page_triggered() {
  ui->pdfView->pageNavigation()->goToNextPage();
}

void MainWindow::on_actionContinuous_triggered() {
  ui->pdfView->setPageMode(ui->actionContinuous->isChecked()
                               ? QPdfView::MultiPage
                               : QPdfView::SinglePage);
}

// 选择颜色
void MainWindow::on_btnColorSelect_clicked() {
  QColorDialog dialog(Qt::red, this);                // 创建对象
  dialog.setOption(QColorDialog::ShowAlphaChannel);  // 显示alpha选项
  dialog.exec();                         // 以模态方式运行对话框
  QColor color = dialog.currentColor();  // 获取当前颜色
  if (color.isValid()) {
    // 获取16进制颜色码的红、绿、蓝通道值
    int m_red = color.red();
    int m_blue = color.blue();
    int m_green = color.green();

    // 构建RGB颜色代码文本
    QString rStr = QString("%1").arg(m_red & 0xFF, 2, 16, QChar('0'));
    QString bstr = QString("%1").arg(m_blue & 0xFF, 2, 16, QChar('0'));
    QString gStr = QString("%1").arg(m_green & 0xFF, 2, 16, QChar('0'));
    ui->lineEditColor->setText("#" + rStr + gStr + bstr);
    ui->lineEditColor->setStyleSheet("color:" + ui->lineEditColor->text());

    viewWatermark();
  }
}

// 选择输入目录
void MainWindow::on_btnSelectInput_clicked() {
  QString directory = QFileDialog::getExistingDirectory(
      nullptr, "选择输入目录", "",
      QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

  // 如果用户选择了目录，则输出所选目录的路径
  if (!directory.isEmpty()) {
    qDebug() << "选择的目录：" << directory;
    ui->lineEditInput->setText(directory);
    if (ui->lineEditOutput->text().isEmpty()) {
      ui->lineEditOutput->setText(directory);
    }
  }
}
// 选择输出目录
void MainWindow::on_btnSelectOutput_clicked() {
  QString directory = QFileDialog::getExistingDirectory(
      nullptr, "选择输出目录", "",
      QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

  // 如果用户选择了目录，则输出所选目录的路径
  if (!directory.isEmpty()) {
    // qDebug() << "选择的目录：" << directory;
    ui->lineEditOutput->setText(directory);
  }
}

// 增加水印
void MainWindow::on_btnAddWater_clicked() {
  QString text = ui->lineEditWaterText->toPlainText();
  if (text.isEmpty()) {
    QMessageBox::information(nullptr, "提示", "水印文本不能为空");
    return;
  }
  QString inputDir = ui->lineEditInput->text();
  QFileInfo dir(inputDir);
  if (inputDir.isEmpty() || !dir.isDir()) {
    QMessageBox::information(nullptr, "提示", "输入目录或者选择正确目录");
    ui->lineEditInput->setFocus();
    ui->lineEditInput->setText("");
    return;
  }
  QString outputDir = ui->lineEditOutput->text();
  QFileInfo output(outputDir);
  if (outputDir.isEmpty() || !output.isDir()) {
    QMessageBox::information(nullptr, "提示", "输入目录或者选择正确目录");
    ui->lineEditOutput->setFocus();
    ui->lineEditOutput->setText("");
    return;
  }
  QString color = ui->lineEditColor->text();
  QString opacity = ui->lineEditOpacity->text();
  QString rotate = ui->lineEditRotate->text();
  int fontIndex = ui->cBoxFont->currentIndex();
  QString fontsize = ui->lineEdit_fs->text();
  QString font;
  switch (fontIndex) {
    case 0:
      font = "NSimSun";
      break;
    case 1:
      font = "simkai";
      break;
    case 2:
      font = "simfang";
      break;
    case 3:
      font = "simhei";
      break;
    default:
      font = "simkai";
  }
  QElapsedTimer time;
  time.start();
  // 多行水印文本处理
  if (ui->lineEditWaterText->document()->blockCount() > 1) {
    if (dir.isFile() && dir.suffix().toLower() == "pdf") {  //文件处理
      // addWatermark(inputDir, outputDir + "/_out_/" + dir.fileName(), text,
      //            opacity + "%", color, "-" + rotate, font);
      addWatermark_multiline(
          inputDir.toStdString(), outputDir.toStdString() + "/_out_/", text,
          font, fontsize.toInt(), color, rotate.toDouble(), opacity.toDouble());

    } else {
      //目录处理
      addWatermarkSingle(text, inputDir, outputDir, color, opacity, rotate,
                         ui->cBoxFont->currentText(), fontsize);
    }

  } else {
    //单行水印文本处理
    if (dir.isFile() && dir.suffix().toLower() == "pdf") {  //文件处理
      addWatermark(inputDir, outputDir + "/_out_/" + dir.fileName(), text,
                   opacity + "%", color, "-" + rotate, font);
    } else {
      //目录出路
      addWatermarkSingle(text, inputDir, outputDir, color, opacity,
                         "-" + rotate, font, fontsize);
    }
  }

  // qDebug() << "总共用时:" << time.elapsed() << "毫秒";
  ui->textEditLog->append("增加水印用时:" + QString::number(time.elapsed()) +
                          "毫秒,结果文件保存在:" + ui->lineEditOutput->text() +
                          "/_out_/");
  ui->btnExportPDF->setEnabled(true);
}

/*
 * 用于参数修改后的交过预览
 **/
void MainWindow::viewWatermark() {
  QElapsedTimer time;
  time.start();
  QString text = ui->lineEditWaterText->toPlainText();
  if (text.isEmpty()) {
    QMessageBox::information(nullptr, "提示", "水印文本不能为空");
    return;
  }
  /*
  QString inputDir = ui->lineEditInput->text();
  if (inputDir.isEmpty()) {
    QMessageBox::information(nullptr, "提示", "输入目录不能为空");
    return;
  }
  QString outputDir = ui->lineEditOutput->text();
  if (outputDir.isEmpty()) {
    QMessageBox::information(nullptr, "提示", "输出目录不能为空");
    return;
  }
*/
  QString color = ui->lineEditColor->text();
  QString opacity = ui->lineEditOpacity->text() + "%";
  QString rotate = ui->lineEditRotate->text();
  int fontIndex = ui->cBoxFont->currentIndex();
  // qDebug() << "fontindex:" << fontIndex;
  QString font;

  switch (fontIndex) {
    case 0:
      font = "NSimSun";
      break;
    case 1:
      font = "simkai";
      break;
    case 2:
      font = "simfang";
      break;
    case 3:
      font = "simhei";
      break;
    default:
      font = "simkai";
  }
  /*
      qDebug()<<"text"<<text;
      qDebug()<<"opacity"<<opacity;
      qDebug()<<"color"<<color;
      qDebug()<<"rotate"<<rotate;
      qDebug()<<"font"<<font;
  */

  // 多行文本处理
  if (ui->lineEditWaterText->document()->blockCount() > 1) {
    addWatermark_multiline(
        "doc/1.pdf", "doc/2.pdf", ui->lineEditWaterText->toPlainText(),
        ui->cBoxFont->currentText(), ui->lineEdit_fs->text().toInt(),
        ui->lineEditColor->text(), ui->lineEditRotate->text().toDouble(),
        ui->lineEditOpacity->text().toDouble() / 100);
    //由于多行文本用create_annotation处理，在预览中不显示，所以做转换处理
    pdf2image("doc/2.pdf", "doc/", 72);
    image2pdf("doc/0.png", "doc/2.pdf");
  } else {
    //单上文本处理
    addWatermark("doc/1.pdf", "doc/2.pdf", text, opacity, color, "-" + rotate,
                 font);
  }

  this->open(QUrl::fromLocalFile("doc/2.pdf"));
  emit this->m_zoomSelector->zoomModeChanged(QPdfView::FitToWidth);
}
/**
 * 增加水印功能函数
 **/

void MainWindow::addWatermarkSingle(QString text, QString inputDir,
                                    QString outputDir, QString color,
                                    QString opacity, QString rotate,
                                    QString font, QString fontSize) {
  QDir dir(inputDir);
  QStringList files;
  traverseDirectory(dir, files, "pdf", "_out_");
  QMutex m_mutex;
  wMap map;
  if (files.length() == 0) {
    QMessageBox::information(nullptr, "提示！", "输入目录中文件为空");
  } else {
    for (const auto &file : files) {
      // qDebug() << "file:" << file;
      QString outfile =
          pathChange(inputDir, outputDir, file, "_out_").replace("//", "/");

      if (ui->lineEditWaterText->document()->blockCount() > 1) {
        mwmThreadSinge = new multiWatermarkThreadSingle(&m_mutex, &map);
        mwmThreadSinge->setText(text);
        mwmThreadSinge->setFont(font);
        mwmThreadSinge->setColor(color);
        mwmThreadSinge->setRotate(rotate);
        mwmThreadSinge->setOpacity(opacity);
        mwmThreadSinge->setFontsize(fontSize);
        mwmThreadSinge->setInputFilename(file);
        mwmThreadSinge->setOutputFilename(outfile);
        threadPool.start(mwmThreadSinge);
      } else {
        wmThreadSinge = new watermarkThreadSingle(&m_mutex, &map);
        /*
        connect(wmThreadSinge, &watermarkThreadSingle::addFinish, this,
                [=](wMap map) {

                              QMap<QString, int>::const_iterator i;
                              for (i = map.constBegin(); i != map.constEnd();
                     ++i)
                              {
                                  qDebug() << i.key() << ":" << i.value();
                              }
                  // qDebug() << "-------:"<< map.size();
                });*/
        wmThreadSinge->setText(text);
        wmThreadSinge->setFont(font);
        wmThreadSinge->setColor(color);
        wmThreadSinge->setRotate(rotate);
        wmThreadSinge->setOpacity(opacity);
        wmThreadSinge->setFontsize(fontSize);
        wmThreadSinge->setInputFilename(file);
        wmThreadSinge->setOutputFilename(outfile);
        threadPool.start(wmThreadSinge);
      }
    }

    threadPool.waitForDone();
    QMessageBox::information(nullptr, "提示！",
                             "增加水印完成！<br>共" +
                                 QString::number(files.length()) +
                                 "个文件参与处理，<br>实际处理文件" +
                                 QString::number(map.size()) + "个");
  }
}

/**
 * 导出pdf按钮操作
 **/

void MainWindow::on_btnExportPDF_clicked() {
  QString outputDir = ui->lineEditOutput->text();
  QFileInfo output(outputDir);
  if (!output.isDir()) {
    ui->lineEditOutput->setFocus();
    QMessageBox::information(nullptr, "提示", "输入目录或者选择正确目录");
    return;
  }
  QElapsedTimer time;
  time.start();
  QDir dir(ui->lineEditOutput->text() + "/_out_/");
  exportPdf(dir);
  // qDebug() << "导出图片用时:" << time.elapsed() << "毫秒";
  ui->textEditLog->append("导出图片用时:" + QString::number(time.elapsed()) +
                          "毫秒,结果文件保存在:" + ui->lineEditOutput->text() +
                          "/_pdf_/");
}

/**
 *本函数是将文字pdf转为图片pdf
 *将源目录pdfdir及子目录中的pdf文件，导出为png图片，然后再将png转换为pdf文件
 *操作过程通过Qthreadpool线程池来启用多线程处理（很恶心...）
 **/
void MainWindow::exportPdf(QDir pdfdir) {
  ui->btnAddWater->setEnabled(false);
  QStringList files;
  traverseDirectory(pdfdir, files, "pdf", "_pdf_");
  if (files.length() < 1) {
    QMessageBox::information(
        nullptr, "系统提示！",
        "目标目录：" + ui->lineEditOutput->text() + "/_out_ 中没有文件");
  }
  for (QString &file : files) {
    pdf2imageThread = new pdf2imageThreadSingle();
    //为每个线程连接一个信号，用于监测线程池的线程工作完成情况，建议Qthreadpoll增加一个finish信号。
    connect(pdf2imageThread, &pdf2imageThreadSingle::addFinish, this, [=]() {
      // 判断线程池中的活动线程数，如果为0则认为所有工作线程结束。
      if (threadPool.activeThreadCount() == 0) {
        //将增加水印按钮设置为可用状态
        emit this->Finished();
        ui->btnAddWater->setEnabled(true);
        QMessageBox::information(nullptr, "导出PDF完成！",
                                 "导出PDF完成！<br>共" +
                                     QString::number(files.length()) +
                                     "个文件参与处理");
        //获取图片根目录，转换工作完成后删除之
        QDir imageRootDir(pdfdir.path().replace("_out_", "_image_"));
        // 删除整个目录及其子目录
        bool success = imageRootDir.removeRecursively();
        // 检查删除操作是否成功
        if (success) {
          ui->textEditLog->append("图片目录：" + imageRootDir.path() +
                                  "删除成功.");
        } else {
          ui->textEditLog->append("图片目录：" + imageRootDir.path() +
                                  "删除失败");
        }
      }
    });
    pdf2imageThread->setSourceFile(file);
    //通过pdf文件目录，生成图片目录，并创建。
    QString imagePath = file.replace("_out_", "_image_");
    createDirectory(imagePath);
    pdf2imageThread->setImagePath(imagePath);
    //通过图片文件目录生成目标PDF文件及保存目录，并创建目录
    QFileInfo fileInfo(imagePath.replace("_image_", "_pdf_"));
    QString path = fileInfo.absolutePath();
    QString filename = fileInfo.fileName();
    createDirectory(path);
    pdf2imageThread->setTargetFile(path + "/" + filename);
    pdf2imageThread->setIs2pdf(true);
    pdf2imageThread->setResolution(ui->cBoxResolution->currentText().toInt());
    threadPool.start(pdf2imageThread);
    // threadPool.waitForDone();
  }
  if (files.length() > 0) {
    qprogresssindicat();
  }
}

void MainWindow::on_cBoxFont_currentIndexChanged(int index) { viewWatermark(); }

// 选择图片文件
void MainWindow::on_btnSelectImageFile_clicked() {
  QString filePath = QFileDialog::getOpenFileName(
      nullptr, "选择图像文件", QDir::homePath(),
      "图像文件 (*.png *.PNG *.jpg *.JPG *.bmp *.BMP)");
  if (!filePath.isEmpty()) {
    ui->lineEditImageFile->setText(filePath);
  }
}

void MainWindow::on_btnTransform_clicked() {
  QString file = ui->lineEditImageFile->text();
  if (!isImageByExtension(file) || !isImageByMagicNumber(file) || file == "") {
    QMessageBox::information(nullptr, "提示信息！", "请选择正确的图像文件");
    ui->lineEditImageFile->setText("");
    return;
  }
  QFileInfo info(ui->lineEditImageFile->text());
  if (image2pdf(info.filePath().toStdString(),
                (info.filePath() + ".pdf").toStdString()) == 0) {
    ui->textEditLog->append("文件保存在：" + info.filePath() + ".pdf\n");
    this->open(QUrl::fromLocalFile(info.filePath() + ".pdf"));
    emit this->m_zoomSelector->zoomModeChanged(QPdfView::FitToWidth);
    ui->textEditLog->append("PDF转换完成保存在：" + info.filePath() + ".pdf");
    QMessageBox::information(nullptr, "PDF转换完成！",
                             "文件保存在：" + info.filePath() + ".pdf\n");
  }
}

// 选择图片目录
void MainWindow::on_btnSelectImageDir_clicked() {
  QString directory = QFileDialog::getExistingDirectory(
      nullptr, "选择图片目录", "",
      QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
  if (!directory.isEmpty()) {
    // qDebug() << "选择的目录：" << directory;
    ui->lineEditImageDir->setText(directory);
  }
}

// 批量转换
void MainWindow::on_btnTransformBat_clicked() {
  if (ui->lineEditImageDir->text() == "") {
    QMessageBox::information(nullptr, "提示信息！", "请选择图像目录");
    return;
  }
  QFileInfo file(ui->lineEditImageDir->text());
  if (!file.isDir()) {
    QMessageBox::information(nullptr, "提示信息！", "请选择正确的图像目录");
    ui->lineEditImageDir->setText("");
    return;
  }
  QDir dir(ui->lineEditImageDir->text());
  QStringList files;
  tDirectory(dir, files, {"png", "jpg", "bmp"});
  for (QString &file : files) {
    image2pdf(file.toStdString(), (file + ".pdf").toStdString());
  }
  QMessageBox::information(nullptr, "PDF转换完成！", "PDF保存在图片源目录中");
  ui->textEditLog->append("PDF保存源目录中:" + ui->lineEditImageDir->text());
}

// pdf转图片--选择pdf文件
void MainWindow::on_btnSelectPDFFile_clicked() {
  QString filePath = QFileDialog::getOpenFileName(
      nullptr, "选择PDF文件", QDir::homePath(), "PDF文件 (*.pdf *.PDF)");
  if (!filePath.isEmpty()) {
    qDebug() << "选择的目录：" << filePath;
    ui->lineEditPdfFile->setText(filePath);
  }

  this->open(QUrl::fromLocalFile(filePath));
}
// pdf转图片操作
void MainWindow::on_btnPdfToImage_clicked() {
  QString file = ui->lineEditPdfFile->text();
  if (file == "") {
    QMessageBox::information(nullptr, "提示信息！", "请选择PDF文件");
    return;
  }
  if (!isPDFByExtension(file) || !isPDFByMagicNumber(file)) {
    QMessageBox::information(nullptr, "提示信息！", "请选择正确的PDF文件");
    ui->lineEditPdfFile->setText("");
    return;
  }
  QString path = file.left(file.length() - 4) + "_YXNkZmRzZmFzZGZhZHNmYWRzZgo";
  pdf2imageThread = new pdf2imageThreadSingle();
  pdf2imageThread->setSourceFile(file);
  pdf2imageThread->setIs2pdf(false);
  //通过pdf文件目录，生成图片目录，并创建。
  qDebug() << "file:" << path;
  if (!createDirectory(path)) {
  }
  pdf2imageThread->setImagePath(path);
  pdf2imageThread->setResolution(100);
  //为每个线程连接一个信号，用于监测线程池的线程工作完成情况，建议Qthreadpoll增加一个finish信号。
  connect(pdf2imageThread, &pdf2imageThreadSingle::addFinish, this, [=]() {
    // 判断线程池中的活动线程数，如果为0则认为所有工作线程结束。
    if (threadPool.activeThreadCount() == 0) {
      //将增加水印按钮设置为可用状态
      emit this->Finished();
      ui->textEditLog->append("图片保存目录:" + path);
      QMessageBox::information(nullptr, "导出图片完成！", "导出图片完成！");
    }
  });

  threadPool.start(pdf2imageThread);
  qprogresssindicat();
}
// 长时间操作动画延时函数
void MainWindow::qprogresssindicat() {
  QProgressIndicator *pIndicator = nullptr;
  QDialog dialog;
  QObject::connect(this, &MainWindow::Finished, &dialog, &QDialog::close);
  //转换开始 增加水印按钮设置为不可用
  dialog.setWindowTitle("文件转换处理...");
  dialog.resize(200, 20);
  // /dialog.setWindowFlags(Qt::CustomizeWindowHint |
  // Qt::WindowCloseButtonHint);
  dialog.setWindowFlags(dialog.windowFlags() | Qt::FramelessWindowHint);
  pIndicator = new QProgressIndicator(&dialog);
  QVBoxLayout *hLayout = new QVBoxLayout(&dialog);  // 水平布局
  QLabel *l1 = new QLabel(
      "正在转换。。。。"
      ""
      "");
  hLayout->setMargin(1);           // 与窗体边无距离 尽量占满
  hLayout->addWidget(pIndicator);  // 加入控件
  hLayout->addWidget(l1);
  hLayout->setAlignment(pIndicator, Qt::AlignCenter);  // 控件居中
  hLayout->setAlignment(l1, Qt::AlignCenter);          // 控件居中
  // ui->tab_2->setLayout(hLayout);
  dialog.setLayout(hLayout);

  pIndicator->setColor(QColor(12, 52, 255));
  pIndicator->startAnimation();  //启动动画
  dialog.exec();
}
// 选择拆分pdf文件
void MainWindow::on_btnSelectFilesplit_clicked() {
  QString fileName = QFileDialog::getOpenFileName(
      nullptr, "选择PDF文件", QDir::homePath(), "PDF文件 (*.pdf *.PDF)");
  if (!fileName.isEmpty()) {
    ui->lineEditInputFilesplit->setText(fileName);
  }
  if (fileName.length() > 0) {
    this->open(QUrl::fromLocalFile(fileName));

    ui->lineEditSplitpages->setText(
        QString::number(getPages(fileName.toStdString())));
    ui->lineEditSplitEnd->setText(
        QString::number(getPages(fileName.toStdString())));
    QFileInfo path(fileName);
    ui->lineEditSplitOutput->setText(path.absolutePath());
  }
}
// 拆分pdf的文件检查
void MainWindow::on_lineEditInputFilesplit_textChanged(
    const QString &filename) {
  // 判断是否为pdf文件。
  QFileInfo fileInfo(filename);
  if ((fileInfo.isFile()) && (filename.contains(".pdf"))) {
    this->open(QUrl::fromLocalFile(filename));
    ui->lineEditSplitpages->setText(
        QString::number(getPages(filename.toStdString())));
    ui->lineEditSplitEnd->setText(
        QString::number(getPages(filename.toStdString())));
    QFileInfo path(filename);
    ui->lineEditSplitOutput->setText(path.absolutePath());

  } else {
    if (!filename.isEmpty()) {
      QMessageBox::information(nullptr, "警告！", "请选择正确的pdf文件");
    }
    ui->lineEditInputFilesplit->setText("");
    return;
  }
}
// 选择拆分目录
void MainWindow::on_btnSelectSplitDir_clicked() {
  QString directory = QFileDialog::getExistingDirectory(
      nullptr, "选择输出目录", "",
      QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

  // 如果用户选择了目录，则输出所选目录的路径
  if (!directory.isEmpty()) {
    // qDebug() << "选择的目录：" << directory;
    ui->lineEditSplitOutput->setText(directory);
  }
}
// 拆分目录进行有效性检查
void MainWindow::on_lineEditSplitOutput_textChanged(const QString &filepath) {
  QFileInfo path(filepath);
  if (!path.isDir()) {
    if (!filepath.isEmpty()) {
      QMessageBox::information(nullptr, "警告！", "请选择正确的目录");
    }
    ui->lineEditSplitOutput->setText("");
    return;
  }
}

//文件拆分操作
void MainWindow::on_btnSplitPdf_clicked() {
  if ((ui->lineEditInputFilesplit->text() == "") ||
      (ui->lineEditSplitOutput->text() == "")) {
    QMessageBox::information(nullptr, "警告！", "请选择输入文件、输出目录");
    return;
  }
  // 文件页数
  int pages = ui->lineEditSplitpages->text().toInt();
  string in = ui->lineEditInputFilesplit->text().toStdString();
  QFileInfo file(ui->lineEditInputFilesplit->text());
  string filename =
      file.fileName().left(file.fileName().length() - 4).toStdString();
  string out = ui->lineEditSplitOutput->text().toStdString() + "/" + filename;

  //通过radiobutton来选择两种拆分方式
  if (ui->radioButtonSpliterange->isChecked()) {
    //第一种拆分方式：从原始文件中提取区间页数为新文件
    QString range, sub_range;
    QStringList parts, sub_parts;
    int start = 0, end = 0;
    range = ui->lineEditSplitscope->text();
    // 通过正则表达式来验证拆分规则的有效性
    QRegularExpression regex("^\\d+-\\d+(,\\d+-\\d+)*$");
    QRegularExpressionMatch match = regex.match(range);
    if (match.hasMatch()) {
      if (regex.match(range).hasMatch()) {
        parts = range.split(",");
        for (int i = 0; i < parts.size(); ++i) {
          sub_range = parts.at(i);
          sub_parts = sub_range.split("-");
          start = sub_parts[0].toInt() - 1;
          end = sub_parts[1].toInt();
          qDebug() << "start:" << start;
          qDebug() << "end:" << end;
          if (end > pages) {
            QMessageBox::information(nullptr, "警告！",
                                     "拆分范围不能大于文档总页数");
            return;
          }
          if (start > end) {
            QMessageBox::information(nullptr, "警告！",
                                     "拆分范围开始值不能大于结束值");
            return;
          }
        }
      }
    } else {
      QMessageBox::information(nullptr, "警告！",
                               "目前只支持n-n,n-n...格式的拆分");
      return;
    }

    if (regex.match(range).hasMatch()) {
      parts = range.split(",");
      for (int i = 0; i < parts.size(); ++i) {
        sub_range = parts.at(i);
        sub_parts = sub_range.split("-");
        start = sub_parts[0].toInt() - 1;
        end = sub_parts[1].toInt();
        string sub_out = out + "_" + sub_parts[0].toStdString() + "-" +
                         sub_parts[1].toStdString();
        splitPdf(in, sub_out, start, end);
      }
    }

    QMessageBox::information(
        nullptr, "PDF拆分完成！",
        "文件保存在保存：" + ui->lineEditSplitOutput->text());

    ui->textEditLog->append("拆分PDF保存目录中:" +
                            ui->lineEditSplitOutput->text());

  } else {
    /*第二种拆分方式：
    先按照拆分范围生成一个临时文件保存在临时文件夹
    再将临时文件按照规则拆分成多个文件
    */
    int start = ui->lineEditSplitStart->text().toInt() - 1;
    int end = ui->lineEditSplitEnd->text().toInt();
    if ((end > pages) || (start > pages)) {
      QMessageBox::information(nullptr, "警告！", "拆分范围不能大于文档总页数");
      return;
    }
    if (start > end) {
      QMessageBox::information(nullptr, "警告！",
                               "拆分范围开始值不能大于结束值");
      return;
    }
    QString tempDir =
        QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    // 第一次拆分生成中间结果文件
    splitPdf(in, tempDir.toStdString() + "/" + filename, start, end);
    // 每个拆分子文件的页数
    int subPages = ui->lineEditSubPages->text().toInt();
    // 第二次拆分
    splitPdf(tempDir.toStdString() + "/" + filename + "_split.pdf", out,
             subPages);
    QFile tempfile(tempDir + "/" + QString::fromStdString(filename) +
                   "_split.pdf");
    tempfile.remove();
    QMessageBox::information(
        nullptr, "PDF拆分完成！",
        "文件保存在保存：" + ui->lineEditSplitOutput->text());

    ui->textEditLog->append("拆分PDF保存目录中:" +
                            ui->lineEditSplitOutput->text());
  }
}

// 初始化文件合并表格
void MainWindow::initTable() {
  // 设置表头
  QStringList headerLabels;
  headerLabels << "文件名称"
               << "页数"
               << "合并范围"
               << "文件操作";
  ui->tableWidgetMerge->setHorizontalHeaderLabels(headerLabels);
  ui->tableWidgetMerge->setColumnWidth(0, 210);
  ui->tableWidgetMerge->setColumnWidth(1, 40);
  ui->tableWidgetMerge->setColumnWidth(2, 85);
  ui->tableWidgetMerge->setColumnWidth(3, 80);
}

// 增加并文件列表
void MainWindow::on_btnAddFile_clicked() {
  QString fileName = QFileDialog::getOpenFileName(
      nullptr, "选择PDF文件", QDir::homePath(), "PDF文件 (*.pdf *.PDF )");
  if (fileName.isEmpty()) {
    QMessageBox::information(nullptr, "提示信息！", "文件名为空");
    return;
  }
  ui->tableWidgetMerge->AddFile(fileName);
}

// 选择输出文件目录
void MainWindow::on_btnSelectMergeDir_clicked() {
  QString directory = QFileDialog::getExistingDirectory(
      nullptr, "选择输出目录", "",
      QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

  // 如果用户选择了目录，则输出所选目录的路径
  if (!directory.isEmpty()) {
    // qDebug() << "选择的目录：" << directory;
    ui->lineEditOutMerge->setText(directory);
  }
}

// PDF合并操作
void MainWindow::on_btnMerge_clicked() {
  // 临时文件列表
  std::list<std::string> fileList;
  QString outDir = ui->lineEditOutMerge->text();
  QString outfileName = ui->lineEditMergeOutFile->text();
  // 合并前的临时文件目录
  QString tempFile = outDir + "/" + "YXNkZmRzZmFzZGZhZHNmYWRzZgo";
  if (outfileName == "") {
    QMessageBox::information(nullptr, "提示信息！", "请输入合并后的文件名");
    ui->lineEditMergeOutFile->setFocus();
    return;
  }
  if (outDir == "") {
    QMessageBox::information(nullptr, "提示信息！", "合并后的输出目录");
    ui->lineEditOutMerge->setFocus();
    return;
  }

  if (outfileName.right(4).toUpper() != ".PDF") {
    outfileName = outfileName + ".pdf";
  }
  // 创建临时目录
  if (!createDirectory(tempFile)) {
    QMessageBox::information(nullptr, "提示信息！", "请选择正确的输出目录");
    ui->lineEditOutMerge->setFocus();
    return;
  }

  QString infilename;
  for (int row = 0; row < ui->tableWidgetMerge->rowCount(); ++row) {
    // 从第一列中获取原始文件名
    QTableWidgetItem *item = ui->tableWidgetMerge->item(row, 0);
    if (item) {
      infilename = item->text();
    } else {
      qDebug() << "行:" << row << ", 列:" << 0 << ", 文本: (Empty)";
      return;
    }
    int pStart, pEnd;
    // 从第三列中获取 承载开始行 结束行的lineedit对象，并获取开始、结束页码
    QWidget *currenCell = ui->tableWidgetMerge->cellWidget(row, 2);
    QList<QLineEdit *> currLine = currenCell->findChildren<QLineEdit *>();
    pStart = currLine[0]->text().toInt();
    pEnd = currLine[1]->text().toInt();
    // 按行拆到临时文件
    if (splitPdf(infilename.toStdString(),
                 tempFile.toStdString() + "/" + std::to_string(row), pStart - 1,
                 pEnd) == 0) {
      //临时文件放到列表中
      fileList.push_back(tempFile.toStdString() + "/" + std::to_string(row) +
                         "_split.pdf");
    }
  }
  outfileName = outDir + "/" + outfileName;
  // 将临时文件列表中的文件进行合并
  if (mergePdf(fileList, outfileName.toStdString()) == 0) {
    QFileInfo info(outfileName);
    // 在预览中打开合并完成的文件
    this->open(QUrl::fromLocalFile(outfileName));
    emit this->m_zoomSelector->zoomModeChanged(QPdfView::FitToWidth);
    ui->textEditLog->append("PDF合并完成保存在：" + info.filePath());
    QMessageBox::information(nullptr, "PDF合并完成！",
                             "文件保存在：" + info.filePath() + "\n");
    QDir temp(tempFile);
    // 删除整个目录及其子目录
    temp.removeRecursively();
  }
}

void MainWindow::on_lineEditInputFilesplit_editingFinished() {}
