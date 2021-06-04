#-------------------------------------------------
#
# Project created by QtCreator 2012-05-03T19:01:56
#
#-------------------------------------------------

QT += core gui widgets network
#QT += webkit  //for QWebView
## 在Select Components中点开Qt 6.1.0，勾选Qt 5 Compatibility Module
QT += core5compat
## #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
##     #include <QtCore/QTextCodec>
## #else
##     #include <QtCore5Compat/QTextCodec>
## #endif
## Or simply
## include <QTextCodec>

#for new signal & slot syntax
CONFIG += c++11

TARGET = Qtoolbox
TEMPLATE = app

#Release:
#    # https://upx.github.io/
#    # 解压缩后，只有 upx.exe 是有用的，可以将它放在 mingw32\bin 或 windows\system32 下，省去设置路径的烦恼
#    QMAKE_POST_LINK += $$PWD/upx -9 $(DESTDIR_TARGET)
#win32:CONFIG(release, debug|release): QMAKE_POST_LINK += $$PWD/upx -9 $(DESTDIR_TARGET)
win32{
    CONFIG(release, debug|release): QMAKE_POST_LINK += $$PWD/upx -9 $(DESTDIR_TARGET)
}

#设置输出目录
#DESTDIR = $$PWD/build-out-bin

# Variables
APP_VERSION = 1.0.0
# 0.1.1 <==> 00_01_01
APP_VERSION_ID = 10000
APP_VERSION_RELEASE_TIME = "2020-03-28"

DEFINES += TARGET=\\\"$$TARGET\\\"
DEFINES += APP_VERSION=\\\"$$APP_VERSION\\\"
DEFINES += APP_VERSION_ID=\\\"$$VERSION_ID\\\"
DEFINES += APP_VERSION_RELEASE_TIME=\\\"$$APP_VERSION_RELEASE_TIME\\\"


TRANSLATIONS += i18n/zh_CN.ts

SOURCES += main.cpp\
    commonMethod.cpp \
        mainwindow.cpp \
    aboutdialog.cpp \
    mythread.cpp

HEADERS  += mainwindow.h \
    commonMethod.h \
    lineedit.h \
    constant.h \
    aboutdialog.h \
    mythread.h

FORMS    += mainwindow.ui


MOC_DIR = tmp/moc
RCC_DIR = tmp/rcc
UI_DIR = tmp/ui
OBJECTS_DIR = tmp/obj

win32 {
          RC_FILE = myIcon.rc
}

OTHER_FILES += \
    charset.txt


#  https://blog.csdn.net/lifang303166/article/details/8840234
# iconutil –c icns --output ${project_dir}/icons/icon.icns ${project_dir}/icon.iconset
#ICON = icons/icon.icns
RESOURCES += \
    images.qrc \
    icons.qrc \
    languages.qrc

DISTFILES += \
    .gitignore \
    CHANGELOG \
    README.md

