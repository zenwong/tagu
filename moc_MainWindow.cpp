/****************************************************************************
** Meta object code from reading C++ file 'MainWindow.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "MainWindow.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MainWindow.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[20];
    char stringdata[363];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_MainWindow_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10),
QT_MOC_LITERAL(1, 11, 19),
QT_MOC_LITERAL(2, 31, 0),
QT_MOC_LITERAL(3, 32, 5),
QT_MOC_LITERAL(4, 38, 25),
QT_MOC_LITERAL(5, 64, 25),
QT_MOC_LITERAL(6, 90, 25),
QT_MOC_LITERAL(7, 116, 24),
QT_MOC_LITERAL(8, 141, 4),
QT_MOC_LITERAL(9, 146, 31),
QT_MOC_LITERAL(10, 178, 15),
QT_MOC_LITERAL(11, 194, 13),
QT_MOC_LITERAL(12, 208, 11),
QT_MOC_LITERAL(13, 220, 14),
QT_MOC_LITERAL(14, 235, 9),
QT_MOC_LITERAL(15, 245, 15),
QT_MOC_LITERAL(16, 261, 16),
QT_MOC_LITERAL(17, 278, 31),
QT_MOC_LITERAL(18, 310, 25),
QT_MOC_LITERAL(19, 336, 25)
    },
    "MainWindow\0on_listView_clicked\0\0index\0"
    "on_listView_doubleClicked\0"
    "on_editActs_returnPressed\0"
    "on_editTags_returnPressed\0"
    "on_editSearch_textEdited\0arg1\0"
    "on_comboTag_currentIndexChanged\0"
    "onThumbnailView\0onCompactView\0onCoverView\0"
    "onImportVideos\0onOptions\0onResetDatabase\0"
    "onImportFinished\0on_comboAct_currentIndexChanged\0"
    "on_listTags_doubleClicked\0"
    "on_listActs_doubleClicked\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   94,    2, 0x08,
       4,    1,   97,    2, 0x08,
       5,    0,  100,    2, 0x08,
       6,    0,  101,    2, 0x08,
       7,    1,  102,    2, 0x08,
       9,    1,  105,    2, 0x08,
      10,    0,  108,    2, 0x08,
      11,    0,  109,    2, 0x08,
      12,    0,  110,    2, 0x08,
      13,    0,  111,    2, 0x08,
      14,    0,  112,    2, 0x08,
      15,    0,  113,    2, 0x08,
      16,    0,  114,    2, 0x08,
      17,    1,  115,    2, 0x08,
      18,    1,  118,    2, 0x08,
      19,    1,  121,    2, 0x08,

 // slots: parameters
    QMetaType::Void, QMetaType::QModelIndex,    3,
    QMetaType::Void, QMetaType::QModelIndex,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void, QMetaType::QModelIndex,    3,
    QMetaType::Void, QMetaType::QModelIndex,    3,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->on_listView_clicked((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 1: _t->on_listView_doubleClicked((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 2: _t->on_editActs_returnPressed(); break;
        case 3: _t->on_editTags_returnPressed(); break;
        case 4: _t->on_editSearch_textEdited((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: _t->on_comboTag_currentIndexChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: _t->onThumbnailView(); break;
        case 7: _t->onCompactView(); break;
        case 8: _t->onCoverView(); break;
        case 9: _t->onImportVideos(); break;
        case 10: _t->onOptions(); break;
        case 11: _t->onResetDatabase(); break;
        case 12: _t->onImportFinished(); break;
        case 13: _t->on_comboAct_currentIndexChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 14: _t->on_listTags_doubleClicked((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 15: _t->on_listActs_doubleClicked((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, 0, 0}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 16;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
