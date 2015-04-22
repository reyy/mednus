#include "MedNUSVideoViewer.h"
#include <QDebug>
#include "MedNUSAUISettings.h"

MedNUSVideoViewer::MedNUSVideoViewer(QString filename, interfaceMode currentMode, QWidget *parent) :
    QWidget(parent)
{
    this->currentMode = currentMode;
    this->setAccessibleName(filename);

    videoView = new QGraphicsView(this);

    this->setFocusPolicy(Qt::StrongFocus);
    this->installEventFilter(this);

    scene = new QGraphicsScene(videoView);
    videoView->setScene(scene);
    videoView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    videoView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    videoItem = new QGraphicsVideoItem;

    mediaPlayer.setVideoOutput(videoItem);

    scene->addItem(videoItem);

    videoView->setAutoFillBackground(true);
    videoView->setBackgroundBrush(QBrush(Qt::black, Qt::SolidPattern));
    videoView->setStyleSheet( "QGraphicsView { border-style: none; background-color:black}" );

    control = new MedNUSVideoControl(currentMode, videoView);

    connect(control, SIGNAL(changeVolume(int)), &mediaPlayer, SLOT(setVolume(int)));
    connect(&mediaPlayer, SIGNAL(stateChanged(QMediaPlayer::State)),control, SLOT(mediaStateChanged(QMediaPlayer::State)));
    connect(&mediaPlayer, SIGNAL(positionChanged(qint64)), control, SLOT(positionChanged(qint64)));
    connect(&mediaPlayer, SIGNAL(durationChanged(qint64)), control, SLOT(durationChanged(qint64)));
    connect(&mediaPlayer, &QMediaPlayer::positionChanged, [=](qint64 pos){ emit positionChanged(pos); });

    try
    {
        mediaPlayer.setMedia(QUrl::fromLocalFile(filename));
        mediaPlayer.pause();
    }
    catch(int error)
    {
        qDebug() << "Error Opening File.";
    }

    //Loaded sucessfully!
    this->setProperty("Loaded",true);
}


MedNUSVideoViewer::~MedNUSVideoViewer()
{
    mediaPlayer.stop();
    removeEventFilter(this);

    //layout()->removeWidget(videoView);
    delete control;
    delete videoView;
}

void MedNUSVideoViewer::initStoryPoints(QList<qint64> sp){control->setStoryPoints(sp);}

void MedNUSVideoViewer::setPosition(qint64 position)
{
    mediaPlayer.setPosition(position);
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


void MedNUSVideoViewer::setVolume(int volume)
{
    mediaPlayer.setVolume(volume);
}


MedNUSVideoControl::MedNUSVideoControl(interfaceMode currentMode, QWidget *parent):
    QWidget(parent)
{
    _currentMode = currentMode;
    _volume=3;
    _playButton = new QPushButton((this));
    _playButton->setIconSize(QSize(24,24));
    _playButton->setFlat(true);
    _playButton->setStyleSheet("QPushButton {border-style: outset; border-width: 0px;background-image: url(:/images/bt_play.png); background-color:rgba(0,0,0,0);}");

    _volumeButton = new QPushButton((this));
    _volumeButton->setIconSize(QSize(30,24));
    _volumeButton->setFlat(true);
    _volumeButton->setStyleSheet("QPushButton {border-style: outset; border-width: 0px;background-image: url(:/images/bt_volume_3.png); background-color:rgba(0,0,0,0);}");

    _storyPointContainer = new QWidget(this);

    _positionSlider = new QSlider(Qt::Horizontal,this);
    _positionSlider->setRange(0, 0);
    _positionSlider->setStyleSheet("QSlider {background-color:rgba(0,0,0,0);} QSlider::groove:horizontal {border: 0px solid #FFFFFF;height: 8px;background-color:rgba(255,255,255,128) ;margin: 2px 0;} QSlider::handle:horizontal {background: #FFFFFF;border: 1px solid #EEEEEE;width: 18px;margin: -2px 0;border-radius: 2px;}");

    _videoTimer = new QLabel(this);
    _videoTimer->setStyleSheet("color:#FFFFFF;font-size:8px;text-align:center;background-color:rgba(0,0,0,0);");
    _videoTimer->setText("0:00 / 0:00");

    _nextButton = new QPushButton((this));
    _nextButton->setIconSize(QSize(24,24));
    _nextButton->setFlat(true);
    _nextButton->setStyleSheet("QPushButton {border-style: outset; border-width: 0px;background-image: url(:/images/bt_next.png); background-color:rgba(0,0,0,0);}");

    _prevButton = new QPushButton((this));
    _prevButton->setIconSize(QSize(24,24));
    _prevButton->setFlat(true);
    _prevButton->setStyleSheet("QPushButton {border-style: outset; border-width: 0px;background-image: url(:/images/bt_prev.png); background-color:rgba(0,0,0,0);}");

    _addEditButton = new QPushButton((this));
    _addEditButton->setIconSize(QSize(32,24));
    _addEditButton->setFlat(true);
    _addEditButton->setStyleSheet("QPushButton {border-style: outset; border-width: 0px;background-image: url(:/images/bt_vidadd.png); background-color:rgba(0,0,0,0);}");


    connect(_volumeButton,SIGNAL(clicked()), this, SLOT(volumeClicked()));
    connect(_playButton, SIGNAL(clicked()),parent->parentWidget(), SLOT(togglePlay()));
    connect(_positionSlider, SIGNAL(sliderMoved(int)),parent->parentWidget(), SLOT(setPosition(int)));
    connect(this, SIGNAL(changePosition(qint64)),parent->parentWidget(), SLOT(setPosition(qint64)));

    connect(_nextButton, SIGNAL(clicked()), this, SLOT(goToNextStoryPoint()));
    connect(_prevButton, SIGNAL(clicked()), this, SLOT(goToPrevStoryPoint()));
    connect(_addEditButton, SIGNAL(clicked()), parent->parentWidget(), SLOT(addEditStoryPoint()));
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

    if(_currentMode == interfaceMode::LECTURER)
    {
        //_addEditButton->setStyleSheet("QPushButton {border-style: outset; border-width: 0px;background-image: url(:/images/bt_vidadd.png); background-color:rgba(0,0,0,0);}");
        _addEditButton->setGeometry(QRect(VIDEO_BORDER+24,playerHeight-24,VIDEO_ICON_SIZE+8,VIDEO_ICON_SIZE));

        _prevButton->setGeometry(QRect(VIDEO_BORDER,playerHeight-24,VIDEO_ICON_SIZE,VIDEO_ICON_SIZE));
        _nextButton->setGeometry(QRect(VIDEO_BORDER+24+32,playerHeight-24,VIDEO_ICON_SIZE,VIDEO_ICON_SIZE));
    }
    else
    {
        _prevButton->hide();
        _nextButton->hide();
        _addEditButton->hide();
    }

    _storyPointContainer->setGeometry(_positionSlider->geometry());
    foreach(QObject *storyPointLabel, _storyPointContainer->children())
        if(storyPointLabel->property("pos2").isValid() && _duration!=0)
        {
            qint64 timestamp = storyPointLabel->property("pos2").toString().toLongLong();
            ((QWidget*)(storyPointLabel))->setGeometry(QRect(_positionSlider->width()*timestamp/_duration
                                                             ,VIDEO_ICON_SIZE/4,/*VIDEO_ICON_SIZE/*/1,VIDEO_ICON_SIZE/2));
        }

}

void MedNUSVideoControl::setStoryPoints(QList<qint64> sp)
{
    _storyPoints = sp;

    foreach(QObject *existing, _storyPointContainer->children())
        delete existing;

    foreach(qint64 timestamp, sp)
    {
        QLabel *storyPointLabel = new QLabel(_storyPointContainer);
        storyPointLabel->setProperty("pos2",QString::number(timestamp));
        storyPointLabel->setPixmap(QPixmap(QString::fromStdString(":/images/icon_quiz.png")));
        storyPointLabel->show();
    }
    positionChanged(_position);
    updateUI();
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
        _state = QMediaPlayer::PlayingState;
        this->_playButton->setStyleSheet("QPushButton {border-style: outset; border-width: 0px;background-image: url(:/images/bt_pause.png); background-color:rgba(0,0,0,0);}");
        _addEditButton->hide();
        break;
    default:
        _state = QMediaPlayer::PausedState;
        this->_playButton->setStyleSheet("QPushButton {border-style: outset; border-width: 0px;background-image: url(:/images/bt_play.png); background-color:rgba(0,0,0,0);}");
        if(_currentMode == interfaceMode::LECTURER)
        {
            _addEditButton->show();
            _addEditButton->setStyleSheet("QPushButton {border-style: outset; border-width: 0px;background-image: url(:/images/bt_vidadd.png); background-color:rgba(0,0,0,0);}");
            foreach(qint64 pos, _storyPoints)
                if(pos == _position)
                    _addEditButton->setStyleSheet("QPushButton {border-style: outset; border-width: 0px;background-image: url(:/images/bt_videdit.png); background-color:rgba(0,0,0,0);}");
        }
        else
            _addEditButton->hide();
        break;
    }
}


void MedNUSVideoControl::positionChanged(qint64 position)
{
    _position = position;
    _positionSlider->setValue(position);
    _videoTimer->setText(timeConvert(position) +"/" + _durationText);
    if(_state == QMediaPlayer::PausedState)
    {
        bool isEdit = false;
        foreach(qint64 pos, _storyPoints)
            if(pos == _position)
                isEdit = true;
        if(isEdit)
            _addEditButton->setStyleSheet("QPushButton {border-style: outset; border-width: 0px;background-image: url(:/images/bt_videdit.png); background-color:rgba(0,0,0,0);}");
        else
            _addEditButton->setStyleSheet("QPushButton {border-style: outset; border-width: 0px;background-image: url(:/images/bt_vidadd.png); background-color:rgba(0,0,0,0);}");
    }
}


void MedNUSVideoControl::durationChanged(qint64 duration)
{
    _duration = duration;
    _positionSlider->setRange(0, duration);
    _durationText = timeConvert(duration);
    _videoTimer->setText("00:00/" + _durationText);
}


void MedNUSVideoControl::volumeClicked() {
    if(++_volume>3)
        _volume = 0;

    _volumeButton->setStyleSheet("QPushButton {border-style: outset; border-width: 0px;background-image: url(:/images/bt_volume_"+QString::number(_volume)+".png); background-color:rgba(0,0,0,0);}");
    emit changeVolume(_volume*100/3);
}

void MedNUSVideoControl::goToNextStoryPoint()
{
    foreach(qint64 timestamp, _storyPoints)
        if(timestamp > _position + 200)
        {
            //emit _positionSlider->sliderMoved(timestamp);
            qDebug() << timestamp << _position;
            emit changePosition(timestamp);
            //(static_cast<MedNUSVideoViewer*>(parentWidget()))->setPosition(timestamp);
            return;
        }
}

void MedNUSVideoControl::goToPrevStoryPoint()
{
    for(int i = _storyPoints.size() - 1; i>=0; i--)
        if(_storyPoints[i] + 1000 - 1 < _position)
        {
            qDebug() << _storyPoints[i] << _position;
            emit changePosition(_storyPoints[i]);
            //emit _positionSlider->sliderMoved(_storyPoints[i]);
            return;
        }
}
