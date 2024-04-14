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
  QRegularExpression re("\\d+");  // ƥ��һ����������
  QRegularExpressionMatch match = re.match(str);
  return match.hasMatch() && match.capturedLength() == str.length();
}

std::wstring String2WString(const string& str) {
  wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
  return converter.from_bytes(str);
}
// �� QString ת��Ϊ wstring
std::wstring QString2WString(const QString& str) {
  std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
  return converter.from_bytes(str.toStdString());
}

// image�ļ�ת��Ϊpdf�ļ�
int image2pdf(std::string imageFile, std::string pdfFile) {
  PDFlib p;
  const wstring searchpath = L"./PDFlib-CMap-5.0/resource/cmap";
  wostringstream optlist;
  optlist << L"searchpath={{" << searchpath << L"}";
  optlist << L" {" << GetFontsFolder() << L"}}";
  p.set_option(optlist.str());
  // wcout << optlist.str() << endl;

  p.set_info(L"Creator", L"����̬ҵ�񹤾߼�");
  p.set_info(L"Title", L"���ĵ������ڷ���̬ҵ��Ͷ�갸��");
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
    wcerr << L"PDFlib �����쳣: " << endl
          << L"[" << ex.get_errnum() << L"] " << ex.get_apiname() << L": "
          << ex.get_errmsg() << endl
          << L": " << L"����Ĳ���ѡ����ʹ�� - h�����鿴����" << endl;
    return 2;
  }
}
/**
 *��ͼƬ���Ϻϲ���һ��PDF�ļ�
 **/
int images2pdf(QStringList& images, std::string pdfFile) {
  PDFlib p;
  const wstring searchpath = L"./PDFlib-CMap-5.0/resource/cmap";
  wostringstream optlist;
  optlist << L"searchpath={{" << searchpath << L"}";
  optlist << L" {" << GetFontsFolder() << L"}}";
  p.set_option(optlist.str());
  // wcout << optlist.str() << endl;
  p.set_info(L"Creator", L"����̬ҵ�񹤾߼�");
  p.set_info(L"Title", L"���ĵ������ڷ���̬ҵ��Ͷ�갸��");
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
    wcerr << L"PDFlib �����쳣: " << endl
          << L"[" << ex.get_errnum() << L"] " << ex.get_apiname() << L": "
          << ex.get_errmsg() << endl
          << L": " << L"����Ĳ���ѡ����ʹ�� - h�����鿴����" << endl;
    return 2;
  }
}
/**
 ������ͼƬĿ¼�е�ͼƬ�ļ��ϲ���һ��PDF�ļ������ݹ���Ŀ¼��
imagesDir ͼƬĿ¼
pdfile Ŀ���ļ�����
ͼƬĿ¼�е��ļ���Ŀ��
ע�⣺����һ�����⺯����ͼƬĿ¼�е��ļ���������0.pdf---(num-1).pdf
**/
int images2pdf(std::string imagesDir, std::string pdfFile, int num) {
  PDFlib p;
  const wstring searchpath = L"./PDFlib-CMap-5.0/resource/cmap";
  wostringstream optlist;
  optlist << L"searchpath={{" << searchpath << L"}";
  optlist << L" {" << GetFontsFolder() << L"}}";
  p.set_option(optlist.str());
  // wcout << optlist.str() << endl;
  p.set_info(L"Creator", L"����̬ҵ�񹤾߼�");
  p.set_info(L"Title", L"���ĵ������ڷ���̬ҵ��Ͷ�갸��");
  // wstring test;
  int image;
  double imagewidth, imageheight;
  try {
    if (p.begin_document(String2WString(pdfFile), L"") == -1) {
      wcerr << L"Error: " << p.get_errmsg() << endl;
      return 2;
    }
    for (int i = 0; i < num; i++) {
      // ����ͼƬ�ļ�
      image = p.load_image(
          L"auto", String2WString(imagesDir + "/" + to_string(i) + ".png"),
          L"");
      // ��ȡͼƬ�Ŀ���
      imagewidth = p.info_image(image, L"width", L"");
      imageheight = p.info_image(image, L"height", L"");
      // if (image == 0){
      //    wcerr << L"Error: " <<p.get_errmsg();
      //}
      // ��ͼƬ�ĳߴ�����pdfҳ��
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
    wcerr << L"PDFlib �����쳣: " << endl
          << L"[" << ex.get_errnum() << L"] " << ex.get_apiname() << L": "
          << ex.get_errmsg() << endl
          << L": " << L"����Ĳ���ѡ����ʹ�� - h�����鿴����" << endl;
    return 2;
  }
}

//����һ��Ŀ¼������Ŀ¼�е������ļ����������е� type���͵�
//�ļ�·����ӵ�����б��С�
void traverseDirectory(const QDir& dir, QStringList& resultList,
                       QString fileType, QString exclude) {
  QStringList fileList =
      dir.entryList(QDir::Files);  // ��ȡ��ǰĿ¼�µ��ļ��б�
  foreach (const QString& file, fileList) {
    // ��չ��Ϊ��Сд".pdf" ���� ������"_out_",����Ϊ����Ҫ������ļ���
    QFileInfo fileInfo(file);
    QString extension = fileInfo.suffix();  // ��ȡ�ļ���չ��
    // if
    // ((extension.toLower()==fileType)&&(!dir.filePath(file).toLower().contains(exclude)))
    // { // ����ļ��� PDF �ļ�
    if (extension.toLower() == fileType) {  // ����ļ��� PDF �ļ�
      // qDebug() << "�ļ���" << dir.filePath(file);
      resultList.append(dir.filePath(file));  // ���ļ�·����ӵ�����б���
    }
  }
  QStringList dirList = dir.entryList(
      QDir::Dirs |
      QDir::NoDotAndDotDot);  // ��ȡ��ǰĿ¼�µ���Ŀ¼�б��ų�"."��".."��
  foreach (const QString& subdir, dirList) {
    QDir subdirectory = dir;  // ������Ŀ¼�� QDir ����
    subdirectory.cd(subdir);  // �л�����Ŀ¼
    traverseDirectory(subdirectory, resultList, fileType,
                      exclude);  // �ݹ������Ŀ¼
  }
}

//����һ��Ŀ¼������Ŀ¼�е������ļ����������е� type���͵�
//�ļ�·����ӵ�����б��С�
void tDirectory(const QDir& dir, QStringList& resultList,
                const QVariantList& fileTypes) {
  QStringList fileList =
      dir.entryList(QDir::Files);  // ��ȡ��ǰĿ¼�µ��ļ��б�
  foreach (const QString& file, fileList) {
    // ��չ��Ϊ��Сд".pdf" ���� ������"_out_",����Ϊ����Ҫ������ļ���
    QFileInfo fileInfo(file);
    QString extension = fileInfo.suffix();  // ��ȡ�ļ���չ��
    // if
    // ((extension.toLower()==fileType)&&(!dir.filePath(file).toLower().contains(exclude)))
    // { // ����ļ��� PDF �ļ�
    for (const QVariant& fileType : fileTypes) {
      // if (arg.type() == QVariant::String) {
      QString Value = fileType.toString().toLower();
      // qDebug() << Value;                   // �����ַ�������
      if (extension.toLower() == Value) {  // ����ļ��� PDF �ļ�
        // qDebug() << "�ļ���" << dir.filePath(file);
        resultList.append(dir.filePath(file));  // ���ļ�·����ӵ�����б���
      }
      // }
      // �����������͵Ĵ���...
    }
  }
  QStringList dirList = dir.entryList(
      QDir::Dirs |
      QDir::NoDotAndDotDot);  // ��ȡ��ǰĿ¼�µ���Ŀ¼�б��ų�"."��".."��
  foreach (const QString& subdir, dirList) {
    QDir subdirectory = dir;  // ������Ŀ¼�� QDir ����
    subdirectory.cd(subdir);  // �л�����Ŀ¼
    tDirectory(subdirectory, resultList, fileTypes);  // �ݹ������Ŀ¼
  }
}

// ��fileȫ·�� �е�rootInput����ΪrootOut ����rootOutput֮�����insert�ַ�����
QString pathChange(QString rootInput, QString rootOutput, QString file,
                   QString insert) {
  // QString root = "c:\\temp";
  // QString filePath = "C:\\temp\\dir1\\dir2\\test.pdf";

  QFileInfo fileInfo(file);             // ʹ��QFileInfo����ȡ�ļ���Ϣ
  QString directory = fileInfo.path();  // ��ȡĿ¼·��
  // qDebug() << "Directory: " << directory;
  QString fileName = fileInfo.fileName();  // ��ȡ�ļ���
  // qDebug() << "File Name: " << fileName;
  // ��ȡ��Ŀ¼�����Ŀ¼
  QString suffix = directory.right(directory.length() - rootInput.length());
  // qDebug() << "File suffix: " << suffix;
  // ��ȡ����·��������Ŀ¼·�����ļ�����
  // QString absoluteFilePath = fileInfo.absoluteFilePath();
  // qDebug() << "Absolute File Path: " << absoluteFilePath;
  // �����Ŀ¼
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
    // ���Ŀ¼�Ƿ����
    if (!rootDir.exists()) {
        qDebug() << "Directory does not exist!";
        return ;
    }
     qDebug() << "Starting directory traversal from: " << rootDir.path();
     traverseDirectory(rootDir, resultList);
     // ����������
     qDebug() << "Files and directories:";
     foreach (const QString &entry, resultList) {
         qDebug() << entry;
     }

*/
// ���ļ�ȫ·���л�ȡ�ļ�����
std::string extractFileName(const std::string& fullPath) {
  // �ҵ����һ��·���ָ�����λ��
  size_t pos = fullPath.find_last_of("/\\");

  // ��ȡ�ļ�������
  if (pos != std::string::npos) {
    return fullPath.substr(pos + 1);
  } else {
    // ���û���ҵ�·���ָ������򷵻�����·��
    return fullPath;
  }
}
// ���ļ�ȫ·���л�ȡ�ļ�·��
std::string extractDirectory(const std::string& fullPath) {
  // �ҵ����һ��·���ָ�����λ��
  size_t pos = fullPath.find_last_of("/\\");

  // ��ȡ·������
  if (pos != std::string::npos) {
    return fullPath.substr(0, pos);
  } else {
    // ���û���ҵ�·���ָ������򷵻ؿ��ַ���
    return "";
  }
}

// ��ȡpdf�ļ���ҳ��
int getPages(string pdfFile) {
  std::string sfile(pdfFile);
  fz_context* ctx = fz_new_context(NULL, NULL, FZ_STORE_DEFAULT);
  fz_register_document_handlers(ctx);
  fz_document* doc = fz_open_document(ctx, pdfFile.c_str());
  if (!doc) {
    qDebug() << "�ļ��������" << QString::fromStdString(pdfFile);
    return 1;
  }
  int num = fz_count_pages(ctx, doc);
  fz_drop_document(ctx, doc);
  fz_drop_context(ctx);
  return num;
}

//��pdf��ֶ���ļ���subpages�ǲ����ÿһ���ļ���ҳ��
// in:�����ļ�
// out:����ļ�
// subpages ÿ������ļ��ĸ���
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
    p.set_info(L"Creator", L"����̬ҵ�񹤾߼�");
    p.set_info(L"Title", L"���ĵ������ڷ���̬ҵ��Ͷ�갸��");
    indoc = p.open_pdi_document(infile, L"");
    if (indoc == -1) {
      wcerr << L"���ĵ�����: " << p.get_errmsg() << endl;
    }
    page_count = (int)p.pcos_get_number(indoc, L"length:pages");
    //������Ҫ��ֳɵ��ļ���
    int outdoc_count =
        page_count / subpages + (page_count % subpages > 0 ? 1 : 0);
    for (int outdoc_counter = 0, page = 0; outdoc_counter < outdoc_count;
         outdoc_counter++) {
      std::wstringstream s1, s0;
      s0 << outdoc_counter;
      s1 << outdoc_counter + 1;
      wstring outfile = outfile_basename + L"_" + s1.str() + L".pdf";

      /*
       * ���µ����ĵ�.
       */
      if (p.begin_document(outfile, L"") == -1) {
      }
      p.set_info(L"Creator", L"����̬ҵ�񹤾߼�");
      p.set_info(L"Title", L"���ĵ������ڷ���̬ҵ��Ͷ�갸��");
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

      /* �ر����ĵ� */
      p.end_document(L"");
    }
    p.close_pdi_document(indoc);

  } catch (PDFlib::Exception& ex) {
    wcerr << L"PDFlib �����쳣: " << endl
          << L"[" << ex.get_errnum() << L"] " << ex.get_apiname() << L": "
          << ex.get_errmsg() << endl
          << L": " << L"����Ĳ���ѡ����ʹ�� - h�����鿴����" << endl;
    return 2;
  }
  return 0;
}
// ��pdf����ȡstartҳ��endҳ��Ϊһ���µ�pdf�ļ�
// in:�����ļ�
// out: ����ļ�
// start:��ȡ����ʼҳ
// end:��ȡ����ֹҳ
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
    p.set_info(L"Creator", L"����̬ҵ�񹤾߼�");
    p.set_info(L"Title", L"���ĵ������ڷ���̬ҵ��Ͷ�갸��");
    indoc = p.open_pdi_document(infile, L"");
    if (indoc == -1) {
      wcerr << L"���ĵ�����: " << p.get_errmsg() << endl;
    }
    page_count = (int)p.pcos_get_number(indoc, L"length:pages");
    wstring outfile = outfile_basename + L"_split.pdf";
    /*
     * �����ĵ�
     */
    if (p.begin_document(outfile, L"") == -1) {
    }
    p.set_info(L"Creator", L"����̬ҵ�񹤾߼�");
    p.set_info(L"Title", L"���ĵ������ڷ���̬ҵ��Ͷ�갸��");
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
    /* �ر����ĵ� */
    p.end_document(L"");
    p.close_pdi_document(indoc);

  } catch (PDFlib::Exception& ex) {
    wcerr << L"PDFlib �����쳣: " << endl
          << L"[" << ex.get_errnum() << L"] " << ex.get_apiname() << L": "
          << ex.get_errmsg() << endl
          << L": " << L"����Ĳ���ѡ����ʹ�� - h�����鿴����" << endl;
    return 2;
  }
  return 0;
}
//���ļ��б�fileList�е��ļ��ϲ���һ��PDF�ļ�
// fileList:�ļ��б�
// outFile: �ϲ�����ļ�
int mergePdf(std::list<std::string> fileList, string outFile) {
  /* This is where the data files are. Adjust as necessary. */
  wstring searchpath = L"../input";

  /* By default annotations are also imported. In some cases this
   * requires the Noto fonts for creating annotation appearance streams.
   * We therefore set the searchpath to also point to the font directory.
   */
  wstring fontpath = L"../resource/font";
  wstring outfile = String2WString(outFile);
  wstring title = L"��ʼ�ϲ�...";

  PDFlib p;

  try {
    PDFlib p;
    const wstring searchpath = L"./PDFlib-CMap-5.0/resource/cmap";
    wostringstream optlist;
    optlist << L"searchpath={{" << searchpath << L"}";
    optlist << L" {" << GetFontsFolder() << L"}}";
    p.set_option(optlist.str());
    p.set_info(L"Creator", L"����̬ҵ�񹤾߼�");
    p.set_info(L"Title", L"���ĵ������ڷ���̬ҵ��Ͷ�갸��");
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
    wcerr << L"PDFlib �����쳣: " << endl
          << L"[" << ex.get_errnum() << L"] " << ex.get_apiname() << L": "
          << ex.get_errmsg() << endl
          << L": " << L"����Ĳ���ѡ����ʹ�� - h�����鿴����" << endl;
    return 2;
  }
}
