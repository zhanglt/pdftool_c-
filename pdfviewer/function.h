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
  int x;       // 中心点横坐标
  int y;       // 中心点纵坐标
  int width;   // 矩形宽度
  int height;  // 矩形高度
};

bool isPDFByExtension(const QString& fileName);
bool isPDFByMagicNumber(const QString& filePath);
bool isImageByExtension(const QString& fileName);
bool isImageByMagicNumber(const QString& filePath);
bool isNumeric(const QString& str);
std::wstring String2WString(const string& str);
std::wstring QString2WString(const QString& str);
//遍历一个目录及其子目录中的所有文件，并将其中的 PDF 文件路径添加到结果列表中。
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
//将pdf拆分多个文件，subpages是拆分收每一个文件的页数
// in:输入文件
// out:输出文件
// subpages 每个拆分文件的个数
int splitPdf(string in, string out, int subpages);
// 从pdf总提取start页到end页，为一个新的pdf文件
// in:输入文件
// out: 输出文件
// start:截取的起始页
// end:截取的终止页
int splitPdf(string in, string out, int start, int end);
int mergePdf(std::list<string> fileList, string outFile);
//创建多行文本水印
int addWatermark_multiline(string in, string out, QString mark_txt,
                           QString fontName, int fontSize, QString color,
                           qreal angle, qreal opacity);
int getPageheight(wstring filename);
void getSVGDimensions(const char* filename, int& width, int& height);
// 获取一定字体、字号文字的宽度
SIZE getTextWidth(QString text, QString fontName, int fontSize);
double toRadians(double degrees);
// 对一个矩形按照一定角度angle进行旋转，并测算旋转以后x y的补偿值
void rotateRectangle(struct Rectangle& rect, double angle, double& offsetX,
                     double& offsetY);
//根据行或者一段文字生成svg文件，并设置透明度、旋转角、字体、字号、颜色、等属性
//生成svg的目的是作为stamp插入到目标pdf页面，所以输入目标页面的的宽度和高度。
void createSVG(QString svgName, QString text, int pageWidth, int pageHeight,
               QString fontName, int fontSize, QString color, qreal angle,
               qreal opacity);
#endif  // FUNCTION
