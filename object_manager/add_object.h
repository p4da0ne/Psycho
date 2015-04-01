#ifndef ADD_OBJECT_H
#define ADD_OBJECT_H

#include <QDialog>
//#include "ui_add_object.h"

namespace Ui
{
    class add_object;
};

class add_object : public QDialog
{
	Q_OBJECT

public:
	
	add_object(int id_region, bool is_parent_ko,QWidget *parent = 0);
	~add_object();


	
private slots:

	//void set_type_object_comboBox();
	//void set_departament_object_comboBox();
	//void set_zveno_object_comboBox();
	void add_new_object();

private:
	int id_region_1;
	bool id_bool_preg;
	Ui::add_object *UI;
	

};

#endif // ADD_OBJECT_H
