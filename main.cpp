/*============================================================================
 Name        : stack.c
 Author      : MHD Bashar Al Fattal
 Version     : v1
 Copyright   : Your copyright notice
 Description : Radio Player and recorder
 Datum       : 29.08.2022
 ============================================================================*/

#include "recorder.h"

#include <QApplication>

//welches Logo und Name hat die App und der Aufruf des Fensters

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/new/icon/ibs-logo_1.png"));
    app.setApplicationName("IBS-Radio Recoder");
    app.setOrganizationName("IBS-Pruefsysteme");
    app.setApplicationVersion("1.0");
    Recorder w;
    w.show();
    return app.exec();
}
