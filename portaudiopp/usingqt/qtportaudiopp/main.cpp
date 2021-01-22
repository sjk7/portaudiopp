#include "dialog.h"

#include <QApplication>

auto main(int argc, char *argv[]) -> int
{
    QApplication a(argc, argv);
    Dialog w;
    w.show();
    return a.exec();
}
