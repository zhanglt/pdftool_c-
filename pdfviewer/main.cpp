
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

  // 加载本地化文件
  QTranslator base;
  if (base.load("languages/qt_zh_CN.qm")) {
    a.installTranslator(&base);
  } else {
    QMessageBox::warning(
        nullptr, QObject::tr("警告"),
        QObject::tr("加载翻译文件qt_zh_CN.qm失败，部分界面可能无法汉化显示！"));
  }

  //加载样式表
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
  //打开一个样例文档
  w.open(QUrl::fromLocalFile("doc/2.pdf"));
  // 发送信号，调整样例文档的显示大小
  emit w.m_zoomSelector->zoomModeChanged(QPdfView::FitToWidth);
  return a.exec();
}
