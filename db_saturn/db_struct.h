#ifndef DB_STRUCT_H
#define DB_STRUCT_H


#include <QObject>
#include <QString>


class db_struct : public QObject
{
	Q_OBJECT
public:
	db_struct();
	~db_struct();
	class MILITARYRANK{
		public: 
			QString name_table;
			QString id_military_rank;
			QString	rank_name;
	};
	MILITARYRANK military_rank;
};


#endif // DB_STRUCT_H
