/****************************************************************************
** Meta object code from reading C++ file 'object_manager.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../object_manager/object_manager.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'object_manager.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN13ObjectmanagerE_t {};
} // unnamed namespace

template <> constexpr inline auto Objectmanager::qt_create_metaobjectdata<qt_meta_tag_ZN13ObjectmanagerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "Objectmanager",
        "customMenuTree",
        "",
        "customMenuView",
        "add_country_blok",
        "delete_country_blok",
        "update_one_click",
        "QModelIndex",
        "index",
        "updateDB",
        "QStandardItem*",
        "item",
        "show_nations_region",
        "show_nations_ls",
        "show_nations_gr",
        "show_confess_region",
        "show_confess_ls",
        "show_confess_gr",
        "show_profess_region",
        "show_profess_gr",
        "show_age_region",
        "show_age_gr",
        "show_sekas_region",
        "show_sekas_gr",
        "show_state_gr",
        "show_state_ls",
        "add_new_blok",
        "get_path",
        "get_path_edit",
        "get_path_flag",
        "get_path_flag_edit",
        "delete_blok",
        "delete_country",
        "add_new_country",
        "edit_country_blok",
        "edit_country",
        "show_redaktor_blok",
        "row",
        "column",
        "show_redaktor_country",
        "show_objects",
        "column_item_clicked",
        "delete_region",
        "delete_smi",
        "delete_ls",
        "delete_groups",
        "delete_mpo",
        "delete_sc",
        "delete_pers",
        "edit_persones",
        "clicked_open_file",
        "slotSearchObject",
        "otchet_groups",
        "add_new_coordinates",
        "show_dialog_add_file",
        "get_coordinates",
        "change_coord_system",
        "delete_coordinates",
        "edit_coordinates_view",
        "WGS_to_other",
        "PLANE_to_other"
    };

    QtMocHelpers::UintData qt_methods {
        // Slot 'customMenuTree'
        QtMocHelpers::SlotData<void(const QPoint &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QPoint, 2 },
        }}),
        // Slot 'customMenuView'
        QtMocHelpers::SlotData<void(const QPoint &)>(3, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QPoint, 2 },
        }}),
        // Slot 'add_country_blok'
        QtMocHelpers::SlotData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'delete_country_blok'
        QtMocHelpers::SlotData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'update_one_click'
        QtMocHelpers::SlotData<void(const QModelIndex &)>(6, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 7, 8 },
        }}),
        // Slot 'updateDB'
        QtMocHelpers::SlotData<void(QStandardItem *)>(9, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 10, 11 },
        }}),
        // Slot 'show_nations_region'
        QtMocHelpers::SlotData<void()>(12, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'show_nations_ls'
        QtMocHelpers::SlotData<void()>(13, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'show_nations_gr'
        QtMocHelpers::SlotData<void()>(14, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'show_confess_region'
        QtMocHelpers::SlotData<void()>(15, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'show_confess_ls'
        QtMocHelpers::SlotData<void()>(16, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'show_confess_gr'
        QtMocHelpers::SlotData<void()>(17, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'show_profess_region'
        QtMocHelpers::SlotData<void()>(18, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'show_profess_gr'
        QtMocHelpers::SlotData<void()>(19, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'show_age_region'
        QtMocHelpers::SlotData<void()>(20, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'show_age_gr'
        QtMocHelpers::SlotData<void()>(21, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'show_sekas_region'
        QtMocHelpers::SlotData<void()>(22, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'show_sekas_gr'
        QtMocHelpers::SlotData<void()>(23, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'show_state_gr'
        QtMocHelpers::SlotData<void()>(24, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'show_state_ls'
        QtMocHelpers::SlotData<void()>(25, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'add_new_blok'
        QtMocHelpers::SlotData<void()>(26, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'get_path'
        QtMocHelpers::SlotData<void()>(27, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'get_path_edit'
        QtMocHelpers::SlotData<void()>(28, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'get_path_flag'
        QtMocHelpers::SlotData<void()>(29, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'get_path_flag_edit'
        QtMocHelpers::SlotData<void()>(30, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'delete_blok'
        QtMocHelpers::SlotData<void()>(31, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'delete_country'
        QtMocHelpers::SlotData<void()>(32, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'add_new_country'
        QtMocHelpers::SlotData<void()>(33, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'edit_country_blok'
        QtMocHelpers::SlotData<void()>(34, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'edit_country'
        QtMocHelpers::SlotData<void()>(35, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'show_redaktor_blok'
        QtMocHelpers::SlotData<void(int, int)>(36, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 37 }, { QMetaType::Int, 38 },
        }}),
        // Slot 'show_redaktor_country'
        QtMocHelpers::SlotData<void(int, int)>(39, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 37 }, { QMetaType::Int, 38 },
        }}),
        // Slot 'show_objects'
        QtMocHelpers::SlotData<void(const QModelIndex &)>(40, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 7, 8 },
        }}),
        // Slot 'column_item_clicked'
        QtMocHelpers::SlotData<void(const QModelIndex &)>(41, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 7, 8 },
        }}),
        // Slot 'delete_region'
        QtMocHelpers::SlotData<void()>(42, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'delete_smi'
        QtMocHelpers::SlotData<void()>(43, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'delete_ls'
        QtMocHelpers::SlotData<void()>(44, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'delete_groups'
        QtMocHelpers::SlotData<void()>(45, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'delete_mpo'
        QtMocHelpers::SlotData<void()>(46, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'delete_sc'
        QtMocHelpers::SlotData<void()>(47, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'delete_pers'
        QtMocHelpers::SlotData<void()>(48, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'edit_persones'
        QtMocHelpers::SlotData<void()>(49, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'clicked_open_file'
        QtMocHelpers::SlotData<void()>(50, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'slotSearchObject'
        QtMocHelpers::SlotData<void()>(51, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'otchet_groups'
        QtMocHelpers::SlotData<void()>(52, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'add_new_coordinates'
        QtMocHelpers::SlotData<void()>(53, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'show_dialog_add_file'
        QtMocHelpers::SlotData<void()>(54, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'get_coordinates'
        QtMocHelpers::SlotData<void(int, QString, QString, QString)>(55, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 2 }, { QMetaType::QString, 2 }, { QMetaType::QString, 2 }, { QMetaType::QString, 2 },
        }}),
        // Slot 'change_coord_system'
        QtMocHelpers::SlotData<void(int)>(56, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 2 },
        }}),
        // Slot 'delete_coordinates'
        QtMocHelpers::SlotData<void()>(57, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'edit_coordinates_view'
        QtMocHelpers::SlotData<void()>(58, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'WGS_to_other'
        QtMocHelpers::SlotData<void()>(59, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'PLANE_to_other'
        QtMocHelpers::SlotData<void()>(60, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<Objectmanager, qt_meta_tag_ZN13ObjectmanagerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject Objectmanager::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13ObjectmanagerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13ObjectmanagerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN13ObjectmanagerE_t>.metaTypes,
    nullptr
} };

void Objectmanager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Objectmanager *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->customMenuTree((*reinterpret_cast< std::add_pointer_t<QPoint>>(_a[1]))); break;
        case 1: _t->customMenuView((*reinterpret_cast< std::add_pointer_t<QPoint>>(_a[1]))); break;
        case 2: _t->add_country_blok(); break;
        case 3: _t->delete_country_blok(); break;
        case 4: _t->update_one_click((*reinterpret_cast< std::add_pointer_t<QModelIndex>>(_a[1]))); break;
        case 5: _t->updateDB((*reinterpret_cast< std::add_pointer_t<QStandardItem*>>(_a[1]))); break;
        case 6: _t->show_nations_region(); break;
        case 7: _t->show_nations_ls(); break;
        case 8: _t->show_nations_gr(); break;
        case 9: _t->show_confess_region(); break;
        case 10: _t->show_confess_ls(); break;
        case 11: _t->show_confess_gr(); break;
        case 12: _t->show_profess_region(); break;
        case 13: _t->show_profess_gr(); break;
        case 14: _t->show_age_region(); break;
        case 15: _t->show_age_gr(); break;
        case 16: _t->show_sekas_region(); break;
        case 17: _t->show_sekas_gr(); break;
        case 18: _t->show_state_gr(); break;
        case 19: _t->show_state_ls(); break;
        case 20: _t->add_new_blok(); break;
        case 21: _t->get_path(); break;
        case 22: _t->get_path_edit(); break;
        case 23: _t->get_path_flag(); break;
        case 24: _t->get_path_flag_edit(); break;
        case 25: _t->delete_blok(); break;
        case 26: _t->delete_country(); break;
        case 27: _t->add_new_country(); break;
        case 28: _t->edit_country_blok(); break;
        case 29: _t->edit_country(); break;
        case 30: _t->show_redaktor_blok((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 31: _t->show_redaktor_country((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 32: _t->show_objects((*reinterpret_cast< std::add_pointer_t<QModelIndex>>(_a[1]))); break;
        case 33: _t->column_item_clicked((*reinterpret_cast< std::add_pointer_t<QModelIndex>>(_a[1]))); break;
        case 34: _t->delete_region(); break;
        case 35: _t->delete_smi(); break;
        case 36: _t->delete_ls(); break;
        case 37: _t->delete_groups(); break;
        case 38: _t->delete_mpo(); break;
        case 39: _t->delete_sc(); break;
        case 40: _t->delete_pers(); break;
        case 41: _t->edit_persones(); break;
        case 42: _t->clicked_open_file(); break;
        case 43: _t->slotSearchObject(); break;
        case 44: _t->otchet_groups(); break;
        case 45: _t->add_new_coordinates(); break;
        case 46: _t->show_dialog_add_file(); break;
        case 47: _t->get_coordinates((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[4]))); break;
        case 48: _t->change_coord_system((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 49: _t->delete_coordinates(); break;
        case 50: _t->edit_coordinates_view(); break;
        case 51: _t->WGS_to_other(); break;
        case 52: _t->PLANE_to_other(); break;
        default: ;
        }
    }
}

const QMetaObject *Objectmanager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Objectmanager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13ObjectmanagerE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int Objectmanager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 53)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 53;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 53)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 53;
    }
    return _id;
}
QT_WARNING_POP
