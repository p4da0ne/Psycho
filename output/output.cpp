#include "output.h"
#include "QMap"
#include "QStringList"

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
//===== ‘ормирование заголовка HTML-документа ======
QString Output::createHtmlHeader()
{
	QString str;
	str="<html><head> </head><body>";
	return str;
}
//===== ‘ормирование заголовка документа ======
QString Output::createHtmlH(QString text,int size,QString align)
{   
	if(size <1) size = 1;
	if(size >6) size = 6;
	QString t = "<h" + QString::number(size) + " align='"+ align+ "'>" + text + "</h" + QString::number(size) + ">";
	return t;
}



//===== ‘ормирование текста в документе ======
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
    r.append("<TABLE WIDTH ='"+ QString::number(width)+ "%' BORDER='1'  cellpadding=4  align = center> ");

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
        r.append("</table>");
return r;
}


//===== ‘ормирование таблицы на основе двумерного массива и списка ======
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
//======= ‘ормирование списков (маркированный\нумерованный) ========
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
//===== ‘ормирование таблицы на основе массива QMap<QString,QMap<QString,QString>> (_,_,_) ======
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
//===== «акрытие HTML-документа ======
QString Output::createHtmlFooter()
{
	QString close;
	close="</body></html>";
	return close;
}


