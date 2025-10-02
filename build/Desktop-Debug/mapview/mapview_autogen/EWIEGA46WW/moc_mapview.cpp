/****************************************************************************
** Meta object code from reading C++ file 'mapview.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../mapview/mapview.h"
#include <QtGui/qtextcursor.h>
#include <QtGui/qscreen.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mapview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN7MapViewE_t {};
} // unnamed namespace

template <> constexpr inline auto MapView::qt_create_metaobjectdata<qt_meta_tag_ZN7MapViewE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "MapView",
        "openNewMap",
        "",
        "openMapFromSettings",
        "openRST",
        "closeRST",
        "closeSelectedRST",
        "closeAllRST",
        "lessScale",
        "greateScale",
        "closeMap",
        "changeBrihgtUp",
        "changeBrihgtDown",
        "changeContrastUp",
        "changeContrastDown",
        "showCoordinates",
        "xyCoord",
        "slotObjectInfo",
        "slotObjectReport",
        "slotFormationPsiLooses",
        "slotRegionMpos",
        "slotMoveObject",
        "changeObjectCoordInDB",
        "x",
        "y",
        "slotMouseLeftButtonClicked",
        "pe",
        "QList<QStringList>",
        "objectsList",
        "slotMouseRightButtonClicked",
        "mouseRightSimpleMenu",
        "addEvent",
        "slotEventMedia",
        "slotDeleteEvent",
        "showCheckedObjects",
        "showCheckedCalcResults",
        "showCheckedEvents",
        "PrintMapSlot",
        "PrintScreenSlot",
        "slotSelectButtonToggled",
        "checked",
        "slotSearchObject",
        "chooseSelectedObjects",
        "chooseAllObjects",
        "slotSelectedObjectsListViewCustomMenu",
        "slotRemoveOneObject",
        "slotClearSelectedList"
    };

    QtMocHelpers::UintData qt_methods {
        // Slot 'openNewMap'
        QtMocHelpers::SlotData<void()>(1, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'openMapFromSettings'
        QtMocHelpers::SlotData<void()>(3, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'openRST'
        QtMocHelpers::SlotData<void()>(4, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'closeRST'
        QtMocHelpers::SlotData<void()>(5, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'closeSelectedRST'
        QtMocHelpers::SlotData<void()>(6, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'closeAllRST'
        QtMocHelpers::SlotData<void()>(7, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'lessScale'
        QtMocHelpers::SlotData<void()>(8, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'greateScale'
        QtMocHelpers::SlotData<void()>(9, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'closeMap'
        QtMocHelpers::SlotData<void()>(10, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'changeBrihgtUp'
        QtMocHelpers::SlotData<void()>(11, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'changeBrihgtDown'
        QtMocHelpers::SlotData<void()>(12, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'changeContrastUp'
        QtMocHelpers::SlotData<void()>(13, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'changeContrastDown'
        QtMocHelpers::SlotData<void()>(14, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'showCoordinates'
        QtMocHelpers::SlotData<void(QPointF)>(15, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QPointF, 16 },
        }}),
        // Slot 'slotObjectInfo'
        QtMocHelpers::SlotData<void()>(17, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'slotObjectReport'
        QtMocHelpers::SlotData<void()>(18, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'slotFormationPsiLooses'
        QtMocHelpers::SlotData<void()>(19, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'slotRegionMpos'
        QtMocHelpers::SlotData<void()>(20, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'slotMoveObject'
        QtMocHelpers::SlotData<void()>(21, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'changeObjectCoordInDB'
        QtMocHelpers::SlotData<void(double, double)>(22, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Double, 23 }, { QMetaType::Double, 24 },
        }}),
        // Slot 'slotMouseLeftButtonClicked'
        QtMocHelpers::SlotData<void(QPoint, QList<QStringList>)>(25, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QPoint, 26 }, { 0x80000000 | 27, 28 },
        }}),
        // Slot 'slotMouseRightButtonClicked'
        QtMocHelpers::SlotData<void(QPoint, QList<QStringList>)>(29, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QPoint, 26 }, { 0x80000000 | 27, 28 },
        }}),
        // Slot 'mouseRightSimpleMenu'
        QtMocHelpers::SlotData<void(QPoint)>(30, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QPoint, 26 },
        }}),
        // Slot 'addEvent'
        QtMocHelpers::SlotData<void()>(31, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'slotEventMedia'
        QtMocHelpers::SlotData<void()>(32, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'slotDeleteEvent'
        QtMocHelpers::SlotData<void()>(33, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'showCheckedObjects'
        QtMocHelpers::SlotData<void()>(34, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'showCheckedCalcResults'
        QtMocHelpers::SlotData<void()>(35, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'showCheckedEvents'
        QtMocHelpers::SlotData<void()>(36, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'PrintMapSlot'
        QtMocHelpers::SlotData<void()>(37, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'PrintScreenSlot'
        QtMocHelpers::SlotData<void()>(38, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'slotSelectButtonToggled'
        QtMocHelpers::SlotData<void(bool)>(39, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 40 },
        }}),
        // Slot 'slotSearchObject'
        QtMocHelpers::SlotData<void()>(41, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'chooseSelectedObjects'
        QtMocHelpers::SlotData<void()>(42, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'chooseAllObjects'
        QtMocHelpers::SlotData<void()>(43, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'slotSelectedObjectsListViewCustomMenu'
        QtMocHelpers::SlotData<void(const QPoint &)>(44, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QPoint, 26 },
        }}),
        // Slot 'slotRemoveOneObject'
        QtMocHelpers::SlotData<void()>(45, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'slotClearSelectedList'
        QtMocHelpers::SlotData<void()>(46, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<MapView, qt_meta_tag_ZN7MapViewE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject MapView::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7MapViewE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7MapViewE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN7MapViewE_t>.metaTypes,
    nullptr
} };

void MapView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<MapView *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->openNewMap(); break;
        case 1: _t->openMapFromSettings(); break;
        case 2: _t->openRST(); break;
        case 3: _t->closeRST(); break;
        case 4: _t->closeSelectedRST(); break;
        case 5: _t->closeAllRST(); break;
        case 6: _t->lessScale(); break;
        case 7: _t->greateScale(); break;
        case 8: _t->closeMap(); break;
        case 9: _t->changeBrihgtUp(); break;
        case 10: _t->changeBrihgtDown(); break;
        case 11: _t->changeContrastUp(); break;
        case 12: _t->changeContrastDown(); break;
        case 13: _t->showCoordinates((*reinterpret_cast< std::add_pointer_t<QPointF>>(_a[1]))); break;
        case 14: _t->slotObjectInfo(); break;
        case 15: _t->slotObjectReport(); break;
        case 16: _t->slotFormationPsiLooses(); break;
        case 17: _t->slotRegionMpos(); break;
        case 18: _t->slotMoveObject(); break;
        case 19: _t->changeObjectCoordInDB((*reinterpret_cast< std::add_pointer_t<double>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[2]))); break;
        case 20: _t->slotMouseLeftButtonClicked((*reinterpret_cast< std::add_pointer_t<QPoint>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QList<QStringList>>>(_a[2]))); break;
        case 21: _t->slotMouseRightButtonClicked((*reinterpret_cast< std::add_pointer_t<QPoint>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QList<QStringList>>>(_a[2]))); break;
        case 22: _t->mouseRightSimpleMenu((*reinterpret_cast< std::add_pointer_t<QPoint>>(_a[1]))); break;
        case 23: _t->addEvent(); break;
        case 24: _t->slotEventMedia(); break;
        case 25: _t->slotDeleteEvent(); break;
        case 26: _t->showCheckedObjects(); break;
        case 27: _t->showCheckedCalcResults(); break;
        case 28: _t->showCheckedEvents(); break;
        case 29: _t->PrintMapSlot(); break;
        case 30: _t->PrintScreenSlot(); break;
        case 31: _t->slotSelectButtonToggled((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 32: _t->slotSearchObject(); break;
        case 33: _t->chooseSelectedObjects(); break;
        case 34: _t->chooseAllObjects(); break;
        case 35: _t->slotSelectedObjectsListViewCustomMenu((*reinterpret_cast< std::add_pointer_t<QPoint>>(_a[1]))); break;
        case 36: _t->slotRemoveOneObject(); break;
        case 37: _t->slotClearSelectedList(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 20:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 1:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QList<QStringList> >(); break;
            }
            break;
        case 21:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 1:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QList<QStringList> >(); break;
            }
            break;
        }
    }
}

const QMetaObject *MapView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MapView::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7MapViewE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int MapView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 38)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 38;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 38)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 38;
    }
    return _id;
}
QT_WARNING_POP
