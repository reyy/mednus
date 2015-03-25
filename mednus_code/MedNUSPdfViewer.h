#ifndef MEDNUSPDFVIEWER_H
#define MEDNUSPDFVIEWER_H

#include <QDebug>
#include <QWidget>
#include <QLabel>
#include <QKeyEvent>
#include <QScrollArea>
#include <QGridLayout>
#include <QScrollBar>
#include <poppler-qt5.h>

class MedNUSPdfViewer : public QWidget
{
    Q_OBJECT
public:
    explicit MedNUSPdfViewer(QString filename, QWidget *parent = 0);
    void setPage(int num);

protected:
    int pageNum = 0;
    Poppler::Document* document;
    Poppler::Page* pdfPage;
    QLabel *imageLabel;
    QGridLayout *layout;
    QImage image;
    QScrollArea *scrollArea;
    QWidget * scrollAreaWidgetContents;

    void wheelEvent(QWheelEvent *);
    void resizeEvent(QResizeEvent *);
    void keyPressEvent(QKeyEvent *event);

signals:
    // No signals as of yet.

public slots:
    // No slots as of yet.
};


#endif // MEDNUSPDFVIEWER_H
