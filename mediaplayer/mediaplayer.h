#ifndef MEDIALAYER_H
#define MEDIAPLAYER_H

#include <QtGui/QWidget>
#include <QtGui/QApplication>
#include <QtCore/QTimerEvent>
#include <QtGui/QShowEvent>
#include <QtGui/QIcon>

#include </usr/include/qt4/phonon/Phonon/AudioOutput>
#include </usr/include/qt4/phonon/Phonon/BackendCapabilities>
#include </usr/include/qt4/phonon/Phonon/Effect>
#include </usr/include/qt4/phonon/Phonon/EffectParameter>
#include </usr/include/qt4/phonon/Phonon/EffectWidget>
#include </usr/include/qt4/phonon/Phonon/MediaObject>
#include </usr/include/qt4/phonon/Phonon/SeekSlider>
#include </usr/include/qt4/phonon/Phonon/VideoWidget>
#include </usr/include/qt4/phonon/Phonon/VolumeSlider>

QT_BEGIN_NAMESPACE
class QPushButton;
class QLabel;
class QSlider;
class QTextEdit;
class QMenu;
class Ui_settings;
QT_END_NAMESPACE

class MediaPlayer :
            public QWidget
{
    Q_OBJECT
public:
    MediaPlayer(const QString &);
    
    void dragEnterEvent(QDragEnterEvent *e);
    void dragMoveEvent(QDragMoveEvent *e);
    void dropEvent(QDropEvent *e);
    void handleDrop(QDropEvent *e);
    void setFile(const QString &text);
    void initVideoWindow();
    void initSettingsDialog();
    void setVolume(qreal volume);
    
public slots:
    void openFile();
    void rewind();
    void forward();
    void updateInfo();
    void updateTime();
    void finished();
    void playPause();
    void scaleChanged(QAction *);
    void aspectChanged(QAction *);

private slots:
    void setAspect(int);
    void setScale(int);
    void setSaturation(int);
    void setContrast(int);
    void setHue(int);
    void setBrightness(int);
    void stateChanged(Phonon::State newstate, Phonon::State oldstate);
    void effectChanged();
    void showSettingsDialog();
    void showContextMenu(const QPoint &);
    void bufferStatus(int percent);
    void openUrl();
    void configureEffect();

private:
    QIcon playIcon;
    QIcon pauseIcon;
    QMenu *fileMenu;
    QPushButton *playButton;
    QPushButton *rewindButton;
    QPushButton *forwardButton;
    Phonon::SeekSlider *slider;
    QLabel *timeLabel;
    QLabel *progressLabel;
    QSlider *m_hueSlider;
    QSlider *m_satSlider;
    QSlider *m_contSlider;
    QLabel *info;
    Phonon::Effect *nextEffect;
    QDialog *settingsDialog;
    Ui_settings *ui;
        
    QWidget m_videoWindow;
    Phonon::MediaObject m_MediaObject;
    Phonon::AudioOutput m_AudioOutput;
    Phonon::VideoWidget *m_videoWidget;
    Phonon::Path m_audioOutputPath;
};

#endif //MEDIAPLAYER_H
