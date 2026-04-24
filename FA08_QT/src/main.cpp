#include <QApplication>
#include <QFont>
#include <QFontDatabase>
#include "ui/LoginWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("FA08全自动荧光免疫分析仪");
    app.setOrganizationName("FA08");

    // Set default font (Chinese-friendly)
    QFont font("Microsoft YaHei", 9);
    font.setStyleHint(QFont::SansSerif);
    app.setFont(font);

    LoginWindow w;
    w.show();

    return app.exec();
}
