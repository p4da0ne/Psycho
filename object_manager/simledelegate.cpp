#include "simledelegate.h"
#include <QDoubleSpinBox>



simleDelegate::~simleDelegate()
{

}

void simleDelegate::paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index )const{

	QRect rect = option.rect;
	
    if (index.sibling(index.row(),index.column()).data().toString() == "0") {
		painter->fillRect(option.rect,QColor(Qt::red));
	}
	else{
		painter->fillRect(option.rect,QColor(Qt::green));
	}
	if ((index.column() == 14 || index.column() == 16 || index.column() == 18 || index.column() == 20 || index.column() == 22 || index.column() == 24 || index.column() == 26 || index.column() == 28 || index.column() == 30 || index.column() == 32 || index.column() == 34 || index.column() == 36 || index.column() == 38 || index.column() == 40 || index.column() == 42 || index.column() == 44 || index.column() == 46 || index.column() == 48 || index.column() == 50 || index.column() == 52 || index.column() == 54 || index.column() == 56 || index.column() == 58 || index.column() == 60 || index.column() == 62 || index.column() == 64 || index.column() == 66 || index.column() == 68 || index.column() == 70 || index.column() == 72 || index.column() == 74 || index.column() == 76 || index.column() == 78) && index.data().toString() != "0"){
		painter->fillRect(option.rect,QColor(Qt::yellow));
	}
	
	//	QStyleOptionViewItem option=option;
//	o.font.setBold(true);
//	o.palette.setBrush(QPalette::Base,fontt);

    QSqlRelationalDelegate::paint(painter,option,index);
}
QSize simleDelegate::sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index )
{
    return QSqlRelationalDelegate::sizeHint (option,index);
}

void simleDelegate::updateEditorGeometry ( QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    editor->setGeometry(option.rect);

    // QItemDelegate::updateEditorGeometry (editor,option,index );
}
void simleDelegate::setModelData ( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const{

//    QDoubleSpinBox *spinbox = static_cast<QDoubleSpinBox*>(editor);

//    spinbox->interpretText();

//    int value = spinbox->value();

//    model->setData(index, value, Qt::EditRole);


     QSqlRelationalDelegate::setModelData (editor,model,index );

}
void simleDelegate::setEditorData ( QWidget * editor, const QModelIndex & index ) const{

//    int value = index.model()->data(index, Qt::EditRole).toInt();

//    QDoubleSpinBox *spinbox = static_cast<QDoubleSpinBox*>(editor);

//    spinbox->setValue(value);


     QSqlRelationalDelegate::setEditorData (editor,index );
}
QWidget * simleDelegate::createEditor ( QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const{


//    QDoubleSpinBox *editor = new QDoubleSpinBox(parent);
//    editor->setDecimals(4);
//    editor->setSingleStep(0.01);
//    editor->setMinimum(0);
//    editor->setMaximum(1);

//    return editor;


    QWidget *w = QSqlRelationalDelegate::createEditor(parent,option,index);
    if (QDoubleSpinBox *sb = qobject_cast<QDoubleSpinBox *>(w)){
        sb->setDecimals(4);
        sb->setSingleStep(0.01);
        sb->setMinimum(0);
        sb->setMaximum(1);
    }
    return w;
//    return ::createEditor (parent,option,index);
}
