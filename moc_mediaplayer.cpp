/****************************************************************************
** Meta object code from reading C++ file 'mediaplayer.h'
**
** Created: Tue Jun 7 02:58:35 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "mediaplayer/mediaplayer.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mediaplayer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MediaPlayer[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      22,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x0a,
      24,   12,   12,   12, 0x0a,
      33,   12,   12,   12, 0x0a,
      43,   12,   12,   12, 0x0a,
      56,   12,   12,   12, 0x0a,
      69,   12,   12,   12, 0x0a,
      80,   12,   12,   12, 0x0a,
      92,   12,   12,   12, 0x0a,
     115,   12,   12,   12, 0x0a,
     139,   12,   12,   12, 0x08,
     154,   12,   12,   12, 0x08,
     168,   12,   12,   12, 0x08,
     187,   12,   12,   12, 0x08,
     204,   12,   12,   12, 0x08,
     216,   12,   12,   12, 0x08,
     253,  235,   12,   12, 0x08,
     295,   12,   12,   12, 0x08,
     311,   12,   12,   12, 0x08,
     332,   12,   12,   12, 0x08,
     364,  356,   12,   12, 0x08,
     382,   12,   12,   12, 0x08,
     392,   12,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MediaPlayer[] = {
    "MediaPlayer\0\0openFile()\0rewind()\0"
    "forward()\0updateInfo()\0updateTime()\0"
    "finished()\0playPause()\0scaleChanged(QAction*)\0"
    "aspectChanged(QAction*)\0setAspect(int)\0"
    "setScale(int)\0setSaturation(int)\0"
    "setContrast(int)\0setHue(int)\0"
    "setBrightness(int)\0newstate,oldstate\0"
    "stateChanged(Phonon::State,Phonon::State)\0"
    "effectChanged()\0showSettingsDialog()\0"
    "showContextMenu(QPoint)\0percent\0"
    "bufferStatus(int)\0openUrl()\0"
    "configureEffect()\0"
};

const QMetaObject MediaPlayer::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_MediaPlayer,
      qt_meta_data_MediaPlayer, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MediaPlayer::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MediaPlayer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MediaPlayer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MediaPlayer))
        return static_cast<void*>(const_cast< MediaPlayer*>(this));
    return QWidget::qt_metacast(_clname);
}

int MediaPlayer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: openFile(); break;
        case 1: rewind(); break;
        case 2: forward(); break;
        case 3: updateInfo(); break;
        case 4: updateTime(); break;
        case 5: finished(); break;
        case 6: playPause(); break;
        case 7: scaleChanged((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 8: aspectChanged((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 9: setAspect((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: setScale((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: setSaturation((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: setContrast((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: setHue((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: setBrightness((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: stateChanged((*reinterpret_cast< Phonon::State(*)>(_a[1])),(*reinterpret_cast< Phonon::State(*)>(_a[2]))); break;
        case 16: effectChanged(); break;
        case 17: showSettingsDialog(); break;
        case 18: showContextMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 19: bufferStatus((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 20: openUrl(); break;
        case 21: configureEffect(); break;
        default: ;
        }
        _id -= 22;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
