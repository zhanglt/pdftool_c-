#ifndef PDF2IMAGETHREADSINGLE_H
#define PDF2IMAGETHREADSINGLE_H
#include <qmap.h>

#include <QMutex>
#include <QObject>
#include <QRunnable>
#include <QVector>

#include "qthreadpool.h"
using namespace std;

typedef QMap<QString, int> wMap;  // typedef操作符为QMap起一别名

class pdf2imageThreadSingle : public QObject, public QRunnable {
  Q_OBJECT
 public:
  //   pdf2imageThreadSingle(QMutex *mutex=nullptr,int *count=0);
  // explicit  pdf2imageThreadSingle(QObject *parent = nullptr) :
  // QObject(parent), m_value(0) {}
  explicit pdf2imageThreadSingle(QObject* parent = nullptr);
  void setSourceFile(QString sourceFile);
  void setImagePath(QString imagePath);
  void setTargetFile(QString targetFile);
  int pdf2image(string pdfFile, string imagePath, int resolution);
  void setResolution(int resolution);
  //设置是否启用图片转pdf功能
  void setIs2pdf(bool is2pdf);
  void run() override;
  QMutex m_mutex;
  wMap* m_wmap;
  QThreadPool ImagethreadPool;

 public slots:

 signals:
  void addFinish();

 private:
  int m_id, m_resolution;
  bool m_is2pdf = true;
  QString m_sourceFile, m_imagePath, m_targetFile;
};

#endif  // PDF2IMAGETHREADSINGLE_H
