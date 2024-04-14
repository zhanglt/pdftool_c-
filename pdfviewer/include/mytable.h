#include "qdebug.h"
#include "qmimedata.h"
#pragma execution_character_set("utf-8")
#ifndef MYTABLE_H
#define MYTABLE_H
#include <QApplication>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QTableWidget>

#include "function.h"

/**
    扩展QTableWidget 增加行移动方法
**/
class mytable : public QTableWidget {
  Q_OBJECT
 public:
  explicit mytable(QWidget *parent = nullptr) : QTableWidget(parent) {
    setAcceptDrops(true);
  }

  // 设置当前行
  void setCurrrow(int row) { currRow = row; }
  // 获取当前行
  int getCurrrow() { return currRow; }
  // 设置当前行文件名
  void setCurrFile(QString file) { currFile = file; }
  // 获取当前行文件名
  QString getCurrFile() { return currFile; }

 protected:
  void keyPressEvent(QKeyEvent *event) override {
    if (event->key() == Qt::Key_Up) {
      moveRowUp(currentRow());
    } else if (event->key() == Qt::Key_Down) {
      moveRowDown(currentRow());
    } else {
      QTableWidget::keyPressEvent(event);
    }
  }

  void dragEnterEvent(QDragEnterEvent *event) override {
    if (event->mimeData()->hasUrls()) {
      event->acceptProposedAction();
    }
  }

  void dragMoveEvent(QDragMoveEvent *event) override {
    if (event->mimeData()->hasUrls()) {
      event->acceptProposedAction();
    }
  }

  void dropEvent(QDropEvent *event) override {
    if (event->mimeData()->hasUrls()) {
      QList<QUrl> urls = event->mimeData()->urls();
      for (const QUrl &url : urls) {
        if (url.isLocalFile()) {
          QString filePath = url.toLocalFile();
          if (isPDFByExtension(filePath) && isPDFByMagicNumber(filePath)) {
            AddFile(filePath);
          } else {
            QMessageBox::information(nullptr, "提示信息！",
                                     "请选择正确的PDF类型文件");
            return;
          }
        }
      }
      event->acceptProposedAction();
    }
  }
  void mouseDoubleClickEvent(QMouseEvent *event) override {
    QTableWidgetItem *item = currentItem();
    if (item != nullptr) {
      setCurrFile(item->text());
    }
    QTableWidget::mouseDoubleClickEvent(event);
    emit selectRow();
  }

 public slots:

  // 行上移
  void moveRowUp(int rowIndex) {
    if (rowIndex > 0 && rowIndex < rowCount()) {
      // 交换当前行和上一行的数据
      for (int col = 0; col < columnCount() - 1; ++col) {
        QTableWidgetItem *currentItem = takeItem(rowIndex, col);
        QTableWidgetItem *upperItem = takeItem(rowIndex - 1, col);
        setItem(rowIndex - 1, col, currentItem);
        setItem(rowIndex, col, upperItem);
      }
      // 获取第三列中的lineedit对象、获取其值，并进行行间交换
      QWidget *currenCell = cellWidget(rowIndex, 2);
      QWidget *upperCell = cellWidget(rowIndex - 1, 2);
      QList<QLineEdit *> currLine = currenCell->findChildren<QLineEdit *>();
      QList<QLineEdit *> upperLine = upperCell->findChildren<QLineEdit *>();
      // 保存当前行的开始、结束页码
      QString cStart = currLine[0]->text();
      QString cEnd = currLine[1]->text();
      // 设置当前行的值为 上一行的值
      currLine[0]->setText(upperLine[0]->text());
      currLine[1]->setText(upperLine[1]->text());
      // 设置上一行的值 为保存的当行含的值
      upperLine[0]->setText(cStart);
      upperLine[1]->setText(cEnd);
      // 更新当前行索引
      setCurrentCell(rowIndex - 1, currentColumn());
    }
  }
  // 行下移
  void moveRowDown(int rowIndex) {
    // qDebug() << "当前行" << currentRow();
    // qDebug() << "rowIndex" << rowIndex;
    if (rowIndex > -1 && rowIndex < rowCount() - 1) {
      // 交换当前行和上一行的数据
      for (int col = 0; col < columnCount(); ++col) {
        QTableWidgetItem *currentItem = takeItem(rowIndex, col);
        QTableWidgetItem *upperItem = takeItem(rowIndex + 1, col);
        setItem(rowIndex + 1, col, currentItem);
        setItem(rowIndex, col, upperItem);
      }
      QWidget *currenCell = cellWidget(rowIndex, 2);
      QWidget *upperCell = cellWidget(rowIndex + 1, 2);
      QList<QLineEdit *> currLine = currenCell->findChildren<QLineEdit *>();
      QList<QLineEdit *> upperLine = upperCell->findChildren<QLineEdit *>();
      QString cStart = currLine[0]->text();
      QString cEnd = currLine[1]->text();
      currLine[0]->setText(upperLine[0]->text());
      currLine[1]->setText(upperLine[1]->text());
      upperLine[0]->setText(cStart);
      upperLine[1]->setText(cEnd);
      setCurrentCell(rowIndex + 1, currentColumn());
    }
  }
  // 增加并文件列表
  void AddFile(QString fileName) {
    if (!fileName.isEmpty()) {
      int rowCount = this->rowCount() - 1;
      this->insertRow(this->rowCount());  // 在最后插入行

      // 创建容器控件
      QWidget *containerBtn = new QWidget();
      QHBoxLayout *layoutBtn = new QHBoxLayout(containerBtn);
      layoutBtn->setContentsMargins(0, 0, 0, 0);  // 设置布局边距为0

      // 创建三个 QPushButton 控件，用于操作 上、下、删除操作
      QPushButton *btnDown = new QPushButton("↓");
      QPushButton *btnUp = new QPushButton("↑");
      QPushButton *btnRemove = new QPushButton("×");
      btnDown->setStyleSheet("min-width: 20px;border-width: 0px;");
      btnUp->setStyleSheet("min-width: 20px;border-width: 0px;");
      btnRemove->setStyleSheet("min-width: 20px;border-width: 0px;");
      btnDown->setFixedSize(20, 20);
      btnUp->setFixedSize(20, 20);
      btnRemove->setFixedSize(20, 20);
      // 将两个按钮添加到容器的布局中
      layoutBtn->addWidget(btnDown);
      layoutBtn->addWidget(btnUp);
      layoutBtn->addWidget(btnRemove);

      // 设置容器控件为单元格的控件
      rowCount = this->rowCount() - 1;
      //当点击按钮时设置当前行
      connect(btnDown, SIGNAL(clicked()), this, SLOT(SlotSetCurrRow()));
      // 下移一行操作
      connect(btnDown, &QPushButton::clicked, this,
              [=]() { this->moveRowDown(this->getCurrrow()); });
      connect(btnUp, SIGNAL(clicked()), this, SLOT(SlotSetCurrRow()));
      // 上移一行操作
      connect(btnUp, &QPushButton::clicked, this,
              [=]() { this->moveRowUp(this->getCurrrow()); });
      connect(btnRemove, SIGNAL(clicked()), this, SLOT(SlotSetCurrRow()));
      // 删除操作
      connect(btnRemove, &QPushButton::clicked, this,
              [=]() { this->removeRow(this->getCurrrow()); });

      QWidget *containerEdit = new QWidget();
      QHBoxLayout *layoutEdit = new QHBoxLayout(containerEdit);
      layoutEdit->setContentsMargins(0, 0, 0, 0);  // 设置布局边距为0
      // 创建两个lineedit用于保存 开始、结束页码
      QLineEdit *leStart = new QLineEdit("1");
      QLabel *label = new QLabel("-");
      QLineEdit *leEnd =
          new QLineEdit(QString::number(getPages(fileName.toStdString())));
      leStart->setFixedWidth(30);
      label->setFixedWidth(10);
      leEnd->setFixedWidth(30);
      layoutEdit->addWidget(leStart);
      layoutEdit->addWidget(label);
      layoutEdit->addWidget(leEnd);
      // 校验开始页码
      connect(leStart, &QLineEdit::editingFinished, this, [=]() {
        QString start = leStart->text();
        QString end = leEnd->text();
        QRegularExpression re("\\d+");  // 匹配一个或多个数字
        QRegularExpressionMatch match = re.match(start);

        if (!(isNumeric(start)) || (start.toInt() <= 0)) {
          leStart->setText("1");
          QMessageBox::information(nullptr, "提示信息！", "必须是大于0的数字");
        }
        if (start.toInt() > end.toInt()) {
          QString page = QString::number(getPages(fileName.toStdString()));
          leStart->setText("1");
          leEnd->setText(page);
          QMessageBox::information(nullptr, "提示信息！",
                                   "结束页数需要大于等于开始页数");
        }
      });
      // 校验结束页码
      connect(leEnd, &QLineEdit::editingFinished, this, [=]() {
        QString start = leStart->text();
        QString end = leEnd->text();
        QRegularExpression re("\\d+");  // 匹配一个或多个数字
        QRegularExpressionMatch match = re.match(start);

        if (!(isNumeric(start)) || (start.toInt() <= 0)) {
          leStart->setText("1");
          QMessageBox::information(nullptr, "提示信息！", "必须是大于0的数字");
        }
        if (start.toInt() > end.toInt()) {
          QString page = QString::number(getPages(fileName.toStdString()));
          leEnd->setText(page);
          leStart->setText("1");
          QMessageBox::information(nullptr, "提示信息！",
                                   "结束页数需要大于等于开始页数");
        }
      });

      QTableWidgetItem *itemFile = new QTableWidgetItem(fileName);
      // 设置不可编辑
      itemFile->setFlags(itemFile->flags() & ~Qt::ItemIsEditable);
      QTableWidgetItem *itemPages = new QTableWidgetItem(
          QString::number(getPages(fileName.toStdString())));
      itemPages->setFlags(itemPages->flags() & ~Qt::ItemIsEditable);

      this->setItem(rowCount, 0, itemFile);
      this->setItem(rowCount, 1,
                    itemPages);  // 将容器放置在第一行第二列
      this->setCellWidget(rowCount, 2,
                          containerEdit);  // 将容器放置在第一行第三列
      this->setCellWidget(rowCount, 3,
                          containerBtn);  // 将容器放置当前行 第四列
    }
  }

  // 获取pushbutton所在的行位置，并将其设置为当前行
  void SlotSetCurrRow() {
    QPushButton *senderButton = qobject_cast<QPushButton *>(QObject::sender());
    if (!senderButton) {
      qDebug() << "获取按钮及所在行失败:";
      return;
    }
    QModelIndex idx = this->indexAt(
        QPoint(senderButton->parentWidget()->frameGeometry().x(),
               senderButton->parentWidget()->frameGeometry().y()));
    int row = idx.row();
    this->setCurrrow(row);
  }

 signals:
  void selectRow();

 public:
  // 当前行
  int currRow;
  QString currFile;
};
#endif  // MYTABLE_H
