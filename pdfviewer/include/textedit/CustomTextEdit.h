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
  ///< �϶��ļ������� ����
  void dragEnterEvent(QDragEnterEvent* event);
  ///< �϶��ļ��������ƶ��ļ� ����
  void dragMoveEvent(QDragMoveEvent* event);
  ///< �϶��ļ��������ͷ��ļ�����
  void dropEvent(QDropEvent* event);
  ///< ʧȥ���㴥��
  void focusOutEvent(QFocusEvent* event);
 signals:
  void dataChanged();
};
