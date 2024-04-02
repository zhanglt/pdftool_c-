#include "include/slider/CustomSlider.h"

#include <QMessageBox>

#include "qcoreapplication.h"
#include "qcoreevent.h"

void CustomSlider::mousePressEvent(QMouseEvent* event) {
  //��ȡ�������ǰ��ֵ
  const int value = this->value();
  //���ø��������������¼�
  QSlider::mousePressEvent(event);
  setValue(value);
  //�򸸴��ڷ����Զ����¼�event type�������Ϳ����ڸ������в�������¼����д���
  QEvent evEvent(static_cast<QEvent::Type>(QEvent::User + 1));
  QCoreApplication::sendEvent(parentWidget(), &evEvent);
}

CustomSlider::CustomSlider(QWidget* parent) : QSlider(parent) {}

CustomSlider::CustomSlider::~CustomSlider() {}
