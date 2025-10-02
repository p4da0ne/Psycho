/****************************************************************************
** Meta object code from reading C++ file 'event_manager.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../event_manager/event_manager.h"
#include <QtGui/qtextcursor.h>
#include <QtGui/qscreen.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'event_manager.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN12EventManagerE_t {};
} // unnamed namespace

template <> constexpr inline auto EventManager::qt_create_metaobjectdata<qt_meta_tag_ZN12EventManagerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "EventManager",
        "eventDataChanged",
        "",
        "addNewEventDialog",
        "QWidget*",
        "parent",
        "openNewEventDialog",
        "eventClick",
        "QModelIndex",
        "index",
        "mediaClick",
        "openFileDialog",
        "viewMediaContentDialog",
        "id_event",
        "setEventsPropertyEnabled",
        "enabled",
        "restEventsProperty",
        "nameLEEChanged",
        "text",
        "DTSEChanged",
        "dateTime",
        "DTEEChanged",
        "sourceTypeChange",
        "sourceTypeChangeE",
        "getTypeChange",
        "getTypeChangeE",
        "saveNewEvent",
        "filterNameTextChanged",
        "updateModel",
        "resizeTableView",
        "newEventMediaDialog",
        "addNewEventMedia",
        "MediaContentInserted",
        "ErrorDialog",
        "error",
        "EventsTableCustomMenu",
        "slotRemoveEvent",
        "EventsMediaTableCustomMenu",
        "pe",
        "slotRemoveEventMedia",
        "statusCBEChanged",
        "typeCBEChanged",
        "descriptionTEEChanged",
        "resumeTEEChanged",
        "CoordChanged",
        "suorceObjectCBNEEChanged",
        "getObjectCBNEEChanged"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'eventDataChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'addNewEventDialog'
        QtMocHelpers::SlotData<void(QWidget *)>(3, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 4, 5 },
        }}),
        // Slot 'openNewEventDialog'
        QtMocHelpers::SlotData<void()>(6, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'eventClick'
        QtMocHelpers::SlotData<void(QModelIndex)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 8, 9 },
        }}),
        // Slot 'mediaClick'
        QtMocHelpers::SlotData<void(QModelIndex)>(10, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 8, 2 },
        }}),
        // Slot 'openFileDialog'
        QtMocHelpers::SlotData<void()>(11, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'viewMediaContentDialog'
        QtMocHelpers::SlotData<void(int, QWidget *)>(12, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 13 }, { 0x80000000 | 4, 5 },
        }}),
        // Slot 'viewMediaContentDialog'
        QtMocHelpers::SlotData<void(int)>(12, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void, {{
            { QMetaType::Int, 13 },
        }}),
        // Slot 'setEventsPropertyEnabled'
        QtMocHelpers::SlotData<void(bool)>(14, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 15 },
        }}),
        // Slot 'restEventsProperty'
        QtMocHelpers::SlotData<void()>(16, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'nameLEEChanged'
        QtMocHelpers::SlotData<void(QString)>(17, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 18 },
        }}),
        // Slot 'DTSEChanged'
        QtMocHelpers::SlotData<void(QDateTime)>(19, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QDateTime, 20 },
        }}),
        // Slot 'DTEEChanged'
        QtMocHelpers::SlotData<void(QDateTime)>(21, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QDateTime, 20 },
        }}),
        // Slot 'sourceTypeChange'
        QtMocHelpers::SlotData<void(int)>(22, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 9 },
        }}),
        // Slot 'sourceTypeChangeE'
        QtMocHelpers::SlotData<void(int)>(23, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 9 },
        }}),
        // Slot 'getTypeChange'
        QtMocHelpers::SlotData<void(int)>(24, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 9 },
        }}),
        // Slot 'getTypeChangeE'
        QtMocHelpers::SlotData<void(int)>(25, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 9 },
        }}),
        // Slot 'saveNewEvent'
        QtMocHelpers::SlotData<void()>(26, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'filterNameTextChanged'
        QtMocHelpers::SlotData<void(QString)>(27, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 18 },
        }}),
        // Slot 'updateModel'
        QtMocHelpers::SlotData<void()>(28, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'resizeTableView'
        QtMocHelpers::SlotData<void()>(29, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'newEventMediaDialog'
        QtMocHelpers::SlotData<void()>(30, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'addNewEventMedia'
        QtMocHelpers::SlotData<void()>(31, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'MediaContentInserted'
        QtMocHelpers::SlotData<void(int)>(32, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 2 },
        }}),
        // Slot 'ErrorDialog'
        QtMocHelpers::SlotData<void(QString)>(33, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 34 },
        }}),
        // Slot 'EventsTableCustomMenu'
        QtMocHelpers::SlotData<void(const QPoint &)>(35, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QPoint, 2 },
        }}),
        // Slot 'slotRemoveEvent'
        QtMocHelpers::SlotData<void()>(36, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'EventsMediaTableCustomMenu'
        QtMocHelpers::SlotData<void(const QPoint &)>(37, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QPoint, 38 },
        }}),
        // Slot 'slotRemoveEventMedia'
        QtMocHelpers::SlotData<void()>(39, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'statusCBEChanged'
        QtMocHelpers::SlotData<void(int)>(40, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 9 },
        }}),
        // Slot 'typeCBEChanged'
        QtMocHelpers::SlotData<void(int)>(41, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 9 },
        }}),
        // Slot 'descriptionTEEChanged'
        QtMocHelpers::SlotData<void()>(42, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'resumeTEEChanged'
        QtMocHelpers::SlotData<void()>(43, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'CoordChanged'
        QtMocHelpers::SlotData<void()>(44, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'suorceObjectCBNEEChanged'
        QtMocHelpers::SlotData<void(int)>(45, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 9 },
        }}),
        // Slot 'getObjectCBNEEChanged'
        QtMocHelpers::SlotData<void(int)>(46, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 9 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<EventManager, qt_meta_tag_ZN12EventManagerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject EventManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12EventManagerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12EventManagerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN12EventManagerE_t>.metaTypes,
    nullptr
} };

void EventManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<EventManager *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->eventDataChanged(); break;
        case 1: _t->addNewEventDialog((*reinterpret_cast< std::add_pointer_t<QWidget*>>(_a[1]))); break;
        case 2: _t->openNewEventDialog(); break;
        case 3: _t->eventClick((*reinterpret_cast< std::add_pointer_t<QModelIndex>>(_a[1]))); break;
        case 4: _t->mediaClick((*reinterpret_cast< std::add_pointer_t<QModelIndex>>(_a[1]))); break;
        case 5: _t->openFileDialog(); break;
        case 6: _t->viewMediaContentDialog((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QWidget*>>(_a[2]))); break;
        case 7: _t->viewMediaContentDialog((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 8: _t->setEventsPropertyEnabled((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 9: _t->restEventsProperty(); break;
        case 10: _t->nameLEEChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 11: _t->DTSEChanged((*reinterpret_cast< std::add_pointer_t<QDateTime>>(_a[1]))); break;
        case 12: _t->DTEEChanged((*reinterpret_cast< std::add_pointer_t<QDateTime>>(_a[1]))); break;
        case 13: _t->sourceTypeChange((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 14: _t->sourceTypeChangeE((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 15: _t->getTypeChange((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 16: _t->getTypeChangeE((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 17: _t->saveNewEvent(); break;
        case 18: _t->filterNameTextChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 19: _t->updateModel(); break;
        case 20: _t->resizeTableView(); break;
        case 21: _t->newEventMediaDialog(); break;
        case 22: _t->addNewEventMedia(); break;
        case 23: _t->MediaContentInserted((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 24: _t->ErrorDialog((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 25: _t->EventsTableCustomMenu((*reinterpret_cast< std::add_pointer_t<QPoint>>(_a[1]))); break;
        case 26: _t->slotRemoveEvent(); break;
        case 27: _t->EventsMediaTableCustomMenu((*reinterpret_cast< std::add_pointer_t<QPoint>>(_a[1]))); break;
        case 28: _t->slotRemoveEventMedia(); break;
        case 29: _t->statusCBEChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 30: _t->typeCBEChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 31: _t->descriptionTEEChanged(); break;
        case 32: _t->resumeTEEChanged(); break;
        case 33: _t->CoordChanged(); break;
        case 34: _t->suorceObjectCBNEEChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 35: _t->getObjectCBNEEChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QWidget* >(); break;
            }
            break;
        case 6:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 1:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QWidget* >(); break;
            }
            break;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (EventManager::*)()>(_a, &EventManager::eventDataChanged, 0))
            return;
    }
}

const QMetaObject *EventManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *EventManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12EventManagerE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int EventManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 36)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 36;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 36)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 36;
    }
    return _id;
}

// SIGNAL 0
void EventManager::eventDataChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
