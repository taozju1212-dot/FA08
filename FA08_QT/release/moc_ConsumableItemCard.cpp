/****************************************************************************
** Meta object code from reading C++ file 'ConsumableItemCard.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../src/ui/widgets/ConsumableItemCard.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ConsumableItemCard.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ConsumableItemCard_t {
    QByteArrayData data[8];
    char stringdata0[113];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ConsumableItemCard_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ConsumableItemCard_t qt_meta_stringdata_ConsumableItemCard = {
    {
QT_MOC_LITERAL(0, 0, 18), // "ConsumableItemCard"
QT_MOC_LITERAL(1, 19, 18), // "panelOpenRequested"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 16), // "replaceRequested"
QT_MOC_LITERAL(4, 56, 6), // "newQty"
QT_MOC_LITERAL(5, 63, 16), // "onReplaceClicked"
QT_MOC_LITERAL(6, 80, 16), // "onConfirmClicked"
QT_MOC_LITERAL(7, 97, 15) // "onCancelClicked"

    },
    "ConsumableItemCard\0panelOpenRequested\0"
    "\0replaceRequested\0newQty\0onReplaceClicked\0"
    "onConfirmClicked\0onCancelClicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ConsumableItemCard[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x06 /* Public */,
       3,    1,   40,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   43,    2, 0x08 /* Private */,
       6,    0,   44,    2, 0x08 /* Private */,
       7,    0,   45,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    4,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ConsumableItemCard::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ConsumableItemCard *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->panelOpenRequested(); break;
        case 1: _t->replaceRequested((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->onReplaceClicked(); break;
        case 3: _t->onConfirmClicked(); break;
        case 4: _t->onCancelClicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ConsumableItemCard::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ConsumableItemCard::panelOpenRequested)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ConsumableItemCard::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ConsumableItemCard::replaceRequested)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ConsumableItemCard::staticMetaObject = { {
    QMetaObject::SuperData::link<QFrame::staticMetaObject>(),
    qt_meta_stringdata_ConsumableItemCard.data,
    qt_meta_data_ConsumableItemCard,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ConsumableItemCard::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ConsumableItemCard::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ConsumableItemCard.stringdata0))
        return static_cast<void*>(this);
    return QFrame::qt_metacast(_clname);
}

int ConsumableItemCard::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void ConsumableItemCard::panelOpenRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void ConsumableItemCard::replaceRequested(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
