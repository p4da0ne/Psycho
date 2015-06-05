#include <QtGui>
#include "add_elements_dialog.h"
#include "mylabel.h"
#include "reports.h"

Add_elements_dialog::Add_elements_dialog(int type_element,int id_object,QWidget *parent)
	: QDialog(parent)
{
	in_id_object=id_object;
	switch(type_element)
	{
	case 1:// ============ СМИ ================================================
	{
	QMessageBox msgBox;
	msgBox.setWindowTitle("Предупреждение");
    msgBox.setWindowIcon(QIcon("./icons/chast.ico"));
	msgBox.setText("Выбрать СМИ из имеющихся в базе?"); 
	
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
	msgBox.setButtonText(QMessageBox::Yes, "Да");
	msgBox.setButtonText(QMessageBox::No, "Нет");
		switch (msgBox.exec()){ 
		  case QMessageBox::Yes:{
			 label_16 = new QLabel("Наименование СМИ:");
			 comboBox_10 = new QComboBox();
			 comboBox_10->setFixedWidth(200);
			 label_16->setBuddy(comboBox_10);
			 QStringList column_9;
			 column_9.clear();
			 column_9<<"name_smi";
			 fill_combobox(comboBox_10,"smi",column_9);
			 
			 addButton = new QPushButton("Добавить");
			 addButton->setDefault(true);
			 connect(addButton,SIGNAL(clicked()),this,SLOT(add_name_smi()));
			 cancelButton = new QPushButton("Отмена");
			 connect(cancelButton,SIGNAL(clicked()),this,SLOT(close()));

			 QHBoxLayout *buttonsLayout = new QHBoxLayout;
			 buttonsLayout->addStretch();
			 buttonsLayout->addWidget(addButton);
			 buttonsLayout->addWidget(cancelButton);

			 QHBoxLayout *topLeftLayout_29 = new QHBoxLayout;
			 topLeftLayout_29->addWidget(label_16);
			 QHBoxLayout *topLeftLayout_30 = new QHBoxLayout;
			 topLeftLayout_30->addWidget(comboBox_10);

			 QVBoxLayout *leftLayout_31 = new QVBoxLayout;
			 leftLayout_31->addLayout(topLeftLayout_29);
			 QVBoxLayout *leftLayout_32 = new QVBoxLayout;
			 leftLayout_32->addLayout(topLeftLayout_30);

			 QGridLayout *mainLayout = new QGridLayout;
			 mainLayout->setSizeConstraint(QLayout::SetFixedSize);
			 mainLayout->addLayout(leftLayout_31, 0, 0);
			 mainLayout->addLayout(leftLayout_32, 0, 1);
			 mainLayout->addLayout(buttonsLayout, 1, 0, 1, 2);
			 setLayout(mainLayout);
			 setWindowTitle("Добавить СМИ");
             setWindowIcon(QIcon(":/Resources/book.png"));
			 break;
			 }  
	 case QMessageBox::No:{
			 label = new QLabel("Наименование СМИ:");
			 lineEdit_name = new QLineEdit;
			 label->setBuddy(lineEdit_name);
			 
			 label_2 = new QLabel("Тип СМИ:");
			 comboBox = new QComboBox();
			 label_2->setBuddy(comboBox);
			 QStringList column;
			 column.clear();
			 column<<"nametype_smi";
			 fill_combobox(comboBox,"type_smi",column);
		//	 connect(comboBox,SIGNAL(currentIndexChanged ( int )),this,SLOT(software_combobox_index_changed ( int )));
			 label_3 = new QLabel("Представительство СМИ:");
			 comboBox_2 = new QComboBox();
			 label_3->setBuddy(comboBox_2);
			 QStringList column_1;
			 column_1.clear();
			 column_1<<"name_type_office_smi";
			 fill_combobox(comboBox_2,"type_office_smi",column_1);
			 label_4 = new QLabel("Способ вещания:");
			 comboBox_3 = new QComboBox();
			 label_3->setBuddy(comboBox_3);
			 QStringList column_2;
			 column_2.clear();
			 column_2<<"name_type_broadcast_smi";
			 fill_combobox(comboBox_3,"type_broadcast_smi",column_2);
			 label_5 = new QLabel("Направленность СМИ:");
			 comboBox_4 = new QComboBox();
			 label_5->setBuddy(comboBox_4);
			 QStringList column_3;
			 column_3.clear();
			 column_3<<"name_position_smi";
			 fill_combobox(comboBox_4,"position_smi",column_3);
			 label_6 = new QLabel("Уровень вещания:");
			 comboBox_5 = new QComboBox();
			 label_6->setBuddy(comboBox_5);
			 QStringList column_4;
			 column_4.clear();
			 column_4<<"name_level_smi";
			 fill_combobox(comboBox_5,"level_smi",column_4);
			 label_7 = new QLabel("Тематика СМИ:");
			 comboBox_6 = new QComboBox();
			 label_7->setBuddy(comboBox_6);
			 QStringList column_5;
			 column_5.clear();
			 column_5<<"name_theme_smi";
			 fill_combobox(comboBox_6,"theme_smi",column_5);
			 			
			 label_8 = new QLabel("Аудитория СМИ:");
			 lineEdit_aud = new QLineEdit;

			 label_8->setBuddy(lineEdit_aud);
			 label_9 = new QLabel("Тираж СМИ:");
			 lineEdit_tirag = new QLineEdit;
			 label_9->setBuddy(lineEdit_tirag);
			 label_11 = new QLabel("Мощность сигнала:");
			 lineEdit_broad = new QLineEdit;
			 label_11->setBuddy(lineEdit_broad);
			 label_12 = new QLabel("Сайт СМИ:");
			 lineEdit_site = new QLineEdit;
			 label_12->setBuddy(lineEdit_site);
			 label_13 = new QLabel("Описание СМИ:");
			 textEdit_desc = new QTextEdit;
			 textEdit_desc->setFixedHeight(50);
			 label_13->setBuddy(textEdit_desc);
			 label_14 = new QLabel("Частоты вещания:");
			 lineEdit_freq = new QLineEdit;
			 label_14->setBuddy(lineEdit_freq);
			 			 
			 addButton = new QPushButton("Добавить");
			 addButton->setDefault(true);
			 connect(addButton,SIGNAL(clicked()),this,SLOT(add_smi()));
			 cancelButton = new QPushButton("Отмена");
			 connect(cancelButton,SIGNAL(clicked()),this,SLOT(close()));

			 QHBoxLayout *buttonsLayout = new QHBoxLayout;
			 buttonsLayout->addStretch();
			 buttonsLayout->addWidget(addButton);
			 buttonsLayout->addWidget(cancelButton);

			 QHBoxLayout *topLeftLayout = new QHBoxLayout;
			 topLeftLayout->addWidget(label);
			 QHBoxLayout *topLeftLayout_2 = new QHBoxLayout;
			 topLeftLayout_2->addWidget(lineEdit_name);
			 
			 QHBoxLayout *centrLeftLayout = new QHBoxLayout;
			 centrLeftLayout->addWidget(label_2);
			 QHBoxLayout *centrLeftLayout_2 = new QHBoxLayout;
			 centrLeftLayout_2->addWidget(comboBox);
			 QHBoxLayout *topLeftLayout_3 = new QHBoxLayout;
			 topLeftLayout_3->addWidget(label_3);
			 QHBoxLayout *topLeftLayout_4 = new QHBoxLayout;
			 topLeftLayout_4->addWidget(comboBox_2);
			 QHBoxLayout *topLeftLayout_5 = new QHBoxLayout;
			 topLeftLayout_5->addWidget(label_4);
			 QHBoxLayout *topLeftLayout_6 = new QHBoxLayout;
			 topLeftLayout_6->addWidget(comboBox_3);
			 QHBoxLayout *topLeftLayout_7 = new QHBoxLayout;
			 topLeftLayout_7->addWidget(label_5);
			 QHBoxLayout *topLeftLayout_8 = new QHBoxLayout;
			 topLeftLayout_8->addWidget(comboBox_4);
			 QHBoxLayout *topLeftLayout_9 = new QHBoxLayout;
			 topLeftLayout_9->addWidget(label_6);
			 QHBoxLayout *topLeftLayout_10 = new QHBoxLayout;
			 topLeftLayout_10->addWidget(comboBox_5);
			 QHBoxLayout *topLeftLayout_11 = new QHBoxLayout;
			 topLeftLayout_11->addWidget(label_7);
			 QHBoxLayout *topLeftLayout_12 = new QHBoxLayout;
			 topLeftLayout_12->addWidget(comboBox_6);
			 QHBoxLayout *topLeftLayout_13 = new QHBoxLayout;
			 topLeftLayout_13->addWidget(label_8);
			 QHBoxLayout *topLeftLayout_14 = new QHBoxLayout;
			 topLeftLayout_14->addWidget(lineEdit_aud);
			 QHBoxLayout *topLeftLayout_15 = new QHBoxLayout;
			 topLeftLayout_15->addWidget(label_9);
			 QHBoxLayout *topLeftLayout_16 = new QHBoxLayout;
			 topLeftLayout_16->addWidget(lineEdit_tirag);
			 QHBoxLayout *topLeftLayout_19 = new QHBoxLayout;
			 topLeftLayout_19->addWidget(label_11);
			 QHBoxLayout *topLeftLayout_20 = new QHBoxLayout;
			 topLeftLayout_20->addWidget(lineEdit_broad);
			 QHBoxLayout *topLeftLayout_21 = new QHBoxLayout;
			 topLeftLayout_21->addWidget(label_12);
			 QHBoxLayout *topLeftLayout_22 = new QHBoxLayout;
			 topLeftLayout_22->addWidget(lineEdit_site);
			 QHBoxLayout *topLeftLayout_23 = new QHBoxLayout;
			 topLeftLayout_23->addWidget(label_13);
			 QHBoxLayout *topLeftLayout_24 = new QHBoxLayout;
			 topLeftLayout_24->addWidget(textEdit_desc);
			 QHBoxLayout *topLeftLayout_25 = new QHBoxLayout;
			 topLeftLayout_25->addWidget(label_14);
			 QHBoxLayout *topLeftLayout_26 = new QHBoxLayout;
			 topLeftLayout_26->addWidget(lineEdit_freq);
			 			 
			 QVBoxLayout *leftLayout = new QVBoxLayout;
			 leftLayout->addLayout(topLeftLayout);
			 QVBoxLayout *leftLayout_2 = new QVBoxLayout;
			 leftLayout_2->addLayout(topLeftLayout_2);
			 QVBoxLayout *leftLayout_3 = new QVBoxLayout;
			 leftLayout_3->addLayout(centrLeftLayout);
			 QVBoxLayout *leftLayout_4 = new QVBoxLayout;
			 leftLayout_4->addLayout(centrLeftLayout_2);
			 QVBoxLayout *leftLayout_5 = new QVBoxLayout;
			 leftLayout_5->addLayout(topLeftLayout_3);
			 QVBoxLayout *leftLayout_6 = new QVBoxLayout;
			 leftLayout_6->addLayout(topLeftLayout_4);
			 QVBoxLayout *leftLayout_7 = new QVBoxLayout;
			 leftLayout_7->addLayout(topLeftLayout_5);
			 QVBoxLayout *leftLayout_8 = new QVBoxLayout;
			 leftLayout_8->addLayout(topLeftLayout_6);
			 QVBoxLayout *leftLayout_9 = new QVBoxLayout;
			 leftLayout_9->addLayout(topLeftLayout_7);
			 QVBoxLayout *leftLayout_10 = new QVBoxLayout;
			 leftLayout_10->addLayout(topLeftLayout_8);
			 QVBoxLayout *leftLayout_11 = new QVBoxLayout;
			 leftLayout_11->addLayout(topLeftLayout_9);
			 QVBoxLayout *leftLayout_12 = new QVBoxLayout;
			 leftLayout_12->addLayout(topLeftLayout_10);
			 QVBoxLayout *leftLayout_13 = new QVBoxLayout;
			 leftLayout_13->addLayout(topLeftLayout_11);
			 QVBoxLayout *leftLayout_14 = new QVBoxLayout;
			 leftLayout_14->addLayout(topLeftLayout_12);
			 QVBoxLayout *leftLayout_15 = new QVBoxLayout;
			 leftLayout_15->addLayout(topLeftLayout_13);
			 QVBoxLayout *leftLayout_16 = new QVBoxLayout;
			 leftLayout_16->addLayout(topLeftLayout_14);
			 QVBoxLayout *leftLayout_17 = new QVBoxLayout;
			 leftLayout_17->addLayout(topLeftLayout_15);
			 QVBoxLayout *leftLayout_18 = new QVBoxLayout;
			 leftLayout_18->addLayout(topLeftLayout_16);
			 QVBoxLayout *leftLayout_21 = new QVBoxLayout;
			 leftLayout_21->addLayout(topLeftLayout_19);
			 QVBoxLayout *leftLayout_22 = new QVBoxLayout;
			 leftLayout_22->addLayout(topLeftLayout_20);
			 QVBoxLayout *leftLayout_23 = new QVBoxLayout;
			 leftLayout_23->addLayout(topLeftLayout_21);
			 QVBoxLayout *leftLayout_24 = new QVBoxLayout;
			 leftLayout_24->addLayout(topLeftLayout_22);
			 QVBoxLayout *leftLayout_25 = new QVBoxLayout;
			 leftLayout_25->addLayout(topLeftLayout_23);
			 QVBoxLayout *leftLayout_26 = new QVBoxLayout;
			 leftLayout_26->addLayout(topLeftLayout_24);
			 QVBoxLayout *leftLayout_27 = new QVBoxLayout;
			 leftLayout_27->addLayout(topLeftLayout_25);
			 QVBoxLayout *leftLayout_28 = new QVBoxLayout;
			 leftLayout_28->addLayout(topLeftLayout_26);
			 			 
			 leftLayout_28->addStretch(150);

			 QGridLayout *mainLayout = new QGridLayout;
			 mainLayout->setSizeConstraint(QLayout::SetFixedSize);
			 mainLayout->addLayout(leftLayout, 2, 0);
			 mainLayout->addLayout(leftLayout_2, 2, 1);
			 mainLayout->addLayout(leftLayout_3, 3, 0);
			 mainLayout->addLayout(leftLayout_4, 3, 1);
			 mainLayout->addLayout(leftLayout_5, 4, 0);
			 mainLayout->addLayout(leftLayout_6, 4, 1);
			 mainLayout->addLayout(leftLayout_7, 5, 0);
			 mainLayout->addLayout(leftLayout_8, 5, 1);
			 mainLayout->addLayout(leftLayout_9, 6, 0);
			 mainLayout->addLayout(leftLayout_10, 6, 1);
			 mainLayout->addLayout(leftLayout_11, 7, 0);
			 mainLayout->addLayout(leftLayout_12, 7, 1);
			 mainLayout->addLayout(leftLayout_13, 8, 0);
			 mainLayout->addLayout(leftLayout_14, 8, 1);
			 mainLayout->addLayout(leftLayout_21, 9, 0);
			 mainLayout->addLayout(leftLayout_22, 9, 1);
			 mainLayout->addLayout(leftLayout_23, 10, 0);
			 mainLayout->addLayout(leftLayout_24, 10, 1);
			 mainLayout->addLayout(leftLayout_25, 11, 0);
			 mainLayout->addLayout(leftLayout_26, 11, 1);
			 mainLayout->addLayout(leftLayout_27, 12, 0);
			 mainLayout->addLayout(leftLayout_28, 12, 1);
			 mainLayout->addLayout(leftLayout_15, 13, 0);
			 mainLayout->addLayout(leftLayout_16, 13, 1);
			 mainLayout->addLayout(leftLayout_17, 14, 0);
			 mainLayout->addLayout(leftLayout_18, 14, 1);

			 mainLayout->addLayout(buttonsLayout, 16, 0, 1, 2);
			 setLayout(mainLayout);
			 setWindowTitle("Добавить СМИ");
             setWindowIcon(QIcon(":/Resources/book.png"));
			break;
			}
		default:
		return;
		break;
	}
break;	
}
	case 2:{ //========== Воинские формирования ====================================
		 
		 label = new QLabel("Наименование подразделения:");
		 lineEdit_name = new QLineEdit;
		 label->setBuddy(lineEdit_name);
		 
		 label_2 = new QLabel("Тип подразделения:");
		 comboBox = new QComboBox();
		 label_2->setBuddy(comboBox);
		 QStringList column;
		 column.clear();
		 column<<"name_type_ls";
		 fill_combobox(comboBox,"type_ls",column);
		 label_3 = new QLabel("Общая численность личного состава:");
		 lineEdit_counte_ls = new QLineEdit;
		 label_3->setBuddy(lineEdit_counte_ls);
		 label_4 = new QLabel("Общая численность личного состава,\n участвующего в БД:");
		 lineEdit_counte_ls_bd = new QLineEdit;
		 label_4->setBuddy(lineEdit_counte_ls_bd);
		 label_5 = new QLabel("Враждебность подразделения:");
		 checkbox_enemy = new QCheckBox(" [-V- враждебное]", this);
		 label_5->setBuddy(checkbox_enemy);
		 
////	     QGroupBox *configGroup = new QGroupBox("Прямоугольные координаты объекта:");
////		 QLabel *x_label = new QLabel("X:");
////		 lineEdit_x = new QLineEdit();
////		 QLabel *y_label = new QLabel("Y:");
////		 lineEdit_y = new QLineEdit();
		
////	 	 QHBoxLayout *coord_Layout = new QHBoxLayout;
////		 coord_Layout->addWidget(x_label);
////		 coord_Layout->addWidget(lineEdit_x);
////		 coord_Layout->addWidget(y_label);
////		 coord_Layout->addWidget(lineEdit_y);
   
//		 QVBoxLayout *configLayout = new QVBoxLayout;
//		 configLayout->addLayout(coord_Layout);
//		 configGroup->setLayout(configLayout);

		 addButton = new QPushButton("Добавить");
		 addButton->setDefault(true);
		 connect(addButton,SIGNAL(clicked()),this,SLOT(add_ls()));
		 cancelButton = new QPushButton("Отмена");
		 connect(cancelButton,SIGNAL(clicked()),this,SLOT(close()));

		 QHBoxLayout *buttonsLayout = new QHBoxLayout;
		 buttonsLayout->addStretch();
		 buttonsLayout->addWidget(addButton);
		 buttonsLayout->addWidget(cancelButton);
		 
		 QHBoxLayout *topLeftLayout = new QHBoxLayout;
		 topLeftLayout->addWidget(label);
		 QHBoxLayout *topLeftLayout_2 = new QHBoxLayout;
		 topLeftLayout_2->addWidget(lineEdit_name);
		 QHBoxLayout *topLeftLayout_3 = new QHBoxLayout;
		 topLeftLayout_3->addWidget(label_2);
		 QHBoxLayout *topLeftLayout_4 = new QHBoxLayout;
		 topLeftLayout_4->addWidget(comboBox);
		 QHBoxLayout *topLeftLayout_5 = new QHBoxLayout;
		 topLeftLayout_5->addWidget(label_3);
		 QHBoxLayout *topLeftLayout_6 = new QHBoxLayout;
		 topLeftLayout_6->addWidget(lineEdit_counte_ls);
		 QHBoxLayout *topLeftLayout_7 = new QHBoxLayout;
		 topLeftLayout_7->addWidget(label_4);
		 QHBoxLayout *topLeftLayout_8 = new QHBoxLayout;
		 topLeftLayout_8->addWidget(lineEdit_counte_ls_bd);
		 QHBoxLayout *topLeftLayout_9 = new QHBoxLayout;
		 topLeftLayout_9->addWidget(label_5);
		 QHBoxLayout *topLeftLayout_10 = new QHBoxLayout;
		 topLeftLayout_10->addWidget(checkbox_enemy);


		 QVBoxLayout *leftLayout = new QVBoxLayout;
		 leftLayout->addLayout(topLeftLayout);
		 QVBoxLayout *leftLayout_2 = new QVBoxLayout;
		 leftLayout_2->addLayout(topLeftLayout_2);
		 QVBoxLayout *leftLayout_3 = new QVBoxLayout;
		 leftLayout_3->addLayout(topLeftLayout_3);
		 QVBoxLayout *leftLayout_4 = new QVBoxLayout;
		 leftLayout_4->addLayout(topLeftLayout_4);
		 QVBoxLayout *leftLayout_5 = new QVBoxLayout;
		 leftLayout_5->addLayout(topLeftLayout_5);
		 QVBoxLayout *leftLayout_6 = new QVBoxLayout;
		 leftLayout_6->addLayout(topLeftLayout_6);
		 QVBoxLayout *leftLayout_7 = new QVBoxLayout;
		 leftLayout_7->addLayout(topLeftLayout_7);
		 QVBoxLayout *leftLayout_8 = new QVBoxLayout;
		 leftLayout_8->addLayout(topLeftLayout_8);
		 QVBoxLayout *leftLayout_9 = new QVBoxLayout;
		 leftLayout_9->addLayout(topLeftLayout_9);
		 QVBoxLayout *leftLayout_10 = new QVBoxLayout;
		 leftLayout_10->addLayout(topLeftLayout_10);
		 leftLayout->addStretch(1);

		 QGridLayout *mainLayout = new QGridLayout;
		 mainLayout->setSizeConstraint(QLayout::SetFixedSize);
		 mainLayout->addLayout(leftLayout, 0, 0);
		 mainLayout->addLayout(leftLayout_2, 0, 1);
		 mainLayout->addLayout(leftLayout_3, 1, 0);
		 mainLayout->addLayout(leftLayout_4, 1, 1);
		 mainLayout->addLayout(leftLayout_5, 2, 0);
		 mainLayout->addLayout(leftLayout_6, 2, 1);
		 mainLayout->addLayout(leftLayout_7, 3, 0);
		 mainLayout->addLayout(leftLayout_8, 3, 1);
		 mainLayout->addLayout(leftLayout_9, 4, 0);
		 mainLayout->addLayout(leftLayout_10, 4, 1);

//		 mainLayout->addWidget(configGroup, 5, 0, 1, 2);
         mainLayout->addLayout(buttonsLayout, 5, 0, 1, 2);
		 setLayout(mainLayout);
		 setWindowTitle("Добавить воинское формирование");
         setWindowIcon(QIcon(":/Resources/weapon.png"));
		 break;
		}
	case 3://=============== Организации ============================================
		{
		 label = new QLabel("Наименование организации:");
		 lineEdit_name = new QLineEdit;
		 label->setBuddy(lineEdit_name);
		 
		 label_2 = new QLabel("Направленность");
		 comboBox = new QComboBox();
		 label_2->setBuddy(comboBox);
		 QStringList column;
		 column.clear();
		 column<<"name_trend_groups";
		 fill_combobox(comboBox,"trend_groups",column);
		 label_3 = new QLabel("Сфера деятельности:");
		 comboBox_2 = new QComboBox();
		 label_3->setBuddy(comboBox_2);
		 QStringList column_1;
		 column_1.clear();
		 column_1<<"name_sphere_groups";
		 fill_combobox(comboBox_2,"sphere_groups",column_1);
		 label_4 = new QLabel("Форма организации:");
		 comboBox_3 = new QComboBox();
		 label_4->setBuddy(comboBox_3);
		 QStringList column_2;
		 column_2.clear();
		 column_2<<"name_form_groups";
		 fill_combobox(comboBox_3,"form_groups",column_2);
		 label_5 = new QLabel("Информационные органы:");
		 textEdit_propa = new QTextEdit;
		 label_5->setBuddy(textEdit_propa);
		 textEdit_propa->setFixedHeight(50);
		 label_6 = new QLabel("Численность организации:");
		 lineEdit_counte = new QLineEdit;
		 label_6->setBuddy(lineEdit_counte);
		 label_7 = new QLabel("Учредители организации:");
		 textEdit_founder = new QTextEdit;
		 textEdit_founder->setFixedHeight(50);
		 label_7->setBuddy(textEdit_founder);
		 label_8 = new QLabel("Руководство организации:");
		 textEdit_menegm = new QTextEdit;
		 textEdit_menegm->setFixedHeight(50);
		 label_8->setBuddy(textEdit_menegm);
		 label_9 = new QLabel("Представительство организации:");
		 textEdit_office = new QTextEdit;
		 textEdit_office->setFixedHeight(50);
		 label_9->setBuddy(textEdit_office);
		 label_10 = new QLabel("Описание организации:");
		 textEdit_descr = new QTextEdit;
		 textEdit_descr->setFixedHeight(50);
		 label_10->setBuddy(textEdit_descr);

		 addButton = new QPushButton("Добавить");
		 addButton->setDefault(true);
		 connect(addButton,SIGNAL(clicked()),this,SLOT(add_groups()));
		 cancelButton = new QPushButton("Отмена");
		 connect(cancelButton,SIGNAL(clicked()),this,SLOT(close()));

//		 QGroupBox *configGroup = new QGroupBox("Прямоугольные координаты объекта:");
//		 QLabel *x_label = new QLabel("X:");
//		 lineEdit_x = new QLineEdit();
//		 QLabel *y_label = new QLabel("Y:");
//		 lineEdit_y = new QLineEdit();
		
//	 	 QHBoxLayout *coord_Layout = new QHBoxLayout;
//		 coord_Layout->addWidget(x_label);
//		 coord_Layout->addWidget(lineEdit_x);
//		 coord_Layout->addWidget(y_label);
//		 coord_Layout->addWidget(lineEdit_y);
   
//		 QVBoxLayout *configLayout = new QVBoxLayout;
//		 configLayout->addLayout(coord_Layout);
//		 configGroup->setLayout(configLayout);

		 QHBoxLayout *buttonsLayout = new QHBoxLayout;
		 buttonsLayout->addStretch();
		 buttonsLayout->addWidget(addButton);
		 buttonsLayout->addWidget(cancelButton);

		 QHBoxLayout *topLeftLayout = new QHBoxLayout;
		 topLeftLayout->addWidget(label);
		 QHBoxLayout *topLeftLayout_2 = new QHBoxLayout;
		 topLeftLayout_2->addWidget(lineEdit_name);
		 
		 QHBoxLayout *topLeftLayout_3 = new QHBoxLayout;
		 topLeftLayout_3->addWidget(label_2);
		 QHBoxLayout *topLeftLayout_4 = new QHBoxLayout;
		 topLeftLayout_4->addWidget(comboBox);
		 QHBoxLayout *topLeftLayout_5 = new QHBoxLayout;
		 topLeftLayout_5->addWidget(label_3);
		 QHBoxLayout *topLeftLayout_6 = new QHBoxLayout;
		 topLeftLayout_6->addWidget(comboBox_2);
		 QHBoxLayout *topLeftLayout_7 = new QHBoxLayout;
		 topLeftLayout_7->addWidget(label_4);
		 QHBoxLayout *topLeftLayout_8 = new QHBoxLayout;
		 topLeftLayout_8->addWidget(comboBox_3);
		 QHBoxLayout *topLeftLayout_9 = new QHBoxLayout;
		 topLeftLayout_9->addWidget(label_5);
		 QHBoxLayout *topLeftLayout_10 = new QHBoxLayout;
		 topLeftLayout_10->addWidget(textEdit_propa);
		 QHBoxLayout *topLeftLayout_11 = new QHBoxLayout;
		 topLeftLayout_11->addWidget(label_6);
		 QHBoxLayout *topLeftLayout_12 = new QHBoxLayout;
		 topLeftLayout_12->addWidget(lineEdit_counte);
		 QHBoxLayout *topLeftLayout_13 = new QHBoxLayout;
		 topLeftLayout_13->addWidget(label_7);
		 QHBoxLayout *topLeftLayout_14 = new QHBoxLayout;
		 topLeftLayout_14->addWidget(textEdit_founder);
		 QHBoxLayout *topLeftLayout_15 = new QHBoxLayout;
		 topLeftLayout_15->addWidget(label_8);
		 QHBoxLayout *topLeftLayout_16 = new QHBoxLayout;
		 topLeftLayout_16->addWidget(textEdit_menegm);
		 QHBoxLayout *topLeftLayout_17 = new QHBoxLayout;
		 topLeftLayout_17->addWidget(label_9);
		 QHBoxLayout *topLeftLayout_18 = new QHBoxLayout;
		 topLeftLayout_18->addWidget(textEdit_office);
		 QHBoxLayout *topLeftLayout_19 = new QHBoxLayout;
		 topLeftLayout_19->addWidget(label_10);
		 QHBoxLayout *topLeftLayout_20 = new QHBoxLayout;
		 topLeftLayout_20->addWidget(textEdit_descr);

		 QVBoxLayout *leftLayout = new QVBoxLayout;
		 leftLayout->addLayout(topLeftLayout);
		 QVBoxLayout *leftLayout_2 = new QVBoxLayout;
		 leftLayout_2->addLayout(topLeftLayout_2);
		 QVBoxLayout *leftLayout_3 = new QVBoxLayout;
		 leftLayout_3->addLayout(topLeftLayout_3);
		 QVBoxLayout *leftLayout_4 = new QVBoxLayout;
		 leftLayout_4->addLayout(topLeftLayout_4);
		 QVBoxLayout *leftLayout_5 = new QVBoxLayout;
		 leftLayout_5->addLayout(topLeftLayout_5);
		 QVBoxLayout *leftLayout_6 = new QVBoxLayout;
		 leftLayout_6->addLayout(topLeftLayout_6);
		 QVBoxLayout *leftLayout_7 = new QVBoxLayout;
		 leftLayout_7->addLayout(topLeftLayout_7);
		 QVBoxLayout *leftLayout_8 = new QVBoxLayout;
		 leftLayout_8->addLayout(topLeftLayout_8);
		 QVBoxLayout *leftLayout_9 = new QVBoxLayout;
		 leftLayout_9->addLayout(topLeftLayout_9);
		 QVBoxLayout *leftLayout_10 = new QVBoxLayout;
		 leftLayout_10->addLayout(topLeftLayout_10);
		 QVBoxLayout *leftLayout_11 = new QVBoxLayout;
		 leftLayout_11->addLayout(topLeftLayout_11);
		 QVBoxLayout *leftLayout_12 = new QVBoxLayout;
		 leftLayout_12->addLayout(topLeftLayout_12);
		 QVBoxLayout *leftLayout_13 = new QVBoxLayout;
		 leftLayout_13->addLayout(topLeftLayout_13);
		 QVBoxLayout *leftLayout_14 = new QVBoxLayout;
		 leftLayout_14->addLayout(topLeftLayout_14);
		 QVBoxLayout *leftLayout_15 = new QVBoxLayout;
		 leftLayout_15->addLayout(topLeftLayout_15);
		 QVBoxLayout *leftLayout_16 = new QVBoxLayout;
		 leftLayout_16->addLayout(topLeftLayout_16);
		 QVBoxLayout *leftLayout_17 = new QVBoxLayout;
		 leftLayout_17->addLayout(topLeftLayout_17);
		 QVBoxLayout *leftLayout_18 = new QVBoxLayout;
		 leftLayout_18->addLayout(topLeftLayout_18);
		 QVBoxLayout *leftLayout_19 = new QVBoxLayout;
		 leftLayout_19->addLayout(topLeftLayout_19);
		 QVBoxLayout *leftLayout_20 = new QVBoxLayout;
		 leftLayout_20->addLayout(topLeftLayout_20);

		 QGridLayout *mainLayout = new QGridLayout;
		 mainLayout->setSizeConstraint(QLayout::SetFixedSize);
		 mainLayout->addLayout(leftLayout, 0, 0);
		 mainLayout->addLayout(leftLayout_2, 0, 1);
		 mainLayout->addLayout(leftLayout_3, 1, 0);
		 mainLayout->addLayout(leftLayout_4, 1, 1);
		 mainLayout->addLayout(leftLayout_5, 2, 0);
		 mainLayout->addLayout(leftLayout_6, 2, 1);
		 mainLayout->addLayout(leftLayout_7, 3, 0);
		 mainLayout->addLayout(leftLayout_8, 3, 1);
//		 mainLayout->addWidget(configGroup, 4, 0, 1, 2);
         mainLayout->addLayout(leftLayout_9, 5, 0);
         mainLayout->addLayout(leftLayout_10, 5, 1);
         mainLayout->addLayout(leftLayout_11, 4, 0);
         mainLayout->addLayout(leftLayout_12, 4, 1);
         mainLayout->addLayout(leftLayout_13, 6, 0);
         mainLayout->addLayout(leftLayout_14, 6, 1);
         mainLayout->addLayout(leftLayout_15, 7, 0);
         mainLayout->addLayout(leftLayout_16, 7, 1);
         mainLayout->addLayout(leftLayout_17, 8, 0);
         mainLayout->addLayout(leftLayout_18, 8, 1);
         mainLayout->addLayout(leftLayout_19, 9, 0);
         mainLayout->addLayout(leftLayout_20, 9, 1);
         mainLayout->addLayout(buttonsLayout, 12, 0, 1, 2);


		 setLayout(mainLayout);
		 setWindowTitle("Добавить организацию");
         setWindowIcon(QIcon(":/Resources/group.png"));
		 break;
		}
case 4:// ============ Национальность ================================================
	{
		 directs_view = new QTableWidget;
		 directs_view->setFixedSize(600,300);

		addButton = new QPushButton("Добавить");
		addButton->setDefault(true);
		connect(addButton,SIGNAL(clicked()),this,SLOT(add_nations_table_dlg()));
		cancelButton = new QPushButton("Выход");
		connect(cancelButton,SIGNAL(clicked()),this,SLOT(close()));

		QHBoxLayout *buttonsLayout = new QHBoxLayout;
		buttonsLayout->addStretch();
		buttonsLayout->addWidget(addButton);
		buttonsLayout->addWidget(cancelButton);


		 QHBoxLayout *h_lay = new QHBoxLayout;

		 h_lay->addWidget(directs_view);
				 
		 QGridLayout *mainLayout = new QGridLayout;
		 mainLayout->setSizeConstraint(QLayout::SetFixedSize);
		 mainLayout->addLayout(h_lay, 1, 0);
		 mainLayout->addLayout(buttonsLayout, 3, 0, 3, 1);
		 this->setLayout(mainLayout);
		 setWindowTitle("Национальный состав");
         setWindowIcon(QIcon(":/Resources/user_config.png"));
	//==========================================
	 table_nations();

     connect(directs_view,SIGNAL(cellClicked(int,int)),this,SLOT(show_redaktor(int,int)));

	 break;
		} 
case 5:{ //========== Воинские формирования ====================================
		 
		 label = new QLabel("Наименование подразделения:");
		 lineEdit_name = new QLineEdit;
		 label->setBuddy(lineEdit_name);
		 
		 label_2 = new QLabel("Тип подразделения:");
		 comboBox = new QComboBox();
		 label_2->setBuddy(comboBox);
		 QStringList column;
		 column.clear();
		 column<<"name_type_ls";
		 fill_combobox(comboBox,"type_ls",column);
		 label_3 = new QLabel("Общая численность личного состава:");
		 lineEdit_counte_ls = new QLineEdit;
		 label_3->setBuddy(lineEdit_counte_ls);
		 label_4 = new QLabel("Общая численность личного состава,\n участвующего в БД:");
		 lineEdit_counte_ls_bd = new QLineEdit;
		 label_4->setBuddy(lineEdit_counte_ls_bd);
		 label_5 = new QLabel("Враждебность подразделения:");
		 checkbox_enemy = new QCheckBox(" [-V- враждебное]", this);
		 label_5->setBuddy(checkbox_enemy);
		 
//	     QGroupBox *configGroup = new QGroupBox("Прямоугольные координаты объекта:");
//		 QLabel *x_label = new QLabel("X:");
//		 lineEdit_x = new QLineEdit();
//		 QLabel *y_label = new QLabel("Y:");
//		 lineEdit_y = new QLineEdit();
		
//	 	 QHBoxLayout *coord_Layout = new QHBoxLayout;
//		 coord_Layout->addWidget(x_label);
//		 coord_Layout->addWidget(lineEdit_x);
//		 coord_Layout->addWidget(y_label);
//		 coord_Layout->addWidget(lineEdit_y);
   
//		 QVBoxLayout *configLayout = new QVBoxLayout;
//		 configLayout->addLayout(coord_Layout);
//		 configGroup->setLayout(configLayout);

		 addButton = new QPushButton("Добавить");
		 addButton->setDefault(true);
		 connect(addButton,SIGNAL(clicked()),this,SLOT(add_ls_parent()));
		 cancelButton = new QPushButton("Отмена");
		 connect(cancelButton,SIGNAL(clicked()),this,SLOT(close()));

		 QHBoxLayout *buttonsLayout = new QHBoxLayout;
		 buttonsLayout->addStretch();
		 buttonsLayout->addWidget(addButton);
		 buttonsLayout->addWidget(cancelButton);
		 
		 QHBoxLayout *topLeftLayout = new QHBoxLayout;
		 topLeftLayout->addWidget(label);
		 QHBoxLayout *topLeftLayout_2 = new QHBoxLayout;
		 topLeftLayout_2->addWidget(lineEdit_name);
		 QHBoxLayout *topLeftLayout_3 = new QHBoxLayout;
		 topLeftLayout_3->addWidget(label_2);
		 QHBoxLayout *topLeftLayout_4 = new QHBoxLayout;
		 topLeftLayout_4->addWidget(comboBox);
		 QHBoxLayout *topLeftLayout_5 = new QHBoxLayout;
		 topLeftLayout_5->addWidget(label_3);
		 QHBoxLayout *topLeftLayout_6 = new QHBoxLayout;
		 topLeftLayout_6->addWidget(lineEdit_counte_ls);
		 QHBoxLayout *topLeftLayout_7 = new QHBoxLayout;
		 topLeftLayout_7->addWidget(label_4);
		 QHBoxLayout *topLeftLayout_8 = new QHBoxLayout;
		 topLeftLayout_8->addWidget(lineEdit_counte_ls_bd);
		 QHBoxLayout *topLeftLayout_9 = new QHBoxLayout;
		 topLeftLayout_9->addWidget(label_5);
		 QHBoxLayout *topLeftLayout_10 = new QHBoxLayout;
		 topLeftLayout_10->addWidget(checkbox_enemy);


		 QVBoxLayout *leftLayout = new QVBoxLayout;
		 leftLayout->addLayout(topLeftLayout);
		 QVBoxLayout *leftLayout_2 = new QVBoxLayout;
		 leftLayout_2->addLayout(topLeftLayout_2);
		 QVBoxLayout *leftLayout_3 = new QVBoxLayout;
		 leftLayout_3->addLayout(topLeftLayout_3);
		 QVBoxLayout *leftLayout_4 = new QVBoxLayout;
		 leftLayout_4->addLayout(topLeftLayout_4);
		 QVBoxLayout *leftLayout_5 = new QVBoxLayout;
		 leftLayout_5->addLayout(topLeftLayout_5);
		 QVBoxLayout *leftLayout_6 = new QVBoxLayout;
		 leftLayout_6->addLayout(topLeftLayout_6);
		 QVBoxLayout *leftLayout_7 = new QVBoxLayout;
		 leftLayout_7->addLayout(topLeftLayout_7);
		 QVBoxLayout *leftLayout_8 = new QVBoxLayout;
		 leftLayout_8->addLayout(topLeftLayout_8);
		 QVBoxLayout *leftLayout_9 = new QVBoxLayout;
		 leftLayout_9->addLayout(topLeftLayout_9);
		 QVBoxLayout *leftLayout_10 = new QVBoxLayout;
		 leftLayout_10->addLayout(topLeftLayout_10);
		 leftLayout->addStretch(1);

		 QGridLayout *mainLayout = new QGridLayout;
		 mainLayout->setSizeConstraint(QLayout::SetFixedSize);
		 mainLayout->addLayout(leftLayout, 0, 0);
		 mainLayout->addLayout(leftLayout_2, 0, 1);
		 mainLayout->addLayout(leftLayout_3, 1, 0);
		 mainLayout->addLayout(leftLayout_4, 1, 1);
		 mainLayout->addLayout(leftLayout_5, 2, 0);
		 mainLayout->addLayout(leftLayout_6, 2, 1);
		 mainLayout->addLayout(leftLayout_7, 3, 0);
		 mainLayout->addLayout(leftLayout_8, 3, 1);
		 mainLayout->addLayout(leftLayout_9, 4, 0);
		 mainLayout->addLayout(leftLayout_10, 4, 1);
//		 mainLayout->addWidget(configGroup, 5, 0, 1, 2);
         mainLayout->addLayout(buttonsLayout, 5, 0, 1, 2);
		 setLayout(mainLayout);
		 setWindowTitle("Добавить воинское формирование");
         setWindowIcon(QIcon(":/Resources/weapon.png"));
		 break;
		}
case 6:{// ============ Религия ================================================

		confess_view = new QTableWidget;
		confess_view->setFixedSize(400,300);

		addButton = new QPushButton("Добавить");
		addButton->setDefault(true);
		connect(addButton,SIGNAL(clicked()),this,SLOT(add_confess_table_dlg()));
		cancelButton = new QPushButton("Выход");
		connect(cancelButton,SIGNAL(clicked()),this,SLOT(close()));

		QHBoxLayout *buttonsLayout = new QHBoxLayout;
		buttonsLayout->addStretch();
		buttonsLayout->addWidget(addButton);
		buttonsLayout->addWidget(cancelButton);

		QHBoxLayout *h_lay = new QHBoxLayout;

		h_lay->addWidget(confess_view);
				 
		QGridLayout *mainLayout = new QGridLayout;
		mainLayout->setSizeConstraint(QLayout::SetFixedSize);
		mainLayout->addLayout(h_lay, 1, 0);
		mainLayout->addLayout(buttonsLayout, 3, 0, 3, 1);
		this->setLayout(mainLayout);
		setWindowTitle("Религиозный состав");
        setWindowIcon(QIcon(":/Resources/change_user.png"));
	//==========================================
	 table_confess();

     connect(confess_view,SIGNAL(cellClicked(int,int)),this,SLOT(show_redaktor_conf(int,int)));

	 break;
		} 
case 7:{// ============ Профессиональный состав ================================================

		prof_view = new QTableWidget;
		prof_view->setFixedSize(400,300);

		addButton = new QPushButton("Добавить");
		addButton->setDefault(true);
		connect(addButton,SIGNAL(clicked()),this,SLOT(add_prof_table_dlg()));
		cancelButton = new QPushButton("Выход");
		connect(cancelButton,SIGNAL(clicked()),this,SLOT(close()));

		QHBoxLayout *buttonsLayout = new QHBoxLayout;
		buttonsLayout->addStretch();
		buttonsLayout->addWidget(addButton);
		buttonsLayout->addWidget(cancelButton);
		QHBoxLayout *h_lay = new QHBoxLayout;

		h_lay->addWidget(prof_view);
				 
		QGridLayout *mainLayout = new QGridLayout;
		mainLayout->setSizeConstraint(QLayout::SetFixedSize);
		mainLayout->addLayout(h_lay, 1, 0);
		mainLayout->addLayout(buttonsLayout, 3, 0, 3, 1);
		this->setLayout(mainLayout);
		setWindowTitle("Профессиональный состав");
        setWindowIcon(QIcon(":/Resources/change_user.png"));
	//==========================================
	 table_prof();

     connect(prof_view,SIGNAL(cellClicked(int,int)),this,SLOT(show_redaktor_prof(int,int)));

	 break;
		} 	
case 8:{// ============ Возрастной состав ================================================

		age_view = new QTableWidget;
      //  age_view->resizeColumnToContents(3);

		addButton = new QPushButton("Добавить");
		addButton->setDefault(true);
		connect(addButton,SIGNAL(clicked()),this,SLOT(add_age_table_dlg()));
		cancelButton = new QPushButton("Выход");
		connect(cancelButton,SIGNAL(clicked()),this,SLOT(close()));

		QHBoxLayout *buttonsLayout = new QHBoxLayout;
		buttonsLayout->addStretch();
		buttonsLayout->addWidget(addButton);
		buttonsLayout->addWidget(cancelButton);
		QHBoxLayout *h_lay = new QHBoxLayout;

		h_lay->addWidget(age_view);
				 
		QGridLayout *mainLayout = new QGridLayout;
    //	mainLayout->setSizeConstraint(QLayout::SetFixedSize);
		mainLayout->addLayout(h_lay, 1, 0);
		mainLayout->addLayout(buttonsLayout, 3, 0, 3, 1);
		this->setLayout(mainLayout);
		setWindowTitle("Возрастной состав");
        setWindowIcon(QIcon(":/Resources/change_user.png"));
	//==========================================
	 table_age();

     connect(age_view,SIGNAL(cellClicked(int,int)),this,SLOT(show_redaktor_age(int,int)));

	 break;
		} 	
case 9:{// ============ половой состав ================================================

		sex_view = new QTableWidget;
		sex_view->setFixedSize(520,100);

		addButtonn = new QPushButton("Добавить");
		addButtonn->setDefault(true);
		connect(addButtonn,SIGNAL(clicked()),this,SLOT(add_sex_table_dlg()));
		cancelButton = new QPushButton("Выход");
		connect(cancelButton,SIGNAL(clicked()),this,SLOT(close()));

		QHBoxLayout *buttonsLayout = new QHBoxLayout;
		buttonsLayout->addStretch();
		buttonsLayout->addWidget(addButtonn);
		buttonsLayout->addWidget(cancelButton);
		QHBoxLayout *h_lay = new QHBoxLayout;

		h_lay->addWidget(sex_view);
				 
		QGridLayout *mainLayout = new QGridLayout;
		mainLayout->setSizeConstraint(QLayout::SetFixedSize);
		mainLayout->addLayout(h_lay, 1, 0);
		mainLayout->addLayout(buttonsLayout, 3, 0, 3, 1);
		this->setLayout(mainLayout);
		setWindowTitle("Половой состав");
        setWindowIcon(QIcon(":/Resources/change_user.png"));
	//==========================================
	 table_sex();
	
	 if (sex_view->rowCount()>0) addButtonn->setDisabled(true); 
	
     connect(sex_view,SIGNAL(cellClicked(int,int)),this,SLOT(show_redaktor_sex(int,int)));

	 break;
		} 	
case 10:// ============ Национальность для ВФ================================================
	{
		 directs_view = new QTableWidget;
		 directs_view->setFixedSize(600,300);

		addButton = new QPushButton("Добавить");
		addButton->setDefault(true);
		connect(addButton,SIGNAL(clicked()),this,SLOT(add_nations_table_dlg_vf()));
		cancelButton = new QPushButton("Выход");
		connect(cancelButton,SIGNAL(clicked()),this,SLOT(close()));

		QHBoxLayout *buttonsLayout = new QHBoxLayout;
		buttonsLayout->addStretch();
		buttonsLayout->addWidget(addButton);
		buttonsLayout->addWidget(cancelButton);


		 QHBoxLayout *h_lay = new QHBoxLayout;

		 h_lay->addWidget(directs_view);
				 
		 QGridLayout *mainLayout = new QGridLayout;
		 mainLayout->setSizeConstraint(QLayout::SetFixedSize);
		 mainLayout->addLayout(h_lay, 1, 0);
		 mainLayout->addLayout(buttonsLayout, 3, 0, 3, 1);
		 this->setLayout(mainLayout);
		 setWindowTitle("Национальный состав");
         setWindowIcon(QIcon(":/Resources/change_user.png"));
	//==========================================
	 table_nations_vf();

     connect(directs_view,SIGNAL(cellClicked(int,int)),this,SLOT(show_redaktor(int,int)));

	 break;
		} 
case 11:{// ============ Религия для ВФ ================================================

		confess_view = new QTableWidget;
		confess_view->setFixedSize(400,300);

		addButton = new QPushButton("Добавить");
		addButton->setDefault(true);
		connect(addButton,SIGNAL(clicked()),this,SLOT(add_confess_table_dlg_conf()));
		cancelButton = new QPushButton("Выход");
		connect(cancelButton,SIGNAL(clicked()),this,SLOT(close()));

		QHBoxLayout *buttonsLayout = new QHBoxLayout;
		buttonsLayout->addStretch();
		buttonsLayout->addWidget(addButton);
		buttonsLayout->addWidget(cancelButton);

		QHBoxLayout *h_lay = new QHBoxLayout;

		h_lay->addWidget(confess_view);
				 
		QGridLayout *mainLayout = new QGridLayout;
		mainLayout->setSizeConstraint(QLayout::SetFixedSize);
		mainLayout->addLayout(h_lay, 1, 0);
		mainLayout->addLayout(buttonsLayout, 3, 0, 3, 1);
		this->setLayout(mainLayout);
		setWindowTitle("Религиозный состав");
        setWindowIcon(QIcon(":/Resources/change_user.png"));
	//==========================================
	 table_confess_conf();

     connect(confess_view,SIGNAL(cellClicked(int,int)),this,SLOT(show_redaktor_conf(int,int)));

	 break;
	   } 
case 12:{// ============ Штатка ================================================

		rank_view = new QTableWidget;
		rank_view->setFixedSize(400,300);

		addButton = new QPushButton("Добавить");
		addButton->setDefault(true);
		connect(addButton,SIGNAL(clicked()),this,SLOT(add_rank_table_dlg()));
		cancelButton = new QPushButton("Выход");
		connect(cancelButton,SIGNAL(clicked()),this,SLOT(close()));

		QHBoxLayout *buttonsLayout = new QHBoxLayout;
		buttonsLayout->addStretch();
		buttonsLayout->addWidget(addButton);
		buttonsLayout->addWidget(cancelButton);

		QHBoxLayout *h_lay = new QHBoxLayout;

		h_lay->addWidget(rank_view);
				 
		QGridLayout *mainLayout = new QGridLayout;
		mainLayout->setSizeConstraint(QLayout::SetFixedSize);
		mainLayout->addLayout(h_lay, 1, 0);
		mainLayout->addLayout(buttonsLayout, 3, 0, 3, 1);
		this->setLayout(mainLayout);
		setWindowTitle("Штатно-должностной состав");
        setWindowIcon(QIcon(":/Resources/change_user.png"));
	//==========================================
	 table_rank();

     connect(rank_view,SIGNAL(cellClicked(int,int)),this,SLOT(show_redaktor_rank(int,int)));

	 break;
		} 
case 13: { //=========================== средства =========================================
		 label = new QLabel("Наименование средства:");
		 lineEdit_name = new QLineEdit;
		 label->setBuddy(lineEdit_name);
		 
		 label_2 = new QLabel("Тип средства:");
		 comboBox = new QComboBox();
		 label_2->setBuddy(comboBox);
		 QStringList column;
		 column.clear();
		 column<<"name_type_mpo_pso";
		 fill_combobox(comboBox,"type_mpo_pso",column);
		 			
		 label_3 = new QLabel("количество средств:");
		 lineEdit_counte_mpo_pso = new QLineEdit;
		 label_3->setBuddy(lineEdit_counte_mpo_pso);
		 label_4 = new QLabel("Описание средства:");
		 textEdit_desc = new QTextEdit;
		 textEdit_desc->setFixedHeight(50);
		 label_4->setBuddy(textEdit_desc);

         QLabel *label_f = new QLabel("Фото средства:");
         lineEdit_name_f = new QLineEdit;
         label_f->setBuddy(lineEdit_name_f);


		 addButton = new QPushButton("Добавить");
		 addButton->setDefault(true);
		 connect(addButton,SIGNAL(clicked()),this,SLOT(add_mpo_pso()));
		 cancelButton = new QPushButton("Отмена");
		 connect(cancelButton,SIGNAL(clicked()),this,SLOT(close()));
         QPushButton *openButton = new QPushButton("открыть");
         openButton->setText("...");
         openButton->setFixedSize(30,20);
         connect(openButton,SIGNAL(clicked()),this,SLOT(open_file()));

		 QHBoxLayout *buttonsLayout = new QHBoxLayout;
		 buttonsLayout->addStretch();
		 buttonsLayout->addWidget(addButton);
		 buttonsLayout->addWidget(cancelButton);

         QHBoxLayout *topLeftLayout_15 = new QHBoxLayout;
         topLeftLayout_15->addWidget(label_f);
         QHBoxLayout *topLeftLayout_16 = new QHBoxLayout;
         topLeftLayout_16->addWidget(lineEdit_name_f);
         topLeftLayout_16->addWidget(openButton);


         QHBoxLayout *topLeftLayout = new QHBoxLayout;
		 topLeftLayout->addWidget(label);
		 QHBoxLayout *topLeftLayout_2 = new QHBoxLayout;
		 topLeftLayout_2->addWidget(lineEdit_name);
		 
		 QHBoxLayout *centrLeftLayout = new QHBoxLayout;
		 centrLeftLayout->addWidget(label_2);
		 QHBoxLayout *centrLeftLayout_2 = new QHBoxLayout;
		 centrLeftLayout_2->addWidget(comboBox);
		 QHBoxLayout *topLeftLayout_3 = new QHBoxLayout;
		 topLeftLayout_3->addWidget(label_3);
		 QHBoxLayout *topLeftLayout_4 = new QHBoxLayout;
		 topLeftLayout_4->addWidget(lineEdit_counte_mpo_pso);
		 QHBoxLayout *topLeftLayout_5 = new QHBoxLayout;
		 topLeftLayout_5->addWidget(label_4);
		 QHBoxLayout *topLeftLayout_6 = new QHBoxLayout;
		 topLeftLayout_6->addWidget(textEdit_desc);
						 			 
         QVBoxLayout *leftLayout_15 = new QVBoxLayout;
         leftLayout_15->addLayout(topLeftLayout_15);
         QVBoxLayout *leftLayout_16 = new QVBoxLayout;
         leftLayout_16->addLayout(topLeftLayout_16);
         QVBoxLayout *leftLayout = new QVBoxLayout;
		 leftLayout->addLayout(topLeftLayout);
		 QVBoxLayout *leftLayout_2 = new QVBoxLayout;
		 leftLayout_2->addLayout(topLeftLayout_2);
		 QVBoxLayout *leftLayout_3 = new QVBoxLayout;
		 leftLayout_3->addLayout(centrLeftLayout);
		 QVBoxLayout *leftLayout_4 = new QVBoxLayout;
		 leftLayout_4->addLayout(centrLeftLayout_2);
		 QVBoxLayout *leftLayout_5 = new QVBoxLayout;
		 leftLayout_5->addLayout(topLeftLayout_3);
		 QVBoxLayout *leftLayout_6 = new QVBoxLayout;
		 leftLayout_6->addLayout(topLeftLayout_4);
		 QVBoxLayout *leftLayout_7 = new QVBoxLayout;
		 leftLayout_7->addLayout(topLeftLayout_5);
		 QVBoxLayout *leftLayout_8 = new QVBoxLayout;
		 leftLayout_8->addLayout(topLeftLayout_6);
		 		 		 			 
		 QGridLayout *mainLayout = new QGridLayout;
		 mainLayout->setSizeConstraint(QLayout::SetFixedSize);
         mainLayout->addLayout(leftLayout_15, 1, 0);
         mainLayout->addLayout(leftLayout_16, 1, 1);
		 mainLayout->addLayout(leftLayout, 2, 0);
		 mainLayout->addLayout(leftLayout_2, 2, 1);
		 mainLayout->addLayout(leftLayout_3, 3, 0);
		 mainLayout->addLayout(leftLayout_4, 3, 1);
		 mainLayout->addLayout(leftLayout_5, 4, 0);
		 mainLayout->addLayout(leftLayout_6, 4, 1);
		 mainLayout->addLayout(leftLayout_7, 5, 0);
		 mainLayout->addLayout(leftLayout_8, 5, 1);

         mainLayout->addLayout(buttonsLayout, 6, 0, 1, 2);
		 setLayout(mainLayout);
		 setWindowTitle("Добавить средства");
         setWindowIcon(QIcon(":/Resources/connect_saturn.png"));
		 break;
		 
		 }
 case 14: { //=========================== средства организаций =========================================
		 label = new QLabel("Наименование средства:");
		 lineEdit_name = new QLineEdit;
		 label->setBuddy(lineEdit_name);
		 
		 label_2 = new QLabel("Тип средства:");
		 comboBox = new QComboBox();
		 label_2->setBuddy(comboBox);
		 QStringList column;
		 column.clear();
		 column<<"name_type_mpo_pso";
		 fill_combobox(comboBox,"type_mpo_pso",column);
		 			
		 label_3 = new QLabel("количество средств:");
		 lineEdit_counte_mpo_pso = new QLineEdit;
		 label_3->setBuddy(lineEdit_counte_mpo_pso);
		 label_4 = new QLabel("Описание средства:");
		 textEdit_desc = new QTextEdit;
		 textEdit_desc->setFixedHeight(50);
		 label_4->setBuddy(textEdit_desc);

         QLabel *label_f = new QLabel("Фото средства:");
         lineEdit_name_f = new QLineEdit;
         label_f->setBuddy(lineEdit_name_f);
				 			 
		 addButton = new QPushButton("Добавить");
		 addButton->setDefault(true);
		 connect(addButton,SIGNAL(clicked()),this,SLOT(add_mpo_pso_groups()));
		 cancelButton = new QPushButton("Отмена");
		 connect(cancelButton,SIGNAL(clicked()),this,SLOT(close()));
         QPushButton *openButton = new QPushButton("открыть");
         openButton->setText("...");
         openButton->setFixedSize(30,20);
         connect(openButton,SIGNAL(clicked()),this,SLOT(open_file()));

		 QHBoxLayout *buttonsLayout = new QHBoxLayout;
		 buttonsLayout->addStretch();
		 buttonsLayout->addWidget(addButton);
		 buttonsLayout->addWidget(cancelButton);

         QHBoxLayout *topLeftLayout_15 = new QHBoxLayout;
         topLeftLayout_15->addWidget(label_f);
         QHBoxLayout *topLeftLayout_16 = new QHBoxLayout;
         topLeftLayout_16->addWidget(lineEdit_name_f);
         topLeftLayout_16->addWidget(openButton);

		 QHBoxLayout *topLeftLayout = new QHBoxLayout;
		 topLeftLayout->addWidget(label);
		 QHBoxLayout *topLeftLayout_2 = new QHBoxLayout;
		 topLeftLayout_2->addWidget(lineEdit_name);
		 
		 QHBoxLayout *centrLeftLayout = new QHBoxLayout;
		 centrLeftLayout->addWidget(label_2);
		 QHBoxLayout *centrLeftLayout_2 = new QHBoxLayout;
		 centrLeftLayout_2->addWidget(comboBox);
		 QHBoxLayout *topLeftLayout_3 = new QHBoxLayout;
		 topLeftLayout_3->addWidget(label_3);
		 QHBoxLayout *topLeftLayout_4 = new QHBoxLayout;
		 topLeftLayout_4->addWidget(lineEdit_counte_mpo_pso);
		 QHBoxLayout *topLeftLayout_5 = new QHBoxLayout;
		 topLeftLayout_5->addWidget(label_4);
		 QHBoxLayout *topLeftLayout_6 = new QHBoxLayout;
		 topLeftLayout_6->addWidget(textEdit_desc);

         QVBoxLayout *leftLayout_15 = new QVBoxLayout;
         leftLayout_15->addLayout(topLeftLayout_15);
         QVBoxLayout *leftLayout_16 = new QVBoxLayout;
         leftLayout_16->addLayout(topLeftLayout_16);
						 			 
		 QVBoxLayout *leftLayout = new QVBoxLayout;
		 leftLayout->addLayout(topLeftLayout);
		 QVBoxLayout *leftLayout_2 = new QVBoxLayout;
		 leftLayout_2->addLayout(topLeftLayout_2);
		 QVBoxLayout *leftLayout_3 = new QVBoxLayout;
		 leftLayout_3->addLayout(centrLeftLayout);
		 QVBoxLayout *leftLayout_4 = new QVBoxLayout;
		 leftLayout_4->addLayout(centrLeftLayout_2);
		 QVBoxLayout *leftLayout_5 = new QVBoxLayout;
		 leftLayout_5->addLayout(topLeftLayout_3);
		 QVBoxLayout *leftLayout_6 = new QVBoxLayout;
		 leftLayout_6->addLayout(topLeftLayout_4);
		 QVBoxLayout *leftLayout_7 = new QVBoxLayout;
		 leftLayout_7->addLayout(topLeftLayout_5);
		 QVBoxLayout *leftLayout_8 = new QVBoxLayout;
		 leftLayout_8->addLayout(topLeftLayout_6);
		 		 		 			 
		 QGridLayout *mainLayout = new QGridLayout;
		 mainLayout->setSizeConstraint(QLayout::SetFixedSize);
         mainLayout->addLayout(leftLayout_15, 1, 0);
         mainLayout->addLayout(leftLayout_16, 1, 1);
		 mainLayout->addLayout(leftLayout, 2, 0);
		 mainLayout->addLayout(leftLayout_2, 2, 1);
		 mainLayout->addLayout(leftLayout_3, 3, 0);
		 mainLayout->addLayout(leftLayout_4, 3, 1);
		 mainLayout->addLayout(leftLayout_5, 4, 0);
		 mainLayout->addLayout(leftLayout_6, 4, 1);
		 mainLayout->addLayout(leftLayout_7, 5, 0);
		 mainLayout->addLayout(leftLayout_8, 5, 1);
//		 mainLayout->addWidget(configGroup, 6, 0, 1, 2);
	
         mainLayout->addLayout(buttonsLayout, 6, 0, 1, 2);
		 setLayout(mainLayout);
		 setWindowTitle("Добавить средства");
         setWindowIcon(QIcon(":/Resources/connect_saturn.png"));
		 break;
		 
		 }
case 15: { //=========================== средства СМИ =========================================
		 label = new QLabel("Наименование средства:");
		 lineEdit_name = new QLineEdit;
		 label->setBuddy(lineEdit_name);
		 
		 label_2 = new QLabel("Тип средства:");
		 comboBox = new QComboBox();
		 label_2->setBuddy(comboBox);
		 QStringList column;
		 column.clear();
		 column<<"name_type_mpo_pso";
		 fill_combobox(comboBox,"type_mpo_pso",column);
		 			
		 label_3 = new QLabel("количество средств:");
		 lineEdit_counte_mpo_pso = new QLineEdit;
		 label_3->setBuddy(lineEdit_counte_mpo_pso);
		 label_4 = new QLabel("Описание средства:");
		 textEdit_desc = new QTextEdit;
		 textEdit_desc->setFixedHeight(50);
		 label_4->setBuddy(textEdit_desc);

         QLabel *label_f = new QLabel("Фото средства:");
         lineEdit_name_f = new QLineEdit;
         label_f->setBuddy(lineEdit_name_f);
				 			 
		 addButton = new QPushButton("Добавить");
		 addButton->setDefault(true);
		 connect(addButton,SIGNAL(clicked()),this,SLOT(add_mpo_pso_smi()));
		 cancelButton = new QPushButton("Отмена");
		 connect(cancelButton,SIGNAL(clicked()),this,SLOT(close()));
         QPushButton *openButton = new QPushButton("открыть");
         openButton->setText("...");
         openButton->setFixedSize(30,20);
         connect(openButton,SIGNAL(clicked()),this,SLOT(open_file()));

		 QHBoxLayout *buttonsLayout = new QHBoxLayout;
		 buttonsLayout->addStretch();
		 buttonsLayout->addWidget(addButton);
		 buttonsLayout->addWidget(cancelButton);

         QHBoxLayout *topLeftLayout_15 = new QHBoxLayout;
         topLeftLayout_15->addWidget(label_f);
         QHBoxLayout *topLeftLayout_16 = new QHBoxLayout;
         topLeftLayout_16->addWidget(lineEdit_name_f);
         topLeftLayout_16->addWidget(openButton);

         QHBoxLayout *topLeftLayout = new QHBoxLayout;
		 topLeftLayout->addWidget(label);
		 QHBoxLayout *topLeftLayout_2 = new QHBoxLayout;
		 topLeftLayout_2->addWidget(lineEdit_name);
		 
		 QHBoxLayout *centrLeftLayout = new QHBoxLayout;
		 centrLeftLayout->addWidget(label_2);
		 QHBoxLayout *centrLeftLayout_2 = new QHBoxLayout;
		 centrLeftLayout_2->addWidget(comboBox);
		 QHBoxLayout *topLeftLayout_3 = new QHBoxLayout;
		 topLeftLayout_3->addWidget(label_3);
		 QHBoxLayout *topLeftLayout_4 = new QHBoxLayout;
		 topLeftLayout_4->addWidget(lineEdit_counte_mpo_pso);
		 QHBoxLayout *topLeftLayout_5 = new QHBoxLayout;
		 topLeftLayout_5->addWidget(label_4);
		 QHBoxLayout *topLeftLayout_6 = new QHBoxLayout;
		 topLeftLayout_6->addWidget(textEdit_desc);
						 			 

         QVBoxLayout *leftLayout_15 = new QVBoxLayout;
         leftLayout_15->addLayout(topLeftLayout_15);
         QVBoxLayout *leftLayout_16 = new QVBoxLayout;
         leftLayout_16->addLayout(topLeftLayout_16);


         QVBoxLayout *leftLayout = new QVBoxLayout;
		 leftLayout->addLayout(topLeftLayout);
		 QVBoxLayout *leftLayout_2 = new QVBoxLayout;
		 leftLayout_2->addLayout(topLeftLayout_2);
		 QVBoxLayout *leftLayout_3 = new QVBoxLayout;
		 leftLayout_3->addLayout(centrLeftLayout);
		 QVBoxLayout *leftLayout_4 = new QVBoxLayout;
		 leftLayout_4->addLayout(centrLeftLayout_2);
		 QVBoxLayout *leftLayout_5 = new QVBoxLayout;
		 leftLayout_5->addLayout(topLeftLayout_3);
		 QVBoxLayout *leftLayout_6 = new QVBoxLayout;
		 leftLayout_6->addLayout(topLeftLayout_4);
		 QVBoxLayout *leftLayout_7 = new QVBoxLayout;
		 leftLayout_7->addLayout(topLeftLayout_5);
		 QVBoxLayout *leftLayout_8 = new QVBoxLayout;
		 leftLayout_8->addLayout(topLeftLayout_6);
		 		 		 			 
		 QGridLayout *mainLayout = new QGridLayout;
		 mainLayout->setSizeConstraint(QLayout::SetFixedSize);
         mainLayout->addLayout(leftLayout_15, 1, 0);
         mainLayout->addLayout(leftLayout_16, 1, 1);
		 mainLayout->addLayout(leftLayout, 2, 0);
		 mainLayout->addLayout(leftLayout_2, 2, 1);
		 mainLayout->addLayout(leftLayout_3, 3, 0);
		 mainLayout->addLayout(leftLayout_4, 3, 1);
		 mainLayout->addLayout(leftLayout_5, 4, 0);
		 mainLayout->addLayout(leftLayout_6, 4, 1);
		 mainLayout->addLayout(leftLayout_7, 5, 0);
		 mainLayout->addLayout(leftLayout_8, 5, 1);
//		 mainLayout->addWidget(configGroup, 6, 0, 1, 2);
	
         mainLayout->addLayout(buttonsLayout, 6, 0, 1, 2);
		 setLayout(mainLayout);
		 setWindowTitle("Добавить средства");
         setWindowIcon(QIcon(":/Resources/connect_saturn.png"));
		 break;
		 
		 }
case 16://=============== Условия ============================================
		{
		 label = new QLabel("Наименование особого условия:");
		 lineEdit_name = new QLineEdit;
		 label->setBuddy(lineEdit_name);

		 QLabel *label_f = new QLabel("Фото особого условия:");
         lineEdit_name_f = new QLineEdit;
         label_f->setBuddy(lineEdit_name_f);
		 
		 label_2 = new QLabel("Тип особого условия");
		 comboBox = new QComboBox();
		 comboBox->setFixedWidth(270);
		 label_2->setBuddy(comboBox);
		 QStringList column;
		 column.clear();
		 column<<"name_type_special_conditions";
		 fill_combobox(comboBox,"type_special_conditions",column);
		
		 label_3 = new QLabel("Описание особого условия:");
		 textEdit_propa = new QTextEdit;
		 label_3->setBuddy(textEdit_propa);
		 textEdit_propa->setFixedHeight(50);
		
		 label_4 = new QLabel("Семантика_1:");
		 lineEdit_sem_1 = new QLineEdit;
		 label_4->setBuddy(lineEdit_sem_1);
		 label_5 = new QLabel("Семантика_2:");
		 lineEdit_sem_2 = new QLineEdit;
		 label_5->setBuddy(lineEdit_sem_2);
		

		 addButton = new QPushButton("Добавить");
		 addButton->setDefault(true);
		 connect(addButton,SIGNAL(clicked()),this,SLOT(add_special_cond()));
		 cancelButton = new QPushButton("Отмена");
		 connect(cancelButton,SIGNAL(clicked()),this,SLOT(close()));

         QPushButton *openButton = new QPushButton("открыть");
         openButton->setText("...");
         openButton->setFixedSize(30,20);
         connect(openButton,SIGNAL(clicked()),this,SLOT(open_file()));

		 		 
		 QHBoxLayout *buttonsLayout = new QHBoxLayout;
		 buttonsLayout->addStretch();
		 buttonsLayout->addWidget(addButton);
		 buttonsLayout->addWidget(cancelButton);

		 QHBoxLayout *topLeftLayout_15 = new QHBoxLayout;
         topLeftLayout_15->addWidget(label_f);
         QHBoxLayout *topLeftLayout_16 = new QHBoxLayout;
         topLeftLayout_16->addWidget(lineEdit_name_f);
         topLeftLayout_16->addWidget(openButton);

		 QHBoxLayout *topLeftLayout = new QHBoxLayout;
		 topLeftLayout->addWidget(label);
		 QHBoxLayout *topLeftLayout_2 = new QHBoxLayout;
		 topLeftLayout_2->addWidget(lineEdit_name);
		 
		 QHBoxLayout *topLeftLayout_3 = new QHBoxLayout;
		 topLeftLayout_3->addWidget(label_2);
		 QHBoxLayout *topLeftLayout_4 = new QHBoxLayout;
		 topLeftLayout_4->addWidget(comboBox);
		 QHBoxLayout *topLeftLayout_5 = new QHBoxLayout;
		 topLeftLayout_5->addWidget(label_3);
		 QHBoxLayout *topLeftLayout_6 = new QHBoxLayout;
		 topLeftLayout_6->addWidget(textEdit_propa);
		 QHBoxLayout *topLeftLayout_7 = new QHBoxLayout;
		 topLeftLayout_7->addWidget(label_4);
		 QHBoxLayout *topLeftLayout_8 = new QHBoxLayout;
		 topLeftLayout_8->addWidget(lineEdit_sem_1);
		 QHBoxLayout *topLeftLayout_9 = new QHBoxLayout;
		 topLeftLayout_9->addWidget(label_5);
		 QHBoxLayout *topLeftLayout_10 = new QHBoxLayout;
		 topLeftLayout_10->addWidget(lineEdit_sem_2);
		
		 QVBoxLayout *leftLayout_15 = new QVBoxLayout;
         leftLayout_15->addLayout(topLeftLayout_15);
         QVBoxLayout *leftLayout_16 = new QVBoxLayout;
         leftLayout_16->addLayout(topLeftLayout_16);
		 QVBoxLayout *leftLayout = new QVBoxLayout;
		 leftLayout->addLayout(topLeftLayout);
		 QVBoxLayout *leftLayout_2 = new QVBoxLayout;
		 leftLayout_2->addLayout(topLeftLayout_2);
		 QVBoxLayout *leftLayout_3 = new QVBoxLayout;
		 leftLayout_3->addLayout(topLeftLayout_3);
		 QVBoxLayout *leftLayout_4 = new QVBoxLayout;
		 leftLayout_4->addLayout(topLeftLayout_4);
		 QVBoxLayout *leftLayout_5 = new QVBoxLayout;
		 leftLayout_5->addLayout(topLeftLayout_5);
		 QVBoxLayout *leftLayout_6 = new QVBoxLayout;
		 leftLayout_6->addLayout(topLeftLayout_6);
		 QVBoxLayout *leftLayout_7 = new QVBoxLayout;
		 leftLayout_7->addLayout(topLeftLayout_7);
		 QVBoxLayout *leftLayout_8 = new QVBoxLayout;
		 leftLayout_8->addLayout(topLeftLayout_8);
		 QVBoxLayout *leftLayout_9 = new QVBoxLayout;
		 leftLayout_9->addLayout(topLeftLayout_9);
		 QVBoxLayout *leftLayout_10 = new QVBoxLayout;
		 leftLayout_10->addLayout(topLeftLayout_10);
		 
		 QGridLayout *mainLayout = new QGridLayout;
		 mainLayout->setSizeConstraint(QLayout::SetFixedSize);
		 mainLayout->addLayout(leftLayout, 0, 0);
		 mainLayout->addLayout(leftLayout_2, 0, 1);
		 mainLayout->addLayout(leftLayout_15, 1, 0);
         mainLayout->addLayout(leftLayout_16, 1, 1);
		 mainLayout->addLayout(leftLayout_3, 2, 0);
		 mainLayout->addLayout(leftLayout_4, 2, 1);
		 mainLayout->addLayout(leftLayout_5, 3, 0);
		 mainLayout->addLayout(leftLayout_6, 3, 1);
		 mainLayout->addLayout(leftLayout_7, 4, 0);
		 mainLayout->addLayout(leftLayout_8, 4, 1);
		 mainLayout->addLayout(leftLayout_9, 5, 0);
		 mainLayout->addLayout(leftLayout_10, 5, 1);
         mainLayout->addLayout(buttonsLayout, 6, 0, 1, 2);

		 setLayout(mainLayout);
		 setWindowTitle("Добавить особое условие");
         setWindowIcon(QIcon(":/Resources/Stop2.png"));

		 break;
		}
case 17:// ============ Национальность для Организаций ================================================
	{
		 directs_view = new QTableWidget;
		 directs_view->setFixedSize(600,300);

		addButton = new QPushButton("Добавить");
		addButton->setDefault(true);
		connect(addButton,SIGNAL(clicked()),this,SLOT(add_nations_table_dlg_gr()));
		cancelButton = new QPushButton("Выход");
		connect(cancelButton,SIGNAL(clicked()),this,SLOT(close()));

		QHBoxLayout *buttonsLayout = new QHBoxLayout;
		buttonsLayout->addStretch();
		buttonsLayout->addWidget(addButton);
		buttonsLayout->addWidget(cancelButton);


		 QHBoxLayout *h_lay = new QHBoxLayout;

		 h_lay->addWidget(directs_view);
				 
		 QGridLayout *mainLayout = new QGridLayout;
		 mainLayout->setSizeConstraint(QLayout::SetFixedSize);
		 mainLayout->addLayout(h_lay, 1, 0);
		 mainLayout->addLayout(buttonsLayout, 3, 0, 3, 1);
		 this->setLayout(mainLayout);
		 setWindowTitle("Национальный состав");

	//==========================================
	 table_nations_gr();

     connect(directs_view,SIGNAL(cellClicked(int,int)),this,SLOT(show_redaktor(int,int)));

	 break;
		} 
case 18:{// ============ Религия для Организаций ================================================

		confess_view = new QTableWidget;
		confess_view->setFixedSize(400,300);

		addButton = new QPushButton("Добавить");
		addButton->setDefault(true);
		connect(addButton,SIGNAL(clicked()),this,SLOT(add_confess_table_dlg_gr()));
		cancelButton = new QPushButton("Выход");
		connect(cancelButton,SIGNAL(clicked()),this,SLOT(close()));

		QHBoxLayout *buttonsLayout = new QHBoxLayout;
		buttonsLayout->addStretch();
		buttonsLayout->addWidget(addButton);
		buttonsLayout->addWidget(cancelButton);

		QHBoxLayout *h_lay = new QHBoxLayout;

		h_lay->addWidget(confess_view);
				 
		QGridLayout *mainLayout = new QGridLayout;
		mainLayout->setSizeConstraint(QLayout::SetFixedSize);
		mainLayout->addLayout(h_lay, 1, 0);
		mainLayout->addLayout(buttonsLayout, 3, 0, 3, 1);
		this->setLayout(mainLayout);
		setWindowTitle("Религиозный состав");

	//==========================================
	 table_confess_gr();

     connect(confess_view,SIGNAL(cellClicked(int,int)),this,SLOT(show_redaktor_conf(int,int)));

	 break;
	   } 
case 19:{// ============ Профессиональный состав Организаций ================================================

		prof_view = new QTableWidget;
		prof_view->setFixedSize(400,300);

		addButton = new QPushButton("Добавить");
		addButton->setDefault(true);
		connect(addButton,SIGNAL(clicked()),this,SLOT(add_prof_table_dlg_gr()));
		cancelButton = new QPushButton("Выход");
		connect(cancelButton,SIGNAL(clicked()),this,SLOT(close()));

		QHBoxLayout *buttonsLayout = new QHBoxLayout;
		buttonsLayout->addStretch();
		buttonsLayout->addWidget(addButton);
		buttonsLayout->addWidget(cancelButton);
		QHBoxLayout *h_lay = new QHBoxLayout;

		h_lay->addWidget(prof_view);
				 
		QGridLayout *mainLayout = new QGridLayout;
		mainLayout->setSizeConstraint(QLayout::SetFixedSize);
		mainLayout->addLayout(h_lay, 1, 0);
		mainLayout->addLayout(buttonsLayout, 3, 0, 3, 1);
		this->setLayout(mainLayout);
		setWindowTitle("Профессиональный состав");

	//==========================================
	 table_prof_gr();

     connect(prof_view,SIGNAL(cellClicked(int,int)),this,SLOT(show_redaktor_prof(int,int)));

	 break;
		} 	
case 20:{// ============ Возрастной состав для организаций ================================================

		age_view = new QTableWidget;
        //setFixedSize(400,500);

		addButton = new QPushButton("Добавить");
		addButton->setDefault(true);
		connect(addButton,SIGNAL(clicked()),this,SLOT(add_age_table_dlg_gr()));
		cancelButton = new QPushButton("Выход");
		connect(cancelButton,SIGNAL(clicked()),this,SLOT(close()));

		QHBoxLayout *buttonsLayout = new QHBoxLayout;
		buttonsLayout->addStretch();
		buttonsLayout->addWidget(addButton);
		buttonsLayout->addWidget(cancelButton);
		QHBoxLayout *h_lay = new QHBoxLayout;

		h_lay->addWidget(age_view);
				 
		QGridLayout *mainLayout = new QGridLayout;
    //	mainLayout->setSizeConstraint(QLayout::SetFixedSize);
		mainLayout->addLayout(h_lay, 1, 0);
		mainLayout->addLayout(buttonsLayout, 3, 0, 3, 1);
		this->setLayout(mainLayout);
		setWindowTitle("Возрастной состав");

	//==========================================
	 table_age_gr();

     connect(age_view,SIGNAL(cellClicked(int,int)),this,SLOT(show_redaktor_age(int,int)));

	 break;
		} 	
case 21:{// ============ половой состав для организаций ================================================

		sex_view = new QTableWidget;
		sex_view->setFixedSize(520,100);

		addButtonn = new QPushButton("Добавить");
		addButtonn->setDefault(true);
		connect(addButtonn,SIGNAL(clicked()),this,SLOT(add_sex_table_dlg_gr()));
		cancelButton = new QPushButton("Выход");
		connect(cancelButton,SIGNAL(clicked()),this,SLOT(close()));

		QHBoxLayout *buttonsLayout = new QHBoxLayout;
		buttonsLayout->addStretch();
		buttonsLayout->addWidget(addButtonn);
		buttonsLayout->addWidget(cancelButton);
		QHBoxLayout *h_lay = new QHBoxLayout;

		h_lay->addWidget(sex_view);
				 
		QGridLayout *mainLayout = new QGridLayout;
		mainLayout->setSizeConstraint(QLayout::SetFixedSize);
		mainLayout->addLayout(h_lay, 1, 0);
		mainLayout->addLayout(buttonsLayout, 3, 0, 3, 1);
		this->setLayout(mainLayout);
		setWindowTitle("Половой состав");

	//==========================================
	 table_sex_gr();
	
	 if (sex_view->rowCount()>0) addButtonn->setDisabled(true); 
	
     connect(sex_view,SIGNAL(cellClicked(int,int)),this,SLOT(show_redaktor_sex(int,int)));

	 break;
		} 	
case 22:{// ============ Штатка для организаций ================================================

		rank_view = new QTableWidget;
		rank_view->setFixedSize(400,300);

		addButton = new QPushButton("Добавить");
		addButton->setDefault(true);
		connect(addButton,SIGNAL(clicked()),this,SLOT(add_rank_table_dlg_gr()));
		cancelButton = new QPushButton("Выход");
		connect(cancelButton,SIGNAL(clicked()),this,SLOT(close()));

		QHBoxLayout *buttonsLayout = new QHBoxLayout;
		buttonsLayout->addStretch();
		buttonsLayout->addWidget(addButton);
		buttonsLayout->addWidget(cancelButton);

		QHBoxLayout *h_lay = new QHBoxLayout;

		h_lay->addWidget(rank_view);
				 
		QGridLayout *mainLayout = new QGridLayout;
		mainLayout->setSizeConstraint(QLayout::SetFixedSize);
		mainLayout->addLayout(h_lay, 1, 0);
		mainLayout->addLayout(buttonsLayout, 3, 0, 3, 1);
		this->setLayout(mainLayout);
		setWindowTitle("Штатно-должностной состав");

	//==========================================
	 table_rank_gr();

     connect(rank_view,SIGNAL(cellClicked(int,int)),this,SLOT(show_redaktor_rank(int,int)));

	 break;
		} 
case 23:{ //========== Персонал для групп ====================================
		 

		 break;
		}
case 24:{ //========== Персонал данные ====================================

     break;
	 }
case 25:{ //=========== Персонал данные ====================================

		 break;
        }
 case 26:{ //========== Персонал для СМИ ====================================

          break;
         }
	}
}

Add_elements_dialog::~Add_elements_dialog()
{

}


//=============================================================================
void Add_elements_dialog::fill_combobox(QComboBox *in_comboBox, QString table, QStringList name_column){
		in_comboBox->clear();
		QSqlQuery query;
		in_comboBox->addItem("-");
		int column_count=name_column.count();
		QString columns="id_" + table + ", ";
		for (int nc=0 ; nc<column_count ; nc++)
			{
			columns+=name_column.value(nc);
			if ((nc+1) != column_count)columns+=", ";
			}
		query.exec(QString("SELECT %1 FROM %2").arg(columns).arg(table));
		while (query.next()) 
		{
			QString text_combobox ="";
			for (int nc=0 ; nc < column_count; nc++)
				text_combobox+=query.value(nc+1).toString() + " ";
			in_comboBox->addItem(text_combobox,query.value(0).toInt());
		}
		query.clear();
	}

int Add_elements_dialog::insert_in_table(QString table,QMap<QString,QString> map,QString id){
		QString table_columns="";
		QString columns_values="";
		QMapIterator<QString, QString> i(map);
        while (i.hasNext()) {
             i.next();
             table_columns += i.key() + ",";

columns_values += "'" + i.value() + "',";
        }
		table_columns.chop(1);
		columns_values.chop(1);
		QSqlQuery query;
		bool res=query.exec(QString("insert into %1 (%2) values (%3) returning %4").arg(table).arg(table_columns).arg(columns_values).arg(id));
		if(res){
			while(query.next()){
			int last_id= query.value(0).toInt();
			query.clear();
			return last_id;
			}
		}else{
			query.clear();
			return 0;
		}
	}

//======================== добавление нового сми ============================
void Add_elements_dialog::add_smi()
{
		int id_type_smi=comboBox->itemData(comboBox->currentIndex()).toInt();
		int id_type_office_smi=comboBox_2->itemData(comboBox_2->currentIndex()).toInt();
		int id_type_broad=comboBox_3->itemData(comboBox_3->currentIndex()).toInt();
		int id_pozition=comboBox_4->itemData(comboBox_4->currentIndex()).toInt();
		int id_level=comboBox_5->itemData(comboBox_5->currentIndex()).toInt();
		int id_theme=comboBox_6->itemData(comboBox_6->currentIndex()).toInt();
		QString name_smi = lineEdit_name->text();
		QString power_smi = lineEdit_broad->text();
		QString site_smi = lineEdit_site->text();
		QString auditoria=lineEdit_aud->text();
		QString tirag=lineEdit_tirag->text();
		QString desc_smi = textEdit_desc->toPlainText();
		QString freq_smi = lineEdit_freq->text();
		

//        if(comboBox->currentIndex()==0){QMessageBox::StandardButton ret; ret = QMessageBox::critical (this,"Ошибка",("Выберите тип СМИ "),QMessageBox::Ok );return;}
//        else if(comboBox_2->currentIndex()==0){QMessageBox::StandardButton ret; ret = QMessageBox::critical (this,"Ошибка",("Выберите представительство СМИ "),QMessageBox::Ok );return;}
//        else if(comboBox_3->currentIndex()==0){QMessageBox::StandardButton ret; ret = QMessageBox::critical (this,"Ошибка",("Выберите способ вещания "),QMessageBox::Ok );return;}
//        else if (lineEdit_name->text() == "")
//        {QMessageBox::StandardButton ret; ret = QMessageBox::critical (this,"Ошибка",("Введите наименование СМИ "),QMessageBox::Ok );return;}
//        else if (lineEdit_broad->text() == "")
//        {QMessageBox::StandardButton ret; ret = QMessageBox::critical (this,"Ошибка",("Введите мощность сигнала "),QMessageBox::Ok );return;}
//        else if (textEdit_desc->toPlainText() == "")
//        {QMessageBox::StandardButton ret; ret = QMessageBox::critical (this,"Ошибка",("Введите описание СМИ "),QMessageBox::Ok );return;}
//        else if (lineEdit_site->text() == "")
//        {QMessageBox::StandardButton ret; ret = QMessageBox::critical (this,"Ошибка",("Введите сайт СМИ "),QMessageBox::Ok );return;}
//        else if (lineEdit_aud->text() == "")
//        {QMessageBox::StandardButton ret; ret = QMessageBox::critical (this,"Ошибка",("Введите аудиторию СМИ "),QMessageBox::Ok );return;}
//        else if (lineEdit_tirag->text() == "")
//        {QMessageBox::StandardButton ret; ret = QMessageBox::critical (this,"Ошибка",("Введите тираж СМИ "),QMessageBox::Ok );return;}
//        else if (lineEdit_freq->text() == "")
//        {QMessageBox::StandardButton ret; ret = QMessageBox::critical (this,"Ошибка",("Введите частоту вещания СМИ "),QMessageBox::Ok );return;}



		if (in_id_object<1) return;
		QMap<QString,QString> map;
		map.clear();
		map.insert("name_smi",name_smi);
		map.insert("power_broadcast_smi",power_smi);
		map.insert("site_smi",site_smi);
		map.insert("description_smi",desc_smi);
		map.insert("frequency_smi",freq_smi);
		map.insert("auditoria",auditoria);
		map.insert("tirag",tirag);
		map.insert("id_type_smi",QString::number(id_type_smi));
		map.insert("id_type_office_smi",QString::number(id_type_office_smi));
		map.insert("id_type_broadcast_smi",QString::number(id_type_broad));
		map.insert("id_position_smi",QString::number(id_pozition));
		map.insert("id_level_smi",QString::number(id_level));
		map.insert("id_theme_smi",QString::number(id_theme));
		

		int id_smi=insert_in_table("smi",map,"id_smi");
		if(id_smi>0){
			map.clear();
			map.insert("id_smi",QString::number(id_smi));
			map.insert("id_region",QString::number(in_id_object));
			int id_smi_region=insert_in_table("smi_region",map,"id_smi_region");
			if(id_smi_region>0){
                this->done(id_smi_region);
			}else{
				this->done(0);
			}
		}
}
 // ===================== добавление существующего сми =========================
void Add_elements_dialog::add_name_smi(){
	int id_smi=comboBox_10->itemData(comboBox_10->currentIndex()).toInt();
	if(comboBox_10->currentIndex()==0){QMessageBox::StandardButton ret; ret = QMessageBox::critical (this,"Ошибка",("Выберите СМИ "),QMessageBox::Ok );return;}
	if (in_id_object<1) return;
	QMap<QString,QString> map;
	map.clear();
	if(id_smi>0){
		map.clear();
		map.insert("id_smi",QString::number(id_smi));
		map.insert("id_region",QString::number(in_id_object));
		int id_smi_region=insert_in_table("smi_region",map,"id_smi_region");
		if(id_smi_region>0){
			this->done(id_smi_region); 
		}else{
			this->done(0);
		}
	}
}
//=============== добавление организации ===============================================
void Add_elements_dialog::add_groups(){

	int id_trend=comboBox->itemData(comboBox->currentIndex()).toInt();
	int id_sphere_groups=comboBox_2->itemData(comboBox_2->currentIndex()).toInt();
	int id_form_groups=comboBox_3->itemData(comboBox_3->currentIndex()).toInt();
	QString propa_groups=textEdit_propa->toPlainText();
	QString name_groups = lineEdit_name->text();
	int counte_groups = lineEdit_counte->text().toInt();
	QString founder = textEdit_founder->toPlainText();
	QString menegm = textEdit_menegm->toPlainText();
	QString office = textEdit_office->toPlainText();
	QString descr = textEdit_descr->toPlainText();
//	int x_object=lineEdit_x->text().toInt();
//	int y_object=lineEdit_y->text().toInt();
	int id_reg = in_id_object;

		QMap<QString,QString> map;
		map.clear();
//		map.insert("x_coordinates",QString::number(x_object));
//		map.insert("y_coordinates",QString::number(y_object));
//		int id_coordinates=insert_in_table("coordinates",map,"id_coordinates");

//		if(id_coordinates<1)return;
			
		if (in_id_object<1) return;
		map.clear();
		map.insert("name_groups",name_groups);
		map.insert("id_sphere_groups",QString::number(id_sphere_groups));
		map.insert("id_trend",QString::number(id_trend));
		map.insert("id_form_groups",QString::number(id_form_groups));
		map.insert("id_region",QString::number(id_reg));
//		map.insert("id_coordinates",QString::number(id_coordinates));
		map.insert("counte_groups",QString::number(counte_groups));
		map.insert("propaganda_groups",propa_groups);
		map.insert("founder_group",founder);
		map.insert("menegement_groups",menegm);
		map.insert("officce_groups",office);
		map.insert("description_groups",descr);
		
		int id_groups=insert_in_table("groups",map,"id_groups");
		if(id_groups>0){
			this->done(id_groups);
		}else{
			this->done(0);
		}
	
}
//================================= добавление ВФ ===========================
void Add_elements_dialog::add_ls(){

	int id_type_ls=comboBox->itemData(comboBox->currentIndex()).toInt();
	QString name_ls = lineEdit_name->text();
	int counte_ls = lineEdit_counte_ls->text().toInt();
	int counte_ls_bd = lineEdit_counte_ls_bd->text().toInt();
	bool enemy = checkbox_enemy->isChecked();
	
//	int x_object=lineEdit_x->text().toInt();
//	int y_object=lineEdit_y->text().toInt();
	
	int id_reg = in_id_object;

	QMap<QString,QString> map;
	map.clear();
//	map.insert("x_coordinates",QString::number(x_object));
//	map.insert("y_coordinates",QString::number(y_object));
//	int id_coordinates=insert_in_table("coordinates",map,"id_coordinates");

//	if(id_coordinates<1)return;
			
		if (in_id_object<1) return;
		map.clear();
		map.insert("name_ls",name_ls);
		map.insert("id_type_ls",QString::number(id_type_ls));
//		map.insert("id_coordinates",QString::number(id_coordinates));
		map.insert("counte_ls",QString::number(counte_ls));
		map.insert("counte_ls_bd",QString::number(counte_ls_bd));
		map.insert("id_region",QString::number(id_reg));
		map.insert("enimy_ls",QString::number(enemy));
		
		
		int id_ls=insert_in_table("ls",map,"id_ls");
		if(id_ls>0){
			this->done(id_ls);
		}else{
			this->done(0);
		}
	
	
}

void Add_elements_dialog::add_ls_parent(){

	int id_type_ls=comboBox->itemData(comboBox->currentIndex()).toInt();
	QString name_ls = lineEdit_name->text();
	int counte_ls = lineEdit_counte_ls->text().toInt();
	int counte_ls_bd = lineEdit_counte_ls_bd->text().toInt();
	int parent = in_id_object;
//	int x_object=lineEdit_x->text().toInt();
//	int y_object=lineEdit_y->text().toInt();
	bool enemy = checkbox_enemy->isChecked();
	
/*	QSqlQuery query_ls;
	query_ls.exec(QString("SELECT id_region FROM ls WHERE id_ls = %1").arg(parent));
	while (query_ls.next()){
		id_region = query_ls.value(0).toInt();
	}*/
	
	int id_region = 501; 
	QMap<QString,QString> map;
	map.clear();
//	map.insert("x_coordinates",QString::number(x_object));
//	map.insert("y_coordinates",QString::number(y_object));
//	int id_coordinates=insert_in_table("coordinates",map,"id_coordinates");

//	if(id_coordinates<1)return;
			
		if (in_id_object<1) return;
		map.clear();
		map.insert("name_ls",name_ls);
		map.insert("id_type_ls",QString::number(id_type_ls));
    //	map.insert("id_coordinates",QString::number(id_coordinates));
		map.insert("counte_ls",QString::number(counte_ls));
		map.insert("counte_ls_bd",QString::number(counte_ls_bd));
		map.insert("parent_ls",QString::number(parent));
		map.insert("id_region",QString::number(id_region));
		map.insert("enimy_ls",QString::number(enemy));

		int id_ls=insert_in_table("ls",map,"id_ls");
		if(id_ls>0){
			this->done(id_ls);
		}else{
			this->done(0);
		}
		
}

//================================ для национального состава ===================================
void Add_elements_dialog::show_redaktor(int row,int column){
	QSqlQuery query;
	if((column!=0) && (column!=5)){
		return;
	}
	 else if(column==0){
	
	int id_ls_nations = directs_view->item(row,1)->text().toInt();
	pravka_nations(id_ls_nations);
		return;
	}
	 else{    
		int id_ls_nations= directs_view->item(row,1)->text().toInt();
	    if(!query.exec(QString ("delete from ls_nations where id_ls_nations = %1").arg(id_ls_nations))){
		return;
		}
		directs_view->removeRow(row);
	 }
}
// =====================================================
void Add_elements_dialog::pravka_nations (int id_ls_nations){
		
	edit_dlg = new QDialog;
	edit_dlg->setWindowTitle("Редактирование национального состава");
	edit_dlg->setMinimumSize(QSize(600,400));

	label = new QLabel("Национальность:");
	comboBox = new QComboBox();
	comboBox->setFixedWidth(200);
    label->setBuddy(comboBox);
	
	label_2 = new QLabel("Процент:");
	lineEdit_counte = new QLineEdit;
	label_2->setBuddy(lineEdit_counte);

	label_3 = new QLabel("Коренное население:");
	QCheckBox *checkbox = new QCheckBox(" [-V- коренное население]", this);
	label_3->setBuddy(checkbox);
	 
	QSqlQuery query;
	QString str = QString("SELECT nat.id_nations, nat.name_nations, lsnat.id_nations, lsnat.persent_nations, lsnat.root, lsnat.id_ls_nations FROM nations nat, ls_nations lsnat where id_ls_nations = %1 and nat.id_nations = lsnat.id_nations").arg(id_ls_nations);
	if(!query.exec(str)){
	 return;
	}
    
	QSqlRecord rec = query.record();
	QString name_nations;
	float persent;
	int id_nations;	
	bool root;
	while(query.next()){	
		id_nations=query.value(rec.indexOf("id_nations")).toInt();
		name_nations=query.value(rec.indexOf("name_nations")).toString();
		persent=query.value(rec.indexOf("persent_nations")).toString().toFloat();
		root = query.value(rec.indexOf("root")).toBool();
	
		comboBox->clear();
        fill_combobox_nations(comboBox,id_nations) ;
		lineEdit_counte->setText(QString::number(persent));	
		checkbox->setChecked(root);
	}
		 
	 addButton = new QPushButton("Сохранить");
	 addButton->setDefault(true);
	 connect(addButton,SIGNAL(clicked()),edit_dlg,SLOT(accept()));
	 cancelButton = new QPushButton("Отмена");
	 connect(cancelButton,SIGNAL(clicked()),edit_dlg,SLOT(close()));

	 QHBoxLayout *buttonsLayout = new QHBoxLayout;
	 buttonsLayout->addStretch();
	 buttonsLayout->addWidget(addButton);
	 buttonsLayout->addWidget(cancelButton);

	 QHBoxLayout *topLeftLayout_29 = new QHBoxLayout;
	 topLeftLayout_29->addWidget(label);
	 QHBoxLayout *topLeftLayout_30 = new QHBoxLayout;
	 topLeftLayout_30->addWidget(comboBox);
	 QHBoxLayout *topLeftLayout_1 = new QHBoxLayout;
	 topLeftLayout_1->addWidget(label_2);
	 QHBoxLayout *topLeftLayout_2 = new QHBoxLayout;
	 topLeftLayout_2->addWidget(lineEdit_counte);	
	  QHBoxLayout *topLeftLayout_3 = new QHBoxLayout;
	 topLeftLayout_3->addWidget(label_3);
	 QHBoxLayout *topLeftLayout_4 = new QHBoxLayout;
	 topLeftLayout_4->addWidget(checkbox);


	 QVBoxLayout *leftLayout_31 = new QVBoxLayout;
	 leftLayout_31->addLayout(topLeftLayout_29);
	 QVBoxLayout *leftLayout_32 = new QVBoxLayout;
	 leftLayout_32->addLayout(topLeftLayout_30);
	 QVBoxLayout *leftLayout_1 = new QVBoxLayout;
	 leftLayout_1->addLayout(topLeftLayout_1);
	 QVBoxLayout *leftLayout_2 = new QVBoxLayout;
	 leftLayout_2->addLayout(topLeftLayout_2);
	 QVBoxLayout *leftLayout_3 = new QVBoxLayout;
	 leftLayout_3->addLayout(topLeftLayout_3);
	 QVBoxLayout *leftLayout_4 = new QVBoxLayout;
	 leftLayout_4->addLayout(topLeftLayout_4);

	 QGridLayout *mainLayout = new QGridLayout;
	 mainLayout->setSizeConstraint(QLayout::SetFixedSize);
	 mainLayout->addLayout(leftLayout_31, 0, 0);
	 mainLayout->addLayout(leftLayout_32, 0, 1);
	 mainLayout->addLayout(leftLayout_1, 1, 0);
	 mainLayout->addLayout(leftLayout_2, 1, 1);
	 mainLayout->addLayout(leftLayout_3, 2, 0);
	 mainLayout->addLayout(leftLayout_4, 2, 1);

	 mainLayout->addLayout(buttonsLayout, 3, 0, 1, 2);
	 edit_dlg->setLayout(mainLayout);
	if(edit_dlg->exec() == QDialog::Accepted){
	
	clear_table(directs_view);
	int id_nat = comboBox->itemData(comboBox->currentIndex()).toInt();
	float persent_nat = lineEdit_counte->text().toFloat();
	bool root_1 = checkbox->isChecked();	
						
	QSqlQuery query;
	QString str = QString("UPDATE ls_nations SET id_nations='%1',persent_nations='%2', root='%3' WHERE id_ls_nations=%4").arg(id_nat).arg(persent_nat).arg(root_1).arg(id_ls_nations);
	
	if(!query.exec(str)){
			 return;
		 }
	lineEdit_counte->clear();
	
	comboBox->setCurrentIndex(0);
	table_nations();
	table_nations_vf();
	table_nations_gr();
	
  }
	return; 
}
//================== заполнение таблицы национальность ==============================
void Add_elements_dialog::table_nations() {
directs_view->setColumnCount(6);
	directs_view->hideColumn(1);
	directs_view->setEditTriggers(QAbstractItemView::NoEditTriggers);
	
	QStringList header_list;
	header_list<<" "<<" "<<"Национальность"<<"Процент"<<"Коренное население"<<" ";
	
	directs_view->setHorizontalHeaderLabels(header_list);
	
	
	QSqlQuery query;
	QString str = QString("SELECT nat.name_nations, lsnat.persent_nations, lsnat.root, lsnat.id_ls_nations FROM nations nat, ls_nations lsnat WHERE lsnat.id_region = %1 AND nat.id_nations=lsnat.id_nations ORDER BY lsnat.persent_nations ").arg(in_id_object);
	if(!query.exec(str))
	{
	 return;
	}
    
	QSqlRecord rec = query.record();
	int id_ls_nations;
	bool code;
	int row = 0;
    QString type_name,nat_view;
	float short_name;
    QTableWidgetItem *item;
    QIcon icon(QString(":/Resources/edit_1.png"));
	while(query.next())
	{	
		id_ls_nations = query.value(rec.indexOf("id_ls_nations")).toInt();
		type_name = query.value(rec.indexOf("name_nations")).toString();
		short_name = query.value(rec.indexOf("persent_nations")).toString().toFloat();
		code = query.value(rec.indexOf("root")).toBool();
		
		if (code == true) nat_view = "коренное население";
		else nat_view = " ";
	
		directs_view->insertRow(row);

		item = new QTableWidgetItem(icon," Редактировать",0);
		directs_view->setItem(row,0,item);

		item = new QTableWidgetItem(QString::number(id_ls_nations));
		directs_view->setItem(row,1,item);

		item = new QTableWidgetItem(type_name);
		directs_view->setItem(row,2,item);

		item = new QTableWidgetItem(QString::number(short_name));
		directs_view->setItem(row,3,item);
	//	directs_view->sortItems(3,Qt::DescendingOrder);

		item = new QTableWidgetItem(nat_view);
		directs_view->setItem(row,4,item);

		QIcon icon2(QString(":/Resources/saturn_delete.png"));
		item = new QTableWidgetItem(icon2," Удалить",0);
		directs_view->setItem(row,5,item);
	}	
	
    row++;

   directs_view->resizeColumnsToContents();
}
void Add_elements_dialog::table_nations_vf() {
directs_view->setColumnCount(6);
	directs_view->hideColumn(1);
	directs_view->setEditTriggers(QAbstractItemView::NoEditTriggers);
	
	QStringList header_list;
	header_list<<" "<<" "<<"Национальность"<<"Процент"<<"Коренное население"<<" ";
	
	directs_view->setHorizontalHeaderLabels(header_list);
	
	QSqlQuery query;
	QString str = QString("SELECT nat.name_nations, lsnat.persent_nations, lsnat.root, lsnat.id_ls_nations FROM nations nat, ls_nations lsnat WHERE lsnat.id_ls = %1 AND nat.id_nations=lsnat.id_nations ORDER BY lsnat.persent_nations").arg(in_id_object);
	if(!query.exec(str))
	{
	 return;
	}
    
	QSqlRecord rec = query.record();
	int id_ls_nations;
	bool code;
	int row = 0;
    QString type_name,nat_view;
	float short_name;
    QTableWidgetItem *item;
    QIcon icon(QString(":/Resources/edit_1.png"));
    while(query.next())
	{	
		id_ls_nations = query.value(rec.indexOf("id_ls_nations")).toInt();
		type_name = query.value(rec.indexOf("name_nations")).toString();
		short_name = query.value(rec.indexOf("persent_nations")).toString().toFloat();
		code = query.value(rec.indexOf("root")).toBool();
		
		if (code == true) nat_view = "коренное население";
		else nat_view = " ";
	
		directs_view->insertRow(row);

		item = new QTableWidgetItem(icon," Редактировать",0);
		directs_view->setItem(row,0,item);

		item = new QTableWidgetItem(QString::number(id_ls_nations));
		directs_view->setItem(row,1,item);

		item = new QTableWidgetItem(type_name);
		directs_view->setItem(row,2,item);

		item = new QTableWidgetItem(QString::number(short_name));
		directs_view->setItem(row,3,item);
		//directs_view->sortItems(3,Qt::DescendingOrder);

		item = new QTableWidgetItem(nat_view);
		directs_view->setItem(row,4,item);

		QIcon icon2(QString(":/Resources/saturn_delete.png"));
		item = new QTableWidgetItem(icon2," Удалить",0);
		directs_view->setItem(row,5,item);
	}	
   row++;
   directs_view->resizeColumnsToContents();
}
void Add_elements_dialog::table_nations_gr() {
	directs_view->setColumnCount(6);
	directs_view->hideColumn(1);
	directs_view->setEditTriggers(QAbstractItemView::NoEditTriggers);
	
	QStringList header_list;
	header_list<<" "<<" "<<"Национальность"<<"Процент"<<"Коренное население"<<" ";
	
	directs_view->setHorizontalHeaderLabels(header_list);
	
	QSqlQuery query;
	QString str = QString("SELECT nat.name_nations, lsnat.persent_nations, lsnat.root, lsnat.id_ls_nations FROM nations nat, ls_nations lsnat WHERE lsnat.id_groups = %1 AND nat.id_nations=lsnat.id_nations ORDER BY lsnat.persent_nations").arg(in_id_object);
	if(!query.exec(str))
	{
	 return;
	}
    
	QSqlRecord rec = query.record();
	int id_ls_nations;
	bool code;
	int row = 0;
    QString type_name,nat_view;
	float short_name;
    QTableWidgetItem *item;
    QIcon icon(QString(":/Resources/edit_1.png"));
    while(query.next())
	{	
		id_ls_nations = query.value(rec.indexOf("id_ls_nations")).toInt();
		type_name = query.value(rec.indexOf("name_nations")).toString();
		short_name = query.value(rec.indexOf("persent_nations")).toString().toFloat();
		code = query.value(rec.indexOf("root")).toBool();
		
		if (code == true) nat_view = "коренное население";
		else nat_view = " ";
	
		directs_view->insertRow(row);

		item = new QTableWidgetItem(icon," Редактировать",0);
		directs_view->setItem(row,0,item);

		item = new QTableWidgetItem(QString::number(id_ls_nations));
		directs_view->setItem(row,1,item);

		item = new QTableWidgetItem(type_name);
		directs_view->setItem(row,2,item);

		item = new QTableWidgetItem(QString::number(short_name));
		directs_view->setItem(row,3,item);
		//directs_view->sortItems(3,Qt::DescendingOrder);

		item = new QTableWidgetItem(nat_view);
		directs_view->setItem(row,4,item);

		QIcon icon2(QString(":/Resources/saturn_delete.png"));
		item = new QTableWidgetItem(icon2," Удалить",0);
		directs_view->setItem(row,5,item);
	}	
   row++;
   directs_view->resizeColumnsToContents();
}
//======================= диалог добавления нации =====================
void Add_elements_dialog::add_nations_table_dlg()
{
	add_dlg = new QDialog;
	add_dlg->setWindowTitle("Добавлении нации");
	add_dlg->setMinimumSize(QSize(600,400));

	label = new QLabel("Национальность:");
	 comboBox = new QComboBox();
	 comboBox->setFixedWidth(200);
	 label->setBuddy(comboBox);

	
     QStringList column;
	 column.clear();
	 column<<"name_nations";
     fill_combobox(comboBox,"nations",column);

	 label_2 = new QLabel("Процент:");
	 lineEdit_counte = new QLineEdit;
	 label_2->setBuddy(lineEdit_counte);

	 label_3 = new QLabel("Коренное население:");
	 checkbox = new QCheckBox(" [-V- коренное население]",this);
	 label_3->setBuddy(checkbox);

	 addButton = new QPushButton("Добавить");
	 addButton->setDefault(true);
	 connect(addButton,SIGNAL(clicked()),this,SLOT(add_nations()));
	 cancelButton = new QPushButton("Отмена");
	 connect(cancelButton,SIGNAL(clicked()),add_dlg,SLOT(accept()));

	 QHBoxLayout *buttonsLayout = new QHBoxLayout;
	 buttonsLayout->addStretch();
	 buttonsLayout->addWidget(addButton);
	 buttonsLayout->addWidget(cancelButton);

	 QHBoxLayout *topLeftLayout_29 = new QHBoxLayout;
	 topLeftLayout_29->addWidget(label);
	 QHBoxLayout *topLeftLayout_30 = new QHBoxLayout;
	 topLeftLayout_30->addWidget(comboBox);
	 QHBoxLayout *topLeftLayout_1 = new QHBoxLayout;
	 topLeftLayout_1->addWidget(label_2);
	 QHBoxLayout *topLeftLayout_2 = new QHBoxLayout;
	 topLeftLayout_2->addWidget(lineEdit_counte);
	 QHBoxLayout *topLeftLayout_3 = new QHBoxLayout;
	 topLeftLayout_3->addWidget(label_3);
	 QHBoxLayout *topLeftLayout_4 = new QHBoxLayout;
	 topLeftLayout_4->addWidget(checkbox);

	 QVBoxLayout *leftLayout_31 = new QVBoxLayout;
	 leftLayout_31->addLayout(topLeftLayout_29);
	 QVBoxLayout *leftLayout_32 = new QVBoxLayout;
	 leftLayout_32->addLayout(topLeftLayout_30);
	 QVBoxLayout *leftLayout_1 = new QVBoxLayout;
	 leftLayout_1->addLayout(topLeftLayout_1);
	 QVBoxLayout *leftLayout_2 = new QVBoxLayout;
	 leftLayout_2->addLayout(topLeftLayout_2);
	 QVBoxLayout *leftLayout_3 = new QVBoxLayout;
	 leftLayout_3->addLayout(topLeftLayout_3);
	 QVBoxLayout *leftLayout_4 = new QVBoxLayout;
	 leftLayout_4->addLayout(topLeftLayout_4);

	 QGridLayout *mainLayout = new QGridLayout;
	 mainLayout->setSizeConstraint(QLayout::SetFixedSize);
	 mainLayout->addLayout(leftLayout_31, 0, 0);
	 mainLayout->addLayout(leftLayout_32, 0, 1);
	 mainLayout->addLayout(leftLayout_1, 1, 0);
	 mainLayout->addLayout(leftLayout_2, 1, 1);
	 mainLayout->addLayout(leftLayout_3, 2, 0);
	 mainLayout->addLayout(leftLayout_4, 2, 1);

	 mainLayout->addLayout(buttonsLayout, 3, 0, 1, 2);
	 add_dlg->setLayout(mainLayout);

	 if(add_dlg->exec() == QDialog::Accepted){
	 }
	
}

void Add_elements_dialog::add_nations()
{
		int id_nat=comboBox->itemData(comboBox->currentIndex()).toInt();
		float counte_ls = lineEdit_counte->text().toFloat();
		bool root = checkbox->isChecked();	
			
		if(comboBox->currentIndex()== 0){QMessageBox::StandardButton ret; ret = QMessageBox::warning (this,"Ошибка",("Национальность не выбрана "),QMessageBox::Ok);return;}
		else if (lineEdit_counte->text() == ""){QMessageBox::StandardButton ret; ret = QMessageBox::warning (this,"Ошибка",("Введите процент населения "),QMessageBox::Ok);return;}
		
		int id_reg = in_id_object;

		QMap<QString,QString> map;
				
		if (in_id_object<1) return;
		map.clear();
		map.insert("id_nations",QString::number(id_nat));
		map.insert("persent_nations",QString::number(counte_ls));
		map.insert("id_region",QString::number(id_reg));
		map.insert("root",QString::number(root));
				
		insert_in_table("ls_nations",map,"id_ls_nations");
		
		clear_table(directs_view);
		table_nations();
		add_dlg->close();
		
}
//============================================================
void Add_elements_dialog::add_nations_table_dlg_vf()
{
	add_dlg = new QDialog;
	add_dlg->setWindowTitle("Добавлении нации");
	add_dlg->setMinimumSize(QSize(600,400));

	label = new QLabel("Национальность:");
	 comboBox = new QComboBox();
	 comboBox->setFixedWidth(200);
	 label->setBuddy(comboBox);

	
     QStringList column;
	 column.clear();
	 column<<"name_nations";
     fill_combobox(comboBox,"nations",column);

	 label_2 = new QLabel("Процент:");
	 lineEdit_counte = new QLineEdit;
	 label_2->setBuddy(lineEdit_counte);

	 label_3 = new QLabel("Коренное население:");
	 checkbox = new QCheckBox(" [-V- коренное население]",this);
	 label_3->setBuddy(checkbox);

	 addButton = new QPushButton("Добавить");
	 addButton->setDefault(true);
	 connect(addButton,SIGNAL(clicked()),this,SLOT(add_nations_vf()));
	 cancelButton = new QPushButton("Отмена");
	 connect(cancelButton,SIGNAL(clicked()),add_dlg,SLOT(accept()));

	 QHBoxLayout *buttonsLayout = new QHBoxLayout;
	 buttonsLayout->addStretch();
	 buttonsLayout->addWidget(addButton);
	 buttonsLayout->addWidget(cancelButton);

	 QHBoxLayout *topLeftLayout_29 = new QHBoxLayout;
	 topLeftLayout_29->addWidget(label);
	 QHBoxLayout *topLeftLayout_30 = new QHBoxLayout;
	 topLeftLayout_30->addWidget(comboBox);
	 QHBoxLayout *topLeftLayout_1 = new QHBoxLayout;
	 topLeftLayout_1->addWidget(label_2);
	 QHBoxLayout *topLeftLayout_2 = new QHBoxLayout;
	 topLeftLayout_2->addWidget(lineEdit_counte);
	 QHBoxLayout *topLeftLayout_3 = new QHBoxLayout;
	 topLeftLayout_3->addWidget(label_3);
	 QHBoxLayout *topLeftLayout_4 = new QHBoxLayout;
	 topLeftLayout_4->addWidget(checkbox);

	 QVBoxLayout *leftLayout_31 = new QVBoxLayout;
	 leftLayout_31->addLayout(topLeftLayout_29);
	 QVBoxLayout *leftLayout_32 = new QVBoxLayout;
	 leftLayout_32->addLayout(topLeftLayout_30);
	 QVBoxLayout *leftLayout_1 = new QVBoxLayout;
	 leftLayout_1->addLayout(topLeftLayout_1);
	 QVBoxLayout *leftLayout_2 = new QVBoxLayout;
	 leftLayout_2->addLayout(topLeftLayout_2);
	 QVBoxLayout *leftLayout_3 = new QVBoxLayout;
	 leftLayout_3->addLayout(topLeftLayout_3);
	 QVBoxLayout *leftLayout_4 = new QVBoxLayout;
	 leftLayout_4->addLayout(topLeftLayout_4);

	 QGridLayout *mainLayout = new QGridLayout;
	 mainLayout->setSizeConstraint(QLayout::SetFixedSize);
	 mainLayout->addLayout(leftLayout_31, 0, 0);
	 mainLayout->addLayout(leftLayout_32, 0, 1);
	 mainLayout->addLayout(leftLayout_1, 1, 0);
	 mainLayout->addLayout(leftLayout_2, 1, 1);
	 mainLayout->addLayout(leftLayout_3, 2, 0);
	 mainLayout->addLayout(leftLayout_4, 2, 1);

	 mainLayout->addLayout(buttonsLayout, 3, 0, 1, 2);
	 add_dlg->setLayout(mainLayout);

	 if(add_dlg->exec() == QDialog::Accepted){
	 }
	
}

void Add_elements_dialog::add_nations_vf()
{
		int id_nat=comboBox->itemData(comboBox->currentIndex()).toInt();
		float counte_ls = lineEdit_counte->text().toFloat();
		bool root = checkbox->isChecked();	
			
		if(comboBox->currentIndex()== 0){QMessageBox::StandardButton ret; ret = QMessageBox::warning (this,"Ошибка",("Национальность не выбрана "),QMessageBox::Ok);return;}
		else if (lineEdit_counte->text() == ""){QMessageBox::StandardButton ret; ret = QMessageBox::warning (this,"Ошибка",("Введите процент населения "),QMessageBox::Ok);return;}
		
		int id_ls = in_id_object;

		QMap<QString,QString> map;
				
		if (in_id_object<1) return;
		map.clear();
		map.insert("id_nations",QString::number(id_nat));
		map.insert("persent_nations",QString::number(counte_ls));
		map.insert("id_ls",QString::number(id_ls));
		map.insert("root",QString::number(root));
				
		insert_in_table("ls_nations",map,"id_ls_nations");
		
		clear_table(directs_view);
		table_nations_vf();
		add_dlg->close();
		
}
//============================================================
void Add_elements_dialog::add_nations_table_dlg_gr()
{
	add_dlg = new QDialog;
	add_dlg->setWindowTitle("Добавлении нации");
	add_dlg->setMinimumSize(QSize(600,400));

	label = new QLabel("Национальность:");
	 comboBox = new QComboBox();
	 comboBox->setFixedWidth(200);
	 label->setBuddy(comboBox);

	
     QStringList column;
	 column.clear();
	 column<<"name_nations";
     fill_combobox(comboBox,"nations",column);

	 label_2 = new QLabel("Процент:");
	 lineEdit_counte = new QLineEdit;
	 label_2->setBuddy(lineEdit_counte);

	 label_3 = new QLabel("Коренное население:");
	 checkbox = new QCheckBox(" [-V- коренное население]",this);
	 label_3->setBuddy(checkbox);

	 addButton = new QPushButton("Добавить");
	 addButton->setDefault(true);
	 connect(addButton,SIGNAL(clicked()),this,SLOT(add_nations_gr()));
	 cancelButton = new QPushButton("Отмена");
	 connect(cancelButton,SIGNAL(clicked()),add_dlg,SLOT(accept()));

	 QHBoxLayout *buttonsLayout = new QHBoxLayout;
	 buttonsLayout->addStretch();
	 buttonsLayout->addWidget(addButton);
	 buttonsLayout->addWidget(cancelButton);

	 QHBoxLayout *topLeftLayout_29 = new QHBoxLayout;
	 topLeftLayout_29->addWidget(label);
	 QHBoxLayout *topLeftLayout_30 = new QHBoxLayout;
	 topLeftLayout_30->addWidget(comboBox);
	 QHBoxLayout *topLeftLayout_1 = new QHBoxLayout;
	 topLeftLayout_1->addWidget(label_2);
	 QHBoxLayout *topLeftLayout_2 = new QHBoxLayout;
	 topLeftLayout_2->addWidget(lineEdit_counte);
	 QHBoxLayout *topLeftLayout_3 = new QHBoxLayout;
	 topLeftLayout_3->addWidget(label_3);
	 QHBoxLayout *topLeftLayout_4 = new QHBoxLayout;
	 topLeftLayout_4->addWidget(checkbox);

	 QVBoxLayout *leftLayout_31 = new QVBoxLayout;
	 leftLayout_31->addLayout(topLeftLayout_29);
	 QVBoxLayout *leftLayout_32 = new QVBoxLayout;
	 leftLayout_32->addLayout(topLeftLayout_30);
	 QVBoxLayout *leftLayout_1 = new QVBoxLayout;
	 leftLayout_1->addLayout(topLeftLayout_1);
	 QVBoxLayout *leftLayout_2 = new QVBoxLayout;
	 leftLayout_2->addLayout(topLeftLayout_2);
	 QVBoxLayout *leftLayout_3 = new QVBoxLayout;
	 leftLayout_3->addLayout(topLeftLayout_3);
	 QVBoxLayout *leftLayout_4 = new QVBoxLayout;
	 leftLayout_4->addLayout(topLeftLayout_4);

	 QGridLayout *mainLayout = new QGridLayout;
	 mainLayout->setSizeConstraint(QLayout::SetFixedSize);
	 mainLayout->addLayout(leftLayout_31, 0, 0);
	 mainLayout->addLayout(leftLayout_32, 0, 1);
	 mainLayout->addLayout(leftLayout_1, 1, 0);
	 mainLayout->addLayout(leftLayout_2, 1, 1);
	 mainLayout->addLayout(leftLayout_3, 2, 0);
	 mainLayout->addLayout(leftLayout_4, 2, 1);

	 mainLayout->addLayout(buttonsLayout, 3, 0, 1, 2);
	 add_dlg->setLayout(mainLayout);

	 if(add_dlg->exec() == QDialog::Accepted){
	 }
	
}

void Add_elements_dialog::add_nations_gr()
{
		int id_nat=comboBox->itemData(comboBox->currentIndex()).toInt();
		float counte_ls = lineEdit_counte->text().toFloat();
		bool root = checkbox->isChecked();	
			
		if(comboBox->currentIndex()== 0){QMessageBox::StandardButton ret; ret = QMessageBox::warning (this,"Ошибка",("Национальность не выбрана "),QMessageBox::Ok);return;}
		else if (lineEdit_counte->text() == ""){QMessageBox::StandardButton ret; ret = QMessageBox::warning (this,"Ошибка",("Введите процент населения "),QMessageBox::Ok);return;}
		
		int id_gr = in_id_object;

		QMap<QString,QString> map;
				
		if (in_id_object<1) return;
		map.clear();
		map.insert("id_nations",QString::number(id_nat));
		map.insert("persent_nations",QString::number(counte_ls));
		map.insert("id_groups",QString::number(id_gr));
		map.insert("root",QString::number(root));
				
		insert_in_table("ls_nations",map,"id_ls_nations");
		
		clear_table(directs_view);
		table_nations_gr();
		add_dlg->close();
		
}
//================================== очистка таблицы ===================================
void Add_elements_dialog::clear_table(QTableWidget *table)
{
	int row_count = table->rowCount();
	
	while(row_count>=0)
	{
	table->removeRow(row_count);
	row_count--;
	}
	return;
}
//======================================================================================
void Add_elements_dialog::fill_combobox_nations(QComboBox *comboBox,int current_index)
{
	QSqlQuery query;									
	query.exec("SELECT nat.id_nations, nat.name_nations FROM nations nat"); 
	int ci_3=0;
	while (query.next())												
	{
		QString blok = query.value(1).toString();				
		int id_blok=query.value(0).toInt();							
		comboBox->addItem(blok,id_blok);	
		if (current_index==id_blok) ci_3=comboBox->count()-1;
   	}

	comboBox->setCurrentIndex(ci_3);
}
//================== заполнение таблицы религия ==============================
void Add_elements_dialog::table_confess() {
	confess_view->setColumnCount(5);
	confess_view->hideColumn(1);
	confess_view->setEditTriggers(QAbstractItemView::NoEditTriggers);
	
	QStringList header_list;
	header_list<<" "<<" "<<"Религия"<<"Процент"<<" ";
	confess_view->setHorizontalHeaderLabels(header_list);
	
	QSqlQuery query;
	QString str = QString("SELECT conf.name_confessions, lsconf.persent_confessions, lsconf.id_ls_confessions FROM confessions conf, ls_confessions lsconf WHERE lsconf.id_region = %1 AND conf.id_confessions=lsconf.id_confessions ORDER BY lsconf.persent_confessions").arg(in_id_object);
	if(!query.exec(str))
	{
	 return;
	}
    
	QSqlRecord rec = query.record();
	int id_ls_conf;
	bool code;
	int row = 0;
    QString type_name;
	float persent_name;
    QTableWidgetItem *item;
    QIcon icon(QString(":/Resources/edit_1.png"));
    while(query.next())
	{	
		id_ls_conf = query.value(rec.indexOf("id_ls_confessions")).toInt();
		type_name = query.value(rec.indexOf("name_confessions")).toString();
		persent_name = query.value(rec.indexOf("persent_confessions")).toString().toFloat();
			
		confess_view->insertRow(row);

		item = new QTableWidgetItem(icon," Редактировать",0);
		confess_view->setItem(row,0,item);

		item = new QTableWidgetItem(QString::number(id_ls_conf));
		confess_view->setItem(row,1,item);

		item = new QTableWidgetItem(type_name);
		confess_view->setItem(row,2,item);

		item = new QTableWidgetItem(QString::number(persent_name));
		confess_view->setItem(row,3,item);
	//	confess_view->sortItems(3,Qt::DescendingOrder);

		QIcon icon2(QString(":/Resources/saturn_delete.png"));
		item = new QTableWidgetItem(icon2," Удалить",0);
		confess_view->setItem(row,4,item);
	}	
   row++;
   confess_view->resizeColumnsToContents();
}
void Add_elements_dialog::table_confess_conf() {
	confess_view->setColumnCount(5);
	confess_view->hideColumn(1);
	confess_view->setEditTriggers(QAbstractItemView::NoEditTriggers);
	
	QStringList header_list;
	header_list<<" "<<" "<<"Религия"<<"Процент"<<" ";
	confess_view->setHorizontalHeaderLabels(header_list);
	
	QSqlQuery query;
	QString str = QString("SELECT conf.name_confessions, lsconf.persent_confessions, lsconf.id_ls_confessions FROM confessions conf, ls_confessions lsconf WHERE lsconf.id_ls = %1 AND conf.id_confessions=lsconf.id_confessions ORDER BY lsconf.persent_confessions").arg(in_id_object);
	if(!query.exec(str))
	{
	 return;
	}
    
	QSqlRecord rec = query.record();
	int id_ls_conf;
	bool code;
	int row = 0;
    QString type_name;
	float persent_name;
    QTableWidgetItem *item;
    QIcon icon(QString(":/Resources/edit_1.png"));
    while(query.next())
	{	
		id_ls_conf = query.value(rec.indexOf("id_ls_confessions")).toInt();
		type_name = query.value(rec.indexOf("name_confessions")).toString();
		persent_name = query.value(rec.indexOf("persent_confessions")).toString().toFloat();
			
		confess_view->insertRow(row);

		item = new QTableWidgetItem(icon," Редактировать",0);
		confess_view->setItem(row,0,item);

		item = new QTableWidgetItem(QString::number(id_ls_conf));
		confess_view->setItem(row,1,item);

		item = new QTableWidgetItem(type_name);
		confess_view->setItem(row,2,item);

		item = new QTableWidgetItem(QString::number(persent_name));
		confess_view->setItem(row,3,item);
	//	confess_view->sortItems(3,Qt::DescendingOrder);

		QIcon icon2(QString(":/Resources/saturn_delete.png"));
		item = new QTableWidgetItem(icon2," Удалить",0);
		confess_view->setItem(row,4,item);
	}	
   row++;
   confess_view->resizeColumnsToContents();
}
void Add_elements_dialog::table_confess_gr() {
	confess_view->setColumnCount(5);
	confess_view->hideColumn(1);
	confess_view->setEditTriggers(QAbstractItemView::NoEditTriggers);
	
	QStringList header_list;
	header_list<<" "<<" "<<"Религия"<<"Процент"<<" ";
	confess_view->setHorizontalHeaderLabels(header_list);
	
	QSqlQuery query;
	QString str = QString("SELECT conf.name_confessions, lsconf.persent_confessions, lsconf.id_ls_confessions FROM confessions conf, ls_confessions lsconf WHERE lsconf.id_groups = %1 AND conf.id_confessions=lsconf.id_confessions ORDER BY lsconf.persent_confessions").arg(in_id_object);
	if(!query.exec(str))
	{
	 return;
	}
    
	QSqlRecord rec = query.record();
	int id_ls_conf;
	bool code;
	int row = 0;
    QString type_name;
	float persent_name;
    QTableWidgetItem *item;
    QIcon icon(QString(":/Resources/edit_1.png"));
    while(query.next())
	{	
		id_ls_conf = query.value(rec.indexOf("id_ls_confessions")).toInt();
		type_name = query.value(rec.indexOf("name_confessions")).toString();
		persent_name = query.value(rec.indexOf("persent_confessions")).toString().toFloat();
			
		confess_view->insertRow(row);

		item = new QTableWidgetItem(icon," Редактировать",0);
		confess_view->setItem(row,0,item);

		item = new QTableWidgetItem(QString::number(id_ls_conf));
		confess_view->setItem(row,1,item);

		item = new QTableWidgetItem(type_name);
		confess_view->setItem(row,2,item);

		item = new QTableWidgetItem(QString::number(persent_name));
		confess_view->setItem(row,3,item);
	//	confess_view->sortItems(3,Qt::DescendingOrder);

		QIcon icon2(QString(":/Resources/saturn_delete.png"));
		item = new QTableWidgetItem(icon2," Удалить",0);
		confess_view->setItem(row,4,item);
	}	
   row++;
   confess_view->resizeColumnsToContents();
}
//======================= диалог добавления религии =====================
void Add_elements_dialog::add_confess_table_dlg()
{
	add_dlg = new QDialog;
	add_dlg->setWindowTitle("Добавлении религии");
	add_dlg->setMinimumSize(QSize(600,400));

	label = new QLabel("Религия:");
	 comboBox = new QComboBox();
	 comboBox->setFixedWidth(200);
	 label->setBuddy(comboBox);

	
     QStringList column;
	 column.clear();
	 column<<"name_confessions";
     fill_combobox(comboBox,"confessions",column);

	 label_2 = new QLabel("Процент:");
	 lineEdit_counte = new QLineEdit;
	 label_2->setBuddy(lineEdit_counte);

	 addButton = new QPushButton("Добавить");
	 addButton->setDefault(true);
	 connect(addButton,SIGNAL(clicked()),this,SLOT(add_confess()));
	 cancelButton = new QPushButton("Отмена");
	 connect(cancelButton,SIGNAL(clicked()),add_dlg,SLOT(accept()));

	 QHBoxLayout *buttonsLayout = new QHBoxLayout;
	 buttonsLayout->addStretch();
	 buttonsLayout->addWidget(addButton);
	 buttonsLayout->addWidget(cancelButton);

	 QHBoxLayout *topLeftLayout_29 = new QHBoxLayout;
	 topLeftLayout_29->addWidget(label);
	 QHBoxLayout *topLeftLayout_30 = new QHBoxLayout;
	 topLeftLayout_30->addWidget(comboBox);
	 QHBoxLayout *topLeftLayout_1 = new QHBoxLayout;
	 topLeftLayout_1->addWidget(label_2);
	 QHBoxLayout *topLeftLayout_2 = new QHBoxLayout;
	 topLeftLayout_2->addWidget(lineEdit_counte);

	 QVBoxLayout *leftLayout_31 = new QVBoxLayout;
	 leftLayout_31->addLayout(topLeftLayout_29);
	 QVBoxLayout *leftLayout_32 = new QVBoxLayout;
	 leftLayout_32->addLayout(topLeftLayout_30);
	 QVBoxLayout *leftLayout_1 = new QVBoxLayout;
	 leftLayout_1->addLayout(topLeftLayout_1);
	 QVBoxLayout *leftLayout_2 = new QVBoxLayout;
	 leftLayout_2->addLayout(topLeftLayout_2);
	
	 QGridLayout *mainLayout = new QGridLayout;
	 mainLayout->setSizeConstraint(QLayout::SetFixedSize);
	 mainLayout->addLayout(leftLayout_31, 0, 0);
	 mainLayout->addLayout(leftLayout_32, 0, 1);
	 mainLayout->addLayout(leftLayout_1, 1, 0);
	 mainLayout->addLayout(leftLayout_2, 1, 1);

	 mainLayout->addLayout(buttonsLayout, 2, 0, 1, 2);
	 add_dlg->setLayout(mainLayout);

	 if(add_dlg->exec() == QDialog::Accepted){
	 }
	
}

void Add_elements_dialog::add_confess()
{
		int id_conf=comboBox->itemData(comboBox->currentIndex()).toInt();
		float counte_conf = lineEdit_counte->text().toFloat();
			
		if(comboBox->currentIndex()== 0){QMessageBox::StandardButton ret; ret = QMessageBox::warning (this,"Ошибка",("Религия не выбрана "),QMessageBox::Ok);return;}
		else if (lineEdit_counte->text() == ""){QMessageBox::StandardButton ret; ret = QMessageBox::warning (this,"Ошибка",("Введите процент "),QMessageBox::Ok);return;}
		
		int id_reg = in_id_object;

		QMap<QString,QString> map;
				
		if (in_id_object<1) return;
		map.clear();
		map.insert("id_confessions",QString::number(id_conf));
		map.insert("persent_confessions",QString::number(counte_conf));
		map.insert("id_region",QString::number(id_reg));
				
		insert_in_table("ls_confessions",map,"id_ls_confessions");
		
		clear_table(confess_view);
		table_confess();
		add_dlg->close();
}
//===============================================================
void Add_elements_dialog::add_confess_table_dlg_gr()
{
	add_dlg = new QDialog;
	add_dlg->setWindowTitle("Добавлении религии");
	add_dlg->setMinimumSize(QSize(600,400));

	label = new QLabel("Религия:");
	 comboBox = new QComboBox();
	 comboBox->setFixedWidth(200);
	 label->setBuddy(comboBox);

	
     QStringList column;
	 column.clear();
	 column<<"name_confessions";
     fill_combobox(comboBox,"confessions",column);

	 label_2 = new QLabel("Процент:");
	 lineEdit_counte = new QLineEdit;
	 label_2->setBuddy(lineEdit_counte);

	 addButton = new QPushButton("Добавить");
	 addButton->setDefault(true);
	 connect(addButton,SIGNAL(clicked()),this,SLOT(add_confess_gr()));
	 cancelButton = new QPushButton("Отмена");
	 connect(cancelButton,SIGNAL(clicked()),add_dlg,SLOT(accept()));

	 QHBoxLayout *buttonsLayout = new QHBoxLayout;
	 buttonsLayout->addStretch();
	 buttonsLayout->addWidget(addButton);
	 buttonsLayout->addWidget(cancelButton);

	 QHBoxLayout *topLeftLayout_29 = new QHBoxLayout;
	 topLeftLayout_29->addWidget(label);
	 QHBoxLayout *topLeftLayout_30 = new QHBoxLayout;
	 topLeftLayout_30->addWidget(comboBox);
	 QHBoxLayout *topLeftLayout_1 = new QHBoxLayout;
	 topLeftLayout_1->addWidget(label_2);
	 QHBoxLayout *topLeftLayout_2 = new QHBoxLayout;
	 topLeftLayout_2->addWidget(lineEdit_counte);

	 QVBoxLayout *leftLayout_31 = new QVBoxLayout;
	 leftLayout_31->addLayout(topLeftLayout_29);
	 QVBoxLayout *leftLayout_32 = new QVBoxLayout;
	 leftLayout_32->addLayout(topLeftLayout_30);
	 QVBoxLayout *leftLayout_1 = new QVBoxLayout;
	 leftLayout_1->addLayout(topLeftLayout_1);
	 QVBoxLayout *leftLayout_2 = new QVBoxLayout;
	 leftLayout_2->addLayout(topLeftLayout_2);
	
	 QGridLayout *mainLayout = new QGridLayout;
	 mainLayout->setSizeConstraint(QLayout::SetFixedSize);
	 mainLayout->addLayout(leftLayout_31, 0, 0);
	 mainLayout->addLayout(leftLayout_32, 0, 1);
	 mainLayout->addLayout(leftLayout_1, 1, 0);
	 mainLayout->addLayout(leftLayout_2, 1, 1);

	 mainLayout->addLayout(buttonsLayout, 2, 0, 1, 2);
	 add_dlg->setLayout(mainLayout);

	 if(add_dlg->exec() == QDialog::Accepted){
	 }
	
}

void Add_elements_dialog::add_confess_gr()
{
		int id_conf=comboBox->itemData(comboBox->currentIndex()).toInt();
		float counte_conf = lineEdit_counte->text().toFloat();
			
		if(comboBox->currentIndex()== 0){QMessageBox::StandardButton ret; ret = QMessageBox::warning (this,"Ошибка",("Религия не выбрана "),QMessageBox::Ok);return;}
		else if (lineEdit_counte->text() == ""){QMessageBox::StandardButton ret; ret = QMessageBox::warning (this,"Ошибка",("Введите процент "),QMessageBox::Ok);return;}
		
		int id_gr = in_id_object;

		QMap<QString,QString> map;
				
		if (in_id_object<1) return;
		map.clear();
		map.insert("id_confessions",QString::number(id_conf));
		map.insert("persent_confessions",QString::number(counte_conf));
		map.insert("id_groups",QString::number(id_gr));
				
		insert_in_table("ls_confessions",map,"id_ls_confessions");
		
		clear_table(confess_view);
		table_confess_gr();
		add_dlg->close();
}
//===============================================================
void Add_elements_dialog::add_confess_table_dlg_conf()
{
	add_dlg = new QDialog;
	add_dlg->setWindowTitle("Добавлении религии");
	add_dlg->setMinimumSize(QSize(600,400));

	label = new QLabel("Религия:");
	 comboBox = new QComboBox();
	 comboBox->setFixedWidth(200);
	 label->setBuddy(comboBox);

	
     QStringList column;
	 column.clear();
	 column<<"name_confessions";
     fill_combobox(comboBox,"confessions",column);

	 label_2 = new QLabel("Процент:");
	 lineEdit_counte = new QLineEdit;
	 label_2->setBuddy(lineEdit_counte);

	 addButton = new QPushButton("Добавить");
	 addButton->setDefault(true);
	 connect(addButton,SIGNAL(clicked()),this,SLOT(add_confess_conf()));
	 cancelButton = new QPushButton("Отмена");
	 connect(cancelButton,SIGNAL(clicked()),add_dlg,SLOT(accept()));

	 QHBoxLayout *buttonsLayout = new QHBoxLayout;
	 buttonsLayout->addStretch();
	 buttonsLayout->addWidget(addButton);
	 buttonsLayout->addWidget(cancelButton);

	 QHBoxLayout *topLeftLayout_29 = new QHBoxLayout;
	 topLeftLayout_29->addWidget(label);
	 QHBoxLayout *topLeftLayout_30 = new QHBoxLayout;
	 topLeftLayout_30->addWidget(comboBox);
	 QHBoxLayout *topLeftLayout_1 = new QHBoxLayout;
	 topLeftLayout_1->addWidget(label_2);
	 QHBoxLayout *topLeftLayout_2 = new QHBoxLayout;
	 topLeftLayout_2->addWidget(lineEdit_counte);

	 QVBoxLayout *leftLayout_31 = new QVBoxLayout;
	 leftLayout_31->addLayout(topLeftLayout_29);
	 QVBoxLayout *leftLayout_32 = new QVBoxLayout;
	 leftLayout_32->addLayout(topLeftLayout_30);
	 QVBoxLayout *leftLayout_1 = new QVBoxLayout;
	 leftLayout_1->addLayout(topLeftLayout_1);
	 QVBoxLayout *leftLayout_2 = new QVBoxLayout;
	 leftLayout_2->addLayout(topLeftLayout_2);
	
	 QGridLayout *mainLayout = new QGridLayout;
	 mainLayout->setSizeConstraint(QLayout::SetFixedSize);
	 mainLayout->addLayout(leftLayout_31, 0, 0);
	 mainLayout->addLayout(leftLayout_32, 0, 1);
	 mainLayout->addLayout(leftLayout_1, 1, 0);
	 mainLayout->addLayout(leftLayout_2, 1, 1);

	 mainLayout->addLayout(buttonsLayout, 2, 0, 1, 2);
	 add_dlg->setLayout(mainLayout);

	 if(add_dlg->exec() == QDialog::Accepted){
	 }
	
}

void Add_elements_dialog::add_confess_conf()
{
		int id_conf=comboBox->itemData(comboBox->currentIndex()).toInt();
		float counte_conf = lineEdit_counte->text().toFloat();
			
		if(comboBox->currentIndex()== 0){QMessageBox::StandardButton ret; ret = QMessageBox::warning (this,"Ошибка",("Религия не выбрана "),QMessageBox::Ok);return;}
		else if (lineEdit_counte->text() == ""){QMessageBox::StandardButton ret; ret = QMessageBox::warning (this,"Ошибка",("Введите процент "),QMessageBox::Ok);return;}
		
		int id_ls = in_id_object;

		QMap<QString,QString> map;
				
		if (in_id_object<1) return;
		map.clear();
		map.insert("id_confessions",QString::number(id_conf));
		map.insert("persent_confessions",QString::number(counte_conf));
		map.insert("id_ls",QString::number(id_ls));
				
		insert_in_table("ls_confessions",map,"id_ls_confessions");
		
		clear_table(confess_view);
		table_confess_conf();
		add_dlg->close();
}
//=========================== правка религия =====================================
void Add_elements_dialog::show_redaktor_conf(int row,int column){
	QSqlQuery query;
	if((column!=0) && (column!=4)){
		return;
	}
	 else if(column==0){
	
	int id_ls_conf = confess_view->item(row,1)->text().toInt();
	pravka_conf(id_ls_conf);
		return;
	}
	 else{    
		int id_ls_conf= confess_view->item(row,1)->text().toInt();
	    if(!query.exec(QString ("delete from ls_confessions where id_ls_confessions = %1").arg(id_ls_conf))){
		return;
		}
		confess_view->removeRow(row);
	 }
}
void Add_elements_dialog::pravka_conf (int id_ls_conf){
		
	edit_dlg = new QDialog;
	edit_dlg->setWindowTitle("Редактирование данных Религия");
	edit_dlg->setMinimumSize(QSize(600,400));

	label = new QLabel("Религия:");
	comboBox = new QComboBox();
	comboBox->setFixedWidth(200);
    label->setBuddy(comboBox);
	
	label_2 = new QLabel("Процент:");
	lineEdit_counte = new QLineEdit;
	label_2->setBuddy(lineEdit_counte);

	QSqlQuery query;
	QString str = QString("SELECT conf.id_confessions, conf.name_confessions, lsconf.id_confessions, lsconf.persent_confessions, lsconf.id_ls_confessions FROM confessions conf, ls_confessions lsconf where id_ls_confessions = %1 and conf.id_confessions = lsconf.id_confessions").arg(id_ls_conf);
	if(!query.exec(str)){
	 return;
	}
    
	QSqlRecord rec = query.record();
	QString name_conf;
	float persent;
	int id_conf;	
	while(query.next()){	
		id_conf=query.value(rec.indexOf("id_confessions")).toInt();
		name_conf=query.value(rec.indexOf("name_confessions")).toString();
		persent=query.value(rec.indexOf("persent_confessions")).toString().toFloat();
	
		comboBox->clear();
		fill_combobox_conf(comboBox,id_conf) ; 
		lineEdit_counte->setText(QString::number(persent));	
	}
		 
	 addButton = new QPushButton("Сохранить");
	 addButton->setDefault(true);
	 connect(addButton,SIGNAL(clicked()),edit_dlg,SLOT(accept()));
	 cancelButton = new QPushButton("Отмена");
	 connect(cancelButton,SIGNAL(clicked()),edit_dlg,SLOT(close()));

	 QHBoxLayout *buttonsLayout = new QHBoxLayout;
	 buttonsLayout->addStretch();
	 buttonsLayout->addWidget(addButton);
	 buttonsLayout->addWidget(cancelButton);

	 QHBoxLayout *topLeftLayout_29 = new QHBoxLayout;
	 topLeftLayout_29->addWidget(label);
	 QHBoxLayout *topLeftLayout_30 = new QHBoxLayout;
	 topLeftLayout_30->addWidget(comboBox);
	 QHBoxLayout *topLeftLayout_1 = new QHBoxLayout;
	 topLeftLayout_1->addWidget(label_2);
	 QHBoxLayout *topLeftLayout_2 = new QHBoxLayout;
	 topLeftLayout_2->addWidget(lineEdit_counte);	

	 QVBoxLayout *leftLayout_31 = new QVBoxLayout;
	 leftLayout_31->addLayout(topLeftLayout_29);
	 QVBoxLayout *leftLayout_32 = new QVBoxLayout;
	 leftLayout_32->addLayout(topLeftLayout_30);
	 QVBoxLayout *leftLayout_1 = new QVBoxLayout;
	 leftLayout_1->addLayout(topLeftLayout_1);
	 QVBoxLayout *leftLayout_2 = new QVBoxLayout;
	 leftLayout_2->addLayout(topLeftLayout_2);

	 QGridLayout *mainLayout = new QGridLayout;
	 mainLayout->setSizeConstraint(QLayout::SetFixedSize);
	 mainLayout->addLayout(leftLayout_31, 0, 0);
	 mainLayout->addLayout(leftLayout_32, 0, 1);
	 mainLayout->addLayout(leftLayout_1, 1, 0);
	 mainLayout->addLayout(leftLayout_2, 1, 1);

	 mainLayout->addLayout(buttonsLayout, 2, 0, 1, 2);
	 edit_dlg->setLayout(mainLayout);
	if(edit_dlg->exec() == QDialog::Accepted){
	
	clear_table(confess_view);
	int id_confes = comboBox->itemData(comboBox->currentIndex()).toInt();
	float persent_conf = lineEdit_counte->text().toFloat();
						
	QSqlQuery query;
	QString str = QString("UPDATE ls_confessions SET id_confessions='%1',persent_confessions='%2' WHERE id_ls_confessions=%3").arg(id_confes).arg(persent_conf).arg(id_ls_conf);
	
	if(!query.exec(str)){
			 return;
		 }
	lineEdit_counte->clear();
	
	comboBox->setCurrentIndex(0);
	table_confess();
	table_confess_conf();
	table_confess_gr();
	
  }
	return; 
}
void Add_elements_dialog::fill_combobox_conf(QComboBox *comboBox,int current_index)
{
	QSqlQuery query;									
	query.exec("SELECT id_confessions, name_confessions FROM confessions"); 
	int ci_3=0;
	while (query.next())												
	{
		QString name_c = query.value(1).toString();				
		int id_c=query.value(0).toInt();							
		comboBox->addItem(name_c,id_c);	
		if (current_index==id_c) ci_3=comboBox->count()-1;
   	}

	comboBox->setCurrentIndex(ci_3);
}
//================== заполнение таблицы prof ==============================================
void Add_elements_dialog::table_prof() {
	prof_view->setColumnCount(5);
	prof_view->hideColumn(1);
	prof_view->setEditTriggers(QAbstractItemView::NoEditTriggers);
	
	QStringList header_list;
	header_list<<" "<<" "<<"Профессия"<<"Процент"<<" ";
	prof_view->setHorizontalHeaderLabels(header_list);
	
	QSqlQuery query;
	QString str = QString("SELECT prof.name_profession, popp.persent_profession, popp.id_pop_profession FROM profession prof, pop_profession popp WHERE popp.id_region = %1 AND prof.id_profession=popp.id_profession ORDER BY popp.persent_profession").arg(in_id_object);
	if(!query.exec(str))
	{
	 return;
	}
    
	QSqlRecord rec = query.record();
	int id_pop_prof;
	int row = 0;
    QString type_name;
	float persent_name;
    QTableWidgetItem *item;
    QIcon icon(QString(":/Resources/edit_1.png"));
    while(query.next())
	{	
		id_pop_prof = query.value(rec.indexOf("id_pop_profession")).toInt();
		type_name = query.value(rec.indexOf("name_profession")).toString();
		persent_name = query.value(rec.indexOf("persent_profession")).toString().toFloat();
			
		prof_view->insertRow(row);

		item = new QTableWidgetItem(icon," Редактировать",0);
		prof_view->setItem(row,0,item);

		item = new QTableWidgetItem(QString::number(id_pop_prof));
		prof_view->setItem(row,1,item);

		item = new QTableWidgetItem(type_name);
		prof_view->setItem(row,2,item);

		item = new QTableWidgetItem(QString::number(persent_name));
		prof_view->setItem(row,3,item);
	//	prof_view->sortItems(3,Qt::DescendingOrder);

		QIcon icon2(QString(":/Resources/saturn_delete.png"));
		item = new QTableWidgetItem(icon2," Удалить",0);
		prof_view->setItem(row,4,item);
	}	
   row++;
   prof_view->resizeColumnsToContents();
}
void Add_elements_dialog::table_prof_gr() {
	prof_view->setColumnCount(5);
	prof_view->hideColumn(1);
	prof_view->setEditTriggers(QAbstractItemView::NoEditTriggers);
	
	QStringList header_list;
	header_list<<" "<<" "<<"Профессия"<<"Процент"<<" ";
	prof_view->setHorizontalHeaderLabels(header_list);
	
	QSqlQuery query;
	QString str = QString("SELECT prof.name_profession, popp.persent_profession, popp.id_pop_profession FROM profession prof, pop_profession popp WHERE popp.id_groups = %1 AND prof.id_profession=popp.id_profession ORDER BY popp.persent_profession").arg(in_id_object);
	if(!query.exec(str))
	{
	 return;
	}
    
	QSqlRecord rec = query.record();
	int id_pop_prof;
	int row = 0;
    QString type_name;
	float persent_name;
    QTableWidgetItem *item;
    QIcon icon(QString(":/Resources/edit_1.png"));
    while(query.next())
	{	
		id_pop_prof = query.value(rec.indexOf("id_pop_profession")).toInt();
		type_name = query.value(rec.indexOf("name_profession")).toString();
		persent_name = query.value(rec.indexOf("persent_profession")).toString().toFloat();
			
		prof_view->insertRow(row);

		item = new QTableWidgetItem(icon," Редактировать",0);
		prof_view->setItem(row,0,item);

		item = new QTableWidgetItem(QString::number(id_pop_prof));
		prof_view->setItem(row,1,item);

		item = new QTableWidgetItem(type_name);
		prof_view->setItem(row,2,item);

		item = new QTableWidgetItem(QString::number(persent_name));
		prof_view->setItem(row,3,item);
	//	prof_view->sortItems(3,Qt::DescendingOrder);

		QIcon icon2(QString(":/Resources/saturn_delete.png"));
		item = new QTableWidgetItem(icon2," Удалить",0);
		prof_view->setItem(row,4,item);
	}	
   row++;
   prof_view->resizeColumnsToContents();
}
//======================= диалог добавления профессии =====================================
void Add_elements_dialog::add_prof_table_dlg()
{
	add_dlg = new QDialog;
	add_dlg->setWindowTitle("Добавлении професии");
	add_dlg->setMinimumSize(QSize(600,400));

	label = new QLabel("Профессия:");
	 comboBox = new QComboBox();
	 comboBox->setFixedWidth(200);
	 label->setBuddy(comboBox);

	
     QStringList column;
	 column.clear();
	 column<<"name_profession";
     fill_combobox(comboBox,"profession",column);

	 label_2 = new QLabel("Процент:");
	 lineEdit_counte = new QLineEdit;
	 label_2->setBuddy(lineEdit_counte);

	 addButton = new QPushButton("Добавить");
	 addButton->setDefault(true);
	 connect(addButton,SIGNAL(clicked()),this,SLOT(add_prof()));
	 cancelButton = new QPushButton("Отмена");
	 connect(cancelButton,SIGNAL(clicked()),add_dlg,SLOT(accept()));

	 QHBoxLayout *buttonsLayout = new QHBoxLayout;
	 buttonsLayout->addStretch();
	 buttonsLayout->addWidget(addButton);
	 buttonsLayout->addWidget(cancelButton);

	 QHBoxLayout *topLeftLayout_29 = new QHBoxLayout;
	 topLeftLayout_29->addWidget(label);
	 QHBoxLayout *topLeftLayout_30 = new QHBoxLayout;
	 topLeftLayout_30->addWidget(comboBox);
	 QHBoxLayout *topLeftLayout_1 = new QHBoxLayout;
	 topLeftLayout_1->addWidget(label_2);
	 QHBoxLayout *topLeftLayout_2 = new QHBoxLayout;
	 topLeftLayout_2->addWidget(lineEdit_counte);

	 QVBoxLayout *leftLayout_31 = new QVBoxLayout;
	 leftLayout_31->addLayout(topLeftLayout_29);
	 QVBoxLayout *leftLayout_32 = new QVBoxLayout;
	 leftLayout_32->addLayout(topLeftLayout_30);
	 QVBoxLayout *leftLayout_1 = new QVBoxLayout;
	 leftLayout_1->addLayout(topLeftLayout_1);
	 QVBoxLayout *leftLayout_2 = new QVBoxLayout;
	 leftLayout_2->addLayout(topLeftLayout_2);
	
	 QGridLayout *mainLayout = new QGridLayout;
	 mainLayout->setSizeConstraint(QLayout::SetFixedSize);
	 mainLayout->addLayout(leftLayout_31, 0, 0);
	 mainLayout->addLayout(leftLayout_32, 0, 1);
	 mainLayout->addLayout(leftLayout_1, 1, 0);
	 mainLayout->addLayout(leftLayout_2, 1, 1);

	 mainLayout->addLayout(buttonsLayout, 2, 0, 1, 2);
	 add_dlg->setLayout(mainLayout);

	 if(add_dlg->exec() == QDialog::Accepted){
	 }
	
}

void Add_elements_dialog::add_prof()
{
		int id_prof=comboBox->itemData(comboBox->currentIndex()).toInt();
		float counte_prof = lineEdit_counte->text().toFloat();
			
		if(comboBox->currentIndex()== 0){QMessageBox::StandardButton ret; ret = QMessageBox::warning (this,"Ошибка",("Профессия не выбрана "),QMessageBox::Ok);return;}
		else if (lineEdit_counte->text() == ""){QMessageBox::StandardButton ret; ret = QMessageBox::warning (this,"Ошибка",("Введите процент "),QMessageBox::Ok);return;}
		
		int id_reg = in_id_object;

		QMap<QString,QString> map;
				
		if (in_id_object<1) return;
		map.clear();
		map.insert("id_profession",QString::number(id_prof));
		map.insert("persent_profession",QString::number(counte_prof));
		map.insert("id_region",QString::number(id_reg));
				
		insert_in_table("pop_profession",map,"id_pop_profession");
		
		clear_table(prof_view);
		table_prof();
		add_dlg->close();
}
//=========================================================================================
void Add_elements_dialog::add_prof_table_dlg_gr()
{
	add_dlg = new QDialog;
	add_dlg->setWindowTitle("Добавлении професии");
	add_dlg->setMinimumSize(QSize(600,400));

	label = new QLabel("Профессия:");
	 comboBox = new QComboBox();
	 comboBox->setFixedWidth(200);
	 label->setBuddy(comboBox);

	
     QStringList column;
	 column.clear();
	 column<<"name_profession";
     fill_combobox(comboBox,"profession",column);

	 label_2 = new QLabel("Процент:");
	 lineEdit_counte = new QLineEdit;
	 label_2->setBuddy(lineEdit_counte);

	 addButton = new QPushButton("Добавить");
	 addButton->setDefault(true);
	 connect(addButton,SIGNAL(clicked()),this,SLOT(add_prof_gr()));
	 cancelButton = new QPushButton("Отмена");
	 connect(cancelButton,SIGNAL(clicked()),add_dlg,SLOT(accept()));

	 QHBoxLayout *buttonsLayout = new QHBoxLayout;
	 buttonsLayout->addStretch();
	 buttonsLayout->addWidget(addButton);
	 buttonsLayout->addWidget(cancelButton);

	 QHBoxLayout *topLeftLayout_29 = new QHBoxLayout;
	 topLeftLayout_29->addWidget(label);
	 QHBoxLayout *topLeftLayout_30 = new QHBoxLayout;
	 topLeftLayout_30->addWidget(comboBox);
	 QHBoxLayout *topLeftLayout_1 = new QHBoxLayout;
	 topLeftLayout_1->addWidget(label_2);
	 QHBoxLayout *topLeftLayout_2 = new QHBoxLayout;
	 topLeftLayout_2->addWidget(lineEdit_counte);

	 QVBoxLayout *leftLayout_31 = new QVBoxLayout;
	 leftLayout_31->addLayout(topLeftLayout_29);
	 QVBoxLayout *leftLayout_32 = new QVBoxLayout;
	 leftLayout_32->addLayout(topLeftLayout_30);
	 QVBoxLayout *leftLayout_1 = new QVBoxLayout;
	 leftLayout_1->addLayout(topLeftLayout_1);
	 QVBoxLayout *leftLayout_2 = new QVBoxLayout;
	 leftLayout_2->addLayout(topLeftLayout_2);
	
	 QGridLayout *mainLayout = new QGridLayout;
	 mainLayout->setSizeConstraint(QLayout::SetFixedSize);
	 mainLayout->addLayout(leftLayout_31, 0, 0);
	 mainLayout->addLayout(leftLayout_32, 0, 1);
	 mainLayout->addLayout(leftLayout_1, 1, 0);
	 mainLayout->addLayout(leftLayout_2, 1, 1);

	 mainLayout->addLayout(buttonsLayout, 2, 0, 1, 2);
	 add_dlg->setLayout(mainLayout);

	 if(add_dlg->exec() == QDialog::Accepted){
	 }
	
}

void Add_elements_dialog::add_prof_gr()
{
		int id_prof=comboBox->itemData(comboBox->currentIndex()).toInt();
		float counte_prof = lineEdit_counte->text().toFloat();
			
		if(comboBox->currentIndex()== 0){QMessageBox::StandardButton ret; ret = QMessageBox::warning (this,"Ошибка",("Профессия не выбрана "),QMessageBox::Ok);return;}
		else if (lineEdit_counte->text() == ""){QMessageBox::StandardButton ret; ret = QMessageBox::warning (this,"Ошибка",("Введите процент "),QMessageBox::Ok);return;}
		
		int id_gr = in_id_object;

		QMap<QString,QString> map;
				
		if (in_id_object<1) return;
		map.clear();
		map.insert("id_profession",QString::number(id_prof));
		map.insert("persent_profession",QString::number(counte_prof));
		map.insert("id_groups",QString::number(id_gr));
				
		insert_in_table("pop_profession",map,"id_pop_profession");
		
		clear_table(prof_view);
		table_prof_gr();
		add_dlg->close();
}
//=========================== правка профессии ============================================
void Add_elements_dialog::show_redaktor_prof(int row,int column){
	QSqlQuery query;
	if((column!=0) && (column!=4)){
		return;
	}
	 else if(column==0){
	
	int id_ls_prof = prof_view->item(row,1)->text().toInt();
	pravka_prof(id_ls_prof);
		return;
	}
	 else{    
		int id_ls_prof= prof_view->item(row,1)->text().toInt();
	    if(!query.exec(QString ("delete from pop_profession where id_pop_profession = %1").arg(id_ls_prof))){
		return;
		}
		prof_view->removeRow(row);
	 }
}
void Add_elements_dialog::pravka_prof (int id_ls_prof){
		
	edit_dlg = new QDialog;
	edit_dlg->setWindowTitle("Редактирование данных профессия");
	edit_dlg->setMinimumSize(QSize(600,400));

	label = new QLabel("Профессия:");
	comboBox = new QComboBox();
	comboBox->setFixedWidth(200);
    label->setBuddy(comboBox);
	
	label_2 = new QLabel("Процент:");
	lineEdit_counte = new QLineEdit;
	label_2->setBuddy(lineEdit_counte);

	QSqlQuery query;
	QString str = QString("SELECT prof.id_profession, prof.name_profession, popp.id_profession, popp.persent_profession, popp.id_pop_profession FROM profession prof, pop_profession popp where id_pop_profession = %1 and prof.id_profession = popp.id_profession").arg(id_ls_prof);
	if(!query.exec(str)){
	 return;
	}
    
	QSqlRecord rec = query.record();
	QString name_prof;
	float persent;
	int id_prof;	
	while(query.next()){	
		id_prof=query.value(rec.indexOf("id_profession")).toInt();
		name_prof=query.value(rec.indexOf("name_profession")).toString();
		persent=query.value(rec.indexOf("persent_profession")).toString().toFloat();
	
		comboBox->clear();
		fill_combobox_prof(comboBox,id_prof) ; 
		lineEdit_counte->setText(QString::number(persent));	
	}
		 
	 addButton = new QPushButton("Сохранить");
	 addButton->setDefault(true);
	 connect(addButton,SIGNAL(clicked()),edit_dlg,SLOT(accept()));
	 cancelButton = new QPushButton("Отмена");
	 connect(cancelButton,SIGNAL(clicked()),edit_dlg,SLOT(close()));

	 QHBoxLayout *buttonsLayout = new QHBoxLayout;
	 buttonsLayout->addStretch();
	 buttonsLayout->addWidget(addButton);
	 buttonsLayout->addWidget(cancelButton);

	 QHBoxLayout *topLeftLayout_29 = new QHBoxLayout;
	 topLeftLayout_29->addWidget(label);
	 QHBoxLayout *topLeftLayout_30 = new QHBoxLayout;
	 topLeftLayout_30->addWidget(comboBox);
	 QHBoxLayout *topLeftLayout_1 = new QHBoxLayout;
	 topLeftLayout_1->addWidget(label_2);
	 QHBoxLayout *topLeftLayout_2 = new QHBoxLayout;
	 topLeftLayout_2->addWidget(lineEdit_counte);	

	 QVBoxLayout *leftLayout_31 = new QVBoxLayout;
	 leftLayout_31->addLayout(topLeftLayout_29);
	 QVBoxLayout *leftLayout_32 = new QVBoxLayout;
	 leftLayout_32->addLayout(topLeftLayout_30);
	 QVBoxLayout *leftLayout_1 = new QVBoxLayout;
	 leftLayout_1->addLayout(topLeftLayout_1);
	 QVBoxLayout *leftLayout_2 = new QVBoxLayout;
	 leftLayout_2->addLayout(topLeftLayout_2);

	 QGridLayout *mainLayout = new QGridLayout;
	 mainLayout->setSizeConstraint(QLayout::SetFixedSize);
	 mainLayout->addLayout(leftLayout_31, 0, 0);
	 mainLayout->addLayout(leftLayout_32, 0, 1);
	 mainLayout->addLayout(leftLayout_1, 1, 0);
	 mainLayout->addLayout(leftLayout_2, 1, 1);

	 mainLayout->addLayout(buttonsLayout, 2, 0, 1, 2);
	 edit_dlg->setLayout(mainLayout);
	if(edit_dlg->exec() == QDialog::Accepted){
	
	clear_table(prof_view);
	int id_proff = comboBox->itemData(comboBox->currentIndex()).toInt();
	float persent_proff = lineEdit_counte->text().toFloat();
						
	QSqlQuery query;
	QString str = QString("UPDATE pop_profession SET id_profession='%1',persent_profession='%2' WHERE id_pop_profession=%3").arg(id_proff).arg(persent_proff).arg(id_ls_prof);
	
	if(!query.exec(str)){
			 return;
		 }
	lineEdit_counte->clear();
	
	comboBox->setCurrentIndex(0);
	table_prof();
	table_prof_gr();
	
  }
	return; 
}
void Add_elements_dialog::fill_combobox_prof(QComboBox *comboBox,int current_index)
{
	QSqlQuery query;									
	query.exec("SELECT id_profession, name_profession FROM profession"); 
	int ci_3=0;
	while (query.next())												
	{
		QString name_prof = query.value(1).toString();				
		int id_prof=query.value(0).toInt();							
		comboBox->addItem(name_prof,id_prof);	
		if (current_index==id_prof) ci_3=comboBox->count()-1;
   	}

	comboBox->setCurrentIndex(ci_3);
}
//================== заполнение таблицы возраст ==============================================
void Add_elements_dialog::table_age() {
	age_view->setColumnCount(5);
	age_view->hideColumn(1);
	age_view->setEditTriggers(QAbstractItemView::NoEditTriggers);
	
	QStringList header_list;
	header_list<<" "<<" "<<"Возраст"<<"Процент"<<" ";
	age_view->setHorizontalHeaderLabels(header_list);
	
	QSqlQuery query;
	QString str = QString("SELECT age.name_age, popa.persent_age, popa.id_pop_age FROM age age, pop_age popa WHERE popa.id_region = %1 AND age.id_age=popa.id_age").arg(in_id_object);
	if(!query.exec(str))
	{
	 return;
	}
    
	QSqlRecord rec = query.record();
	int id_pop_age;
	int row = 0;
    QString type_name;
	float persent_name;
    QTableWidgetItem *item;
    QIcon icon(QString(":/Resources/edit_1.png"));
    while(query.next())
	{	
		id_pop_age = query.value(rec.indexOf("id_pop_age")).toInt();
		type_name = query.value(rec.indexOf("name_age")).toString();
		persent_name = query.value(rec.indexOf("persent_age")).toString().toFloat();
			
		age_view->insertRow(row);

		item = new QTableWidgetItem(icon," Редактировать",0);
		age_view->setItem(row,0,item);

		item = new QTableWidgetItem(QString::number(id_pop_age));
		age_view->setItem(row,1,item);

		item = new QTableWidgetItem(type_name);
		age_view->setItem(row,2,item);

		item = new QTableWidgetItem(QString::number(persent_name));
		age_view->setItem(row,3,item);
	//	age_view->sortItems(3,Qt::DescendingOrder);

		QIcon icon2(QString(":/Resources/saturn_delete.png"));
		item = new QTableWidgetItem(icon2," Удалить",0);
		age_view->setItem(row,4,item);
	}	
   row++;
   age_view->resizeColumnsToContents();
}
void Add_elements_dialog::table_age_gr() {
	age_view->setColumnCount(5);
	age_view->hideColumn(1);
	age_view->setEditTriggers(QAbstractItemView::NoEditTriggers);
	
	QStringList header_list;
	header_list<<" "<<" "<<"Возраст"<<"Процент"<<" ";
	age_view->setHorizontalHeaderLabels(header_list);
	
	QSqlQuery query;
	QString str = QString("SELECT age.name_age, popa.persent_age, popa.id_pop_age FROM age age, pop_age popa WHERE popa.id_groups = %1 AND age.id_age=popa.id_age").arg(in_id_object);
	if(!query.exec(str))
	{
	 return;
	}
    
	QSqlRecord rec = query.record();
	int id_pop_age;
	int row = 0;
    QString type_name;
	float persent_name;
    QTableWidgetItem *item;
    QIcon icon(QString(":/Resources/edit_1.png"));
    while(query.next())
	{	
		id_pop_age = query.value(rec.indexOf("id_pop_age")).toInt();
		type_name = query.value(rec.indexOf("name_age")).toString();
		persent_name = query.value(rec.indexOf("persent_age")).toString().toFloat();
			
		age_view->insertRow(row);

		item = new QTableWidgetItem(icon," Редактировать",0);
		age_view->setItem(row,0,item);

		item = new QTableWidgetItem(QString::number(id_pop_age));
		age_view->setItem(row,1,item);

		item = new QTableWidgetItem(type_name);
		age_view->setItem(row,2,item);

		item = new QTableWidgetItem(QString::number(persent_name));
		age_view->setItem(row,3,item);
	//	age_view->sortItems(3,Qt::DescendingOrder);

		QIcon icon2(QString(":/Resources/saturn_delete.png"));
		item = new QTableWidgetItem(icon2," Удалить",0);
		age_view->setItem(row,4,item);
	}	
   row++;
   age_view->resizeColumnsToContents();
}
//======================= диалог добавления возраста =====================================
void Add_elements_dialog::add_age_table_dlg()
{
	add_dlg = new QDialog;
	add_dlg->setWindowTitle("Добавлении возраста");
	add_dlg->setMinimumSize(QSize(600,400));

	label = new QLabel("Возраст:");
	 comboBox = new QComboBox();
	 comboBox->setFixedWidth(200);
	 label->setBuddy(comboBox);

	
     QStringList column;
	 column.clear();
	 column<<"name_age";
     fill_combobox(comboBox,"age",column);

	 label_2 = new QLabel("Процент:");
	 lineEdit_counte = new QLineEdit;
	 label_2->setBuddy(lineEdit_counte);

	 addButton = new QPushButton("Добавить");
	 addButton->setDefault(true);
	 connect(addButton,SIGNAL(clicked()),this,SLOT(add_age()));
	 cancelButton = new QPushButton("Отмена");
	 connect(cancelButton,SIGNAL(clicked()),add_dlg,SLOT(accept()));

	 QHBoxLayout *buttonsLayout = new QHBoxLayout;
	 buttonsLayout->addStretch();
	 buttonsLayout->addWidget(addButton);
	 buttonsLayout->addWidget(cancelButton);

	 QHBoxLayout *topLeftLayout_29 = new QHBoxLayout;
	 topLeftLayout_29->addWidget(label);
	 QHBoxLayout *topLeftLayout_30 = new QHBoxLayout;
	 topLeftLayout_30->addWidget(comboBox);
	 QHBoxLayout *topLeftLayout_1 = new QHBoxLayout;
	 topLeftLayout_1->addWidget(label_2);
	 QHBoxLayout *topLeftLayout_2 = new QHBoxLayout;
	 topLeftLayout_2->addWidget(lineEdit_counte);

	 QVBoxLayout *leftLayout_31 = new QVBoxLayout;
	 leftLayout_31->addLayout(topLeftLayout_29);
	 QVBoxLayout *leftLayout_32 = new QVBoxLayout;
	 leftLayout_32->addLayout(topLeftLayout_30);
	 QVBoxLayout *leftLayout_1 = new QVBoxLayout;
	 leftLayout_1->addLayout(topLeftLayout_1);
	 QVBoxLayout *leftLayout_2 = new QVBoxLayout;
	 leftLayout_2->addLayout(topLeftLayout_2);
	
	 QGridLayout *mainLayout = new QGridLayout;
	 mainLayout->setSizeConstraint(QLayout::SetFixedSize);
	 mainLayout->addLayout(leftLayout_31, 0, 0);
	 mainLayout->addLayout(leftLayout_32, 0, 1);
	 mainLayout->addLayout(leftLayout_1, 1, 0);
	 mainLayout->addLayout(leftLayout_2, 1, 1);

	 mainLayout->addLayout(buttonsLayout, 2, 0, 1, 2);
	 add_dlg->setLayout(mainLayout);

	 if(add_dlg->exec() == QDialog::Accepted){
	 }
	
}

void Add_elements_dialog::add_age()
{
		int id_age=comboBox->itemData(comboBox->currentIndex()).toInt();
		float counte_age = lineEdit_counte->text().toFloat();
			
		if(comboBox->currentIndex()== 0){QMessageBox::StandardButton ret; ret = QMessageBox::warning (this,"Ошибка",("Возраст не выбран "),QMessageBox::Ok);return;}
		else if (lineEdit_counte->text() == ""){QMessageBox::StandardButton ret; ret = QMessageBox::warning (this,"Ошибка",("Введите процент "),QMessageBox::Ok);return;}
		
		int id_reg = in_id_object;

		QMap<QString,QString> map;
				
		if (in_id_object<1) return;
		map.clear();
		map.insert("id_age",QString::number(id_age));
		map.insert("persent_age",QString::number(counte_age));
		map.insert("id_region",QString::number(id_reg));
				
		insert_in_table("pop_age",map,"id_pop_age");
		
		clear_table(age_view);
		table_age();
		add_dlg->close();
}
//========================================================================================
void Add_elements_dialog::add_age_table_dlg_gr()
{
	add_dlg = new QDialog;
	add_dlg->setWindowTitle("Добавлении возраста");
	add_dlg->setMinimumSize(QSize(600,400));

	label = new QLabel("Возраст:");
	 comboBox = new QComboBox();
	 comboBox->setFixedWidth(200);
	 label->setBuddy(comboBox);

	
     QStringList column;
	 column.clear();
	 column<<"name_age";
     fill_combobox(comboBox,"age",column);

	 label_2 = new QLabel("Процент:");
	 lineEdit_counte = new QLineEdit;
	 label_2->setBuddy(lineEdit_counte);

	 addButton = new QPushButton("Добавить");
	 addButton->setDefault(true);
	 connect(addButton,SIGNAL(clicked()),this,SLOT(add_age_gr()));
	 cancelButton = new QPushButton("Отмена");
	 connect(cancelButton,SIGNAL(clicked()),add_dlg,SLOT(accept()));

	 QHBoxLayout *buttonsLayout = new QHBoxLayout;
	 buttonsLayout->addStretch();
	 buttonsLayout->addWidget(addButton);
	 buttonsLayout->addWidget(cancelButton);

	 QHBoxLayout *topLeftLayout_29 = new QHBoxLayout;
	 topLeftLayout_29->addWidget(label);
	 QHBoxLayout *topLeftLayout_30 = new QHBoxLayout;
	 topLeftLayout_30->addWidget(comboBox);
	 QHBoxLayout *topLeftLayout_1 = new QHBoxLayout;
	 topLeftLayout_1->addWidget(label_2);
	 QHBoxLayout *topLeftLayout_2 = new QHBoxLayout;
	 topLeftLayout_2->addWidget(lineEdit_counte);

	 QVBoxLayout *leftLayout_31 = new QVBoxLayout;
	 leftLayout_31->addLayout(topLeftLayout_29);
	 QVBoxLayout *leftLayout_32 = new QVBoxLayout;
	 leftLayout_32->addLayout(topLeftLayout_30);
	 QVBoxLayout *leftLayout_1 = new QVBoxLayout;
	 leftLayout_1->addLayout(topLeftLayout_1);
	 QVBoxLayout *leftLayout_2 = new QVBoxLayout;
	 leftLayout_2->addLayout(topLeftLayout_2);
	
	 QGridLayout *mainLayout = new QGridLayout;
	 mainLayout->setSizeConstraint(QLayout::SetFixedSize);
	 mainLayout->addLayout(leftLayout_31, 0, 0);
	 mainLayout->addLayout(leftLayout_32, 0, 1);
	 mainLayout->addLayout(leftLayout_1, 1, 0);
	 mainLayout->addLayout(leftLayout_2, 1, 1);

	 mainLayout->addLayout(buttonsLayout, 2, 0, 1, 2);
	 add_dlg->setLayout(mainLayout);

	 if(add_dlg->exec() == QDialog::Accepted){
	 }
	
}

void Add_elements_dialog::add_age_gr()
{
		int id_age=comboBox->itemData(comboBox->currentIndex()).toInt();
		float counte_age = lineEdit_counte->text().toFloat();
			
		if(comboBox->currentIndex()== 0){QMessageBox::StandardButton ret; ret = QMessageBox::warning (this,"Ошибка",("Возраст не выбран "),QMessageBox::Ok);return;}
		else if (lineEdit_counte->text() == ""){QMessageBox::StandardButton ret; ret = QMessageBox::warning (this,"Ошибка",("Введите процент "),QMessageBox::Ok);return;}
		
		int id_gr = in_id_object;

		QMap<QString,QString> map;
				
		if (in_id_object<1) return;
		map.clear();
		map.insert("id_age",QString::number(id_age));
		map.insert("persent_age",QString::number(counte_age));
		map.insert("id_groups",QString::number(id_gr));
				
		insert_in_table("pop_age",map,"id_pop_age");
		
		clear_table(age_view);
		table_age_gr();
		add_dlg->close();
}
//=========================== правка возраста ============================================
void Add_elements_dialog::show_redaktor_age(int row,int column){
	QSqlQuery query;
	if((column!=0) && (column!=4)){
		return;
	}
	 else if(column==0){
	
	int id_ls_age = age_view->item(row,1)->text().toInt();
	pravka_age(id_ls_age);
		return;
	}
	 else{    
		int id_ls_age= age_view->item(row,1)->text().toInt();
	    if(!query.exec(QString ("delete from pop_age where id_pop_age = %1").arg(id_ls_age))){
		return;
		}
		age_view->removeRow(row);
	 }
}
void Add_elements_dialog::pravka_age (int id_ls_age){
		
	edit_dlg = new QDialog;
	edit_dlg->setWindowTitle("Редактирование данных возраст");
	edit_dlg->setMinimumSize(QSize(600,400));

	label = new QLabel("Возраст:");
	comboBox = new QComboBox();
	comboBox->setFixedWidth(200);
    label->setBuddy(comboBox);
	
	label_2 = new QLabel("Процент:");
	lineEdit_counte = new QLineEdit;
	label_2->setBuddy(lineEdit_counte);

	QSqlQuery query;
	QString str = QString("SELECT age.id_age, age.name_age, popa.id_age, popa.persent_age, popa.id_pop_age FROM age age, pop_age popa where id_pop_age = %1 and age.id_age = popa.id_age").arg(id_ls_age);
	if(!query.exec(str)){
	 return;
	}
    
	QSqlRecord rec = query.record();
	QString name_age;
	float persent;
	int id_age;	
	while(query.next()){	
		id_age=query.value(rec.indexOf("id_age")).toInt();
		name_age=query.value(rec.indexOf("name_age")).toString();
		persent=query.value(rec.indexOf("persent_age")).toString().toFloat();
	
		comboBox->clear();
		fill_combobox_age(comboBox,id_age) ; 
		lineEdit_counte->setText(QString::number(persent));	
	}
		 
	 addButton = new QPushButton("Сохранить");
	 addButton->setDefault(true);
	 connect(addButton,SIGNAL(clicked()),edit_dlg,SLOT(accept()));
	 cancelButton = new QPushButton("Отмена");
	 connect(cancelButton,SIGNAL(clicked()),edit_dlg,SLOT(close()));

	 QHBoxLayout *buttonsLayout = new QHBoxLayout;
	 buttonsLayout->addStretch();
	 buttonsLayout->addWidget(addButton);
	 buttonsLayout->addWidget(cancelButton);

	 QHBoxLayout *topLeftLayout_29 = new QHBoxLayout;
	 topLeftLayout_29->addWidget(label);
	 QHBoxLayout *topLeftLayout_30 = new QHBoxLayout;
	 topLeftLayout_30->addWidget(comboBox);
	 QHBoxLayout *topLeftLayout_1 = new QHBoxLayout;
	 topLeftLayout_1->addWidget(label_2);
	 QHBoxLayout *topLeftLayout_2 = new QHBoxLayout;
	 topLeftLayout_2->addWidget(lineEdit_counte);	

	 QVBoxLayout *leftLayout_31 = new QVBoxLayout;
	 leftLayout_31->addLayout(topLeftLayout_29);
	 QVBoxLayout *leftLayout_32 = new QVBoxLayout;
	 leftLayout_32->addLayout(topLeftLayout_30);
	 QVBoxLayout *leftLayout_1 = new QVBoxLayout;
	 leftLayout_1->addLayout(topLeftLayout_1);
	 QVBoxLayout *leftLayout_2 = new QVBoxLayout;
	 leftLayout_2->addLayout(topLeftLayout_2);

	 QGridLayout *mainLayout = new QGridLayout;
	 mainLayout->setSizeConstraint(QLayout::SetFixedSize);
	 mainLayout->addLayout(leftLayout_31, 0, 0);
	 mainLayout->addLayout(leftLayout_32, 0, 1);
	 mainLayout->addLayout(leftLayout_1, 1, 0);
	 mainLayout->addLayout(leftLayout_2, 1, 1);

	 mainLayout->addLayout(buttonsLayout, 2, 0, 1, 2);
	 edit_dlg->setLayout(mainLayout);
	if(edit_dlg->exec() == QDialog::Accepted){
	
	clear_table(age_view);
	int id_ageg = comboBox->itemData(comboBox->currentIndex()).toInt();
	float persent_ageg = lineEdit_counte->text().toFloat();
						
	QSqlQuery query;
	QString str = QString("UPDATE pop_age SET id_age='%1',persent_age='%2' WHERE id_pop_age=%3").arg(id_ageg).arg(persent_ageg).arg(id_ls_age);
	
	if(!query.exec(str)){
			 return;
		 }
	lineEdit_counte->clear();
	
	comboBox->setCurrentIndex(0);
	table_age();
	table_age_gr();

	
  }
	return; 
}
void Add_elements_dialog::fill_combobox_age(QComboBox *comboBox,int current_index)
{
	QSqlQuery query;									
	query.exec("SELECT id_age, name_age FROM age"); 
	int ci_3=0;
	while (query.next())												
	{
		QString name_age = query.value(1).toString();				
		int id_age=query.value(0).toInt();							
		comboBox->addItem(name_age,id_age);	
		if (current_index==id_age) ci_3=comboBox->count()-1;
   	}

	comboBox->setCurrentIndex(ci_3);
}
//==========================================================================================
//================== заполнение таблицы пол ==============================================
void Add_elements_dialog::table_sex() {
	
	sex_view->setColumnCount(5);
//	sex_view->setRowCount(1);
	sex_view->hideColumn(1);
	sex_view->setEditTriggers(QAbstractItemView::NoEditTriggers);
	sex_view->setColumnWidth(0,120);
	sex_view->setColumnWidth(2,150);
	sex_view->setColumnWidth(3,150);
	sex_view->setColumnWidth(4,80);

	QStringList header_list;
	QStringList header_list_1;
	header_list<<" "<<" "<<"Мужское население \n (процент)"<<"Женское население \n (процент)"<<" ";
	//header_list_1<<"Процент";
	sex_view->setHorizontalHeaderLabels(header_list);
//	sex_view->setVerticalHeaderLabels(header_list_1);

	QSqlQuery query;
	QString str = QString("SELECT persent_sex_m,persent_sex_w, id_pop_sex FROM pop_sex WHERE id_region = %1").arg(in_id_object);
	if(!query.exec(str))
	{
	 return;
	}
    
	QSqlRecord rec = query.record();
	int id_pop_sex;
	int row = 0;
	float persent_m,persent_w;
    QTableWidgetItem *item;
    QIcon icon(QString(":/Resources/edit_1.png"));
    while(query.next())
	{	
		id_pop_sex = query.value(rec.indexOf("id_pop_sex")).toInt();
		persent_m = query.value(rec.indexOf("persent_sex_m")).toString().toFloat();
		persent_w = query.value(rec.indexOf("persent_sex_w")).toString().toFloat();
			
		sex_view->insertRow(row);

		item = new QTableWidgetItem(icon," Редактировать",0);
		sex_view->setItem(row,0,item);

		item = new QTableWidgetItem(QString::number(id_pop_sex));
		sex_view->setItem(row,1,item);

		item = new QTableWidgetItem(QString::number(persent_m));
		sex_view->setItem(row,2,item);

		item = new QTableWidgetItem(QString::number(persent_w));
		sex_view->setItem(row,3,item);

		QIcon icon2(QString(":/Resources/saturn_delete.png"));
		item = new QTableWidgetItem(icon2," Удалить",0);
		sex_view->setItem(row,4,item);
	}	
	if (sex_view->rowCount()>0) addButtonn->setDisabled(true);
//	addButtonn->setEnabled(false); 
	
    
}
void Add_elements_dialog::table_sex_gr() {
	
	sex_view->setColumnCount(5);
//	sex_view->setRowCount(1);
	sex_view->hideColumn(1);
	sex_view->setEditTriggers(QAbstractItemView::NoEditTriggers);
	sex_view->setColumnWidth(0,120);
	sex_view->setColumnWidth(2,150);
	sex_view->setColumnWidth(3,150);
	sex_view->setColumnWidth(4,80);

	QStringList header_list;
	QStringList header_list_1;
	header_list<<" "<<" "<<"Мужское население \n (процент)"<<"Женское население \n (процент)"<<" ";
	//header_list_1<<"Процент";
	sex_view->setHorizontalHeaderLabels(header_list);
//	sex_view->setVerticalHeaderLabels(header_list_1);

	QSqlQuery query;
	QString str = QString("SELECT persent_sex_m,persent_sex_w, id_pop_sex FROM pop_sex WHERE id_groups = %1").arg(in_id_object);
	if(!query.exec(str))
	{
	 return;
	}
    
	QSqlRecord rec = query.record();
	int id_pop_sex;
	int row = 0;
	float persent_m,persent_w;
    QTableWidgetItem *item;
    QIcon icon(QString(":/Resources/edit_1.png"));
    while(query.next())
	{	
		id_pop_sex = query.value(rec.indexOf("id_pop_sex")).toInt();
		persent_m = query.value(rec.indexOf("persent_sex_m")).toString().toFloat();
		persent_w = query.value(rec.indexOf("persent_sex_w")).toString().toFloat();
			
		sex_view->insertRow(row);

		item = new QTableWidgetItem(icon," Редактировать",0);
		sex_view->setItem(row,0,item);

		item = new QTableWidgetItem(QString::number(id_pop_sex));
		sex_view->setItem(row,1,item);

		item = new QTableWidgetItem(QString::number(persent_m));
		sex_view->setItem(row,2,item);

		item = new QTableWidgetItem(QString::number(persent_w));
		sex_view->setItem(row,3,item);

		QIcon icon2(QString(":/Resources/saturn_delete.png"));
		item = new QTableWidgetItem(icon2," Удалить",0);
		sex_view->setItem(row,4,item);
	}	
	if (sex_view->rowCount()>0) addButtonn->setDisabled(true);
//	addButtonn->setEnabled(false); 
	
    
}
//======================= диалог добавления пол =====================================
void Add_elements_dialog::add_sex_table_dlg()
{
	add_dlg = new QDialog;
	add_dlg->setWindowTitle("Добавлении Пол");
	add_dlg->setMinimumSize(QSize(600,400));

	 label = new QLabel("Процент мужского населения:");
	 lineEdit_counte_m = new QLineEdit;
	 label->setBuddy(lineEdit_counte_m);
	 
	 label_2 = new QLabel("Процент женского населения:");
	 lineEdit_counte_w = new QLineEdit;
	 label_2->setBuddy(lineEdit_counte_w);

	 addButton = new QPushButton("Добавить");
	 addButton->setDefault(true);
	 connect(addButton,SIGNAL(clicked()),this,SLOT(add_sex()));
	 cancelButton = new QPushButton("Отмена");
	 connect(cancelButton,SIGNAL(clicked()),add_dlg,SLOT(accept()));

	 QHBoxLayout *buttonsLayout = new QHBoxLayout;
	 buttonsLayout->addStretch();
	 buttonsLayout->addWidget(addButton);
	 buttonsLayout->addWidget(cancelButton);

	 QHBoxLayout *topLeftLayout_29 = new QHBoxLayout;
	 topLeftLayout_29->addWidget(label);
	 QHBoxLayout *topLeftLayout_30 = new QHBoxLayout;
	 topLeftLayout_30->addWidget(lineEdit_counte_m);
	 QHBoxLayout *topLeftLayout_1 = new QHBoxLayout;
	 topLeftLayout_1->addWidget(label_2);
	 QHBoxLayout *topLeftLayout_2 = new QHBoxLayout;
	 topLeftLayout_2->addWidget(lineEdit_counte_w);

	 QVBoxLayout *leftLayout_31 = new QVBoxLayout;
	 leftLayout_31->addLayout(topLeftLayout_29);
	 QVBoxLayout *leftLayout_32 = new QVBoxLayout;
	 leftLayout_32->addLayout(topLeftLayout_30);
	 QVBoxLayout *leftLayout_1 = new QVBoxLayout;
	 leftLayout_1->addLayout(topLeftLayout_1);
	 QVBoxLayout *leftLayout_2 = new QVBoxLayout;
	 leftLayout_2->addLayout(topLeftLayout_2);
	
	 QGridLayout *mainLayout = new QGridLayout;
	 mainLayout->setSizeConstraint(QLayout::SetFixedSize);
	 mainLayout->addLayout(leftLayout_31, 0, 0);
	 mainLayout->addLayout(leftLayout_32, 0, 1);
	 mainLayout->addLayout(leftLayout_1, 1, 0);
	 mainLayout->addLayout(leftLayout_2, 1, 1);

	 mainLayout->addLayout(buttonsLayout, 2, 0, 1, 2);
	 add_dlg->setLayout(mainLayout);

	 if(add_dlg->exec() == QDialog::Accepted){
	 }
	
}

void Add_elements_dialog::add_sex()
{
		float m_sex = lineEdit_counte_m->text().toFloat();
		float w_sex = lineEdit_counte_w->text().toFloat();
		
		int id_reg = in_id_object;

		QMap<QString,QString> map;
	
		if (in_id_object<1) return;
		map.clear();
		map.insert("persent_sex_m",QString::number(m_sex));
		map.insert("persent_sex_w",QString::number(w_sex));
		map.insert("id_region",QString::number(id_reg));
				
		insert_in_table("pop_sex",map,"id_pop_sex");
		
		clear_table(sex_view);
		table_sex();
		add_dlg->close();
}
//==================================================================================
void Add_elements_dialog::add_sex_table_dlg_gr()
{
	add_dlg = new QDialog;
	add_dlg->setWindowTitle("Добавлении Пол");
	add_dlg->setMinimumSize(QSize(600,400));

	 label = new QLabel("Процент мужского населения:");
	 lineEdit_counte_m = new QLineEdit;
	 label->setBuddy(lineEdit_counte_m);
	 
	 label_2 = new QLabel("Процент женского населения:");
	 lineEdit_counte_w = new QLineEdit;
	 label_2->setBuddy(lineEdit_counte_w);

	 addButton = new QPushButton("Добавить");
	 addButton->setDefault(true);
	 connect(addButton,SIGNAL(clicked()),this,SLOT(add_sex_gr()));
	 cancelButton = new QPushButton("Отмена");
	 connect(cancelButton,SIGNAL(clicked()),add_dlg,SLOT(accept()));

	 QHBoxLayout *buttonsLayout = new QHBoxLayout;
	 buttonsLayout->addStretch();
	 buttonsLayout->addWidget(addButton);
	 buttonsLayout->addWidget(cancelButton);

	 QHBoxLayout *topLeftLayout_29 = new QHBoxLayout;
	 topLeftLayout_29->addWidget(label);
	 QHBoxLayout *topLeftLayout_30 = new QHBoxLayout;
	 topLeftLayout_30->addWidget(lineEdit_counte_m);
	 QHBoxLayout *topLeftLayout_1 = new QHBoxLayout;
	 topLeftLayout_1->addWidget(label_2);
	 QHBoxLayout *topLeftLayout_2 = new QHBoxLayout;
	 topLeftLayout_2->addWidget(lineEdit_counte_w);

	 QVBoxLayout *leftLayout_31 = new QVBoxLayout;
	 leftLayout_31->addLayout(topLeftLayout_29);
	 QVBoxLayout *leftLayout_32 = new QVBoxLayout;
	 leftLayout_32->addLayout(topLeftLayout_30);
	 QVBoxLayout *leftLayout_1 = new QVBoxLayout;
	 leftLayout_1->addLayout(topLeftLayout_1);
	 QVBoxLayout *leftLayout_2 = new QVBoxLayout;
	 leftLayout_2->addLayout(topLeftLayout_2);
	
	 QGridLayout *mainLayout = new QGridLayout;
	 mainLayout->setSizeConstraint(QLayout::SetFixedSize);
	 mainLayout->addLayout(leftLayout_31, 0, 0);
	 mainLayout->addLayout(leftLayout_32, 0, 1);
	 mainLayout->addLayout(leftLayout_1, 1, 0);
	 mainLayout->addLayout(leftLayout_2, 1, 1);

	 mainLayout->addLayout(buttonsLayout, 2, 0, 1, 2);
	 add_dlg->setLayout(mainLayout);

	 if(add_dlg->exec() == QDialog::Accepted){
	 }
	
}

void Add_elements_dialog::add_sex_gr()
{
		float m_sex = lineEdit_counte_m->text().toFloat();
		float w_sex = lineEdit_counte_w->text().toFloat();
		
		int id_gr = in_id_object;

		QMap<QString,QString> map;
	
		if (in_id_object<1) return;
		map.clear();
		map.insert("persent_sex_m",QString::number(m_sex));
		map.insert("persent_sex_w",QString::number(w_sex));
		map.insert("id_groups",QString::number(id_gr));
				
		insert_in_table("pop_sex",map,"id_pop_sex");
		
		clear_table(sex_view);
		table_sex_gr();
		add_dlg->close();
}
//=========================== правка пол ============================================
void Add_elements_dialog::show_redaktor_sex(int row,int column){
	QSqlQuery query;
	if((column!=0) && (column!=4)){
		return;
	}
	 else if(column==0){
	
	int id_ls_sex = sex_view->item(row,1)->text().toInt();
	pravka_sex(id_ls_sex);
		return;
	}
	 else{    
		int id_ls_sex= sex_view->item(row,1)->text().toInt();
	    if(!query.exec(QString ("delete from pop_sex where id_pop_sex = %1").arg(id_ls_sex))){
		return;
		}
		sex_view->removeRow(row);
		addButtonn->setEnabled(true); 
	 }
}
void Add_elements_dialog::pravka_sex (int id_ls_sex){
		
	edit_dlg = new QDialog;
	edit_dlg->setWindowTitle("Редактирование данных");
	edit_dlg->setMinimumSize(QSize(600,400));

	label = new QLabel("Процент мужского населения:");
	 lineEdit_counte_m = new QLineEdit;
	 label->setBuddy(lineEdit_counte_m);
	 
	 label_2 = new QLabel("Процент женского населения:");
	 lineEdit_counte_w = new QLineEdit;
	 label_2->setBuddy(lineEdit_counte_w);

	QSqlQuery query;
	QString str = QString("SELECT id_pop_sex, persent_sex_m, persent_sex_w FROM pop_sex where id_pop_sex = %1").arg(id_ls_sex);
	if(!query.exec(str)){
	 return;
	}
    
	QSqlRecord rec = query.record();
	QString name_age;
	float persent_m,persent_w;
	int id_age;	
	while(query.next()){	
		
		persent_m=query.value(rec.indexOf("persent_sex_m")).toString().toFloat();
		persent_w=query.value(rec.indexOf("persent_sex_w")).toString().toFloat();
	
		lineEdit_counte_m->setText(QString::number(persent_m));	
		lineEdit_counte_w->setText(QString::number(persent_w));	
	}
		 
	 addButton = new QPushButton("Сохранить");
	 addButton->setDefault(true);
	 connect(addButton,SIGNAL(clicked()),edit_dlg,SLOT(accept()));
	 cancelButton = new QPushButton("Отмена");
	 connect(cancelButton,SIGNAL(clicked()),edit_dlg,SLOT(close()));

	 QHBoxLayout *buttonsLayout = new QHBoxLayout;
	 buttonsLayout->addStretch();
	 buttonsLayout->addWidget(addButton);
	 buttonsLayout->addWidget(cancelButton);

	 QHBoxLayout *topLeftLayout_29 = new QHBoxLayout;
	 topLeftLayout_29->addWidget(label);
	 QHBoxLayout *topLeftLayout_30 = new QHBoxLayout;
	 topLeftLayout_30->addWidget(lineEdit_counte_m);
	 QHBoxLayout *topLeftLayout_1 = new QHBoxLayout;
	 topLeftLayout_1->addWidget(label_2);
	 QHBoxLayout *topLeftLayout_2 = new QHBoxLayout;
	 topLeftLayout_2->addWidget(lineEdit_counte_w);	

	 QVBoxLayout *leftLayout_31 = new QVBoxLayout;
	 leftLayout_31->addLayout(topLeftLayout_29);
	 QVBoxLayout *leftLayout_32 = new QVBoxLayout;
	 leftLayout_32->addLayout(topLeftLayout_30);
	 QVBoxLayout *leftLayout_1 = new QVBoxLayout;
	 leftLayout_1->addLayout(topLeftLayout_1);
	 QVBoxLayout *leftLayout_2 = new QVBoxLayout;
	 leftLayout_2->addLayout(topLeftLayout_2);

	 QGridLayout *mainLayout = new QGridLayout;
	 mainLayout->setSizeConstraint(QLayout::SetFixedSize);
	 mainLayout->addLayout(leftLayout_31, 0, 0);
	 mainLayout->addLayout(leftLayout_32, 0, 1);
	 mainLayout->addLayout(leftLayout_1, 1, 0);
	 mainLayout->addLayout(leftLayout_2, 1, 1);

	 mainLayout->addLayout(buttonsLayout, 2, 0, 1, 2);
	 edit_dlg->setLayout(mainLayout);
	if(edit_dlg->exec() == QDialog::Accepted){
	
	clear_table(sex_view);
	float persent_mm = lineEdit_counte_m->text().toFloat();
	float persent_ww = lineEdit_counte_w->text().toFloat();
						
	QSqlQuery query;
	QString str = QString("UPDATE pop_sex SET persent_sex_m = '%1',persent_sex_w = '%2' WHERE id_pop_sex=%3").arg(persent_mm).arg(persent_ww).arg(id_ls_sex);
	
	if(!query.exec(str)){
			 return;
		 }
	lineEdit_counte_m->clear();
	lineEdit_counte_w->clear();
	
	table_sex();
	table_sex_gr();
	
  }
	return; 
} //================================================================================
//================== заполнение таблицы штатка ======================================================
void Add_elements_dialog::table_rank() {
	rank_view->setColumnCount(5);
	rank_view->hideColumn(1);
	rank_view->setEditTriggers(QAbstractItemView::NoEditTriggers);
	
	QStringList header_list;
	header_list<<" "<<" "<<"Штатно-должностной \n состав"<<"Процент"<<" ";
	rank_view->setHorizontalHeaderLabels(header_list);
	
	QSqlQuery query;
	QString str = QString("SELECT rank.name_rank, lsr.persent_rank, lsr.id_ls_rank FROM rank rank, ls_rank lsr WHERE lsr.id_ls = %1 AND rank.id_rank=lsr.id_rank ORDER BY lsr.persent_rank").arg(in_id_object);
	if(!query.exec(str))
	{
	 return;
	}
    
	QSqlRecord rec = query.record();
	int id_ls_rank;
	int row = 0;
    QString type_name;
	float persent_name;
    QTableWidgetItem *item;
    QIcon icon(QString(":/Resources/edit_1.png"));
    while(query.next())
	{	
		id_ls_rank = query.value(rec.indexOf("id_ls_rank")).toInt();
		type_name = query.value(rec.indexOf("name_rank")).toString();
		persent_name = query.value(rec.indexOf("persent_rank")).toString().toFloat();
			
		rank_view->insertRow(row);

		item = new QTableWidgetItem(icon," Редактировать",0);
		rank_view->setItem(row,0,item);

		item = new QTableWidgetItem(QString::number(id_ls_rank));
		rank_view->setItem(row,1,item);

		item = new QTableWidgetItem(type_name);
		rank_view->setItem(row,2,item);

		item = new QTableWidgetItem(QString::number(persent_name));
		rank_view->setItem(row,3,item);
	//	rank_view->sortItems(3,Qt::DescendingOrder);

		QIcon icon2(QString(":/Resources/saturn_delete.png"));
		item = new QTableWidgetItem(icon2," Удалить",0);
		rank_view->setItem(row,4,item);
	}	
   row++;
   rank_view->resizeColumnsToContents();
}
//====================================================================================================
void Add_elements_dialog::table_rank_gr() {
	rank_view->setColumnCount(5);
	rank_view->hideColumn(1);
	rank_view->setEditTriggers(QAbstractItemView::NoEditTriggers);
	
	QStringList header_list;
	header_list<<" "<<" "<<"Штатно-должностной \n состав"<<"Процент"<<" ";
	rank_view->setHorizontalHeaderLabels(header_list);
	
	QSqlQuery query;
	QString str = QString("SELECT rank.name_rank, lsr.persent_rank, lsr.id_ls_rank FROM rank rank, ls_rank lsr WHERE lsr.id_groups = %1 AND rank.id_rank=lsr.id_rank ORDER BY lsr.persent_rank").arg(in_id_object);
	if(!query.exec(str))
	{
	 return;
	}
    
	QSqlRecord rec = query.record();
	int id_ls_rank;
	int row = 0;
    QString type_name;
	float persent_name;
    QTableWidgetItem *item;
    QIcon icon(QString(":/Resources/edit_1.png"));
    while(query.next())
	{	
		id_ls_rank = query.value(rec.indexOf("id_ls_rank")).toInt();
		type_name = query.value(rec.indexOf("name_rank")).toString();
		persent_name = query.value(rec.indexOf("persent_rank")).toString().toFloat();
			
		rank_view->insertRow(row);

		item = new QTableWidgetItem(icon," Редактировать",0);
		rank_view->setItem(row,0,item);

		item = new QTableWidgetItem(QString::number(id_ls_rank));
		rank_view->setItem(row,1,item);

		item = new QTableWidgetItem(type_name);
		rank_view->setItem(row,2,item);

		item = new QTableWidgetItem(QString::number(persent_name));
		rank_view->setItem(row,3,item);
	//	rank_view->sortItems(3,Qt::DescendingOrder);

		QIcon icon2(QString(":/Resources/saturn_delete.png"));
		item = new QTableWidgetItem(icon2," Удалить",0);
		rank_view->setItem(row,4,item);
	}	
   row++;
   rank_view->resizeColumnsToContents();
}
//======================= диалог добавления штатка ===================================================
void Add_elements_dialog::add_rank_table_dlg()
{
	add_dlg = new QDialog;
	add_dlg->setWindowTitle("Добавлении ШДС");
	add_dlg->setMinimumSize(QSize(600,400));

	label = new QLabel("Штатно-должностной состав:");
	 comboBox = new QComboBox();
	 comboBox->setFixedWidth(200);
	 label->setBuddy(comboBox);

	
     QStringList column;
	 column.clear();
	 column<<"name_rank";
     fill_combobox(comboBox,"rank",column);

	 label_2 = new QLabel("Процент:");
	 lineEdit_counte = new QLineEdit;
	 label_2->setBuddy(lineEdit_counte);

	 addButton = new QPushButton("Добавить");
	 addButton->setDefault(true);
	 connect(addButton,SIGNAL(clicked()),this,SLOT(add_rank()));
	 cancelButton = new QPushButton("Отмена");
	 connect(cancelButton,SIGNAL(clicked()),add_dlg,SLOT(accept()));

	 QHBoxLayout *buttonsLayout = new QHBoxLayout;
	 buttonsLayout->addStretch();
	 buttonsLayout->addWidget(addButton);
	 buttonsLayout->addWidget(cancelButton);

	 QHBoxLayout *topLeftLayout_29 = new QHBoxLayout;
	 topLeftLayout_29->addWidget(label);
	 QHBoxLayout *topLeftLayout_30 = new QHBoxLayout;
	 topLeftLayout_30->addWidget(comboBox);
	 QHBoxLayout *topLeftLayout_1 = new QHBoxLayout;
	 topLeftLayout_1->addWidget(label_2);
	 QHBoxLayout *topLeftLayout_2 = new QHBoxLayout;
	 topLeftLayout_2->addWidget(lineEdit_counte);

	 QVBoxLayout *leftLayout_31 = new QVBoxLayout;
	 leftLayout_31->addLayout(topLeftLayout_29);
	 QVBoxLayout *leftLayout_32 = new QVBoxLayout;
	 leftLayout_32->addLayout(topLeftLayout_30);
	 QVBoxLayout *leftLayout_1 = new QVBoxLayout;
	 leftLayout_1->addLayout(topLeftLayout_1);
	 QVBoxLayout *leftLayout_2 = new QVBoxLayout;
	 leftLayout_2->addLayout(topLeftLayout_2);
	
	 QGridLayout *mainLayout = new QGridLayout;
	 mainLayout->setSizeConstraint(QLayout::SetFixedSize);
	 mainLayout->addLayout(leftLayout_31, 0, 0);
	 mainLayout->addLayout(leftLayout_32, 0, 1);
	 mainLayout->addLayout(leftLayout_1, 1, 0);
	 mainLayout->addLayout(leftLayout_2, 1, 1);

	 mainLayout->addLayout(buttonsLayout, 2, 0, 1, 2);
	 add_dlg->setLayout(mainLayout);

	 if(add_dlg->exec() == QDialog::Accepted){
	 }
	
}

void Add_elements_dialog::add_rank()
{
		int id_rank=comboBox->itemData(comboBox->currentIndex()).toInt();
		float counte_rank = lineEdit_counte->text().toFloat();
			
		if(comboBox->currentIndex()== 0){QMessageBox::StandardButton ret; ret = QMessageBox::warning (this,"Ошибка",("Штатно-должностной состав не выбран "),QMessageBox::Ok);return;}
		else if (lineEdit_counte->text() == ""){QMessageBox::StandardButton ret; ret = QMessageBox::warning (this,"Ошибка",("Введите процент "),QMessageBox::Ok);return;}
		
		int id_ls = in_id_object;

		QMap<QString,QString> map;
				
		if (in_id_object<1) return;
		map.clear();
		map.insert("id_rank",QString::number(id_rank));
		map.insert("persent_rank",QString::number(counte_rank));
		map.insert("id_ls",QString::number(id_ls));
				
		insert_in_table("ls_rank",map,"id_ls_rank");
		
		clear_table(rank_view);
		table_rank();
		add_dlg->close();
}
void Add_elements_dialog::add_rank_table_dlg_gr()
{
	add_dlg = new QDialog;
	add_dlg->setWindowTitle("Добавлении ШДС");
	add_dlg->setMinimumSize(QSize(600,400));

	label = new QLabel("Штатно-должностной состав:");
	 comboBox = new QComboBox();
	 comboBox->setFixedWidth(200);
	 label->setBuddy(comboBox);

	
     QStringList column;
	 column.clear();
	 column<<"name_rank";
     fill_combobox(comboBox,"rank",column);

	 label_2 = new QLabel("Процент:");
	 lineEdit_counte = new QLineEdit;
	 label_2->setBuddy(lineEdit_counte);

	 addButton = new QPushButton("Добавить");
	 addButton->setDefault(true);
	 connect(addButton,SIGNAL(clicked()),this,SLOT(add_rank_gr()));
	 cancelButton = new QPushButton("Отмена");
	 connect(cancelButton,SIGNAL(clicked()),add_dlg,SLOT(accept()));

	 QHBoxLayout *buttonsLayout = new QHBoxLayout;
	 buttonsLayout->addStretch();
	 buttonsLayout->addWidget(addButton);
	 buttonsLayout->addWidget(cancelButton);

	 QHBoxLayout *topLeftLayout_29 = new QHBoxLayout;
	 topLeftLayout_29->addWidget(label);
	 QHBoxLayout *topLeftLayout_30 = new QHBoxLayout;
	 topLeftLayout_30->addWidget(comboBox);
	 QHBoxLayout *topLeftLayout_1 = new QHBoxLayout;
	 topLeftLayout_1->addWidget(label_2);
	 QHBoxLayout *topLeftLayout_2 = new QHBoxLayout;
	 topLeftLayout_2->addWidget(lineEdit_counte);

	 QVBoxLayout *leftLayout_31 = new QVBoxLayout;
	 leftLayout_31->addLayout(topLeftLayout_29);
	 QVBoxLayout *leftLayout_32 = new QVBoxLayout;
	 leftLayout_32->addLayout(topLeftLayout_30);
	 QVBoxLayout *leftLayout_1 = new QVBoxLayout;
	 leftLayout_1->addLayout(topLeftLayout_1);
	 QVBoxLayout *leftLayout_2 = new QVBoxLayout;
	 leftLayout_2->addLayout(topLeftLayout_2);
	
	 QGridLayout *mainLayout = new QGridLayout;
	 mainLayout->setSizeConstraint(QLayout::SetFixedSize);
	 mainLayout->addLayout(leftLayout_31, 0, 0);
	 mainLayout->addLayout(leftLayout_32, 0, 1);
	 mainLayout->addLayout(leftLayout_1, 1, 0);
	 mainLayout->addLayout(leftLayout_2, 1, 1);

	 mainLayout->addLayout(buttonsLayout, 2, 0, 1, 2);
	 add_dlg->setLayout(mainLayout);

	 if(add_dlg->exec() == QDialog::Accepted){
	 }
	
}

void Add_elements_dialog::add_rank_gr()
{
		int id_rank=comboBox->itemData(comboBox->currentIndex()).toInt();
		float counte_rank = lineEdit_counte->text().toFloat();
			
		if(comboBox->currentIndex()== 0){QMessageBox::StandardButton ret; ret = QMessageBox::warning (this,"Ошибка",("Штатно-должностной состав не выбран "),QMessageBox::Ok);return;}
		else if (lineEdit_counte->text() == ""){QMessageBox::StandardButton ret; ret = QMessageBox::warning (this,"Ошибка",("Введите процент "),QMessageBox::Ok);return;}
		
		int id_gr = in_id_object;

		QMap<QString,QString> map;
				
		if (in_id_object<1) return;
		map.clear();
		map.insert("id_rank",QString::number(id_rank));
		map.insert("persent_rank",QString::number(counte_rank));
		map.insert("id_groups",QString::number(id_gr));
				
		insert_in_table("ls_rank",map,"id_ls_rank");
		
		clear_table(rank_view);
		table_rank_gr();
		add_dlg->close();
}
//============================= правка штатка ========================================================
void Add_elements_dialog::show_redaktor_rank (int row,int column){
	QSqlQuery query;
	if((column!=0) && (column!=4)){
		return;
	}
	 else if(column==0){
	
	int id_ls_rank = rank_view->item(row,1)->text().toInt();
	pravka_rank(id_ls_rank);
		return;
	}
	 else{    
		int id_ls_rank= rank_view->item(row,1)->text().toInt();
	    if(!query.exec(QString ("delete from ls_rank where id_ls_rank = %1").arg(id_ls_rank))){
		return;
		}
		rank_view->removeRow(row);
	 }
}
void Add_elements_dialog::pravka_rank(int id_ls_rank){
		
	edit_dlg = new QDialog;
	edit_dlg->setWindowTitle("Редактирование данных ШДС");
	edit_dlg->setMinimumSize(QSize(600,400));

	label = new QLabel("Штатно-должностной состав:");
	comboBox = new QComboBox();
	comboBox->setFixedWidth(200);
    label->setBuddy(comboBox);
	
	label_2 = new QLabel("Процент:");
	lineEdit_counte = new QLineEdit;
	label_2->setBuddy(lineEdit_counte);

	QSqlQuery query;
	QString str = QString("SELECT rank.id_rank, rank.name_rank, lsr.id_rank, lsr.persent_rank, lsr.id_ls_rank FROM rank rank, ls_rank lsr where id_ls_rank = %1 and rank.id_rank = lsr.id_rank").arg(id_ls_rank);
	if(!query.exec(str)){
	 return;
	}
    
	QSqlRecord rec = query.record();
	QString name_rank;
	float persent;
	int id_rank;	
	while(query.next()){	
		id_rank=query.value(rec.indexOf("id_rank")).toInt();
		name_rank=query.value(rec.indexOf("name_rank")).toString();
		persent=query.value(rec.indexOf("persent_rank")).toString().toFloat();
	
		comboBox->clear();
		fill_combobox_rank(comboBox,id_rank) ; 
		lineEdit_counte->setText(QString::number(persent));	
	}
		 
	 addButton = new QPushButton("Сохранить");
	 addButton->setDefault(true);
	 connect(addButton,SIGNAL(clicked()),edit_dlg,SLOT(accept()));
	 cancelButton = new QPushButton("Отмена");
	 connect(cancelButton,SIGNAL(clicked()),edit_dlg,SLOT(close()));

	 QHBoxLayout *buttonsLayout = new QHBoxLayout;
	 buttonsLayout->addStretch();
	 buttonsLayout->addWidget(addButton);
	 buttonsLayout->addWidget(cancelButton);

	 QHBoxLayout *topLeftLayout_29 = new QHBoxLayout;
	 topLeftLayout_29->addWidget(label);
	 QHBoxLayout *topLeftLayout_30 = new QHBoxLayout;
	 topLeftLayout_30->addWidget(comboBox);
	 QHBoxLayout *topLeftLayout_1 = new QHBoxLayout;
	 topLeftLayout_1->addWidget(label_2);
	 QHBoxLayout *topLeftLayout_2 = new QHBoxLayout;
	 topLeftLayout_2->addWidget(lineEdit_counte);	

	 QVBoxLayout *leftLayout_31 = new QVBoxLayout;
	 leftLayout_31->addLayout(topLeftLayout_29);
	 QVBoxLayout *leftLayout_32 = new QVBoxLayout;
	 leftLayout_32->addLayout(topLeftLayout_30);
	 QVBoxLayout *leftLayout_1 = new QVBoxLayout;
	 leftLayout_1->addLayout(topLeftLayout_1);
	 QVBoxLayout *leftLayout_2 = new QVBoxLayout;
	 leftLayout_2->addLayout(topLeftLayout_2);

	 QGridLayout *mainLayout = new QGridLayout;
	 mainLayout->setSizeConstraint(QLayout::SetFixedSize);
	 mainLayout->addLayout(leftLayout_31, 0, 0);
	 mainLayout->addLayout(leftLayout_32, 0, 1);
	 mainLayout->addLayout(leftLayout_1, 1, 0);
	 mainLayout->addLayout(leftLayout_2, 1, 1);

	 mainLayout->addLayout(buttonsLayout, 2, 0, 1, 2);
	 edit_dlg->setLayout(mainLayout);
	if(edit_dlg->exec() == QDialog::Accepted){
	
	clear_table(rank_view);
	int id_rankk = comboBox->itemData(comboBox->currentIndex()).toInt();
	float persent_rankk = lineEdit_counte->text().toFloat();
						
	QSqlQuery query;
	QString str = QString("UPDATE ls_rank SET id_rank='%1',persent_rank='%2' WHERE id_ls_rank=%3").arg(id_rankk).arg(persent_rankk).arg(id_ls_rank);
	
	if(!query.exec(str)){
			 return;
		 }
	lineEdit_counte->clear();
	
	comboBox->setCurrentIndex(0);
	table_rank();
	table_rank_gr();
	
  }
	return; 
}
void Add_elements_dialog::fill_combobox_rank(QComboBox *comboBox,int current_index)
{
	QSqlQuery query;									
	query.exec("SELECT id_rank, name_rank FROM rank"); 
	int ci_3=0;
	while (query.next())												
	{
		QString name_rank = query.value(1).toString();				
		int id_rank=query.value(0).toInt();							
		comboBox->addItem(name_rank,id_rank);	
		if (current_index==id_rank) ci_3=comboBox->count()-1;
   	}
	comboBox->setCurrentIndex(ci_3);
}
//======================== добавление средства для ВФ, Организаций и СМИ ============================
void Add_elements_dialog::add_mpo_pso()
{
		int id_type_mpo_pso = comboBox->itemData(comboBox->currentIndex()).toInt();
		QString name_mpo = lineEdit_name->text();
		QString counte_mpo = lineEdit_counte_mpo_pso->text();
		QString desc_mpo = textEdit_desc->toPlainText();

/*		if(pavComboBox->currentIndex()==0){QMessageBox::StandardButton ret; ret = QMessageBox::critical (this,"Ошибка",("Выберите тип программно-аппаратного воздействия "),QMessageBox::Ok );return;}
		else if(vul_pavComboBox->currentIndex()==0){QMessageBox::StandardButton ret; ret = QMessageBox::critical (this,"Ошибка",("Выберите уязвимость "),QMessageBox::Ok );return;}
		else if(itv_pavComboBox->currentIndex()==0){QMessageBox::StandardButton ret; ret = QMessageBox::critical (this,"Ошибка",("Выберите воздействие "),QMessageBox::Ok );return;}
		else if (lineEdit->text() == "")
		{QMessageBox::StandardButton ret; ret = QMessageBox::critical (this,"Ошибка",("Введите наименование программно-аппаратного воздействия "),QMessageBox::Ok );return;}
		else if (lineEdit_2->text() == "")
		{QMessageBox::StandardButton ret; ret = QMessageBox::critical (this,"Ошибка",("Введите нарушаемые ТО "),QMessageBox::Ok );return;}
		else if (textEdit->toPlainText() == "")
		{QMessageBox::StandardButton ret; ret = QMessageBox::critical (this,"Ошибка",("Введите характеристику нарушения "),QMessageBox::Ok );return;}
		else if (textEdit_2->toPlainText() == "")
		{QMessageBox::StandardButton ret; ret = QMessageBox::critical (this,"Ошибка",("Введите нанесенный(планируемый) ущерб "),QMessageBox::Ok );return;} */	

		QMap<QString,QString> map;
	
		if (in_id_object<1) return;
		map.clear();
		map.insert("name_mpo_pso",name_mpo);
		map.insert("counte_mpo_pso",counte_mpo);
		map.insert("description_mpo_pso",desc_mpo);
		map.insert("id_type_mpo_pso",QString::number(id_type_mpo_pso));
		map.insert("id_ls",QString::number(in_id_object));
	
		int id_mpo_pso=insert_in_table("mpo_pso",map,"id_mpo_pso");

 //======================= для картинки ===================================
       QFile file(lineEdit_name_f->text());
       if(!file.open(QIODevice::ReadOnly))
       {
            QMessageBox msgBox;
            msgBox.setWindowTitle("Внимание");
            msgBox.setText("Изображение отсутствует.Нажмите ДА, чтобы продолжить");
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            msgBox.setButtonText(QMessageBox::Yes, "Да");
            msgBox.setButtonText(QMessageBox::No, "Нет");
            if (msgBox.exec()== QMessageBox::Yes){

                if(id_mpo_pso>0){
                    this->done(id_mpo_pso);
                }else{
                    this->done(0);
                }

            }
           else{
                return;
            }
       }

        QSqlQuery query;

        query.prepare("UPDATE mpo_pso SET image_mpo_pso = ? WHERE id_mpo_pso = ?");

        QByteArray image_mpo_pso = file.readAll();
        query.addBindValue(image_mpo_pso);
        query.addBindValue(id_mpo_pso);
        if(!query.exec())
        {
            QString s = query.lastError().text();
        }

        if(id_mpo_pso>0){
            this->done(id_mpo_pso);
        }else{
            this->done(0);
        }
}
void Add_elements_dialog::add_mpo_pso_groups()
{
		int id_type_mpo_pso = comboBox->itemData(comboBox->currentIndex()).toInt();
		QString name_mpo = lineEdit_name->text();
		QString counte_mpo = lineEdit_counte_mpo_pso->text();
		QString desc_mpo = textEdit_desc->toPlainText();

		QMap<QString,QString> map;
	
		if (in_id_object<1) return;
		map.clear();
		map.insert("name_mpo_pso",name_mpo);
		map.insert("counte_mpo_pso",counte_mpo);
		map.insert("description_mpo_pso",desc_mpo);
		map.insert("id_type_mpo_pso",QString::number(id_type_mpo_pso));
		map.insert("id_groups",QString::number(in_id_object));
	
		int id_mpo_pso=insert_in_table("mpo_pso",map,"id_mpo_pso");
 //======================= для картинки ===================================
        QFile file(lineEdit_name_f->text());
        if(!file.open(QIODevice::ReadOnly))
       {
            QMessageBox msgBox;
            msgBox.setWindowTitle("Внимание");
            msgBox.setText("Изображение отсутствует.Нажмите ДА, чтобы продолжить");
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            msgBox.setButtonText(QMessageBox::Yes, "Да");
            msgBox.setButtonText(QMessageBox::No, "Нет");
            if (msgBox.exec()== QMessageBox::Yes){

                if(id_mpo_pso>0){
                    this->done(id_mpo_pso);
                }else{
                    this->done(0);
                }

            }
           else{
                return;
            }
       }

        QSqlQuery query;

        query.prepare("UPDATE mpo_pso SET image_mpo_pso = ? WHERE id_mpo_pso = ?");

        QByteArray image_mpo_pso = file.readAll();
        query.addBindValue(image_mpo_pso);
        query.addBindValue(id_mpo_pso);
        if(!query.exec())
        {
            QString s = query.lastError().text();
        }

        if(id_mpo_pso>0){
            this->done(id_mpo_pso);
        }else{
            this->done(0);
        }

}
void Add_elements_dialog::add_mpo_pso_smi()
{
		int id_type_mpo_pso = comboBox->itemData(comboBox->currentIndex()).toInt();
		QString name_mpo = lineEdit_name->text();
		QString counte_mpo = lineEdit_counte_mpo_pso->text();
		QString desc_mpo = textEdit_desc->toPlainText();

/*		if(pavComboBox->currentIndex()==0){QMessageBox::StandardButton ret; ret = QMessageBox::critical (this,"Ошибка",("Выберите тип программно-аппаратного воздействия "),QMessageBox::Ok );return;}
		else if(vul_pavComboBox->currentIndex()==0){QMessageBox::StandardButton ret; ret = QMessageBox::critical (this,"Ошибка",("Выберите уязвимость "),QMessageBox::Ok );return;}
		else if(itv_pavComboBox->currentIndex()==0){QMessageBox::StandardButton ret; ret = QMessageBox::critical (this,"Ошибка",("Выберите воздействие "),QMessageBox::Ok );return;}
		else if (lineEdit->text() == "")
		{QMessageBox::StandardButton ret; ret = QMessageBox::critical (this,"Ошибка",("Введите наименование программно-аппаратного воздействия "),QMessageBox::Ok );return;}
		else if (lineEdit_2->text() == "")
		{QMessageBox::StandardButton ret; ret = QMessageBox::critical (this,"Ошибка",("Введите нарушаемые ТО "),QMessageBox::Ok );return;}
		else if (textEdit->toPlainText() == "")
		{QMessageBox::StandardButton ret; ret = QMessageBox::critical (this,"Ошибка",("Введите характеристику нарушения "),QMessageBox::Ok );return;}
		else if (textEdit_2->toPlainText() == "")
		{QMessageBox::StandardButton ret; ret = QMessageBox::critical (this,"Ошибка",("Введите нанесенный(планируемый) ущерб "),QMessageBox::Ok );return;} */	

		QMap<QString,QString> map;
	
		if (in_id_object<1) return;
		map.clear();
		map.insert("name_mpo_pso",name_mpo);
		map.insert("counte_mpo_pso",counte_mpo);
		map.insert("description_mpo_pso",desc_mpo);
		map.insert("id_type_mpo_pso",QString::number(id_type_mpo_pso));
		map.insert("id_smi",QString::number(in_id_object));
	

        int id_mpo_pso=insert_in_table("mpo_pso",map,"id_mpo_pso");

        //======================= для картинки ===================================

         QFile file(lineEdit_name_f->text());
         if(!file.open(QIODevice::ReadOnly))
        {
             QMessageBox msgBox;
             msgBox.setWindowTitle("Внимание");
             msgBox.setText("Изображение отсутствует.Нажмите ДА, чтобы продолжить");
             msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
             msgBox.setButtonText(QMessageBox::Yes, "Да");
             msgBox.setButtonText(QMessageBox::No, "Нет");
             if (msgBox.exec()== QMessageBox::Yes){

                 if(id_mpo_pso>0){
                     this->done(id_mpo_pso);
                 }else{
                     this->done(0);
                 }

             }
            else{
                 return;
             }
        }

         QSqlQuery query;

         query.prepare("UPDATE mpo_pso SET image_mpo_pso = ? WHERE id_mpo_pso = ?");

         QByteArray image_mpo_pso = file.readAll();
         query.addBindValue(image_mpo_pso);
         query.addBindValue(id_mpo_pso);
         if(!query.exec())
         {
             QString s = query.lastError().text();
         }

         if(id_mpo_pso>0){
             this->done(id_mpo_pso);
         }else{
             this->done(0);
         }

}

//================================ добавление условия ================================================
void Add_elements_dialog::add_special_cond(){

	int id_type_sc=comboBox->itemData(comboBox->currentIndex()).toInt();

   	QString descr = textEdit_propa->toPlainText();
	QString name_sc = lineEdit_name->text();
	QString name_sem_1 = lineEdit_sem_1->text();
	QString name_sem_2 = lineEdit_sem_2->text();

	if (lineEdit_name->text() == ""){QMessageBox::StandardButton ret; ret = QMessageBox::warning (this,"Ошибка",("Введите наименование особого условия "),QMessageBox::Ok);return;}
	else if(comboBox->currentIndex()== 0){QMessageBox::StandardButton ret; ret = QMessageBox::warning (this,"Ошибка",("Тип особого условия не выбран "),QMessageBox::Ok);return;}
	//else if (lineEdit_sem_1->text() == ""){QMessageBox::StandardButton ret; ret = QMessageBox::warning (this,"Ошибка",("Введите семантику_1 "),QMessageBox::Ok);return;}
	//else if (lineEdit_sem_2->text() == ""){QMessageBox::StandardButton ret; ret = QMessageBox::warning (this,"Ошибка",("Введите семантику_2 "),QMessageBox::Ok);return;}
	else if (textEdit_propa->toPlainText() == ""){QMessageBox::StandardButton ret; ret = QMessageBox::warning (this,"Ошибка",("Введите описание особого условия "),QMessageBox::Ok);return;}

	int id_reg = in_id_object;

		QMap<QString,QString> map;
				
		if (in_id_object<1) return;
		map.clear();
		map.insert("name_special_conditions",name_sc);
		map.insert("id_type_special_conditions",QString::number(id_type_sc));
		map.insert("id_region",QString::number(id_reg));
		map.insert("description_special_conditions",descr);
		map.insert("semantika_1",name_sem_1);
		map.insert("semantika_2",name_sem_2);
			
		int id_sc=insert_in_table("special_conditions",map,"id_special_conditions");
		
//======================= для картинки ===================================
        QFile file(lineEdit_name_f->text());
        if(!file.open(QIODevice::ReadOnly))
       {
            QMessageBox msgBox;
            msgBox.setWindowTitle("Внимание");
            msgBox.setText("Изображение отсутствует.Нажмите ДА, чтобы продолжить");
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            msgBox.setButtonText(QMessageBox::Yes, "Да");
            msgBox.setButtonText(QMessageBox::No, "Нет");
            if (msgBox.exec()== QMessageBox::Yes){

                if(id_sc>0){
                    this->done(id_sc);
                }else{
                    this->done(0);
                }

            }
           else{
                return;
            }
       }

        QSqlQuery query;

        query.prepare("UPDATE special_conditions SET image_special_conditions = ? WHERE id_special_conditions = ?");

        QByteArray image_special_conditions = file.readAll();
        query.addBindValue(image_special_conditions);
        query.addBindValue(id_sc);
        if(!query.exec())
        {
            QString s = query.lastError().text();
        }

        if(id_sc>0){
            this->done(id_sc);
        }else{
            this->done(0);
        }

}

void Add_elements_dialog::open_file()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Выбор фотографии", "",
                                                    "Images (*.jpg *.png)");
    if (fileName.isEmpty()) return;
    lineEdit_name_f->setText(fileName);
	this->raise();
   
}

void Add_elements_dialog::otchet_pers()
{
    this->close();
    int id_pers = in_id_object;
    Reports *r = new Reports;
    QString report = r->create_object_formular_pers(id_pers);
    r->show_preview_dialog(report);
}

void Add_elements_dialog::fill_combobox_type_pers(QComboBox *box)
{
    box->clear();
    QSqlQuery query;
    box->addItem("-");
    query.exec("SELECT id_type_persones,name_type_persones FROM type_persones ");

    while (query.next()){

        QString name_type = query.value(1).toString();
        int id_type = query.value(0).toInt();
        box->addItem(name_type,id_type);
        }
    query.clear();
}
