/****************************************************************************
** Meta object code from reading C++ file 'DetectTab.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../src/ui/tabs/DetectTab.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DetectTab.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_DetectTab_t {
    QByteArrayData data[16];
    char stringdata0[183];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DetectTab_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DetectTab_t qt_meta_stringdata_DetectTab = {
    {
QT_MOC_LITERAL(0, 0, 9), // "DetectTab"
QT_MOC_LITERAL(1, 10, 12), // "slotsChanged"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 17), // "QList<SampleSlot>"
QT_MOC_LITERAL(4, 42, 9), // "slotsList"
QT_MOC_LITERAL(5, 52, 13), // "onSlotClicked"
QT_MOC_LITERAL(6, 66, 5), // "index"
QT_MOC_LITERAL(7, 72, 15), // "onEditRequested"
QT_MOC_LITERAL(8, 88, 9), // "slotIndex"
QT_MOC_LITERAL(9, 98, 17), // "onDeleteRequested"
QT_MOC_LITERAL(10, 116, 10), // "QList<int>"
QT_MOC_LITERAL(11, 127, 7), // "indices"
QT_MOC_LITERAL(12, 135, 15), // "onFormSubmitted"
QT_MOC_LITERAL(13, 151, 10), // "SampleSlot"
QT_MOC_LITERAL(14, 162, 4), // "slot"
QT_MOC_LITERAL(15, 167, 15) // "onFormCancelled"

    },
    "DetectTab\0slotsChanged\0\0QList<SampleSlot>\0"
    "slotsList\0onSlotClicked\0index\0"
    "onEditRequested\0slotIndex\0onDeleteRequested\0"
    "QList<int>\0indices\0onFormSubmitted\0"
    "SampleSlot\0slot\0onFormCancelled"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DetectTab[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    1,   47,    2, 0x08 /* Private */,
       7,    1,   50,    2, 0x08 /* Private */,
       9,    1,   53,    2, 0x08 /* Private */,
      12,    2,   56,    2, 0x08 /* Private */,
      15,    0,   61,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 13,    8,   14,
    QMetaType::Void,

       0        // eod
};

void DetectTab::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<DetectTab *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->slotsChanged((*reinterpret_cast< const QList<SampleSlot>(*)>(_a[1]))); break;
        case 1: _t->onSlotClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->onEditRequested((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->onDeleteRequested((*reinterpret_cast< QList<int>(*)>(_a[1]))); break;
        case 4: _t->onFormSubmitted((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< SampleSlot(*)>(_a[2]))); break;
        case 5: _t->onFormCancelled(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<int> >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (DetectTab::*)(const QList<SampleSlot> & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DetectTab::slotsChanged)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject DetectTab::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_DetectTab.data,
    qt_meta_data_DetectTab,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *DetectTab::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DetectTab::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DetectTab.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int DetectTab::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void DetectTab::slotsChanged(const QList<SampleSlot> & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
