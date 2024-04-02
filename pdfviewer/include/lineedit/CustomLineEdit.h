#pragma once

#include <QDragEnterEvent>
#include <QDropEvent>
#include <QLineEdit>
#include <QMimeData>
#include <QtWidgets/Qwidget>
class CustomLineEdit : public QLineEdit {
  Q_OBJECT

 public:
  CustomLineEdit(QWidget* parent = nullptr);
  ~CustomLineEdit();

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
