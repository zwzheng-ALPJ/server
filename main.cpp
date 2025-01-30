#include "mainwindow.h"

#include <QApplication>
#include <QProcess>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 强制使用浅色模式 Force bright mode
    // QPalette palette;
    // palette.setColor(QPalette::Window, Qt::white);
    // palette.setColor(QPalette::WindowText, Qt::black);
    // palette.setColor(QPalette::Base, Qt::white);
    // palette.setColor(QPalette::AlternateBase, Qt::lightGray);
    // palette.setColor(QPalette::ToolTipBase, Qt::white);
    // palette.setColor(QPalette::ToolTipText, Qt::black);
    // palette.setColor(QPalette::Text, Qt::black);
    // palette.setColor(QPalette::Button, Qt::white);
    // palette.setColor(QPalette::ButtonText, Qt::black);
    // palette.setColor(QPalette::BrightText, Qt::red);
    // palette.setColor(QPalette::Link, Qt::blue);
    // palette.setColor(QPalette::Highlight, Qt::blue);
    // palette.setColor(QPalette::HighlightedText, Qt::white);
    // a.setPalette(palette);

    // 强制使用深色模式 Force dark mode
    QPalette palette;
    palette.setColor(QPalette::Window, QColor(53,53,53));
    palette.setColor(QPalette::WindowText, Qt::white);
    palette.setColor(QPalette::Base, QColor(25,25,25));
    palette.setColor(QPalette::AlternateBase, QColor(53,53,53));
    palette.setColor(QPalette::ToolTipBase, Qt::white);
    palette.setColor(QPalette::ToolTipText, Qt::white);
    palette.setColor(QPalette::Text, Qt::white);
    palette.setColor(QPalette::Button, QColor(53,53,53));
    palette.setColor(QPalette::ButtonText, Qt::white);
    palette.setColor(QPalette::BrightText, Qt::red);
    palette.setColor(QPalette::Link, QColor(42, 130, 218));
    palette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    palette.setColor(QPalette::HighlightedText, Qt::black);
    a.setPalette(palette);

    MainWindow w;
    w.show();
    //return a.exec();
    int e=a.exec();
    if(e==773)
    {
        QProcess::startDetached(qApp->applicationFilePath(),QStringList());
        return 0;
    }
    return e;
}
