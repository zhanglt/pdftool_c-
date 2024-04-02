#pragma execution_character_set("utf-8")  //防止出现中文乱码
#include "include/mark/watermarkThread.h"

#include <QDebug>
#include <QElapsedTimer>
#include <QThread>

#include "../../include/mark/mark.h"
#include "function.h"

watermarkThread::watermarkThread(QObject* parent)
    : QObject(parent), QRunnable() {
  setAutoDelete(true);
}
void watermarkThread::recvNum(int num) { m_num = num; }
void watermarkThread::setFilename(QString filename) { m_filename = filename; }

void watermarkThread::setFiles(QStringList files) { m_files = files; }
void watermarkThread::setInputDir(QString input) { m_input = input; }
void watermarkThread::setOutputDir(QString ouput) { m_output = ouput; }
void watermarkThread::setText(QString text) {
  if (!text.isEmpty()) {
    m_text = text;
  }
}
void watermarkThread::setOpacity(QString opacity) { m_opacity = opacity + "%"; }
void watermarkThread::setColor(QString color) { m_color = color; }
void watermarkThread::setFont(QString font) { m_font = font; }
void watermarkThread::setRotate(QString rotate) { m_rotate = rotate; }
void watermarkThread::setFontsize(QString fontsize) { m_fontsize = fontsize; }

void watermarkThread::run() {
  qDebug() << "生成随机数的线程的线程地址: " << QThread::currentThread();

  QElapsedTimer time;
  time.start();
  QMap<QString, int> map;
  int milsec = time.elapsed();

  for (const QString& file : m_files) {
    // qDebug() << "文件名称：" << file;
    // qDebug() << "输出文件：" << pathChange(m_output, str, "_out_");
    qDebug() << "文件名称：" << file << "输出文件："
             << pathChange(m_input, m_output, file, "_out_").replace("//", "/");
    qDebug() << "文本：" << m_text << "不透明：" << m_opacity;
    qDebug() << "颜色：" << m_color << "旋转：" << m_rotate;
    qDebug() << "字体：" << m_font;
    int r = addWatermark(
        file, pathChange(m_input, m_output, file, "_out_").replace("//", "/"),
        m_text, m_opacity, m_color, m_rotate, m_font);
    map.insert(m_filename, r);
  }

  // int r=addWatermark("h:/in.pdf", "h:/out.pdf",
  // "联通数字科技有限公司总部投标专用文档", "15%", "#c8250c", "45", "simkai");

  qDebug() << "生成" << m_num << "个随机数总共用时:" << milsec << "毫秒";

  emit addFinish(map);
}
