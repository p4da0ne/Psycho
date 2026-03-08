#ifndef COORD_H
#define COORD_H

#include "config_coord.h"
#include <QString>

class __EXPORT_COORD Coord 
{
 private:
	double x,y,h; //прямоугольные координаты
	int latDegrees, latMinutes, longDegrees, longMinutes; //градусы и минуты в геодезической СК
	double latSeconds, longSeconds; //секунды в геодезической СК
	double xRad, yRad; //X и Y в радианах
    double lat, lon; //WGS84 latitude longitude
public:
	Coord();
	Coord(double x, double y, double h=0);
	//Coord(double xRad, double yRad);
	Coord(int latDegrees, int latMinutes, double latSeconds, int longDegrees, int longMinutes, double longSeconds);
	~Coord();


	void setX(double x);
	void setY(double y);
	void setH(double h);
	void setXRad(double xRad);
	void setYRad(double yRad);
	void setLatitude(int degrees=0, int minutes=0, double seconds=0.0);
	void setLongitude(int degrees=0, int minutes=0, double seconds=0.0);

	double getX();
	double getY();
	double getH();
	double getXRad();
	double getYRad();
	
	int getLatDegrees();
	int getLatMinutes();
	double getLatSeconds();
	int getLongDegrees();
	int getLongMinutes();
	double getLongSeconds();

    double getLat() const { return lat; };
    double getLon() const { return lon; };

	QString latitudeToString();
	QString longitudeToString();


	bool operator ==(Coord& coord);

};

#endif // COORD_H
