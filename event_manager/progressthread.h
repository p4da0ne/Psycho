#ifndef PROGRESSTHREAD_H
#define PROGRESSTHREAD_H

#include <QObject>
#include <QThread>
#include <QSqlDatabase>
#include <QSettings>
#include <QWidget>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDesktopServices>
#include <QFile>
#include <QUrl>
#include <QDir>
#include <QDebug>
#include <QProgressDialog>

class ProgressThread : public QThread
{
public:
    void run();
    int id_event_media;
};

#endif // PROGRESSTHREAD_H
