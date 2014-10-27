#include "MedNUSPdfViewer.h"

MedNUSPdfViewer::MedNUSPdfViewer(QString filename, QWidget *parent) :
    QWidget(parent)
{
    //QString filename = ":/content/test.pdf";
    document = Poppler::Document::load(filename);
    if (!document || document->isLocked()) {
      // err
      delete document;
      return;
    }

    if (document == 0) {
      return;
    }

    pdfPage = document->page(0);
    if (pdfPage == 0) {
      return;
    }
    image = pdfPage->renderToImage(/*1.0 * physicalDpiX(), 1.0 * physicalDpiY()*/);

    //image = image.scaledToHeight(this->height());
    //image = image.scaled(this->/*visibleRegion().boundingRect().*/size(),Qt::AspectRatioMode::KeepAspectRatio);

    imageLabel = new QLabel();
    imageLabel->setPixmap(QPixmap::fromImage(image));


    scrollArea = new QScrollArea(this);
    scrollArea->setObjectName(QStringLiteral("scrollArea"));
    scrollArea->setWidgetResizable(true);
    scrollArea->setAlignment(Qt::AlignCenter);
    scrollArea->setContentsMargins(0,0,0,0);
//    scrollAreaWidgetContents = new QWidget();
//    scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
//    scrollAreaWidgetContents->setGeometry(QRect(0, 0, 850, 256));
    scrollArea->setWidget(imageLabel);
    scrollArea->setGeometry(this->geometry());

//    layout = new QGridLayout();
//    layout->addWidget(scrollArea);
//    this->setLayout(layout);
    //this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    //this->set
    //this->set
    //delete pdfPage;
    //delete document;
}

void MedNUSPdfViewer::setPage(int num)
{
    pdfPage = document->page(num);
    if (pdfPage == 0) {
      return;
    }
    image = pdfPage->renderToImage(0.3 * physicalDpiX(), 0.3 * physicalDpiY());
    imageLabel->setPixmap(QPixmap::fromImage(image));
}

void MedNUSPdfViewer::wheelEvent(QWheelEvent *event)
{
    if (scrollArea->verticalScrollBar()->value() == scrollArea->verticalScrollBar()->maximum()
            && event->pixelDelta().y() < 0)
        setPage(pageNum+1>document->numPages()?document->numPages():++pageNum);
    else if (scrollArea->verticalScrollBar()->value() == scrollArea->verticalScrollBar()->minimum()
             && event->pixelDelta().y() > 0)
        setPage(pageNum-1<0?0:--pageNum);
}

void MedNUSPdfViewer::resizeEvent(QResizeEvent *event)
{
    scrollArea->setGeometry(this->geometry());
    //image = pdfPage->renderToImage(1.0 * physicalDpiX(), 1.0 * physicalDpiY());
    //Qreal(this->width()/image.width())
    image = pdfPage->renderToImage(0.3 * physicalDpiX(), 0.3 * physicalDpiY());
    imageLabel->setPixmap(QPixmap::fromImage(image));
}

void MedNUSPdfViewer::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Up || event->key() == Qt::Key_Left) {
        setPage(pageNum-1<0?0:--pageNum);
    } else if(event->key() == Qt::Key_Down || event->key() == Qt::Key_Right) {
        setPage(pageNum+1>document->numPages()?document->numPages():++pageNum);
    }
}
