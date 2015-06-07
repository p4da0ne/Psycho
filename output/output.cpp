#include "output.h"
#include "QMap"
#include "QStringList"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QVariant>

#if defined Q_OS_WIN
#define kodec QTextCodec::setCodecForCStrings(QTextCodec::codecForName("Windows-1251"));
#else
#define kodec QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
#endif
//
Output::Output(QObject *parent)
    : QObject(parent)
{
  
    
}

Output::~Output()
{
    
}
//===== Формирование заголовка HTML-документа ======
QString Output::createHtmlHeader()
{
	QString str;
    str="<html><head> </head><body><FONT FACE = 'Times new Roman'>";
	return str;
}
//===== Формирование заголовка документа ======
QString Output::createHtmlH(QString text,int size,QString align)
{   
	if(size <1) size = 1;
	if(size >6) size = 6;
    QString t = "<h" + QString::number(size) + " align='"+ align+ "'>" + text + "</h" + QString::number(size) + ">";


    return t;
}
QString Output::createHtmlH_p(QString text,int size,QString align)
{
    if(size <1) size = 1;
    if(size >6) size = 6;
    QString t = "<h" + QString::number(size) + " align='"+ align+ "'>" + text + "</h" + QString::number(size) + "><FONT FACE = 'Times new Roman'>";


    return t;
}
QString Output::createHtmlH_pers_2_page(int id_object)
{
    QString t;
    QSqlQuery query;
    query.prepare ("SELECT pers.name_persones,pers.age_persones,pers.contact_persones, pers.description_persones, pers.authority_persones, pers.opposition_persones, pers.rank_persones, type_persones.name_type_persones FROM  persones pers, type_persones WHERE pers.id_persones = ? AND pers.id_type_persones = type_persones.id_type_persones ");

    query.addBindValue(id_object);
    if(!query.exec())
    {
        QString sss = query.lastError().text();
        return t;
    }
    QSqlRecord rec = query.record();
    QString f_name,rank;

    query.next();

       f_name = query.value(rec.indexOf("name_persones")).toString();
       rank = query.value(rec.indexOf("rank_persones")).toString();
       t = "<br><br><br>"

           "<u><center><FONT size='5' FACE = 'Times new Roman'>" + f_name + "</font></u></center><br><br>"
           "<center><p><FONT size='5' FACE = 'Times new Roman'> Личный номер <u>" + rank + "</font></u></p></center>"
           "<br>";
    return t;
}
QString Output::createHtmlH_pers(int id_object)
{
    QString t;
    QSqlQuery query;
    query.prepare ("SELECT pers.name_persones,pers.age_persones,pers.contact_persones, pers.description_persones, pers.authority_persones, pers.opposition_persones, pers.rank_persones, type_persones.name_type_persones FROM  persones pers, type_persones WHERE pers.id_persones = ? AND pers.id_type_persones = type_persones.id_type_persones ");

    query.addBindValue(id_object);
    if(!query.exec())
    {
        QString sss = query.lastError().text();
        return t;
    }
    QSqlRecord rec = query.record();
    QString f_name,rank;

    query.next();

       f_name = query.value(rec.indexOf("name_persones")).toString();
       rank = query.value(rec.indexOf("rank_persones")).toString();
        //     <style type='text/css'> div{border:3px solid #D3D3D3;}</style>
       t = // "<div>" // style = margin-top:120px;margin-bottom:120px;>"
               "<h5 align='right'> _________________ </h5>"
           "<h5 align='right'> <FONT FACE = 'Times new Roman'> (гриф секретности)</h5>"
           "<h5 align='right'> <FONT FACE = 'Times new Roman'> Экз. № __________ </h5> <br><br><br><br><br><br>"

           "<h1 align='center'><b><FONT FACE = 'Times new Roman'> ЛИЧНОЕ ДЕЛО </b></h1><br><br>"

           "<h2 align='center'> <FONT FACE = 'Times new Roman'><b> должность </b></FONT></h2>"
           "<h2 align='center'> <FONT FACE = 'Times new Roman'>" + rank +" </FONT></h2> <br>"

           "<table align='center' width=50% border='1' cellspacing=0 cellpadding=0>"
           "<CENTER><tr align='center' ><td ><CENTER><FONT size='10' FACE = 'Times new Roman'>" + f_name + "</FONT></CENTER></td></tr></table>"
           "<CENTER><FONT SIZE = '2' FACE = 'Times new Roman'> (фамилия) <br>"

           "<table align='center' width=50%  border='1' cellspacing=0 cellpadding=0>"
           "<tr align='center' ><td><CENTER><CENTER><FONT size='10' FACE = 'Times new Roman'> name_name + surname  </FONT></CENTER></td></tr></table>"

           "<CENTER><FONT SIZE = '2' align='center' FACE = 'Times new Roman'> (имя, отчество) <br><br><br><br><br><br><br>"
             //  "</div>";
            "<br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br></center>";
    return t;
}
//
QString Output::createHtmlTable_vzv(QMap<QString,QString> table_data, QStringList header_list,int width)
{
    QString r;
    r.append("<FONT size='4' FACE = 'Times new Roman'><TABLE WIDTH ='"+ QString::number(width)+ "%' BORDER='1' ><tr>");
//	r.append
    QList<QString>::iterator sp = header_list.begin();
    while (sp != header_list.end())
    {
        r.append("<th>");
        r.append(*sp);
        r.append("</th>");
        ++sp;
    }
    r.append("</tr>");
    QMap<QString,QString>::iterator it=table_data.begin();
    for (;it !=table_data.end(); ++it)
    {
        r.append("<TR> <TD>");
        r.append(it.key());
        r.append("</TD> <TD>");
        r.append(it.value());
        r.append("</TD></TR>");
    }
    r.append("</table></font>");
return r;
}

//===== Формирование текста в документе ======
QString Output::createHtmlP(QString text,  int weight_italic,QString align)
{	
	QString weight_italic1;QString weight_italic2;

	if (weight_italic ==1) {weight_italic2="<b>" ; weight_italic1="</b>";}       //b
	if (weight_italic ==2) {weight_italic2="<i>"; weight_italic1="</i>";}        //i
	if (weight_italic ==3) {weight_italic2="<b><i>"; weight_italic1="</b></i>";} //bi
	if ((weight_italic< 1)&&(weight_italic>3)) {weight_italic2=""; weight_italic1="";}      //
	QString p = weight_italic2 + "<p align='" +  align + "'>" + text + " </p>" +weight_italic1 ;
	return p;
}
QString Output::createHtmlTable_3(QMap<QString,QString> table_data,int width)
{
    QString r;
  //  r.append("<TABLE WIDTH ='"+ QString::number(width)+ "%' BORDER='1' cellpadding=4 align = center>");
    r.append("<TABLE style=\"border-color:#808080;\" border=\"1\" cellpadding=\"4\" cellspacing=\"1\" width='"+ QString::number(width)+ "%'>");


    QMap<QString,QString>::iterator it=table_data.begin();
    for (;it !=table_data.end(); ++it)
    {
        r.append("<TR> <TD>");
        r.append(it.key());
        r.append("</TD> <TD>");
        r.append(it.value());
        r.append("</TD></TR>");
    }
    r.append("</table>");
return r;
}

QString Output::createHtmlTable_2(QMap <int, QMap< QString,QString> > table_data,int width)
{
    QString r;
    r.append("<FONT size='4' FACE = 'Times new Roman'><TABLE WIDTH ='"+ QString::number(width)+ "%' BORDER='1'  cellpadding=4  align = center> ");

    QMap<QString,QString> map;
    QMap <int, QMap< QString,QString> >::iterator it=table_data.begin();
    for (;it !=table_data.end(); ++it)
    {
        map=it.value();

        QMap<QString,QString> :: iterator it1=map.begin();
        for (;it1 !=map.end(); ++it1)
        {
         r.append("<TR><TD>");
         r.append(it1.key());
         r.append("</TD><TD>");
         r.append(it1.value());
         r.append("</TD></TR>");
        }

       }
        r.append("</table></font>");
return r;
}
QString Output::createHtmlTable_row(QMap<QString,QString> table_data,int width)
{
    QString r;
     r.append("<FONT size='4' FACE = 'Times new Roman'><TABLE style=\"border-color:#808080;\" border=\"1\" cellpadding=\"4\" cellspacing=\"1\" width='"+ QString::number(width)+ "%'>");

    QMap<QString,QString>::iterator it=table_data.begin();
    for (;it !=table_data.end(); ++it)
    {
        r.append("<TR><TD>");
        r.append(it.value());
        r.append("</TR></TD>");
    }
    r.append("</TABLE></font>");
return r;
}
QString Output::createHtmlTable_row_foto(QMap<QString,QString> table_data,int width)
{
    QString r;
     r.append("<br><TABLE style=\ border=\"0\" cellpadding=\"4\" cellspacing=\"1\" width='"+ QString::number(width)+ "%'>");

    QMap<QString,QString>::iterator it=table_data.begin();
    for (;it !=table_data.end(); ++it)
    {
        r.append("<TR><TD>");
        r.append(it.value());
        r.append("</TR></TD>");
    }
    r.append("</TABLE>");
return r;
}

//===== Формирование таблицы на основе двумерного массива и списка ======
QString Output::createHtmlTable(QMap<QString,QString> table_data, QStringList header_list,int width)
{
	QString r;
	r.append("<TABLE WIDTH ='"+ QString::number(width)+ "%' BORDER='1' ><tr>");
//	r.append
	QList<QString>::iterator sp = header_list.begin();
	while (sp != header_list.end())
	{
		r.append("<th>");
		r.append(*sp);
		r.append("</th>");
		++sp;
	}
	r.append("</tr>");
	QMap<QString,QString>::iterator it=table_data.begin();
	for (;it !=table_data.end(); ++it)
	{
		r.append("<TR> <TD>");
		r.append(it.key());
        r.append("</TD> <TD>");
		r.append(it.value());
		r.append("</TD></TR>");
	}
	r.append("</table>");
return r;
}
//======= Формирование списков (маркированный\нумерованный) ========
QString Output::createHtmlLi(QStringList list, int type, int type_style)
{
	QString ss, type_n_m, type_n_m2;
//
	switch (type) 
	{
		case 0:
		type_n_m="ol";
		switch (type_style) 
		{
		case 1: type_n_m2=" type = A";break;
		case 2: type_n_m2=" type = a";break;
		case 3: type_n_m2=" type = I";break;
		case 4: type_n_m2=" type = i";break;
		default : type_n_m2=" type = 1";
		}
		default:
		type_n_m="ul";
		switch (type_style) 
		{
		case 1: type_n_m2=" type = circle";break;
		case 2: type_n_m2=" type = square";break;
		default : type_n_m2=" type = disc";
		}
	}


//
	//if (type==0) type_n_m="ol"; else type_n_m="ul";
	//ss.append("<" + type_n_m + ">");
	ss.append("<" + type_n_m + type_n_m2 +">");
	QList<QString>::iterator sp =list.begin();
	while (sp != list.end())
	{
		ss.append("<li>");
		ss.append(*sp);
		ss.append("</li>");
		++sp;
	}
	ss.append("</" + type_n_m + ">");
return ss;
}
//===== Формирование таблицы на основе массива QMap<QString,QMap<QString,QString>> (_,_,_) ======
QString Output::createHtmlTableM(QMap<QString, QMap<QString, QString> > table_data,  int width)
{
    QString r;

    r.append("<TABLE style=\"border-color:#808080;\" border=\"1\" cellpadding=\"4\" cellspacing=\"1\" width='"+ QString::number(width)+ "%'>");

    QMap<QString,QString> map;

    QMap<QString,QMap<QString,QString> >::iterator it=table_data.begin();
    for (;it !=table_data.end(); ++it)
    {
        r.append("<TR><TD colspan=2 >");//width = 100%
        r.append(it.key());
        r.append("</TD></TR>");
        map=it.value();

        QMap<QString,QString> :: iterator it1=map.begin();
        for (;it1 !=map.end(); ++it1)
        {
        r.append("<TR><TD width=35%>");
        r.append(it1.key());
        r.append("</TD><TD width=65%>");
        r.append(it1.value());
        r.append("</TD ></TR>");
        }
    }
    r.append("</table>");
return r;
}

QString Output::createHtmlFooter_pers(int id_object)
{

    QString close;
    close = "<br> <p><FONT size='4'> Послужной список составлен:"
            "<u>  ДАТА </font></u></p>"

            "<table align='center' width=50%  border='0' cellspacing=0 cellpadding=0>"
            "<tr align='center' ><td><u><center><FONT size='4'> name_name + surname  </FONT></u></td></tr></table>"
            "<CENTER><FONT SIZE = '2' align='center' FACE = 'Times new Roman'> (наименование воинской части, соединения и т.д.)</font></center>"
            "<br>"
            "</font></body></html>";
    return close;
}
//===== Закрытие HTML-документа ======
QString Output::createHtmlFooter()
{
	QString close;
    close="</font></body></html>";
	return close;
}


