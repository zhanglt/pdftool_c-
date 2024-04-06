#pragma once
#ifndef FUNCTION_H
#define FUNCTION_H
#include <qobject.h>

#include <QDir>

#include "mupdf/fitz.h"
#include "mupdf/pdf.h"
using namespace std;
//����һ��Ŀ¼������Ŀ¼�е������ļ����������е� PDF �ļ�·����ӵ�����б��С�
void traverseDirectory(const QDir& dir, QStringList& resultList,
                       QString fileType, QString exclude);
QString pathChange(QString rootInput, QString rootOutput, QString file,
                   QString insert);
bool createDirectory(const QString& path);
string extractFileName(const std::string& fullPath);
string extractDirectory(const std::string& fullPath);
int pdf2image(string pdfFile, string imagePath);

int image2pdf(std::string imageFile, std::string pdfFile);
int images2pdf(QStringList& images, std::string pdfFile);
int images2pdf(std::string imagesDir, std::string pdfFile, int num);
void tDirectory(const QDir& dir, QStringList& resultList,
                const QVariantList& fileTypes);
int getPages(string pdfFile);
//��pdf��ֶ���ļ���subpages�ǲ����ÿһ���ļ���ҳ��
// in:�����ļ�
// out:����ļ�
// subpages ÿ������ļ��ĸ���
int splitPdf(string in, string out, int subpages);
// ��pdf����ȡstartҳ��endҳ��Ϊһ���µ�pdf�ļ�
// in:�����ļ�
// out: ����ļ�
// start:��ȡ����ʼҳ
// end:��ȡ����ֹҳ
int splitPdf(string in, string out, int start, int end);
#endif  // FUNCTION
