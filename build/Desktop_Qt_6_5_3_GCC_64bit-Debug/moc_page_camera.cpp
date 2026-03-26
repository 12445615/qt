/****************************************************************************
** Meta object code from reading C++ file 'page_camera.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../page_camera.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'page_camera.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.5.3. It"
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

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSPageCameraENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSPageCameraENDCLASS = QtMocHelpers::stringData(
    "PageCamera",
    "frameReady",
    "",
    "img",
    "positionChanged",
    "pos",
    "durationChanged",
    "duration",
    "btn_play_clicked",
    "btn_next_clicked",
    "btn_volmeup_clicked",
    "btn_volmedown_clicked",
    "btn_fullscreen_clicked",
    "listWidgetClicked",
    "QListWidgetItem*",
    "item"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSPageCameraENDCLASS_t {
    uint offsetsAndSizes[32];
    char stringdata0[11];
    char stringdata1[11];
    char stringdata2[1];
    char stringdata3[4];
    char stringdata4[16];
    char stringdata5[4];
    char stringdata6[16];
    char stringdata7[9];
    char stringdata8[17];
    char stringdata9[17];
    char stringdata10[20];
    char stringdata11[22];
    char stringdata12[23];
    char stringdata13[18];
    char stringdata14[17];
    char stringdata15[5];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSPageCameraENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSPageCameraENDCLASS_t qt_meta_stringdata_CLASSPageCameraENDCLASS = {
    {
        QT_MOC_LITERAL(0, 10),  // "PageCamera"
        QT_MOC_LITERAL(11, 10),  // "frameReady"
        QT_MOC_LITERAL(22, 0),  // ""
        QT_MOC_LITERAL(23, 3),  // "img"
        QT_MOC_LITERAL(27, 15),  // "positionChanged"
        QT_MOC_LITERAL(43, 3),  // "pos"
        QT_MOC_LITERAL(47, 15),  // "durationChanged"
        QT_MOC_LITERAL(63, 8),  // "duration"
        QT_MOC_LITERAL(72, 16),  // "btn_play_clicked"
        QT_MOC_LITERAL(89, 16),  // "btn_next_clicked"
        QT_MOC_LITERAL(106, 19),  // "btn_volmeup_clicked"
        QT_MOC_LITERAL(126, 21),  // "btn_volmedown_clicked"
        QT_MOC_LITERAL(148, 22),  // "btn_fullscreen_clicked"
        QT_MOC_LITERAL(171, 17),  // "listWidgetClicked"
        QT_MOC_LITERAL(189, 16),  // "QListWidgetItem*"
        QT_MOC_LITERAL(206, 4)   // "item"
    },
    "PageCamera",
    "frameReady",
    "",
    "img",
    "positionChanged",
    "pos",
    "durationChanged",
    "duration",
    "btn_play_clicked",
    "btn_next_clicked",
    "btn_volmeup_clicked",
    "btn_volmedown_clicked",
    "btn_fullscreen_clicked",
    "listWidgetClicked",
    "QListWidgetItem*",
    "item"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSPageCameraENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   68,    2, 0x06,    1 /* Public */,
       4,    1,   71,    2, 0x06,    3 /* Public */,
       6,    1,   74,    2, 0x06,    5 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       8,    0,   77,    2, 0x08,    7 /* Private */,
       9,    0,   78,    2, 0x08,    8 /* Private */,
      10,    0,   79,    2, 0x08,    9 /* Private */,
      11,    0,   80,    2, 0x08,   10 /* Private */,
      12,    0,   81,    2, 0x08,   11 /* Private */,
      13,    1,   82,    2, 0x08,   12 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QImage,    3,
    QMetaType::Void, QMetaType::LongLong,    5,
    QMetaType::Void, QMetaType::LongLong,    7,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 14,   15,

       0        // eod
};

Q_CONSTINIT const QMetaObject PageCamera::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_CLASSPageCameraENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSPageCameraENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSPageCameraENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<PageCamera, std::true_type>,
        // method 'frameReady'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QImage &, std::false_type>,
        // method 'positionChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<qint64, std::false_type>,
        // method 'durationChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<qint64, std::false_type>,
        // method 'btn_play_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'btn_next_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'btn_volmeup_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'btn_volmedown_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'btn_fullscreen_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'listWidgetClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QListWidgetItem *, std::false_type>
    >,
    nullptr
} };

void PageCamera::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<PageCamera *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->frameReady((*reinterpret_cast< std::add_pointer_t<QImage>>(_a[1]))); break;
        case 1: _t->positionChanged((*reinterpret_cast< std::add_pointer_t<qint64>>(_a[1]))); break;
        case 2: _t->durationChanged((*reinterpret_cast< std::add_pointer_t<qint64>>(_a[1]))); break;
        case 3: _t->btn_play_clicked(); break;
        case 4: _t->btn_next_clicked(); break;
        case 5: _t->btn_volmeup_clicked(); break;
        case 6: _t->btn_volmedown_clicked(); break;
        case 7: _t->btn_fullscreen_clicked(); break;
        case 8: _t->listWidgetClicked((*reinterpret_cast< std::add_pointer_t<QListWidgetItem*>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (PageCamera::*)(const QImage & );
            if (_t _q_method = &PageCamera::frameReady; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (PageCamera::*)(qint64 );
            if (_t _q_method = &PageCamera::positionChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (PageCamera::*)(qint64 );
            if (_t _q_method = &PageCamera::durationChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject *PageCamera::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PageCamera::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSPageCameraENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int PageCamera::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void PageCamera::frameReady(const QImage & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void PageCamera::positionChanged(qint64 _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void PageCamera::durationChanged(qint64 _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
