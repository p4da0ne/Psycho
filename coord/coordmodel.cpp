#include "coordmodel.h"
#include <QVariant>

CoordModel::CoordModel(QObject *parent) :
    QObject(parent)
{
}

bool CoordModel::insertObjectCoord(Coord *coordinates, QString ObjectsTypes, int id_object)
{
    int id_coordinates = this->insertCoord(coordinates);
    return this->objectCoord(id_coordinates,ObjectsTypes,id_object);
}

bool CoordModel::insertObjectCoord(QList<Coord *> coordinates, QString ObjectsTypes, int id_object)
{
    QList<int> id_coordinates = this->insertCoord(coordinates);
    for(int i = 0; i < id_coordinates.size(); i++)
        this->objectCoord(id_coordinates.at(i),ObjectsTypes,id_object);
    return true;
}

int CoordModel::insertCoord(Coord *coordinates)
{
    QSqlQuery query;
    QString str = QString("INSERT INTO coordinates (latitude_wgs_84_g, latitude_wgs_84_m, latitude_wgs_84_s, longitude_wgs_84_g,longitude_wgs_84_m,longitude_wgs_84_s) VALUES (%1,%2,%3,%4,%5,%6) RETURNING id_coordinates")
            .arg(coordinates->getLatDegrees())
            .arg(coordinates->getLatMinutes())
            .arg(coordinates->getLatSeconds())
            .arg(coordinates->getLongDegrees())
            .arg(coordinates->getLongMinutes())
            .arg(coordinates->getLongSeconds());
    if(!query.exec(str)){
        qDebug() << query.lastError().text();
        qDebug() << query.lastQuery();
        query.clear();
        return 0;
    }
    int id_coordinates ;
    while(query.next()){
        id_coordinates = query.value(0).toInt();
    }
    query.clear();
    return id_coordinates;
}

QList<int> CoordModel::insertCoord(QList<Coord *> coordinates)
{
    QList<int > coordinatesId;
    for(int i = 0; i< coordinates.size(); i++){
        coordinatesId.append(this->insertCoord(coordinates.at(i)));
    }
    return coordinatesId;
}

bool CoordModel::objectCoord(int id_coordinate, QString ObjectsTypes, int id_object)
{
    QString str;
    QSqlQuery query;
    if((ObjectsTypes == "region") || (ObjectsTypes == "groups")){
        str = QString("INSERT INTO coord_%3 VALUES (DEFAULT , %1 , %2)").arg(id_coordinate).arg(id_object).arg(ObjectsTypes);
    }else{
        str = QString("INSERT INTO coord_%3 VALUES (DEFAULT , %2 , %1)").arg(id_coordinate).arg(id_object).arg(ObjectsTypes);
    }
    if(!query.exec(str)){
        qDebug() << query.lastError().text();
        qDebug() << query.lastQuery();
        query.clear();
        return false;
    }
    query.clear();
    return true;
}

QList<Coord*> CoordModel::getCoordinates(QString ObjectsTypes, int id_object)
{
    QList<Coord*> coordList;

    QSqlQuery query;
    QString str=QString("SELECT cc.latitude_wgs_84_g,cc.latitude_wgs_84_m,cc.latitude_wgs_84_s,cc.longitude_wgs_84_g, \
                        cc.longitude_wgs_84_m,cc.longitude_wgs_84_s \
                        FROM %2 c_e, coordinates cc \
                        WHERE c_e.id_coordinates = cc.id_coordinates \
            AND c_e.id_%2 = %1").arg(id_object).arg(ObjectsTypes);
            if(query.exec(str))
    {
            QSqlRecord rec = query.record();
            while (query.next())
    {
            int wgs_g = query.value(rec.indexOf("latitude_wgs_84_g")).toInt();
    int wgs_m = query.value(rec.indexOf("latitude_wgs_84_m")).toInt();
    double wgs_s = query.value(rec.indexOf("latitude_wgs_84_s")).toDouble();
    int long_wgs_g = query.value(rec.indexOf("longitude_wgs_84_g")).toInt();
    int long_wgs_m = query.value(rec.indexOf("longitude_wgs_84_m")).toInt();
    double long_wgs_s = query.value(rec.indexOf("longitude_wgs_84_s")).toDouble();

    Coord *coord = new Coord(wgs_g,wgs_m,wgs_s,long_wgs_g,long_wgs_m,long_wgs_s);
    coordList.append(coord);
}
}
return coordList;
}
