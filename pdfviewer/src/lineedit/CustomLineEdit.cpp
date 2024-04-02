

#include "include/lineedit/CustomLineEdit.h"

#include <QMessageBox>
CustomLineEdit::CustomLineEdit(QWidget* parent) : QLineEdit(parent) {
  setAcceptDrops(true);
}

CustomLineEdit::~CustomLineEdit() {}
void CustomLineEdit::dragEnterEvent(QDragEnterEvent* event) {
  if (event->mimeData()->hasUrls())
    event->acceptProposedAction();  //�¼������д���·���������¼�
  else
    event->ignore();
}

void CustomLineEdit::dragMoveEvent(QDragMoveEvent* event) {}

void CustomLineEdit::dropEvent(QDropEvent* event) {
  const QMimeData* mimeData = event->mimeData();
  if (mimeData->hasUrls()) {
    QList<QUrl> urls = mimeData->urls();
    QString fileName = urls.at(0).toLocalFile();
    setText(fileName);
    event->accept();
  } else {
    event->ignore();
  }
}

void CustomLineEdit::focusOutEvent(QFocusEvent* event) {
  QLineEdit::focusOutEvent(event);  // ���û����ʵ��
  if (isModified()) {
    setText(this->text());
    // QMessageBox::information(this, "Focus Out", "LineEdit has lost focus.");
    emit dataChanged();
  }
}
