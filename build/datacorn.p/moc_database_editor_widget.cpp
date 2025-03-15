/****************************************************************************
** Meta object code from reading C++ file 'database_editor_widget.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.16)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../src/gui/database_editor_widget.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'database_editor_widget.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.16. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_DatabaseEditorWidget_t {
    QByteArrayData data[21];
    char stringdata0[302];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DatabaseEditorWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DatabaseEditorWidget_t qt_meta_stringdata_DatabaseEditorWidget = {
    {
QT_MOC_LITERAL(0, 0, 20), // "DatabaseEditorWidget"
QT_MOC_LITERAL(1, 21, 7), // "newCard"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 8), // "saveData"
QT_MOC_LITERAL(4, 39, 15), // "refreshCardList"
QT_MOC_LITERAL(5, 55, 18), // "addArchetypeToList"
QT_MOC_LITERAL(6, 74, 7), // "clicked"
QT_MOC_LITERAL(7, 82, 23), // "removeArchetypeFromList"
QT_MOC_LITERAL(8, 106, 22), // "onArcheListItemChanged"
QT_MOC_LITERAL(9, 129, 16), // "QListWidgetItem*"
QT_MOC_LITERAL(10, 146, 7), // "current"
QT_MOC_LITERAL(11, 154, 8), // "previous"
QT_MOC_LITERAL(12, 163, 26), // "onArcheComboIndexActivated"
QT_MOC_LITERAL(13, 190, 5), // "index"
QT_MOC_LITERAL(14, 196, 27), // "onArcheComboEditTextChanged"
QT_MOC_LITERAL(15, 224, 4), // "text"
QT_MOC_LITERAL(16, 229, 22), // "onCardsListItemClicked"
QT_MOC_LITERAL(17, 252, 11), // "QModelIndex"
QT_MOC_LITERAL(18, 264, 21), // "onListCheckboxClicked"
QT_MOC_LITERAL(19, 286, 4), // "item"
QT_MOC_LITERAL(20, 291, 10) // "setUnsaved"

    },
    "DatabaseEditorWidget\0newCard\0\0saveData\0"
    "refreshCardList\0addArchetypeToList\0"
    "clicked\0removeArchetypeFromList\0"
    "onArcheListItemChanged\0QListWidgetItem*\0"
    "current\0previous\0onArcheComboIndexActivated\0"
    "index\0onArcheComboEditTextChanged\0"
    "text\0onCardsListItemClicked\0QModelIndex\0"
    "onListCheckboxClicked\0item\0setUnsaved"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DatabaseEditorWidget[] = {

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
       1,    0,   69,    2, 0x0a /* Public */,
       3,    0,   70,    2, 0x0a /* Public */,
       4,    0,   71,    2, 0x0a /* Public */,
       5,    1,   72,    2, 0x08 /* Private */,
       7,    1,   75,    2, 0x08 /* Private */,
       8,    2,   78,    2, 0x08 /* Private */,
      12,    1,   83,    2, 0x08 /* Private */,
      14,    1,   86,    2, 0x08 /* Private */,
      16,    1,   89,    2, 0x08 /* Private */,
      18,    1,   92,    2, 0x08 /* Private */,
      20,    0,   95,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    6,
    QMetaType::Void, QMetaType::Bool,    6,
    QMetaType::Void, 0x80000000 | 9, 0x80000000 | 9,   10,   11,
    QMetaType::Void, QMetaType::Int,   13,
    QMetaType::Void, QMetaType::QString,   15,
    QMetaType::Void, 0x80000000 | 17,   13,
    QMetaType::Void, 0x80000000 | 9,   19,
    QMetaType::Void,

       0        // eod
};

void DatabaseEditorWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<DatabaseEditorWidget *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->newCard(); break;
        case 1: _t->saveData(); break;
        case 2: _t->refreshCardList(); break;
        case 3: _t->addArchetypeToList((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->removeArchetypeFromList((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->onArcheListItemChanged((*reinterpret_cast< QListWidgetItem*(*)>(_a[1])),(*reinterpret_cast< QListWidgetItem*(*)>(_a[2]))); break;
        case 6: _t->onArcheComboIndexActivated((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->onArcheComboEditTextChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 8: _t->onCardsListItemClicked((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 9: _t->onListCheckboxClicked((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 10: _t->setUnsaved(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject DatabaseEditorWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_DatabaseEditorWidget.data,
    qt_meta_data_DatabaseEditorWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *DatabaseEditorWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DatabaseEditorWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DatabaseEditorWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int DatabaseEditorWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
