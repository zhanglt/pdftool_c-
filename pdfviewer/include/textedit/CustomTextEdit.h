#pragma once

#include <QDragEnterEvent>
#include <QDropEvent>
#include <QTextEdit>
#include <QMimeData>
#include <QtWidgets/Qwidget>
class CustomTextEdit : public QTextEdit {
  Q_OBJECT

 public:
  CustomTextEdit(QWidget* parent = nullptr);
  ~CustomTextEdit();

 protected:
  ///< 拖动文件到窗口 触发
  void dragEnterEvent(QDragEnterEvent* event);
  ///< 拖动文件到窗口移动文件 触发
  void dragMoveEvent(QDragMoveEvent* event);
  ///< 拖动文件到窗口释放文件触发
  void dropEvent(QDropEvent* event);
  ///< 失去焦点触发
  void focusOutEvent(QFocusEvent* event);
 signals:
  void dataChanged();
};
