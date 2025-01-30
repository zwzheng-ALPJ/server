QT       += core gui concurrent sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    communi_protocol_handler.cpp \
    ecg_predict.cpp \
    logger.cpp \
    main.cpp \
    mainwindow.cpp \
    mobile_communi_protocol_handler.cpp \
    quitdialog.cpp \
    sql_connect.cpp \
    ssl_tcp_server.cpp \
    tcp_server.cpp \
    user_access_control.cpp

HEADERS += \
    communi_protocol_handler.h \
    ecg_predict.h \
    logger.h \
    mainwindow.h \
    mobile_communi_protocol_handler.h \
    quitdialog.h \
    sql_connect.h \
    ssl_tcp_server.h \
    tcp_server.h \
    user_access_control.h

FORMS += \
    mainwindow.ui \
    quitdialog.ui

INCLUDEPATH += /usr/local/mysql-8.4.2-macos14-x86_64/include \
#    /usr/local/lib/python3.12/site-packages/tensorflow/include

LIBS += \
#    /usr/local/lib/python3.12/site-packages/tensorflow/libtensorflow_cc.2.dylib \
#    /usr/local/lib/python3.12/site-packages/tensorflow/libtensorflow_framework.2.dylib

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icon.qrc


# OpenSSL
# 指定OpenSSL库的路径
OPENSSL_LIBS = -L/usr/local/opt/openssl/lib -lssl -lcrypto
# 指定OpenSSL头文件的路径
INCLUDEPATH += /usr/local/opt/openssl/include

# 将OpenSSL库链接到项目中
LIBS += $$OPENSSL_LIBS
