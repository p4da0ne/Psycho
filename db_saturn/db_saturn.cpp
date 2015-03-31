#include "db_saturn.h"


#if defined Q_OS_WIN
#define kodec QTextCodec::setCodecForCStrings(QTextCodec::codecForName("Windows-1251"));
#else
#define kodec QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
#endif
//
db_saturn::db_saturn(DataAccess *parent)
    : DataAccess(parent){
	db_st= new db_struct();
	QString name=db_st->military_rank.name_table;
	QString str = "SELECT " + db_st->military_rank.id_military_rank + "," + db_st->military_rank.rank_name+ " FROM "+db_st->military_rank.name_table;
}

db_saturn::~db_saturn()
{
    
}


QMap<int, QString> db_saturn::get_all_military_rank(){
	//QSqlQuery query;
	//QString str = "SELECT id_military_rank,rank_name FROM military_rank";
	//query.exec(str);
	//QSqlRecord data = query.record();
	//QString rang;
	//int id_rang;
	//int id_rank;
	//while(query.next())
	//{
	//	rang = query.value(data.indexOf("rank_name")).toString();
	//	id_rank = query.value(data.indexOf("id_military_rank")).toInt();
	//	military_rang->addItem(rang,id_rank);
	//}
	//return military_rang;
	QMap<int, QString> map;
	map[1]="один";
	return map;
}