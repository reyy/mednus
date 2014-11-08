#include "MedNUSVideoViewer.h"
#include <QDebug>

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
    delete videoItem;
    delete scene;
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
    _playButton = new QPushButton((this));
    _playButton->setIconSize(QSize(24,24));
    _playButton->setFlat(true);
    _playButton->setStyleSheet("QPushButton {border-style: outset; border-width: 0px;background-image: url(:/images/bt_play.png); background-color:rgba(0,0,0,0);}");

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
}

void MedNUSVideoControl::updateUI() {
    _playButton->setGeometry(QRect(10,this->height()-24-10,24,24));
    _positionSlider->setGeometry(QRect(10+24+5,this->height()-24-10,this->width()-10-48-5-15-40,24));
    _videoTimer->setGeometry(QRect(10+24+5+this->width()-10-48-5-15-40+15,this->height()-24-10,50,24));
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
}

void MedNUSVideoControl::durationChanged(qint64 duration)
{
    _positionSlider->setRange(0, duration);
    //TODO Assigned duration value over the total duration of video.
    //_videoTimer->setText("1:11 / 3:20");
}
