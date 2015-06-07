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
    QMap <int, QMap< QString,QString> > obj_data;
    obj_data = report_model->pers_info(id_object); // начало по персоналиям

    QMap<QString,QString> obj_data_vzv; // таблица по званиям
    //obj_data_vzv = report_model->pers_info(id_object);

    QMap<QString,QString> obj_data_trud; // таблица по трудовой деятельности
    //obj_data_trud = report_model->pers_info(id_object);

    QMap<QString,QString> obj_data_vid; // таблица по прохождению видов гос. службы
    //obj_data_vid = report_model->pers_info(id_object);

    QMap<QString,QString> obj_data_war; // таблица участие в войнах
    //obj_data_war = report_model->pers_info(id_object);

    QMap<QString,QString> obj_data_foto; // фотка
    obj_data_foto = report_model->pers_info_foto(id_object);

    QMap<QString,QString> obj_data_travma; //таблица увечья
    obj_data_travma = report_model->region_info_factor(id_object);

    QMap<QString,QString> obj_data_medal; //таблица нагрудные знаки
  //  obj_data_medal = report_model->region_info_factor(id_object);

    QMap<QString,QString> obj_data_plen; //таблица плен
    obj_data_plen = report_model->region_info_factor(id_object);

    QMap<QString,QString> obj_data_kompromat; //таблица плен
    obj_data_kompromat = report_model->region_info_factor(id_object);
//=============== координаты персоны =============================
//    QMap<QString,QMap<QString,QString> > obj_data2;
//    obj_data2 = report_model->pers_info_coord(id_object);
//================================================================

    QMap <int, QMap< QString,QString> > obj_data_continue;
    obj_data_continue = report_model->pers_info_continue(id_object);

    QStringList list,list_trud,list_vid,list_war,list_medal,list_komro;
    list << "Воинские звания \n (специальные звания, \n классные чины )" << "Правовой акт о присвоении \n воинского звания \n (специального звания, классного чина), \n его дата и номер";
    list_trud << "С какого времени (число, \n месяц и год)"<< "По какое время (число, \n месяц и год)" << "Место работы, занимаемая должность" << "примечание";
    list_vid << "С какого времени (число, \n месяц и год)"<< "По какое время (число, \n месяц и год)" << "Должность, номер ВУС (код специальности), в/зв (специальное звание по штату), тарифный разряд (должностной оклад)" << "Воинская часть, орган управления, учреждение, заведение, соединение, армия, группа войск, фронт или военнй округ, флот или флотилия" << "Чей приказ, дата и номер приказа";
    list_war << "Участие в войнах и других боевых действиях"<< "С какого времени (число, \n месяц и год)"<< "По какое время (число, \n месяц и год)" ;
    list_medal << "Наименование нагрудного знака"<< "За что награжден" << "Чей приказ, его дата и номер";
    list_komro << "Дата" << "Обстоятельства";

    Output *out = new Output;

    QString s = out->createHtmlHeader();
    s.append(out->createHtmlH_pers(id_object));
    s.append(out->createHtmlH("ПОСЛУЖНОЙ СПИСОК", 3));
    s.append(out->createHtmlTable_row_foto(obj_data_foto));
    s.append(out->createHtmlH_pers_2_page(id_object));
    s.append(out->createHtmlTable_vzv(obj_data_vzv,list));
    s.append(out->createHtmlTable_2(obj_data));
    s.append(out->createHtmlH("11. Самостоятельная трудовая деятельность до государственной службы", 3, "left"));
    s.append(out->createHtmlTable_vzv(obj_data_trud,list_trud));
    s.append(out->createHtmlH("12. Прохождение видов государственной службы", 3, "left"));
    s.append(out->createHtmlTable_vzv(obj_data_trud,list_vid));
    s.append(out->createHtmlH("13. Участие в войнах и других боевых действиях", 3, "left"));
    s.append(out->createHtmlTable_vzv(obj_data_war,list_war));
    s.append(out->createHtmlH("14. Увечья (ранения, травмы, контузии) и другие боевые поражения, их характер. Когда и где получены", 3, "left"));
    s.append(out->createHtmlTable_row(obj_data_travma));
    s.append(out->createHtmlH("15. Какими нагрудными знаками награжден", 3, "left"));
    s.append(out->createHtmlTable_vzv(obj_data_medal,list_medal));
    s.append(out->createHtmlH("16. Был ли в плену, при каких обстоятельствах, где и когда пленен и освобожден из плена", 3, "left"));
    s.append(out->createHtmlTable_row(obj_data_plen));
    s.append(out->createHtmlTable_2(obj_data_continue));
    s.append(out->createHtmlH("20. Компрометирующие данные на военнослужащего", 3, "left"));
    s.append(out->createHtmlTable_vzv(obj_data_kompromat,list_komro));

 // s.append(out->createHtmlTable_3(obj_data));
 // s.append(out->createHtmlTableM(obj_data2));

    s.append(out->createHtmlFooter_pers(id_object));
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

