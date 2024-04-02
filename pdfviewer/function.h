#pragma once
#ifndef FUNCTION_H
#define FUNCTION_H
#include <qobject.h>

#include <QDir>

#include "mupdf/fitz.h"
#include "mupdf/pdf.h"
using namespace std;
//遍历一个目录及其子目录中的所有文件，并将其中的 PDF 文件路径添加到结果列表中。
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
#endif  // FUNCTION
