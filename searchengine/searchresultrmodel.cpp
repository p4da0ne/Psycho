#include "searchresultrmodel.h"


SearchResultModel::SearchResultModel(QObject *parent) :
    QStandardItemModel(parent)
{

    
}



//=====================================================================================
//==== Конструктор копирования ========================================================
//=====================================================================================
SearchResultModel::SearchResultModel(const SearchResultModel &model, QObject *parent) :
    QStandardItemModel(parent)
{
	QStandardItemModel *resModel = new QStandardItemModel;

	for(int row=0;row<model.rowCount();row++)
	{
		QStandardItem *item = new QStandardItem;;
		QModelIndex index = model.index(row,0);
		item->setData(model.data(index,Qt::DisplayRole));
		item->setData(model.data(index,Qt::UserRole));
		item->setData(model.data(index,Qt::UserRole+1));
		
		resModel->appendRow(item);
	}
    
}

//================================================================================
//====== Перегрузка оператора "+=" =======
//================================================================================
SearchResultModel& SearchResultModel::operator += (SearchResultModel &model)
{
	for(int row=0;row<model.rowCount();row++)
	{
		QStandardItem *item = new QStandardItem;;
		QModelIndex index = model.index(row,0);
		item->setData(model.data(index,Qt::DisplayRole));
		item->setData(model.data(index,Qt::UserRole));
		item->setData(model.data(index,Qt::UserRole+1));
		
		appendRow(item);
	}
	return (*this);
}