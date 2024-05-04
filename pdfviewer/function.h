#pragma once
#ifndef FUNCTION_H
#define FUNCTION_H
#include <qobject.h>

#include <QDir>
#include <QTextEdit>

#include "mupdf/fitz.h"
#include "mupdf/pdf.h"
#include "windows.h"
using namespace std;
struct Rectangle {
  int x;       // ���ĵ������
  int y;       // ���ĵ�������
  int width;   // ���ο��
  int height;  // ���θ߶�
};

bool isPDFByExtension(const QString& fileName);
bool isPDFByMagicNumber(const QString& filePath);
bool isImageByExtension(const QString& fileName);
bool isImageByMagicNumber(const QString& filePath);
bool isNumeric(const QString& str);
std::wstring String2WString(const string& str);
std::wstring QString2WString(const QString& str);
//����һ��Ŀ¼������Ŀ¼�е������ļ����������е� PDF �ļ�·����ӵ�����б��С�
void traverseDirectory(const QDir& dir, QStringList& resultList,
                       QString fileType, QString exclude);
QString pathChange(QString rootInput, QString rootOutput, QString file,
                   QString insert);
bool createDirectory(const QString& path);
string extractFileName(const std::string& fullPath);
string extractDirectory(const std::string& fullPath);
// int pdf2image(string pdfFile, string imagePath);
int pdf2image(string pdfFile, string imagePath, int resolution);
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
int mergePdf(std::list<string> fileList, string outFile);
//���������ı�ˮӡ
int addWatermark_multiline(string in, string out, QString mark_txt,
                           QString fontName, int fontSize, QString color,
                           qreal angle, qreal opacity);
int getPageheight(wstring filename);
void getSVGDimensions(const char* filename, int& width, int& height);
// ��ȡһ�����塢�ֺ����ֵĿ��
SIZE getTextWidth(QString text, QString fontName, int fontSize);
double toRadians(double degrees);
// ��һ�����ΰ���һ���Ƕ�angle������ת����������ת�Ժ�x y�Ĳ���ֵ
void rotateRectangle(struct Rectangle& rect, double angle, double& offsetX,
                     double& offsetY);
//�����л���һ����������svg�ļ���������͸���ȡ���ת�ǡ����塢�ֺš���ɫ��������
//����svg��Ŀ������Ϊstamp���뵽Ŀ��pdfҳ�棬��������Ŀ��ҳ��ĵĿ�Ⱥ͸߶ȡ�
void createSVG(QString svgName, QString text, int pageWidth, int pageHeight,
               QString fontName, int fontSize, QString color, qreal angle,
               qreal opacity);
#endif  // FUNCTION
