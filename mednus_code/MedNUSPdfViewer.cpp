#include "MedNUSPdfViewer.h"

MedNUSPdfViewer::MedNUSPdfViewer(QString filename, QWidget *parent) :
    QWidget(parent)
{
    this->setAccessibleName(filename);
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

    imageLabel = new QLabel();
    imageLabel->setPixmap(QPixmap::fromImage(image));
    imageLabel->setAutoFillBackground(true);
    imageLabel->setAlignment(Qt::AlignCenter);

    scrollArea = new QScrollArea(this);
    scrollArea->setObjectName(QStringLiteral("scrollArea"));
    scrollArea->setWidgetResizable(true);
    scrollArea->setContentsMargins(0,0,0,0);
    scrollArea->setWidget(imageLabel);
    scrollArea->setGeometry(this->geometry());
    scrollArea->setAutoFillBackground(true);
}

void MedNUSPdfViewer::setPage(int num)
{
    pdfPage = document->page(num);
    if (pdfPage == 0) {
      return;
    }
    image = pdfPage->renderToImage(0.4 * physicalDpiX(), 0.4 * physicalDpiY());
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

    image = pdfPage->renderToImage(0.4 * physicalDpiX(), 0.4 * physicalDpiY());
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
