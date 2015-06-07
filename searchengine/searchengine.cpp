#include "searchengine.h"
#include <saturnSignTypes.h>

SearchEngine::SearchEngine(QObject *parent)
    : QObject(parent)
{
	searchResultModel = new QStandardItemModel;
}


//=======================================================================
//==== Метод поиска объектов по имени (части имени) в таблицах: =========
//==== 1. Персоналий.
//==== 2. Формирований.
//==== 3. Средств.
//==== 4. Регионов. 
//=======================================================================
QStandardItemModel* SearchEngine::findObjects(QString objNamePart)
{
	findPersones(objNamePart);
	findFormations(objNamePart);
	findMeans(objNamePart);
	findRegions(objNamePart);
	findSpecialConditions(objNamePart);
	findSMI(objNamePart);
	findGroups(objNamePart);

	return searchResultModel;
}

//========================================================================
//======= Метод поиска персоналий ========================================
//========================================================================
QStandardItemModel* SearchEngine::findPersones(QString objNamePart)
{
	//---------------------------------------------------
	QSqlQuery query;
	QString str = QString("SELECT ps.id_persones, tp.name_type_persones, ps.name_persones, ps.image_persones \
					FROM persones ps, type_persones tp \
					WHERE ps.id_type_persones = tp.id_type_persones \
					AND ps.name_persones ILIKE '%%1%' \
					ORDER BY ps.name_persones").arg(objNamePart);

	
	if(!query.exec(str))
	{
		QString err = query.lastError().text();
		return searchResultModel;
	}

	QSqlRecord rec = query.record();

	while(query.next())
	{
		QStandardItem *item = new QStandardItem;
		item->setData(query.value(rec.indexOf("id_persones")).toInt(),Qt::UserRole);
		item->setData(PERSONNEL,Qt::UserRole+1);
		QString personName = query.value(rec.indexOf("name_persones")).toString() + " (" + query.value(rec.indexOf("name_type_persones")).toString() + ")";
		item->setData(personName,Qt::DisplayRole);
		item->setData(personName,Qt::ToolTipRole);
		item->setCheckable(true);
		item->setCheckState(Qt::Unchecked);
		searchResultModel->appendRow(item);
	}

	//---------------------------------------------------

	return searchResultModel;
}


//========================================================================
//======= Метод поиска формирований ========================================
//========================================================================
QStandardItemModel* SearchEngine::findFormations(QString objNamePart)
{
	//---------------------------------------------------
	QSqlQuery query;
	QString str = QString("SELECT ls.id_ls, tl.name_type_ls, ls.name_ls \
							FROM ls, type_ls tl \
							WHERE ls.id_type_ls = tl.id_type_ls \
							AND ls.name_ls ILIKE '%%1%' \
							ORDER BY ls.name_ls").arg(objNamePart);
	
	if(!query.exec(str))
	{
		QString err = query.lastError().text();
		return searchResultModel;
	}

	QSqlRecord rec = query.record();

	while(query.next())
	{
		QStandardItem *item = new QStandardItem;
		item->setData(query.value(rec.indexOf("id_ls")).toInt(),Qt::UserRole);
		item->setData(FORMATIONS,Qt::UserRole+1);
		QString lsName = query.value(rec.indexOf("name_ls")).toString() + " (" + query.value(rec.indexOf("name_type_ls")).toString() + ")";
		item->setData(lsName,Qt::DisplayRole);
		item->setData(lsName,Qt::ToolTipRole);
		item->setCheckable(true);
		item->setCheckState(Qt::Unchecked);
		searchResultModel->appendRow(item);
	}

	//---------------------------------------------------

	return searchResultModel;
}

//========================================================================
//======= Метод поиска средств (СМИ, формирований, организаций) ==========
//========================================================================
QStandardItemModel* SearchEngine::findMeans(QString objNamePart)
{
	//---------------------------------------------------
	QSqlQuery query;
	QString str = QString("SELECT mpo.id_mpo_pso, tm.name_type_mpo_pso, mpo.name_mpo_pso \
							FROM mpo_pso mpo, type_mpo_pso tm \
							WHERE mpo.id_type_mpo_pso = tm.id_type_mpo_pso \
							AND mpo.name_mpo_pso ILIKE '%%1%' \
							ORDER BY mpo.name_mpo_pso").arg(objNamePart);
	
	if(!query.exec(str))
	{
		QString err = query.lastError().text();
		return searchResultModel;
	}

	QSqlRecord rec = query.record();

	while(query.next())
	{
		QStandardItem *item = new QStandardItem;
		item->setData(query.value(rec.indexOf("id_mpo_pso")).toInt(),Qt::UserRole);
		item->setData(SMI_MEANS,Qt::UserRole+1);
		QString meansName = query.value(rec.indexOf("name_mpo_pso")).toString() + " (" + query.value(rec.indexOf("name_type_mpo_pso")).toString() + ")";
		item->setData(meansName,Qt::DisplayRole);
		item->setData(meansName,Qt::ToolTipRole);
		item->setCheckable(true);
		item->setCheckState(Qt::Unchecked);
		searchResultModel->appendRow(item);
	}

	//---------------------------------------------------

	return searchResultModel;
}


//========================================================================
//======= Метод поиска регионов ==========================================
//========================================================================
QStandardItemModel* SearchEngine::findRegions(QString objNamePart)
{
	//---------------------------------------------------
	QSqlQuery query;
	QString str = QString("SELECT r.id_region, tr.name_type_region, r.name_region \
							FROM region r, type_region tr \
							WHERE tr.id_type_region = r.id_type_region \
							AND r.name_region ILIKE '%%1%' \
							ORDER BY r.name_region").arg(objNamePart);
	
	if(!query.exec(str))
	{
		QString err = query.lastError().text();
		return searchResultModel;
	}

	QSqlRecord rec = query.record();

	while(query.next())
	{
		QStandardItem *item = new QStandardItem;
		item->setData(query.value(rec.indexOf("id_region")).toInt(),Qt::UserRole);
		item->setData(REGIONS,Qt::UserRole+1);
		QString regionName = query.value(rec.indexOf("name_region")).toString() + " (" + query.value(rec.indexOf("name_type_region")).toString() + ")";
		item->setData(regionName,Qt::DisplayRole);
		item->setData(regionName,Qt::ToolTipRole);
		item->setCheckable(true);
		item->setCheckState(Qt::Unchecked);
		searchResultModel->appendRow(item);
	}

	//---------------------------------------------------

	return searchResultModel;
}


//========================================================================
//======= Метод поиска особых условий ====================================
//========================================================================
QStandardItemModel* SearchEngine::findSpecialConditions(QString objNamePart)
{
	//---------------------------------------------------
	QSqlQuery query;
	QString str = QString("SELECT s.id_special_conditions, t.name_type_special_conditions, s.name_special_conditions \
							FROM special_conditions s, type_special_conditions t \
							WHERE s.id_type_special_conditions = t.id_type_special_conditions \
							AND s.name_special_conditions ILIKE '%%1%' \
							ORDER BY s.name_special_conditions").arg(objNamePart);
	
	if(!query.exec(str))
	{
		QString err = query.lastError().text();
		return searchResultModel;
	}

	QSqlRecord rec = query.record();

	while(query.next())
	{
		QStandardItem *item = new QStandardItem;
		item->setData(query.value(rec.indexOf("id_special_conditions")).toInt(),Qt::UserRole);
		item->setData(SPECIAL_CONDITIONS,Qt::UserRole+1);
		QString conditionsName = query.value(rec.indexOf("name_special_conditions")).toString() + " (" + query.value(rec.indexOf("name_type_special_conditions")).toString() + ")";
		item->setData(conditionsName,Qt::DisplayRole);
		item->setData(conditionsName,Qt::ToolTipRole);
		item->setCheckable(true);
		item->setCheckState(Qt::Unchecked);
		searchResultModel->appendRow(item);
	}

	//---------------------------------------------------

	return searchResultModel;
}


//========================================================================
//======= Метод поиска СМИ ===============================================
//========================================================================
QStandardItemModel* SearchEngine::findSMI(QString objNamePart)
{
	//---------------------------------------------------
	QSqlQuery query;
	QString str = QString("SELECT s.id_smi, t.nametype_smi, s.name_smi \
							FROM smi s, type_smi t \
							WHERE s.id_type_smi = t.id_type_smi \
							AND s.name_smi ILIKE '%%1%' \
							ORDER BY s.name_smi").arg(objNamePart);
	
	if(!query.exec(str))
	{
		QString err = query.lastError().text();
		return searchResultModel;
	}

	QSqlRecord rec = query.record();

	while(query.next())
	{
		QStandardItem *item = new QStandardItem;
		item->setData(query.value(rec.indexOf("id_smi")).toInt(),Qt::UserRole);
		item->setData(SMI,Qt::UserRole+1);
		QString smiName = query.value(rec.indexOf("name_smi")).toString() + " (" + query.value(rec.indexOf("nametype_smi")).toString() + ")";
		item->setData(smiName,Qt::DisplayRole);
		item->setData(smiName,Qt::ToolTipRole);
		item->setCheckable(true);
		item->setCheckState(Qt::Unchecked);
		searchResultModel->appendRow(item);
	}

	//---------------------------------------------------

	return searchResultModel;
}


//========================================================================
//======= Метод поиска организаций ===============================================
//========================================================================
QStandardItemModel* SearchEngine::findGroups(QString objNamePart)
{
	//---------------------------------------------------
	QSqlQuery query;
	QString str = QString("SELECT gr.id_groups, gr.name_groups, r.name_region	\
							FROM groups gr, region r \
							WHERE gr.id_region = r.id_region \
							AND gr.name_groups ILIKE '%%1%' \
							ORDER BY gr.name_groups").arg(objNamePart);
	
	if(!query.exec(str))
	{
		QString err = query.lastError().text();
		return searchResultModel;
	}

	QSqlRecord rec = query.record();

	while(query.next())
	{
		QStandardItem *item = new QStandardItem;
		item->setData(query.value(rec.indexOf("id_groups")).toInt(),Qt::UserRole);
		item->setData(GROUPS,Qt::UserRole+1);
		QString groupName = query.value(rec.indexOf("name_groups")).toString() + " (" + query.value(rec.indexOf("name_region")).toString() + ")";
		item->setData(groupName,Qt::DisplayRole);
		item->setData(groupName,Qt::ToolTipRole);
		item->setCheckable(true);
		item->setCheckState(Qt::Unchecked);
		searchResultModel->appendRow(item);
	}

	//---------------------------------------------------

	return searchResultModel;
}