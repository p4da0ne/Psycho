#include "simledelegate.h"
#include <QDoubleSpinBox>


simleDelegate::simleDelegate(QObject *parent=0)
    : QItemDelegate(parent)
{


}

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

    QItemDelegate::paint(painter,option,index);
}
QSize simleDelegate::sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index )
{
    return QItemDelegate::sizeHint (option,index);
}

void simleDelegate::updateEditorGeometry ( QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    QItemDelegate::updateEditorGeometry (editor,option,index );
}
void simleDelegate::setModelData ( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const{

    QItemDelegate::setModelData (editor,model,index );

}
void simleDelegate::setEditorData ( QWidget * editor, const QModelIndex & index ) const{

QItemDelegate::setEditorData (editor,index );
}
QWidget * simleDelegate::createEditor ( QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const{


    QWidget *w = QItemDelegate::createEditor(parent,option,index);
	if (QDoubleSpinBox *sb = qobject_cast<QDoubleSpinBox *>(w)){
	//	sb->setDecimals(5);
		sb->setSingleStep(0.01);
	}
	return w;
//return ::createEditor (parent,option,index);
}
