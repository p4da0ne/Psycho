#ifndef OUTPUT_H
#define OUTPUT_H

#include "config_output.h"
#include <QObject>
#include <QMap>
#include <QStringList>


class __EXPORT_OUTPUT Output : public QObject
{
    Q_OBJECT
public:
    /// Default constructor
    explicit Output(QObject *parent=0);
    /// Destructor
    ~Output();
	QString createHtmlHeader();
    QString createHtmlFooter();
    QString createHtmlFooter_pers(int id_object);
	QString createHtmlH(QString text = "",int size = 3,QString align = "center");
    QString createHtmlH_pers(int id_object);
    QString createHtmlH_pers_2_page(int id_object);
    QString createHtmlTable_vzv(QMap<QString ,QString> table_data, QStringList header_list, int width = 100);
    QString createHtmlH_p(QString text = "",int size = 3,QString align = "center");
	QString createHtmlP(QString text = "",int weight_talic=3,QString align = "left");
	QString createHtmlTable(QMap<QString ,QString> table_data, QStringList header_list, int width = 100);
    QString createHtmlTable_2(QMap<int, QMap<QString, QString> > table_data, int width = 100);
    QString createHtmlTable_3(QMap<QString,QString> table_data,int width =100);
    QString createHtmlTable_row(QMap<QString,QString> table_data,int width =100);
    QString createHtmlTable_row_foto(QMap<QString,QString> table_data,int width =100);
    QString createHtmlLi(QStringList list, int type = 1 , int type_style = 0);
    QString createHtmlTableM(QMap<QString,QMap<QString,QString> > table_data, int width = 100);

	QString createHtmlTableFromList(QList <QStringList> table_data, QStringList headerList, int width = 100);
	

private:

    
};

#endif // OUTPUT_H
