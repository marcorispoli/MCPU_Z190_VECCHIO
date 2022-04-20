/****************************************************************************
** Meta object code from reading C++ file 'canDriverClassInterface.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../SOURCE/LIB/canDriverClassInterface.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'canDriverClassInterface.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_canDriverInterface_t {
    const uint offsetsAndSize[18];
    char stringdata0[104];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_canDriverInterface_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_canDriverInterface_t qt_meta_stringdata_canDriverInterface = {
    {
QT_MOC_LITERAL(0, 18), // "canDriverInterface"
QT_MOC_LITERAL(19, 12), // "workFinished"
QT_MOC_LITERAL(32, 0), // ""
QT_MOC_LITERAL(33, 12), // "driverTxSlot"
QT_MOC_LITERAL(46, 32), // "canDriverInterface::canDataFrame"
QT_MOC_LITERAL(79, 5), // "frame"
QT_MOC_LITERAL(85, 7), // "odIndex"
QT_MOC_LITERAL(93, 3), // "tmo"
QT_MOC_LITERAL(97, 6) // "device"

    },
    "canDriverInterface\0workFinished\0\0"
    "driverTxSlot\0canDriverInterface::canDataFrame\0"
    "frame\0odIndex\0tmo\0device"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_canDriverInterface[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   26,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       3,    4,   27,    2, 0x0a,    2 /* Public */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 4, QMetaType::Int, QMetaType::UInt, QMetaType::QObjectStar,    5,    6,    7,    8,

       0        // eod
};

void canDriverInterface::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<canDriverInterface *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->workFinished(); break;
        case 1: _t->driverTxSlot((*reinterpret_cast< canDriverInterface::canDataFrame(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< uint(*)>(_a[3])),(*reinterpret_cast< QObject*(*)>(_a[4]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (canDriverInterface::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&canDriverInterface::workFinished)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject canDriverInterface::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_canDriverInterface.offsetsAndSize,
    qt_meta_data_canDriverInterface,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_canDriverInterface_t
, QtPrivate::TypeAndForceComplete<canDriverInterface, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<canDriverInterface::canDataFrame, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<uint, std::false_type>, QtPrivate::TypeAndForceComplete<QObject *, std::false_type>


>,
    nullptr
} };


const QMetaObject *canDriverInterface::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *canDriverInterface::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_canDriverInterface.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int canDriverInterface::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void canDriverInterface::workFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
struct qt_meta_stringdata_canDriverUserClass_t {
    const uint offsetsAndSize[20];
    char stringdata0[113];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_canDriverUserClass_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_canDriverUserClass_t qt_meta_stringdata_canDriverUserClass = {
    {
QT_MOC_LITERAL(0, 18), // "canDriverUserClass"
QT_MOC_LITERAL(19, 10), // "txToDriver"
QT_MOC_LITERAL(30, 0), // ""
QT_MOC_LITERAL(31, 32), // "canDriverInterface::canDataFrame"
QT_MOC_LITERAL(64, 5), // "frame"
QT_MOC_LITERAL(70, 7), // "odIndex"
QT_MOC_LITERAL(78, 3), // "tmo"
QT_MOC_LITERAL(82, 6), // "device"
QT_MOC_LITERAL(89, 12), // "deviceRxSlot"
QT_MOC_LITERAL(102, 10) // "canTxFrame"

    },
    "canDriverUserClass\0txToDriver\0\0"
    "canDriverInterface::canDataFrame\0frame\0"
    "odIndex\0tmo\0device\0deviceRxSlot\0"
    "canTxFrame"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_canDriverUserClass[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    4,   32,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       8,    2,   41,    2, 0x0a,    6 /* Public */,
       9,    3,   46,    2, 0x0a,    9 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::Int, QMetaType::UInt, QMetaType::QObjectStar,    4,    5,    6,    7,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::Int,    4,    5,
    QMetaType::Void, 0x80000000 | 3, QMetaType::Int, QMetaType::UInt,    4,    5,    6,

       0        // eod
};

void canDriverUserClass::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<canDriverUserClass *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->txToDriver((*reinterpret_cast< canDriverInterface::canDataFrame(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< uint(*)>(_a[3])),(*reinterpret_cast< QObject*(*)>(_a[4]))); break;
        case 1: _t->deviceRxSlot((*reinterpret_cast< canDriverInterface::canDataFrame(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->canTxFrame((*reinterpret_cast< canDriverInterface::canDataFrame(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< uint(*)>(_a[3]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (canDriverUserClass::*)(canDriverInterface::canDataFrame , int , uint , QObject * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&canDriverUserClass::txToDriver)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject canDriverUserClass::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_canDriverUserClass.offsetsAndSize,
    qt_meta_data_canDriverUserClass,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_canDriverUserClass_t
, QtPrivate::TypeAndForceComplete<canDriverUserClass, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<canDriverInterface::canDataFrame, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<uint, std::false_type>, QtPrivate::TypeAndForceComplete<QObject *, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<canDriverInterface::canDataFrame, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<canDriverInterface::canDataFrame, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<uint, std::false_type>


>,
    nullptr
} };


const QMetaObject *canDriverUserClass::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *canDriverUserClass::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_canDriverUserClass.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int canDriverUserClass::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void canDriverUserClass::txToDriver(canDriverInterface::canDataFrame _t1, int _t2, uint _t3, QObject * _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
