#ifndef VIEW_MANAGE_H
#define VIEW_MANAGE_H

#include <QObject>
#include <QMap>


class ViewManage : public QObject
{
    Q_OBJECT
public:
    /// Default constructor
    explicit ViewManage(QObject *parent=0);
    /// Destructor
    ~ViewManage();

	QMap<QString,QString> get_obj_info(int id_object = 0);
    QMap<QString,QMap<QString,QString> > get_all_objects_info(double x1 = 0,double y1 = 0, double x2 = 0,double y2 = 0);
	
public:
	QMap<QString,QString> *obj_info;
    QMap<QString,QMap<QString,QString> > *all_obj;
};

#endif // VIEW_MANAGE_H
