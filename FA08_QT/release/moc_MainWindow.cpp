/****************************************************************************
** Meta object code from reading C++ file 'MainWindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../src/ui/MainWindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MainWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[21];
    char stringdata0[252];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 19), // "onTemperatureUpdate"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 7), // "celsius"
QT_MOC_LITERAL(4, 40, 21), // "onDeviceStatusChanged"
QT_MOC_LITERAL(5, 62, 12), // "DeviceStatus"
QT_MOC_LITERAL(6, 75, 6), // "status"
QT_MOC_LITERAL(7, 82, 25), // "onSampleSlotStatusChanged"
QT_MOC_LITERAL(8, 108, 9), // "slotIndex"
QT_MOC_LITERAL(9, 118, 12), // "SampleStatus"
QT_MOC_LITERAL(10, 131, 16), // "onBarcodeScanned"
QT_MOC_LITERAL(11, 148, 7), // "barcode"
QT_MOC_LITERAL(12, 156, 14), // "onLisConnected"
QT_MOC_LITERAL(13, 171, 9), // "connected"
QT_MOC_LITERAL(14, 181, 12), // "onTabChanged"
QT_MOC_LITERAL(15, 194, 5), // "index"
QT_MOC_LITERAL(16, 200, 7), // "onStart"
QT_MOC_LITERAL(17, 208, 7), // "onPause"
QT_MOC_LITERAL(18, 216, 9), // "onAddCard"
QT_MOC_LITERAL(19, 226, 12), // "onAlertsBell"
QT_MOC_LITERAL(20, 239, 12) // "simulateTick"

    },
    "MainWindow\0onTemperatureUpdate\0\0celsius\0"
    "onDeviceStatusChanged\0DeviceStatus\0"
    "status\0onSampleSlotStatusChanged\0"
    "slotIndex\0SampleStatus\0onBarcodeScanned\0"
    "barcode\0onLisConnected\0connected\0"
    "onTabChanged\0index\0onStart\0onPause\0"
    "onAddCard\0onAlertsBell\0simulateTick"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   69,    2, 0x0a /* Public */,
       4,    1,   72,    2, 0x0a /* Public */,
       7,    2,   75,    2, 0x0a /* Public */,
      10,    1,   80,    2, 0x0a /* Public */,
      12,    1,   83,    2, 0x0a /* Public */,
      14,    1,   86,    2, 0x08 /* Private */,
      16,    0,   89,    2, 0x08 /* Private */,
      17,    0,   90,    2, 0x08 /* Private */,
      18,    0,   91,    2, 0x08 /* Private */,
      19,    0,   92,    2, 0x08 /* Private */,
      20,    0,   93,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 9,    8,    6,
    QMetaType::Void, QMetaType::QString,   11,
    QMetaType::Void, QMetaType::Bool,   13,
    QMetaType::Void, QMetaType::Int,   15,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onTemperatureUpdate((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 1: _t->onDeviceStatusChanged((*reinterpret_cast< DeviceStatus(*)>(_a[1]))); break;
        case 2: _t->onSampleSlotStatusChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< SampleStatus(*)>(_a[2]))); break;
        case 3: _t->onBarcodeScanned((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->onLisConnected((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->onTabChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->onStart(); break;
        case 7: _t->onPause(); break;
        case 8: _t->onAddCard(); break;
        case 9: _t->onAlertsBell(); break;
        case 10: _t->simulateTick(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_MainWindow.data,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
