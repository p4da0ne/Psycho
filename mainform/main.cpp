#include <QtGui>
#include "mainform.h"
#include "qapplication.h"
#include "qstylefactory.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    QApplication::setPalette(QApplication::style()->standardPalette());
    QTranslator translator;
    translator.load("akcopp_ru.qm",".");
    a.installTranslator(&translator);

    Mainform w;

    w.setBackgroundRole(QPalette::Shadow);
    w.setWindowIcon(QIcon(":Resources/RedStar.ico"));
    w.showMaximized();
    return a.exec();
}

//int main(int argc, char *argv[])
//{
//    QApplication a(argc, argv);
//    QTranslator translator;
//    translator.load("akcopp_ru.qm",".");
//    a.installTranslator(&translator);

//    Mainform * w = new Mainform;

//    w->setBackgroundRole(QPalette::Shadow);

//    w->showMaximized();
//    return a.exec();
//}
