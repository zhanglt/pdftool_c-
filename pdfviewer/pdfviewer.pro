RC_ICONS = serviceMonitor.ico
TEMPLATE = app
TARGET = pdfviewer
QT += core gui widgets pdfwidgets printsupport svg

SOURCES += \
    function.cpp \
    lib/pdflib.cpp \
    main.cpp \
    mainwindow.cpp \
    pageselector.cpp \
    src/QProgressIndicator.cpp \
    src/lineedit/CustomLineEdit.cpp \
    src/mark/multiWatermarkThreadSingle.cpp \
    src/mark/watermarkThread.cpp \
    src/mark/watermarkThreadSingle.cpp \
    src/mark/wmark.cpp \
    src/pdf2image/pdf2ImageThreadSingle.cpp \
    src/slider/CustomSlider.cpp \
    src/textedit/CustomTextEdit.cpp \
    zoomselector.cpp

HEADERS += \
    function.h \
    include/QProgressIndicator.h \
    include/lineedit/CustomLineEdit.h \
    include/mark/mark.h \
    include/mark/multiWatermarkThreadSingle.h \
    include/mark/watermarkThread.h \
    include/mark/watermarkThreadSingle.h \
    include/mytable.h \
    include/pdf2image/pdf2ImageThreadSingle.h \
    include/slider/CustomSlider.h \
    include/textedit/CustomTextEdit.h \
    lib/cpp_wrapped.h \
    lib/pdflib.h \
    lib/pdflib.hpp \
    mainwindow.h \
    pageselector.h \
    zoomselector.h

FORMS += \
    mainwindow.ui

RESOURCES += \
    resources.qrc

target.path = $$[QT_INSTALL_EXAMPLES]/pdfwidgets/pdfviewer
INSTALLS += target

DISTFILES += \
    lib/pdflib.dll \
    lib/pdflib.lib \
    qss/pdf.css

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/mupdf/lib/release/ -llibmupdf
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/mupdf/lib/debug/ -llibmupdf

INCLUDEPATH += $$PWD/''
DEPENDPATH += $$PWD/''
INCLUDEPATH += $$PWD/mupdf/include/

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/mupdf/lib/release/liblibmupdf.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/mupdf/lib/debug/liblibmupdf.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/mupdf/lib/release/libmupdf.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/mupdf/lib/debug/libmupdf.lib






win32:CONFIG(release, debug|release): LIBS += -L$$PWD/mupdf/lib/release/ -llibpkcs7
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/mupdf/lib/debug/ -llibpkcs7

INCLUDEPATH += $$PWD/''
DEPENDPATH += $$PWD/''

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/mupdf/lib/release/liblibpkcs7.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/mupdf/lib/debug/liblibpkcs7.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/mupdf/lib/release/libpkcs7.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/mupdf/lib/debug/libpkcs7.lib

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/mupdf/lib/release/ -llibmuthreads
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/mupdf/lib/debug/ -llibmuthreads

INCLUDEPATH += $$PWD/mupdf/lib/Release
DEPENDPATH += $$PWD/mupdf/lib/Release

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/mupdf/lib/release/liblibmuthreads.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/mupdf/lib/debug/liblibmuthreads.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/mupdf/lib/release/libmuthreads.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/mupdf/lib/debug/libmuthreads.lib

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/mupdf/lib/release/ -llibthirdparty
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/mupdf/lib/debug/ -llibthirdparty

INCLUDEPATH += $$PWD/mupdf/lib/Release
DEPENDPATH += $$PWD/mupdf/lib/Release

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/mupdf/lib/release/liblibthirdparty.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/mupdf/lib/debug/liblibthirdparty.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/mupdf/lib/release/libthirdparty.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/mupdf/lib/debug/libthirdparty.lib





win32:CONFIG(release, debug|release): LIBS += -L$$PWD/libxml2/lib/release/ -llibxml2
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/libxml2/lib/debug/ -llibxml2d

INCLUDEPATH += $$PWD/libxml2/include/libxml2
DEPENDPATH += $$PWD/libxml2/include/libxml2

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/iconv/lib/release/ -llibiconv
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/iconv/lib/debug/ -llibiconv

INCLUDEPATH += $$PWD/iconv/include
DEPENDPATH += $$PWD/iconv/include
