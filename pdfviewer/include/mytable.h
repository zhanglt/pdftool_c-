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
    ��չQTableWidget �������ƶ�����
**/
class mytable : public QTableWidget {
  Q_OBJECT
 public:
  explicit mytable(QWidget *parent = nullptr) : QTableWidget(parent) {
    setAcceptDrops(true);
  }

  // ���õ�ǰ��
  void setCurrrow(int row) { currRow = row; }
  // ��ȡ��ǰ��
  int getCurrrow() { return currRow; }
  // ���õ�ǰ���ļ���
  void setCurrFile(QString file) { currFile = file; }
  // ��ȡ��ǰ���ļ���
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
            QMessageBox::information(nullptr, "��ʾ��Ϣ��",
                                     "��ѡ����ȷ��PDF�����ļ�");
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

  // ������
  void moveRowUp(int rowIndex) {
    if (rowIndex > 0 && rowIndex < rowCount()) {
      // ������ǰ�к���һ�е�����
      for (int col = 0; col < columnCount() - 1; ++col) {
        QTableWidgetItem *currentItem = takeItem(rowIndex, col);
        QTableWidgetItem *upperItem = takeItem(rowIndex - 1, col);
        setItem(rowIndex - 1, col, currentItem);
        setItem(rowIndex, col, upperItem);
      }
      // ��ȡ�������е�lineedit���󡢻�ȡ��ֵ���������м佻��
      QWidget *currenCell = cellWidget(rowIndex, 2);
      QWidget *upperCell = cellWidget(rowIndex - 1, 2);
      QList<QLineEdit *> currLine = currenCell->findChildren<QLineEdit *>();
      QList<QLineEdit *> upperLine = upperCell->findChildren<QLineEdit *>();
      // ���浱ǰ�еĿ�ʼ������ҳ��
      QString cStart = currLine[0]->text();
      QString cEnd = currLine[1]->text();
      // ���õ�ǰ�е�ֵΪ ��һ�е�ֵ
      currLine[0]->setText(upperLine[0]->text());
      currLine[1]->setText(upperLine[1]->text());
      // ������һ�е�ֵ Ϊ����ĵ��к���ֵ
      upperLine[0]->setText(cStart);
      upperLine[1]->setText(cEnd);
      // ���µ�ǰ������
      setCurrentCell(rowIndex - 1, currentColumn());
    }
  }
  // ������
  void moveRowDown(int rowIndex) {
    // qDebug() << "��ǰ��" << currentRow();
    // qDebug() << "rowIndex" << rowIndex;
    if (rowIndex > -1 && rowIndex < rowCount() - 1) {
      // ������ǰ�к���һ�е�����
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
  // ���Ӳ��ļ��б�
  void AddFile(QString fileName) {
    if (!fileName.isEmpty()) {
      int rowCount = this->rowCount() - 1;
      this->insertRow(this->rowCount());  // ����������

      // ���������ؼ�
      QWidget *containerBtn = new QWidget();
      QHBoxLayout *layoutBtn = new QHBoxLayout(containerBtn);
      layoutBtn->setContentsMargins(0, 0, 0, 0);  // ���ò��ֱ߾�Ϊ0

      // �������� QPushButton �ؼ������ڲ��� �ϡ��¡�ɾ������
      QPushButton *btnDown = new QPushButton("��");
      QPushButton *btnUp = new QPushButton("��");
      QPushButton *btnRemove = new QPushButton("��");
      btnDown->setStyleSheet("min-width: 20px;border-width: 0px;");
      btnUp->setStyleSheet("min-width: 20px;border-width: 0px;");
      btnRemove->setStyleSheet("min-width: 20px;border-width: 0px;");
      btnDown->setFixedSize(20, 20);
      btnUp->setFixedSize(20, 20);
      btnRemove->setFixedSize(20, 20);
      // ��������ť��ӵ������Ĳ�����
      layoutBtn->addWidget(btnDown);
      layoutBtn->addWidget(btnUp);
      layoutBtn->addWidget(btnRemove);

      // ���������ؼ�Ϊ��Ԫ��Ŀؼ�
      rowCount = this->rowCount() - 1;
      //�������ťʱ���õ�ǰ��
      connect(btnDown, SIGNAL(clicked()), this, SLOT(SlotSetCurrRow()));
      // ����һ�в���
      connect(btnDown, &QPushButton::clicked, this,
              [=]() { this->moveRowDown(this->getCurrrow()); });
      connect(btnUp, SIGNAL(clicked()), this, SLOT(SlotSetCurrRow()));
      // ����һ�в���
      connect(btnUp, &QPushButton::clicked, this,
              [=]() { this->moveRowUp(this->getCurrrow()); });
      connect(btnRemove, SIGNAL(clicked()), this, SLOT(SlotSetCurrRow()));
      // ɾ������
      connect(btnRemove, &QPushButton::clicked, this,
              [=]() { this->removeRow(this->getCurrrow()); });

      QWidget *containerEdit = new QWidget();
      QHBoxLayout *layoutEdit = new QHBoxLayout(containerEdit);
      layoutEdit->setContentsMargins(0, 0, 0, 0);  // ���ò��ֱ߾�Ϊ0
      // ��������lineedit���ڱ��� ��ʼ������ҳ��
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
      // У�鿪ʼҳ��
      connect(leStart, &QLineEdit::editingFinished, this, [=]() {
        QString start = leStart->text();
        QString end = leEnd->text();
        QRegularExpression re("\\d+");  // ƥ��һ����������
        QRegularExpressionMatch match = re.match(start);

        if (!(isNumeric(start)) || (start.toInt() <= 0)) {
          leStart->setText("1");
          QMessageBox::information(nullptr, "��ʾ��Ϣ��", "�����Ǵ���0������");
        }
        if (start.toInt() > end.toInt()) {
          QString page = QString::number(getPages(fileName.toStdString()));
          leStart->setText("1");
          leEnd->setText(page);
          QMessageBox::information(nullptr, "��ʾ��Ϣ��",
                                   "����ҳ����Ҫ���ڵ��ڿ�ʼҳ��");
        }
      });
      // У�����ҳ��
      connect(leEnd, &QLineEdit::editingFinished, this, [=]() {
        QString start = leStart->text();
        QString end = leEnd->text();
        QRegularExpression re("\\d+");  // ƥ��һ����������
        QRegularExpressionMatch match = re.match(start);

        if (!(isNumeric(start)) || (start.toInt() <= 0)) {
          leStart->setText("1");
          QMessageBox::information(nullptr, "��ʾ��Ϣ��", "�����Ǵ���0������");
        }
        if (start.toInt() > end.toInt()) {
          QString page = QString::number(getPages(fileName.toStdString()));
          leEnd->setText(page);
          leStart->setText("1");
          QMessageBox::information(nullptr, "��ʾ��Ϣ��",
                                   "����ҳ����Ҫ���ڵ��ڿ�ʼҳ��");
        }
      });

      QTableWidgetItem *itemFile = new QTableWidgetItem(fileName);
      // ���ò��ɱ༭
      itemFile->setFlags(itemFile->flags() & ~Qt::ItemIsEditable);
      QTableWidgetItem *itemPages = new QTableWidgetItem(
          QString::number(getPages(fileName.toStdString())));
      itemPages->setFlags(itemPages->flags() & ~Qt::ItemIsEditable);

      this->setItem(rowCount, 0, itemFile);
      this->setItem(rowCount, 1,
                    itemPages);  // �����������ڵ�һ�еڶ���
      this->setCellWidget(rowCount, 2,
                          containerEdit);  // �����������ڵ�һ�е�����
      this->setCellWidget(rowCount, 3,
                          containerBtn);  // ���������õ�ǰ�� ������
    }
  }

  // ��ȡpushbutton���ڵ���λ�ã�����������Ϊ��ǰ��
  void SlotSetCurrRow() {
    QPushButton *senderButton = qobject_cast<QPushButton *>(QObject::sender());
    if (!senderButton) {
      qDebug() << "��ȡ��ť��������ʧ��:";
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
  // ��ǰ��
  int currRow;
  QString currFile;
};
#endif  // MYTABLE_H
