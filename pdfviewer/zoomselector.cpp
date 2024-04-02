#pragma execution_character_set("utf-8")
#include "zoomselector.h"

#include <QLineEdit>

ZoomSelector::ZoomSelector(QWidget *parent) : QComboBox(parent) {
  setEditable(true);

  addItem("适合宽度");
  addItem("适合页面");
  addItem(QLatin1String("12%"));
  addItem(QLatin1String("25%"));
  addItem(QLatin1String("33%"));
  addItem(QLatin1String("50%"));
  addItem(QLatin1String("66%"));
  addItem(QLatin1String("75%"));
  addItem(QLatin1String("100%"));
  addItem(QLatin1String("125%"));
  addItem(QLatin1String("150%"));
  addItem(QLatin1String("200%"));
  addItem(QLatin1String("400%"));

  connect(this,
          static_cast<void (QComboBox::*)(const QString &)>(
              &QComboBox::currentIndexChanged),
          this, &ZoomSelector::onCurrentTextChanged);

  connect(lineEdit(), &QLineEdit::editingFinished, this,
          [this]() { onCurrentTextChanged(lineEdit()->text()); });
}

void ZoomSelector::setZoomFactor(qreal zoomFactor) {
  setCurrentText(QString::number(qRound(zoomFactor * 100)) +
                 QLatin1String("%"));
}

void ZoomSelector::reset() {
  setCurrentIndex(5);  // 100%
}

void ZoomSelector::onCurrentTextChanged(const QString &text) {
  if (text == "适合宽度") {
    emit zoomModeChanged(QPdfView::FitToWidth);
  } else if (text == "适合页面") {
    emit zoomModeChanged(QPdfView::FitInView);
  } else {
    qreal factor = 1.0;

    QString withoutPercent(text);
    withoutPercent.remove(QLatin1Char('%'));

    bool ok = false;
    const int zoomLevel = withoutPercent.toInt(&ok);
    if (ok) factor = zoomLevel / 100.0;

    emit zoomModeChanged(QPdfView::CustomZoom);
    emit zoomFactorChanged(factor);
  }
}
