TEMPLATE = subdirs
SUBDIRS += pdfviewer

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/pdfviewer/mupdf/lib/release/ -llibpkcs7
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/pdfviewer/mupdf/lib/debug/ -llibpkcs7

INCLUDEPATH += $$PWD/''
DEPENDPATH += $$PWD/''

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/pdfviewer/mupdf/lib/release/liblibpkcs7.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/pdfviewer/mupdf/lib/debug/liblibpkcs7.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/pdfviewer/mupdf/lib/release/libpkcs7.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/pdfviewer/mupdf/lib/debug/libpkcs7.lib

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/pdfviewer/mupdf/lib/release/ -llibmuthreads
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/pdfviewer/mupdf/lib/debug/ -llibmuthreads

INCLUDEPATH += $$PWD/pdfviewer/mupdf/lib/Release
DEPENDPATH += $$PWD/pdfviewer/mupdf/lib/Release

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/pdfviewer/mupdf/lib/release/liblibmuthreads.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/pdfviewer/mupdf/lib/debug/liblibmuthreads.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/pdfviewer/mupdf/lib/release/libmuthreads.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/pdfviewer/mupdf/lib/debug/libmuthreads.lib

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/pdfviewer/mupdf/lib/release/ -llibthirdparty
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/pdfviewer/mupdf/lib/debug/ -llibthirdparty

INCLUDEPATH += $$PWD/pdfviewer/mupdf/lib/Release
DEPENDPATH += $$PWD/pdfviewer/mupdf/lib/Release

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/pdfviewer/mupdf/lib/release/liblibthirdparty.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/pdfviewer/mupdf/lib/debug/liblibthirdparty.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/pdfviewer/mupdf/lib/release/libthirdparty.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/pdfviewer/mupdf/lib/debug/libthirdparty.lib
