#ifndef MEDNUSPDFVIEWER_H
#define MEDNUSPDFVIEWER_H

#include <QWidget>
#include <QLabel>
#include <QKeyEvent>
#include <QPainter>
#include <poppler-qt5.h>

class MedNUSPdfViewer : public QWidget
{
    Q_OBJECT
public:
    explicit MedNUSPdfViewer(QWidget *parent = 0);
    void setPage(int num);
protected:
    int pageNum = 0;
    Poppler::Document* document;
    Poppler::Page* pdfPage;
    QLabel *imageLabel;

    void keyPressEvent(QKeyEvent *event);
signals:

public slots:

};

#endif // MEDNUSPDFVIEWER_H
