#include "coord.h"


Coord::Coord()
{
	this->x = 0;
	this->y = 0;
	this->h = 0;
}

Coord::Coord(double x, double y, double h)
{
	this->x = x;
	this->y = y;
	this->h = h;
}


//Coord::Coord(double xRad, double yRad)
//{
//	this->xRad = xRad;
//	this->yRad = yRad;
//}


Coord::Coord(int latDegrees, int latMinutes, double latSeconds, int longDegrees, int longMinutes, double longSeconds)
{
	this->latDegrees = latDegrees;
	this->latMinutes = latMinutes;
	this->latSeconds = latSeconds;
	this->longDegrees = longDegrees;
	this->longMinutes = longMinutes;
	this->longSeconds = longSeconds;
}

Coord::~Coord()
{

}


void Coord::setX(double x)
{
	this->x = x;
}

void Coord::setY(double y)
{
	this->y = y;
}

void Coord::setH(double h)
{
	this->h = h;
}

void Coord::setXRad(double xRad)
{
	this->xRad = xRad;
}

void Coord::setYRad(double yRad)
{
	this->yRad = yRad;
}

void Coord::setLatitude(int degrees, int minutes, double seconds)
{
	this->latDegrees = degrees;
	this->latMinutes = minutes;
	this->latSeconds = seconds;
}

void Coord::setLongitude(int degrees, int minutes, double seconds)
{
	this->longDegrees = degrees;
	this->longMinutes = minutes;
	this->longSeconds = seconds;
}

double Coord::getX()
{
	return x;
}

double Coord::getY()
{
	return y;
}

double Coord::getH()
{
	return h;
}
double Coord::getXRad()
{
	return xRad;
}
double Coord::getYRad()
{
	return yRad;
}

int Coord::getLatDegrees()
{
	return latDegrees;
}

int Coord::getLatMinutes()
{
	return latMinutes;
}

double Coord::getLatSeconds()
{
	return latSeconds;
}

int Coord::getLongDegrees()
{
	return longDegrees;
}

int Coord::getLongMinutes()
{
	return longMinutes;
}

double Coord::getLongSeconds()
{
	return longSeconds;
}

QString Coord::latitudeToString()
{
	QString lat;
	lat.append(QString::number(latDegrees));
	lat.append("-");
	lat.append(QString::number(latMinutes));
	lat.append("-");
	lat.append(QString::number(latSeconds));
	return lat;
}

QString Coord::longitudeToString()
{
	QString longitude;
	longitude.append(QString::number(longDegrees));
	longitude.append("-");
	longitude.append(QString::number(longMinutes));
	longitude.append("-");
	longitude.append(QString::number(latSeconds));
	return longitude;
}

