#include "reports.h"
#include "report_data.h"
#include "report_view.h"
#include <output.h>

#include <QMap>
#include <QStringList>

Reports::Reports(QObject *parent)
    : QObject(parent)
{
  
    
}

Reports::~Reports()
{
    
}

QString Reports::create_object_formular(int id_object)
{
 ReportData *report_model = new ReportData;
 QMap<QString,QString> obj_data;

 obj_data = report_model->get_obj_info(id_object);

 Output *out = new Output;
 QString s = out->createHtmlHeader();
 QString title="‘ормул€р";
 s.append(out->createHtmlH(title));
 QStringList head;
 head<<"Attribut"<<"Mean";
 s.append(out->createHtmlTable(obj_data,head));
 s.append(out->createHtmlFooter());
return s;
}


void Reports::show_preview_dialog(QString html)
{
 ReportView *view = new ReportView;
 view->preview_dialog(html);
}