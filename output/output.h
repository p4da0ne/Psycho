#ifndef OUTPUT_H
#define OUTPUT_H

#include "config_output.h"
#include <QObject>
#include "QMap"


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
	QString createHtmlH(QString text = "",int size = 3,QString align = "center");
	QString createHtmlP(QString text = "",int weight_talic=3,QString align = "left");
	QString createHtmlTable(QMap<QString ,QString> table_data, QStringList header_list, int width = 100);
	QString createHtmlLi(QStringList list, int type = 1 , int type_style = 0);

 
	

private:

    
};

#endif // OUTPUT_H
