#pragma once
#ifndef WATERMARK_H
#define WATERMARL_H

#include <QObject>
#include <string>

using namespace std;
std::wstring String2WString(const std::string& s);

int getpages(std::wstring filename, string flag);

int setSingleMark(string in, string out, wstring mark_txt, wstring mark_opacity,
                  wstring mark_font, wstring mark_rotate, wstring mark_color);
int setMark(std::string infile, std::string outfile, wstring mark_txt,
            wstring mark_opacity, wstring mark_font, wstring mark_rotate,
            wstring mark_color, wstring scale, wstring vertalign,
            wstring vertshift);
std::wstring GetFontsFolder();
std::string GetTmpFolder();
std::string GetFilename(string path);
std::string char2string(char* c);
std::wstring QString2WString(const QString& str);
int addWatermark(const QString& i = "in.pdf", const QString& o = "out.pdf",
                 const QString& t = "联通数字科技有限公司总部投标专用文档",
                 const QString& p = "15%", const QString& c = "gray",
                 const QString& r = "45", const QString& f = "simkai");

#endif  // WATERMARL_H
