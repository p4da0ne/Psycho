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

    this->lat = latDegrees + latMinutes/60.0 + latSeconds/3600.0;
    this->lon = longDegrees + longMinutes/60.0 + longSeconds/3600.0;
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
	QString number = QString("%1").arg(latDegrees, 2, 10, QChar('0')); 
	lat.append(number);
	lat.append("\xB0-");
	number = QString("%1").arg(latMinutes, 2, 10, QChar('0')); 
	lat.append(number);
	lat.append("'-");
	lat.append(QString::number(latSeconds,'f',2));
	lat.append("\"");
	return lat;
}

QString Coord::longitudeToString()
{
	QString longitude;
	QString number = QString("%1").arg(longDegrees, 2, 10, QChar('0')); 
	longitude.append(number);
	longitude.append("\xB0-");
	number = QString("%1").arg(longMinutes, 2, 10, QChar('0')); 
	longitude.append(number);
	longitude.append("'-");
	longitude.append(QString::number(longSeconds,'f',2));
	longitude.append("\"");
	return longitude;
}


//================================================================
//==== Оператор проверки на равенство объектов класса координат ==
//================================================================
bool Coord::operator ==(Coord& coord)
{
	if( this->latDegrees == coord.getLatDegrees() &&
		this->latMinutes == coord.getLatMinutes() &&
		this->latSeconds == coord.getLatSeconds() &&
		this->longDegrees == coord.getLongDegrees() &&
		this->longMinutes == coord.getLongMinutes() &&
		this->longSeconds == coord.getLongSeconds() )
	{
		return true;
	}
	else if(this->x == coord.getX() &&
			this->y == coord.getY())
	{
		return true;
	}
	return false;
}
