/****************************************************************************
** Meta object code from reading C++ file 'page_mechinemanager.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../page_mechinemanager.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'page_mechinemanager.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSPageMechineManagerENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSPageMechineManagerENDCLASS = QtMocHelpers::stringData(
    "PageMechineManager",
    "updateCPU",
    "",
    "value",
    "updateMemory",
    "updateNPU",
    "updateDisk",
    "updateOTAProgress",
    "appendOTALog",
    "log",
    "appendLog"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSPageMechineManagerENDCLASS_t {
    uint offsetsAndSizes[22];
    char stringdata0[19];
    char stringdata1[10];
    char stringdata2[1];
    char stringdata3[6];
    char stringdata4[13];
    char stringdata5[10];
    char stringdata6[11];
    char stringdata7[18];
    char stringdata8[13];
    char stringdata9[4];
    char stringdata10[10];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSPageMechineManagerENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSPageMechineManagerENDCLASS_t qt_meta_stringdata_CLASSPageMechineManagerENDCLASS = {
    {
        QT_MOC_LITERAL(0, 18),  // "PageMechineManager"
        QT_MOC_LITERAL(19, 9),  // "updateCPU"
        QT_MOC_LITERAL(29, 0),  // ""
        QT_MOC_LITERAL(30, 5),  // "value"
        QT_MOC_LITERAL(36, 12),  // "updateMemory"
        QT_MOC_LITERAL(49, 9),  // "updateNPU"
        QT_MOC_LITERAL(59, 10),  // "updateDisk"
        QT_MOC_LITERAL(70, 17),  // "updateOTAProgress"
        QT_MOC_LITERAL(88, 12),  // "appendOTALog"
        QT_MOC_LITERAL(101, 3),  // "log"
        QT_MOC_LITERAL(105, 9)   // "appendLog"
    },
    "PageMechineManager",
    "updateCPU",
    "",
    "value",
    "updateMemory",
    "updateNPU",
    "updateDisk",
    "updateOTAProgress",
    "appendOTALog",
    "log",
    "appendLog"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSPageMechineManagerENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   56,    2, 0x0a,    1 /* Public */,
       4,    1,   59,    2, 0x0a,    3 /* Public */,
       5,    1,   62,    2, 0x0a,    5 /* Public */,
       6,    1,   65,    2, 0x0a,    7 /* Public */,
       7,    1,   68,    2, 0x0a,    9 /* Public */,
       8,    1,   71,    2, 0x0a,   11 /* Public */,
      10,    1,   74,    2, 0x0a,   13 /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Float,    3,
    QMetaType::Void, QMetaType::Float,    3,
    QMetaType::Void, QMetaType::Float,    3,
    QMetaType::Void, QMetaType::Float,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::QString,    9,

       0        // eod
};

Q_CONSTINIT const QMetaObject PageMechineManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_CLASSPageMechineManagerENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSPageMechineManagerENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSPageMechineManagerENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<PageMechineManager, std::true_type>,
        // method 'updateCPU'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<float, std::false_type>,
        // method 'updateMemory'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<float, std::false_type>,
        // method 'updateNPU'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<float, std::false_type>,
        // method 'updateDisk'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<float, std::false_type>,
        // method 'updateOTAProgress'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'appendOTALog'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'appendLog'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>
    >,
    nullptr
} };

void PageMechineManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<PageMechineManager *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->updateCPU((*reinterpret_cast< std::add_pointer_t<float>>(_a[1]))); break;
        case 1: _t->updateMemory((*reinterpret_cast< std::add_pointer_t<float>>(_a[1]))); break;
        case 2: _t->updateNPU((*reinterpret_cast< std::add_pointer_t<float>>(_a[1]))); break;
        case 3: _t->updateDisk((*reinterpret_cast< std::add_pointer_t<float>>(_a[1]))); break;
        case 4: _t->updateOTAProgress((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 5: _t->appendOTALog((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 6: _t->appendLog((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject *PageMechineManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PageMechineManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSPageMechineManagerENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int PageMechineManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 7;
    }
    return _id;
}
QT_WARNING_POP
