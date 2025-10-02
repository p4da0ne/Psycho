/****************************************************************************
** Meta object code from reading C++ file 'event.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../eventsmodel/event.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'event.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN5EventE_t {};
} // unnamed namespace

template <> constexpr inline auto Event::qt_create_metaobjectdata<qt_meta_tag_ZN5EventE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "Event",
        "MediaContentInserted",
        "",
        "id_media_event",
        "ErrorMediaContentInsert",
        "error",
        "InsertMediaItems",
        "path",
        "idMediaType",
        "name_event_media",
        "description",
        "insertEventToDB",
        "DeleteEvent",
        "id_event",
        "DeleteEventMedia",
        "id_event_media",
        "DeleteThisEventFromDB",
        "openMediaContent",
        "QModelIndex",
        "index",
        "getMediaEvents",
        "QStandardItemModel*",
        "updateMediaEvents",
        "getEventsStatus",
        "QMap<int,QString>",
        "getEventsTypes",
        "QMap<int,QMap<QString,int>>",
        "setName",
        "name",
        "setDescription",
        "setResume",
        "resume",
        "setStartDate",
        "start_date",
        "setEndDate",
        "end_date"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'MediaContentInserted'
        QtMocHelpers::SignalData<void(int)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 },
        }}),
        // Signal 'ErrorMediaContentInsert'
        QtMocHelpers::SignalData<void(QString)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 5 },
        }}),
        // Slot 'InsertMediaItems'
        QtMocHelpers::SlotData<int(QString, int, QString, QString)>(6, 2, QMC::AccessPublic, QMetaType::Int, {{
            { QMetaType::QString, 7 }, { QMetaType::Int, 8 }, { QMetaType::QString, 9 }, { QMetaType::QString, 10 },
        }}),
        // Slot 'InsertMediaItems'
        QtMocHelpers::SlotData<int(QString, int, QString)>(6, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Int, {{
            { QMetaType::QString, 7 }, { QMetaType::Int, 8 }, { QMetaType::QString, 9 },
        }}),
        // Slot 'insertEventToDB'
        QtMocHelpers::SlotData<bool()>(11, 2, QMC::AccessPublic, QMetaType::Bool),
        // Slot 'DeleteEvent'
        QtMocHelpers::SlotData<bool(int)>(12, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::Int, 13 },
        }}),
        // Slot 'DeleteEventMedia'
        QtMocHelpers::SlotData<bool(int)>(14, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::Int, 15 },
        }}),
        // Slot 'DeleteThisEventFromDB'
        QtMocHelpers::SlotData<bool()>(16, 2, QMC::AccessPublic, QMetaType::Bool),
        // Slot 'openMediaContent'
        QtMocHelpers::SlotData<void(QModelIndex)>(17, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 18, 19 },
        }}),
        // Slot 'getMediaEvents'
        QtMocHelpers::SlotData<QStandardItemModel *()>(20, 2, QMC::AccessPublic, 0x80000000 | 21),
        // Slot 'updateMediaEvents'
        QtMocHelpers::SlotData<void()>(22, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'getEventsStatus'
        QtMocHelpers::SlotData<QMap<int,QString>()>(23, 2, QMC::AccessPublic, 0x80000000 | 24),
        // Slot 'getEventsTypes'
        QtMocHelpers::SlotData<QMap<int,QMap<QString,int> >()>(25, 2, QMC::AccessPublic, 0x80000000 | 26),
        // Slot 'setName'
        QtMocHelpers::SlotData<bool(QString)>(27, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QString, 28 },
        }}),
        // Slot 'setDescription'
        QtMocHelpers::SlotData<bool(QString)>(29, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QString, 10 },
        }}),
        // Slot 'setResume'
        QtMocHelpers::SlotData<bool(QString)>(30, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QString, 31 },
        }}),
        // Slot 'setStartDate'
        QtMocHelpers::SlotData<bool(QDateTime)>(32, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QDateTime, 33 },
        }}),
        // Slot 'setEndDate'
        QtMocHelpers::SlotData<bool(QDateTime)>(34, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QDateTime, 35 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<Event, qt_meta_tag_ZN5EventE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject Event::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN5EventE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN5EventE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN5EventE_t>.metaTypes,
    nullptr
} };

void Event::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Event *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->MediaContentInserted((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 1: _t->ErrorMediaContentInsert((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 2: { int _r = _t->InsertMediaItems((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[4])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 3: { int _r = _t->InsertMediaItems((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 4: { bool _r = _t->insertEventToDB();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 5: { bool _r = _t->DeleteEvent((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 6: { bool _r = _t->DeleteEventMedia((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 7: { bool _r = _t->DeleteThisEventFromDB();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 8: _t->openMediaContent((*reinterpret_cast< std::add_pointer_t<QModelIndex>>(_a[1]))); break;
        case 9: { QStandardItemModel* _r = _t->getMediaEvents();
            if (_a[0]) *reinterpret_cast< QStandardItemModel**>(_a[0]) = std::move(_r); }  break;
        case 10: _t->updateMediaEvents(); break;
        case 11: { QMap<int,QString> _r = _t->getEventsStatus();
            if (_a[0]) *reinterpret_cast< QMap<int,QString>*>(_a[0]) = std::move(_r); }  break;
        case 12: { QMap<int,QMap<QString,int>> _r = _t->getEventsTypes();
            if (_a[0]) *reinterpret_cast< QMap<int,QMap<QString,int>>*>(_a[0]) = std::move(_r); }  break;
        case 13: { bool _r = _t->setName((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 14: { bool _r = _t->setDescription((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 15: { bool _r = _t->setResume((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 16: { bool _r = _t->setStartDate((*reinterpret_cast< std::add_pointer_t<QDateTime>>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 17: { bool _r = _t->setEndDate((*reinterpret_cast< std::add_pointer_t<QDateTime>>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (Event::*)(int )>(_a, &Event::MediaContentInserted, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (Event::*)(QString )>(_a, &Event::ErrorMediaContentInsert, 1))
            return;
    }
}

const QMetaObject *Event::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Event::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN5EventE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Event::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 18)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 18;
    }
    return _id;
}

// SIGNAL 0
void Event::MediaContentInserted(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void Event::ErrorMediaContentInsert(QString _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}
namespace {
struct qt_meta_tag_ZN17MediaInsertThreadE_t {};
} // unnamed namespace

template <> constexpr inline auto MediaInsertThread::qt_create_metaobjectdata<qt_meta_tag_ZN17MediaInsertThreadE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "MediaInsertThread",
        "MediaInserted",
        "",
        "id_event_media",
        "ErrorMediaInsert",
        "BigFile"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'MediaInserted'
        QtMocHelpers::SignalData<void(int)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 },
        }}),
        // Signal 'ErrorMediaInsert'
        QtMocHelpers::SignalData<void(QString)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 },
        }}),
        // Signal 'BigFile'
        QtMocHelpers::SignalData<void(QString)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 2 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<MediaInsertThread, qt_meta_tag_ZN17MediaInsertThreadE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject MediaInsertThread::staticMetaObject = { {
    QMetaObject::SuperData::link<QThread::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN17MediaInsertThreadE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN17MediaInsertThreadE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN17MediaInsertThreadE_t>.metaTypes,
    nullptr
} };

void MediaInsertThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<MediaInsertThread *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->MediaInserted((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 1: _t->ErrorMediaInsert((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 2: _t->BigFile((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (MediaInsertThread::*)(int )>(_a, &MediaInsertThread::MediaInserted, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (MediaInsertThread::*)(QString )>(_a, &MediaInsertThread::ErrorMediaInsert, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (MediaInsertThread::*)(QString )>(_a, &MediaInsertThread::BigFile, 2))
            return;
    }
}

const QMetaObject *MediaInsertThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MediaInsertThread::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN17MediaInsertThreadE_t>.strings))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int MediaInsertThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void MediaInsertThread::MediaInserted(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void MediaInsertThread::ErrorMediaInsert(QString _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void MediaInsertThread::BigFile(QString _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}
QT_WARNING_POP
