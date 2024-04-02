#ifndef WATERMARKTHREAD_H
#define WATERMARKTHREAD_H

#include <qmap.h>

#include <QObject>
#include <QRunnable>
#include <QVector>
#include <string>
using namespace std;

typedef QMap<QString, int> wmMap;  // typedef������ΪQMap��һ����

// ���������
class watermarkThread : public QObject, public QRunnable {
  Q_OBJECT
 public:
  explicit watermarkThread(QObject* parent = nullptr);

  void recvNum(int num);
  void setFilename(QString filename);
  void setInputDir(QString input);
  void setOutputDir(QString output);
  void setFiles(QStringList files);
  void setText(QString text);
  void setOpacity(QString opacity);
  void setColor(QString color);
  void setFont(QString font);
  void setFontsize(QString fontsize);
  void setRotate(QString rotate);
  void run() override;

 signals:
  void sendArray(QVector<int> num);
  void addFinish(const wmMap& map);

 private:
  int m_num;
  QString m_filename, m_text = "��ͨ���ֿƼ����޹�˾�ܲ�Ͷ��ר���ĵ�",
                      m_opacity = "15%", m_color = "gray", m_font = "simkai",
                      m_fontsize, m_rotate = "45", m_input, m_output;
  QStringList m_files;
};

#endif  // WATERMARKTHREAD_H
