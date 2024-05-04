//#pragma execution_character_set("utf-8")
//#include "../../stdafx.h"
#include <Windows.h>
#include <fcntl.h>
#include <io.h>
#include <sysinfoapi.h>

#include <codecvt>
#include <iostream>
#include <locale>
#include <string>

#include "include/mark/mark.h"
#include "lib/pdflib.hpp"

using namespace std;
using namespace pdflib;
#include <QDebug>

#include "function.h"
#include "lib/pdflib.hpp"
//#include "E:/test/qt/win/QtWidgetsApplication1/lib/pdflib.hpp"
#pragma comment(lib, "E:/test/qt/win/QtWidgetsApplication1/lib/pdflib.lib")

/*
std::wstring String2WString(const std::string& s)
{
        std::string strLocale = setlocale(LC_ALL, "");
        const char* chSrc = s.c_str();
        size_t nDestSize = mbstowcs(NULL, chSrc, 0) + 1;
        wchar_t* wchDest = new wchar_t[nDestSize];
        wmemset(wchDest, 0, nDestSize);
        mbstowcs(wchDest, chSrc, nDestSize);
        std::wstring wstrResult = wchDest;
        delete[] wchDest;
        setlocale(LC_ALL, strLocale.c_str());
        return wstrResult;
}
*/
std::wstring GetFontsFolder() {
  char buffer[MAX_PATH + 1];
  auto result = GetWindowsDirectoryA(buffer, MAX_PATH);
  if (result) {
    return String2WString(std::string(buffer).append("\\fonts"));
  }
  return L"c:/windows/fonts";  // or however you want to handle the error
}
std::string GetTmpFolder() {
  char buffer[MAX_PATH + 1];
  auto result = GetWindowsDirectoryA(buffer, MAX_PATH);
  if (result) {
    return std::string(buffer).append("\\temp");
  }
  return "c:/windows/temp";  // or however you want to handle the error
}

std::string GetFilename(string path) {
  string::size_type iPos = path.find_last_of('\\') + 1;
  string filename = path.substr(iPos, path.length() - iPos);
  return filename;
}
std::string char2string(char* c) {
  string str = c;
  return str;
}

/*
int main(int argc, char* argv[]) {
        GetOpt_pp ops(argc, argv);
        std::string infile = ops.getopt<std::string>('i', "input", "in.pdf");
        std::string outfile = ops.getopt<std::string>('o', "output", "out.pdf");
        wstring mark_txt = String2WString(ops.getopt<std::string>('t', "text",
"联通数字科技有限公司总部投标专用文档")); wstring mark_opacity =
String2WString(ops.getopt<std::string>('p', "opacity", "15%")); wstring
mark_color = String2WString(ops.getopt<std::string>('c', "color", "gray"));
        wstring mark_rotate = String2WString(ops.getopt<std::string>('r',
"rotate", "40")); wstring mark_font =
String2WString(ops.getopt<std::string>('f', "font", "simkai")); wstring
mark_number =String2WString(ops.getopt<std::string>('n', "number", "1"));
//水印数量，1或者3 wstring file_pages
=String2WString(ops.getopt<std::string>('s', "pages", "")); // 文件页数 wstring
file_pagesize =String2WString(ops.getopt<std::string>('z', "size", "")); //
页面尺寸

        if (file_pages.size() > 1) {
      getpages(file_pages,"pages");
                exit(0);

        }
        if (file_pagesize.size() > 1) {
          getpages(file_pagesize, "size");
          exit(0);
        }

        int numArg = 0;
        for (int i = 1; i < argc; ++i) {
                ++numArg;
        }
        //cout<<numArg<<endl;
        //获取输入文件
        if (numArg == 0) {
                usage();
                exit(0);
        }

        int r;
        // n=3 页面添加3条水印文本
        if (mark_number == L"3") {
          string tmpfile = GetTmpFolder() + "\\" + GetFilename(infile);
          r = setMark(infile, outfile, mark_txt, mark_opacity, mark_font,
                      mark_rotate, mark_color, L"0.6", L"top", L"-10");
          if (r == 0) {
            // DeleteFile(String2WString(infile).c_str());
            // rename(outfile.c_str(), infile.c_str());
            r = setMark(outfile, tmpfile, mark_txt, mark_opacity, mark_font,
                        mark_rotate, mark_color, L"0.6", L"center", L"10");
            if (r == 0) {
              r = setMark(tmpfile, outfile, mark_txt, mark_opacity, mark_font,
                          mark_rotate, mark_color, L"0.6", L"bottom", L"40");
              if (r == 0) {
                DeleteFile(String2WString(tmpfile).c_str());
              } else {
                return 2;
              }
            } else {
              return 2;
            }
          } else {
            return 2;
          }
        } else {
          r = setSingleMark(infile, outfile, mark_txt, mark_opacity, mark_font,
                            mark_rotate, mark_color);
        }

        return 0;
}
*/

int setMark(std::string infile, std::string outfile, wstring mark_txt,
            wstring mark_opacity, wstring mark_font, wstring mark_rotate,
            wstring mark_color, wstring scale, wstring vertalign,
            wstring vertshift) {
  // wcout << mark_color << endl;
  PDFlib p;
  int indoc, endpage, pageno, page, font;
  const wstring searchpath = L"./PDFlib-CMap-5.0/resource/cmap";
  wostringstream optlist;
  optlist << L"searchpath={{" << searchpath << L"}";
  optlist << L" {" << GetFontsFolder() << L"}}";
  p.set_option(optlist.str());
  // wcout << optlist.str() << endl;

  p.set_info(L"Creator", L"泛生态业务工具集");
  p.set_info(L"Title", L"本文档来自于泛生态业务投标案例");
  indoc = p.open_pdi_document(String2WString(infile), L"");
  // font = p.load_font(L"Helvetica", L"winansi", L"unicode");
  // 设置字体、字符集
  p.set_option(L"FontOutline={" + mark_font + L"=" + mark_font + L".ttf}");
  font = p.load_font(mark_font, L"unicode", L"");
  if (font == -1) {
    wcerr << L"Error: " << p.get_errmsg() << endl;
    return 2;
  }
  endpage = (int)p.pcos_get_number(indoc, L"length:pages");

  try {
    if (p.begin_document(String2WString(outfile), L"") == -1) {
      wcerr << L"Error: " << p.get_errmsg() << endl;
      return 2;
    }
    p.begin_template_ext(10, 10,
                         L"watermark={location=ontop opacity=" + mark_opacity +
                             L"  horizalign=right horizshift=-200 scale=" +
                             scale + L" vertalign=" + vertalign +
                             L" vertshift=" + vertshift + L"}  topdown=true");
    p.fit_textline(mark_txt, 800, 200,
                   L"fontsize=10 fontname=" + mark_font +
                       L" encoding=unicode fillcolor=" + mark_color +
                       L" boxsize={95 42} rotate=" + mark_rotate);
    p.end_template_ext(0, 0);

    for (pageno = 1; pageno <= endpage; pageno++) {
      page = p.open_pdi_page(indoc, pageno, L"");

      p.begin_page_ext(0, 0, L"width=a4.width height=a4.height");

      p.fit_pdi_page(page, 0, 0, L"adjustpage");

      p.close_pdi_page(page);

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

int setSingleMark(string in, string out, wstring mark_txt, wstring mark_opacity,
                  wstring mark_font, wstring mark_rotate, wstring mark_color) {
  int indoc, endpage, pageno, page, font;
  wstring infile = String2WString(in);
  wstring outfile = String2WString(out);
  try {
    PDFlib p;
    const wstring searchpath = L"./PDFlib-CMap-5.0/resource/cmap";
    wostringstream optlist;
    optlist << L"searchpath={{" << searchpath << L"}";
    optlist << L" {" << GetFontsFolder() << L"}}";
    p.set_option(optlist.str());
    // wcout << optlist.str() << endl;
    if (p.begin_document(outfile, L"") == -1) {
      wcerr << L"Error: " << p.get_errmsg() << endl;
      return 2;
    }
    p.set_info(L"Creator", L"泛生态业务工具集");
    p.set_info(L"Title", L"本文档来自于泛生态业务投标案例");

    indoc = p.open_pdi_document(infile, L"");
    // font = p.load_font(L"Helvetica", L"winansi", L"unicode");
    // 设置字体、字符集
    p.set_option(L"FontOutline={" + mark_font + L"=" + mark_font + L".ttf}");
    font = p.load_font(mark_font, L"unicode", L"");

    if (font == -1) {
      wcerr << L"Error: " << p.get_errmsg() << endl;
      return 2;
    }
    // wcout << L"infile:" << infile << endl;
    endpage = (int)p.pcos_get_number(indoc, L"length:pages");
    // wcout << L"mark_opacity:" << mark_opacity << endl;

    p.begin_template_ext(
        0, 0, L"watermark={location=ontop opacity=" + mark_opacity + L"}");

    // p.fit_textline(L"继续与设计院、业主重点合作伙伴沟通，争取达成共赢合作模式",
    // 0, 0,L"fontsize=10 fontname=STSong-Light encoding=unicode  fillcolor=red
    // boxsize={595 842} rotate=15 stamp=ll2ur");
    // p.fit_textline(L"“十四五”武都引水灌区现代化改造工程信息化(“智慧武引”二期)系统",
    // 0, 0,L"fontsize=10 fontname=STSong-Light encoding=unicode  fillcolor=red
    // boxsize={195 142}  rotate=-45 "); wstring mark = L"fontsize=10 fontname="
    // + mark_font + L" encoding=unicode  fillcolor=" + mark_color + L"
    // boxsize={95 42}  rotate=" + mark_rotate;
    wstring mark = L"fontsize=10 fontname=" + mark_font +
                   L" encoding=unicode  fillcolor=" + mark_color +
                   L" boxsize={95 42}  rotate=" + mark_rotate;
    // wcout<< mark<<endl;
    p.fit_textline(mark_txt, 100, 100, mark);
    p.end_template_ext(0, 0);

    for (pageno = 1; pageno <= endpage; pageno++) {
      page = p.open_pdi_page(indoc, pageno, L"");

      p.begin_page_ext(0, 0, L"width=a4.width height=a4.height");

      p.fit_pdi_page(page, 0, 0, L"adjustpage");

      p.close_pdi_page(page);

      p.end_page_ext(L"");
    }
    p.end_document(L"");

  }

  catch (PDFlib::Exception& ex) {
    wcerr << L"PDFlib 发生异常: " << endl
          << L"[" << ex.get_errnum() << L"] " << ex.get_apiname() << L": "
          << ex.get_errmsg() << endl
          << L": " << L"错误的参数选项请使用 - h参数查看帮助" << endl;
    return 2;
  }
  return 0;
}

// 获取文件总页数
int getpages(wstring filename, string flag) {
  PDFlib m_oPDF;
  try {
    //打开源pdf文件获取实例id
    // pageName = String2WString(char2string(optarg));
    int nSrcPDFID = m_oPDF.open_pdi_document(filename, L"");
    if (nSrcPDFID == -1) {
      cout << "打开源pdf文件失败!" << endl;
      return 0;
    }
    // m_oPDF.set_option(L"searchpath={{ print_cache }}");//设置默认路径

    //根据SrcPDF实例id获取pdf中的信息

    int endpage =
        (int)m_oPDF.pcos_get_number(nSrcPDFID, L"length:pages");  //总页数
    auto fWidth =
        m_oPDF.pcos_get_number(nSrcPDFID, L"pages[0]/width");  //宽度595
    auto fHeight =
        m_oPDF.pcos_get_number(nSrcPDFID, L"pages[0]/height");  //高度842

    // printf("%d", endpage);

    if (flag == "pages") {
      cout << endpage << endl;
    } else {
      cout << fWidth << "," << fHeight << endl;
    }

    //		m_oPDF.close_font(font);
    m_oPDF.close_pdi_document(nSrcPDFID);
    return endpage;
  } catch (PDFlib::Exception& e) {
    //输出pdflib异常信息
    wprintf(m_oPDF.get_errmsg().c_str());
    return 0;
  } catch (...) {
    //其他异常
    return 0;
  }
}

int addWatermark(const QString& i, const QString& o, const QString& t,
                 const QString& p, const QString& c, const QString& r,
                 const QString& f, const QString& s, const QString& va,
                 const QString& vs) {
  wstring infile = QString2WString(i);
  wstring outfile = QString2WString(o);
  wstring mark_txt = QString2WString(t);

  wstring mark_opacity = QString2WString(p);
  wstring mark_color = QString2WString(c);
  wstring mark_rotate = QString2WString(r);
  wstring mark_font = QString2WString(f);
  wstring mark_scale = QString2WString(s);
  wstring mark_vertalign = QString2WString(va);
  wstring mark_vertshift = QString2WString(vs);

  // wstring mark1 = L"fontsize=10 fontname=" + mark_font + L"
  // encoding=unicode fillcolor=" + mark_color + L" boxsize={95 42} rotate="
  // + mark_rotate; wcout
  // << mark_txt << endl; qDebug() << "mark_txt:" << mark_txt;//流输出方式
  // qDebug() << getpages(infile, "pages");

  int indoc, endpage, pageno, page, font;

  try {
    PDFlib p;
    const wstring searchpath = L"./PDFlib-CMap-5.0/resource/cmap";
    wostringstream optlist;
    wstring mark;
    optlist << L"searchpath={{" << searchpath << L"}";
    optlist << L" {" << GetFontsFolder() << L"}}";
    p.set_option(optlist.str());
    if (p.begin_document(outfile, L"") == -1) {
      wcerr << L"Error: " << p.get_errmsg() << endl;
      return 2;
    }
    p.set_info(L"Creator", L"泛生态业务工具集");
    p.set_info(L"Title", L"本文档来自于泛生态业务投标案例");
    indoc = p.open_pdi_document(infile, L"");
    // font = p.load_font(L"Helvetica", L"winansi", L"unicode");
    // 设置字体、字符集
    // p.set_option(L"FontOutline={" + mark_font + L"=" + mark_font + L".ttf}");
    // wcout << L"font:" << mark_font << endl;
    // font = p.load_font(mark_font, L"unicode", L"");
    if (mark_font == L"NSimSun") {
      p.set_option(L"FontOutline={NSimSun=simsun.ttc}");
      font = p.load_font(L"NSimSun", L"unicode", L"embedding");
    } else {
      p.set_option(L"FontOutline={" + mark_font + L"=" + mark_font + L".ttf}");
      font = p.load_font(mark_font, L"unicode", L"");
    }

    if (font == -1) {
      wcerr << L"Error: " << p.get_errmsg() << endl;
      return 2;
    }
    // wcout << L"infile:" << infile << endl;
    endpage = (int)p.pcos_get_number(indoc, L"length:pages");
    // wcout << L"mark_opacity:" << mark_opacity << endl;

    p.begin_template_ext(
        0, 0, L"watermark={location=ontop opacity=" + mark_opacity + L"}");
    /*
    p.begin_template_ext(0, 0,
                         L"watermark={location=ontop opacity=" + mark_opacity +
                             L"  horizalign=right horizshift=-200 scale=" +
                             mark_scale + L" vertalign=" + mark_vertalign +
                             L" vertshift=" + mark_vertshift +
                             L"}  topdown=true");
*/
    mark = L"fontsize=10 fontname=" + mark_font +
           L" encoding=unicode  fillcolor=" + mark_color +
           L" boxsize={95 42}  rotate=" + mark_rotate;
    // wcout<< mark<<endl;
    p.fit_textline(mark_txt, 100, 100, mark);
    p.end_template_ext(0, 0);

    for (pageno = 1; pageno <= endpage; pageno++) {
      page = p.open_pdi_page(indoc, pageno, L"");

      p.begin_page_ext(0, 0, L"width=a4.width height=a4.height");

      p.fit_pdi_page(page, 0, 0, L"adjustpage");

      p.close_pdi_page(page);

      p.end_page_ext(L"");
    }
    p.end_document(L"");
  }

  catch (PDFlib::Exception& ex) {
    wcerr << L"PDFlib 发生异常: " << endl
          << L"[" << ex.get_errnum() << L"] " << ex.get_apiname() << L": "
          << ex.get_errmsg() << endl
          << L": " << L"错误的参数选项请使用 - h参数查看帮助" << endl;
    return 2;
  }
  return 0;
}

/*

int main(int argc, char* argv[]){

        GetOpt_pp ops(argc, argv);


        wstring infile = String2WString(ops.getopt<std::string>('i', "input",
"in.pdf")); wstring outfile = String2WString(ops.getopt<std::string>('o',
"output", "out.pdf")); wstring mark_txt =
String2WString(ops.getopt<std::string>('t', "text",
"联通数字科技有限公司总部投标专用文档")); wstring mark_opacity =
String2WString(ops.getopt<std::string>('p', "opacity", "15%")); wstring
mark_color = String2WString(ops.getopt<std::string>('c', "color", "gray"));
        wstring mark_rotate = String2WString(ops.getopt<std::string>('r',
"rotate", "45")); wstring mark_font =
String2WString(ops.getopt<std::string>('f', "font", "simkai")); wstring
file_pages = String2WString(ops.getopt<std::string>('s', "pages", ""));

        //wstring mark1 = L"fontsize=10 fontname=" + mark_font + L"
encoding=unicode  fillcolor=" + mark_color + L" boxsize={95 42}  rotate=" +
mark_rotate;
        //wcout << mark1 << endl;

        if (file_pages.size()>1){
                getpages(file_pages);
                exit(0);

        }




        int indoc, endpage, pageno, page, font;

        int numArg = 0;
        for (int i = 1; i < argc; ++i) {
                ++numArg;
        }
        //cout<<numArg<<endl;
        //获取输入文件
        if (numArg == 0) {
                usage();
                exit(0);
        }

        try {

                PDFlib p;
                const wstring searchpath = L"./PDFlib-CMap-5.0/resource/cmap";
                wostringstream optlist;
                optlist << L"searchpath={{" << searchpath << L"}";
                optlist << L" {" << GetFontsFolder() << L"}}";
                p.set_option(optlist.str());
                //wcout << optlist.str() << endl;
                if (p.begin_document(outfile, L"") == -1) {
                        wcerr << L"Error: " << p.get_errmsg() << endl;
                        return 2;
                }
                p.set_info(L"Creator", L"泛生态业务工具集");
                p.set_info(L"Title", L"本文档来自于泛生态业务投标案例");


                indoc = p.open_pdi_document(infile, L"");
                //font = p.load_font(L"Helvetica", L"winansi", L"unicode");
                // 设置字体、字符集
                p.set_option(L"FontOutline={" + mark_font + L"=" + mark_font +
L".ttf}");

                //wcout << L"font:" << mark_font << endl;

                font = p.load_font(mark_font, L"unicode", L"");

                if (font == -1) {
                        wcerr << L"Error: " << p.get_errmsg() << endl;
                        return 2;
                }
                //wcout << L"infile:" << infile << endl;
                endpage = (int)p.pcos_get_number(indoc, L"length:pages");
                //wcout << L"mark_opacity:" << mark_opacity << endl;


                p.begin_template_ext(0, 0,
                        L"watermark={location=ontop opacity=" + mark_opacity +
L"}");


                //p.fit_textline(L"继续与设计院、业主重点合作伙伴沟通，争取达成共赢合作模式",
0, 0,L"fontsize=10 fontname=STSong-Light encoding=unicode  fillcolor=red
boxsize={595 842} rotate=15 stamp=ll2ur");
                //p.fit_textline(L"“十四五”武都引水灌区现代化改造工程信息化(“智慧武引”二期)系统",
0, 0,L"fontsize=10 fontname=STSong-Light encoding=unicode  fillcolor=red
boxsize={195 142}  rotate=-45 ");
                //wstring mark = L"fontsize=10 fontname=" + mark_font + L"
encoding=unicode  fillcolor=" + mark_color + L" boxsize={95 42}  rotate=" +
mark_rotate; wstring mark = L"fontsize=10 fontname=" + mark_font + L"
encoding=unicode  fillcolor=" + mark_color + L" boxsize={95 42}  rotate=" +
mark_rotate;
                //wcout<< mark<<endl;
                p.fit_textline(mark_txt, 100, 100, mark);
                p.end_template_ext(0, 0);




                for (pageno = 1; pageno <= endpage; pageno++)
                {
                        page = p.open_pdi_page(indoc, pageno, L"");

                        p.begin_page_ext(0, 0, L"width=a4.width
height=a4.height");

                        p.fit_pdi_page(page, 0, 0, L"adjustpage");

                        p.close_pdi_page(page);

                        p.end_page_ext(L"");
                }
                p.end_document(L"");

        }

        catch (PDFlib::Exception& ex) {

                wcerr << L"PDFlib 发生异常: " << endl
                        << L"[" << ex.get_errnum() << L"] " << ex.get_apiname()
                        << L": " << ex.get_errmsg() << endl
                        << L": " << L"错误的参数选项请使用 - h参数查看帮助" <<
endl; return 2;
        }



        return 0;

}

*/
