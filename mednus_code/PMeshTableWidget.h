/* PMeshTableWidget.h

   Table widget for a list of meshes.

   Copyright 2012, 2103, 2014 National University of Singapore
   Author: Leow Wee Kheng
*/


#ifndef PMESHTABLEWIDGET_H
#define PMESHTABLEWIDGET_H

class QPushButton;

#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QHeaderView>
#include <QTableWidget>
#include <QItemDelegate>
#include <QColorDialog>
#include <QKeyEvent>

class PMeshTableWidget: public QTableWidget
{
    Q_OBJECT
    
public:
    PMeshTableWidget();
    ~PMeshTableWidget();
    void append(QString name, QString source);

protected:
    void closeEvent(QCloseEvent *event);
    void hideEvent(QHideEvent *event);
    void keyPressEvent(QKeyEvent *event);
    
signals:
    void visibilityChanged(int row, bool visible);
    void colorChanged(int row, QColor color);
    void transparencyChanged(int row, float value);
    void itemSelected(int row);
    void itemDeleted(int row);

protected slots:
    void setVisibility(int row, int col);
    void setColor(int row, int col);
    void showTransparencyDialog(int row, int col);
    void setTransparency(int);
    void selectItem(int row, int col);

protected:
    // Transparency setting dialog
    QWidget *transDialog;
    QSlider *transSlider;
    int transMax;
    int transRow;  // Row selected for transparency adjustment
    QPushButton *transCloseButton;
};

#endif
