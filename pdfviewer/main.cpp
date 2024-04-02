
#pragma execution_character_set("utf-8")
#include <QApplication>
#include <QFile>
#include <QMessageBox>
#include <QPdfView>
#include <QUrl>

#include "mainwindow.h"
#include "qtranslator.h"
#include "zoomselector.h"
int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  // ���ر��ػ��ļ�
  QTranslator base;
  if (base.load("languages/qt_zh_CN.qm")) {
    a.installTranslator(&base);
  } else {
    QMessageBox::warning(
        nullptr, QObject::tr("����"),
        QObject::tr("���ط����ļ�qt_zh_CN.qmʧ�ܣ����ֽ�������޷�������ʾ��"));
  }

  //������ʽ��
  QFile file("qss/pdf.css");
  if (file.open(QFile::ReadOnly)) {
    QString qss = QLatin1String(file.readAll());
    QString paletteColor = qss.mid(20, 7);
    qApp->setPalette(QPalette(QColor(paletteColor)));
    qApp->setStyleSheet(qss);
    file.close();
  }

  MainWindow w;

  QString styleSheet =
      "QWidget::titleBar {"
      "background-color: green;"
      "}";
  w.setStyleSheet("QMainWindow::titleBar {background-color: #bbd4ec;}");
  // w.setStyleSheet(styleSheet);

  QStringList args = a.arguments();

  w.setWindowFlags(w.windowFlags() & ~Qt::WindowMinMaxButtonsHint |
                   Qt::WindowMinimizeButtonHint);
  w.show();
  // if (args.length() > 1)
  //    w.open(QUrl::fromLocalFile(args[1]));
  //��һ�������ĵ�
  w.open(QUrl::fromLocalFile("doc/2.pdf"));
  // �����źţ����������ĵ�����ʾ��С
  emit w.m_zoomSelector->zoomModeChanged(QPdfView::FitToWidth);
  return a.exec();
}
