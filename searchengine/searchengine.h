#ifndef SEARCHENGINE_H
#define SEARCHENGINE_H

#include "config_searchengine.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QStandardItemModel>
#include <QStandardItem>

class __EXPORT_SEARCHENGINE SearchEngine : public QObject
{
    Q_OBJECT

public:
    SearchEngine(QObject *parent = 0);
    
	QStandardItemModel* findObjects(QString objNamePart = "");

	QStandardItemModel* findPersones(QString objNamePart = "");
	QStandardItemModel* findFormations(QString objNamePart = "");
	QStandardItemModel* findMeans(QString objNamePart = "");
	QStandardItemModel* findRegions(QString objNamePart = "");
	QStandardItemModel* findSpecialConditions(QString objNamePart = "");
	QStandardItemModel* findSMI(QString objNamePart ="");
	QStandardItemModel* findGroups(QString objNamePart = "");

private:
	QStandardItemModel *searchResultModel;
};

#endif // SEARCHENGINE_H
