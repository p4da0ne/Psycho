#ifndef COORDMODEL_H
#define COORDMODEL_H

#include <QObject>
#include "coord.h"
#include <QSqlQuery>
#include <saturnSignTypes.h>
#include <QDebug>
#include <QSqlError>
#include <QSqlRecord>

class CoordModel : public QObject
{
    Q_OBJECT
public:
    explicit CoordModel(QObject *parent = 0);
    static bool insertObjectCoord(Coord * coordinate,QString ObjectsTypes, int id_object);
    static int insertCoord(Coord * coordinates);
    static QList<int> insertCoord(QList<Coord *> coordinates);
    static bool insertObjectCoord(QList<Coord * > coordinates, QString ObjectsTypes, int id_object);
    static QList<Coord *> getCoordinates(QString ObjectsTypes, int id_object);
signals:
    
public slots:

protected:
    static bool objectCoord(int id_coordinate, QString ObjectsTypes, int id_object);


    
};

#endif // COORDMODEL_H
