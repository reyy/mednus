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
    playButton = new QPushButton((this));
    playButton->setIconSize(QSize(48,48));
    playButton->setFlat(true);
    playButton->setStyleSheet("QPushButton {border-style: outset; border-width: 0px;background-image: url(:/images/bt_play.png); background-color:rgba(0,0,0,0);}");
    positionSlider = new QSlider(Qt::Horizontal,this);
    positionSlider->setRange(0, 0);
    positionSlider->setStyleSheet("QSlider {background-color:rgba(0,0,0,0);}");

    connect(playButton, SIGNAL(clicked()),parent->parentWidget(), SLOT(togglePlay()));
    connect(positionSlider, SIGNAL(sliderMoved(int)),parent->parentWidget(), SLOT(setPosition(int)));
}

MedNUSVideoControl::~MedNUSVideoControl()
{
    delete playButton;
    delete positionSlider;
}

void MedNUSVideoControl::updateUI() {
    qDebug() <<this->width();
    playButton->setGeometry(QRect(10,this->height()-48-10,48,48));
    positionSlider->setGeometry(QRect(10+48+5,this->height()-48-10,this->width()-10-48-5-15,48));
}

void MedNUSVideoControl::mediaStateChanged(QMediaPlayer::State state)
{
    switch(state) {
    case QMediaPlayer::PlayingState:
        this->playButton->setStyleSheet("QPushButton {border-style: outset; border-width: 0px;background-image: url(:/images/bt_pause.png); background-color:rgba(0,0,0,0);}");
        break;
    default:
        this->playButton->setStyleSheet("QPushButton {border-style: outset; border-width: 0px;background-image: url(:/images/bt_play.png); background-color:rgba(0,0,0,0);}");
        break;
    }
}

void MedNUSVideoControl::positionChanged(qint64 position)
{
    positionSlider->setValue(position);
}

void MedNUSVideoControl::durationChanged(qint64 duration)
{
    positionSlider->setRange(0, duration);
}
