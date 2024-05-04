#include "function.h"
#pragma execution_character_set("utf-8")  //��ֹ������������
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
  // qDebug() << "������������̵߳��̵߳�ַ: " << QThread::currentThread();

  // QElapsedTimer time;
  // time.start();
  // wMap map;
  //  qDebug()<<"���룺"<<m_filename;
  // qDebug()<<"�����"<<pathChange(m_input,m_output, m_filename,
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
  // "��ͨ���ֿƼ����޹�˾�ܲ�Ͷ��ר���ĵ�", "15%", "#c8250c", "45",
  // "simkai"); int milsec = time.elapsed(); qDebug() << "����" << m_num <<
  // "��������ܹ���ʱ:" << milsec << "����";

  emit addFinish(*m_wmap);
  // }
}
