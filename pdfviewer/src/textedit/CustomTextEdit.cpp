

#include "include/textedit/CustomTextEdit.h"

#include <QMessageBox>
CustomTextEdit::CustomTextEdit(QWidget* parent) : QTextEdit(parent) {
  setAcceptDrops(true);
}

CustomTextEdit::~CustomTextEdit() {}
void CustomTextEdit::dragEnterEvent(QDragEnterEvent* event) {
  if (event->mimeData()->hasUrls())
    event->acceptProposedAction();  //事件数据中存在路径，方向事件
  else
    event->ignore();
}

void CustomTextEdit::dragMoveEvent(QDragMoveEvent* event) {}

void CustomTextEdit::dropEvent(QDropEvent* event) {
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

void CustomTextEdit::focusOutEvent(QFocusEvent* event) {
  QTextEdit::focusOutEvent(event);  // 调用基类的实现
  if (this->document()->isModified()) {
    setText(this->toPlainText());
    // QMessageBox::information(this, "Focus Out", "LineEdit has lost focus.");
    emit dataChanged();
  }
}
