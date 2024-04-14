#pragma execution_character_set("utf-8")
#include "function.h"

#include <sys/stat.h>

#include <QDebug>
#include <QRegularExpression>
#include <codecvt>
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
bool isPDFByExtension(const QString& fileName) {
  return fileName.toLower().endsWith(".pdf");
}
bool isPDFByMagicNumber(const QString& filePath) {
  QFile file(filePath);
  if (file.open(QIODevice::ReadOnly)) {
    QByteArray magicNumber = file.read(4);
    file.close();
    return magicNumber.startsWith("%PDF");
  }
  return false;
}
bool isImageByExtension(const QString& fileName) {
  QString ext = fileName.toLower().right(4);
  return ext == ".jpg" || ext == "jpeg" || ext == ".png" || ext == ".bmp";
}
bool isImageByMagicNumber(const QString& filePath) {
  QFile file(filePath);
  if (file.open(QIODevice::ReadOnly)) {
    QByteArray magicNumber = file.read(4);
    file.close();
    return magicNumber.startsWith(QByteArray("\xFF\xD8\xFF", 3))         // JPEG
           || magicNumber.startsWith(QByteArray("\x89\x50\x4E\x47", 4))  // PNG
           || magicNumber.startsWith(QByteArray("BM", 2));               // BMP
  }
  return false;
}

bool isNumeric(const QString& str) {
  QRegularExpression re("\\d+");  // 匹配一个或多个数字
  QRegularExpressionMatch match = re.match(str);
  return match.hasMatch() && match.capturedLength() == str.length();
}

std::wstring String2WString(const string& str) {
  wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
  return converter.from_bytes(str);
}
// 将 QString 转换为 wstring
std::wstring QString2WString(const QString& str) {
  std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
  return converter.from_bytes(str.toStdString());
}

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

// 获取pdf文件的页数
int getPages(string pdfFile) {
  std::string sfile(pdfFile);
  fz_context* ctx = fz_new_context(NULL, NULL, FZ_STORE_DEFAULT);
  fz_register_document_handlers(ctx);
  fz_document* doc = fz_open_document(ctx, pdfFile.c_str());
  if (!doc) {
    qDebug() << "文件处理错误：" << QString::fromStdString(pdfFile);
    return 1;
  }
  int num = fz_count_pages(ctx, doc);
  fz_drop_document(ctx, doc);
  fz_drop_context(ctx);
  return num;
}

//将pdf拆分多个文件，subpages是拆分收每一个文件的页数
// in:输入文件
// out:输出文件
// subpages 每个拆分文件的个数
int splitPdf(string in, string out, int subpages) {
  int indoc, page_count;
  wstring infile = String2WString(in);
  wstring outfile_basename = String2WString(out);
  try {
    PDFlib p;
    const wstring searchpath = L"./PDFlib-CMap-5.0/resource/cmap";
    wostringstream optlist;
    optlist << L"searchpath={{" << searchpath << L"}";
    optlist << L" {" << GetFontsFolder() << L"}}";
    p.set_option(optlist.str());
    p.set_info(L"Creator", L"泛生态业务工具集");
    p.set_info(L"Title", L"本文档来自于泛生态业务投标案例");
    indoc = p.open_pdi_document(infile, L"");
    if (indoc == -1) {
      wcerr << L"打开文档错误: " << p.get_errmsg() << endl;
    }
    page_count = (int)p.pcos_get_number(indoc, L"length:pages");
    //计算需要拆分成的文件数
    int outdoc_count =
        page_count / subpages + (page_count % subpages > 0 ? 1 : 0);
    for (int outdoc_counter = 0, page = 0; outdoc_counter < outdoc_count;
         outdoc_counter++) {
      std::wstringstream s1, s0;
      s0 << outdoc_counter;
      s1 << outdoc_counter + 1;
      wstring outfile = outfile_basename + L"_" + s1.str() + L".pdf";

      /*
       * 打开新的子文档.
       */
      if (p.begin_document(outfile, L"") == -1) {
      }
      p.set_info(L"Creator", L"泛生态业务工具集");
      p.set_info(L"Title", L"本文档来自于泛生态业务投标案例");
      p.set_info(L"Subject", L"Sub-document " + s1.str() + L" of " + s0.str() +
                                 L" of input document '" + infile + L"'");
      for (int i = 0; page < page_count && i < subpages; page++, i++) {
        /* Page size may be adjusted by fit_pdi_page() */
        p.begin_page_ext(0, 0, L"width=a4.width height=a4.height");
        int pagehdl = p.open_pdi_page(indoc, page + 1, L"");
        if (pagehdl == -1) {
        }
        /*
         * Place the imported page on the output page, and adjust
         * the page size
         */
        p.fit_pdi_page(pagehdl, 0, 0, L"adjustpage");
        p.close_pdi_page(pagehdl);

        p.end_page_ext(L"");
      }

      /* 关闭子文档 */
      p.end_document(L"");
    }
    p.close_pdi_document(indoc);

  } catch (PDFlib::Exception& ex) {
    wcerr << L"PDFlib 发生异常: " << endl
          << L"[" << ex.get_errnum() << L"] " << ex.get_apiname() << L": "
          << ex.get_errmsg() << endl
          << L": " << L"错误的参数选项请使用 - h参数查看帮助" << endl;
    return 2;
  }
  return 0;
}
// 从pdf总提取start页到end页，为一个新的pdf文件
// in:输入文件
// out: 输出文件
// start:截取的起始页
// end:截取的终止页
int splitPdf(string in, string out, int start, int end) {
  int indoc, page_count;
  wstring infile = String2WString(in);
  wstring outfile_basename = String2WString(out);
  // qDebug() << "filename:" << QString::fromStdWString(outfile_basename);
  // int SUBDOC_PAGES = subpages;
  try {
    PDFlib p;
    const wstring searchpath = L"./PDFlib-CMap-5.0/resource/cmap";
    wostringstream optlist;
    optlist << L"searchpath={{" << searchpath << L"}";
    optlist << L" {" << GetFontsFolder() << L"}}";
    p.set_option(optlist.str());
    p.set_info(L"Creator", L"泛生态业务工具集");
    p.set_info(L"Title", L"本文档来自于泛生态业务投标案例");
    indoc = p.open_pdi_document(infile, L"");
    if (indoc == -1) {
      wcerr << L"打开文档错误: " << p.get_errmsg() << endl;
    }
    page_count = (int)p.pcos_get_number(indoc, L"length:pages");
    wstring outfile = outfile_basename + L"_split.pdf";
    /*
     * 打开子文档
     */
    if (p.begin_document(outfile, L"") == -1) {
    }
    p.set_info(L"Creator", L"泛生态业务工具集");
    p.set_info(L"Title", L"本文档来自于泛生态业务投标案例");
    for (int page = start; page < end; page++) {
      /* Page size may be adjusted by fit_pdi_page() */
      p.begin_page_ext(0, 0, L"width=a4.width height=a4.height");
      int pagehdl = p.open_pdi_page(indoc, page + 1, L"");
      if (pagehdl == -1) {
      }
      /*
       * Place the imported page on the output page, and adjust
       * the page size
       */
      p.fit_pdi_page(pagehdl, 0, 0, L"adjustpage");
      p.close_pdi_page(pagehdl);

      p.end_page_ext(L"");
    }
    /* 关闭子文档 */
    p.end_document(L"");
    p.close_pdi_document(indoc);

  } catch (PDFlib::Exception& ex) {
    wcerr << L"PDFlib 发生异常: " << endl
          << L"[" << ex.get_errnum() << L"] " << ex.get_apiname() << L": "
          << ex.get_errmsg() << endl
          << L": " << L"错误的参数选项请使用 - h参数查看帮助" << endl;
    return 2;
  }
  return 0;
}
//将文件列表fileList中的文件合并成一个PDF文件
// fileList:文件列表
// outFile: 合并后的文件
int mergePdf(std::list<std::string> fileList, string outFile) {
  /* This is where the data files are. Adjust as necessary. */
  wstring searchpath = L"../input";

  /* By default annotations are also imported. In some cases this
   * requires the Noto fonts for creating annotation appearance streams.
   * We therefore set the searchpath to also point to the font directory.
   */
  wstring fontpath = L"../resource/font";
  wstring outfile = String2WString(outFile);
  wstring title = L"开始合并...";

  PDFlib p;

  try {
    PDFlib p;
    const wstring searchpath = L"./PDFlib-CMap-5.0/resource/cmap";
    wostringstream optlist;
    optlist << L"searchpath={{" << searchpath << L"}";
    optlist << L" {" << GetFontsFolder() << L"}}";
    p.set_option(optlist.str());
    p.set_info(L"Creator", L"泛生态业务工具集");
    p.set_info(L"Title", L"本文档来自于泛生态业务投标案例");
    if (p.begin_document(outfile, L"") == -1)
      wcerr << L"Error: " + p.get_errmsg();

    for (const std::string& file : fileList) {
      wstring wfile = String2WString(file);

      int indoc, endpage, pageno, page;
      /* Open the input PDF */
      indoc = p.open_pdi_document(wfile, L"");
      if (indoc == -1) {
        wcerr << L"Error: " + p.get_errmsg();
        continue;
      }

      endpage = (int)p.pcos_get_number(indoc, L"length:pages");

      /* Loop over all pages of the input document */
      for (pageno = 1; pageno <= endpage; pageno++) {
        page = p.open_pdi_page(indoc, pageno, L"");

        if (page == -1) {
          wcerr << L"Error: " + p.get_errmsg();
          continue;
        }
        /* Page size may be adjusted by fit_pdi_page(). */
        p.begin_page_ext(0, 0, L"width=a4.width height=a4.height");

        /* Create a bookmark with the file name */
        if (pageno == 1) p.create_bookmark(wfile, L"");

        /* Place the imported page on the output page, and
         * adjust the page size. If the page contains annotations
         * these are also imported.
         */
        p.fit_pdi_page(page, 0, 0, L"adjustpage");
        p.close_pdi_page(page);

        p.end_page_ext(L"");
      }
      p.close_pdi_document(indoc);
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
