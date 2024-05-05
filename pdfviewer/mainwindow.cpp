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
    : QMainWindow(parent)  // ʹ�ø��๹�캯����ʼ��
      ,
      m_zoomSelector(new ZoomSelector(this))  // ���� Ui::MainWindow ʵ��
      ,
      ui(new Ui::MainWindow)  // ���� ZoomSelector ʵ�������ø�����
      ,
      m_pageSelector(
          new PageSelector(this))  // ���� PageSelector ʵ�������ø�����
      ,
      m_document(new QPdfDocument(this))  // ���� QPdfDocument ʵ�������ø�����
      ,
      m_title(new QLabel(this)) {
  ui->setupUi(this);  // ʹ�� Ui::MainWindow ʵ���� setupUi ������ʼ������

  m_zoomSelector->setMaximumWidth(
      150);  // ���� ZoomSelector �ؼ��������Ϊ 150
  ui->mainToolBar->insertWidget(
      ui->actionZoom_In, m_zoomSelector);  // �����������в��� ZoomSelector �ؼ�

  m_pageSelector->setMaximumWidth(
      150);  // ���� PageSelector �ؼ��������Ϊ 150
  ui->mainToolBar->addWidget(
      m_pageSelector);  // ��������������� PageSelector �ؼ�
  ui->mainToolBar->addSeparator();  //�ָ���
  QWidget *spacer = new QWidget();
  spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
  ui->mainToolBar->addWidget(spacer);

  m_title->setText("����̬ҵ����֧�Ź���-�����ڲ�ʹ��");
  m_title->setStyleSheet(
      QString::fromUtf8("color: rgb(0, 160, 235);font: 14pt ;"));
  ui->mainToolBar->addWidget(m_title);

  // ���� PageSelector �ؼ���ҳ�浼������Ϊ pdfView �ؼ���ҳ�浼������
  m_pageSelector->setPageNavigation(ui->pdfView->pageNavigation());

  // �� ZoomSelector �ؼ����ź��� pdfView �ؼ�����Ӧ�ۺ�����������
  connect(m_zoomSelector, &ZoomSelector::zoomModeChanged, ui->pdfView,
          &QPdfView::setZoomMode);
  connect(m_zoomSelector, &ZoomSelector::zoomFactorChanged, ui->pdfView,
          &QPdfView::setZoomFactor);
  m_zoomSelector->reset();  // ���� ZoomSelector �ؼ��� reset ����

  // ���� QPdfBookmarkModel ʵ�������ø�����
  // QPdfBookmarkModel *bookmarkModel = new QPdfBookmarkModel(this);
  // bookmarkModel->setDocument(m_document);  // ���� QPdfBookmarkModel
  // ʵ�����ĵ�
  // ���� bookmarkView �ؼ���ģ��Ϊ QPdfBookmarkModel ʵ��
  // ui->bookmarkView->setModel(bookmarkModel);
  // �� bookmarkView �ؼ��� activated �ź����ӵ���ǰ����� bookmarkSelected
  // �ۺ���
  // connect(ui->bookmarkView, SIGNAL(activated(QModelIndex)), this,
  // SLOT(bookmarkSelected(QModelIndex)));

  ui->tabWidget->setTabEnabled(1, true);

  ui->pdfView->setDocument(
      m_document);  // ���� pdfView �ؼ����ĵ�Ϊ QPdfDocument ʵ��

  // �� pdfView �ؼ��� zoomFactorChanged �ź��� ZoomSelector
  // �ؼ�����Ӧ�ۺ�����������
  connect(ui->pdfView, &QPdfView::zoomFactorChanged, m_zoomSelector,
          &ZoomSelector::setZoomFactor);

  qRegisterMetaType<wMap>("wMap");

  // �����̳߳صĴ�С
  threadPool.setMaxThreadCount(10);

  // ��lineEditWaterText�ı��䶯��ʧȥ����ʱ�����viewWatermark()�������������ĵ�Ԥ��
  connect(ui->lineEditWaterText, &CustomTextEdit::dataChanged, this,
          &MainWindow::viewWatermark);
  // �϶�����ı���ֵ����ͬ������Ԥ��
  connect(ui->sliderOpacity, &QSlider::sliderReleased, this,
          &MainWindow::viewWatermark);
  connect(ui->sliderRotate, &QSlider::sliderReleased, this,
          &MainWindow::viewWatermark);

  connect(ui->sliderFontsize, &QSlider::sliderReleased, this,
          &MainWindow::viewWatermark);

  // ����slider��ͬ������
  // ��ֵ��ʾ��������ܿ��Ժϲ���MainWindow::viewWatermark�У�
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
  // ������Ŀ¼lineedit ����֤��ֵ������һ����ȷ���ļ�Ŀ¼
  connect(ui->lineEditOutput, &CustomLineEdit::dataChanged, this, [=]() {
    QFileInfo dir(ui->lineEditOutput->text());
    if (!dir.isDir()) {
      ui->lineEditOutput->setText("");
      QMessageBox::information(nullptr, "���棡",
                               "ѡ�������������һ�����ڵ���ȷĿ¼");
    }
  });

  connect(ui->tableWidgetMerge, &mytable::selectRow, this, [=]() {
    QString file = ui->tableWidgetMerge->getCurrFile();
    if (!file.isEmpty()) {
      this->open(QUrl::fromLocalFile(file));
    }
  });

  //��������QPdfView�ı���ɫ��
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
    setWindowTitle(!documentTitle.isEmpty() ? documentTitle : "PDF�����");
  } else {
    // qCDebug(lcExample) << docLocation << "is not a valid local file";
    QMessageBox::critical(this, tr("�ļ��򿪴���"),
                          tr("%1 ��Ч�ı����ļ�").arg(docLocation.toString()));
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
      this, tr("ѡ���PDF�ļ�"),
      QUrl("file:///" + ui->lineEditOutput->text()), "�ĵ� (*.pdf)");

  if (toOpen.isValid()) open(toOpen);
  emit m_zoomSelector->zoomModeChanged(QPdfView::FitToWidth);
}

void MainWindow::on_actionQuit_triggered() { QApplication::quit(); }

void MainWindow::on_actionAbout_triggered() {
  QMessageBox::about(this, tr("����pdf����"),
                     tr("����-����̬ҵ���߹���-�����ڲ�ʹ��"));
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

// ѡ����ɫ
void MainWindow::on_btnColorSelect_clicked() {
  QColorDialog dialog(Qt::red, this);                // ��������
  dialog.setOption(QColorDialog::ShowAlphaChannel);  // ��ʾalphaѡ��
  dialog.exec();                         // ��ģ̬��ʽ���жԻ���
  QColor color = dialog.currentColor();  // ��ȡ��ǰ��ɫ
  if (color.isValid()) {
    // ��ȡ16������ɫ��ĺ졢�̡���ͨ��ֵ
    int m_red = color.red();
    int m_blue = color.blue();
    int m_green = color.green();

    // ����RGB��ɫ�����ı�
    QString rStr = QString("%1").arg(m_red & 0xFF, 2, 16, QChar('0'));
    QString bstr = QString("%1").arg(m_blue & 0xFF, 2, 16, QChar('0'));
    QString gStr = QString("%1").arg(m_green & 0xFF, 2, 16, QChar('0'));
    ui->lineEditColor->setText("#" + rStr + gStr + bstr);
    ui->lineEditColor->setStyleSheet("color:" + ui->lineEditColor->text());

    viewWatermark();
  }
}

// ѡ������Ŀ¼
void MainWindow::on_btnSelectInput_clicked() {
  QString directory = QFileDialog::getExistingDirectory(
      nullptr, "ѡ������Ŀ¼", "",
      QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

  // ����û�ѡ����Ŀ¼���������ѡĿ¼��·��
  if (!directory.isEmpty()) {
    qDebug() << "ѡ���Ŀ¼��" << directory;
    ui->lineEditInput->setText(directory);
    if (ui->lineEditOutput->text().isEmpty()) {
      ui->lineEditOutput->setText(directory);
    }
  }
}
// ѡ�����Ŀ¼
void MainWindow::on_btnSelectOutput_clicked() {
  QString directory = QFileDialog::getExistingDirectory(
      nullptr, "ѡ�����Ŀ¼", "",
      QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

  // ����û�ѡ����Ŀ¼���������ѡĿ¼��·��
  if (!directory.isEmpty()) {
    // qDebug() << "ѡ���Ŀ¼��" << directory;
    ui->lineEditOutput->setText(directory);
  }
}

// ����ˮӡ
void MainWindow::on_btnAddWater_clicked() {
  QString text = ui->lineEditWaterText->toPlainText();
  if (text.isEmpty()) {
    QMessageBox::information(nullptr, "��ʾ", "ˮӡ�ı�����Ϊ��");
    return;
  }
  QString inputDir = ui->lineEditInput->text();
  QFileInfo dir(inputDir);
  if (inputDir.isEmpty() || !dir.isDir()) {
    QMessageBox::information(nullptr, "��ʾ", "����Ŀ¼����ѡ����ȷĿ¼");
    ui->lineEditInput->setFocus();
    ui->lineEditInput->setText("");
    return;
  }
  QString outputDir = ui->lineEditOutput->text();
  QFileInfo output(outputDir);
  if (outputDir.isEmpty() || !output.isDir()) {
    QMessageBox::information(nullptr, "��ʾ", "����Ŀ¼����ѡ����ȷĿ¼");
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
  // ����ˮӡ�ı�����
  if (ui->lineEditWaterText->document()->blockCount() > 1) {
    if (dir.isFile() && dir.suffix().toLower() == "pdf") {  //�ļ�����
      // addWatermark(inputDir, outputDir + "/_out_/" + dir.fileName(), text,
      //            opacity + "%", color, "-" + rotate, font);
      addWatermark_multiline(
          inputDir.toStdString(), outputDir.toStdString() + "/_out_/", text,
          font, fontsize.toInt(), color, rotate.toDouble(), opacity.toDouble());

    } else {
      //Ŀ¼����
      addWatermarkSingle(text, inputDir, outputDir, color, opacity, rotate,
                         ui->cBoxFont->currentText(), fontsize);
    }

  } else {
    //����ˮӡ�ı�����
    if (dir.isFile() && dir.suffix().toLower() == "pdf") {  //�ļ�����
      addWatermark(inputDir, outputDir + "/_out_/" + dir.fileName(), text,
                   opacity + "%", color, "-" + rotate, font);
    } else {
      //Ŀ¼��·
      addWatermarkSingle(text, inputDir, outputDir, color, opacity,
                         "-" + rotate, font, fontsize);
    }
  }

  // qDebug() << "�ܹ���ʱ:" << time.elapsed() << "����";
  ui->textEditLog->append("����ˮӡ��ʱ:" + QString::number(time.elapsed()) +
                          "����,����ļ�������:" + ui->lineEditOutput->text() +
                          "/_out_/");
  ui->btnExportPDF->setEnabled(true);
}

/*
 * ���ڲ����޸ĺ�Ľ���Ԥ��
 **/
void MainWindow::viewWatermark() {
  QElapsedTimer time;
  time.start();
  QString text = ui->lineEditWaterText->toPlainText();
  if (text.isEmpty()) {
    QMessageBox::information(nullptr, "��ʾ", "ˮӡ�ı�����Ϊ��");
    return;
  }
  /*
  QString inputDir = ui->lineEditInput->text();
  if (inputDir.isEmpty()) {
    QMessageBox::information(nullptr, "��ʾ", "����Ŀ¼����Ϊ��");
    return;
  }
  QString outputDir = ui->lineEditOutput->text();
  if (outputDir.isEmpty()) {
    QMessageBox::information(nullptr, "��ʾ", "���Ŀ¼����Ϊ��");
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

  // �����ı�����
  if (ui->lineEditWaterText->document()->blockCount() > 1) {
    addWatermark_multiline(
        "doc/1.pdf", "doc/2.pdf", ui->lineEditWaterText->toPlainText(),
        ui->cBoxFont->currentText(), ui->lineEdit_fs->text().toInt(),
        ui->lineEditColor->text(), ui->lineEditRotate->text().toDouble(),
        ui->lineEditOpacity->text().toDouble() / 100);
    //���ڶ����ı���create_annotation������Ԥ���в���ʾ��������ת������
    pdf2image("doc/2.pdf", "doc/", 72);
    image2pdf("doc/0.png", "doc/2.pdf");
  } else {
    //�����ı�����
    addWatermark("doc/1.pdf", "doc/2.pdf", text, opacity, color, "-" + rotate,
                 font);
  }

  this->open(QUrl::fromLocalFile("doc/2.pdf"));
  emit this->m_zoomSelector->zoomModeChanged(QPdfView::FitToWidth);
}
/**
 * ����ˮӡ���ܺ���
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
    QMessageBox::information(nullptr, "��ʾ��", "����Ŀ¼���ļ�Ϊ��");
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
    QMessageBox::information(nullptr, "��ʾ��",
                             "����ˮӡ��ɣ�<br>��" +
                                 QString::number(files.length()) +
                                 "���ļ����봦��<br>ʵ�ʴ����ļ�" +
                                 QString::number(map.size()) + "��");
  }
}

/**
 * ����pdf��ť����
 **/

void MainWindow::on_btnExportPDF_clicked() {
  QString outputDir = ui->lineEditOutput->text();
  QFileInfo output(outputDir);
  if (!output.isDir()) {
    ui->lineEditOutput->setFocus();
    QMessageBox::information(nullptr, "��ʾ", "����Ŀ¼����ѡ����ȷĿ¼");
    return;
  }
  QElapsedTimer time;
  time.start();
  QDir dir(ui->lineEditOutput->text() + "/_out_/");
  exportPdf(dir);
  // qDebug() << "����ͼƬ��ʱ:" << time.elapsed() << "����";
  ui->textEditLog->append("����ͼƬ��ʱ:" + QString::number(time.elapsed()) +
                          "����,����ļ�������:" + ui->lineEditOutput->text() +
                          "/_pdf_/");
}

/**
 *�������ǽ�����pdfתΪͼƬpdf
 *��ԴĿ¼pdfdir����Ŀ¼�е�pdf�ļ�������ΪpngͼƬ��Ȼ���ٽ�pngת��Ϊpdf�ļ�
 *��������ͨ��Qthreadpool�̳߳������ö��̴߳����ܶ���...��
 **/
void MainWindow::exportPdf(QDir pdfdir) {
  ui->btnAddWater->setEnabled(false);
  QStringList files;
  traverseDirectory(pdfdir, files, "pdf", "_pdf_");
  if (files.length() < 1) {
    QMessageBox::information(
        nullptr, "ϵͳ��ʾ��",
        "Ŀ��Ŀ¼��" + ui->lineEditOutput->text() + "/_out_ ��û���ļ�");
  }
  for (QString &file : files) {
    pdf2imageThread = new pdf2imageThreadSingle();
    //Ϊÿ���߳�����һ���źţ����ڼ���̳߳ص��̹߳���������������Qthreadpoll����һ��finish�źš�
    connect(pdf2imageThread, &pdf2imageThreadSingle::addFinish, this, [=]() {
      // �ж��̳߳��еĻ�߳��������Ϊ0����Ϊ���й����߳̽�����
      if (threadPool.activeThreadCount() == 0) {
        //������ˮӡ��ť����Ϊ����״̬
        emit this->Finished();
        ui->btnAddWater->setEnabled(true);
        QMessageBox::information(nullptr, "����PDF��ɣ�",
                                 "����PDF��ɣ�<br>��" +
                                     QString::number(files.length()) +
                                     "���ļ����봦��");
        //��ȡͼƬ��Ŀ¼��ת��������ɺ�ɾ��֮
        QDir imageRootDir(pdfdir.path().replace("_out_", "_image_"));
        // ɾ������Ŀ¼������Ŀ¼
        bool success = imageRootDir.removeRecursively();
        // ���ɾ�������Ƿ�ɹ�
        if (success) {
          ui->textEditLog->append("ͼƬĿ¼��" + imageRootDir.path() +
                                  "ɾ���ɹ�.");
        } else {
          ui->textEditLog->append("ͼƬĿ¼��" + imageRootDir.path() +
                                  "ɾ��ʧ��");
        }
      }
    });
    pdf2imageThread->setSourceFile(file);
    //ͨ��pdf�ļ�Ŀ¼������ͼƬĿ¼����������
    QString imagePath = file.replace("_out_", "_image_");
    createDirectory(imagePath);
    pdf2imageThread->setImagePath(imagePath);
    //ͨ��ͼƬ�ļ�Ŀ¼����Ŀ��PDF�ļ�������Ŀ¼��������Ŀ¼
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

// ѡ��ͼƬ�ļ�
void MainWindow::on_btnSelectImageFile_clicked() {
  QString filePath = QFileDialog::getOpenFileName(
      nullptr, "ѡ��ͼ���ļ�", QDir::homePath(),
      "ͼ���ļ� (*.png *.PNG *.jpg *.JPG *.bmp *.BMP)");
  if (!filePath.isEmpty()) {
    ui->lineEditImageFile->setText(filePath);
  }
}

void MainWindow::on_btnTransform_clicked() {
  QString file = ui->lineEditImageFile->text();
  if (!isImageByExtension(file) || !isImageByMagicNumber(file) || file == "") {
    QMessageBox::information(nullptr, "��ʾ��Ϣ��", "��ѡ����ȷ��ͼ���ļ�");
    ui->lineEditImageFile->setText("");
    return;
  }
  QFileInfo info(ui->lineEditImageFile->text());
  if (image2pdf(info.filePath().toStdString(),
                (info.filePath() + ".pdf").toStdString()) == 0) {
    ui->textEditLog->append("�ļ������ڣ�" + info.filePath() + ".pdf\n");
    this->open(QUrl::fromLocalFile(info.filePath() + ".pdf"));
    emit this->m_zoomSelector->zoomModeChanged(QPdfView::FitToWidth);
    ui->textEditLog->append("PDFת����ɱ����ڣ�" + info.filePath() + ".pdf");
    QMessageBox::information(nullptr, "PDFת����ɣ�",
                             "�ļ������ڣ�" + info.filePath() + ".pdf\n");
  }
}

// ѡ��ͼƬĿ¼
void MainWindow::on_btnSelectImageDir_clicked() {
  QString directory = QFileDialog::getExistingDirectory(
      nullptr, "ѡ��ͼƬĿ¼", "",
      QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
  if (!directory.isEmpty()) {
    // qDebug() << "ѡ���Ŀ¼��" << directory;
    ui->lineEditImageDir->setText(directory);
  }
}

// ����ת��
void MainWindow::on_btnTransformBat_clicked() {
  if (ui->lineEditImageDir->text() == "") {
    QMessageBox::information(nullptr, "��ʾ��Ϣ��", "��ѡ��ͼ��Ŀ¼");
    return;
  }
  QFileInfo file(ui->lineEditImageDir->text());
  if (!file.isDir()) {
    QMessageBox::information(nullptr, "��ʾ��Ϣ��", "��ѡ����ȷ��ͼ��Ŀ¼");
    ui->lineEditImageDir->setText("");
    return;
  }
  QDir dir(ui->lineEditImageDir->text());
  QStringList files;
  tDirectory(dir, files, {"png", "jpg", "bmp"});
  for (QString &file : files) {
    image2pdf(file.toStdString(), (file + ".pdf").toStdString());
  }
  QMessageBox::information(nullptr, "PDFת����ɣ�", "PDF������ͼƬԴĿ¼��");
  ui->textEditLog->append("PDF����ԴĿ¼��:" + ui->lineEditImageDir->text());
}

// pdfתͼƬ--ѡ��pdf�ļ�
void MainWindow::on_btnSelectPDFFile_clicked() {
  QString filePath = QFileDialog::getOpenFileName(
      nullptr, "ѡ��PDF�ļ�", QDir::homePath(), "PDF�ļ� (*.pdf *.PDF)");
  if (!filePath.isEmpty()) {
    qDebug() << "ѡ���Ŀ¼��" << filePath;
    ui->lineEditPdfFile->setText(filePath);
  }

  this->open(QUrl::fromLocalFile(filePath));
}
// pdfתͼƬ����
void MainWindow::on_btnPdfToImage_clicked() {
  QString file = ui->lineEditPdfFile->text();
  if (file == "") {
    QMessageBox::information(nullptr, "��ʾ��Ϣ��", "��ѡ��PDF�ļ�");
    return;
  }
  if (!isPDFByExtension(file) || !isPDFByMagicNumber(file)) {
    QMessageBox::information(nullptr, "��ʾ��Ϣ��", "��ѡ����ȷ��PDF�ļ�");
    ui->lineEditPdfFile->setText("");
    return;
  }
  QString path = file.left(file.length() - 4) + "_YXNkZmRzZmFzZGZhZHNmYWRzZgo";
  pdf2imageThread = new pdf2imageThreadSingle();
  pdf2imageThread->setSourceFile(file);
  pdf2imageThread->setIs2pdf(false);
  //ͨ��pdf�ļ�Ŀ¼������ͼƬĿ¼����������
  qDebug() << "file:" << path;
  if (!createDirectory(path)) {
  }
  pdf2imageThread->setImagePath(path);
  pdf2imageThread->setResolution(100);
  //Ϊÿ���߳�����һ���źţ����ڼ���̳߳ص��̹߳���������������Qthreadpoll����һ��finish�źš�
  connect(pdf2imageThread, &pdf2imageThreadSingle::addFinish, this, [=]() {
    // �ж��̳߳��еĻ�߳��������Ϊ0����Ϊ���й����߳̽�����
    if (threadPool.activeThreadCount() == 0) {
      //������ˮӡ��ť����Ϊ����״̬
      emit this->Finished();
      ui->textEditLog->append("ͼƬ����Ŀ¼:" + path);
      QMessageBox::information(nullptr, "����ͼƬ��ɣ�", "����ͼƬ��ɣ�");
    }
  });

  threadPool.start(pdf2imageThread);
  qprogresssindicat();
}
// ��ʱ�����������ʱ����
void MainWindow::qprogresssindicat() {
  QProgressIndicator *pIndicator = nullptr;
  QDialog dialog;
  QObject::connect(this, &MainWindow::Finished, &dialog, &QDialog::close);
  //ת����ʼ ����ˮӡ��ť����Ϊ������
  dialog.setWindowTitle("�ļ�ת������...");
  dialog.resize(200, 20);
  // /dialog.setWindowFlags(Qt::CustomizeWindowHint |
  // Qt::WindowCloseButtonHint);
  dialog.setWindowFlags(dialog.windowFlags() | Qt::FramelessWindowHint);
  pIndicator = new QProgressIndicator(&dialog);
  QVBoxLayout *hLayout = new QVBoxLayout(&dialog);  // ˮƽ����
  QLabel *l1 = new QLabel(
      "����ת����������"
      ""
      "");
  hLayout->setMargin(1);           // �봰����޾��� ����ռ��
  hLayout->addWidget(pIndicator);  // ����ؼ�
  hLayout->addWidget(l1);
  hLayout->setAlignment(pIndicator, Qt::AlignCenter);  // �ؼ�����
  hLayout->setAlignment(l1, Qt::AlignCenter);          // �ؼ�����
  // ui->tab_2->setLayout(hLayout);
  dialog.setLayout(hLayout);

  pIndicator->setColor(QColor(12, 52, 255));
  pIndicator->startAnimation();  //��������
  dialog.exec();
}
// ѡ����pdf�ļ�
void MainWindow::on_btnSelectFilesplit_clicked() {
  QString fileName = QFileDialog::getOpenFileName(
      nullptr, "ѡ��PDF�ļ�", QDir::homePath(), "PDF�ļ� (*.pdf *.PDF)");
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
// ���pdf���ļ����
void MainWindow::on_lineEditInputFilesplit_textChanged(
    const QString &filename) {
  // �ж��Ƿ�Ϊpdf�ļ���
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
      QMessageBox::information(nullptr, "���棡", "��ѡ����ȷ��pdf�ļ�");
    }
    ui->lineEditInputFilesplit->setText("");
    return;
  }
}
// ѡ����Ŀ¼
void MainWindow::on_btnSelectSplitDir_clicked() {
  QString directory = QFileDialog::getExistingDirectory(
      nullptr, "ѡ�����Ŀ¼", "",
      QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

  // ����û�ѡ����Ŀ¼���������ѡĿ¼��·��
  if (!directory.isEmpty()) {
    // qDebug() << "ѡ���Ŀ¼��" << directory;
    ui->lineEditSplitOutput->setText(directory);
  }
}
// ���Ŀ¼������Ч�Լ��
void MainWindow::on_lineEditSplitOutput_textChanged(const QString &filepath) {
  QFileInfo path(filepath);
  if (!path.isDir()) {
    if (!filepath.isEmpty()) {
      QMessageBox::information(nullptr, "���棡", "��ѡ����ȷ��Ŀ¼");
    }
    ui->lineEditSplitOutput->setText("");
    return;
  }
}

//�ļ���ֲ���
void MainWindow::on_btnSplitPdf_clicked() {
  if ((ui->lineEditInputFilesplit->text() == "") ||
      (ui->lineEditSplitOutput->text() == "")) {
    QMessageBox::information(nullptr, "���棡", "��ѡ�������ļ������Ŀ¼");
    return;
  }
  // �ļ�ҳ��
  int pages = ui->lineEditSplitpages->text().toInt();
  string in = ui->lineEditInputFilesplit->text().toStdString();
  QFileInfo file(ui->lineEditInputFilesplit->text());
  string filename =
      file.fileName().left(file.fileName().length() - 4).toStdString();
  string out = ui->lineEditSplitOutput->text().toStdString() + "/" + filename;

  //ͨ��radiobutton��ѡ�����ֲ�ַ�ʽ
  if (ui->radioButtonSpliterange->isChecked()) {
    //��һ�ֲ�ַ�ʽ����ԭʼ�ļ�����ȡ����ҳ��Ϊ���ļ�
    QString range, sub_range;
    QStringList parts, sub_parts;
    int start = 0, end = 0;
    range = ui->lineEditSplitscope->text();
    // ͨ��������ʽ����֤��ֹ������Ч��
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
            QMessageBox::information(nullptr, "���棡",
                                     "��ַ�Χ���ܴ����ĵ���ҳ��");
            return;
          }
          if (start > end) {
            QMessageBox::information(nullptr, "���棡",
                                     "��ַ�Χ��ʼֵ���ܴ��ڽ���ֵ");
            return;
          }
        }
      }
    } else {
      QMessageBox::information(nullptr, "���棡",
                               "Ŀǰֻ֧��n-n,n-n...��ʽ�Ĳ��");
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
        nullptr, "PDF�����ɣ�",
        "�ļ������ڱ��棺" + ui->lineEditSplitOutput->text());

    ui->textEditLog->append("���PDF����Ŀ¼��:" +
                            ui->lineEditSplitOutput->text());

  } else {
    /*�ڶ��ֲ�ַ�ʽ��
    �Ȱ��ղ�ַ�Χ����һ����ʱ�ļ���������ʱ�ļ���
    �ٽ���ʱ�ļ����չ����ֳɶ���ļ�
    */
    int start = ui->lineEditSplitStart->text().toInt() - 1;
    int end = ui->lineEditSplitEnd->text().toInt();
    if ((end > pages) || (start > pages)) {
      QMessageBox::information(nullptr, "���棡", "��ַ�Χ���ܴ����ĵ���ҳ��");
      return;
    }
    if (start > end) {
      QMessageBox::information(nullptr, "���棡",
                               "��ַ�Χ��ʼֵ���ܴ��ڽ���ֵ");
      return;
    }
    QString tempDir =
        QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    // ��һ�β�������м����ļ�
    splitPdf(in, tempDir.toStdString() + "/" + filename, start, end);
    // ÿ��������ļ���ҳ��
    int subPages = ui->lineEditSubPages->text().toInt();
    // �ڶ��β��
    splitPdf(tempDir.toStdString() + "/" + filename + "_split.pdf", out,
             subPages);
    QFile tempfile(tempDir + "/" + QString::fromStdString(filename) +
                   "_split.pdf");
    tempfile.remove();
    QMessageBox::information(
        nullptr, "PDF�����ɣ�",
        "�ļ������ڱ��棺" + ui->lineEditSplitOutput->text());

    ui->textEditLog->append("���PDF����Ŀ¼��:" +
                            ui->lineEditSplitOutput->text());
  }
}

// ��ʼ���ļ��ϲ����
void MainWindow::initTable() {
  // ���ñ�ͷ
  QStringList headerLabels;
  headerLabels << "�ļ�����"
               << "ҳ��"
               << "�ϲ���Χ"
               << "�ļ�����";
  ui->tableWidgetMerge->setHorizontalHeaderLabels(headerLabels);
  ui->tableWidgetMerge->setColumnWidth(0, 210);
  ui->tableWidgetMerge->setColumnWidth(1, 40);
  ui->tableWidgetMerge->setColumnWidth(2, 85);
  ui->tableWidgetMerge->setColumnWidth(3, 80);
}

// ���Ӳ��ļ��б�
void MainWindow::on_btnAddFile_clicked() {
  QString fileName = QFileDialog::getOpenFileName(
      nullptr, "ѡ��PDF�ļ�", QDir::homePath(), "PDF�ļ� (*.pdf *.PDF )");
  if (fileName.isEmpty()) {
    QMessageBox::information(nullptr, "��ʾ��Ϣ��", "�ļ���Ϊ��");
    return;
  }
  ui->tableWidgetMerge->AddFile(fileName);
}

// ѡ������ļ�Ŀ¼
void MainWindow::on_btnSelectMergeDir_clicked() {
  QString directory = QFileDialog::getExistingDirectory(
      nullptr, "ѡ�����Ŀ¼", "",
      QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

  // ����û�ѡ����Ŀ¼���������ѡĿ¼��·��
  if (!directory.isEmpty()) {
    // qDebug() << "ѡ���Ŀ¼��" << directory;
    ui->lineEditOutMerge->setText(directory);
  }
}

// PDF�ϲ�����
void MainWindow::on_btnMerge_clicked() {
  // ��ʱ�ļ��б�
  std::list<std::string> fileList;
  QString outDir = ui->lineEditOutMerge->text();
  QString outfileName = ui->lineEditMergeOutFile->text();
  // �ϲ�ǰ����ʱ�ļ�Ŀ¼
  QString tempFile = outDir + "/" + "YXNkZmRzZmFzZGZhZHNmYWRzZgo";
  if (outfileName == "") {
    QMessageBox::information(nullptr, "��ʾ��Ϣ��", "������ϲ�����ļ���");
    ui->lineEditMergeOutFile->setFocus();
    return;
  }
  if (outDir == "") {
    QMessageBox::information(nullptr, "��ʾ��Ϣ��", "�ϲ�������Ŀ¼");
    ui->lineEditOutMerge->setFocus();
    return;
  }

  if (outfileName.right(4).toUpper() != ".PDF") {
    outfileName = outfileName + ".pdf";
  }
  // ������ʱĿ¼
  if (!createDirectory(tempFile)) {
    QMessageBox::information(nullptr, "��ʾ��Ϣ��", "��ѡ����ȷ�����Ŀ¼");
    ui->lineEditOutMerge->setFocus();
    return;
  }

  QString infilename;
  for (int row = 0; row < ui->tableWidgetMerge->rowCount(); ++row) {
    // �ӵ�һ���л�ȡԭʼ�ļ���
    QTableWidgetItem *item = ui->tableWidgetMerge->item(row, 0);
    if (item) {
      infilename = item->text();
    } else {
      qDebug() << "��:" << row << ", ��:" << 0 << ", �ı�: (Empty)";
      return;
    }
    int pStart, pEnd;
    // �ӵ������л�ȡ ���ؿ�ʼ�� �����е�lineedit���󣬲���ȡ��ʼ������ҳ��
    QWidget *currenCell = ui->tableWidgetMerge->cellWidget(row, 2);
    QList<QLineEdit *> currLine = currenCell->findChildren<QLineEdit *>();
    pStart = currLine[0]->text().toInt();
    pEnd = currLine[1]->text().toInt();
    // ���в���ʱ�ļ�
    if (splitPdf(infilename.toStdString(),
                 tempFile.toStdString() + "/" + std::to_string(row), pStart - 1,
                 pEnd) == 0) {
      //��ʱ�ļ��ŵ��б���
      fileList.push_back(tempFile.toStdString() + "/" + std::to_string(row) +
                         "_split.pdf");
    }
  }
  outfileName = outDir + "/" + outfileName;
  // ����ʱ�ļ��б��е��ļ����кϲ�
  if (mergePdf(fileList, outfileName.toStdString()) == 0) {
    QFileInfo info(outfileName);
    // ��Ԥ���д򿪺ϲ���ɵ��ļ�
    this->open(QUrl::fromLocalFile(outfileName));
    emit this->m_zoomSelector->zoomModeChanged(QPdfView::FitToWidth);
    ui->textEditLog->append("PDF�ϲ���ɱ����ڣ�" + info.filePath());
    QMessageBox::information(nullptr, "PDF�ϲ���ɣ�",
                             "�ļ������ڣ�" + info.filePath() + "\n");
    QDir temp(tempFile);
    // ɾ������Ŀ¼������Ŀ¼
    temp.removeRecursively();
  }
}

void MainWindow::on_lineEditInputFilesplit_editingFinished() {}
