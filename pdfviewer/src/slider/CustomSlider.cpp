#include "include/slider/CustomSlider.h"

#include <QMessageBox>

#include "qcoreapplication.h"
#include "qcoreevent.h"

void CustomSlider::mousePressEvent(QMouseEvent* event) {
  //获取点击触发前的值
  const int value = this->value();
  //调用父类的鼠标点击处理事件
  QSlider::mousePressEvent(event);
  setValue(value);
  //向父窗口发送自定义事件event type，这样就可以在父窗口中捕获这个事件进行处理
  QEvent evEvent(static_cast<QEvent::Type>(QEvent::User + 1));
  QCoreApplication::sendEvent(parentWidget(), &evEvent);
}

CustomSlider::CustomSlider(QWidget* parent) : QSlider(parent) {}

CustomSlider::CustomSlider::~CustomSlider() {}
