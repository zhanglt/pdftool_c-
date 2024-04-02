#pragma execution_character_set("utf-8")  //��ֹ������������
#include "include/mark/watermarkThreadSingle.h"

#include <QDebug>
#include <QElapsedTimer>
#include <QThread>

#include "include/mark/mark.h"

watermarkThreadSingle::watermarkThreadSingle(QMutex *mutex, wMap *map)
    : m_mutex(mutex), m_wmap(map) {
  setAutoDelete(true);
}
watermarkThreadSingle::watermarkThreadSingle(QObject *parent)
    : QObject(parent), QRunnable() {
  setAutoDelete(true);
}

void watermarkThreadSingle::recvNum(int num) { m_num = num; }

void watermarkThreadSingle::setInputFilename(QString input) { m_input = input; }
void watermarkThreadSingle::setOutputFilename(QString ouput) {
  m_output = ouput;
}
void watermarkThreadSingle::setText(QString text) {
  if (!text.isEmpty()) {
    m_text = text;
  }
}

void watermarkThreadSingle::setOpacity(QString opacity) {
  m_opacity = opacity + "%";
}
void watermarkThreadSingle::setColor(QString color) { m_color = color; }
void watermarkThreadSingle::setFont(QString font) { m_font = font; }
void watermarkThreadSingle::setRotate(QString rotate) { m_rotate = rotate; }
void watermarkThreadSingle::setFontsize(QString fontsize) {
  m_fontsize = fontsize;
}

void watermarkThreadSingle::run() {
  // qDebug() << "������������̵߳��̵߳�ַ: " << QThread::currentThread();

  // QElapsedTimer time;
  // time.start();
  // wMap map;
  //  qDebug()<<"���룺"<<m_filename;
  // qDebug()<<"�����"<<pathChange(m_input,m_output, m_filename,
  // "_out_").replace("//", "/");

  int r = addWatermark(m_input, m_output, m_text, m_opacity, m_color, m_rotate,
                       m_font);
  m_mutex->lock();
  m_wmap->insert(m_input, r);
  m_mutex->unlock();
  // int r=addWatermark("h:/in.pdf", "h:/out.pdf",
  // "��ͨ���ֿƼ����޹�˾�ܲ�Ͷ��ר���ĵ�", "15%", "#c8250c", "45", "simkai");
  // int milsec = time.elapsed();
  // qDebug() << "����" << m_num << "��������ܹ���ʱ:" << milsec << "����";

  emit addFinish(*m_wmap);
}
