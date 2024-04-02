#pragma execution_character_set("utf-8")
#include "function.h"

#include <sys/stat.h>

#include <QDebug>
#include <iostream>
#include <string>

#include "include/mark/mark.h"
#include "lib/pdflib.hpp"
#include "mupdf/fitz.h"
#include "mupdf/pdf.h"
//#include "E:/test/qt/win/QtWidgetsApplication1/lib/pdflib.hpp"
#pragma comment(lib, "E:/test/qt/win/QtWidgetsApplication1/lib/pdflib.lib")
using namespace std;
using namespace pdflib;
// image文件转换为pdf文件
int image2pdf(std::string imageFile, std::string pdfFile) {
  PDFlib p;
  const wstring searchpath = L"./PDFlib-CMap-5.0/resource/cmap";
  wostringstream optlist;
  optlist << L"searchpath={{" << searchpath << L"}";
  optlist << L" {" << GetFontsFolder() << L"}}";
  p.set_option(optlist.str());
  // wcout << optlist.str() << endl;

  p.set_info(L"Creator", L"泛生态业务工具集");
  p.set_info(L"Title", L"本文档来自于泛生态业务投标案例");
  // wstring test;
  int image;
  double imagewidth, imageheight;
  try {
    if (p.begin_document(String2WString(pdfFile), L"") == -1) {
      wcerr << L"Error: " << p.get_errmsg() << endl;
      return 2;
    }
    image = p.load_image(L"auto", String2WString(imageFile), L"");
    imagewidth = p.info_image(image, L"width", L"");
    imageheight = p.info_image(image, L"height", L"");
    if (image == 0) {
      // wcerr << L"Error: " <<p.get_errmsg();
    }

    // p.begin_page_ext(1808,2479, L"");
    p.begin_page_ext(imagewidth, imageheight, L"");
    p.fit_image(image, 0, 0, L"");
    p.close_image(image);
    // std::remove(imageFile.c_str());
    p.end_page_ext(L"");

    p.end_document(L"");
    return 0;
  } catch (PDFlib::Exception& ex) {
    wcerr << L"PDFlib 发生异常: " << endl
          << L"[" << ex.get_errnum() << L"] " << ex.get_apiname() << L": "
          << ex.get_errmsg() << endl
          << L": " << L"错误的参数选项请使用 - h参数查看帮助" << endl;
    return 2;
  }
}
/**
 *将图片集合合并到一个PDF文件
 **/
int images2pdf(QStringList& images, std::string pdfFile) {
  PDFlib p;
  const wstring searchpath = L"./PDFlib-CMap-5.0/resource/cmap";
  wostringstream optlist;
  optlist << L"searchpath={{" << searchpath << L"}";
  optlist << L" {" << GetFontsFolder() << L"}}";
  p.set_option(optlist.str());
  // wcout << optlist.str() << endl;
  p.set_info(L"Creator", L"泛生态业务工具集");
  p.set_info(L"Title", L"本文档来自于泛生态业务投标案例");
  int image;
  double imagewidth, imageheight;
  try {
    if (p.begin_document(String2WString(pdfFile), L"") == -1) {
      wcerr << L"Error: " << p.get_errmsg() << endl;
      return 2;
    }
    for (const QString& file : images) {
      image = p.load_image(L"auto", QString2WString(file), L"");
      imagewidth = p.info_image(image, L"width", L"");
      imageheight = p.info_image(image, L"height", L"");
      if (image == 0) {
        wcerr << L"Error: " << p.get_errmsg();
      }
      p.begin_page_ext(imagewidth, imageheight, L"");
      p.fit_image(image, 0, 0, L"");
      p.close_image(image);
      std::remove(file.toUtf8().data());
      p.end_page_ext(L"");
    }
    p.end_document(L"");
    return 0;
  } catch (PDFlib::Exception& ex) {
    wcerr << L"PDFlib 发生异常: " << endl
          << L"[" << ex.get_errnum() << L"] " << ex.get_apiname() << L": "
          << ex.get_errmsg() << endl
          << L": " << L"错误的参数选项请使用 - h参数查看帮助" << endl;
    return 2;
  }
}
/**
 将参数图片目录中的图片文件合并到一个PDF文件（不递归子目录）
imagesDir 图片目录
pdfile 目标文件名称
图片目录中的文件数目。
注意：这是一个特殊函数，图片目录中的文件的名称是0.pdf---(num-1).pdf
**/
int images2pdf(std::string imagesDir, std::string pdfFile, int num) {
  PDFlib p;
  const wstring searchpath = L"./PDFlib-CMap-5.0/resource/cmap";
  wostringstream optlist;
  optlist << L"searchpath={{" << searchpath << L"}";
  optlist << L" {" << GetFontsFolder() << L"}}";
  p.set_option(optlist.str());
  // wcout << optlist.str() << endl;
  p.set_info(L"Creator", L"泛生态业务工具集");
  p.set_info(L"Title", L"本文档来自于泛生态业务投标案例");
  // wstring test;
  int image;
  double imagewidth, imageheight;
  try {
    if (p.begin_document(String2WString(pdfFile), L"") == -1) {
      wcerr << L"Error: " << p.get_errmsg() << endl;
      return 2;
    }
    for (int i = 0; i < num; i++) {
      // 载入图片文件
      image = p.load_image(
          L"auto", String2WString(imagesDir + "/" + to_string(i) + ".png"),
          L"");
      // 获取图片的宽、高
      imagewidth = p.info_image(image, L"width", L"");
      imageheight = p.info_image(image, L"height", L"");
      // if (image == 0){
      //    wcerr << L"Error: " <<p.get_errmsg();
      //}
      // 以图片的尺寸生成pdf页面
      p.begin_page_ext(imagewidth, imageheight, L"");
      p.fit_image(image, 0, 0, L"");
      p.close_image(image);
      //  string tmp=imagesDir+"/"+to_string(i)+".png";
      // cout<<"file:"<<tmp<<endl;
      // qDebug()<<QString::fromStdString(tmp.c_str());
      std::remove((imagesDir + "/" + to_string(i) + ".png").c_str());
      p.end_page_ext(L"");
    }
    p.end_document(L"");
    return 0;
  } catch (PDFlib::Exception& ex) {
    wcerr << L"PDFlib 发生异常: " << endl
          << L"[" << ex.get_errnum() << L"] " << ex.get_apiname() << L": "
          << ex.get_errmsg() << endl
          << L": " << L"错误的参数选项请使用 - h参数查看帮助" << endl;
    return 2;
  }
}

//遍历一个目录及其子目录中的所有文件，并将其中的 type类型的
//文件路径添加到结果列表中。
void traverseDirectory(const QDir& dir, QStringList& resultList,
                       QString fileType, QString exclude) {
  QStringList fileList =
      dir.entryList(QDir::Files);  // 获取当前目录下的文件列表
  foreach (const QString& file, fileList) {
    // 扩展名为大小写".pdf" 并且 不包含"_out_",则认为是需要处理的文件。
    QFileInfo fileInfo(file);
    QString extension = fileInfo.suffix();  // 获取文件扩展名
    // if
    // ((extension.toLower()==fileType)&&(!dir.filePath(file).toLower().contains(exclude)))
    // { // 如果文件是 PDF 文件
    if (extension.toLower() == fileType) {  // 如果文件是 PDF 文件
      // qDebug() << "文件：" << dir.filePath(file);
      resultList.append(dir.filePath(file));  // 将文件路径添加到结果列表中
    }
  }
  QStringList dirList = dir.entryList(
      QDir::Dirs |
      QDir::NoDotAndDotDot);  // 获取当前目录下的子目录列表（排除"."和".."）
  foreach (const QString& subdir, dirList) {
    QDir subdirectory = dir;  // 创建子目录的 QDir 对象
    subdirectory.cd(subdir);  // 切换到子目录
    traverseDirectory(subdirectory, resultList, fileType,
                      exclude);  // 递归遍历子目录
  }
}

//遍历一个目录及其子目录中的所有文件，并将其中的 type类型的
//文件路径添加到结果列表中。
void tDirectory(const QDir& dir, QStringList& resultList,
                const QVariantList& fileTypes) {
  QStringList fileList =
      dir.entryList(QDir::Files);  // 获取当前目录下的文件列表
  foreach (const QString& file, fileList) {
    // 扩展名为大小写".pdf" 并且 不包含"_out_",则认为是需要处理的文件。
    QFileInfo fileInfo(file);
    QString extension = fileInfo.suffix();  // 获取文件扩展名
    // if
    // ((extension.toLower()==fileType)&&(!dir.filePath(file).toLower().contains(exclude)))
    // { // 如果文件是 PDF 文件
    for (const QVariant& fileType : fileTypes) {
      // if (arg.type() == QVariant::String) {
      QString Value = fileType.toString().toLower();
      // qDebug() << Value;                   // 处理字符串参数
      if (extension.toLower() == Value) {  // 如果文件是 PDF 文件
        // qDebug() << "文件：" << dir.filePath(file);
        resultList.append(dir.filePath(file));  // 将文件路径添加到结果列表中
      }
      // }
      // 其他参数类型的处理...
    }
  }
  QStringList dirList = dir.entryList(
      QDir::Dirs |
      QDir::NoDotAndDotDot);  // 获取当前目录下的子目录列表（排除"."和".."）
  foreach (const QString& subdir, dirList) {
    QDir subdirectory = dir;  // 创建子目录的 QDir 对象
    subdirectory.cd(subdir);  // 切换到子目录
    tDirectory(subdirectory, resultList, fileTypes);  // 递归遍历子目录
  }
}

// 将file全路径 中的rootInput更换为rootOut 并在rootOutput之后插入insert字符串。
QString pathChange(QString rootInput, QString rootOutput, QString file,
                   QString insert) {
  // QString root = "c:\\temp";
  // QString filePath = "C:\\temp\\dir1\\dir2\\test.pdf";

  QFileInfo fileInfo(file);             // 使用QFileInfo来获取文件信息
  QString directory = fileInfo.path();  // 获取目录路径
  // qDebug() << "Directory: " << directory;
  QString fileName = fileInfo.fileName();  // 获取文件名
  // qDebug() << "File Name: " << fileName;
  // 截取根目录后的子目录
  QString suffix = directory.right(directory.length() - rootInput.length());
  // qDebug() << "File suffix: " << suffix;
  // 获取完整路径（包括目录路径和文件名）
  // QString absoluteFilePath = fileInfo.absoluteFilePath();
  // qDebug() << "Absolute File Path: " << absoluteFilePath;
  // 组合新目录
  QString newPath = rootOutput + "/" + insert + suffix + "/";

  createDirectory(newPath);
  return newPath + fileName;
}

bool createDirectory(const QString& path) {
  QDir dir(path);
  if (!dir.exists()) {
    if (dir.mkpath(path)) {
      // qDebug() << "Directory created: " << path;
      return true;
    } else {
      qWarning() << "Failed to create directory: " << path;
      return false;
    }
  } else {
    // qDebug() << "Directory already exists: " << path;
    return true;
  }
}
/*
 QString rootPath = QString::fromLocal8Bit("c:/temp");
    QDir rootDir(rootPath);
    // 检查目录是否存在
    if (!rootDir.exists()) {
        qDebug() << "Directory does not exist!";
        return ;
    }
     qDebug() << "Starting directory traversal from: " << rootDir.path();
     traverseDirectory(rootDir, resultList);
     // 输出遍历结果
     qDebug() << "Files and directories:";
     foreach (const QString &entry, resultList) {
         qDebug() << entry;
     }

*/
// 从文件全路径中获取文件名字
std::string extractFileName(const std::string& fullPath) {
  // 找到最后一个路径分隔符的位置
  size_t pos = fullPath.find_last_of("/\\");

  // 提取文件名部分
  if (pos != std::string::npos) {
    return fullPath.substr(pos + 1);
  } else {
    // 如果没有找到路径分隔符，则返回整个路径
    return fullPath;
  }
}
// 从文件全路径中获取文件路径
std::string extractDirectory(const std::string& fullPath) {
  // 找到最后一个路径分隔符的位置
  size_t pos = fullPath.find_last_of("/\\");

  // 提取路径部分
  if (pos != std::string::npos) {
    return fullPath.substr(0, pos);
  } else {
    // 如果没有找到路径分隔符，则返回空字符串
    return "";
  }
}

/*

int pdf2image(string pdfFile,string imagePath){

   // qDebug()<<"imagePath"<<QString::fromStdString(imagePath);
   // qDebug()<<"pdfFile"<<QString::fromStdString(pdfFile);
    std::string sfile(pdfFile);
    fz_context* ctx =  fz_new_context(NULL, NULL, FZ_STORE_DEFAULT);
    fz_register_document_handlers(ctx);
    fz_document* doc =  fz_open_document(ctx, pdfFile.c_str());
    if (!doc) {
        qDebug()<<"文件处理错误："<<QString::fromStdString(pdfFile);
        return 1;
    }
    int num = fz_count_pages(ctx, doc);
   // qDebug()<< "page:" << num << endl;
    float zoom = (float)150 / (float)72;
    fz_matrix ctm = fz_scale(zoom, zoom);

    for(int i=0; i<num; i++)
    {
        //string fileName = imagePath + extractFileName(sfile)+to_string(i) +
".png";
        //string fileName = imagePath+"/"+extractFileName(sfile)+to_string(i) +
".png"; string fileName = imagePath+"/"+to_string(i)+ ".png";
        //qDebug()<< "fileName:" <<QString::fromStdString(fileName);
        fz_pixmap* pix = fz_new_pixmap_from_page_number(ctx, doc, i, ctm,
fz_device_rgb(ctx), 0); fz_save_pixmap_as_png(ctx, pix, fileName.c_str());
        fz_drop_pixmap(ctx, pix);
    }

    fz_drop_document(ctx, doc);
    fz_drop_context(ctx);
    return num;

}

*/
