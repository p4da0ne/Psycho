/****************************************************************************
** Meta object code from reading C++ file 'simledelegate.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../object_manager/simledelegate.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'simledelegate.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN13simleDelegateE_t {};
} // unnamed namespace

template <> constexpr inline auto simleDelegate::qt_create_metaobjectdata<qt_meta_tag_ZN13simleDelegateE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "simleDelegate"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<simleDelegate, qt_meta_tag_ZN13simleDelegateE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject simleDelegate::staticMetaObject = { {
    QMetaObject::SuperData::link<QSqlRelationalDelegate::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13simleDelegateE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13simleDelegateE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN13simleDelegateE_t>.metaTypes,
    nullptr
} };

void simleDelegate::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<simleDelegate *>(_o);
    (void)_t;
    (void)_c;
    (void)_id;
    (void)_a;
}

const QMetaObject *simleDelegate::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *simleDelegate::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13simleDelegateE_t>.strings))
        return static_cast<void*>(this);
    return QSqlRelationalDelegate::qt_metacast(_clname);
}

int simleDelegate::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QSqlRelationalDelegate::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP
