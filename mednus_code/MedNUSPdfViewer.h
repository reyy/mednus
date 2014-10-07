#ifndef MEDNUSPDFVIEWER_H
#define MEDNUSPDFVIEWER_H

#include <QWidget>
#include <QLabel>
#include <QPainter>
#include <poppler-qt5.h>

class MedNUSPdfViewer : public QWidget
{
    Q_OBJECT
public:
    explicit MedNUSPdfViewer(QWidget *parent = 0);
    void setPage(int num);
protected:
    //void paintEvent(QPaintEvent *event);
    Poppler::Document* document;
    Poppler::Page* pdfPage;
    QLabel *imageLabel;
signals:

public slots:

};

#endif // MEDNUSPDFVIEWER_H
