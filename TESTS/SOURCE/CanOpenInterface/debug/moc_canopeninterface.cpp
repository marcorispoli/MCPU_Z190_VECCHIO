/****************************************************************************
** Meta object code from reading C++ file 'canopeninterface.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../../MAIN_CPU/SOURCE/LIB/canopeninterface.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'canopeninterface.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_canOpenInterface_t {
    const uint offsetsAndSize[16];
    char stringdata0[110];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_canOpenInterface_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_canOpenInterface_t qt_meta_stringdata_canOpenInterface = {
    {
QT_MOC_LITERAL(0, 16), // "canOpenInterface"
QT_MOC_LITERAL(17, 25), // "registerActionCompleteSgn"
QT_MOC_LITERAL(43, 0), // ""
QT_MOC_LITERAL(44, 5), // "index"
QT_MOC_LITERAL(50, 12), // "deviceRxSlot"
QT_MOC_LITERAL(63, 32), // "canDriverInterface::canDataFrame"
QT_MOC_LITERAL(96, 5), // "frame"
QT_MOC_LITERAL(102, 7) // "odIndex"

    },
    "canOpenInterface\0registerActionCompleteSgn\0"
    "\0index\0deviceRxSlot\0"
    "canDriverInterface::canDataFrame\0frame\0"
    "odIndex"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_canOpenInterface[] = {

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
       1,    1,   26,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       4,    2,   29,    2, 0x09,    3 /* Protected */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 5, QMetaType::Int,    6,    7,

       0        // eod
};

void canOpenInterface::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<canOpenInterface *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->registerActionCompleteSgn((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->deviceRxSlot((*reinterpret_cast< canDriverInterface::canDataFrame(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (canOpenInterface::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&canOpenInterface::registerActionCompleteSgn)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject canOpenInterface::staticMetaObject = { {
    QMetaObject::SuperData::link<canDriverUserClass::staticMetaObject>(),
    qt_meta_stringdata_canOpenInterface.offsetsAndSize,
    qt_meta_data_canOpenInterface,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_canOpenInterface_t
, QtPrivate::TypeAndForceComplete<canOpenInterface, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<canDriverInterface::canDataFrame, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>


>,
    nullptr
} };


const QMetaObject *canOpenInterface::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *canOpenInterface::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_canOpenInterface.stringdata0))
        return static_cast<void*>(this);
    return canDriverUserClass::qt_metacast(_clname);
}

int canOpenInterface::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = canDriverUserClass::qt_metacall(_c, _id, _a);
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
void canOpenInterface::registerActionCompleteSgn(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
