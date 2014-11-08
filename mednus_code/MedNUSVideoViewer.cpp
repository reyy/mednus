#include "MedNUSVideoViewer.h"
#include <QDebug>
#include "MedNUSAUISettings.h"

MedNUSVideoViewer::MedNUSVideoViewer(QString filename, QWidget *parent) :
    QWidget(parent)
{
    this->setAccessibleName(filename);

    videoView = new QGraphicsView();
    scene = new QGraphicsScene(videoView);
    videoView->setScene(scene);
    videoView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    videoView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    videoItem = new QGraphicsVideoItem;

    mediaPlayer.setVideoOutput(videoItem);
    mediaPlayer.setMedia(QUrl::fromLocalFile(filename));
    mediaPlayer.play();
    mediaPlayer.pause();

    scene->addItem(videoItem);

    videoView->setAutoFillBackground(true);
    videoView->setBackgroundBrush(QBrush(Qt::black, Qt::SolidPattern));
    videoView->setStyleSheet( "QGraphicsView { border-style: none; background-color:black}" );

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(videoView);
    layout->setSpacing(0);
    layout->setMargin(0);

    this->setLayout(layout);
    this->setFocusPolicy(Qt::StrongFocus);
    this->installEventFilter(this);

    control = new MedNUSVideoControl(videoView);

    connect(&mediaPlayer, SIGNAL(stateChanged(QMediaPlayer::State)),
            control, SLOT(mediaStateChanged(QMediaPlayer::State)));
    connect(&mediaPlayer, SIGNAL(positionChanged(qint64)), control, SLOT(positionChanged(qint64)));
    connect(&mediaPlayer, SIGNAL(durationChanged(qint64)), control, SLOT(durationChanged(qint64)));
}

MedNUSVideoViewer::~MedNUSVideoViewer()
{
    mediaPlayer.stop();
    removeEventFilter(this);

    layout()->removeWidget(videoView);
    delete control;
    delete videoView;
}


void MedNUSVideoViewer::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Space)
        this->togglePlay();
    else
        event->ignore();
}

void MedNUSVideoViewer::resizeEvent(QResizeEvent *event)
{
    videoView->setGeometry(this->geometry());
    videoView->setSceneRect(QRectF(0, 0,this->width(), this->height()));
    videoItem->setSize(videoView->size());

    videoView->lower();
    control->raise();
    control->setGeometry(0, 0, this->width(), this->height());
    control->updateUI();
}

bool MedNUSVideoViewer::eventFilter(QObject *obj, QEvent *e)
{
    if(e->type() == QEvent::Enter)
        control->show();
    if(e->type() == QEvent::Leave)
        control->hide();
}

void MedNUSVideoViewer::togglePlay()
{
    if(mediaPlayer.state() == QMediaPlayer::PlayingState)
        mediaPlayer.pause();
    else
        mediaPlayer.play();
}

void MedNUSVideoViewer::setPosition(int position)
{
    mediaPlayer.setPosition(position);
}


MedNUSVideoControl::MedNUSVideoControl(QWidget *parent):
    QWidget(parent)
{
    _volume=3;
    _playButton = new QPushButton((this));
    _playButton->setIconSize(QSize(24,24));
    _playButton->setFlat(true);
    _playButton->setStyleSheet("QPushButton {border-style: outset; border-width: 0px;background-image: url(:/images/bt_play.png); background-color:rgba(0,0,0,0);}");

    _volumeButton = new QPushButton((this));
    _volumeButton->setIconSize(QSize(30,24));
    _volumeButton->setFlat(true);
    _volumeButton->setStyleSheet("QPushButton {border-style: outset; border-width: 0px;background-image: url(:/images/bt_volume_3.png); background-color:rgba(0,0,0,0);}");

    _positionSlider = new QSlider(Qt::Horizontal,this);
    _positionSlider->setRange(0, 0);
    _positionSlider->setStyleSheet("QSlider {background-color:rgba(0,0,0,0);} QSlider::groove:horizontal {border: 0px solid #FFFFFF;height: 8px;background-color:rgba(255,255,255,128) ;margin: 2px 0;} QSlider::handle:horizontal {background: #FFFFFF;border: 1px solid #EEEEEE;width: 18px;margin: -2px 0;border-radius: 2px;}");

    _videoTimer = new QLabel(this);
    _videoTimer->setStyleSheet("color:#FFFFFF;font-size:8px;text-align:center;background-color:rgba(0,0,0,0);");
    _videoTimer->setText("0:00 / 0:00");

    connect(_playButton, SIGNAL(clicked()),parent->parentWidget(), SLOT(togglePlay()));
    connect(_positionSlider, SIGNAL(sliderMoved(int)),parent->parentWidget(), SLOT(setPosition(int)));
}

MedNUSVideoControl::~MedNUSVideoControl()
{
    delete _playButton;
    delete _positionSlider;
    delete _videoTimer;
    delete _volumeButton;
}

void MedNUSVideoControl::updateUI() {
    int playerHeight = this->height()-VIDEO_ICON_SIZE-VIDEO_BORDER;
    _playButton->setGeometry(QRect(VIDEO_BORDER,playerHeight,VIDEO_ICON_SIZE,VIDEO_ICON_SIZE));
    _volumeButton->setGeometry(QRect(VIDEO_BORDER+VIDEO_ICON_SIZE+VIDEO_SEP_LENGTH,playerHeight,VIDEO_ICON_SIZE,VIDEO_ICON_SIZE));
    _positionSlider->setGeometry(QRect(VIDEO_BORDER+VIDEO_ICON_SIZE*2+VIDEO_SEP_LENGTH*2,playerHeight,this->width()-(VIDEO_BORDER*2+VIDEO_ICON_SIZE*2+VIDEO_SEP_LENGTH*3)-VIDEO_TIME_LENGTH,VIDEO_ICON_SIZE));
    _videoTimer->setGeometry(QRect(this->width()-VIDEO_BORDER-VIDEO_TIME_LENGTH,playerHeight,VIDEO_TIME_LENGTH,VIDEO_ICON_SIZE));

}

QString MedNUSVideoControl::timeConvert(qint64 msecs)
{
    QString formattedTime;

    int hours = msecs/(1000*60*60);
    int minutes = (msecs-(hours*1000*60*60))/(1000*60);
    int seconds = (msecs-(minutes*1000*60)-(hours*1000*60*60))/1000;
    //int milliseconds = msecs-(seconds*1000)-(minutes*1000*60)-(hours*1000*60*60);

    formattedTime.append(/*QString("%1").arg(hours, 2, 10, QLatin1Char('0')) + ":" +*/
                         QString( "%1" ).arg(minutes, 2, 10, QLatin1Char('0')) + ":" +
                         QString( "%1" ).arg(seconds, 2, 10, QLatin1Char('0')) /*+ ":" +
                         QString( "%1" ).arg(milliseconds, 3, 10, QLatin1Char('0'))*/);

    return formattedTime;

}

void MedNUSVideoControl::mediaStateChanged(QMediaPlayer::State state)
{
    switch(state) {
    case QMediaPlayer::PlayingState:
        this->_playButton->setStyleSheet("QPushButton {border-style: outset; border-width: 0px;background-image: url(:/images/bt_pause.png); background-color:rgba(0,0,0,0);}");
        break;
    default:
        this->_playButton->setStyleSheet("QPushButton {border-style: outset; border-width: 0px;background-image: url(:/images/bt_play.png); background-color:rgba(0,0,0,0);}");
        break;
    }
}

void MedNUSVideoControl::positionChanged(qint64 position)
{
    _positionSlider->setValue(position);
    _videoTimer->setText(timeConvert(position) +"/" + _durationText);
}

void MedNUSVideoControl::durationChanged(qint64 duration)
{
    _positionSlider->setRange(0, duration);
    _durationText = timeConvert(duration);
    _videoTimer->setText("00:00/" + _durationText);
}

void MedNUSVideoControl::volumeChanged(qint64 value) {
    _volume=MIN(3,MAX(0,value));
    //TODO Assig value to volume. Value of volume range from 0 to 3.
    _volumeButton->setStyleSheet("QPushButton {border-style: outset; border-width: 0px;background-image: url(:/images/bt_volume_"+QString::number(_volume)+".png); background-color:rgba(0,0,0,0);}");
}
