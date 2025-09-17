#include "object_manager.h"
#include <QMenu>
#include <QMouseEvent>
#include <QAction>
#include "../dataaccess/dataaccess.h"
#include <QDialog>
#include <QMessageBox>
#include <QHeaderView>
#include <QFileDialog>
#include <QUrl>
#include <QIcon>
#include <QPainter>
#include <QLineEdit>
#include <QCheckBox>
#include <QStandardItem>
#include <QAbstractItemDelegate>
#include "simledelegate.h"
#include <QProgressDialog>
#include <QStringList>
#include <QSettings>
#include "../reports/reports.h"
#include "ui_add_object.h"
#include "ui_object_manager_form.h"
#include <QSortFilterProxyModel>
#include "mysqlrelationaldelegate.h"
#include "personesdata.h"
#include "persones_info.h"


Objectmanager::Objectmanager(QWidget *parent) //int in_id_object
    : QWidget(parent),
      UI (new Ui::object_manager_form)
{
    UI->setupUi(this);

    delegat= new simleDelegate(UI->property_object);

    /*
    id_object=in_id_object;
    p_menu=new QMenu(UI->columnView);
    p_menu->addAction(MainCodec->toUnicode("Удалить объект"));
    setWindowIcon(QIcon("./img/database.png"));
    UI->delete_button->setEnabled(false);*/
    UI->object_manager_tree->setContextMenuPolicy(Qt::CustomContextMenu);
    UI->columnView->setContextMenuPolicy(Qt::CustomContextMenu);
    UI->add_coord_button->setIcon(QIcon(":/Resources/add_but.png"));
    UI->del_coord_button->setIcon(QIcon(":/Resources/delete_but.png"));
    UI->edit_coord_button->setIcon(QIcon(":/Resources/edit_but.png"));
    UI->add_many_coord_button->setIcon(QIcon(":/Resources/open.png"));
    UI->searchButton->setDisabled(true);
    UI->searchLineEdit->setDisabled(true);

    //==============================COMBOBOX 0 строка нафиг + работа с координатами ===============================

    QListView* listView = qobject_cast<QListView*>(UI->coord_system_comboBox->view());
    Q_CHECK_PTR(listView);
    listView->setRowHidden(0, true);
    UI->add_many_coord_button->setEnabled(false);

    connect(UI->coord_system_comboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(change_coord_system(int)));
    connect(UI->add_coord_button,SIGNAL(clicked()),this,SLOT(add_new_coordinates()));
    connect(UI->del_coord_button,SIGNAL(clicked()),this,SLOT(delete_coordinates()));
    connect(UI->edit_coord_button,SIGNAL(clicked()),this,SLOT(edit_coordinates_view()));
    connect(UI->add_many_coord_button,SIGNAL(clicked()),this,SLOT(show_dialog_add_file()));
    //==============================================================================================================

    connect(UI->object_manager_tree,SIGNAL(doubleClicked(const QModelIndex &)),this,SLOT(show_objects ( const QModelIndex & )));
    connect(UI->object_manager_tree,SIGNAL(customContextMenuRequested(const QPoint &)),this,SLOT(customMenuTree(const QPoint &)));
    connect(UI->columnView,SIGNAL(clicked(QModelIndex)),this,SLOT(update_one_click(const QModelIndex &)));
    connect(UI->columnView,SIGNAL(doubleClicked(const QModelIndex &)),this,SLOT(column_item_clicked ( const QModelIndex & )));
    connect(UI->columnView,SIGNAL(customContextMenuRequested(const QPoint &)),this,SLOT(customMenuView(const QPoint &)));
    //================================== Блоки и Страны ============================================================

    connect(UI->add_pushButton_blok,SIGNAL(clicked()),this,SLOT(add_new_blok()));
    connect(UI->add_pushButton_country,SIGNAL(clicked()),this,SLOT(add_new_country()));

    //================================== Поиск =====================================================================

    connect(UI->searchButton,SIGNAL(clicked()),this,SLOT(slotSearchObject()));
    connect(UI->searchLineEdit,SIGNAL(returnPressed()),UI->searchButton,SIGNAL(clicked()));

    iconsList << ":/Resources/0.png" << ":/Resources/01.png" << ":/Resources/02.png" << ":/Resources/03.png";
    init_object_tree();
}

Objectmanager::~Objectmanager()
{
    delete UI;
}

//======= Формирование списка таблиц управления объектами =======
void Objectmanager::init_object_tree()
{
    QFont font;
    font.setBold(true);
    QString sSql;
    QSqlQuery query;
    query.exec(QString("SELECT id_blok, name_blok,emblem_blok FROM BLOK"));
    QSqlRecord rec = query.record();
    QStandardItemModel *model = new QStandardItemModel(this);
    QStandardItem *parentItem = model->invisibleRootItem();

    while (query.next()){
        QStandardItem *item = new QStandardItem(query.value(1).toString());
        int id_blok_type = query.value(0).toInt();
        QString data_type_obj="type_" + QString::number(id_blok_type);

        QPixmap pixmap;
        QSize size_pic(25,25);
        pixmap.loadFromData(query.value(2).toByteArray());
        pixmap = pixmap.scaled(size_pic,Qt::KeepAspectRatio);

        item->setData(data_type_obj,Qt::UserRole);
        item->setData(pixmap,Qt::DecorationRole);
        parentItem->appendRow(item);

        sSql = (QString("select co.name_country,co.id_country, blc.id_country, co.flag from blok_country blc,country co where co.id_country=blc.id_country and blc.id_blok=%1").arg(query.value(0).toInt()));
        QSqlQuery childrenQuery;
        if(childrenQuery.exec(sSql)){
            while(childrenQuery.next()){
                QStandardItem *item_1 = new QStandardItem(childrenQuery.value(0).toString());
                QString country_data="country_" +QString::number(childrenQuery.value(1).toInt()) + "_" + QString::number(id_blok_type);
                QPixmap pixmap_co;
                pixmap_co.loadFromData(childrenQuery.value(3).toByteArray());
                pixmap_co = pixmap_co.scaled(size_pic,Qt::KeepAspectRatio);

                item_1->setData(country_data,Qt::UserRole);
                item_1->setData(pixmap_co,Qt::DecorationRole);

                item->appendRow(item_1);
            }
        }
    }

    model->setHeaderData(0, Qt::Horizontal,"Блоки и страны");


    UI->object_manager_tree->setModel(model);
    UI->object_manager_tree->setSortingEnabled(true);
    UI->object_manager_tree->sortByColumn(0,Qt::AscendingOrder);
    query.clear();

}	

//================== контекстное меню =========================================
void Objectmanager::customMenuView(const QPoint & pos)
{
    QModelIndex index = UI->columnView->currentIndex();
    if(!index.data(Qt::UserRole).toBool()) return;
    QVariant id=index.data(Qt::UserRole);
    if (id.type() == QVariant::String){
        QString user_data=id.toString();
        QStringList list=user_data.split("_");
        if(list.value(0)=="region" || list.value(0)=="reg"){
            QPushButton *popupButton = new QPushButton;
            QMenu *menu = new QMenu(this);
            QAction *act=new QAction("Удалить регион",this);
            act->setIcon(QIcon(":/Resources/close.png"));
            connect(act,SIGNAL(triggered()),this,SLOT(delete_region()));
            QAction *act_1 = new QAction(QString("Оценка морально-психологической обстановки: %1").arg(list.value(3)),this);
            rez_z_1 = list.value(3).toFloat();
            QAction *otch23 = new QAction(QString("Сформировать отчет"),this);
            groud_id = list.value(1).toInt();
            connect(otch23,SIGNAL(triggered()),this,SLOT(otchet_groups()));

            QAction *nac_sostav = new QAction(QString("Национальный состав"),this);
            connect(nac_sostav,SIGNAL(triggered()),this,SLOT(show_nations_region()));

            QAction *confess = new QAction(QString("Религиозный состав"),this);
            connect(confess,SIGNAL(triggered()),this,SLOT(show_confess_region()));

            QAction *profess = new QAction(QString("Профессиональный состав"),this);
            connect(profess,SIGNAL(triggered()),this,SLOT(show_profess_region()));

            QAction *age = new QAction(QString("Возрастной состав"),this);
            connect(age,SIGNAL(triggered()),this,SLOT(show_age_region()));

            QAction *sekas = new QAction(QString("Половой состав"),this);
            connect(sekas,SIGNAL(triggered()),this,SLOT(show_sekas_region()));

            menu->addAction(act_1);
            menu->addSeparator();
            menu->addAction(nac_sostav);
            menu->addAction(confess);
            menu->addAction(profess);
            menu->addAction(age);
            menu->addAction(sekas);

            menu->addSeparator();
            menu->addAction(otch23);
            menu->addSeparator();
            menu->addAction(act);
            popupButton->setMenu(menu);
            menu->exec(QCursor::pos());
        }
        else if(list.value(0)=="dsmi"){
            QPushButton *popupButton = new QPushButton;
            QMenu *menu = new QMenu(this);
            QAction *act=new QAction("Удалить СМИ",this);
            act->setIcon(QIcon(":/Resources/close.png"));
            connect(act,SIGNAL(triggered()),this,SLOT(delete_smi()));

            QAction *otch_smi = new QAction (QString("Сформировать отчет"),this);
            smi_id = list.value(1).toInt();
            connect(otch_smi,SIGNAL(triggered()),this,SLOT(otchet_groups()));

            menu->addAction(otch_smi);
            menu->addSeparator();
            menu->addAction(act);
            popupButton->setMenu(menu);
            menu->exec(QCursor::pos());
        }
        else if(list.value(0)=="dls" ){
            QPushButton *popupButton = new QPushButton;
            QMenu *menu = new QMenu(this);
            QAction *act=new QAction("Удалить воинское формирование",this);
            connect(act,SIGNAL(triggered()),this,SLOT(delete_ls()));
            act->setIcon(QIcon(":/Resources/close.png"));
            QAction *otch_ls = new QAction (QString("Сформировать отчет"),this);
            ls_id = list.value(1).toInt();
            connect(otch_ls,SIGNAL(triggered()),this,SLOT(otchet_groups()));
            QAction *nac_sostav = new QAction(QString("Национальный состав"),this); //.arg(list.value(3)),this);
            connect(nac_sostav,SIGNAL(triggered()),this,SLOT(show_nations_ls()));

            menu->addSeparator();
            menu->addAction(nac_sostav);
            menu->addAction(otch_ls);
            menu->addSeparator();
            menu->addAction(act);
            popupButton->setMenu(menu);
            menu->exec(QCursor::pos());
        }
        else if((list.value(0)=="dpers") || (list.value(0)=="dperssmi") || (list.value(0)=="dpersls")) {
            QPushButton *popupButton = new QPushButton;
            QMenu *menu = new QMenu(this);
            QAction *act=new QAction("Удалить персоналию",this);
            act->setIcon(QIcon(":/Resources/close.png"));
            connect(act,SIGNAL(triggered()),this,SLOT(delete_pers()));

            QAction *otch_pers = new QAction (QString("Сформировать отчет"),this);
            connect(otch_pers,SIGNAL(triggered()),this,SLOT(otchet_groups()));
            id_persers = list.value(1).toInt();
            QAction *edit = new QAction(QString("Редактировать информацию"),this);
            connect(edit,SIGNAL(triggered()),this,SLOT(edit_persones()));

            menu->addAction(edit);
            menu->addAction(otch_pers);
            menu->addSeparator();
            menu->addAction(act);
            popupButton->setMenu(menu);
            menu->exec(QCursor::pos());
        }
        else if(list.value(0)=="lss" ){//|| list.value(0)=="chls"){
            People_Losses pl;
            pl.get_losses(list.at(1).toInt());

            QPushButton *popupButton = new QPushButton;
            QMenu *menu = new QMenu(this);

            QAction *act=new QAction(QString("Оценка морально-психологического состояния %1: %2").arg(list.value(4)).arg(list.value(3)),this);

            QMenu* subMenu_1 = new QMenu("Потери л/с",menu);

            QMenu* subMenu_1_1 = new QMenu("Интенсивность БД низкая");
            QAction *act_1_1=new QAction(QString("через 1 час: %1 - %2").arg(pl.min[0][0]).arg(pl.max[0][0]),subMenu_1);
            subMenu_1_1->addAction(act_1_1);
            QAction *act_1_2=new QAction(QString("втечение суток: %1 - %2").arg(pl.min[0][1]).arg(pl.max[0][1]),subMenu_1);
            subMenu_1_1->addAction(act_1_2);
            QAction *act_1_3=new QAction(QString("через 3-5 суток: %1 - %2").arg(pl.min[0][2]).arg(pl.max[0][2]),subMenu_1);
            subMenu_1_1->addAction(act_1_3);
            QAction *act_1_4=new QAction(QString("более 5 суток: %1 - %2").arg(pl.min[0][3]).arg(pl.max[0][3]),subMenu_1);
            subMenu_1_1->addAction(act_1_4);
            subMenu_1->addMenu(subMenu_1_1);

            QMenu* subMenu_1_2 = new QMenu("Интенсивность БД средняя");
            QAction *act_2_1=new QAction(QString("через 1 час: %1 - %2").arg(pl.min[1][0]).arg(pl.max[1][0]),subMenu_1);
            subMenu_1_2->addAction(act_2_1);
            QAction *act_2_2=new QAction(QString("втечение суток: %1 - %2").arg(pl.min[1][1]).arg(pl.max[1][1]),subMenu_1);
            subMenu_1_2->addAction(act_2_2);
            QAction *act_2_3=new QAction(QString("через 3-5 суток: %1 - %2").arg(pl.min[1][2]).arg(pl.max[1][2]),subMenu_1);
            subMenu_1_2->addAction(act_2_3);
            QAction *act_2_4=new QAction(QString("более 5 суток: %1 - %2").arg(pl.min[1][3]).arg(pl.max[1][3]),subMenu_1);
            subMenu_1_2->addAction(act_2_4);
            subMenu_1->addMenu(subMenu_1_2);

            QMenu* subMenu_1_3 = new QMenu("Интенсивность БД высокая");
            QAction *act_3_1=new QAction(QString("через 1 час: %1 - %2").arg(pl.min[2][0]).arg(pl.max[2][0]),subMenu_1);
            subMenu_1_3->addAction(act_3_1);
            QAction *act_3_2=new QAction(QString("втечение суток: %1 - %2").arg(pl.min[2][1]).arg(pl.max[2][1]),subMenu_1);
            subMenu_1_3->addAction(act_3_2);
            QAction *act_3_3=new QAction(QString("через 3-5 суток: %1 - %2").arg(pl.min[2][2]).arg(pl.max[2][2]),subMenu_1);
            subMenu_1_3->addAction(act_3_3);
            QAction *act_3_4=new QAction(QString("более 5 суток: %1 - %2").arg(pl.min[2][3]).arg(pl.max[2][3]),subMenu_1);
            subMenu_1_3->addAction(act_3_4);
            subMenu_1->addMenu(subMenu_1_3);

            menu->addMenu(subMenu_1);
            QAction *act_del=new QAction("Удалить воинское формирование",this);
            act_del->setIcon(QIcon(":/Resources/close.png"));
            connect(act_del,SIGNAL(triggered()),this,SLOT(delete_ls()));
            QAction *otch_ls = new QAction (QString("Сформировать отчет"),this);
            ls_id = list.value(1).toInt();
            connect(otch_ls,SIGNAL(triggered()),this,SLOT(otchet_groups()));

            QAction *nac_sostav = new QAction(QString("Национальный состав"),this); //.arg(list.value(3)),this);
            connect(nac_sostav,SIGNAL(triggered()),this,SLOT(show_nations_ls()));
            QAction *confess = new QAction(QString("Религиозный состав"),this); //.arg(list.value(3)),this);
            connect(confess,SIGNAL(triggered()),this,SLOT(show_confess_ls()));

            QAction *state = new QAction(QString("Штатно-должностной состав"),this);
            connect(state,SIGNAL(triggered()),this,SLOT(show_state_ls()));

            menu->addAction(act);

            menu->addSeparator();
            menu->addAction(nac_sostav);
            menu->addAction(confess);
            menu->addAction(state);
            menu->addSeparator();
            menu->addAction(otch_ls);
            menu->addSeparator();
            menu->addAction(act_del);
            popupButton->setMenu(menu);
            menu->exec(QCursor::pos());

        }
        else if(list.value(0)=="chls"){
            People_Losses pl;
            pl.get_losses(list.at(1).toInt());
            QPushButton *popupButton = new QPushButton;
            QMenu *menu = new QMenu(this);

            //	calculating_mps calc;
            QAction *act=new QAction(QString("Оценка моралогического состояния %1: %2").arg(list.value(3)).arg(list.value(2)),this);
            QAction *act_del=new QAction("Удалить воинское формирование",this);
            act_del->setIcon(QIcon(":/Resources/close.png"));
            connect(act_del,SIGNAL(triggered()),this,SLOT(delete_ls()));
            QMenu* subMenu_1 = new QMenu("Потери л/с",menu);

            QMenu* subMenu_1_1 = new QMenu("Интенсивность БД низкая");
            QAction *act_1_1=new QAction(QString("через 1 час: %1 - %2").arg(pl.min[0][0]).arg(pl.max[0][0]),subMenu_1);
            subMenu_1_1->addAction(act_1_1);
            QAction *act_1_2=new QAction(QString("втечение суток: %1 - %2").arg(pl.min[0][1]).arg(pl.max[0][1]),subMenu_1);
            subMenu_1_1->addAction(act_1_2);
            QAction *act_1_3=new QAction(QString("через 3-5 суток: %1 - %2").arg(pl.min[0][2]).arg(pl.max[0][2]),subMenu_1);
            subMenu_1_1->addAction(act_1_3);
            QAction *act_1_4=new QAction(QString("более 5 суток: %1 - %2").arg(pl.min[0][3]).arg(pl.max[0][3]),subMenu_1);
            subMenu_1_1->addAction(act_1_4);
            subMenu_1->addMenu(subMenu_1_1);

            QMenu* subMenu_1_2 = new QMenu("Интенсивность БД средняя");
            QAction *act_2_1=new QAction(QString("через 1 час: %1 - %2").arg(pl.min[1][0]).arg(pl.max[1][0]),subMenu_1);
            subMenu_1_2->addAction(act_2_1);
            QAction *act_2_2=new QAction(QString("втечение суток: %1 - %2").arg(pl.min[1][1]).arg(pl.max[1][1]),subMenu_1);
            subMenu_1_2->addAction(act_2_2);
            QAction *act_2_3=new QAction(QString("через 3-5 суток: %1 - %2").arg(pl.min[1][2]).arg(pl.max[1][2]),subMenu_1);
            subMenu_1_2->addAction(act_2_3);
            QAction *act_2_4=new QAction(QString("более 5 суток: %1 - %2").arg(pl.min[1][3]).arg(pl.max[1][3]),subMenu_1);
            subMenu_1_2->addAction(act_2_4);
            subMenu_1->addMenu(subMenu_1_2);

            QMenu* subMenu_1_3 = new QMenu("Интенсивность БД высокая");
            QAction *act_3_1=new QAction(QString("через 1 час: %1 - %2").arg(pl.min[2][0]).arg(pl.max[2][0]),subMenu_1);
            subMenu_1_3->addAction(act_3_1);
            QAction *act_3_2=new QAction(QString("втечение суток: %1 - %2").arg(pl.min[2][1]).arg(pl.max[2][1]),subMenu_1);
            subMenu_1_3->addAction(act_3_2);
            QAction *act_3_3=new QAction(QString("через 3-5 суток: %1 - %2").arg(pl.min[2][2]).arg(pl.max[2][2]),subMenu_1);
            subMenu_1_3->addAction(act_3_3);
            QAction *act_3_4=new QAction(QString("более 5 суток: %1 - %2").arg(pl.min[2][3]).arg(pl.max[2][3]),subMenu_1);
            subMenu_1_3->addAction(act_3_4);
            subMenu_1->addMenu(subMenu_1_3);

            menu->addMenu(subMenu_1);

            //QAction *act=new QAction("Удалить воинское формирование",this);
            //connect(act,SIGNAL(triggered()),this,SLOT(delete_ls()));
            QAction *otch_ls = new QAction (QString("Сформировать отчет"),this);
            ls_id = list.value(1).toInt();
            connect(otch_ls,SIGNAL(triggered()),this,SLOT(otchet_groups()));
            QAction *nac_sostav = new QAction(QString("Национальный состав"),this); //.arg(list.value(3)),this);
            connect(nac_sostav,SIGNAL(triggered()),this,SLOT(show_nations_ls()));
            QAction *confess = new QAction(QString("Религиозный состав"),this); //.arg(list.value(3)),this);
            connect(confess,SIGNAL(triggered()),this,SLOT(show_confess_ls()));

            QAction *state = new QAction(QString("Штатно-должностной состав"),this);
            connect(state,SIGNAL(triggered()),this,SLOT(show_state_ls()));

            menu->addAction(act);
            menu->addSeparator();
            menu->addAction(nac_sostav);
            menu->addAction(confess);
            menu->addAction(state);
            menu->addSeparator();
            menu->addAction(otch_ls);
            menu->addSeparator();
            menu->addAction(act_del);
            popupButton->setMenu(menu);
            menu->exec(QCursor::pos());
        }

        else if(list.value(0)=="dgr"){
            QPushButton *popupButton = new QPushButton;
            QMenu *menu = new QMenu(this);
            QAction *act=new QAction("Удалить организацию",this);
            act->setIcon(QIcon(":/Resources/close.png"));
            connect(act,SIGNAL(triggered()),this,SLOT(delete_groups()));

            QAction *otch_groups = new QAction (QString("Сформировать отчет"),this);
            group_id = list.value(1).toInt();
            connect(otch_groups,SIGNAL(triggered()),this,SLOT(otchet_groups()));
            QAction *nac_sostav = new QAction(QString("Национальный состав"),this);
            connect(nac_sostav,SIGNAL(triggered()),this,SLOT(show_nations_gr()));
            QAction *confess = new QAction(QString("Религиозный состав"),this);
            connect(confess,SIGNAL(triggered()),this,SLOT(show_confess_gr()));

            QAction *profess = new QAction(QString("Профессиональный состав"),this);
            connect(profess,SIGNAL(triggered()),this,SLOT(show_profess_gr()));

            QAction *age = new QAction(QString("Возрастной состав"),this);
            connect(age,SIGNAL(triggered()),this,SLOT(show_age_gr()));

            QAction *sekas = new QAction(QString("Половой состав"),this);
            connect(sekas,SIGNAL(triggered()),this,SLOT(show_sekas_gr()));

            QAction *state = new QAction(QString("Штатно-должностной состав"),this);
            connect(state,SIGNAL(triggered()),this,SLOT(show_state_gr()));



            menu->addAction(nac_sostav);
            menu->addAction(confess);
            menu->addAction(profess);
            menu->addAction(age);
            menu->addAction(sekas);
            menu->addAction(state);
            menu->addSeparator();
            menu->addAction(otch_groups);
            menu->addSeparator();
            menu->addAction(act);

            popupButton->setMenu(menu);
            menu->exec(QCursor::pos());
        }
        else if(list.value(0)=="dsc"){
            QPushButton *popupButton = new QPushButton;
            QMenu *menu = new QMenu(this);
            QAction *act=new QAction("Удалить особое условие",this);
            QAction *otch_sc = new QAction (QString("Сформировать отчет"),this);
            connect(otch_sc,SIGNAL(triggered()),this,SLOT(otchet_groups()));
            act->setIcon(QIcon(":/Resources/close.png"));
            connect(act,SIGNAL(triggered()),this,SLOT(delete_sc()));
            menu->addAction(otch_sc);
            menu->addSeparator();
            menu->addAction(act);
            popupButton->setMenu(menu);
            menu->exec(QCursor::pos());
        }
        else if(list.value(0)=="dmpo"){
            QPushButton *popupButton = new QPushButton;
            QMenu *menu = new QMenu(this);
            QAction *act=new QAction("Удалить средство",this);
            QAction *otch_mpo_ls = new QAction (QString("Сформировать отчет"),this);
            connect(otch_mpo_ls,SIGNAL(triggered()),this,SLOT(otchet_groups()));
            act->setIcon(QIcon(":/Resources/close.png"));
            connect(act,SIGNAL(triggered()),this,SLOT(delete_mpo()));
            menu->addAction(otch_mpo_ls);
            menu->addSeparator();
            menu->addAction(act);
            popupButton->setMenu(menu);
            menu->exec(QCursor::pos());
        }
        else if(list.value(0)=="dmpos"){
            QPushButton *popupButton = new QPushButton;
            QMenu *menu = new QMenu(this);
            QAction *act=new QAction("Удалить средство",this);
            QAction *otch_mpo_gr = new QAction (QString("Сформировать отчет"),this);
            connect(otch_mpo_gr,SIGNAL(triggered()),this,SLOT(otchet_groups()));
            act->setIcon(QIcon(":/Resources/close.png"));
            connect(act,SIGNAL(triggered()),this,SLOT(delete_mpo()));
            menu->addAction(otch_mpo_gr);
            menu->addSeparator();
            menu->addAction(act);
            popupButton->setMenu(menu);
            menu->exec(QCursor::pos());
        }
        else if(list.value(0)=="dmposmi"){
            QPushButton *popupButton = new QPushButton;
            QMenu *menu = new QMenu(this);
            QAction *act=new QAction("Удалить средство",this);
            QAction *otch_mpo_smi = new QAction (QString("Сформировать отчет"),this);
            connect(otch_mpo_smi,SIGNAL(triggered()),this,SLOT(otchet_groups()));
            act->setIcon(QIcon(":/Resources/close.png"));
            connect(act,SIGNAL(triggered()),this,SLOT(delete_mpo()));
            menu->addAction(otch_mpo_smi);
            menu->addSeparator();
            menu->addAction(act);
            popupButton->setMenu(menu);
            menu->exec(QCursor::pos());
        }
    }
}
//============== Штатка для групп по правому клику ================================
void Objectmanager::show_state_gr(){

    Add_elements_dialog *add_element= new Add_elements_dialog(22,group_id);
    add_element->setModal(true);
    add_element->setFixedSize(600,100);
    add_element->exec();

}
//============== Штатка для регионов по правому клику ================================
void Objectmanager::show_state_ls(){

    Add_elements_dialog *add_element= new Add_elements_dialog(12,ls_id);
    add_element->setModal(true);
    add_element->setFixedSize(600,100);
    add_element->exec();

}
//============== sex для регионов по правому клику ================================
void Objectmanager::show_sekas_region(){

    Add_elements_dialog *add_element= new Add_elements_dialog(9,groud_id);
    add_element->setModal(true);
    add_element->setFixedSize(600,100);
    add_element->exec();

}
//============== sex для gr по правому клику ================================
void Objectmanager::show_sekas_gr(){

    Add_elements_dialog *add_element= new Add_elements_dialog(21,group_id);
    add_element->setModal(true);
    add_element->setFixedSize(600,100);
    add_element->exec();

}
//============== age для gr по правому клику ================================
void Objectmanager::show_age_gr(){

    Add_elements_dialog *add_element= new Add_elements_dialog(20,group_id);
    add_element->setModal(true);
    add_element->setFixedSize(800,450);
    add_element->exec();

}
//============== age для регионов по правому клику ================================
void Objectmanager::show_age_region(){

    Add_elements_dialog *add_element= new Add_elements_dialog(8,groud_id);
    add_element->setModal(true);
    add_element->setFixedSize(800,450);
    add_element->exec();

}
//============== проф_состав для регионов по правому клику ================================
void Objectmanager::show_profess_region(){

    Add_elements_dialog *add_element= new Add_elements_dialog(7,groud_id);
    add_element->setModal(true);
    add_element->setFixedSize(500,450);
    add_element->exec();

}
//============== проф_состав для gr по правому клику ================================
void Objectmanager::show_profess_gr(){

    Add_elements_dialog *add_element= new Add_elements_dialog(19,group_id);
    add_element->setModal(true);
    add_element->setFixedSize(500,450);
    add_element->exec();

}
//============== религия для регионов по правому клику ================================
void Objectmanager::show_confess_region(){

    Add_elements_dialog *add_element= new Add_elements_dialog(6,groud_id);
    add_element->setModal(true);
    add_element->setFixedSize(500,350);
    add_element->exec();

}
//============== религия для ВФ по правому клику ================================
void Objectmanager::show_confess_ls(){

    Add_elements_dialog *add_element= new Add_elements_dialog(11,ls_id);
    add_element->setModal(true);
    add_element->setFixedSize(500,350);
    add_element->exec();

}
//============== религия для ВФ по правому клику ================================
void Objectmanager::show_confess_gr(){

    Add_elements_dialog *add_element= new Add_elements_dialog(18,group_id);
    add_element->setModal(true);
    add_element->setFixedSize(500,350);
    add_element->exec();

}
//============== нац_состав для регионов по правому клику ================================
void Objectmanager::show_nations_region(){

    Add_elements_dialog *add_element= new Add_elements_dialog(4,groud_id);
    add_element->setModal(true);
    add_element->setFixedSize(700,350);
    add_element->exec();

}
//============== нац_состав для ВФ по правому клику ================================
void Objectmanager::show_nations_ls(){

    Add_elements_dialog *add_element= new Add_elements_dialog(10,ls_id);
    add_element->setModal(true);
    add_element->setFixedSize(700,350);
    add_element->exec();

}
//============== нац_состав для организаций по правому клику ================================
void Objectmanager::show_nations_gr(){

    Add_elements_dialog *add_element= new Add_elements_dialog(17,group_id);
    add_element->setModal(true);
    add_element->setFixedSize(700,350);
    add_element->exec();

}
//===========================================================================================
void Objectmanager::customMenuTree(const QPoint & pos)
{
    QModelIndex index = UI->object_manager_tree->currentIndex();
    if(!index.data(Qt::UserRole).toBool()) return;
    QVariant id=index.data(Qt::UserRole);
    if (id.type() == QVariant::String){
        QString user_data=id.toString();
        QStringList list=user_data.split("_");
        if(list.value(0)=="type"){
            int id_type = list.value(1).toInt();
            QPushButton *popupButton = new QPushButton;
            QMenu *menu = new QMenu(this);
            QAction *act=new QAction("Добавить страну в блок",this);
            connect(act,SIGNAL(triggered()),this,SLOT(add_country_blok()));

            QAction *act_edit=new QAction("Информация о блоках",this);
            connect(act_edit,SIGNAL(triggered()),this,SLOT(edit_country_blok()));

            QAction *act_del=new QAction("Удалить блок",this);
            act_del->setIcon(QIcon(":/Resources/close.png"));
            connect(act_del,SIGNAL(triggered()),this,SLOT(delete_blok()));

            menu->addAction(act);
            menu->addSeparator();
            menu->addAction(act_edit);
            menu->addSeparator();
            menu->addAction(act_del);
            popupButton->setMenu(menu);
            menu->exec(QCursor::pos());
        }
        else if(list.value(0)=="country"){
            QPushButton *popupButton = new QPushButton;
            QMenu *menu = new QMenu(this);
            QAction *act=new QAction("Убрать страну из блока",this);
            connect(act,SIGNAL(triggered()),this,SLOT(delete_country_blok()));

            QAction *act_del=new QAction("Удалить страну",this);
            act_del->setIcon(QIcon(":/Resources/close.png"));
            connect(act_del,SIGNAL(triggered()),this,SLOT(delete_country()));

            QAction *act_edit=new QAction("Информация о странах",this);
            connect(act_edit,SIGNAL(triggered()),this,SLOT(edit_country()));

            menu->addAction(act);
            menu->addSeparator();
            menu->addAction(act_edit);
            menu->addSeparator();
            menu->addAction(act_del);
            popupButton->setMenu(menu);
            menu->exec(QCursor::pos());
        }
    }
}
//========================== комбобокс страны =============================
void Objectmanager::fill_combobox_country(QComboBox *box)
{
    box->clear();
    QSqlQuery query;
    box->addItem("-");
    query.exec("SELECT country.id_country,country.name_country FROM country ORDER BY name_country");

    while (query.next()){

        QString country_obj = query.value(1).toString();
        int id_country=query.value(0).toInt();
        box->addItem(country_obj,id_country);
    }
    query.clear();
}
//========================== комбобокс блоки =============================
void Objectmanager::fill_combobox_blok(QComboBox *box, int id_current_blok)
{
    box->clear();
    QSqlQuery query;
    query.exec("SELECT blok.id_blok,blok.name_blok FROM blok ORDER BY name_blok");
    int current_index;
    int index=0;
    while (query.next())
    {
        QString blok_obj = query.value(1).toString();
        int id_blok=query.value(0).toInt();
        box->addItem(blok_obj,id_blok);
        if(id_current_blok==id_blok){
            current_index=index;
        }
        index++;
    }
    box->setCurrentIndex(current_index);
    query.clear();

}

//========================= добавление, редактирование и  удаление блока ======================================
void Objectmanager::add_new_blok(){

    add_blok = new QDialog();
    add_blok->setMinimumSize(500,200);
    add_blok->setWindowTitle("Добавить новый блок");
    add_blok->setWindowIcon(QIcon(":/Resources/add_but.png"));

    path_lab = new QLabel("Эмблема блока:");
    path_lab->setMinimumWidth(100);
    blok_desc = new QLabel("Описание блока");
    blok_desc->setMinimumWidth(100);
    blok_name = new QLabel("Наименование блока:");
    blok_name->setMinimumWidth(100);

    blok_filepath_edit = new QLineEdit();
    blok_desc_edit = new QTextEdit();
    blok_desc_edit->setFixedHeight(100);
    blok_name_edit = new QLineEdit();

    path_button = new QToolButton();
    path_button->setText("...");
    connect(path_button,SIGNAL(clicked()),this,SLOT(get_path()));

    ok_button = new QPushButton("OK");
    connect(ok_button,SIGNAL(clicked()),add_blok,SLOT(accept()));
    cancel_button = new QPushButton("Отмена");
    connect(cancel_button,SIGNAL(clicked()),add_blok,SLOT(close()));

    QHBoxLayout *buttons_layout = new QHBoxLayout();
    buttons_layout->addWidget(ok_button);
    buttons_layout->addWidget(cancel_button);

    QHBoxLayout *path_layout = new QHBoxLayout();
    path_layout->addWidget(path_lab);
    QHBoxLayout *path_layout_2 = new QHBoxLayout();
    path_layout_2->addWidget(blok_filepath_edit);
    path_layout_2->addWidget(path_button);

    QHBoxLayout *code_layout = new QHBoxLayout();
    code_layout->addWidget(blok_desc);
    QHBoxLayout *code_layout_2 = new QHBoxLayout();
    code_layout_2->addWidget(blok_desc_edit);

    QHBoxLayout *name_layout = new QHBoxLayout();
    name_layout->addWidget(blok_name);
    QHBoxLayout *name_layout_2 = new QHBoxLayout();
    name_layout_2->addWidget(blok_name_edit);


    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);
    mainLayout->addLayout(path_layout, 0, 0);
    mainLayout->addLayout(path_layout_2, 0, 1);
    mainLayout->addLayout(name_layout, 1, 0);
    mainLayout->addLayout(name_layout_2, 1, 1);
    mainLayout->addLayout(code_layout, 2, 0);
    mainLayout->addLayout(code_layout_2, 2, 1);
    mainLayout->addLayout(buttons_layout, 3, 1);

    add_blok->setLayout(mainLayout);

    if(add_blok->exec() == QDialog::Accepted)
    {

        if((blok_desc_edit->toPlainText() == "") || (blok_name_edit->text() == "")) return;

        //------ По кнопке ОК добавление в БД блока --------
        QSqlQuery query;
        query.prepare("INSERT INTO blok (name_blok,description_blok,emblem_blok) VALUES (?,?,?)");
        query.addBindValue(blok_name_edit->text());
        query.addBindValue(blok_desc_edit->toPlainText());

        QFile file(blok_filepath_edit->text());
        if(!file.open(QIODevice::ReadOnly))
        {
            /*   //================MessageBox===============================
          QMessageBox msgBox;
          msgBox.setWindowTitle("Внимание");
          msgBox.setText("Необходимо выбрать изображение");
          msgBox.setStandardButtons(QMessageBox::Yes);
          switch (msgBox.exec()) {
          case QMessageBox::Yes:
              return;
              break;
          }*/
        }

        QByteArray ba = file.readAll();
        query.addBindValue(ba);

        if(!query.exec())
        {
            QMessageBox::about(this,"Ошибка",query.lastError().text());
        }
        init_object_tree();
        return;
    }
    return;
}
void Objectmanager::get_path()
{
    QFileDialog *file_dlg = new QFileDialog(add_blok);
    QString filepath =  file_dlg->getOpenFileName(this,
                                                  "Открыть изображение", "", tr("Image Files (*.png *.jpg *.bmp)"));
    blok_filepath_edit->setText(filepath);
    add_blok->raise();
}
void Objectmanager::get_path_edit()
{
    QFileDialog *file_dlg = new QFileDialog(edit_dlg);
    QString filepath =  file_dlg->getOpenFileName(this,
                                                  "Открыть изображение", "", tr("Image Files (*.png *.jpg *.bmp)"));
    blok_filepath_edit->setText(filepath);
    edit_dlg->raise();
}
void Objectmanager::delete_blok(){

    QMessageBox msgBox;
    msgBox.setWindowTitle("Предупреждение");
    msgBox.setText("Вы действительно удалить блок?");

    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setButtonText(QMessageBox::Yes, "Да");
    msgBox.setButtonText(QMessageBox::No, "Нет");
    switch (msgBox.exec()) {
    case QMessageBox::Yes:
        // yes was clicked
        break;
    case QMessageBox::No:
        return;
        break;
    default:
        return;
        break;
    }

    QSqlQuery query;

    QModelIndex index = UI->object_manager_tree->currentIndex();
    if(!index.data(Qt::UserRole).toBool()) return;
    QString id_country=index.data(Qt::UserRole).toString();
    QStringList list=id_country.split("_");

    QString str = QString("DELETE FROM blok WHERE id_blok = %1").arg(list.value(1).toInt());
    if(!query.exec(str)){
        return;
    }

    init_object_tree();
    return;

}
void Objectmanager::edit_country_blok(){

    edit_blok = new QDialog;
    edit_blok->setMinimumSize(600,400);
    edit_blok->setWindowTitle("Информация о блоках");
    edit_blok->setWindowIcon(QIcon(":/Resources/add_but.png"));

    blok_edit_table = new QTableWidget;
    blok_edit_table->setWordWrap(true);
    // blok_edit_table->setMinimumSize(600,300);

    QPushButton *cancelButton = new QPushButton("Выход");
    connect(cancelButton,SIGNAL(clicked()),edit_blok,SLOT(close()));

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch();
    buttonsLayout->addWidget(cancelButton);
    QHBoxLayout *h_lay = new QHBoxLayout;

    h_lay->addWidget(blok_edit_table);

    QGridLayout *mainLayout = new QGridLayout;
    //mainLayout->setSizeConstraint(QLayout::SetFixedSize);
    mainLayout->addLayout(h_lay, 0, 0);
    mainLayout->addLayout(buttonsLayout, 1, 0);
    edit_blok->setLayout(mainLayout);

    table_blok();
    connect(blok_edit_table,SIGNAL(cellClicked(int,int)),this,SLOT(show_redaktor_blok(int,int)));

    if(edit_blok->exec() == QDialog::Accepted)
    {

    }
}
void Objectmanager::table_blok(){

    blok_edit_table->setColumnCount(5);
    blok_edit_table->hideColumn(1);
    blok_edit_table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(blok_edit_table, SIGNAL(cellChanged(int,int)),
            blok_edit_table, SLOT(resizeRowsToContents()));

    QStringList header_list;
    header_list<<" "<<" "<<"Наименование блока"<<"Описание блока"<<"Эмблема";
    blok_edit_table->setHorizontalHeaderLabels(header_list);

    QSqlQuery query;
    QString str = QString("SELECT id_blok,name_blok, description_blok, emblem_blok FROM blok");
    if(!query.exec(str))
    {
        return;
    }

    QSqlRecord rec = query.record();
    int id_blok;
    int row = 0;
    QString name_bl,desc_bl;
    QTableWidgetItem *item;
    blok_edit_table->setIconSize(QSize(25,25));
    while(query.next())
    {
        id_blok = query.value(rec.indexOf("id_blok")).toInt();
        name_bl = query.value(rec.indexOf("name_blok")).toString();
        desc_bl = query.value(rec.indexOf("description_blok")).toString();

        blok_edit_table->insertRow(row);

        QIcon icon(QString(":/Resources/edit_1.png"));
        item = new QTableWidgetItem(icon,0);
        item->setToolTip("Редактировать блок");
        blok_edit_table->setItem(row,0,item);

        item = new QTableWidgetItem(QString::number(id_blok));
        blok_edit_table->setItem(row,1,item);

        item = new QTableWidgetItem(name_bl);
        blok_edit_table->setItem(row,2,item);

        item = new QTableWidgetItem(desc_bl);
        blok_edit_table->setItem(row,3,item);

        QPixmap pixmap;
        pixmap.loadFromData( query.value(rec.indexOf("emblem_blok")).toByteArray());
        QTableWidgetItem *foto_item = new QTableWidgetItem(QIcon(pixmap),"");
        blok_edit_table->setItem(row,4,foto_item);

    }
    row++;
    blok_edit_table->resizeColumnsToContents();
}
void Objectmanager::show_redaktor_blok(int row,int column){

    if(column==0){

        int id_blok = blok_edit_table->item(row,1)->text().toInt();

        edit_dlg = new QDialog;
        edit_dlg->setWindowTitle("Редактирование информации о блоке");
        edit_dlg->setMinimumSize(QSize(600,400));

        path_lab = new QLabel("Эмблема блока:");
        path_lab->setMinimumWidth(100);
        blok_desc = new QLabel("Описание блока");
        blok_desc->setMinimumWidth(100);
        blok_name = new QLabel("Наименование блока:");
        blok_name->setMinimumWidth(100);

        blok_filepath_edit = new QLineEdit();
        blok_desc_edit = new QTextEdit();
        blok_desc_edit->setFixedHeight(100);
        blok_name_edit = new QLineEdit();

        path_button = new QToolButton();
        path_button->setText("...");
        connect(path_button,SIGNAL(clicked()),this,SLOT(get_path_edit()));

        QSqlQuery query;
        QString str = QString("SELECT id_blok,name_blok, description_blok, emblem_blok FROM blok WHERE id_blok = %1").arg(id_blok);
        if(!query.exec(str)){
            return;
        }

        QSqlRecord rec = query.record();
        int id_blok_;
        QByteArray emblem;
        while(query.next()){

            id_blok_= query.value(rec.indexOf("id_blok")).toInt();
            blok_name_edit->setText(query.value(rec.indexOf("name_blok")).toString());
            blok_desc_edit->setPlainText(query.value(rec.indexOf("description_blok")).toString());
            emblem = query.value(rec.indexOf("emblem_blok")).toByteArray();
        }


        ok_button = new QPushButton("Сохранить");
        connect(ok_button,SIGNAL(clicked()),edit_dlg,SLOT(accept()));
        cancel_button = new QPushButton("Отмена");
        connect(cancel_button,SIGNAL(clicked()),edit_dlg,SLOT(close()));

        QHBoxLayout *buttons_layout = new QHBoxLayout();
        buttons_layout->addWidget(ok_button);
        buttons_layout->addWidget(cancel_button);

        QHBoxLayout *path_layout = new QHBoxLayout();
        path_layout->addWidget(path_lab);
        QHBoxLayout *path_layout_2 = new QHBoxLayout();
        path_layout_2->addWidget(blok_filepath_edit);
        path_layout_2->addWidget(path_button);

        QHBoxLayout *code_layout = new QHBoxLayout();
        code_layout->addWidget(blok_desc);
        QHBoxLayout *code_layout_2 = new QHBoxLayout();
        code_layout_2->addWidget(blok_desc_edit);

        QHBoxLayout *name_layout = new QHBoxLayout();
        name_layout->addWidget(blok_name);
        QHBoxLayout *name_layout_2 = new QHBoxLayout();
        name_layout_2->addWidget(blok_name_edit);

        QGridLayout *mainLayout = new QGridLayout;
        mainLayout->setSizeConstraint(QLayout::SetFixedSize);
        mainLayout->addLayout(path_layout, 0, 0);
        mainLayout->addLayout(path_layout_2, 0, 1);
        mainLayout->addLayout(name_layout, 1, 0);
        mainLayout->addLayout(name_layout_2, 1, 1);
        mainLayout->addLayout(code_layout, 2, 0);
        mainLayout->addLayout(code_layout_2, 2, 1);
        mainLayout->addLayout(buttons_layout, 3, 1);

        edit_dlg->setLayout(mainLayout);

        if(edit_dlg->exec() == QDialog::Accepted){

            clear_tableWidget(blok_edit_table);

            if(emblem.isEmpty()){

                QSqlQuery query;
                query.prepare("UPDATE blok SET name_blok = ?,description_blok = ?,emblem_blok = ? WHERE id_blok = ?");
                query.addBindValue(blok_name_edit->text());
                query.addBindValue(blok_desc_edit->toPlainText());

                QFile file(blok_filepath_edit->text());
                if(!file.open(QIODevice::ReadOnly)){
                }

                QByteArray ba = file.readAll();
                query.addBindValue(ba);
                query.addBindValue(id_blok_);
                if(!query.exec())
                {
                    QMessageBox::about(this,"Ошибка",query.lastError().text());
                }

                table_blok();
            }
            else if((!emblem.isEmpty()) && (blok_filepath_edit->text()=="")){

                QSqlQuery query;
                query.prepare("UPDATE blok SET name_blok = ?, description_blok = ?,emblem_blok = ? WHERE id_blok = ?");
                query.addBindValue(blok_name_edit->text());
                query.addBindValue(blok_desc_edit->toPlainText());
                query.addBindValue(emblem);
                query.addBindValue(id_blok_);
                if(!query.exec())
                {
                    QMessageBox::about(this,"Ошибка",query.lastError().text());
                }
                table_blok();
            }
            else 	{
                QSqlQuery query;
                query.prepare("UPDATE blok SET name_blok = ?,description_blok = ?,emblem_blok = ? WHERE id_blok = ?");
                query.addBindValue(blok_name_edit->text());
                query.addBindValue(blok_desc_edit->toPlainText());

                QFile file(blok_filepath_edit->text());
                if(!file.open(QIODevice::ReadOnly)){
                }

                QByteArray ba = file.readAll();
                query.addBindValue(ba);
                query.addBindValue(id_blok_);
                if(!query.exec())
                {
                    QMessageBox::about(this,"Ошибка",query.lastError().text());
                }

                table_blok();
            }
        }

        init_object_tree();
        return;
    }


}

//========================= добавление, редактирование и удаление страны ======================================
void Objectmanager::add_new_country(){
    add_country = new QDialog();
    add_country->setMinimumSize(500,200);
    add_country->setWindowTitle("Добавить новую страну");
    add_country->setWindowIcon(QIcon(":/Resources/add_but.png"));

    path_lab = new QLabel("Флаг страны:");
    path_lab->setMinimumWidth(100);
    blok_desc = new QLabel("Описание страны");
    blok_desc->setMinimumWidth(100);
    blok_name = new QLabel("Наименование страны:");
    blok_name->setMinimumWidth(100);

    blok_filepath_edit = new QLineEdit();
    blok_desc_edit = new QTextEdit();
    blok_desc_edit->setFixedWidth(300);
    blok_name_edit = new QLineEdit();

    QLabel *label_enemy = new QLabel("Враждебность страны:");
    checkbox_enemy = new QCheckBox(" [-V- враждебное]", add_country);
    label_enemy->setBuddy(checkbox_enemy);

    path_button = new QToolButton();
    path_button->setText("...");
    connect(path_button,SIGNAL(clicked()),this,SLOT(get_path_flag()));

    ok_button = new QPushButton("OK");
    connect(ok_button,SIGNAL(clicked()),add_country,SLOT(accept()));
    cancel_button = new QPushButton("Отмена");
    connect(cancel_button,SIGNAL(clicked()),add_country,SLOT(close()));

    QHBoxLayout *buttons_layout = new QHBoxLayout();
    buttons_layout->addWidget(ok_button);
    buttons_layout->addWidget(cancel_button);

    QHBoxLayout *path_layout = new QHBoxLayout();
    path_layout->addWidget(path_lab);
    QHBoxLayout *path_layout_2 = new QHBoxLayout();
    path_layout_2->addWidget(blok_filepath_edit);
    path_layout_2->addWidget(path_button);

    QHBoxLayout *code_layout = new QHBoxLayout();
    code_layout->addWidget(blok_desc);
    QHBoxLayout *code_layout_2 = new QHBoxLayout();
    code_layout_2->addWidget(blok_desc_edit);

    QHBoxLayout *check_layout = new QHBoxLayout;
    check_layout->addWidget(label_enemy);
    QHBoxLayout *check_layout_2 = new QHBoxLayout;
    check_layout_2->addWidget(checkbox_enemy);

    QHBoxLayout *name_layout = new QHBoxLayout();
    name_layout->addWidget(blok_name);
    QHBoxLayout *name_layout_2 = new QHBoxLayout();
    name_layout_2->addWidget(blok_name_edit);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);
    mainLayout->addLayout(path_layout, 0, 0);
    mainLayout->addLayout(path_layout_2, 0, 1);
    mainLayout->addLayout(name_layout, 1, 0);
    mainLayout->addLayout(name_layout_2, 1, 1);
    mainLayout->addLayout(check_layout, 2, 0);
    mainLayout->addLayout(check_layout_2, 2, 1);
    mainLayout->addLayout(code_layout, 3, 0);
    mainLayout->addLayout(code_layout_2, 3, 1);
    mainLayout->addLayout(buttons_layout, 4, 1);

    add_country->setLayout(mainLayout);

    if(add_country->exec() == QDialog::Accepted)
    {

        if((blok_desc_edit->toPlainText() == "") || (blok_name_edit->text() == "")) return;

        //------ По кнопке ОК добавление в БД блока --------
        QSqlQuery query;
        query.prepare("INSERT INTO country (name_country,description_country,enimy_coutry,flag) VALUES (?,?,?,?)");
        query.addBindValue(blok_name_edit->text());
        query.addBindValue(blok_desc_edit->toPlainText());
        query.addBindValue(checkbox_enemy->isChecked());

        QFile file(blok_filepath_edit->text());
        if(!file.open(QIODevice::ReadOnly))
        {
            /* //================MessageBox===============================
            QMessageBox msgBox;
            msgBox.setWindowTitle("Внимание");
            msgBox.setText("Необходимо выбрать изображение");
            msgBox.setStandardButtons(QMessageBox::Yes);
            switch (msgBox.exec()) {
            case QMessageBox::Yes:
                return;
                break;
            }*/
        }

        QByteArray ba = file.readAll();
        query.addBindValue(ba);

        if(!query.exec())
        {
            QMessageBox::about(this,"Ошибка",query.lastError().text());
        }

        return;
    }
    return;
}
void Objectmanager::get_path_flag()
{
    QFileDialog *file_dlg = new QFileDialog(add_country);
    QString filepath =  file_dlg->getOpenFileName(this,
                                                  "Открыть изображение", "", tr("Image Files (*.png *.jpg *.bmp)"));
    blok_filepath_edit->setText(filepath);
    add_country->raise();
}
void Objectmanager::get_path_flag_edit()
{
    QFileDialog *file_dlg = new QFileDialog(edit_dlg);
    QString filepath =  file_dlg->getOpenFileName(this,
                                                  "Открыть изображение", "", tr("Image Files (*.png *.jpg *.bmp)"));
    blok_filepath_edit->setText(filepath);
    edit_dlg->raise();
}
void Objectmanager::delete_country(){

    QMessageBox msgBox;
    msgBox.setWindowTitle("Предупреждение");
    msgBox.setText("Вы действительно удалить страну?");

    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setButtonText(QMessageBox::Yes, "Да");
    msgBox.setButtonText(QMessageBox::No, "Нет");
    switch (msgBox.exec()) {
    case QMessageBox::Yes:
        // yes was clicked
        break;
    case QMessageBox::No:
        return;
        break;
    default:
        return;
        break;
    }

    QSqlQuery query;

    QModelIndex index = UI->object_manager_tree->currentIndex();
    if(!index.data(Qt::UserRole).toBool()) return;
    QString id_country=index.data(Qt::UserRole).toString();
    QStringList list=id_country.split("_");

    QString str = QString("DELETE FROM blok_country WHERE id_country = %1 and id_blok=%2").arg(list.value(1)).arg(list.value(2));

    if(!query.exec(str)){
        return;
    }

    QSqlQuery query_co;
    QString str_co = QString("DELETE FROM country WHERE id_country = %1").arg(list.value(1).toInt());
    if(!query_co.exec(str_co)){
        return;
    }

    init_object_tree();
    return;

}
void Objectmanager::edit_country(){

    edit_country_dlg = new QDialog;
    edit_country_dlg->setMinimumSize(600,400);
    edit_country_dlg->setWindowTitle("Информация о странах");
    edit_country_dlg->setWindowIcon(QIcon(":/Resources/add_but.png"));

    country_edit_table = new QTableWidget;
    country_edit_table->setWordWrap(true);
    // blok_edit_table->setMinimumSize(600,300);

    QPushButton *cancelButton = new QPushButton("Выход");
    connect(cancelButton,SIGNAL(clicked()),edit_country_dlg,SLOT(close()));

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch();
    buttonsLayout->addWidget(cancelButton);
    QHBoxLayout *h_lay = new QHBoxLayout;

    h_lay->addWidget(country_edit_table);

    QGridLayout *mainLayout = new QGridLayout;
    //mainLayout->setSizeConstraint(QLayout::SetFixedSize);
    mainLayout->addLayout(h_lay, 0, 0);
    mainLayout->addLayout(buttonsLayout, 1, 0);
    edit_country_dlg->setLayout(mainLayout);

    table_country();
    connect(country_edit_table,SIGNAL(cellClicked(int,int)),this,SLOT(show_redaktor_country(int,int)));

    if(edit_country_dlg->exec() == QDialog::Accepted)
    {

    }
}
void Objectmanager::table_country(){

    country_edit_table->setColumnCount(6);
    country_edit_table->hideColumn(1);
    country_edit_table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(country_edit_table, SIGNAL(cellChanged(int,int)),
            country_edit_table, SLOT(resizeRowsToContents()));

    QStringList header_list;
    header_list<<" "<<" "<<"Наименование страны"<<"Описание страны"<<"Враждебность"<<"Флаг";
    country_edit_table->setHorizontalHeaderLabels(header_list);

    QSqlQuery query;
    QString str = QString("SELECT id_country,name_country, description_country, enimy_coutry, flag FROM country");
    if(!query.exec(str))
    {
        return;
    }

    QSqlRecord rec = query.record();
    int id_country;
    int row = 0;
    QString name_bl,desc_bl,enemy_bool;
    bool enemy;
    QTableWidgetItem *item;
    country_edit_table->setIconSize(QSize(25,25));
    while(query.next())
    {
        id_country = query.value(rec.indexOf("id_country")).toInt();
        name_bl = query.value(rec.indexOf("name_country")).toString();
        desc_bl = query.value(rec.indexOf("description_country")).toString();
        enemy = query.value(rec.indexOf("enimy_coutry")).toBool();

        if(enemy == true)
        {
            enemy_bool = "Враждебно настроена";
        }
        else
            enemy_bool = "Не враждебно настроена";

        country_edit_table->insertRow(row);

        QIcon icon(QString(":/Resources/edit_1.png"));
        item = new QTableWidgetItem(icon,0);
        item->setToolTip("Редактировать страну");
        country_edit_table->setItem(row,0,item);

        item = new QTableWidgetItem(QString::number(id_country));
        country_edit_table->setItem(row,1,item);

        item = new QTableWidgetItem(name_bl);
        country_edit_table->setItem(row,2,item);

        item = new QTableWidgetItem(desc_bl);
        country_edit_table->setItem(row,3,item);

        item = new QTableWidgetItem(enemy_bool);
        country_edit_table->setItem(row,4,item);

        QPixmap pixmap;
        pixmap.loadFromData( query.value(rec.indexOf("flag")).toByteArray());
        QTableWidgetItem *foto_item = new QTableWidgetItem(QIcon(pixmap),"");
        country_edit_table->setItem(row,5,foto_item);

    }
    row++;
    country_edit_table->resizeColumnsToContents();
}
void Objectmanager::show_redaktor_country(int row,int column){

    if(column==0){

        int id_country = country_edit_table->item(row,1)->text().toInt();

        edit_dlg = new QDialog;
        edit_dlg->setWindowTitle("Редактирование информации о стране");
        edit_dlg->setMinimumSize(QSize(600,400));

        path_lab = new QLabel("Флаг страны:");
        path_lab->setMinimumWidth(100);
        blok_desc = new QLabel("Описание страны");
        blok_desc->setMinimumWidth(100);
        blok_name = new QLabel("Наименование страны:");
        blok_name->setMinimumWidth(100);

        blok_filepath_edit = new QLineEdit();
        blok_desc_edit = new QTextEdit();
        blok_desc_edit->setFixedWidth(300);
        blok_name_edit = new QLineEdit();

        QLabel *label_enemy = new QLabel("Враждебность страны:");
        checkbox_enemy = new QCheckBox(" [-V- враждебное]", edit_dlg);
        label_enemy->setBuddy(checkbox_enemy);

        path_button = new QToolButton();
        path_button->setText("...");
        connect(path_button,SIGNAL(clicked()),this,SLOT(get_path_flag_edit()));

        QSqlQuery query;
        QString str = QString("SELECT id_country,name_country, description_country, enimy_coutry, flag FROM country WHERE id_country = %1").arg(id_country);
        if(!query.exec(str)){
            return;
        }

        QSqlRecord rec = query.record();
        int id_country_;
        bool enemy;
        QByteArray emblem;
        while(query.next()){

            id_country_= query.value(rec.indexOf("id_country")).toInt();
            blok_name_edit->setText(query.value(rec.indexOf("name_country")).toString());
            blok_desc_edit->setPlainText(query.value(rec.indexOf("description_country")).toString());
            emblem = query.value(rec.indexOf("flag")).toByteArray();
            checkbox_enemy->setChecked(query.value(rec.indexOf("enimy_coutry")).toBool());
        }


        ok_button = new QPushButton("Сохранить");
        connect(ok_button,SIGNAL(clicked()),edit_dlg,SLOT(accept()));
        cancel_button = new QPushButton("Отмена");
        connect(cancel_button,SIGNAL(clicked()),edit_dlg,SLOT(close()));

        QHBoxLayout *buttons_layout = new QHBoxLayout();
        buttons_layout->addWidget(ok_button);
        buttons_layout->addWidget(cancel_button);

        QHBoxLayout *path_layout = new QHBoxLayout();
        path_layout->addWidget(path_lab);
        QHBoxLayout *path_layout_2 = new QHBoxLayout();
        path_layout_2->addWidget(blok_filepath_edit);
        path_layout_2->addWidget(path_button);

        QHBoxLayout *code_layout = new QHBoxLayout();
        code_layout->addWidget(blok_desc);
        QHBoxLayout *code_layout_2 = new QHBoxLayout();
        code_layout_2->addWidget(blok_desc_edit);

        QHBoxLayout *check_layout = new QHBoxLayout;
        check_layout->addWidget(label_enemy);
        QHBoxLayout *check_layout_2 = new QHBoxLayout;
        check_layout_2->addWidget(checkbox_enemy);

        QHBoxLayout *name_layout = new QHBoxLayout();
        name_layout->addWidget(blok_name);
        QHBoxLayout *name_layout_2 = new QHBoxLayout();
        name_layout_2->addWidget(blok_name_edit);

        QGridLayout *mainLayout = new QGridLayout;
        mainLayout->setSizeConstraint(QLayout::SetFixedSize);
        mainLayout->addLayout(path_layout, 0, 0);
        mainLayout->addLayout(path_layout_2, 0, 1);
        mainLayout->addLayout(name_layout, 1, 0);
        mainLayout->addLayout(name_layout_2, 1, 1);
        mainLayout->addLayout(check_layout, 2, 0);
        mainLayout->addLayout(check_layout_2, 2, 1);
        mainLayout->addLayout(code_layout, 3, 0);
        mainLayout->addLayout(code_layout_2, 3, 1);
        mainLayout->addLayout(buttons_layout, 4, 1);

        edit_dlg->setLayout(mainLayout);

        if(edit_dlg->exec() == QDialog::Accepted){

            clear_tableWidget(country_edit_table);

            if(emblem.isEmpty()){

                QSqlQuery query;
                query.prepare("UPDATE country SET name_country = ?,description_country = ?,enimy_coutry = ?, flag = ? WHERE id_country = ?");
                query.addBindValue(blok_name_edit->text());
                query.addBindValue(blok_desc_edit->toPlainText());
                query.addBindValue(checkbox_enemy->isChecked());

                QFile file(blok_filepath_edit->text());
                if(!file.open(QIODevice::ReadOnly)){
                }

                QByteArray ba = file.readAll();
                query.addBindValue(ba);
                query.addBindValue(id_country_);
                if(!query.exec())
                {
                    QMessageBox::about(this,"Ошибка",query.lastError().text());
                }

                table_country();
            }
            else if((!emblem.isEmpty()) && (blok_filepath_edit->text()=="")){

                QSqlQuery query;
                query.prepare("UPDATE country SET name_country = ?, description_country = ?,enimy_coutry = ?, flag = ? WHERE id_country = ?");
                query.addBindValue(blok_name_edit->text());
                query.addBindValue(blok_desc_edit->toPlainText());
                query.addBindValue(checkbox_enemy->isChecked());
                query.addBindValue(emblem);
                query.addBindValue(id_country_);
                if(!query.exec())
                {
                    QMessageBox::about(this,"Ошибка",query.lastError().text());
                }
                table_country();
            }
            else 	{
                QSqlQuery query;
                query.prepare("UPDATE country SET name_country = ?,description_country = ?,enimy_coutry = ?, flag = ? WHERE id_country = ?");
                query.addBindValue(blok_name_edit->text());
                query.addBindValue(blok_desc_edit->toPlainText());
                query.addBindValue(checkbox_enemy->isChecked());

                QFile file(blok_filepath_edit->text());
                if(!file.open(QIODevice::ReadOnly)){
                }

                QByteArray ba = file.readAll();
                query.addBindValue(ba);
                query.addBindValue(id_country_);
                if(!query.exec())
                {
                    QMessageBox::about(this,"Ошибка",query.lastError().text());
                }

                table_country();
            }
        }

        init_object_tree();
        return;
    }

}

//========================= добавление страны в блок ===========================================================
void Objectmanager::add_country_blok(){

    QDialog *add_element = new QDialog();

    QModelIndex index = UI->object_manager_tree->currentIndex();
    QString id=index.data(Qt::UserRole).toString();
    QStringList list=id.split("_");
    add_element->setWindowTitle("Добвление страны в блок");

    QLabel *element_name_blok = new QLabel("Блок:");
    QLabel *element_name_country = new QLabel("Страна:");

    QComboBox *blok_combo = new QComboBox();
    QComboBox *country_combo = new QComboBox();

    blok_combo->setFixedWidth(199);
    country_combo->setFixedWidth(199);

    fill_combobox_blok(blok_combo,list.value(1).toInt());
    fill_combobox_country(country_combo);

    QHBoxLayout *element_name_blok_layout = new QHBoxLayout();
    element_name_blok_layout->addWidget(element_name_blok);
    element_name_blok_layout->addWidget(blok_combo);

    QHBoxLayout *BP_count_layout = new QHBoxLayout();
    BP_count_layout->addWidget(element_name_country);
    BP_count_layout->addWidget(country_combo);

    QPushButton *ok_button = new QPushButton("Добавить");
    connect(ok_button,SIGNAL(clicked()),add_element,SLOT(accept()));
    QPushButton *cancel_button = new QPushButton("Отмена");
    connect(cancel_button,SIGNAL(clicked()),add_element,SLOT(close()));

    QHBoxLayout *buttons_layout = new QHBoxLayout();
    buttons_layout->addStretch();
    buttons_layout->addWidget(ok_button);
    buttons_layout->addWidget(cancel_button);

    QVBoxLayout *vert_layout = new QVBoxLayout;
    vert_layout->addLayout(element_name_blok_layout);
    vert_layout->addLayout(BP_count_layout);
    vert_layout->addSpacing(30);
    vert_layout->addLayout(buttons_layout);

    add_element->setLayout(vert_layout);

    if(add_element->exec() == QDialog::Accepted){
        if (country_combo->currentIndex() == 0){
            QMessageBox::StandardButton ret;
            ret = QMessageBox::critical (this,"Ошибка",("Выберите страну "),QMessageBox::Ok );
        }

        //=============== По кнопке Добавить добавление в БД нового уязвимого элемента ===========

        int id_blok = blok_combo->itemData(blok_combo->currentIndex()).toInt();
        int id_country = country_combo->itemData(country_combo->currentIndex()).toInt();
        int id_country_1;

        QSqlQuery query_1;
        query_1.exec(QString("SELECT id_country FROM blok_country where id_blok=%1").arg(id_blok));

        while (query_1.next()){

            id_country_1=query_1.value(0).toInt();

            if (id_country == id_country_1){

                QMessageBox::StandardButton ret;
                ret = QMessageBox::critical (this,"Ошибка",("Страна присутствует в блоке "),QMessageBox::Ok );
                return;
            }
        }
        query_1.clear();

        QSqlQuery query;
        query.prepare("INSERT INTO blok_country (id_blok,id_country) VALUES (?,?)");
        query.addBindValue(id_blok);
        query.addBindValue(id_country);

        if(!query.exec()){

            return;
        }

        init_object_tree();
        return;
    }

}
//===================== Удаление страны из блока ===============================================================
void Objectmanager::delete_country_blok()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Предупреждение");
    msgBox.setText("Вы действительно хотите удалить страну из блока?");

    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setButtonText(QMessageBox::Yes, "Да");
    msgBox.setButtonText(QMessageBox::No, "Нет");
    switch (msgBox.exec()) {
    case QMessageBox::Yes:
        // yes was clicked
        break;
    case QMessageBox::No:
        return;
        break;
    default:
        return;
        break;
    }

    QSqlQuery query;

    QModelIndex index = UI->object_manager_tree->currentIndex();
    if(!index.data(Qt::UserRole).toBool()) return;
    QString id_country=index.data(Qt::UserRole).toString();
    QStringList list=id_country.split("_");

    QString str = QString("DELETE FROM blok_country WHERE id_country = %1 and id_blok=%2").arg(list.value(1)).arg(list.value(2));
    if(!query.exec(str)){
        return;
    }

    init_object_tree();
    return;

}

void Objectmanager::updateDB(QStandardItem *item)
{
    QString table;
    QString column;
    int ID;
    table=item->data(Qt::UserRole+2).toString();
    column=item->data(Qt::UserRole+3).toString();
    ID=item->data(Qt::UserRole+4).toInt();
    QSqlQuery query;
    QString str;
    str=QString("UPDATE %1 SET %2 = '%4' where id_%1=%3").arg(table).arg(column).arg(ID).arg(item->text());
    query.exec(str);
    qDebug(query.lastError().text().toLocal8Bit());
}
//================= выбор объекта из дерева ====================================================================
void Objectmanager::show_objects(const QModelIndex &index)
{
    UI->property_object->setModel(0);
    clear_tableWidget(UI->coord_table);
 //   QProgressDialog * progress = new QProgressDialog("Формирование информации о регионах", "Отмена", 0, 0,this);
  //  QThread *thr = new QThread(this);
  //  progress->moveToThread(thr);
//progress->show();
 //  progress->moveToThread(new QThread(this));
    QFont font;
    font.setBold(true);
    QVariant id=index.data(Qt::UserRole);
    if (id.type() == QVariant::String)
    {
        QString user_data=id.toString();

        UI->add_many_coord_button->setEnabled(true);
        ;
       // progress->setValue(-1);
        QStringList list=user_data.split("_");
        QStringList listtt;
        if(list.value(0)=="country")
        {
            int id_country = list.value(1).toInt();
            QSqlQuery query;


            query.exec(QString("SELECT id_region, name_region,parent_region FROM region WHERE id_country=%1 order by name_region").arg(id_country));
            model = new QStandardItemModel(this);

            QStandardItem *parentItem = model->invisibleRootItem();

            while (query.next())
            {
                QSqlQuery query_count; // подсчет ********
                query_count.exec(QString("select count(name_region) from region where parent_region = %1").arg(query.value(0).toInt()));

                while (query_count.next())
                {

                    int g = query_count.value(0).toInt(); //********
                    int id_region=query.value(0).toInt();
                    QIcon icon = QIcon(iconsList.at(calc_info_for_region(query.value(0).toString())));
                    QStandardItem *item = new QStandardItem(query.value(1).toString() + " ["  + QString::number(g) + "]");
                    item->setIcon(icon);
                    QString data_region="region_" + QString::number(id_region) + "_" + QString::number(id_country) + "_" + QString::number(calc.get_Rez_on_id_region(id_region));
                    item->setData(data_region,Qt::UserRole);
                    model->appendRow(item);
                    child_region_objects(item,id_region);

                }
            }
            QStandardItem *item = new QStandardItem(QIcon(":/Resources/add.png"),"Добавить регион");
            item->setFont(font);
            item->setData(QString("pregion_%1").arg(id_country),Qt::UserRole);
            model->appendRow(item);
            model->setHeaderData(0,Qt::Horizontal,"Регионы");
            UI->columnView->setModel(model);

        }

    }
    //progress->close();
    return;
}

//================= Расчеты МПО ================================================================================
int Objectmanager::calcul(int id_region){

    int count_smi_ = count_smi(id_region);

    QSqlQuery query;
    QString str = QString("SELECT id_region FROM region WHERE parent_region = %1").arg(id_region);
    if(!query.exec(str))
    {
        return 0;
    }

    QSqlRecord rec = query.record();

    while(query.next())
    {
        int id_reg = query.value(rec.indexOf("id_region")).toInt();
        count_smi_ += calcul(id_reg);
    }
    return count_smi_;
}
int Objectmanager::count_smi(int id){

    int count;

    QSqlQuery query_count;
    query_count.exec(QString("SELECT count(id_smi) FROM smi_region WHERE id_region=%1").arg(id));

    while (query_count.next()){
        count = query_count.value(0).toInt();
    }
    return count;

}
//==============================================================================================================
void Objectmanager::child_region_objects(QStandardItem *parent_item,int id_parent_region)
{
    //select all Region
    QSqlQuery query;
    int row=0;
    query.exec(QString("SELECT id_region, name_region, parent_region FROM region WHERE parent_region=%1 ORDER BY name_region").arg(id_parent_region));

    if (query.size() == 0)
    {

    }
    else
    {
        while (query.next())
        {
            QSqlQuery query_count;
            query_count.exec(QString("select count(name_region) from region where parent_region = %1 ").arg(query.value(0).toInt()));
            while (query_count.next()) {
                int g = query_count.value(0).toInt();
                int id_object=query.value(0).toInt();
                if (id_object == id_parent_region)continue;
                QString icon = iconsList.at(calc_info_for_region(query.value(0).toString()));
                child_region_objects(set_child_item(query.value(1).toString() + " [" + QString::number(g)+ "]",
                                                    "reg_" + QString::number(id_object) + "_" + QString::number(id_parent_region) + "_" + QString::number(calc.get_Rez_on_id_region(id_object)),
                                                    parent_item,
                                                    row,icon),id_object);
                //set_icon(query.value(2).toInt())),
                row++;
            }
        }
    }
    QFont font;
    font.setBold(true);

    set_child_item("Добавить регион",QString("preg_%1").arg(id_parent_region),parent_item,row,":/Resources/add.png" ,font);
    add_region_components(parent_item,id_parent_region,row);
    query.clear();
    UI->searchButton->setEnabled(true);
    UI->searchLineEdit->setEnabled(true);
}

//============================== Все для региона ===============================================================
void Objectmanager::add_region_components(QStandardItem *parent_item,int id_parent_region,int start_row){
    //=== СМИ ===
    QSqlQuery query;
    query.exec(QString("SELECT sm.id_region,sm.id_smi,sm.id_smi_region,poz.id_position_smi,poz.id_smi FROM smi_region sm,smi poz WHERE id_region=%1 AND sm.id_smi = poz.id_smi").arg(id_parent_region));
    int row=start_row+1;
    int row_sw=0;
    int ggg= calcul(id_parent_region);

    int g;
    QSqlQuery query_count;
    query_count.exec(QString("SELECT count(id_smi) FROM smi_region WHERE id_region=%1").arg(id_parent_region));
    while (query_count.next()){
        g = query_count.value(0).toInt();}

    QStandardItem *item=set_child_item("СМИ ["  + QString::number(g) +"/" + QString::number(ggg)+ "]","smi",parent_item,row,":/Resources/printer.png");
    //QStandardItem *item=set_child_item("СМИ","smi",parent_item,row,"./icons/printer.png");
    row++;

    if (query.size() != 0)
    {//	ветка СМИ
        row_sw=0;
        while (query.next())
        {
            QSqlQuery query_smi;
            int id_smi=query.value(1).toInt();
            int id_smi_region=query.value(2).toInt();
            int id_poz_smi = query.value(3).toInt();
            query_smi.exec(QString("SELECT id_smi,name_smi FROM smi WHERE id_smi=%1").arg(query.value(1).toInt()));
            while (query_smi.next())
            {
                if (id_poz_smi == 3 || id_poz_smi == 5) {
                    QBrush b(Qt::blue);
                    child_smi_objects(set_child_item(query_smi.value(1).toString(),QString("dsmi_%1").arg(id_smi_region) + "_" + QString::number(id_smi),item,row_sw,b),
                                      id_smi);
                }
                else if (id_poz_smi == 1 || id_poz_smi == 6){
                    QBrush b(Qt::red);
                    child_smi_objects(set_child_item(query_smi.value(1).toString(),QString("dsmi_%1").arg(id_smi_region) + "_" + QString::number(id_smi),item,row_sw,b),
                                      id_smi);

                }
                else{
                    QBrush b(Qt::black);
                    child_smi_objects(set_child_item(query_smi.value(1).toString(),QString("dsmi_%1").arg(id_smi_region) + "_" + QString::number(id_smi),item,row_sw,b),
                                      id_smi);
                }
            }
            row_sw++;
        }

    }
    QFont font;
    font.setBold(true);

    set_child_item("Добавить CМИ",QString("psmi_%1").arg(id_parent_region),item,row_sw,font);
    query.clear();
    //=== Воинские формирования ===
    query.exec(QString("SELECT id_region, id_ls,name_ls,enimy_ls FROM ls WHERE id_region=%1 ORDER BY name_ls").arg(id_parent_region));
    int row_vf=0;
    int b;

    QSqlQuery query_count_ls;
    query_count_ls.exec(QString("SELECT count(id_ls) FROM ls WHERE id_region=%1").arg(id_parent_region));
    while (query_count_ls.next()){
        b = query_count_ls.value(0).toInt();
    }
    item=set_child_item("ВОИНСКИЕ ФОРМИРОВАНИЯ ["  + QString::number(b) + "]",QString("ls_%1").arg(id_parent_region),parent_item,row,":/Resources/weapon.png");
    row++;
    if (query.size() != 0)
    {//Ветка воинские формирования
        row_vf=0;
        while (query.next())
        {
            int id_ls=query.value(1).toInt();
            int id_region = query.value(0).toInt();
            bool enemy_ls = query.value(3).toBool();
            float mps_for_ls = calc_mps.calculating(id_ls);

            if (enemy_ls == true) {
                QBrush b(Qt::blue);
                child_ls_objects(set_child_item(query.value(2).toString(),QString("chls_%1_%2_%3").arg(id_ls).arg(mps_for_ls).arg(calc_mps.isEnemy(id_ls)),item,row_vf,iconsList.at(calc_mps_for_ls(mps_for_ls)),b),
                                 id_ls);
            }
            else {
                QBrush b(Qt::red);
                child_ls_objects(set_child_item(query.value(2).toString(),QString("chls_%1_%2_%3").arg(id_ls).arg(mps_for_ls).arg(calc_mps.isEnemy(id_ls)),item,row_vf,iconsList.at(calc_mps_for_ls(mps_for_ls)),b),
                                 id_ls);
            }
            row_vf++;
        }
    }

    set_child_item("Добавить Воинские формирования",QString("pls_%1").arg(id_parent_region),item,row_vf,font);
    query.clear();
    //=== Организации ===
    query.exec(QString("SELECT id_region, id_groups,name_groups FROM groups WHERE id_region=%1").arg(id_parent_region));
    int row_gr=0;

    int a;
    QSqlQuery query_count_gr;
    query_count_gr.exec(QString("SELECT count(id_groups) FROM groups WHERE id_region=%1").arg(id_parent_region));

    while (query_count_gr.next()){
        a = query_count_gr.value(0).toInt();}

    item=set_child_item("ОРГАНИЗАЦИИ ["  + QString::number(a) + "]","gr",parent_item,row,":/Resources/group.png");

    //item = set_child_item("ОРГАНИЗАЦИИ","gr",parent_item,row,"./icons/group.png");
    row++;
    if (query.size() != 0)
    {//Ветка организации
        row_gr=0;
        while (query.next())
        {
            QSqlQuery query_gr;
            query_gr.exec(QString("SELECT id_groups,name_groups FROM groups WHERE id_groups=%1").arg(query.value(1).toInt()));
            while (query_gr.next())
            {
                int id_gr=query_gr.value(0).toInt();

                child_groups_objects(set_child_item(query_gr.value(1).toString(),QString("dgr_%1").arg(id_gr),item,row_gr),
                                     id_gr);

                //				(set_child_item(query_gr.value(1).toString(),QString("dgr_%1").arg(query_gr.value(0).toInt()),item,row_gr);

            }
            row_gr++;
        }
    }
    set_child_item("Добавить Организации",QString("pgr_%1").arg(id_parent_region),item,row_gr,font);
    query.clear();
    //=== Условия ===
    query.exec(QString("SELECT id_region, id_special_conditions,name_special_conditions FROM special_conditions WHERE id_region=%1 ORDER BY name_special_conditions").arg(id_parent_region));
    int row_sc=0;

    int y;
    QSqlQuery query_count_sc;
    query_count_sc.exec(QString("SELECT count(id_special_conditions) FROM special_conditions WHERE id_region=%1").arg(id_parent_region));

    while (query_count_sc.next()){
        y = query_count_sc.value(0).toInt();}

    item=set_child_item("ОСОБЫЕ УСЛОВИЯ ["  + QString::number(y) + "]","gr",parent_item,row,":/Resources/Stop2.png");

    //item = set_child_item("ОРГАНИЗАЦИИ","gr",parent_item,row,"./icons/group.png");
    row++;
    if (query.size() != 0)
    {//Ветка условия
        row_sc=0;
        while (query.next())
        {
            QSqlQuery query_sc;
            query_sc.exec(QString("SELECT id_special_conditions,name_special_conditions FROM special_conditions WHERE id_special_conditions=%1 ").arg(query.value(1).toInt()));
            while (query_sc.next())
            {
                int id_sc=query_sc.value(0).toInt();

                //	child_groups_objects(set_child_item(query_sc.value(1).toString(),QString("dsc_%1").arg(id_sc),item,row_sc),
                //										id_sc);
                set_child_item(query_sc.value(1).toString(),QString("dsc_%1").arg(query_sc.value(0).toInt()),item,row_sc);

            }
            row_sc++;
        }
    }
    set_child_item("Добавить Особые условия",QString("psc_%1").arg(id_parent_region),item,row_sc,font);
    query.clear();
}	
void Objectmanager::column_item_clicked ( const QModelIndex &index){

    QFont font;
    font.setBold(true);
    UI->property_object->setModel(0);
    QVariant id=index.data(Qt::UserRole);
    if (id.type() == QVariant::String)
    {
        QString user_data=id.toString();
        QStringList list=user_data.split("_");
        if(list.value(0)=="pregion"){
            add_object *reg = new add_object(list.value(1).toInt(),true);
            reg->setModal(true);
            reg->exec();
        }
        else if(list.value(0)=="preg"){
            add_object *reg = new add_object(list.value(1).toInt(),false);
            reg->setModal(true);
            reg->exec();
        }
        else if(list.value(0)=="psmi"){
            //вызов диалога добавления СМИ на объекте
            Add_elements_dialog *add_element= new Add_elements_dialog(1,list.value(1).toInt());
            add_element->setModal(true);
            int result=add_element->exec();
            //вернул id
            if (result==0)return;
            //обновление модели, добавление нового item-а
            QSqlQuery query_smi;
            query_smi.exec(QString("SELECT s.id_smi,s.name_smi FROM smi_region r,smi s WHERE r.id_smi_region=%1 and s.id_smi=r.id_smi").arg(result));
            while (query_smi.next())
            {
                QString name_smi=query_smi.value(1).toString();
                QString user_data_smi=QString("dsmi_%1_%2").arg(QString::number(result)).arg(query_smi.value(0).toInt());

                QStandardItem *par = model->itemFromIndex(index)->parent();
                int r1 = model->itemFromIndex(index)->parent()->rowCount()-1;
                int r2 = model->itemFromIndex(index)->parent()->rowCount();
                //добавляем в модель новый элемент, затирая "Добавить СМИ"
                child_smi_objects(set_child_item(name_smi,user_data_smi,par,r1),result);

                //вновь добавляем элемент "Добавить СМИ"
                set_child_item("Добавить СМИ",QString("psmi_%1").arg(list.value(1)),par,r2,font);

            }
        }
        //воинские формирования
        else if(list.value(0)=="pls"){

            Add_elements_dialog *add_element= new Add_elements_dialog(2,list.value(1).toInt());
            add_element->setModal(true);
            add_element->setMinimumSize(QSize(500,300));
            add_element->setMaximumSize(QSize(500,300));
            int result=add_element->exec();

            if (result==0)return;
            QSqlQuery query_ls;
            query_ls.exec(QString("SELECT id_ls,name_ls,enimy_ls FROM ls WHERE id_ls=%1").arg(result));
            while (query_ls.next()){
                QString name_ls=query_ls.value(1).toString();
                QString user_data_ls=QString("dls_%1").arg(query_ls.value(0).toInt());
                bool enemy_ls = query_ls.value(2).toBool();
                if (enemy_ls == true) {
                    QBrush b(Qt::blue);
                    QStandardItem *par = model->itemFromIndex(index)->parent();
                    int r1 = model->itemFromIndex(index)->parent()->rowCount()-1;
                    int r2 = model->itemFromIndex(index)->parent()->rowCount();

                    child_ls_objects(set_child_item(name_ls,user_data_ls,par,r1,b),result);

                    set_child_item("Добавить воинское формирование",QString("pls_%1").arg(list.value(1)),par,r2,font);
                }
                else{
                    QBrush b(Qt::red);
                    QStandardItem *par = model->itemFromIndex(index)->parent();
                    int r1 = model->itemFromIndex(index)->parent()->rowCount()-1;
                    int r2 = model->itemFromIndex(index)->parent()->rowCount();
                    //
                    child_ls_objects(set_child_item(name_ls,user_data_ls,par,r1,b),result);
                    //
                    set_child_item("Добавить воинское формирование",QString("pls_%1").arg(list.value(1)),par,r2,font);

                }
            }
        }
        // подчиненные воинские формирования
        else if(list.value(0)=="plss"){

            Add_elements_dialog *add_element= new Add_elements_dialog(5,list.value(1).toInt());
            add_element->setModal(true);
            add_element->setMinimumSize(QSize(500,300));
            add_element->setMaximumSize(QSize(500,300));
            int result=add_element->exec();

            QFont font;
            font.setBold(true);

            if (result==0)return;
            QSqlQuery query_ls;
            query_ls.exec(QString("SELECT id_ls,name_ls,enimy_ls FROM ls WHERE id_ls=%1").arg(result));
            while (query_ls.next()){
                QString name_ls=query_ls.value(1).toString();
                QString user_data_ls=QString("lss_%1").arg(query_ls.value(0).toInt());
                bool enemy_ls = query_ls.value(2).toBool();
                QBrush b;
                if (enemy_ls == true) {
                    b.setColor(Qt::blue);
                }else{
                    b.setColor(Qt::red);
                }
                QStandardItem *par = model->itemFromIndex(index)->parent();
                int r4 = model->itemFromIndex(index)->parent()->rowCount()-6;
                int r1 = model->itemFromIndex(index)->parent()->rowCount()-5;
                int r2 = model->itemFromIndex(index)->parent()->rowCount()-4;
                int r3 = model->itemFromIndex(index)->parent()->rowCount()-3;
                //			int r4 = model->itemFromIndex(index)->parent()->rowCount()-2;
                //			int r5 = model->itemFromIndex(index)->parent()->rowCount()-1;
                //			int r6 = model->itemFromIndex(index)->parent()->rowCount();

                QStandardItem *par_i=par->takeChild(r1);
                QStandardItem *par_y=par->takeChild(r2);
                child_ls_objects(set_child_item(name_ls,user_data_ls,par,r4,b),result);
                set_child_item("Добавить подчиненные ВФ",QString("plss_%1").arg(list.value(1)),par,r1,font);
                par->setChild(r2,par_i);
                par->setChild(r3,par_y);
                //			set_child_item("Национальный состав",QString("nationss_%1").arg(list.value(1)),par,r4,font);
                //			set_child_item("Религиозный состав",QString("confesss_%1").arg(list.value(1)),par,r5,font);
                //			set_child_item("Штатно-должностной состав",QString("rankss_%1").arg(list.value(1)),par,r6,font);
            }

        }
        // ================  организации
        else if(list.value(0)=="pgr"){
            QFont font;
            font.setBold(true);

            Add_elements_dialog *add_element= new Add_elements_dialog(3,list.value(1).toInt());
            add_element->setModal(true);
            int result=add_element->exec();

            if (result==0)return;
            QSqlQuery query_group;
            query_group.exec(QString("SELECT id_groups,name_groups FROM groups WHERE id_groups=%1").arg(result));
            while (query_group.next()){
                QString name_group=query_group.value(1).toString();
                QString user_data_group=QString("dgr_%1").arg(query_group.value(0).toInt());
                QStandardItem *par = model->itemFromIndex(index)->parent();
                int r1 = model->itemFromIndex(index)->parent()->rowCount()-1;
                int r2 = model->itemFromIndex(index)->parent()->rowCount();

                child_groups_objects(set_child_item(name_group,user_data_group,par,r1),result);

                set_child_item("Добавить организацию",QString("pgr_%1").arg(list.value(1)),par,r2,font);
            }
        }
        // ================  условия =========================================================
        else if(list.value(0)=="psc"){
            QFont font;
            font.setBold(true);

            Add_elements_dialog *add_element= new Add_elements_dialog(16,list.value(1).toInt());
            add_element->setModal(true);
            int result=add_element->exec();

            if (result==0)return;
            QSqlQuery query_scond;
            query_scond.exec(QString("SELECT id_special_conditions,name_special_conditions FROM special_conditions WHERE id_special_conditions=%1").arg(result));
            while (query_scond.next()){
                QString name_sc=query_scond.value(1).toString();
                QString user_data_sc=QString("dsc_%1").arg(query_scond.value(0).toInt());
                QStandardItem *par = model->itemFromIndex(index)->parent();
                int r1 = model->itemFromIndex(index)->parent()->rowCount()-1;
                int r2 = model->itemFromIndex(index)->parent()->rowCount();

                set_child_item(name_sc,user_data_sc,par,r1);

                set_child_item("Добавить Особые условия",QString("psc_%1").arg(list.value(1)),par,r2,font);
            }
        }
        // ==================================== средства ======================================
        else if(list.value(0)=="pmpo"){

            Add_elements_dialog *add_element= new Add_elements_dialog(13,list.value(1).toInt());
            add_element->setModal(true);
            int result=add_element->exec();
            //вернул id
            if (result==0)return;

            QSqlQuery query_mpo;
            query_mpo.exec(QString("SELECT mpo.id_mpo_pso,mpo.name_mpo_pso FROM mpo_pso mpo WHERE id_mpo_pso=%1").arg(result));
            while (query_mpo.next())
            {
                QString name_mpo=query_mpo.value(1).toString();
                QString user_data_mpo=QString("dmpo_%1_%2").arg(QString::number(result)).arg(query_mpo.value(0).toInt());

                QStandardItem *par = model->itemFromIndex(index)->parent();
                int r1 = model->itemFromIndex(index)->parent()->rowCount()-1;
                int r2 = model->itemFromIndex(index)->parent()->rowCount();

                set_child_item(name_mpo,user_data_mpo,par,r1);

                set_child_item("Добавить Средства",QString("pmpo_%1").arg(list.value(1)),par,r2,font);

            }
        }
        else if(list.value(0)=="pmpos"){

            Add_elements_dialog *add_element= new Add_elements_dialog(14,list.value(1).toInt());
            add_element->setModal(true);
            int result=add_element->exec();
            //вернул id
            if (result==0)return;

            QSqlQuery query_mpo;
            query_mpo.exec(QString("SELECT mpo.id_mpo_pso,mpo.name_mpo_pso FROM mpo_pso mpo WHERE id_mpo_pso=%1").arg(result));
            while (query_mpo.next())
            {
                QString name_mpo=query_mpo.value(1).toString();
                QString user_data_mpo=QString("dmpos_%1_%2").arg(QString::number(result)).arg(query_mpo.value(0).toInt());

                QStandardItem *par = model->itemFromIndex(index)->parent();
                int r1 = model->itemFromIndex(index)->parent()->rowCount()-1;
                int r2 = model->itemFromIndex(index)->parent()->rowCount();

                set_child_item(name_mpo,user_data_mpo,par,r1);

                set_child_item("Добавить Средства",QString("pmpos_%1").arg(list.value(1)),par,r2,font);

            }
        }
        else if(list.value(0)=="pmposmi"){

            Add_elements_dialog *add_element= new Add_elements_dialog(15,list.value(1).toInt());
            add_element->setModal(true);
            int result=add_element->exec();
            //вернул id
            if (result==0)return;

            QSqlQuery query_mpo;
            query_mpo.exec(QString("SELECT mpo.id_mpo_pso,mpo.name_mpo_pso FROM mpo_pso mpo WHERE id_mpo_pso=%1").arg(result));
            while (query_mpo.next())
            {
                QString name_mpo=query_mpo.value(1).toString();
                QString user_data_mpo=QString("dmposmi_%1_%2").arg(QString::number(result)).arg(query_mpo.value(0).toInt());

                QStandardItem *par = model->itemFromIndex(index)->parent();
                int r1 = model->itemFromIndex(index)->parent()->rowCount()-1;
                int r2 = model->itemFromIndex(index)->parent()->rowCount();

                set_child_item(name_mpo,user_data_mpo,par,r1);

                set_child_item("Добавить Средства",QString("pmposmi_%1").arg(list.value(1)),par,r2,font);

            }
        }
        else if(list.value(0)=="ppers"){
            QString type_elem = "group";
            PersonesData *persona = new PersonesData(type_elem,list.value(1).toInt(),0,this);
            persona->setModal(true);
            persona->showMaximized();
            int result = persona->exec();

//          Add_elements_dialog *add_element= new Add_elements_dialog(23,list.value(1).toInt());
//          add_element->setModal(true);
//          int result=add_element->exec();
            //вернул id
            if (result==0)return;

            QSqlQuery query_pe;
            query_pe.exec(QString("SELECT id_persones,name_persones FROM persones WHERE id_persones=%1").arg(result));
            while (query_pe.next())
            {
                QString name_pers=query_pe.value(1).toString();
                QString user_data_pers=QString("dpers_%1_%2").arg(QString::number(result)).arg(query_pe.value(0).toInt());

                QStandardItem *par = model->itemFromIndex(index)->parent();
                int r1 = model->itemFromIndex(index)->parent()->rowCount()-1;
                int r2 = model->itemFromIndex(index)->parent()->rowCount();

                set_child_item(name_pers,user_data_pers,par,r1);

                set_child_item("Добавить Персоналии",QString("ppers_%1").arg(list.value(1)),par,r2,font);

            }
        }
        else if(list.value(0)=="pperssmi"){

            QString type_elem = "smi";
            PersonesData *persona = new PersonesData(type_elem,list.value(1).toInt(),0,this);
            persona->setModal(true);
            persona->showMaximized();
            int result = persona->exec();

//            Add_elements_dialog *add_element= new Add_elements_dialog(26,list.value(1).toInt());
//            add_element->setModal(true);
//            int result=add_element->exec();
            //вернул id
            if (result==0)return;

            QSqlQuery query_pers_smi;
            query_pers_smi.exec(QString("SELECT id_persones,name_persones FROM persones WHERE id_persones=%1").arg(result));
            while (query_pers_smi.next())
            {
                QString name_pers_smi=query_pers_smi.value(1).toString();
                QString user_data_pers_smi=QString("dperssmi_%1_%2").arg(QString::number(result)).arg(query_pers_smi.value(0).toInt());

                QStandardItem *par = model->itemFromIndex(index)->parent();
                int r1 = model->itemFromIndex(index)->parent()->rowCount()-1;
                int r2 = model->itemFromIndex(index)->parent()->rowCount();

                set_child_item(name_pers_smi,user_data_pers_smi,par,r1);

                set_child_item("Добавить персоналии",QString("pperssmi_%1").arg(list.value(1)),par,r2,font);

            }
        }

        // ======================= для ВФ персоналии ===============================================================
        else if(list.value(0)=="ppersls"){

            QString type_elem = "ls";
            PersonesData *persona = new PersonesData(type_elem,list.value(1).toInt(),0,this);

            persona->setModal(true);
            persona->showMaximized();
            int result = persona->exec();
//            Add_elements_dialog *add_element= new Add_elements_dialog(25,list.value(1).toInt());
//            add_element->setModal(true);
//            int result=add_element->exec();
            //вернул id
            if (result==0)return;

            QSqlQuery query_pe;
            query_pe.exec(QString("SELECT id_persones,name_persones FROM persones WHERE id_persones=%1").arg(result));
            while (query_pe.next())
            {
                QString name_pers=query_pe.value(1).toString();
                QString user_data_pers=QString("dpersls_%1_%2").arg(QString::number(result)).arg(query_pe.value(0).toInt());

                QStandardItem *par = model->itemFromIndex(index)->parent();
                int r1 = model->itemFromIndex(index)->parent()->rowCount()-1;
                int r2 = model->itemFromIndex(index)->parent()->rowCount();

                set_child_item(name_pers,user_data_pers,par,r1);

                set_child_item("Добавить Персоналии",QString("ppersls_%1").arg(list.value(1)),par,r2,font);

            }
        }
        //================== работа с координатами ***** АПГРЕЙД САТУРНА *****
        else if(list.value(0)=="dpers"){
            QString type_elem = "group";
            show_coordinates(list.value(0),list.value(1).toInt(),"coord_persones","id_persones");
            persInfo = new Persones_info(type_elem,list.value(1).toInt(),this);
            persInfo->setModal(true);
            persInfo->exec();

        }// =============== для формирований по персоналу ================================
        else if(list.value(0)=="dpersls"){
            QString type_elem = "ls";
            show_coordinates(list.value(0),list.value(1).toInt(),"coord_persones","id_persones");
            persInfo = new Persones_info(type_elem,list.value(1).toInt(),this);
            persInfo->setModal(true);
            persInfo->exec();
        }
        // =============== для smi по персоналу ================================
        else if(list.value(0)=="dperssmi"){
            QString type_elem = "smi";
            show_coordinates(list.value(0),list.value(1).toInt(),"coord_persones","id_persones");
            persInfo = new Persones_info(type_elem,list.value(1).toInt(),this);
            persInfo->setModal(true);
            persInfo->exec();
        }
        //============== выбор для заполнения таблицы ====================================
        else if(list.value(0)=="region"){
            region_click(list.value(1).toInt());
            show_coordinates(list.value(0),list.value(1).toInt(),"coord_region","id_region");
            UI->add_many_coord_button->setEnabled(true);
        }
        else if(list.value(0)=="reg"){
            region_click(list.value(1).toInt());
            show_coordinates(list.value(0),list.value(1).toInt(),"coord_region","id_region");
            UI->add_many_coord_button->setEnabled(true);
        } // ================= в таблицу данные о СМИ =======================================
        else if(list.value(0)=="dsmi"){
            smi_click(list.value(2).toInt());
        } // ================= в таблицу данные о ВФ =======================
        else if(list.value(0)=="dls"){
            ls_click(list.value(1).toInt());
            show_coordinates(list.value(0),list.value(1).toInt(),"coord_ls","id_ls");
        } // ================= в таблицу данные о Организациях =======================
        else if(list.value(0)=="dgr"){
            gr_click(list.value(1).toInt());
//            show_coordinates(list.value(0),list.value(1).toInt(),"coord_groups","id_groups");

        }	// ================= в таблицу данные о Условиях =======================
        else if(list.value(0)=="dsc"){
            sc_click(list.value(1).toInt());
            show_coordinates(list.value(0),list.value(1).toInt(),"coord_spec_cond","id_special_conditions");
        }//======================= в таблицу воинские формирования (подчиненные)======
        else if(list.value(0)=="lss"){
            ls_click(list.value(1).toInt());
            show_coordinates(list.value(0),list.value(1).toInt(),"coord_ls","id_ls");
        }
        else if(list.value(0)=="chls"){
            ls_click(list.value(1).toInt());
            show_coordinates(list.value(0),list.value(1).toInt(),"coord_ls","id_ls");
        } // ================= в таблицу данные о СРЕДСТВАХ =======================
        else if(list.value(0)=="dmpo" || list.value(0)=="dmpos" || list.value(0)=="dmposmi" ){
            mpo_click(list.value(1).toInt());
            show_coordinates(list.value(0),list.value(1).toInt(),"coord_mpo_pso","id_mpo_pso");
        }
    }
}

//========================== ветка для воинских формирований ===========================================
void Objectmanager::child_ls_objects(QStandardItem *parent_item,int parent_ls)
{	
    QFont font;
    font.setBold(true);
    QSqlQuery query;
    int row=0;
    query.exec(QString("SELECT id_ls, name_ls, parent_ls,enimy_ls FROM ls WHERE parent_ls=%1 ORDER BY name_ls").arg(parent_ls));

    if (query.size() == 0){

    }
    else{
        while (query.next()){
            int id_object=query.value(0).toInt();
            if (id_object == parent_ls)continue;
            float mps_for_ls = calc_mps.calculating(id_object);
            bool enemy_ls = query.value(3).toBool();

            if (enemy_ls == true) {
                QBrush b(Qt::blue);
                child_ls_objects(set_child_item(query.value(1).toString(),
                                                "lss_" + QString::number(id_object) + "_" + QString::number(parent_ls) + "_" + QString::number(mps_for_ls) + "_" + QString("%1").arg(calc_mps.isEnemy(id_object)),
                                                parent_item,
                                                row,iconsList.at(calc_mps_for_ls(mps_for_ls)),b),id_object);}   //b
            else {
                QBrush b(Qt::red);
                child_ls_objects(set_child_item(query.value(1).toString(),
                                                "lss_" + QString::number(id_object) + "_" + QString::number(parent_ls) + "_" + QString::number(mps_for_ls) + "_" + QString("%1").arg(calc_mps.isEnemy(id_object)),
                                                parent_item,
                                                row,iconsList.at(calc_mps_for_ls(mps_for_ls)),b),id_object);} //b
            row++;
        }
    }
    set_child_item("Добавить подчиненные ВФ",QString("plss_%1").arg(parent_ls),parent_item,row,font);
    add_ls_components(parent_item,parent_ls,row);
    query.clear();
}
void Objectmanager::child_groups_objects(QStandardItem *parent_item,int gr)
{
    QFont font;
    font.setBold(true);

    QSqlQuery query;
    int row=0;
    query.exec(QString("SELECT id_groups, name_groups FROM groups WHERE id_groups=%1 ORDER BY name_groups").arg(gr));

    if (query.size() == 0){

    }
    else{
        while (query.next()){
            int id_object=query.value(0).toInt();
            if (id_object == gr)continue;

            child_groups_objects(set_child_item(query.value(1).toString(),
                                                "dgr_" + QString::number(id_object) + "_" + QString::number(gr),
                                                parent_item,
                                                row),id_object);}
        row++;
    }
    add_groups_components(parent_item,gr,row);
    query.clear();
}
void Objectmanager::child_smi_objects(QStandardItem *parent_item,int smi)
{
    QSqlQuery query;
    int row=0;
    query.exec(QString("SELECT id_smi, name_smi FROM smi WHERE id_smi=%1 ORDER BY name_smi").arg(smi));

    if (query.size() == 0){

    }
    else{
        while (query.next()){
            int id_object=query.value(0).toInt();
            if (id_object == smi)continue;

            child_smi_objects(set_child_item(query.value(1).toString(),
                                             "dsm_" + QString::number(id_object) + "_" + QString::number(smi),
                                             parent_item,
                                             row),id_object);}
        row++;
    }
    add_smi_components(parent_item,smi,row);
    query.clear();
}
void Objectmanager::add_ls_components(QStandardItem *parent_item,int id_parent_region,int start_row){
    // ========================================= средства =================================================

    QFont font;
    font.setBold(true);
    QSqlQuery query;
    query.exec(QString("SELECT id_ls,id_mpo_pso FROM mpo_pso WHERE id_ls=%1").arg(id_parent_region));
    int row=start_row+1;
    int row_sw=0;
    int g;
    QSqlQuery query_count;
    query_count.exec(QString("SELECT count(id_mpo_pso) FROM mpo_pso WHERE id_ls=%1").arg(id_parent_region));
    while (query_count.next()){
        g = query_count.value(0).toInt();}

    QStandardItem *item=set_child_item("СРЕДСТВА ["  + QString::number(g) + "]","mpo",parent_item,row,":/Resources/connect_saturn.png");
    //QStandardItem *item=set_child_item("СМИ","smi",parent_item,row,"./icons/printer.png");
    row++;

    if (query.size() != 0)
    {
        row_sw=0;
        while (query.next())
        {
            QSqlQuery query_smi;
            int id_mpo=query.value(1).toInt();
            int id_mpo_ls=query.value(2).toInt();
            query_smi.exec(QString("SELECT id_mpo_pso,name_mpo_pso FROM mpo_pso WHERE id_mpo_pso=%1").arg(query.value(1).toInt()));
            while (query_smi.next())
            {
                set_child_item(query_smi.value(1).toString(),QString("dmpo_%1").arg(id_mpo),item,row_sw);
            }
            row_sw++;
        }

    }
    set_child_item("Добавить Средства",QString("pmpo_%1").arg(id_parent_region),item,row_sw,font);
    query.clear();
    // ========================================= персонал =================================================
    QSqlQuery query_pers;
    query_pers.exec(QString("SELECT id_ls,id_persones FROM persones WHERE id_ls=%1").arg(id_parent_region));
    int row_p=0;
    int f=0;
    QSqlQuery query_count_p;
    query_count_p.exec(QString("SELECT count(id_persones) FROM persones WHERE id_ls=%1").arg(id_parent_region));
    while (query_count_p.next()){
        f = query_count_p.value(0).toInt();}

    item=set_child_item("ПЕРСОНАЛИИ ["  + QString::number(f) + "]","persls",parent_item,row,":/Resources/connect_saturn.png");
    //QStandardItem *item=set_child_item("СМИ","smi",parent_item,row,"./icons/printer.png");
    row++;

    if (query_pers.size() != 0)
    {
        row_p=0;
        while (query_pers.next())
        {
            QSqlQuery query_p;
            int id_person=query_pers.value(1).toInt();
            query_p.exec(QString("SELECT id_persones,name_persones FROM persones WHERE id_persones=%1").arg(query_pers.value(1).toInt()));
            while (query_p.next())
            {
                set_child_item(query_p.value(1).toString(),QString("dpersls_%1").arg(id_person),item,row_p);
            }
            row_p++;
        }

    }
    set_child_item("Добавить Персоналии",QString("ppersls_%1").arg(id_parent_region),item,row_p,font);
    query.clear();
}	

void Objectmanager::add_groups_components(QStandardItem *parent_item,int id_parent_region,int start_row)
{
    // ========================================= средства =================================================
    QFont font;
    font.setBold(true);

    QSqlQuery query;
    query.exec(QString("SELECT id_groups,id_mpo_pso FROM mpo_pso WHERE id_groups=%1").arg(id_parent_region));
    int row=0;
    int row_sw=0;
    int g;
    QSqlQuery query_count;
    query_count.exec(QString("SELECT count(id_mpo_pso) FROM mpo_pso WHERE id_groups=%1").arg(id_parent_region));
    while (query_count.next()){
        g = query_count.value(0).toInt();}

    QStandardItem *item=set_child_item("СРЕДСТВА ["  + QString::number(g) + "]","mpo",parent_item,row,":/Resources/connect_saturn.png");
    //QStandardItem *item=set_child_item("СМИ","smi",parent_item,row,"./icons/printer.png");
    row++;

    if (query.size() != 0)
    {
        row_sw=0;
        while (query.next())
        {
            QSqlQuery query_smi;
            int id_groups=query.value(1).toInt();
            query_smi.exec(QString("SELECT id_mpo_pso,name_mpo_pso FROM mpo_pso WHERE id_mpo_pso=%1").arg(query.value(1).toInt()));
            while (query_smi.next())
            {
                set_child_item(query_smi.value(1).toString(),QString("dmpos_%1").arg(id_groups),item,row_sw);
            }
            row_sw++;
        }

    }
    set_child_item("Добавить Средства",QString("pmpos_%1").arg(id_parent_region),item,row_sw,font);
    query.clear();
    // ========================================= персонал =================================================
    QSqlQuery query_pers;
    query_pers.exec(QString("SELECT id_groups,id_persones FROM persones WHERE id_groups=%1").arg(id_parent_region));
    int row_p=0;
    int f=0;
    QSqlQuery query_count_p;
    query_count_p.exec(QString("SELECT count(id_persones) FROM persones WHERE id_groups=%1").arg(id_parent_region));
    while (query_count_p.next()){
        f = query_count_p.value(0).toInt();}

    item=set_child_item("ПЕРСОНАЛИИ ["  + QString::number(f) + "]","pers",parent_item,row,":/Resources/connect_saturn.png");
    //QStandardItem *item=set_child_item("СМИ","smi",parent_item,row,"./icons/printer.png");
    row++;

    if (query_pers.size() != 0)
    {
        row_p=0;
        while (query_pers.next())
        {
            QSqlQuery query_p;
            int id_groups=query_pers.value(1).toInt();
            query_p.exec(QString("SELECT id_persones,name_persones FROM persones WHERE id_persones=%1").arg(query_pers.value(1).toInt()));
            while (query_p.next())
            {
                set_child_item(query_p.value(1).toString(),QString("dpers_%1").arg(id_groups),item,row_p);
            }
            row_p++;
        }

    }
    set_child_item("Добавить Персоналии",QString("ppers_%1").arg(id_parent_region),item,row_p,font);
    query.clear();

}

void Objectmanager::add_smi_components(QStandardItem *parent_item,int id_parent_region,int start_row)
{
    // ========================================= средства =================================================
    QFont font;
    font.setBold(true);

    QSqlQuery query;
    query.exec(QString("SELECT id_smi,id_mpo_pso FROM mpo_pso WHERE id_smi=%1").arg(id_parent_region));
    int row=0;
    int row_sw=0;
    int g;
    QSqlQuery query_count;
    query_count.exec(QString("SELECT count(id_mpo_pso) FROM mpo_pso WHERE id_smi=%1").arg(id_parent_region));
    while (query_count.next()){
        g = query_count.value(0).toInt();}

    QStandardItem *item=set_child_item("СРЕДСТВА ["  + QString::number(g) + "]","mpo",parent_item,row,":/Resources/connect_saturn.png");
    //QStandardItem *item=set_child_item("СМИ","smi",parent_item,row,"./icons/printer.png");
    row++;

    if (query.size() != 0)
    {
        row_sw=0;
        while (query.next())
        {
            QSqlQuery query_smi;
            int id_smi=query.value(1).toInt();
            query_smi.exec(QString("SELECT id_mpo_pso,name_mpo_pso FROM mpo_pso WHERE id_mpo_pso=%1").arg(query.value(1).toInt()));
            while (query_smi.next())
            {
                set_child_item(query_smi.value(1).toString(),QString("dmposmi_%1").arg(id_smi),item,row_sw);
            }
            row_sw++;
        }

    }
    set_child_item("Добавить Средства",QString("pmposmi_%1").arg(id_parent_region),item,row_sw,font);
    query.clear();
    // ========================================= персонал =================================================
    QSqlQuery query_pers;
    query_pers.exec(QString("SELECT id_smi,id_persones FROM persones WHERE id_smi=%1").arg(id_parent_region));
    int row_p=0;
    int t=0;
    QSqlQuery query_count_p;
    query_count_p.exec(QString("SELECT count(id_persones) FROM persones WHERE id_smi=%1").arg(id_parent_region));
    while (query_count_p.next()){
        t = query_count_p.value(0).toInt();}

    item=set_child_item("ПЕРСОНАЛИИ ["  + QString::number(t) + "]","perssmi",parent_item,row,":/Resources/connect_saturn.png");
    //QStandardItem *item=set_child_item("СМИ","smi",parent_item,row,"./icons/printer.png");
    row++;

    if (query_pers.size() != 0)
    {
        row_p=0;
        while (query_pers.next())
        {
            QSqlQuery query_p;
            int id_smi=query_pers.value(1).toInt();
            query_p.exec(QString("SELECT id_persones,name_persones FROM persones WHERE id_persones=%1").arg(query_pers.value(1).toInt()));
            while (query_p.next())
            {
                set_child_item(query_p.value(1).toString(),QString("dperssmi_%1").arg(id_smi),item,row_p);
            }
            row_p++;
        }

    }
    set_child_item("Добавить Персоналии",QString("pperssmi_%1").arg(id_parent_region),item,row_p,font);
    query.clear();
}

QStandardItem * Objectmanager::set_child_item(QString item_text,QString user_data,QStandardItem *parent_item,int row)
{
    QStandardItem *item = new QStandardItem(item_text.toStdString().c_str());
    item->setData(user_data,Qt::UserRole);

    parent_item->setChild(row,item);
    return item;
}
QStandardItem * Objectmanager::set_child_item(QString item_text,QString user_data,QStandardItem *parent_item,int row,QBrush b)
{
    QStandardItem *item = new QStandardItem(item_text.toStdString().c_str());
    item->setForeground(b);
    item->setData(user_data,Qt::UserRole);
    parent_item->setChild(row,item);
    return item;
}
QStandardItem * Objectmanager::set_child_item(QString item_text,QString user_data,QStandardItem *parent_item,int row,QFont font)
{
    QStandardItem *item = new QStandardItem(item_text.toStdString().c_str());
    item->setFont(font);
    item->setData(user_data,Qt::UserRole);
    parent_item->setChild(row,item);
    return item;
}
QStandardItem * Objectmanager::set_child_item(QString item_text,QString user_data,QStandardItem *parent_item,int row,QString icon_path)	{
    QStandardItem *item = new QStandardItem(QIcon(icon_path),item_text.toStdString().c_str());
    item->setData(user_data,Qt::UserRole);
    parent_item->setChild(row,item);
    return item;
}
QStandardItem * Objectmanager::set_child_item(QString item_text,QString user_data,QStandardItem *parent_item,int row,QString icon_path,QFont font)	{
    QStandardItem *item = new QStandardItem(QIcon(icon_path),item_text.toStdString().c_str());
    item->setFont(font);
    item->setData(user_data,Qt::UserRole);
    parent_item->setChild(row,item);
    return item;
}
QStandardItem * Objectmanager::set_child_item(QString item_text,QString user_data,QStandardItem *parent_item,int row,QString icon_path,QBrush b)	{
    QStandardItem *item = new QStandardItem(QIcon(icon_path),item_text.toStdString().c_str());
    item->setForeground(b);
    item->setData(user_data,Qt::UserRole);
    parent_item->setChild(row,item);
    return item;
}
//============= функции заполнения свойств в tableview ==================================
void Objectmanager::region_click(int id_region){

//***************************************************************************************
//===================== standardItemModel - может пригодится ============================
  /*  QMap<QString, QString> map_info;

    QStandardItemModel *model_region = new QStandardItemModel(UI->info_tableView);
    model_region->setColumnCount(2);

    QStringList list;
    list<<"Характеристика"<<"Свойства";
    model_region->setHorizontalHeaderLabels(list);
    QString name_reg,type_region, desc_reg;
    QSqlQuery query;

    query.exec(QString("SELECT * FROM region WHERE id_region=%1").arg(id_region));

    QSqlRecord rec = query.record();
    //rec.fieldName()
    QMap<QString, QString>::iterator it_;
    map_info.clear();
    QStringList sl,nameList;
    for(int fields=3;fields<rec.count();fields++){
        sl.append(rec.fieldName(fields));
        nameList.append(rec.fieldName(fields));
    }
//    QList<int> hiddens_rows;
//    hiddens_rows.append(81);
    nameList.replace(0,"Наименование");
    nameList.replace(1,"Тип региона");
    nameList.replace(2,"Описание региона");
    //nameList    << "Наименование"<< "Тип региона" << "Описание региона";
    while(query.next()){
        for (int field=0;field<sl.count();field++){
         //   if(hiddens_rows.contains(field)) continue;
            QList<QStandardItem*> temprow;
            QStandardItem *field_name_item = new QStandardItem(nameList.at(field));
            field_name_item->setEditable(false);
            temprow.append(field_name_item);
            QStandardItem *field_value_item = new QStandardItem(query.value(rec.indexOf(sl.at(field))).toString());
            field_value_item->setData("region",Qt::UserRole + 2);
            field_value_item->setData(sl.at(field),Qt::UserRole + 3);
            field_value_item->setData(id_region,Qt::UserRole + 4);
            temprow.append(field_value_item);
            model_region->appendRow(temprow);
        }
    }



    QSqlRelationalDelegate *delegat_reg=new QSqlRelationalDelegate(UI->info_tableView);

    UI->info_tableView->setModel(model_region);

    UI->info_tableView->setColumnWidth(0,200);
    UI->info_tableView->setColumnWidth(1,200);
    UI->info_tableView->setItemDelegateForRow(13,delegat);
    UI->info_tableView->setItemDelegateForRow(14,delegat);
    UI->info_tableView->setItemDelegateForRow(15,delegat);
    //UI->info_tableView->hideRow(1);
    UI->info_tableView->verticalHeader()->moveSection(80,4);
    UI->info_tableView->setItemDelegateForColumn(80,delegat_reg);


    connect(model_region,SIGNAL(itemChanged(QStandardItem*)),this,SLOT(updateDB(QStandardItem*)));

    QProxyModel *modelproxy = new QProxyModel();
    modelproxy->setModel(model_region);
    int i = modelproxy->columnCount();

    qDebug(QString::number(i).toStdString().c_str());*/

//=======================================================================================
//***************************************************************************************
    QSqlRelationalTableModel *model_region = new QSqlRelationalTableModel(this);
        QString db_name=model_region->database().databaseName();
        QStringList tables=model_region->database().tables();


        const QString tableName="region";
        model_region->setTable(tableName);
        model_region->setFilter(QString("id_region=%1").arg(id_region));


        QSqlRelationalDelegate *delegat_reg=new QSqlRelationalDelegate(UI->property_object);

        model_region->setHeaderData(3, Qt::Horizontal,"Наименование региона");model_region->setHeaderData(5, Qt::Horizontal, "Описание региона");

        model_region->setHeaderData(6, Qt::Horizontal, "Численность населения");
        model_region->setHeaderData(7, Qt::Horizontal, "Плотность населения");model_region->setHeaderData(8, Qt::Horizontal, "Уровень имиграции");
        model_region->setHeaderData(9, Qt::Horizontal, "Уровень эммиграции");model_region->setHeaderData(10, Qt::Horizontal, "Уровень рождаемости");
        model_region->setHeaderData(11, Qt::Horizontal, "Уровень смертности");model_region->setHeaderData(12, Qt::Horizontal, "Естественный прирост населения");
        model_region->setHeaderData(13, Qt::Horizontal, "Уровень бедности(спф1)");
        model_region->setHeaderData(14, Qt::Horizontal, "Pспф1");model_region->setHeaderData(15, Qt::Horizontal, "Уровень цен(спф2)");
        model_region->setHeaderData(16, Qt::Horizontal, "Pспф2");model_region->setHeaderData(17, Qt::Horizontal, "Уровень образования(спф3)");
        model_region->setHeaderData(18, Qt::Horizontal, "Pспф3");model_region->setHeaderData(19, Qt::Horizontal, "Степень доверия населения(спф4)");
        model_region->setHeaderData(20, Qt::Horizontal, "Pспф4");model_region->setHeaderData(21, Qt::Horizontal, "Степень поддержки населением(спф5)");
        model_region->setHeaderData(22, Qt::Horizontal, "Pспф5");model_region->setHeaderData(23, Qt::Horizontal, "Поддержка гос.структур(спф6)");
        model_region->setHeaderData(24, Qt::Horizontal, "Pспф6");model_region->setHeaderData(25, Qt::Horizontal, "Поддержка организациями ВС(спф7)");
        model_region->setHeaderData(26, Qt::Horizontal, "Pспф7");model_region->setHeaderData(27, Qt::Horizontal, "Влияние оппозиционных организаций(спф8)");
        model_region->setHeaderData(28, Qt::Horizontal, "Pспф8");model_region->setHeaderData(29, Qt::Horizontal, "Поддержка авторитетных деятелей(спф9)");
        model_region->setHeaderData(30, Qt::Horizontal, "Pспф9");model_region->setHeaderData(31, Qt::Horizontal, "Уровень безработицы(спф10)");
        model_region->setHeaderData(32, Qt::Horizontal, "Pспф10");model_region->setHeaderData(33, Qt::Horizontal, "Миграция населения (спф11)");
        model_region->setHeaderData(34, Qt::Horizontal, "Pспф11");model_region->setHeaderData(35, Qt::Horizontal, "Демография(спф12)");
        model_region->setHeaderData(36, Qt::Horizontal, "Pспф12");model_region->setHeaderData(37, Qt::Horizontal, "Уровень информатизации(спф13)");
        model_region->setHeaderData(38, Qt::Horizontal, "Pспф13");model_region->setHeaderData(39, Qt::Horizontal, "Направленность информации(спф14)");
        model_region->setHeaderData(40, Qt::Horizontal, "Pспф14");model_region->setHeaderData(41, Qt::Horizontal, "Защищенность инфраструктуры(спф15)");
        model_region->setHeaderData(42, Qt::Horizontal, "Pспф15");model_region->setHeaderData(43, Qt::Horizontal, "Неправительственные организации(спф16)");
        model_region->setHeaderData(44, Qt::Horizontal, "Pспф16");model_region->setHeaderData(45, Qt::Horizontal, "Уровень патриотизма(спф17)");
        model_region->setHeaderData(46, Qt::Horizontal, "Pспф17");model_region->setHeaderData(47, Qt::Horizontal, "Уровень преступности(кф1)");
        model_region->setHeaderData(48, Qt::Horizontal, "Pкф1");model_region->setHeaderData(49, Qt::Horizontal, "Коррупция(кф2)");
        model_region->setHeaderData(50, Qt::Horizontal, "Pкф2");model_region->setHeaderData(51, Qt::Horizontal, "Теневой сектор(кф3)");
        model_region->setHeaderData(52, Qt::Horizontal, "Pкф3");model_region->setHeaderData(53, Qt::Horizontal, "Влияние некоренного населения(кф4)");
        model_region->setHeaderData(54, Qt::Horizontal, "Pкф4");model_region->setHeaderData(55, Qt::Horizontal, "Экстремизм,НВФ(кф5)");
        model_region->setHeaderData(56, Qt::Horizontal, "Pкф5");model_region->setHeaderData(57, Qt::Horizontal, "Исправительные учереждения(кф6)");
        model_region->setHeaderData(58, Qt::Horizontal, "Pкф6");model_region->setHeaderData(59, Qt::Horizontal, "Протестная активность(кф7)");
        model_region->setHeaderData(60, Qt::Horizontal, "Pкф7");model_region->setHeaderData(61, Qt::Horizontal, "Организованная преступность(кф8)");
        model_region->setHeaderData(62, Qt::Horizontal, "Pкф8");model_region->setHeaderData(63, Qt::Horizontal, "Оружие,наркотрафик(кф9)");
        model_region->setHeaderData(64, Qt::Horizontal, "Pкф9");model_region->setHeaderData(65, Qt::Horizontal, "Межнациональные конфликты(кф10)");
        model_region->setHeaderData(66, Qt::Horizontal, "Pкф10");model_region->setHeaderData(67, Qt::Horizontal, "Поддержка религиозных объединений(рф1)");
        model_region->setHeaderData(68, Qt::Horizontal, "Pрф1");model_region->setHeaderData(69, Qt::Horizontal, "Культовые сооружения(рф2)");
        model_region->setHeaderData(70, Qt::Horizontal, "Pрф2");model_region->setHeaderData(71, Qt::Horizontal, "Авторитет религиозных лидеров(рф3)");
        model_region->setHeaderData(72, Qt::Horizontal, "Pрф3");model_region->setHeaderData(73, Qt::Horizontal, "Поддержка религиозных лидеров(рф4)");
        model_region->setHeaderData(74, Qt::Horizontal, "Pрф4");model_region->setHeaderData(75, Qt::Horizontal, "Религиозность противника(рф5)");
        model_region->setHeaderData(76, Qt::Horizontal, "Pрф5");model_region->setHeaderData(77, Qt::Horizontal, "Религиозность своих войск(рф6)");
        model_region->setHeaderData(78, Qt::Horizontal, "Pрф6");model_region->setHeaderData(79, Qt::Horizontal, "Групповой коэффициент(спф)");
        model_region->setHeaderData(80, Qt::Horizontal, "Групповой коэффициент(кф)");model_region->setHeaderData(81, Qt::Horizontal, "Групповой коэффициент(рф)");
        model_region->setHeaderData(82, Qt::Horizontal, "Код значка региона");
        model_region->setRelation(83,QSqlRelation("type_region","id_type_region","name_type_region"));
        model_region->setHeaderData(83, Qt::Horizontal, "Тип региона");

        model_region->setEditStrategy(QSqlTableModel::OnFieldChange);

        bool is= model_region->select();
        QString str=model_region->lastError().text();


        UI->property_object->setModel(model_region);

        UI->property_object->hideColumn(0);
        UI->property_object->hideColumn(1);
        UI->property_object->hideColumn(2);
        UI->property_object->horizontalHeader()->moveSection(83,4);
        UI->property_object->hideColumn(4);
        UI->property_object->hideColumn(82);
        UI->property_object->setColumnWidth(3,150);UI->property_object->setColumnWidth(7,150);UI->property_object->setColumnWidth(11,150);
        UI->property_object->setColumnWidth(4,150);UI->property_object->setColumnWidth(8,150);UI->property_object->setColumnWidth(12,200);
        UI->property_object->setColumnWidth(5,150);UI->property_object->setColumnWidth(9,150);UI->property_object->setColumnWidth(13,155);
        UI->property_object->setColumnWidth(6,150);UI->property_object->setColumnWidth(10,150);UI->property_object->setColumnWidth(14,50);
        UI->property_object->setColumnWidth(15,130);UI->property_object->setColumnWidth(16,50);UI->property_object->setColumnWidth(17,170);
        UI->property_object->setColumnWidth(18,50);UI->property_object->setColumnWidth(19,200);UI->property_object->setColumnWidth(20,50);
        UI->property_object->setColumnWidth(21,225);UI->property_object->setColumnWidth(22,50);UI->property_object->setColumnWidth(23,210);
        UI->property_object->setColumnWidth(24,50);UI->property_object->setColumnWidth(25,215);UI->property_object->setColumnWidth(26,50);
        UI->property_object->setColumnWidth(27,265);UI->property_object->setColumnWidth(28,50);UI->property_object->setColumnWidth(29,245);
        UI->property_object->setColumnWidth(30,50);UI->property_object->setColumnWidth(31,180);UI->property_object->setColumnWidth(32,50);
        UI->property_object->setColumnWidth(33,180);UI->property_object->setColumnWidth(34,50);
        UI->property_object->setColumnWidth(35,130);UI->property_object->setColumnWidth(36,50);UI->property_object->setColumnWidth(49,115);
        UI->property_object->setColumnWidth(37,200);UI->property_object->setColumnWidth(38,50);UI->property_object->setColumnWidth(51,145);
        UI->property_object->setColumnWidth(39,220);UI->property_object->setColumnWidth(40,50);UI->property_object->setColumnWidth(53,240);
        UI->property_object->setColumnWidth(41,235);UI->property_object->setColumnWidth(42,50);UI->property_object->setColumnWidth(52,50);
        UI->property_object->setColumnWidth(43,250);UI->property_object->setColumnWidth(44,50);UI->property_object->setColumnWidth(54,50);
        UI->property_object->setColumnWidth(45,175);UI->property_object->setColumnWidth(46,50);UI->property_object->setColumnWidth(48,50);
        UI->property_object->setColumnWidth(47,175);UI->property_object->setColumnWidth(50,50);
        UI->property_object->setColumnWidth(55,150);UI->property_object->setColumnWidth(56,50);
        UI->property_object->setColumnWidth(57,220);UI->property_object->setColumnWidth(58,50);UI->property_object->setColumnWidth(65,225);
        UI->property_object->setColumnWidth(59,180);UI->property_object->setColumnWidth(60,50);UI->property_object->setColumnWidth(66,50);
        UI->property_object->setColumnWidth(61,220);UI->property_object->setColumnWidth(62,50);
        UI->property_object->setColumnWidth(63,170);UI->property_object->setColumnWidth(64,50);
        UI->property_object->setColumnWidth(67,250);UI->property_object->setColumnWidth(68,50);
        UI->property_object->setColumnWidth(69,170);UI->property_object->setColumnWidth(70,50);
        UI->property_object->setColumnWidth(71,235);UI->property_object->setColumnWidth(72,50);
        UI->property_object->setColumnWidth(73,235);UI->property_object->setColumnWidth(74,50);
        UI->property_object->setColumnWidth(75,210);UI->property_object->setColumnWidth(76,50);
        UI->property_object->setColumnWidth(77,210);UI->property_object->setColumnWidth(78,50);
        UI->property_object->setColumnWidth(79,140);
        UI->property_object->setColumnWidth(80,180);
        UI->property_object->setColumnWidth(81,180);
        UI->property_object->setColumnWidth(82,140);
        UI->property_object->setShowGrid(true);

        UI->property_object->setItemDelegateForColumn(13,delegat); UI->property_object->setItemDelegateForColumn(14,delegat);
        UI->property_object->setItemDelegateForColumn(15,delegat); UI->property_object->setItemDelegateForColumn(16,delegat);
        UI->property_object->setItemDelegateForColumn(17,delegat); UI->property_object->setItemDelegateForColumn(18,delegat);
        UI->property_object->setItemDelegateForColumn(19,delegat); UI->property_object->setItemDelegateForColumn(20,delegat);
        UI->property_object->setItemDelegateForColumn(21,delegat); UI->property_object->setItemDelegateForColumn(22,delegat);
        UI->property_object->setItemDelegateForColumn(23,delegat); UI->property_object->setItemDelegateForColumn(24,delegat);
        UI->property_object->setItemDelegateForColumn(25,delegat); UI->property_object->setItemDelegateForColumn(26,delegat);
        UI->property_object->setItemDelegateForColumn(27,delegat); UI->property_object->setItemDelegateForColumn(28,delegat);
        UI->property_object->setItemDelegateForColumn(29,delegat); UI->property_object->setItemDelegateForColumn(30,delegat);
        UI->property_object->setItemDelegateForColumn(31,delegat); UI->property_object->setItemDelegateForColumn(32,delegat);
        UI->property_object->setItemDelegateForColumn(33,delegat); UI->property_object->setItemDelegateForColumn(34,delegat);
        UI->property_object->setItemDelegateForColumn(35,delegat); UI->property_object->setItemDelegateForColumn(36,delegat);
        UI->property_object->setItemDelegateForColumn(37,delegat); UI->property_object->setItemDelegateForColumn(38,delegat);
        UI->property_object->setItemDelegateForColumn(39,delegat); UI->property_object->setItemDelegateForColumn(40,delegat);
        UI->property_object->setItemDelegateForColumn(41,delegat); UI->property_object->setItemDelegateForColumn(42,delegat);
        UI->property_object->setItemDelegateForColumn(43,delegat); UI->property_object->setItemDelegateForColumn(44,delegat);
        UI->property_object->setItemDelegateForColumn(45,delegat); UI->property_object->setItemDelegateForColumn(46,delegat);
        UI->property_object->setItemDelegateForColumn(47,delegat); UI->property_object->setItemDelegateForColumn(48,delegat);
        UI->property_object->setItemDelegateForColumn(49,delegat); UI->property_object->setItemDelegateForColumn(50,delegat);
        UI->property_object->setItemDelegateForColumn(51,delegat); UI->property_object->setItemDelegateForColumn(52,delegat);
        UI->property_object->setItemDelegateForColumn(53,delegat); UI->property_object->setItemDelegateForColumn(54,delegat);
        UI->property_object->setItemDelegateForColumn(55,delegat); UI->property_object->setItemDelegateForColumn(56,delegat);
        UI->property_object->setItemDelegateForColumn(57,delegat); UI->property_object->setItemDelegateForColumn(58,delegat);
        UI->property_object->setItemDelegateForColumn(59,delegat); UI->property_object->setItemDelegateForColumn(60,delegat);
        UI->property_object->setItemDelegateForColumn(61,delegat); UI->property_object->setItemDelegateForColumn(62,delegat);
        UI->property_object->setItemDelegateForColumn(63,delegat); UI->property_object->setItemDelegateForColumn(64,delegat);
        UI->property_object->setItemDelegateForColumn(65,delegat); UI->property_object->setItemDelegateForColumn(66,delegat);
        UI->property_object->setItemDelegateForColumn(67,delegat); UI->property_object->setItemDelegateForColumn(68,delegat);
        UI->property_object->setItemDelegateForColumn(69,delegat); UI->property_object->setItemDelegateForColumn(70,delegat);
        UI->property_object->setItemDelegateForColumn(71,delegat); UI->property_object->setItemDelegateForColumn(72,delegat);
        UI->property_object->setItemDelegateForColumn(73,delegat); UI->property_object->setItemDelegateForColumn(74,delegat);
        UI->property_object->setItemDelegateForColumn(75,delegat); UI->property_object->setItemDelegateForColumn(76,delegat);
        UI->property_object->setItemDelegateForColumn(77,delegat); UI->property_object->setItemDelegateForColumn(78,delegat);
        UI->property_object->setItemDelegateForColumn(79,delegat); UI->property_object->setItemDelegateForColumn(80,delegat);
        UI->property_object->setItemDelegateForColumn(81,delegat);
        UI->property_object->setItemDelegateForColumn(83,delegat_reg);
    //	UI->property_object->setItemDelegate(new QSqlRelationalDelegate(UI->property_object));
    //=============================================================================================

    //=============================================================================================
}
//=======================================================================================
void Objectmanager::smi_click(int id_smi){

    QSqlRelationalTableModel *model_region = new QSqlRelationalTableModel(this);
    if(id_smi<1){
        UI->property_object->setModel(model_region);
        return;
    }
    const QString tableName="smi";
    model_region->setTable(tableName);
    model_region->setFilter(QString("id_smi=%1").arg(id_smi));
    UI->property_object->verticalHeader()->setVisible(true);
    /*
QStandardItemModel *model = new QStandardItemModel(3,3, UI->property_object);
UI->property_object->setModel(model);

for(int row=0; row!=model->rowCount(); ++row){
    for(int column=0; column!=model->columnCount(); ++column) {
        QStandardItem *newItem = new QStandardItem(tr("%1").arg((row+1)*(column+1)));
        model->setItem(row, column, newItem);
    }
}*/

    model_region->setRelation(1,QSqlRelation("type_smi","id_type_smi","nametype_smi"));
    model_region->setHeaderData(1,Qt::Horizontal,"Тип СМИ");
    model_region->setRelation(2,QSqlRelation("type_office_smi","id_type_office_smi","name_type_office_smi"));
    model_region->setHeaderData(2,Qt::Horizontal,"Представительство СМИ");
    model_region->setRelation(3,QSqlRelation("type_broadcast_smi","id_type_broadcast_smi","name_type_broadcast_smi"));
    model_region->setHeaderData(3,Qt::Horizontal,"Способ вещания СМИ");
    model_region->setRelation(4,QSqlRelation("position_smi","id_position_smi","name_position_smi"));
    model_region->setHeaderData(4,Qt::Horizontal,"Направленность СМИ");
    model_region->setRelation(5,QSqlRelation("level_smi","id_level_smi","name_level_smi"));
    model_region->setHeaderData(5,Qt::Horizontal,"Уровень вещания");
    model_region->setRelation(6,QSqlRelation("theme_smi","id_theme_smi","name_theme_smi"));
    model_region->setHeaderData(6,Qt::Horizontal,"Тематика СМИ");
    model_region->setHeaderData(7, Qt::Horizontal,"Наименование СМИ");
    model_region->setHeaderData(8, Qt::Horizontal,"Мощность сигнала");
    model_region->setHeaderData(9, Qt::Horizontal,"Сайт СМИ");
    model_region->setHeaderData(10, Qt::Horizontal,"Описание СМИ");
    model_region->setHeaderData(11, Qt::Horizontal,"Частоты вещания");
    model_region->setHeaderData(13, Qt::Horizontal,"Аудитория СМИ");
    model_region->setHeaderData(14, Qt::Horizontal,"Тираж СМИ");
    //	model_region->setHeaderData(15, Qt::Horizontal,"Защищенность");

    bool is= model_region->select();

    model_region->setEditStrategy(QSqlTableModel::OnFieldChange);

    UI->property_object->clearSpans();
    //  UI->property_object->selectionModel();
    UI->property_object->setModel(model_region);

    UI->property_object->hideColumn(0);
    UI->property_object->hideColumn(12);
    //	UI->property_object->hideColumn(15);

    //	UI->property_object->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    //	UI->property_object->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //	UI->property_object->setAlternatingRowColors(true);

    UI->property_object->setColumnWidth(1,200);UI->property_object->setColumnWidth(2,200);UI->property_object->setColumnWidth(3,200);
    UI->property_object->setColumnWidth(4,200);UI->property_object->setColumnWidth(5,200);UI->property_object->setColumnWidth(6,200);
    UI->property_object->setColumnWidth(7,200);UI->property_object->setColumnWidth(8,200);UI->property_object->setColumnWidth(9,200);
    UI->property_object->setColumnWidth(10,200);UI->property_object->setColumnWidth(11,200);UI->property_object->setColumnWidth(13,200);
    UI->property_object->setColumnWidth(14,200);

    UI->property_object->setShowGrid(true);
    QSqlRelationalDelegate *delegat_smi=new QSqlRelationalDelegate(UI->property_object);
    UI->property_object->setItemDelegateForColumn(13,delegat_smi);UI->property_object->setItemDelegateForColumn(14,delegat_smi);

}
void Objectmanager::ls_click (int id_ls)
{
    QSqlRelationalTableModel *model_region = new QSqlRelationalTableModel(this);

    const QString tableName="ls";
    model_region->setTable(tableName);
    model_region->setFilter(QString("id_ls=%1").arg(id_ls));
    UI->property_object->verticalHeader()->setVisible(true);

    model_region->setRelation(2,QSqlRelation("region","id_region","name_region"));
    model_region->setHeaderData(2,Qt::Horizontal,"Регион");
    model_region->setHeaderData(3,Qt::Horizontal,"Наименование");
    model_region->setHeaderData(4,Qt::Horizontal,"Враждебность подразделения");
    model_region->setHeaderData(5,Qt::Horizontal,"Подразделение МПО");
    model_region->setHeaderData(6,Qt::Horizontal,"Численность л/с");
    model_region->setHeaderData(7,Qt::Horizontal,"Численность л/с в БД");
    model_region->setHeaderData(8,Qt::Horizontal,"л/с по призыву");
    model_region->setHeaderData(9,Qt::Horizontal,"л/с по по контракту");
    model_region->setHeaderData(10,Qt::Horizontal,"Офицеры");
    model_region->setRelation(12,QSqlRelation("type_ls","id_type_ls","name_type_ls"));
    model_region->setHeaderData(12,Qt::Horizontal,"Тип подразделения");
    model_region->setHeaderData(13,Qt::Horizontal,"Сокращенное наименование подразделения");



    bool is= model_region->select();

    model_region->setEditStrategy(QSqlTableModel::OnFieldChange);

    UI->property_object->setModel(model_region);
    UI->property_object->sortByColumn(2,Qt::AscendingOrder);
    UI->property_object->hideColumn(0);
    UI->property_object->hideColumn(1);
    UI->property_object->hideColumn(11);

    UI->property_object->setColumnWidth(2,200);
    UI->property_object->setColumnWidth(3,200);//UI->property_object->setColumnWidth(3,200);
    UI->property_object->setColumnWidth(4,200);
    UI->property_object->setColumnWidth(5,200);
    UI->property_object->setColumnWidth(6,200);
    UI->property_object->setColumnWidth(7,200);
    UI->property_object->setColumnWidth(8,200);
    UI->property_object->setColumnWidth(9,200);
    UI->property_object->setColumnWidth(10,200);
    UI->property_object->setColumnWidth(12,200);
    UI->property_object->setColumnWidth(13,200);

    UI->property_object->setShowGrid(true);
    QSqlRelationalDelegate *delegat_ls=new QSqlRelationalDelegate(UI->property_object);
    UI->property_object->setItemDelegate(delegat_ls);
    UI->property_object->setItemDelegateForColumn(13,delegat_ls);
}
void Objectmanager::gr_click (int id_gr)
{
    QSqlRelationalTableModel *model_region = new QSqlRelationalTableModel(this);

    if(id_gr<1){
        UI->property_object->setModel(model_region);
        return;
    }
    //else UI->property_object->horizontalHeader()->setVisible(true);
    const QString tableName="groups";
    model_region->setTable(tableName);
    model_region->setFilter(QString("id_groups=%1").arg(id_gr));

    model_region->setRelation(1,QSqlRelation("sphere_groups","id_sphere_groups","name_sphere_groups"));
    model_region->setHeaderData(1,Qt::Horizontal,"Сфера деятельности");
    model_region->setRelation(2,QSqlRelation("form_groups","id_form_groups","name_form_groups"));
    model_region->setHeaderData(2,Qt::Horizontal,"Форма организации");
    model_region->setRelation(3,QSqlRelation("trend_groups","id_trend_groups","name_trend_groups"));
    model_region->setHeaderData(3,Qt::Horizontal,"Направленность");
    model_region->setHeaderData(4, Qt::Horizontal,"Наименование организации");
    model_region->setHeaderData(5, Qt::Horizontal,"Численность организации");
    model_region->setHeaderData(6, Qt::Horizontal,"Учредители организации");
    model_region->setHeaderData(7, Qt::Horizontal,"Руководство организации");
    model_region->setHeaderData(8, Qt::Horizontal,"Представительство организации");
    model_region->setHeaderData(9, Qt::Horizontal,"Описание организации");
    model_region->setHeaderData(10, Qt::Horizontal,"Оппозиционность организации");
    model_region->setHeaderData(14, Qt::Horizontal,"Информационные органы");

    bool is= model_region->select();

    model_region->setEditStrategy(QSqlTableModel::OnFieldChange);
    UI->property_object->clearSpans();
    UI->property_object->setModel(model_region);
    UI->property_object->hideColumn(0);
    UI->property_object->hideColumn(11);
    UI->property_object->hideColumn(12);
    UI->property_object->hideColumn(13);

    //UI->property_object->setColumnWidth(1,150);
    UI->property_object->setColumnWidth(1,200);UI->property_object->setColumnWidth(2,200);
    UI->property_object->setColumnWidth(3,200);UI->property_object->setColumnWidth(4,200);UI->property_object->setColumnWidth(5,200);
    UI->property_object->setColumnWidth(6,200);UI->property_object->setColumnWidth(7,200);UI->property_object->setColumnWidth(8,200);
    UI->property_object->setColumnWidth(9,200);UI->property_object->setColumnWidth(10,200);UI->property_object->setColumnWidth(14,200);

    UI->property_object->setShowGrid(true);
    QSqlRelationalDelegate *delegat_gr=new QSqlRelationalDelegate(UI->property_object);
    UI->property_object->setItemDelegate(delegat_gr);
    UI->property_object->setItemDelegateForColumn(13,delegat_gr);UI->property_object->setItemDelegateForColumn(14,delegat_gr);
}

void Objectmanager::mpo_click (int id_mpo){
    QSqlRelationalTableModel *model_region = new QSqlRelationalTableModel(this);

    if(id_mpo<1){
        UI->property_object->setModel(model_region);
        return;
    }
    //else UI->property_object->horizontalHeader()->setVisible(true);
    const QString tableName="mpo_pso";
    model_region->setTable(tableName);
    model_region->setFilter(QString("id_mpo_pso=%1").arg(id_mpo));

    model_region->setRelation(1,QSqlRelation("type_mpo_pso","id_type_mpo_pso","name_type_mpo_pso"));
    model_region->setHeaderData(1,Qt::Horizontal,"Тип средства");
    model_region->setHeaderData(6, Qt::Horizontal,"Наименование средства");
    model_region->setHeaderData(7, Qt::Horizontal,"Количество средств");
    model_region->setHeaderData(8, Qt::Horizontal,"Описание средства");

    bool is= model_region->select();

    model_region->setEditStrategy(QSqlTableModel::OnFieldChange);
    UI->property_object->clearSpans();
    UI->property_object->setModel(model_region);
    UI->property_object->hideColumn(0);
    UI->property_object->hideColumn(2);
    UI->property_object->hideColumn(3);
    UI->property_object->hideColumn(4);
    UI->property_object->hideColumn(5);
    UI->property_object->hideColumn(12);


    //UI->property_object->setColumnWidth(1,150);
    UI->property_object->setColumnWidth(1,200);UI->property_object->setColumnWidth(6,200);
    UI->property_object->setColumnWidth(7,200);UI->property_object->setColumnWidth(8,200);

    UI->property_object->setShowGrid(true);
    QSqlRelationalDelegate *delegat_mpo=new QSqlRelationalDelegate(UI->property_object);
    UI->property_object->setItemDelegate(delegat_mpo);


}

void Objectmanager::sc_click (int id_sc){
    QSqlRelationalTableModel *model_region = new QSqlRelationalTableModel(this);

    if(id_sc<1){
        UI->property_object->setModel(model_region);
        return;
    }
    //else UI->property_object->horizontalHeader()->setVisible(true);
    const QString tableName="special_conditions";
    model_region->setTable(tableName);
    model_region->setFilter(QString("id_special_conditions=%1").arg(id_sc));

    model_region->setRelation(2,QSqlRelation("type_special_conditions","id_type_special_conditions","name_type_special_conditions"));
    model_region->setHeaderData(2,Qt::Horizontal,"Тип особого условия");
    model_region->setHeaderData(3, Qt::Horizontal,"Наименование особого условия");
    model_region->setHeaderData(4, Qt::Horizontal,"Описание особого условия");
    model_region->setHeaderData(5, Qt::Horizontal,"Семантика_1");
    model_region->setHeaderData(6, Qt::Horizontal,"Семантика_2");

    bool is= model_region->select();

    model_region->setEditStrategy(QSqlTableModel::OnFieldChange);
    UI->property_object->clearSpans();
    UI->property_object->setModel(model_region);
    UI->property_object->hideColumn(0);
    UI->property_object->hideColumn(1);


    //UI->property_object->setColumnWidth(1,150);
    UI->property_object->setColumnWidth(2,200);UI->property_object->setColumnWidth(3,200);
    UI->property_object->setColumnWidth(4,200);UI->property_object->setColumnWidth(5,200);
    UI->property_object->setColumnWidth(6,200);

    UI->property_object->setShowGrid(true);
    QSqlRelationalDelegate *delegat_sc=new QSqlRelationalDelegate(UI->property_object);
    UI->property_object->setItemDelegate(delegat_sc);

}


//==========================  удаление по нажатию контекстного меню ================================
void Objectmanager::delete_region(){
    QMessageBox msgBox;
    msgBox.setWindowTitle("Предупреждение");
    msgBox.setText("Вы действительно хотите удалить регион?");

    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setButtonText(QMessageBox::Yes, "Да");
    msgBox.setButtonText(QMessageBox::No, "Нет");
    switch (msgBox.exec()) {
    case QMessageBox::Yes:
        // yes was clicked
        break;
    case QMessageBox::No:
        return;
        break;
    default:
        return;
        break;
    }
    QSqlQuery query;

    QModelIndex index = UI->columnView->currentIndex();
    if(!index.data(Qt::UserRole).toBool()) return;
    QString id_region=index.data(Qt::UserRole).toString();
    QStringList list=id_region.split("_");

    QString str = QString("DELETE FROM region WHERE id_region = %1").arg(list.value(1));//.toInt());

    if(!query.exec(str)){
        QMessageBox::StandardButton ret; ret = QMessageBox::critical (this,"Ошибка",("Удаление региона не выполнено"),QMessageBox::Ok );
        return;
    }
    UI->columnView->model()->removeRow(index.row(),index.parent());

    QMessageBox::StandardButton ret; ret = QMessageBox::information(this,"Предупреждение",("Удаление региона выполнено"),QMessageBox::Ok );
    UI->property_object->setModel(0);

}
void Objectmanager::delete_smi(){
    QMessageBox msgBox;
    msgBox.setWindowTitle("Предупреждение");
    msgBox.setText("Вы действительно хотите удалить СМИ?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setButtonText(QMessageBox::Yes, "Да");
    msgBox.setButtonText(QMessageBox::No, "Нет");
    switch (msgBox.exec()) {
    case QMessageBox::Yes:
        // yes was clicked
        break;
    case QMessageBox::No:
        return;
        break;
    default:
        return;
        break;
    }
    QSqlQuery query;

    QModelIndex index = UI->columnView->currentIndex();
    if(!index.data(Qt::UserRole).toBool()) return;
    QString id_smi_region=index.data(Qt::UserRole).toString();
    QStringList list=id_smi_region.split("_");

    QString str = QString("DELETE FROM smi_region WHERE id_smi_region = %1").arg(list.value(1));

    if(!query.exec(str)){
        QMessageBox::StandardButton ret; ret = QMessageBox::critical (this,"Ошибка",("Удаление СМИ не выполнено"),QMessageBox::Ok );
        return;
    }
    UI->columnView->model()->removeRow(index.row(),index.parent());
    QMessageBox::StandardButton ret; ret = QMessageBox::information(this,"Предупреждение",("Удаление СМИ выполнено"),QMessageBox::Ok );

    UI->property_object->setModel(0);

}
void Objectmanager::delete_ls(){
    QMessageBox msgBox;
    msgBox.setWindowTitle("Предупреждение");
    msgBox.setText("Вы действительно хотите удалить воинское формирование?");

    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setButtonText(QMessageBox::Yes, "Да");
    msgBox.setButtonText(QMessageBox::No, "Нет");
    switch (msgBox.exec()) {
    case QMessageBox::Yes:
        // yes was clicked
        break;
    case QMessageBox::No:
        return;
        break;
    default:
        return;
        break;
    }
    QSqlQuery query;

    QModelIndex index = UI->columnView->currentIndex();
    if(!index.data(Qt::UserRole).toBool()) return;
    QString id_ls=index.data(Qt::UserRole).toString();
    QStringList list=id_ls.split("_");

    QString str = QString("DELETE FROM ls WHERE id_ls = %1").arg(list.value(1));

    if(!query.exec(str)){
        QMessageBox::StandardButton ret; ret = QMessageBox::critical (this,"Ошибка",("Удаление ВФ не выполнено"),QMessageBox::Ok );
        return;
    }
    UI->columnView->model()->removeRow(index.row(),index.parent());
    QMessageBox::StandardButton ret; ret = QMessageBox::information(this,"Предупреждение",("Удаление ВФ выполнено"),QMessageBox::Ok );
    UI->property_object->setModel(0);

    clear_tableWidget(UI->coord_table);
}
void Objectmanager::delete_groups(){
    QMessageBox msgBox;
    msgBox.setWindowTitle("Предупреждение");
    msgBox.setText("Вы действительно хотите удалить организацию?");

    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setButtonText(QMessageBox::Yes, "Да");
    msgBox.setButtonText(QMessageBox::No, "Нет");
    switch (msgBox.exec()) {
    case QMessageBox::Yes:
        // yes was clicked
        break;
    case QMessageBox::No:
        return;
        break;
    default:
        return;
        break;
    }
    QSqlQuery query;

    QModelIndex index = UI->columnView->currentIndex();
    if(!index.data(Qt::UserRole).toBool()) return;
    QString id_groups=index.data(Qt::UserRole).toString();
    QStringList list=id_groups.split("_");

    QString str = QString("DELETE FROM groups WHERE id_groups = %1").arg(list.value(1));

    if(!query.exec(str)){
        QMessageBox::StandardButton ret; ret = QMessageBox::critical (this,"Ошибка",("Удаление организации не выполнено"),QMessageBox::Ok );
        return;
    }
    UI->columnView->model()->removeRow(index.row(),index.parent());
    QMessageBox::StandardButton ret; ret = QMessageBox::information(this,"Предупреждение",("Удаление организации выполнено"),QMessageBox::Ok );
    UI->property_object->setModel(0);
    clear_tableWidget(UI->coord_table);

}
void Objectmanager::delete_mpo(){
    QMessageBox msgBox;
    msgBox.setWindowTitle("Предупреждение");
    msgBox.setText("Вы действительно хотите удалить средство?");

    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setButtonText(QMessageBox::Yes, "Да");
    msgBox.setButtonText(QMessageBox::No, "Нет");
    switch (msgBox.exec()) {
    case QMessageBox::Yes:
        // yes was clicked
        break;
    case QMessageBox::No:
        return;
        break;
    default:
        return;
        break;
    }
    QSqlQuery query;

    QModelIndex index = UI->columnView->currentIndex();
    if(!index.data(Qt::UserRole).toBool()) return;
    QString id_groups=index.data(Qt::UserRole).toString();
    QStringList list=id_groups.split("_");


    QString str1 = QString("DELETE FROM mpo_pso WHERE id_mpo_pso = %1").arg(list.value(1));
    if(!query.exec(str1)){
        QMessageBox::StandardButton ret; ret = QMessageBox::critical (this,"Ошибка",("Удаление средства не выполнено"),QMessageBox::Ok );
        return;
    }
    UI->columnView->model()->removeRow(index.row(),index.parent());
    QMessageBox::StandardButton ret; ret = QMessageBox::information(this,"Предупреждение",("Удаление средства выполнено"),QMessageBox::Ok );
    UI->property_object->setModel(0);
    clear_tableWidget(UI->coord_table);
}
void Objectmanager::delete_sc(){
    QMessageBox msgBox;
    msgBox.setWindowTitle("Предупреждение");
    msgBox.setText("Вы действительно хотите удалить особое условие?");

    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setButtonText(QMessageBox::Yes, "Да");
    msgBox.setButtonText(QMessageBox::No, "Нет");
    switch (msgBox.exec()) {
    case QMessageBox::Yes:
        // yes was clicked
        break;
    case QMessageBox::No:
        return;
        break;
    default:
        return;
        break;
    }
    QSqlQuery query;

    QModelIndex index = UI->columnView->currentIndex();
    if(!index.data(Qt::UserRole).toBool()) return;
    QString id_sc=index.data(Qt::UserRole).toString();
    QStringList list=id_sc.split("_");

    QString str = QString("DELETE FROM special_conditions WHERE id_special_conditions = %1").arg(list.value(1));

    if(!query.exec(str)){
        QMessageBox::StandardButton ret; ret = QMessageBox::critical (this,"Ошибка",("Удаление особого условия не выполнено"),QMessageBox::Ok );
        return;
    }
    UI->columnView->model()->removeRow(index.row(),index.parent());
    QMessageBox::StandardButton ret; ret = QMessageBox::information(this,"Предупреждение",("Удаление особого условия выполнено"),QMessageBox::Ok );
    UI->property_object->setModel(0);
    clear_tableWidget(UI->coord_table);
}
void Objectmanager::edit_persones(){
    QString type_elem = "";
    PersonesData *pers = new PersonesData(type_elem,0,id_persers,this);
	pers->showMaximized();



}
void Objectmanager::clicked_open_file()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Выбор фотографии", "",
                                                    "Images (*.jpg *.png)");
    if (fileName.isEmpty()) return;
    //	edit_dlg_pers->raise();

    // label_foto->setText(fileName);

    QSqlQuery query;

    query.prepare("UPDATE persones SET image_persones = ? WHERE id_persones = ?");

    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Внимание"));
        msgBox.setText(tr("Изображение не выбрано"));
        msgBox.setStandardButtons(QMessageBox::Yes);
        switch (msgBox.exec()) {
        case QMessageBox::Yes:
            return;
            break;
        }
    }
    QByteArray image_pers = file.readAll();
    query.addBindValue(image_pers);
    query.addBindValue(id_persers);

    if(!query.exec())
    {
        QString s = query.lastError().text();
    }

    QPixmap pixmap;
    QSize size_pic(200,200);
    pixmap.loadFromData(image_pers);
    pixmap = pixmap.scaled(size_pic,Qt::KeepAspectRatio);
    label_foto->setPixmap(pixmap);
    label_foto->setAlignment(Qt::AlignCenter);
    edit_dlg_pers->raise();

}
void Objectmanager::delete_pers(){
    QMessageBox msgBox;
    msgBox.setWindowTitle("Предупреждение");
    msgBox.setText("Вы действительно хотите удалить данные по персоналу?");

    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setButtonText(QMessageBox::Yes, "Да");
    msgBox.setButtonText(QMessageBox::No, "Нет");
    switch (msgBox.exec()) {
    case QMessageBox::Yes:
        // yes was clicked
        break;
    case QMessageBox::No:
        return;
        break;
    default:
        return;
        break;
    }
    QSqlQuery query;

    QModelIndex index = UI->columnView->currentIndex();
    if(!index.data(Qt::UserRole).toBool()) return;
    QString id_sc=index.data(Qt::UserRole).toString();
    QStringList list=id_sc.split("_");

    QString str = QString("DELETE FROM persones WHERE id_persones = %1").arg(list.value(1));

    if(!query.exec(str)){
        QMessageBox::StandardButton ret; ret = QMessageBox::critical (this,"Ошибка",("Удаление данных по персоналу не выполнено"),QMessageBox::Ok );
        return;
    }
    UI->columnView->model()->removeRow(index.row(),index.parent());
    QMessageBox::StandardButton ret; ret = QMessageBox::information(this,"Предупреждение",("Удаление данных по персоналу выполнено"),QMessageBox::Ok );
    UI->property_object->setModel(0);
    clear_tableWidget(UI->coord_table);

}
//============================      отчеты    ================================================================
void Objectmanager::otchet_groups()
{
    QModelIndex index = UI->columnView->currentIndex();
    if(!index.data(Qt::UserRole).toBool()) return;
    QVariant id=index.data(Qt::UserRole);
    if (id.type() == QVariant::String){
        QString user_data=id.toString();
        QStringList list=user_data.split("_");
        if(list.value(0)=="dgr"){
            Reports *r = new Reports;
            int id_suka = list.value(1).toInt();
            QString report = r->create_object_formular(id_suka);
            r->show_preview_dialog(report);
        }
        else if(list.value(0)=="dsmi"){
            Reports *r = new Reports;
            int id_suka_smi = list.value(2).toInt();
            QString report = r->create_object_formular_smi(id_suka_smi);
            r->show_preview_dialog(report);
        }
        else if((list.value(0)=="chls") || (list.value(0) == "lss")){
            Reports *r = new Reports;
            int id_suka_ls = list.value(1).toInt();
            QString report = r->create_object_formular_ls(id_suka_ls);
            r->show_preview_dialog(report);
        }
        else if((list.value(0)=="region") || (list.value(0) == "reg")){
            Reports *r = new Reports;
            int id_region = list.value(1).toInt();
            float rez_ = list.value(3).toFloat();
            QString report = r->create_object_formular_region(rez_,id_region);
            r->show_preview_dialog(report);
        }
        else if(list.value(0)=="dsc"){
            Reports *r = new Reports;
            int id_suka_sc = list.value(1).toInt();
            QString report = r->create_object_formular_sc(id_suka_sc);
            r->show_preview_dialog(report);
        }
        else if(list.value(0)=="dmpo"){
            Reports *r = new Reports;
            int id_suka_ls_mpo = list.value(1).toInt();
            QString report = r->create_object_formular_mpo_pso_ls(id_suka_ls_mpo);
            r->show_preview_dialog(report);
        }
        else if(list.value(0)=="dmpos"){
            Reports *r = new Reports;
            int id_suka_gr_mpo = list.value(1).toInt();
            QString report = r->create_object_formular_mpo_pso_gr(id_suka_gr_mpo);
            r->show_preview_dialog(report);
        }
        else if(list.value(0)=="dmposmi"){
            Reports *r = new Reports;
            int id_suka_smi_mpo = list.value(1).toInt();
            QString report = r->create_object_formular_mpo_pso_smi(id_suka_smi_mpo);
            r->show_preview_dialog(report);
        }
        else if ((list.value(0)=="dpers") || (list.value(0)=="dperssmi") || (list.value(0)=="dpersls"))   {
            Reports *r = new Reports;
            int id_suka_pers = list.value(1).toInt();
            QString report = r->create_object_formular_pers(id_suka_pers);
            r->show_preview_dialog(report);
        }
    }
}
// QString str = QString("select gr.name_groups,gr.counte_groups,gr.founder_group,gr.menegement_groups,gr.officce_groups,gr.description_groups,gr.propaganda_groups,tr.name_trend_groups,sph.name_sphere_groups, form.name_form_groups, reg.name_region FROM groups gr,trend_groups tr,sphere_groups sph, form_groups form, region reg where gr.id_trend=tr.id_trend_groups AND gr.id_sphere_groups=sph.id_sphere_groups AND gr.id_form_groups=form.id_form_groups AND gr.id_region = reg.id_region AND gr.id_groups=%1").arg(group_id);
//==============================   расчеты   =============================================================
int Objectmanager::calc_info_for_region(QString id_region)
{
    Calculate_K_omkrf calc;
    calc.set_parametr(id_region);
    calc.calc_Rezultat();
    float rez = calc.getRezultate();
    if(rez > 0 && rez < 0.3)
        return 1;
    if(rez >= 0.3 && rez < 0.5)
        return 2;
    if(rez >=0.5 && rez <= 0.8)
        return 3;
    if(rez == 0)
        return 0;
    if(rez < 0 || rez > 0.8)
        return -1;
    //return -678;
}
int Objectmanager::calc_mps_for_ls(float n)
{
    if(n > 0 && n < 0.3)
        return 1;
    if(n >= 0.3 && n < 0.6)
        return 2;
    if(n >=0.6 && n <= 1)
        return 3;
    if(n == 0)
        return 0;
    if(n < 0 || n > 1)
        return -1;
}

void Objectmanager::add_new_coordinates()
{
    add_coord = new QDialog();

    add_coord->setWindowTitle(tr("Добавить новые координаты"));

    QModelIndex index = UI->columnView->currentIndex();
    if(!index.data(Qt::UserRole).toBool()) return;
    QString id_sc=index.data(Qt::UserRole).toString();
    QStringList list=id_sc.split("_");
    int id_obj = list.value(1).toInt();

    QLabel *lab_wgs = new QLabel("<b>" + tr("WGS-84") + "</b>");
    lab_wgs->setAlignment(Qt::AlignCenter);

    QLabel *lab1 = new QLabel(tr("N ")+QChar(176)+" :");
    QLabel *lab2 = new QLabel(tr("N ") + "' :");
    QLabel *lab3 = new QLabel(tr("N ") + "\" :");
    QLabel *lab4 = new QLabel(tr("E ")+QChar(176)+" :");
    QLabel *lab5 = new QLabel(tr("E ") + "' :");
    QLabel *lab6 = new QLabel(tr("E ") + "\" :");

    QLabel *lab_rect = new QLabel("<b>" + tr("Прямоугольные") + "</b>");
    lab_rect->setAlignment(Qt::AlignCenter);

    QLabel *lab15 = new QLabel(tr("X ") + ":");
    QLabel *lab16 = new QLabel(tr("Y ") + ":");

    e1 = new QLineEdit();
    e2 = new QLineEdit();
    e3 = new QLineEdit();
    e4 = new QLineEdit();
    e5 = new QLineEdit();
    e6 = new QLineEdit();
    e15 = new QLineEdit();
    e16 = new QLineEdit();

    QPushButton *wgs_button = new QPushButton(tr("Перевести"));
    QPushButton *plane_button = new QPushButton(tr("Перевести"));
    connect(wgs_button,SIGNAL(clicked()),this,SLOT(WGS_to_other()));
    connect(plane_button,SIGNAL(clicked()),this,SLOT(PLANE_to_other()));

    QGridLayout *grid = new QGridLayout();

    grid->addWidget(lab_wgs,0,0,1,2);

    grid->addWidget(lab1,1,0);
    grid->addWidget(e1,1,1);
    grid->addWidget(lab2,2,0);
    grid->addWidget(e2,2,1);
    grid->addWidget(lab3,3,0);
    grid->addWidget(e3,3,1);
    grid->addWidget(lab4,4,0);
    grid->addWidget(e4,4,1);
    grid->addWidget(lab5,5,0);
    grid->addWidget(e5,5,1);
    grid->addWidget(lab6,6,0);
    grid->addWidget(e6,6,1);

    grid->addWidget(wgs_button,7,1);

    grid->addWidget(lab_rect,0,3,1,2);
    grid->addWidget(lab15,1,3);
    grid->addWidget(e15,1,4);
    grid->addWidget(lab16,2,3);
    grid->addWidget(e16,2,4);
    grid->addWidget(plane_button,3,4);


    QPushButton *ok_button = new QPushButton("OK");
    connect(ok_button,SIGNAL(clicked()),add_coord,SLOT(accept()));
    QPushButton *cancel_button = new QPushButton("Отмена");
    connect(cancel_button,SIGNAL(clicked()),add_coord,SLOT(close()));

    QHBoxLayout *b_lay = new QHBoxLayout;
    b_lay->addStretch();
    b_lay->addWidget(ok_button);
    b_lay->addWidget(cancel_button);

    QVBoxLayout *main_layout = new QVBoxLayout;
    main_layout->addLayout(grid);
    main_layout->addLayout(b_lay);

    add_coord->setLayout(main_layout);

    if(list.value(0)=="nations" || list.value(0)=="nationss" || list.value(0)=="nat" || list.value(0)=="dsmi" || list.value(0)=="ran" || list.value(0)=="rankss"
            || list.value(0)=="sexss" || list.value(0)=="sex" || list.value(0)=="se" || list.value(0)=="ag" || list.value(0)=="agess" || list.value(0)=="prof"
            || list.value(0)=="profess" || list.value(0)=="confess" || list.value(0)=="confesss" || list.value(0)=="conf" || list.value(0)=="psmi"
            || list.value(0)=="smi" || list.value(0)=="ls" || list.value(0)=="gr" || list.value(0)=="dgr"){

        QMessageBox::StandardButton ret;
        ret = QMessageBox::critical (this,"Ошибка",("Нет возможности ввести координаты "),QMessageBox::Ok );
    }
    else if(add_coord->exec() == QDialog::Accepted)
    {

        QSqlQuery query;

        query.prepare("INSERT INTO coordinates (latitude_wgs_84_g,latitude_wgs_84_m,latitude_wgs_84_s,longitude_wgs_84_g,longitude_wgs_84_m,longitude_wgs_84_s,x_coordinates,y_coordinates) VALUES (?,?,?,?,?,?,?,?)RETURNING id_coordinates");
        query.addBindValue(e1->text().toInt());
        query.addBindValue(e2->text().toInt());
        query.addBindValue(e3->text().toDouble());
        query.addBindValue(e4->text().toInt());
        query.addBindValue(e5->text().toInt());
        query.addBindValue(e6->text().toDouble());
        query.addBindValue(e15->text().toDouble());
        query.addBindValue(e16->text().toDouble());

        if(!query.exec())
        {
            QString str22= query.lastError().databaseText();
            return;
        }
        // Определение последнего id_coordinates
        int id_coordinates;
        while (query.next())
        {
            id_coordinates=query.value(0).toInt();
        }

        if(list.value(0)=="dls" || list.value(0)=="chls" || list.value(0)=="lss" ){

            query.clear();
            query.prepare("INSERT INTO coord_ls (id_coordinates, id_ls) VALUES (?,?)");
            query.addBindValue(id_coordinates);
            query.addBindValue(id_obj);

            if(!query.exec())
            {
                QString sds = query.lastError().text();
            }

            show_coordinates(list.value(0),list.value(1).toInt(),"coord_ls","id_ls");

            return;
        }

        else if(list.value(0)=="dgr"){
            query.clear();
            query.prepare("INSERT INTO coord_groups (id_coordinates, id_groups) VALUES (?,?)");
            query.addBindValue(id_coordinates);
            query.addBindValue(id_obj);

            if(!query.exec())
            {
                QString sds = query.lastError().text();
            }
            show_coordinates(list.value(0),list.value(1).toInt(),"coord_groups","id_groups");

            return;
        }
        else if(list.value(0)=="region" || list.value(0)=="reg"){
            query.clear();
            query.prepare("INSERT INTO coord_region (id_coordinates, id_region) VALUES (?,?)");
            query.addBindValue(id_coordinates);
            query.addBindValue(id_obj);

            if(!query.exec())
            {
                QString sds = query.lastError().text();
            }
            show_coordinates(list.value(0),list.value(1).toInt(),"coord_region","id_region");

            return;

        }
        else if(list.value(0)=="dsc"){
            query.clear();
            query.prepare("INSERT INTO coord_spec_cond (id_coordinates, id_special_conditions) VALUES (?,?)");
            query.addBindValue(id_coordinates);
            query.addBindValue(id_obj);

            if(!query.exec())
            {
                QString sds = query.lastError().text();
            }
            show_coordinates(list.value(0),list.value(1).toInt(),"coord_spec_cond","id_special_conditions");

            return;

        }
        else if(list.value(0)=="dmpo" || list.value(0)=="dmpos" || list.value(0)=="dmposmi" ){
            query.clear();
            query.prepare("INSERT INTO coord_mpo_pso (id_coordinates, id_mpo_pso) VALUES (?,?)");
            query.addBindValue(id_coordinates);
            query.addBindValue(id_obj);

            if(!query.exec())
            {
                QString sds = query.lastError().text();
            }
            show_coordinates(list.value(0),list.value(1).toInt(),"coord_mpo_pso","id_mpo_pso");

            return;
        }
        else if(list.value(0)=="dpers" || list.value(0)=="dperssmi" || list.value(0)=="dpersls" ){
            query.clear();
            query.prepare("INSERT INTO coord_persones(id_coordinates, id_persones) VALUES (?,?)");
            query.addBindValue(id_coordinates);
            query.addBindValue(id_obj);

            if(!query.exec())
            {
                QString sds = query.lastError().text();
            }
            show_coordinates(list.value(0),list.value(1).toInt(),"coord_persones","id_persones");

            return;
        }
        return;
    }
}
//========================== добавление списка координат =================================
void Objectmanager::show_dialog_add_file()
{
    int id_obj = 0;
    QModelIndex index = UI->columnView->currentIndex();
    if(!index.data(Qt::UserRole).toBool()) return;
    QString id_sc=index.data(Qt::UserRole).toString();
    QStringList list_id=id_sc.split("_");
    id_obj = list_id.value(1).toInt  ();

    if(list_id.value(0)=="region" || list_id.value(0)=="reg" ) {

        QString filepath = QFileDialog::getOpenFileName(this,
                                                        "Открыть txt-файл", "../", tr("Text Files (*.txt *.csv)"));

        ///============== проверку сделать на пустой стринг ========================================

        QFile file(filepath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return;

        QSqlQuery query;


        query.clear();
        QString str = QString("SELECT id_coordinates FROM coord_region where id_region=%1").arg(id_obj);
        query.prepare(str);
        if(!query.exec())
        {
            QString err = query.lastError().text();
            return;
        }
        QSqlRecord rec = query.record();
        while(query.next())
        {
            int id_coord = query.value(rec.indexOf("id_coordinates")).toInt();
            QSqlQuery query1;
            query1.prepare("DELETE FROM coordinates WHERE id_coordinates = ?");
            query1.addBindValue(id_coord);
            if(!query1.exec())
            {
                QString err = query1.lastError().text();
                return;
            }
        }
        QSettings *settings = new QSettings("vka","saturnMap");
        QString mapPath = settings->value("/mapSettings/mapPath","").toString();
        if(mapPath.isEmpty())
        {
            //================MessageBox===============================
            QMessageBox msgBox;
            msgBox.setWindowTitle("Сообщение");
            msgBox.setText("Вы должны открыть карту, чтобы перевести координаты объекта\nОткрыть карту??");
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            msgBox.setButtonText(QMessageBox::Yes, "Да");
            msgBox.setButtonText(QMessageBox::No, "Нет");

            switch (msgBox.exec()) {
            case QMessageBox::Yes:
                mapPath = QFileDialog::getOpenFileName(this, NULL, NULL,"Maps (*.map)" );
                break;
            case QMessageBox::No:
                return;
                break;
            default:
                return;
                break;
            }
            //==============================================================
        }

        if(!query.exec(str))   {
            QString err = query.lastError().text();
            int sa=0;
            return;
        }

        while (!file.atEnd())  {
            QString line = file.readLine();
            QStringList list = line.split("   ");
            if (list.value(0)=="" || list.value(1) == ""){
                QMessageBox::StandardButton ret;
                ret = QMessageBox::critical (this,"Ошибка",("В файле нет координат "),QMessageBox::Ok );
                return;
            }

            MyMapAccess *map = new MyMapAccess();
            hmap = 0;
            hmap = map->mapOpen(mapPath.toStdString().c_str(),0);
            if(hmap == 0) return;
            int nD,nM,eD,eM;
            double nS,eS;
            if(map->mapIsGeoSupported(hmap)){
                GEODEGREE N, E;
                double N_rad, E_rad, H;
                N_rad = list.value(0).toDouble();;
                E_rad = list.value(1).toDouble();;
                map->mapPlaneToGeoWGS843D(hmap,&N_rad,&E_rad,&H);
                map->mapRadianToDegree(&N_rad,&N);
                map->mapRadianToDegree(&E_rad,&E);
                nD = N.Degree;
                nM = N.Minute;
                nS = N.Second;
                eD = E.Degree;
                eM = E.Minute;
                eS = E.Second;
            }

            if(hmap){
                map->mapCloseData(hmap);
            }

            query.clear();
            query.prepare("INSERT INTO coordinates(latitude_wgs_84_g,latitude_wgs_84_m,latitude_wgs_84_s,longitude_wgs_84_g,longitude_wgs_84_m,longitude_wgs_84_s) VALUES (?,?,?,?,?,?)RETURNING id_coordinates");

            query.addBindValue(nD);
            query.addBindValue(nM);
            query.addBindValue(nS);
            query.addBindValue(eD);
            query.addBindValue(eM);
            query.addBindValue(eS);

            if(!query.exec()) {
                QString err = query.lastError().text();
                return;
            }
            int id_coordinates=0;

            while (query.next()){
                id_coordinates=query.value(0).toInt();
            }
            query.prepare("INSERT INTO coord_region (id_coordinates, id_region) VALUES (?,?)");
            query.addBindValue(id_coordinates);
            query.addBindValue(id_obj);

            if(!query.exec()) {
                QString err = query.lastError().text();
                return;
            }

        }

        show_coordinates(list_id.value(0),list_id.value(1).toInt(),"coord_region","id_region");
        return;
    }

    else if(list_id.value(0)=="dsc" ) {

        QString filepath = QFileDialog::getOpenFileName(this,
                                                        "Открыть txt-файл", "../", tr("Text Files (*.txt *.csv)"));

        ///============== проверку сделать на пустой стринг ========================================

        QFile file(filepath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return;

        QSqlQuery query;


        query.clear();
        QString str = QString("SELECT id_coordinates FROM coord_spec_cond where id_special_conditions=%1").arg(id_obj);
        query.prepare(str);
        if(!query.exec())
        {
            QString err = query.lastError().text();
            return;
        }
        QSqlRecord rec = query.record();
        while(query.next())
        {
            int id_coord = query.value(rec.indexOf("id_coordinates")).toInt();
            QSqlQuery query1;
            query1.prepare("DELETE FROM coordinates WHERE id_coordinates = ?");
            query1.addBindValue(id_coord);
            if(!query1.exec())
            {
                QString err = query1.lastError().text();
                return;
            }
        }
        QSettings *settings = new QSettings("vka","saturnMap");
        QString mapPath = settings->value("/mapSettings/mapPath","").toString();
        if(mapPath.isEmpty())
        {
            //================MessageBox===============================
            QMessageBox msgBox;
            msgBox.setWindowTitle("Сообщение");
            msgBox.setText("Вы должны открыть карту, чтобы перевести координаты объекта\nОткрыть карту??");
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            msgBox.setButtonText(QMessageBox::Yes, "Да");
            msgBox.setButtonText(QMessageBox::No, "Нет");

            switch (msgBox.exec()) {
            case QMessageBox::Yes:
                mapPath = QFileDialog::getOpenFileName(this, NULL, NULL,"Maps (*.map)" );
                break;
            case QMessageBox::No:
                return;
                break;
            default:
                return;
                break;
            }
            //==============================================================
        }

        if(!query.exec(str))   {
            QString err = query.lastError().text();
            int sa=0;
            return;
        }

        while (!file.atEnd())  {
            QString line = file.readLine();
            QStringList list = line.split("   ");
            if (list.value(0)=="" || list.value(1) == ""){
                QMessageBox::StandardButton ret;
                ret = QMessageBox::critical (this,"Ошибка",("В файле нет координат "),QMessageBox::Ok );
                return;
            }

            MyMapAccess *map = new MyMapAccess();
            hmap = 0;
            hmap = map->mapOpen(mapPath.toStdString().c_str(),0);
            if(hmap == 0) return;
            int nD,nM,eD,eM;
            double nS,eS;
            if(map->mapIsGeoSupported(hmap)){
                GEODEGREE N, E;
                double N_rad, E_rad, H;
                N_rad = list.value(0).toDouble();;
                E_rad = list.value(1).toDouble();;
                map->mapPlaneToGeoWGS843D(hmap,&N_rad,&E_rad,&H);
                map->mapRadianToDegree(&N_rad,&N);
                map->mapRadianToDegree(&E_rad,&E);
                nD = N.Degree;
                nM = N.Minute;
                nS = N.Second;
                eD = E.Degree;
                eM = E.Minute;
                eS = E.Second;
            }

            if(hmap){
                map->mapCloseData(hmap);
            }

            query.clear();
            query.prepare("INSERT INTO coordinates(latitude_wgs_84_g,latitude_wgs_84_m,latitude_wgs_84_s,longitude_wgs_84_g,longitude_wgs_84_m,longitude_wgs_84_s) VALUES (?,?,?,?,?,?)RETURNING id_coordinates");

            query.addBindValue(nD);
            query.addBindValue(nM);
            query.addBindValue(nS);
            query.addBindValue(eD);
            query.addBindValue(eM);
            query.addBindValue(eS);

            if(!query.exec()) {
                QString err = query.lastError().text();
                return;
            }
            int id_coordinates=0;

            while (query.next()){
                id_coordinates=query.value(0).toInt();
            }
            query.prepare("INSERT INTO coord_spec_cond (id_coordinates, id_special_conditions) VALUES (?,?)");
            query.addBindValue(id_coordinates);
            query.addBindValue(id_obj);

            if(!query.exec()) {
                QString err = query.lastError().text();
                return;
            }

        }

        show_coordinates(list_id.value(0),list_id.value(1).toInt(),"coord_spec_cond","id_special_conditions");
        return;
    }


    else if (list_id.value(0)=="nations" || list_id.value(0)=="nationss" || list_id.value(0)=="nat" || list_id.value(0)=="dsmi" || list_id.value(0)=="ran" || list_id.value(0)=="rankss"
             || list_id.value(0)=="sexss" || list_id.value(0)=="sex" || list_id.value(0)=="se" || list_id.value(0)=="ag" || list_id.value(0)=="agess" || list_id.value(0)=="prof"
             || list_id.value(0)=="profess" || list_id.value(0)=="confess" || list_id.value(0)=="confesss" || list_id.value(0)=="conf" || list_id.value(0)=="psmi"
             || list_id.value(0)=="smi" || list_id.value(0)=="ls" || list_id.value(0)=="gr" || list_id.value(0)=="mpo" || list_id.value(0)=="perssmi" || list_id.value(0)=="persls"
             || list_id.value(0)=="dls" || list_id.value(0)=="chls" || list_id.value(0)=="lss" || list_id.value(0)=="dgr" || list_id.value(0)=="dmpo"
             || list_id.value(0)=="dmpos" || list_id.value(0)=="dmposmi" || list_id.value(0)=="dpers" || list_id.value(0)=="dperssmi" || list_id.value(0)=="dpersls" || list_id.value(0)=="pers"){
        QMessageBox::StandardButton ret;
        ret = QMessageBox::critical (this,"Ошибка",("Для данного объекта невозможно добавить список координат"),QMessageBox::Ok );
        return;
    }

}
void Objectmanager::show_coordinates(QString ob_name,int id_object_for_coord,QString table_name,QString id_name){

    clear_tableWidget(UI->coord_table);
    get_coordinates(id_object_for_coord,ob_name,table_name,id_name);

    int row_count = object_map.count();
    int col_count = object_map["0"].count();
    UI->coord_table->setColumnCount(col_count+1);
    UI->coord_table->hideColumn(1);


    QList<QString> header_list;
    header_list.append(tr(""));
    header_list.append(tr("id_coord"));
    header_list.append(tr("N ")+QChar(176));
    header_list.append(tr("N ") + "'");
    header_list.append(tr("N ")+ "\"");
    header_list.append(tr("E ")+QChar(176));
    header_list.append(tr("E ") + "'");
    header_list.append(tr("E ")+ "\"");

    header_list.append(tr("X"));
    header_list.append(tr("Y"));


    UI->coord_table->setHorizontalHeaderLabels(header_list);

    for(int row=0;row<row_count;row++)
    {
        UI->coord_table->insertRow(row);

        item = new QTableWidgetItem();
        item->setData(Qt::CheckStateRole, Qt::Unchecked);
        UI->coord_table->setItem(row,0,item);

        item = new QTableWidgetItem(object_map[QString::number(row)]["id_coordinates"]);
        UI->coord_table->setItem(row,1,item);

        item = new QTableWidgetItem(object_map[QString::number(row)]["lat_wgs_g"]);
        UI->coord_table->setItem(row,2,item);

        item = new QTableWidgetItem(object_map[QString::number(row)]["lat_wgs_m"]);
        UI->coord_table->setItem(row,3,item);

        item = new QTableWidgetItem(object_map[QString::number(row)]["lat_wgs_s"]);
        UI->coord_table->setItem(row,4,item);

        item = new QTableWidgetItem(object_map[QString::number(row)]["long_wgs_g"]);
        UI->coord_table->setItem(row,5,item);

        item = new QTableWidgetItem(object_map[QString::number(row)]["long_wgs_m"]);
        UI->coord_table->setItem(row,6,item);

        item = new QTableWidgetItem(object_map[QString::number(row)]["long_wgs_s"]);
        UI->coord_table->setItem(row,7,item);

        item = new QTableWidgetItem(object_map[QString::number(row)]["x"]);
        UI->coord_table->setItem(row,8,item);

        item = new QTableWidgetItem(object_map[QString::number(row)]["y"]);
        UI->coord_table->setItem(row,9,item);


        change_coord_system(0);
    }
}

//========== Изменение отображения системы координат =========
void Objectmanager::change_coord_system(int)
{
    for(int i=2;i<UI->coord_table->columnCount();i++)
    {
        UI->coord_table->showColumn(i);
    }

    switch(UI->coord_system_comboBox->currentIndex())
    {
    case 1:
        for(int i=8;i<=9;i++) UI->coord_table->hideColumn(i);
        break;
    case 2:
        for(int i=2;i<=7;i++) UI->coord_table->hideColumn(i);
        break;
    }
    UI->coord_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    UI->coord_table->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Fixed);
    UI->coord_table->setColumnWidth(0,20);
}
//======== Получение координат и сохранение их в ассоц. массиве opp_map<QString,QMap<QString,QString>> ==============
void Objectmanager::get_coordinates(int id_object_coord, QString ob_name,QString table_name,QString id_name)
{
    object_map.clear();

    QSqlQuery coord_query;

    QString str = QString("SELECT coord.id_coordinates, coord.latitude_wgs_84_g,coord.latitude_wgs_84_m,coord.latitude_wgs_84_s,coord.longitude_wgs_84_g, coord.longitude_wgs_84_m,coord.longitude_wgs_84_s,coord.x_coordinates,coord.y_coordinates FROM coordinates coord, %2 cls WHERE cls.%3=%1 AND coord.id_coordinates = cls.id_coordinates  order by coord.id_coordinates").arg(id_object_coord).arg(table_name).arg(id_name);

    if(!coord_query.exec(str))
    {
        QString sss = coord_query.lastError().text();
        return;
    }

    QSqlRecord rec = coord_query.record();

    int count = rec.count();
    int i = 0;
    while(coord_query.next())
    {
        coord_line.clear();

        coord_line["id_coordinates"] = coord_query.value(rec.indexOf("id_coordinates")).toString();

        coord_line["lat_wgs_g"] = coord_query.value(rec.indexOf("latitude_wgs_84_g")).toString();
        coord_line["lat_wgs_m"] = coord_query.value(rec.indexOf("latitude_wgs_84_m")).toString();
        coord_line["lat_wgs_s"] = coord_query.value(rec.indexOf("latitude_wgs_84_s")).toString();
        coord_line["long_wgs_g"] = coord_query.value(rec.indexOf("longitude_wgs_84_g")).toString();
        coord_line["long_wgs_m"] = coord_query.value(rec.indexOf("longitude_wgs_84_m")).toString();
        coord_line["long_wgs_s"] = coord_query.value(rec.indexOf("longitude_wgs_84_s")).toString();

        coord_line["x"] = coord_query.value(rec.indexOf("x_coordinates")).toString();
        coord_line["y"] = coord_query.value(rec.indexOf("y_coordinates")).toString();

        //coord_line["is_center"] = coord_query.value(rec.indexOf("is_object_center")).toString();

        object_map.insert(QString::number(i),coord_line);

        i++;
    }
}

//========== Функция очищения таблицы (удаление всех строк и столбцов) ===============
void Objectmanager::clear_tableWidget(QTableWidget *table)
{
    int count_rows = table->rowCount();
    while(count_rows >= 0)
    {
        table->removeRow(count_rows);
        count_rows--;
    }

    int count_cols = table->columnCount();
    while(count_cols >= 0)
    {
        table->removeColumn(count_cols);
        count_cols--;
    }
}
//========== Удаление координат ============
void Objectmanager::delete_coordinates()
{
    QModelIndex index = UI->columnView->currentIndex();
    if(!index.data(Qt::UserRole).toBool()) return;
    QString id_sc=index.data(Qt::UserRole).toString();
    QStringList list=id_sc.split("_");
    int id_obj = list.value(1).toInt();

    int row_count = UI->coord_table->rowCount();
    bool fl;
    int f = 0;


    for(int i=0;i<row_count;i++)
    {
        f = f + UI->coord_table->item(i,0)->data(Qt::CheckStateRole).toInt();
    }

    if(list.value(0)=="nations" || list.value(0)=="nationss" || list.value(0)=="nat" || list.value(0)=="dsmi" || list.value(0)=="ran" || list.value(0)=="rankss"
            || list.value(0)=="sexss" || list.value(0)=="sex" || list.value(0)=="se" || list.value(0)=="ag" || list.value(0)=="agess" || list.value(0)=="prof"
            || list.value(0)=="profess" || list.value(0)=="confess" || list.value(0)=="confesss" || list.value(0)=="conf" || list.value(0)=="psmi"
            || list.value(0)=="smi" || list.value(0)=="ls" || list.value(0)=="gr" || list.value(0)=="mpo" || list.value(0)=="perssmi" || list.value(0)=="persls"){

        QMessageBox::StandardButton ret;
        ret = QMessageBox::critical (this,"Ошибка",("Нет возможности  удалить координаты "),QMessageBox::Ok );

    }
    else
        if(f > 0)
        {
            //================MessageBox===============================
            QMessageBox msgBox;
            msgBox.setWindowTitle("Внимание");
            msgBox.setText(tr("Вы действительно хотите удалить координаты?"));
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            msgBox.setButtonText(QMessageBox::Yes, "Да");
            msgBox.setButtonText(QMessageBox::No, "Нет");
            switch (msgBox.exec()) {
            case QMessageBox::Yes:
                // yes was clicked
                break;
            case QMessageBox::No:
                return;
                break;
            default:
                return;
                break;
            }
        }
        else
        {
            //================MessageBox===============================
            QMessageBox msgBox;
            msgBox.setWindowTitle(tr("Сообщение"));
            msgBox.setText(tr("Ни одна строка не выбрана!"));
            msgBox.setStandardButtons(QMessageBox::Ok);
            switch (msgBox.exec()) {
            case QMessageBox::Yes:
                // yes was clicked
                return;
                break;
            }
        }
    //=========================================================

    QSqlQuery query;

    int id;
    QString str;
    for(int i=0;i<row_count;i++)
    {
        fl = UI->coord_table->item(i,0)->data(Qt::CheckStateRole).toBool();
        if(fl == true)
        {
            id = (UI->coord_table->item(i,1)->text()).toInt();
            str = QString("DELETE FROM coordinates WHERE id_coordinates = %1").arg(id);
            if(!query.exec(str))
            {
                QString sss = query.lastError().text();
                int a = 0;
                return;
            }
        }
    }

    if(list.value(0)=="dls" || list.value(0)=="chls" || list.value(0)=="lss" ){
        show_coordinates(list.value(0),list.value(1).toInt(),"coord_ls","id_ls");
    }
    else if(list.value(0)=="dgr"){
        show_coordinates(list.value(0),list.value(1).toInt(),"coord_groups","id_groups");
    }
    else if(list.value(0)=="region" || list.value(0)=="reg"){
        show_coordinates(list.value(0),list.value(1).toInt(),"coord_region","id_region");
    }
    else if(list.value(0)=="dsc"){
        show_coordinates(list.value(0),list.value(1).toInt(),"coord_spec_cond","id_special_conditions");
    }
    else if(list.value(0)=="dmpo" || list.value(0)=="dmpos" || list.value(0)=="dmposmi" ){
        show_coordinates(list.value(0),list.value(1).toInt(),"coord_mpo_pso","id_mpo_pso");
    }
    else if(list.value(0)=="dpers" || list.value(0)=="dperssmi" || list.value(0)=="dpersls" ){
        show_coordinates(list.value(0),list.value(1).toInt(),"coord_persones","id_persones");
    }
    return;
}

void Objectmanager::edit_coordinates_view()
{

    QModelIndex index = UI->columnView->currentIndex();
    if(!index.data(Qt::UserRole).toBool()) return;
    QString id_sc=index.data(Qt::UserRole).toString();
    QStringList list=id_sc.split("_");
    int id_obj = list.value(1).toInt();

    int row_count = UI->coord_table->rowCount();
    bool fl;
    int f = 0;
    for(int i=0;i<row_count;i++)
    {
        f = f + UI->coord_table->item(i,0)->data(Qt::CheckStateRole).toInt();

    }

    if(list.value(0)=="nations" || list.value(0)=="nationss" || list.value(0)=="nat" || list.value(0)=="dsmi" || list.value(0)=="ran" || list.value(0)=="rankss"
            || list.value(0)=="sexss" || list.value(0)=="sex" || list.value(0)=="se" || list.value(0)=="ag" || list.value(0)=="agess" || list.value(0)=="prof"
            || list.value(0)=="profess" || list.value(0)=="confess" || list.value(0)=="confesss" || list.value(0)=="conf" || list.value(0)=="psmi"
            || list.value(0)=="smi" || list.value(0)=="ls" || list.value(0)=="gr" || list.value(0)=="mpo" || list.value(0)=="perssmi" || list.value(0)=="persls"){

        QMessageBox::StandardButton ret;
        ret = QMessageBox::critical (this,"Ошибка",("Нет возможности  редактировать координаты "),QMessageBox::Ok );

    }else
        if(f > 0)
        {
            //================MessageBox===============================
            QMessageBox msgBox;
            msgBox.setWindowTitle("Внимание");
            msgBox.setText(tr("Вы действительно хотите редактировать координаты?"));
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            msgBox.setButtonText(QMessageBox::Yes, "Да");
            msgBox.setButtonText(QMessageBox::No, "Нет");
            switch (msgBox.exec()) {
            case QMessageBox::Yes:
                if(list.value(0)=="dls" || list.value(0)=="chls" || list.value(0)=="lss" ){
                    edit_coordinates(list.value(0),list.value(1).toInt(),"coord_ls","id_ls");
                }
                else if(list.value(0)=="dgr"){
                    edit_coordinates(list.value(0),list.value(1).toInt(),"coord_groups","id_groups");
                }
                else if(list.value(0)=="region" || list.value(0)=="reg"){
                    edit_coordinates(list.value(0),list.value(1).toInt(),"coord_region","id_region");
                }
                else if(list.value(0)=="dsc"){
                    edit_coordinates(list.value(0),list.value(1).toInt(),"coord_spec_cond","id_special_conditions");
                }
                else if(list.value(0)=="dmpo" || list.value(0)=="dmpos" || list.value(0)=="dmposmi" ){
                    edit_coordinates(list.value(0),list.value(1).toInt(),"coord_mpo_pso","id_mpo_pso");
                }
                else if(list.value(0)=="dpers" || list.value(0)=="dperssmi" || list.value(0)=="dpersls" ){
                    edit_coordinates(list.value(0),list.value(1).toInt(),"coord_persones","id_persones");
                }
                break;
            case QMessageBox::No:
                return;
                break;
            default:
                return;
                break;
            }
        }
        else
        {
            //================MessageBox===============================
            QMessageBox msgBox;
            msgBox.setWindowTitle(tr("Сообщение"));
            msgBox.setText(tr("Ни одна строка не выбрана!"));
            msgBox.setStandardButtons(QMessageBox::Ok);
            switch (msgBox.exec()) {
            case QMessageBox::Yes:
                // yes was clicked
                return;
                break;
            }
        }
}

void Objectmanager::edit_coordinates(QString ob_name, int id_obj, QString table_name, QString id_name)
{
    add_coord = new QDialog();

    add_coord->setWindowTitle(tr("Добавить новые координаты"));

    QLabel *lab_wgs = new QLabel("<b>" + tr("WGS-84") + "</b>");
    lab_wgs->setAlignment(Qt::AlignCenter);

    QLabel *lab1 = new QLabel(tr("N ")+QChar(176)+" :");
    QLabel *lab2 = new QLabel(tr("N ") + "' :");
    QLabel *lab3 = new QLabel(tr("N ") + "\" :");
    QLabel *lab4 = new QLabel(tr("E ")+QChar(176)+" :");
    QLabel *lab5 = new QLabel(tr("E ") + "' :");
    QLabel *lab6 = new QLabel(tr("E ") + "\" :");

    QLabel *lab_rect = new QLabel("<b>" + tr("Прямоугольные") + "</b>");
    lab_rect->setAlignment(Qt::AlignCenter);

    QLabel *lab15 = new QLabel(tr("X ") + ":");
    QLabel *lab16 = new QLabel(tr("Y ") + ":");

    e1 = new QLineEdit();
    e2 = new QLineEdit();
    e3 = new QLineEdit();
    e4 = new QLineEdit();
    e5 = new QLineEdit();
    e6 = new QLineEdit();
    e15 = new QLineEdit();
    e16 = new QLineEdit();

    QPushButton *wgs_button = new QPushButton(tr("Перевести"));
    QPushButton *plane_button = new QPushButton(tr("Перевести"));
    connect(wgs_button,SIGNAL(clicked()),this,SLOT(WGS_to_other()));
    connect(plane_button,SIGNAL(clicked()),this,SLOT(PLANE_to_other()));

    QGridLayout *grid = new QGridLayout();

    grid->addWidget(lab_wgs,0,0,1,2);

    grid->addWidget(lab1,1,0);
    grid->addWidget(e1,1,1);
    grid->addWidget(lab2,2,0);
    grid->addWidget(e2,2,1);
    grid->addWidget(lab3,3,0);
    grid->addWidget(e3,3,1);
    grid->addWidget(lab4,4,0);
    grid->addWidget(e4,4,1);
    grid->addWidget(lab5,5,0);
    grid->addWidget(e5,5,1);
    grid->addWidget(lab6,6,0);
    grid->addWidget(e6,6,1);

    grid->addWidget(wgs_button,7,1);

    grid->addWidget(lab_rect,0,3,1,2);
    grid->addWidget(lab15,1,3);
    grid->addWidget(e15,1,4);
    grid->addWidget(lab16,2,3);
    grid->addWidget(e16,2,4);
    grid->addWidget(plane_button,3,4);


    QPushButton *ok_button = new QPushButton("Сохранить");
    connect(ok_button,SIGNAL(clicked()),add_coord,SLOT(accept()));
    QPushButton *cancel_button = new QPushButton("Отмена");
    connect(cancel_button,SIGNAL(clicked()),add_coord,SLOT(close()));

    QHBoxLayout *b_lay = new QHBoxLayout;
    b_lay->addStretch();
    b_lay->addWidget(ok_button);
    b_lay->addWidget(cancel_button);

    QVBoxLayout *main_layout = new QVBoxLayout;
    main_layout->addLayout(grid);
    main_layout->addLayout(b_lay);

    add_coord->setLayout(main_layout);

    //int id_coord = UI->coord_table->item(row,1)->text().toInt();
    int gr,m,m_,gr_,id_c,id;
    double s,s_,x_,y_;
    bool fl;
    int row_count = UI->coord_table->rowCount();
    for(int i=0;i<row_count;i++)
    {
        fl = UI->coord_table->item(i,0)->data(Qt::CheckStateRole).toBool();
        //if((fl == true) && (!UI->opp_coord_table->item(i,1))) continue;

        if(fl == true)
        {
            id = (UI->coord_table->item(i,1)->text()).toInt();
        }
    }

    QSqlQuery query;

    QString str = QString("SELECT * FROM coordinates where id_coordinates=%1").arg(id);
    if(!query.exec(str))
    {
        return;
    }

    QSqlRecord rec = query.record();
    while(query.next())
    {
        e1->setText(query.value(rec.indexOf("latitude_wgs_84_g")).toString());
        e2->setText(query.value(rec.indexOf("latitude_wgs_84_m")).toString());
        e3->setText(query.value(rec.indexOf("latitude_wgs_84_s")).toString());
        e4->setText(query.value(rec.indexOf("longitude_wgs_84_g")).toString());
        e5->setText(query.value(rec.indexOf("longitude_wgs_84_m")).toString());
        e6->setText(query.value(rec.indexOf("longitude_wgs_84_s")).toString());
        e15->setText(query.value(rec.indexOf("x_coordinates")).toString());
        e16->setText(query.value(rec.indexOf("y_coordinates")).toString());
    }

    if(add_coord->exec() == QDialog::Accepted)
    {

        QSqlQuery query;
        QString str = QString("UPDATE coordinates SET latitude_wgs_84_g='%1',latitude_wgs_84_m='%2',latitude_wgs_84_s='%3',longitude_wgs_84_g='%4',longitude_wgs_84_m='%5',longitude_wgs_84_s='%6',x_coordinates='%7',y_coordinates='%8' \
                              WHERE id_coordinates=%9") \
                .arg(e1->text().toInt()).arg(e2->text().toInt()).arg(e3->text().toDouble()) \
                .arg(e4->text().toInt()).arg(e5->text().toInt()).arg(e6->text().toDouble()) \
                .arg(e15->text().toDouble()).arg(e16->text().toDouble()).arg(id);

                if(!query.exec(str))
        {
                QString s = query.lastError().text();
        return;
    }

    show_coordinates(ob_name,id_obj,table_name,id_name);
}
return;
}
//=============== Автоперевод систем координат ====================
void Objectmanager::WGS_to_other()
{
    QSettings *settings = new QSettings("vka","saturnMap");
    QString mapPath = settings->value("/mapSettings/mapPath","").toString();

    if(mapPath.isEmpty())
    {
        //================MessageBox===============================
        QMessageBox msgBox;
        msgBox.setWindowTitle("Сообщение");
        msgBox.setText("Вы должны открыть карту, чтобы перевести координаты объекта\nОткрыть карту?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setButtonText(QMessageBox::Yes, "Да");
        msgBox.setButtonText(QMessageBox::No, "Нет");

        switch (msgBox.exec()) {
        case QMessageBox::Yes:
            mapPath = QFileDialog::getOpenFileName(this, NULL, NULL,"Maps (*.map)" );
            break;
        case QMessageBox::No:
            return;
            break;
        default:
            return;
            break;
        }
        //==============================================================
    }
    if(mapPath.isEmpty()) return;//если карта не выбрана

    MyMapAccess *map = new MyMapAccess();
    hmap = 0;
    hmap = map->mapOpen(mapPath.toStdString().c_str(),0);
    if(hmap == 0) return;

    if(map->mapIsGeoSupported(hmap))
    {
        GEODEGREE N, E;
        double N_rad, E_rad, H;

        N.Degree = e1->text().toLong();
        N.Minute = e2->text().toLong();
        N.Second = e3->text().toDouble();

        map->mapDegreeToRadian(&N, &N_rad);

        E.Degree = e4->text().toLong();
        E.Minute = e5->text().toLong();
        E.Second = e6->text().toDouble();

        map->mapDegreeToRadian(&E, &E_rad);

        //----- Перезаписываем введенные координаты в поля Edit для WGS-84------------
        e1->setText(QString::number(N.Degree));
        e2->setText(QString::number(N.Minute));
        e3->setText(QString::number(N.Second,'f',2));
        e4->setText(QString::number(E.Degree));
        e5->setText(QString::number(E.Minute));
        e6->setText(QString::number(E.Second,'f',2));
        //-----------------
        map->mapGeoWGS84ToPlane3D(hmap,&N_rad,&E_rad,&H);

        e15->setText(QString::number(N_rad,'f',2));
        e16->setText(QString::number(E_rad,'f',2));

        map->mapPlaneToGeo423D(hmap,&N_rad,&E_rad,&H);
        map->mapRadianToDegree(&N_rad, &N);
        map->mapRadianToDegree(&E_rad, &E);
    }
    if(hmap)
    {
        map->mapCloseData(hmap);
    }

    add_coord->raise();
}

void Objectmanager::PLANE_to_other()
{
    QSettings *settings = new QSettings("vka","saturnMap");
    QString mapPath = settings->value("/mapSettings/mapPath","").toString();

    if(mapPath.isEmpty())
    {
        //================MessageBox===============================
        QMessageBox msgBox;
        msgBox.setWindowTitle("Сообщение");
        msgBox.setText("Вы должны открыть карту, чтобы перевести координаты объекта\nОткрыть карту??");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setButtonText(QMessageBox::Yes, "Да");
        msgBox.setButtonText(QMessageBox::No, "Нет");

        switch (msgBox.exec()) {
        case QMessageBox::Yes:
            mapPath = QFileDialog::getOpenFileName(this, NULL, NULL,"Maps (*.map)" );
            break;
        case QMessageBox::No:
            return;
            break;
        default:
            return;
            break;
        }
        //==============================================================
    }
    if(mapPath.isEmpty()) return;//если карта не выбрана

    MyMapAccess *map = new MyMapAccess();
    hmap = 0;
    hmap = map->mapOpen(mapPath.toStdString().c_str(),0);
    if(hmap == 0)
	{
		 //================MessageBox===============================
        QMessageBox msgBox;
        msgBox.setWindowTitle("Сообщение");
        msgBox.setText("hmap == 0");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setButtonText(QMessageBox::Yes, "Да");

        switch (msgBox.exec()) {
        case QMessageBox::Yes:
            break;
        }
        //==============================================================
		return;
	}

    if(map->mapIsGeoSupported(hmap))
    {
        GEODEGREE N, E;
        double N_rad, E_rad, H;

        N_rad = e15->text().toDouble();
        E_rad = e16->text().toDouble();

        e15->setText(QString::number(N_rad,'f',2));
        e16->setText(QString::number(E_rad,'f',2));

        map->mapPlaneToGeoWGS843D(hmap,&N_rad,&E_rad,&H);
        map->mapRadianToDegree(&N_rad,&N);
        map->mapRadianToDegree(&E_rad,&E);

        e1->setText(QString::number(N.Degree));
        e2->setText(QString::number(N.Minute));
        e3->setText(QString::number(N.Second,'f',2));
        e4->setText(QString::number(E.Degree));
        e5->setText(QString::number(E.Minute));
        e6->setText(QString::number(E.Second,'f',2));

    }
	else
	{
				 //================MessageBox===============================
        QMessageBox msgBox;
        msgBox.setWindowTitle("Сообщение");
        msgBox.setText("Map is not geosupported");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setButtonText(QMessageBox::Yes, "Да");

        switch (msgBox.exec()) {
        case QMessageBox::Yes:
            break;
        }
        //==============================================================
	}

    if(hmap)
    {
        map->mapCloseData(hmap);
    }

    add_coord->raise();
}

//======================================================================================
void Objectmanager::fill_combobox_persones_(QComboBox *Box,int current_index)
{
    QSqlQuery query;
    query.exec("SELECT id_type_persones, name_type_persones FROM type_persones");
    int ci_3=0;
    while (query.next())
    {
        QString blok = query.value(1).toString();
        int id_blok=query.value(0).toInt();
        Box->addItem(blok,id_blok);
        if (current_index==id_blok) ci_3=Box->count()-1;
    }

    Box->setCurrentIndex(ci_3);
}
//==========================================================================
//====== Слот поиска объектов по введенной строке в поле ввода =============
//====== Результаты поиска отображаются в диалоговом окне ==================
//==========================================================================
void Objectmanager::slotSearchObject()
{
    int iCurrSearch = 0;
    QModelIndexList indexes = UI->columnView->model()->match(UI->columnView->model()->index(0, 0), Qt::DisplayRole, QVariant( UI->searchLineEdit->text() ), -1, Qt::MatchFlags(Qt::MatchRecursive | Qt::MatchExactly | Qt::MatchFixedString | Qt::MatchWrap | Qt::MatchStartsWith));

    if(indexes.isEmpty() ) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Результат поиска");
        msgBox.setText("Указанный объект не найден");

        msgBox.setStandardButtons(QMessageBox::Yes);
        msgBox.setButtonText(QMessageBox::Yes, "Да");
        switch (msgBox.exec()) {
       case QMessageBox::Yes:
            return;
         break;
        }
    }

    UI->columnView->setCurrentIndex( indexes.at(iCurrSearch));
}
void Objectmanager::update_one_click(const QModelIndex &index){
    UI->property_object->setModel(0);
    clear_tableWidget(UI->coord_table);
    QVariant id=index.data(Qt::UserRole);
    if (id.type() == QVariant::String)
    {
        QString user_data=id.toString();
        QStringList list=user_data.split("_");
        if(list.value(0)=="region"){
        region_click(list.value(1).toInt());
        show_coordinates(list.value(0),list.value(1).toInt(),"coord_region","id_region");
        UI->add_many_coord_button->setEnabled(true);
    }
        else if(list.value(0)=="reg"){
        region_click(list.value(1).toInt());
        show_coordinates(list.value(0),list.value(1).toInt(),"coord_region","id_region");
        UI->add_many_coord_button->setEnabled(true);
    } // ================= в таблицу данные о СМИ =======================================
        else if(list.value(0)=="dsmi"){
        smi_click(list.value(2).toInt());
    } // ================= в таблицу данные о ВФ =======================
        else if(list.value(0)=="dls"){
        ls_click(list.value(1).toInt());
        show_coordinates(list.value(0),list.value(1).toInt(),"coord_ls","id_ls");
    } // ================= в таблицу данные о Организациях =======================
        else if(list.value(0)=="dgr"){
        gr_click(list.value(1).toInt());
//            show_coordinates(list.value(0),list.value(1).toInt(),"coord_groups","id_groups");

    }	// ================= в таблицу данные о Условиях =======================
        else if(list.value(0)=="dsc"){
        sc_click(list.value(1).toInt());
        show_coordinates(list.value(0),list.value(1).toInt(),"coord_spec_cond","id_special_conditions");
    }//======================= в таблицу воинские формирования (подчиненные)======
        else if(list.value(0)=="lss"){
        ls_click(list.value(1).toInt());
        show_coordinates(list.value(0),list.value(1).toInt(),"coord_ls","id_ls");
    }
        else if(list.value(0)=="chls"){
        ls_click(list.value(1).toInt());
        show_coordinates(list.value(0),list.value(1).toInt(),"coord_ls","id_ls");
    } // ================= в таблицу данные о СРЕДСТВАХ =======================
        else if(list.value(0)=="dmpo" || list.value(0)=="dmpos" || list.value(0)=="dmposmi" ){
        mpo_click(list.value(1).toInt());
        show_coordinates(list.value(0),list.value(1).toInt(),"coord_mpo_pso","id_mpo_pso");
    }
        else if(list.value(0)=="dpers" || list.value(0)=="dperssmi" || list.value(0)=="dpersls" ){
        show_coordinates(list.value(0),list.value(1).toInt(),"coord_persones","id_persones");
    }
   }
}
