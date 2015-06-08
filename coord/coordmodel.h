#ifndef COORDMODEL_H
#define COORDMODEL_H

#include <QObject>
#include "coord.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlResult>
#include <saturnSignTypes.h>
#include <QDebug>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>

class __EXPORT_COORD CoordModel : public QObject
{
    Q_OBJECT
public:
    explicit CoordModel(QObject *parent = 0);
    bool insertObjectCoord(Coord * coordinate,QString ObjectTypes, int id_object);
    bool insertObjectCoord(QList<Coord * > coordinates, QString ObjectsTypes, int id_object);
    int insertCoord(Coord * coordinates);
    QList<int> insertCoord(QList<Coord *> coordinates);
    QList<Coord *> getCoordinates(QString ObjectsTypes, int id_object, QString idObjectName);
    int updateCoord(Coord *coordinates, int id_coordinates);
signals:
    
public slots:

protected:
    bool objectCoord(int id_coordinate, QString ObjectsTypes, int id_object);


    
};

#endif // COORDMODEL_H
