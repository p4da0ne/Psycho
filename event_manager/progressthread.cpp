#include "progressthread.h"

void ProgressThread::run()
{
    QSettings *settings_db_connect = new QSettings("vka_db","saturn");
    QString _db_name=settings_db_connect->value("/settings_db_connect/db_name","SATURN").toString();
    QString _user_name=settings_db_connect->value("/settings_db_connect/user_name","postgres").toString();
    QString _user_password=settings_db_connect->value("/settings_db_connect/user_password","").toString();
    QString _host=settings_db_connect->value("/settings_db_connect/host","localhost").toString();

    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName(_host);
    db.setDatabaseName(_db_name);
    db.setUserName(_user_name);
    db.setPassword(_user_password);
    bool ok = db.open();
    QSqlQuery query;
    if(!query.exec(QString("SELECT * FROM event_media where id_event_media = %1").arg(this->id_event_media))){
        qDebug() << query.lastError();
        return;
    }
    int index_name_event_media = query.record().indexOf("filename_media");
    int index_media = query.record().indexOf("media");
    while(query.next()){
        QString file_path=QDir::tempPath() + "/" + query.value(index_name_event_media).toString();
        QFile file(file_path);
        file.open(QIODevice::WriteOnly);
        file.write(query.value(index_media).toByteArray());
        file.close();
        QDesktopServices::openUrl(QUrl(file_path.toUtf8()));
    }
}
