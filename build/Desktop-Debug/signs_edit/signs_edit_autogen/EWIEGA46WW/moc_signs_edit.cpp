/****************************************************************************
** Meta object code from reading C++ file 'signs_edit.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../signs_edit/signs_edit.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'signs_edit.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN9SignsEditE_t {};
} // unnamed namespace

template <> constexpr inline auto SignsEdit::qt_create_metaobjectdata<qt_meta_tag_ZN9SignsEditE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "SignsEdit",
        "show_object_types",
        "",
        "show_signs_table",
        "QModelIndex",
        "index",
        "create_signs_table",
        "clear_tableWidget",
        "QTableWidget*",
        "table",
        "get_path",
        "add_new_sign",
        "delete_sign",
        "row",
        "column",
        "save_changes"
    };

    QtMocHelpers::UintData qt_methods {
        // Slot 'show_object_types'
        QtMocHelpers::SlotData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'show_signs_table'
        QtMocHelpers::SlotData<void(const QModelIndex &)>(3, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 4, 5 },
        }}),
        // Slot 'create_signs_table'
        QtMocHelpers::SlotData<void(const QModelIndex &)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 4, 5 },
        }}),
        // Slot 'clear_tableWidget'
        QtMocHelpers::SlotData<void(QTableWidget *)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 8, 9 },
        }}),
        // Slot 'get_path'
        QtMocHelpers::SlotData<void()>(10, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'add_new_sign'
        QtMocHelpers::SlotData<void()>(11, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'delete_sign'
        QtMocHelpers::SlotData<void(int, int)>(12, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 13 }, { QMetaType::Int, 14 },
        }}),
        // Slot 'save_changes'
        QtMocHelpers::SlotData<void()>(15, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<SignsEdit, qt_meta_tag_ZN9SignsEditE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject SignsEdit::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9SignsEditE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9SignsEditE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN9SignsEditE_t>.metaTypes,
    nullptr
} };

void SignsEdit::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<SignsEdit *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->show_object_types(); break;
        case 1: _t->show_signs_table((*reinterpret_cast< std::add_pointer_t<QModelIndex>>(_a[1]))); break;
        case 2: _t->create_signs_table((*reinterpret_cast< std::add_pointer_t<QModelIndex>>(_a[1]))); break;
        case 3: _t->clear_tableWidget((*reinterpret_cast< std::add_pointer_t<QTableWidget*>>(_a[1]))); break;
        case 4: _t->get_path(); break;
        case 5: _t->add_new_sign(); break;
        case 6: _t->delete_sign((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 7: _t->save_changes(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QTableWidget* >(); break;
            }
            break;
        }
    }
}

const QMetaObject *SignsEdit::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SignsEdit::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9SignsEditE_t>.strings))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int SignsEdit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}
QT_WARNING_POP
