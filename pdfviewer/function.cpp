#include "qmath.h"
#include "qpainter.h"
#include "qsvggenerator.h"
#pragma execution_character_set("utf-8")
#include <sys/stat.h>
#include <windows.h>

#include <QDebug>
#include <QRegularExpression>
#include <QSvgRenderer>
#include <QTextEdit>
#include <cmath>
#include <codecvt>
#include <iostream>
#include <string>

#include "function.h"
#include "include/mark/mark.h"
#include "lib/pdflib.hpp"
#include "libxml/parser.h"
#include "libxml/tree.h"
#include "mupdf/fitz.h"
#include "mupdf/pdf.h"

//#include "E:/test/qt/win/QtWidgetsApplication1/lib/pdflib.hpp"
#pragma comment(lib, "E:/test/qt/win/QtWidgetsApplication1/lib/pdflib.lib")
#pragma comment(lib, "Gdi32.lib")
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
// pdf转为图片文件
int pdf2image(string pdfFile, string imagePath, int resolution) {
  fz_context* ctx = fz_new_context(NULL, NULL, FZ_STORE_DEFAULT);
  fz_register_document_handlers(ctx);
  fz_document* doc = fz_open_document(ctx, pdfFile.c_str());
  if (!doc) {
    qDebug() << "文件处理错误：" << QString::fromStdString(pdfFile);
    return 1;
  }
  int num = fz_count_pages(ctx, doc);
  // qDebug()<< "page:" << num << endl;
  float zoom = (float)resolution / (float)72;
  fz_matrix ctm = fz_scale(zoom, zoom);

  for (int i = 0; i < num; i++) {
    string fileName = imagePath + "/" + to_string(i) + ".png";
    fz_pixmap* pix =
        fz_new_pixmap_from_page_number(ctx, doc, i, ctm, fz_device_rgb(ctx), 0);
    fz_save_pixmap_as_png(ctx, pix, fileName.c_str());
    fz_drop_pixmap(ctx, pix);
  }

  fz_drop_document(ctx, doc);
  fz_drop_context(ctx);
  return num;
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
    //  // 如果文件是 PDF 文件
    if ((extension.toLower() == fileType) &&
        (!dir.filePath(file).contains(exclude)) &&
        (!dir.filePath(file).contains("_pdf_"))) {  // 如果文件是 PDF 文件
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

// 获取页面高度
int getPageheight(wstring filename) {
  PDFlib p;
  try {
    //打开源pdf文件获取实例id
    // pageName = String2WString(char2string(optarg));
    int doc = p.open_pdi_document(filename, L"");
    if (doc == -1) {
      cout << "打开源pdf文件失败!" << endl;
      return 0;
    }
    // auto fWidth = (p.pcos_get_number(doc, L"pages[0]/width"));  //宽度595
    auto fHeight = p.pcos_get_number(doc, L"pages[0]/height");  //高度842

    //		m_oPDF.close_font(font);
    p.close_pdi_document(doc);
    return fHeight;
  } catch (PDFlib::Exception& e) {
    //输出pdflib异常信息
    wprintf(p.get_errmsg().c_str());
    return 0;
  } catch (...) {
    //其他异常
    return 0;
  }
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
  int indoc;
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
    // int page_count = (int)p.pcos_get_number(indoc, L"length:pages");
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
void getSVGDimensions(const char* filename, int& width, int& height) {
  xmlDocPtr doc;
  xmlNodePtr rootNode;

  // Parse the SVG file
  doc = xmlReadFile(filename, nullptr, 0);
  if (doc == nullptr) {
    std::cerr << "Error: Unable to parse SVG file." << std::endl;
    return;
  }

  // Get the root node
  rootNode = xmlDocGetRootElement(doc);
  if (rootNode == nullptr || xmlStrcmp(rootNode->name, (const xmlChar*)"svg")) {
    std::cerr << "Error: Invalid SVG file format." << std::endl;
    xmlFreeDoc(doc);
    return;
  }

  // Extract width and height attributes from <svg> element
  const xmlChar* widthStr = xmlGetProp(rootNode, (const xmlChar*)"width");
  const xmlChar* heightStr = xmlGetProp(rootNode, (const xmlChar*)"height");

  if (widthStr == nullptr || heightStr == nullptr) {
    std::cerr << "Error: SVG file missing width or height attribute."
              << std::endl;
    xmlFreeDoc(doc);
    return;
  }

  // Convert width and height strings to integers
  width = atoi((const char*)widthStr);
  height = atoi((const char*)heightStr);

  // Free memory
  xmlFreeDoc(doc);
}
//创建多行文本水印
int addWatermark_multiline(string inFile, string outFile, QString mark_txt,
                           QString fontName, int fontSize, QString color,
                           qreal angle, qreal opacity) {
  /* This is where the data files are. Adjust as necessary. */

  const wstring outfile = String2WString(outFile);
  const wstring textline = L"Giant Wing Paper Plane";
  wstring pdffile = String2WString(inFile);
  try {
    PDFlib p;
    //  This means we must check return values of load_font() etc.
    p.set_option(L"errorpolicy=return");
    const wstring searchpath = L"./PDFlib-CMap-5.0/resource/cmap";
    wostringstream optlist;
    optlist << L"searchpath={{" << searchpath << L"}";
    optlist << L" {" << GetFontsFolder() << L"}}";
    p.set_option(optlist.str());
    p.set_option(L"errorpolicy=return");
    // p.set_option(L"stringformat=utf8");

    /* Set an output path according to the name of the topic */
    if (p.begin_document(outfile, L"") == 0) {
      qDebug() << L"Error: " << p.get_errmsg() << endl;
      return 2;
    }
    /* Open the input PDF */
    // qDebug() << "pdf name:" << pdffile;
    int indoc = p.open_pdi_document(pdffile, L"");
    if (indoc != 0) {
      qDebug() << "error----------------indoc == 0";
    }

    int endpage = (int)p.pcos_get_number(indoc, L"length:pages");
    wstring graphicsfile = pdffile + L".svg";
    int w = 0, h = 0;
    // 设置字体和文本
    QFont font(fontName, fontSize);
    QStringList textLines = mark_txt.split("\n");
    // 创建一个 QFontMetrics 对象，用于获取文本尺寸
    QFontMetrics fontMetrics(font);
    //获取字体的高度
    int lineHeight = fontMetrics.height();
    int maxWidth = 0;
    // 遍历文本各行，获取最长值
    foreach (const QString& line, textLines) {
      int lineWidth = getTextWidth(line, fontName, fontSize).cx;
      if (lineWidth > maxWidth) {
        maxWidth = lineWidth;
      }
    }

    int maxHeight = lineHeight * textLines.size();  //单行高度 乘 行数
    /* Loop over all pages of the input document */
    for (int pageno = 1; pageno <= endpage; pageno++) {
      int page = p.open_pdi_page(indoc, pageno, L"");
      if (page != 0) {
        qDebug() << "error----------------page == 0";
      }
      //页面宽度:
      int pageWidth =
          p.pcos_get_number(indoc,
                            L"pages[" + String2WString(to_string(pageno - 1)) +
                                L"]/width");  //宽度595
      //页面高度：
      int pageHeigth =
          p.pcos_get_number(indoc,
                            L"pages[" + String2WString(to_string(pageno - 1)) +
                                L"]/height");  //高度842

      struct Rectangle rect = {pageWidth / 2 + 50,  // 50魔术补偿值
                               pageHeigth / 2 + maxHeight / 2, maxWidth,
                               maxHeight};
      double offsetX, offsetY;  //旋转后 xy的补偿值
      rotateRectangle(rect, angle, offsetX, offsetY);
      if ((pageWidth != w) || (pageHeigth != h)) {
        createSVG(QString::fromStdString(inFile) + ".svg", mark_txt, pageWidth,
                  pageHeigth, fontName, fontSize, color, angle, opacity);
        w = pageWidth;
        h = pageHeigth;
      }

      p.begin_page_ext(0, 0, L"width=a4.width height=a4.height");

      /* Place the imported page on the output page, and
       * adjust the page size
       */
      p.fit_pdi_page(page, 0, 0, L"adjustpage");
      // p.set_option(L"usercoordinates=true");
      // Create template for use as annotation icon
      p.begin_template_ext(pageWidth, pageHeigth, L"topdown=true");
      /* Load SVG graphics */
      int gs = p.load_graphics(L"auto", graphicsfile, L"");
      /* Place SVG in the lower left corner of the template
       p.fit_graphics(gs, 300, 10,*/
      p.fit_graphics(gs, pageWidth / 2 - offsetX / 2,
                     pageHeigth / 2 - offsetY / 2,
                     L" scale=1 position={center} "
                     L"fitmethod=auto showborder");
      p.close_graphics(gs);
      p.end_template_ext(0, 0);
      // Create the annotation with template for "normal" appearance
      p.create_annotation(0, 0, pageWidth, pageHeigth, L"Stamp",
                          L"template={normal=1} ");
      p.close_pdi_page(page);

      p.end_page_ext(L"");
      // p.close_image(image);
    }

    p.end_document(L"");
    string svg = inFile + ".svg";
    std::remove(svg.c_str());
  } catch (PDFlib::Exception& ex) {
    wcerr << L"PDFlib exception occurred:" << endl
          << L"[" << ex.get_errnum() << L"] " << ex.get_apiname() << L": "
          << ex.get_errmsg() << endl;
    return 99;
  } catch (exception& e) {
    cerr << "C++ exception occurred: " << e.what() << endl;
    return 99;
  } catch (...) {
    cerr << "Generic C++ exception occurred!" << endl;
    return 99;
  }

  return 0;
}
// 获取一段文字的宽度
SIZE getTextWidth(QString text, QString fontName, int fontSize) {
  // 创建设备上下文
  HDC hdc = CreateCompatibleDC(NULL);
  if (hdc == NULL) {
    qDebug() << "无法创建设备上下文！" << endl;
  }
  // 将 QString 转换为 wchar_t 数组
  std::vector<wchar_t> wcharArray(fontName.length() + 1);
  fontName.toWCharArray(wcharArray.data());

  // 获取 wchar_t 数组指针
  wchar_t* wcharPtr_fontname = wcharArray.data();
  // 输出 wchar_t 数组内容
  while (*wcharPtr_fontname) {
    std::wcout << *wcharPtr_fontname;
    wcharPtr_fontname++;
  }
  // 选择字体
  HFONT hFont = CreateFont(fontSize, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                           DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                           CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                           DEFAULT_PITCH | FF_DONTCARE, wcharPtr_fontname);
  if (hFont == NULL) {
    qDebug() << "无法创建字体！" << endl;
    DeleteDC(hdc);
  }
  // 选择字体到设备上下文
  SelectObject(hdc, hFont);
  // 设置文本
  // 获取文本的宽度
  // 将 QString 转换为 std::wstring
  std::wstring wtext = text.toStdWString();
  // 获取 LPCWSTR 指针
  LPCWSTR lpcwstr = wtext.c_str();

  SIZE size;
  if (!GetTextExtentPoint32(hdc, lpcwstr, wcslen(lpcwstr), &size)) {
    qDebug() << "无法获取文本尺寸！" << endl;
    DeleteObject(hFont);
    DeleteDC(hdc);
  }
  // 输出宽度
  // qDebug() << "文本宽度为：" << size.cx << "像素" << endl;
  // 释放资源
  DeleteObject(hFont);
  DeleteDC(hdc);
  return size;
}
// 定义角度转弧度的函数
double toRadians(double degrees) { return degrees * M_PI / 180.0; }
// 对一个矩形按照一定角度angle进行旋转，并测算旋转以后x y的补偿值
void rotateRectangle(struct Rectangle& rect, double angle, double& offsetX,
                     double& offsetY) {
  // 计算矩形中心点坐标
  double centerX = rect.x;
  double centerY = rect.y;

  // 将角度转换为弧度
  double radians = toRadians(angle);
  // 计算旋转后的各个顶点坐标
  double cosAngle = cos(radians);
  double sinAngle = sin(radians);
  double halfWidth = rect.width / 2.0;
  double halfHeight = rect.height / 2.0;
  // 计算旋转后的顶点坐标
  // double rotatedX1 = centerX + halfWidth * cosAngle - halfHeight * sinAngle;
  // double rotatedY1 = centerY + halfWidth * sinAngle + halfHeight * cosAngle;
  double rotatedX2 = centerX - halfWidth * cosAngle - halfHeight * sinAngle;
  double rotatedY2 = centerY - halfWidth * sinAngle + halfHeight * cosAngle;
  double rotatedX3 = centerX - halfWidth * cosAngle + halfHeight * sinAngle;
  double rotatedY3 = centerY - halfWidth * sinAngle - halfHeight * cosAngle;
  // double rotatedX4 = centerX + halfWidth * cosAngle + halfHeight * sinAngle;
  // double rotatedY4 = centerY + halfWidth * sinAngle - halfHeight * cosAngle;

  // 输出旋转后的顶点坐标
  // qDebug() << "旋转后的顶点坐标：";
  // qDebug() << "顶点1：(" << rotatedX1 << ", " << rotatedY1 << ")";
  // qDebug() << "顶点2：(" << rotatedX2 << ", " << rotatedY2 << ")";
  // qDebug() << "顶点3：(" << rotatedX3 << ", " << rotatedY3 << ")";
  // qDebug() << "顶点4：(" << rotatedX4 << ", " << rotatedY4 << ")";
  // qDebug() << "顶点4：(" << rotatedX4 << ", " << rotatedY4 << ")";
  offsetX = rotatedX3 - rotatedX2;
  offsetY = rotatedY2 - rotatedY3;
}

void createSVG(QString svgName, QString text, int pageWidth, int pageHeight,
               QString fontName, int fontSize, QString color, qreal angle,
               qreal opacity) {
  // 设置字体和文本
  QFont font(fontName, fontSize);
  QStringList textLines = text.split("\n");
  // 创建一个 QFontMetrics 对象，用于获取文本尺寸
  QFontMetrics fontMetrics(font);
  int lineHeight = fontMetrics.height();
  int maxWidth = 0;
  // 计算每行文本的宽度和高度
  foreach (const QString& line, textLines) {
    int lineWidth = getTextWidth(line, fontName, fontSize).cx;
    if (lineWidth > maxWidth) {
      maxWidth = lineWidth;
    }
  }
  // 旋转角度
  int h = lineHeight * textLines.size();  //单行高度 乘 行数
  int xMax = qMax(maxWidth, pageWidth);   //文字宽度
  int yMax = pageHeight;                  //页面高度
  QSize rotatedTextBlockSize(xMax, yMax);
  QSvgGenerator generator;
  generator.setFileName(svgName);
  generator.setSize(rotatedTextBlockSize);        // 设置大小
  generator.setViewBox(QRect(0, 0, xMax, yMax));  // 视口大小
  // 创建绘图设备
  QPainter painter;

  painter.begin(&generator);
  painter.setPen(QColor(color));  //设置文字颜色
  painter.setFont(font);
  QPoint c = QPoint(
      xMax / 2 + 50,
      yMax / 2 +
          h / 2);  //页面宽度一半+增量（50），页面高度的一半+文字高度的一半
  painter.translate(c);   //坐标平移
  painter.rotate(angle);  //旋转角度
  painter.translate(-c);
  painter.setOpacity(opacity);  //文字透明度
  for (int i = 0; i < textLines.size(); ++i) {
    QPoint textPosition(
        0,
        yMax / 2 + i * lineHeight);  //当文字旋转时，纵坐标要做相关调整
    painter.drawText(
        QRect(textPosition, QSize(rotatedTextBlockSize.width(), lineHeight)),
        Qt::AlignLeft | Qt::AlignVCenter, textLines.at(i));
  }
  // 结束绘图
  painter.end();
}
