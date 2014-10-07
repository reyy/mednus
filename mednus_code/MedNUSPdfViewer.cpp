#include "MedNUSPdfViewer.h"

MedNUSPdfViewer::MedNUSPdfViewer(QWidget *parent) :
    QWidget(parent)
{
    QString filename = "/Users/rey/Desktop/thorax.ppt.pdf";
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
    QImage image = pdfPage->renderToImage();
    imageLabel = new QLabel(this);
    imageLabel->setPixmap(QPixmap::fromImage(image));
    //delete pdfPage;
    //delete document;
}

void MedNUSPdfViewer::setPage(int num)
{
    pdfPage = document->page(num);
    if (pdfPage == 0) {
      return;
    }
    QImage image = pdfPage->renderToImage();
    imageLabel->setPixmap(QPixmap::fromImage(image));
}

//void MedNUSPdfViewer::paintEvent(QPaintEvent *event)
//{
//    QPainter p(this);
//    pdfPage->renderToPainter(&p);

//}
