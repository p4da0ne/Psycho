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
    QMap <int, QMap< QString,QString> > obj_data;
    obj_data = report_model->get_obj_info(id_object);

    QMap<QString,QMap<QString,QString> > obj_data2;
    obj_data2 = report_model->obj_info_coord(id_object);
    Output *out = new Output;

    QString s = out->createHtmlHeader();
    s.append(out->createHtmlH("Формуляр организации", 2));
    s.append(out->createHtmlTable_2(obj_data));
    s.append(out->createHtmlTableM(obj_data2));
    s.append(out->createHtmlFooter());
    return s;
}
QString Reports::create_object_formular_pers(int id_object)
{
    ReportData *report_model = new ReportData;
    QMap<QString,QString> obj_data;
    obj_data = report_model->pers_info(id_object);

    QMap<QString,QMap<QString,QString> > obj_data2;
    obj_data2 = report_model->pers_info_coord(id_object);

    Output *out = new Output;

    QString s = out->createHtmlHeader();
    s.append(out->createHtmlH("Формуляр персоналии",2));
    s.append(out->createHtmlTable_3(obj_data));
    s.append(out->createHtmlTableM(obj_data2));
    s.append(out->createHtmlFooter());
    return s;
}

QString Reports::create_object_formular_smi(int id_object)
{
    ReportData *report_model = new ReportData;
    QMap <int, QMap< QString,QString> > obj_data;
    obj_data = report_model->smi_info(id_object);

    Output *out = new Output;

    QString s = out->createHtmlHeader();
    s.append(out->createHtmlH("Формуляр СМИ", 2));
    s.append(out->createHtmlTable_2(obj_data));
    s.append(out->createHtmlFooter());
    return s;
}

QString Reports::create_object_formular_ls(int id_object)
{
    ReportData *report_model = new ReportData;
    QMap <int, QMap< QString,QString> > obj_data;
    obj_data = report_model->ls_info(id_object);

    QMap<QString,QMap<QString,QString> > obj_data2;
    obj_data2 = report_model->ls_info_coord(id_object);

    Output *out = new Output;

    QString s = out->createHtmlHeader();
    s.append(out->createHtmlH("Формуляр Воинского формирования", 2));
    s.append(out->createHtmlTable_2(obj_data));
    s.append(out->createHtmlTableM(obj_data2));
    s.append(out->createHtmlFooter());
    return s;
}

QString Reports::create_object_formular_mpo_pso_smi(int id_object)
{
    ReportData *report_model = new ReportData;
    QMap <int, QMap< QString,QString> > obj_data;
    obj_data = report_model->mpo_pso_smi_info(id_object);

    QMap<QString,QMap<QString,QString> > obj_data2;
    obj_data2 = report_model->mpo_pso_info_coord(id_object);

    Output *out = new Output;

    QString s = out->createHtmlHeader();
    s.append(out->createHtmlH("Формуляр средства СМИ", 2));
    s.append(out->createHtmlTable_2(obj_data));
    s.append(out->createHtmlTableM(obj_data2));
    s.append(out->createHtmlFooter());
    return s;
}
QString Reports::create_object_formular_mpo_pso_ls(int id_object)
{
    ReportData *report_model = new ReportData;
    QMap <int, QMap< QString,QString> > obj_data;
    obj_data = report_model->mpo_pso_ls_info(id_object);

    QMap<QString,QMap<QString,QString> > obj_data2;
    obj_data2 = report_model->mpo_pso_info_coord(id_object);

    Output *out = new Output;

    QString s = out->createHtmlHeader();
    s.append(out->createHtmlH("Формуляр средства Воинского формирования", 2));
    s.append(out->createHtmlTable_2(obj_data));
    s.append(out->createHtmlTableM(obj_data2));
    s.append(out->createHtmlFooter());
    return s;
}
QString Reports::create_object_formular_mpo_pso_gr(int id_object)
{
    ReportData *report_model = new ReportData;
    QMap <int, QMap< QString,QString> > obj_data;
    obj_data = report_model->mpo_pso_gr_info(id_object);

    QMap<QString,QMap<QString,QString> > obj_data2;
    obj_data2 = report_model->mpo_pso_info_coord(id_object);

    Output *out = new Output;

    QString s = out->createHtmlHeader();
    s.append(out->createHtmlH("Формуляр средства организации", 2));
    s.append(out->createHtmlTable_2(obj_data));
    s.append(out->createHtmlTableM(obj_data2));
    s.append(out->createHtmlFooter());
    return s;
}
QString Reports::create_object_formular_sc(int id_object)
{
    ReportData *report_model = new ReportData;
    QMap <int, QMap< QString,QString> > obj_data;
    obj_data = report_model->sc_info(id_object);

    QMap<QString,QMap<QString,QString> > obj_data2;
    obj_data2 = report_model->sc_info_coord(id_object);

    Output *out = new Output;

    QString s = out->createHtmlHeader();
    s.append(out->createHtmlH("Формуляр особого условия", 2));
    s.append(out->createHtmlTable_2(obj_data));
    s.append(out->createHtmlTableM(obj_data2));
    s.append(out->createHtmlFooter());
    return s;
}
QString Reports::create_object_formular_region(float rez_,int id_object)
{
    ReportData *report_model = new ReportData;
    QMap <int, QMap< QString,QString> > obj_data;
    obj_data = report_model->region_info(id_object);

    QMap<int,QMap<QString,QString> > obj_data2;
    obj_data2 = report_model->region_info_pop(id_object);

    QMap<int,QMap<QString,QString> > obj_data3;
    obj_data3 = report_model->region_info_smi(id_object);

    QMap<int,QMap<QString,QString> > obj_data4;
    obj_data4 = report_model->region_info_group(id_object);

    QMap<QString,QString> obj_data5;
    obj_data5 = report_model->region_info_factor(id_object);

    QMap<QString,QString> obj_data6;
    obj_data6 = report_model->region_info_itog(rez_);

    Output *out = new Output;

    QString s = out->createHtmlHeader();
    s.append(out->createHtmlH("Формуляр региона (района)", 2));
    s.append(out->createHtmlTable_2(obj_data));
    s.append(out->createHtmlH("1. НАСЕЛЕНИЕ", 2));
    s.append(out->createHtmlTable_2(obj_data2));
    s.append(out->createHtmlH("2. СМИ", 2));
    s.append(out->createHtmlTable_2(obj_data3));
    s.append(out->createHtmlH("3. ОРГАНИЗАЦИИ", 2));
    s.append(out->createHtmlTable_2(obj_data4));
    s.append(out->createHtmlH("4. ФАКТОРЫ", 2));
    s.append(out->createHtmlTable_row(obj_data5));
    s.append(out->createHtmlH("5. ВЫВОД", 2));
    s.append(out->createHtmlTable_row(obj_data6));
    s.append(out->createHtmlFooter());
    return s;
}
void Reports::show_preview_dialog(QString html)
{
 ReportView *view = new ReportView;
 view->preview_dialog(html);
}
void Reports::show_preview_dialog1(QString html)
{
 ReportView *view = new ReportView;
 view->print_formul(html);
}
void Reports::show_preview_dialog2(QString html)
{
 ReportView *view = new ReportView;
 view->print_pdf_formul(html);
}

