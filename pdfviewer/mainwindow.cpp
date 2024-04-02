#pragma execution_character_set("utf-8")
#include "mainwindow.h"

#include <QDebug>
#include <QFileDialog>
#include <QFutureWatcher>
#include <QMessageBox>
#include <QPdfBookmarkModel>
#include <QPdfDocument>
#include <QPdfPageNavigation>
#include <QProgressDialog>
#include <QtConcurrent/QtConcurrent>
#include <QtMath>

#include "function.h"
#include "include/QProgressIndicator.h"
#include "include/mark/mark.h"
#include "pageselector.h"
#include "qColordialog.h"
#include "ui_mainwindow.h"
#include "zoomselector.h"
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
  m_title->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 255);"));
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
  connect(ui->lineEditWaterText, &CustomLineEdit::dataChanged, this,
          &MainWindow::viewWatermark);
  // �϶�����ı���ֵ����ͬ������Ԥ��
  connect(ui->sliderOpacity, &QSlider::sliderReleased, this,
          &MainWindow::viewWatermark);
  connect(ui->sliderRotate, &QSlider::sliderReleased, this,
          &MainWindow::viewWatermark);
  // ����slider��ͬ������
  // ��ֵ��ʾ��������ܿ��Ժϲ���MainWindow::viewWatermark�У�
  connect(ui->sliderRotate, &QSlider::valueChanged, this, [=]() {
    ui->lineEditRotate->setText(QString::number(ui->sliderRotate->value()));
  });
  connect(ui->sliderOpacity, &QSlider::valueChanged, this, [=]() {
    ui->lineEditOpacity->setText(QString::number(ui->sliderOpacity->value()));
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

  //��������QPdfView�ı���ɫ��
  QPalette palette = ui->pdfView->palette();
  QColor color(199, 199, 199);
  palette.setBrush(QPalette::Dark, color);
  ui->pdfView->setPalette(palette);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::open(const QUrl &docLocation) {
  if (docLocation.isLocalFile()) {
    m_document->load(docLocation.toLocalFile());
    const auto documentTitle =
        m_document->metaData(QPdfDocument::Title).toString();
    setWindowTitle(!documentTitle.isEmpty() ? documentTitle
                                            : QStringLiteral("PDF Viewer"));
  } else {
    // qCDebug(lcExample) << docLocation << "is not a valid local file";
    QMessageBox::critical(
        this, tr("Failed to open"),
        tr("%1 is not a valid local file").arg(docLocation.toString()));
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
      QUrl("file:///" + ui->lineEditOutput->text()), "Documents (*.pdf)");

  if (toOpen.isValid()) open(toOpen);
  emit m_zoomSelector->zoomModeChanged(QPdfView::FitToWidth);
}

void MainWindow::on_actionQuit_triggered() { QApplication::quit(); }

void MainWindow::on_actionAbout_triggered() {
  QMessageBox::about(this, tr("����pdf����"),
                     tr("����̬ҵ���߹���-�����ڲ�ʹ��"));
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
  QString text = ui->lineEditWaterText->text();
  if (text.isEmpty()) {
    QMessageBox::information(nullptr, "��ʾ", "ˮӡ�ı�����Ϊ��");
    return;
  }
  QString inputDir = ui->lineEditInput->text();

  if (inputDir.isEmpty()) {
    ui->lineEditInput->setFocus();
    QMessageBox::information(nullptr, "��ʾ", "����Ŀ¼����ѡ����ȷĿ¼");
    return;
  }
  QString outputDir = ui->lineEditOutput->text();
  QFileInfo output(outputDir);
  if (!output.isDir()) {
    ui->lineEditOutput->setFocus();
    QMessageBox::information(nullptr, "��ʾ", "����Ŀ¼����ѡ����ȷĿ¼");
    return;
  }
  QString color = ui->lineEditColor->text();
  QString opacity = ui->lineEditOpacity->text();
  QString rotate = ui->lineEditRotate->text();
  int fontIndex = ui->cBoxFont->currentIndex();
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
  QFileInfo dir(inputDir);
  if (dir.isFile() && dir.suffix().toLower() == "pdf") {
    addWatermark(inputDir, outputDir + "/_out_/" + dir.fileName(), text,
                 opacity + "%", color, rotate, font);
  } else {
    addWatermarkSingle(text, inputDir, outputDir, color, opacity, rotate, font);
  }
  // qDebug() << "�ܹ���ʱ:" << time.elapsed() << "����";
  ui->textEditLog->append("����ˮӡ��ʱ:" + QString::number(time.elapsed()) +
                          "����,����ļ�������:" + ui->lineEditOutput->text() +
                          "/_out_/");
}

/*
 * ���ڲ����޸ĺ�Ľ���Ԥ��
 **/
void MainWindow::viewWatermark() {
  QElapsedTimer time;
  time.start();
  QString text = ui->lineEditWaterText->text();
  if (text.isEmpty()) {
    QMessageBox::information(nullptr, "��ʾ", "ˮӡ�ı�����Ϊ��");
    return;
  }
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
  addWatermark("doc/1.pdf", "doc/2.pdf", text, opacity, color, rotate, font);
  this->open(QUrl::fromLocalFile("doc/2.pdf"));
  emit this->m_zoomSelector->zoomModeChanged(QPdfView::FitToWidth);
}
/**
 * ����ˮӡ���ܺ���
 **/

void MainWindow::addWatermarkSingle(QString text, QString inputDir,
                                    QString outputDir, QString color,
                                    QString opacity, QString rotate,
                                    QString font) {
  QDir dir(inputDir);
  QStringList files;
  traverseDirectory(dir, files, "pdf", "_out_");
  QMutex m_mutex;
  wMap map;
  if (files.length() == 0) {
    QMessageBox::information(nullptr, "��ʾ��", "����Ŀ¼���ļ�Ϊ��");
  } else {
    for (const auto &file : files) {
      QString outfile =
          pathChange(inputDir, outputDir, file, "_out_").replace("//", "/");
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
      wmThreadSinge->setInputFilename(file);
      wmThreadSinge->setOutputFilename(outfile);
      threadPool.start(wmThreadSinge);
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
  traverseDirectory(pdfdir, files, "pdf", "");
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
          qDebug() << "ͼƬĿ¼��" << imageRootDir.path() << "ɾ���ɹ�.";
        } else {
          qDebug() << "ͼƬĿ¼��" << imageRootDir.path() << "ɾ��ʧ��";
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
    pdf2imageThread->setResolution(ui->cBoxResolution->currentText().toInt());
    threadPool.start(pdf2imageThread);
    // threadPool.waitForDone();
  }

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

  hLayout->setMargin(0);           // �봰����޾��� ����ռ��
  hLayout->addWidget(pIndicator);  // ����ؼ�
  hLayout->addWidget(l1);
  hLayout->setAlignment(pIndicator, Qt::AlignCenter);  // �ؼ�����
  hLayout->setAlignment(l1, Qt::AlignCenter);          // �ؼ�����
  // ui->tab_2->setLayout(hLayout);
  dialog.setLayout(hLayout);

  pIndicator->setColor(QColor(9, 126, 186));
  pIndicator->startAnimation();  //��������
  dialog.exec();
}

void MainWindow::on_cBoxFont_currentIndexChanged(int index) { viewWatermark(); }

// ѡ��ͼƬ�ļ�
void MainWindow::on_btnSelectImageFile_clicked() {
  QString filePath = QFileDialog::getOpenFileName(
      nullptr, "Open Image", QDir::homePath(),
      "Images Files (*.png *.PNG *.jpg *.JPG *.jpeg *.JPEG)");

  // ����û�ѡ����Ŀ¼���������ѡĿ¼��·��

  if (!filePath.isEmpty()) {
    qDebug() << "ѡ���Ŀ¼��" << filePath;
    ui->lineEditImageFile->setText(filePath);
  }
}

void MainWindow::on_btnTransform_clicked() {
  QFileInfo info(ui->lineEditImageFile->text());
  // QString path = info.path();
  // QString name = info.filePath();
  if (image2pdf(info.filePath().toStdString(),
                (info.filePath() + ".pdf").toStdString()) == 0) {
    ui->textEditLog->append("�ļ������ڣ�" + info.filePath() + ".pdf\n");
    this->open(QUrl::fromLocalFile(info.filePath() + ".pdf"));
    emit this->m_zoomSelector->zoomModeChanged(QPdfView::FitToWidth);
    QMessageBox::information(nullptr, "PDFת����ɣ�",
                             "�ļ������ڣ�" + info.filePath() + ".pdf\n");
  }
}

void MainWindow::on_btnSelectImageDir_clicked() {
  QString directory = QFileDialog::getExistingDirectory(
      nullptr, "ѡ������Ŀ¼", "",
      QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

  // ����û�ѡ����Ŀ¼���������ѡĿ¼��·��
  if (!directory.isEmpty()) {
    qDebug() << "ѡ���Ŀ¼��" << directory;
    ui->lineEditImageDir->setText(directory);
  }
}

// ����ת��
void MainWindow::on_btnTransformBat_clicked() {
  QDir dir(ui->lineEditImageDir->text());
  QStringList files;
  tDirectory(dir, files, {"png", "jpg", "jpeg"});
  for (QString &file : files) {
    image2pdf(file.toStdString(), (file + ".pdf").toStdString());
  }
  QMessageBox::information(nullptr, "PDFת����ɣ�", "PDF������ͼƬԴĿ¼��");
  ui->textEditLog->append("PDF������ͼƬԴĿ¼��:" +
                          ui->lineEditImageDir->text());
}
