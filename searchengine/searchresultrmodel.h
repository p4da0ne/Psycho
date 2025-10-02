#ifndef SEARCHRESULTMODEL_H
#define SEARCHRESULTMODEL_H

#include <QStandardItemModel>
#include <QList>


class SearchResultModel : public QStandardItemModel
{
    Q_OBJECT
   
public:
    SearchResultModel(QObject *parent = 0);
	SearchResultModel(const SearchResultModel &model, QObject *parent = 0);  //конструктор копирования
    
	SearchResultModel& operator += (SearchResultModel &model);
};

#endif // SEARCHRESULTMODEL_H
