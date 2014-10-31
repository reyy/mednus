#include "MedNUSVideoViewer.h"
#include <QGridLayout>
#include <QPushButton>
#include <QStackedLayout>

MedNUSVideoViewer::MedNUSVideoViewer(QString filename, QWidget *parent) :
    QWidget(parent)
{
    this->setAccessibleName(filename);
//    player = new QMediaPlayer();

//    QMediaPlaylist *playlist = new QMediaPlaylist(player);
//    //playlist->addMedia(QUrl("mms://live-vip-49.nus.edu.sg/INV_CSEWARE/anatomy/respiratory/landmarks_patient.wmv"));
//    playlist->addMedia(QUrl::fromLocalFile(filename));


//    videoWidget = new QVideoWidget();
//    player->setVideoOutput(videoWidget);

//    QStackedLayout *grid2 = new QStackedLayout;
//    QGridLayout *grid = new QGridLayout;
//    grid->setContentsMargins(0,0,0,0);
//    this->setLayout(grid2);
//    grid2->setStackingMode(QStackedLayout::StackAll);
//    grid2->addWidget(videoWidget);
//    player->setPlaylist(playlist);
//    playlist->setCurrentIndex(0);
//    player->play();
//    player->pause();

//    videoWidget->lower();
    // video widget

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
    //videoView->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    //videoView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //videoView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //videoView->setStyleSheet( "QGraphicsView { border-style: none; background-color:black}" );
    videoView->setBackgroundBrush(QBrush(Qt::black, Qt::SolidPattern));

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(videoView);
    this->setLayout(layout);
    this->setFocusPolicy(Qt::StrongFocus);
    layout->setSpacing(0);
    layout->setMargin(0);

    control = new MedNUSVideoControl();
    control->setParent(videoView);
    control->show();
    //grid2->addWidget(control);
    //grid2->setCurrentWidget(control);
    //control->setGeometry(this->width()/2, 4*this->height()/5, this->width()/5, this->height()/5);
    //control->raise();
    //videoWidget->stackUnder(control);

//    QPushButton *hello = new QPushButton("Some veryb long text");
//    grid2->addWidget(hello);
//    grid2->setCurrentWidget(hello);
    //hello->raise();
    //control->setFrameStyle(QFrame::Box | QFrame::Raised);

}

MedNUSVideoViewer::~MedNUSVideoViewer()
{
    player->stop();
    delete player;

    this->layout()->removeWidget(videoWidget);
    delete videoWidget;
}

void MedNUSVideoViewer::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Space) {
        if(player->state() == QMediaPlayer::PlayingState)
            player->pause();
        else
            player->play();
    }
}

void MedNUSVideoViewer::resizeEvent(QResizeEvent *event)
{
    videoView->setGeometry(this->geometry());
    videoView->setSceneRect(QRectF(0, 0,this->width(), this->height()));
    videoItem->setSize(videoView->size());

    videoView->lower();
    control->raise();
    control->setGeometry(50, 50, control->width(), control->height());
    //control->setGeometry(this->width()/2, 4*this->height()/5, this->width()/5, this->height()/5);
}


MedNUSVideoControl::MedNUSVideoControl(QWidget *parent):
    QWidget(parent)
{
    this->play = new QLabel("play",this);
    this->pause = new QLabel("pause",this);
}

MedNUSVideoControl::~MedNUSVideoControl()
{
    delete play;
    delete pause;
}
