#include "function.h"
#pragma execution_character_set("utf-8")  //防止出现中文乱码
#include <QDebug>
#include <QElapsedTimer>
#include <QThread>

#include "include/mark/mark.h"
#include "include/mark/multiWatermarkThreadSingle.h"

multiWatermarkThreadSingle::multiWatermarkThreadSingle(QMutex *mutex, wMap *map)
    : m_mutex(mutex), m_wmap(map) {
  setAutoDelete(true);
}
multiWatermarkThreadSingle::multiWatermarkThreadSingle(QObject *parent)
    : QObject(parent), QRunnable() {
  setAutoDelete(true);
}

void multiWatermarkThreadSingle::recvNum(int num) { m_num = num; }

void multiWatermarkThreadSingle::setInputFilename(QString input) {
  m_input = input;
}
void multiWatermarkThreadSingle::setOutputFilename(QString ouput) {
  m_output = ouput;
}
void multiWatermarkThreadSingle::setText(QString text) {
  if (!text.isEmpty()) {
    m_text = text;
  }
}

void multiWatermarkThreadSingle::setOpacity(QString opacity) {
  m_opacity = opacity;
}
void multiWatermarkThreadSingle::setColor(QString color) { m_color = color; }
void multiWatermarkThreadSingle::setFont(QString font) { m_font = font; }
void multiWatermarkThreadSingle::setRotate(QString rotate) {
  m_rotate = rotate;
}
void multiWatermarkThreadSingle::setFontsize(QString fontsize) {
  m_fontsize = fontsize;
}

void multiWatermarkThreadSingle::run() {
  // qDebug() << "生成随机数的线程的线程地址: " << QThread::currentThread();

  // QElapsedTimer time;
  // time.start();
  // wMap map;
  //  qDebug()<<"输入："<<m_filename;
  // qDebug()<<"输出："<<pathChange(m_input,m_output, m_filename,
  // "_out_").replace("//", "/");

  // int r = addWatermark(m_input, m_output, m_text, m_opacity, m_color,
  // m_rotate,m_font);
  // if ((!m_input.contains("_out_")) && (!m_input.contains("_pdf_"))) {
  int r =
      addWatermark_multiline(m_input.toStdString(), m_output.toStdString(),
                             m_text, m_font, m_fontsize.toInt(), m_color,
                             m_rotate.toDouble(), m_opacity.toDouble() / 100);

  m_mutex->lock();
  m_wmap->insert(m_input, r);
  m_mutex->unlock();
  // int r=addWatermark("h:/in.pdf", "h:/out.pdf",
  // "联通数字科技有限公司总部投标专用文档", "15%", "#c8250c", "45",
  // "simkai"); int milsec = time.elapsed(); qDebug() << "生成" << m_num <<
  // "个随机数总共用时:" << milsec << "毫秒";

  emit addFinish(*m_wmap);
  // }
}
