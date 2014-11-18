/* PMeshTableWidget.cpp

   Table widget for a list of meshes.

   Copyright 2012, 2103, 2014 National University of Singapore
   Author: Leow Wee Kheng
*/

#include "PMeshTableWidget.h"
#include <QtGui>
#include <QPalette>

#include <iostream>

#define NumRow 0
#define NumColumn 7
#define VisibleColumn 0
#define ColorColumn 1
#define Null1Column 2
#define TransparencyColumn 3
#define Null2Column 4
#define NameColumn 5
#define SourceColumn 6


PMeshTableWidget::PMeshTableWidget(): QTableWidget(NumRow, NumColumn)
{
    // Initialise table
    QPalette palette = this->palette();
    palette.setColor(QPalette::Base, QColor(248, 248, 248));
    setPalette(palette);
    verticalHeader()->hide();
    setShowGrid(false);
    setSelectionMode(QTableWidget::ContiguousSelection);

    QStringList labels;
    labels << "" << "" << "" << "" << "";
    setHorizontalHeaderLabels(labels);
    QTableWidgetItem *headerItem;
    headerItem = new QTableWidgetItem;
    headerItem->setIcon(QIcon(QString(":/images/eye.png")));
    setHorizontalHeaderItem(VisibleColumn, headerItem);
    headerItem = new QTableWidgetItem;
    headerItem->setIcon(QIcon(QString(":/images/color.png")));
    setHorizontalHeaderItem(ColorColumn, headerItem);
    headerItem = new QTableWidgetItem;
    headerItem->setIcon(QIcon(QString(":/images/transparent.png")));
    setHorizontalHeaderItem(TransparencyColumn, headerItem);
    headerItem = new QTableWidgetItem(QString("Name"));
    headerItem->setTextAlignment(Qt::AlignLeft);
    setHorizontalHeaderItem(NameColumn, headerItem);
    headerItem = new QTableWidgetItem(QString("Source"));
    headerItem->setTextAlignment(Qt::AlignLeft);
    setHorizontalHeaderItem(SourceColumn, headerItem);

    QList<QHeaderView::ResizeMode> modes;
    modes << QHeaderView::Fixed << QHeaderView::Fixed 
          << QHeaderView::Fixed
          << QHeaderView::Fixed << QHeaderView::Fixed
          << QHeaderView::Interactive << QHeaderView::Stretch;
    QHeaderView *header = horizontalHeader();
    for (int i = 0; i < NumColumn; ++i)
        header->setSectionResizeMode(i, modes[i]);

    QList<int> widths;
    widths << 26 << 26 << 3 << 26 << 3 << 100 << 200;
    for (int i = 0; i < NumColumn; ++i)
        setColumnWidth(i, widths[i]);

    connect(this, SIGNAL(cellClicked(int, int)),
        this, SLOT(setVisibility(int, int)));
    connect(this, SIGNAL(cellDoubleClicked(int, int)),
        this, SLOT(setColor(int, int)));
    connect(this, SIGNAL(cellDoubleClicked(int, int)),
        this, SLOT(showTransparencyDialog(int, int)));
    connect(this, SIGNAL(cellClicked(int, int)),
        this, SLOT(selectItem(int, int)));
        
    // Create transparency setting dialog
    transDialog = new QWidget;
    transDialog->setWindowTitle("Transparency Setting");
    transDialog->setFixedSize(250, 120);
    transDialog->setWindowFlags(Qt::WindowStaysOnTopHint);
    
    transMax = 20;
    transSlider = new QSlider;
    transSlider->setOrientation(Qt::Horizontal);
    transSlider->setRange(0, transMax);
    connect(transSlider, SIGNAL(valueChanged(int)),
        this, SLOT(setTransparency(int)));
    transSlider->setValue(0);
    
    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addWidget(new QLabel("opaque"));
    hbox->addStretch();
    hbox->addWidget(new QLabel("transparent"));
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addLayout(hbox);
    vbox->addWidget(transSlider);
    vbox->addStretch();

    transCloseButton = new QPushButton("close");
    connect(transCloseButton, SIGNAL(clicked()), transDialog, SLOT(close()));
    
    hbox = new QHBoxLayout;
    hbox->addStretch();
    hbox->addWidget(transCloseButton);
    vbox->addLayout(hbox);
    
    transDialog->setLayout(vbox);
}


PMeshTableWidget::~PMeshTableWidget()
{
    delete transDialog;
}


// Event handling

void PMeshTableWidget::closeEvent(QCloseEvent *event)
{
    transDialog->hide();
    event->accept();
}


void PMeshTableWidget::hideEvent(QHideEvent *event)
{
    transDialog->hide();
    event->accept();
}


void PMeshTableWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() != Qt::Key_Delete)
    {
        QTableWidget::keyPressEvent(event);
        return;
    }
        
    QList<QTableWidgetSelectionRange> list = selectedRanges();
    if (list.isEmpty())
        return;
        
    QTableWidgetSelectionRange range = list[0];  // Only 1 range.
    for (int i = range.bottomRow(); i >= range.topRow(); --i)
    {
        removeRow(i);
        emit itemDeleted(i);
    }
}


#define RowHeight 20
#define MaxHeight 600

void PMeshTableWidget::append(QString name, QString source)
{
    // Append item
    int j = rowCount();
    insertRow(j);
    setRowHeight(j, RowHeight);

    QTableWidgetItem *item = new QTableWidgetItem;
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
    item->setCheckState(Qt::Checked);
    setItem(j, VisibleColumn, item);
       
    item = new QTableWidgetItem;
    item->setFlags(Qt::ItemIsEnabled);
    item->setBackground(QBrush(QColor(255, 255, 255)));
    setItem(j, ColorColumn, item);

    item = new QTableWidgetItem;
    item->setFlags(Qt::NoItemFlags);
    setItem(j, Null1Column, item);

    item = new QTableWidgetItem;
    item->setFlags(Qt::ItemIsEnabled);
    item->setBackground(QBrush(QColor(255, 255, 255)));
    item->setData(Qt::UserRole, QVariant(0.0));
    setItem(j, TransparencyColumn, item);

    item = new QTableWidgetItem;
    item->setFlags(Qt::NoItemFlags);
    setItem(j, Null2Column, item);

    item = new QTableWidgetItem(name);
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable |
        Qt::ItemIsEditable);
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    setItem(j, NameColumn, item);

    item = new QTableWidgetItem(source);
    item->setFlags(Qt::ItemIsEnabled);
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    setItem(j, SourceColumn, item);
    
    // Adjust size
    int width = this->width();
    int height = this->height();
    int bestHeight = qMin(MaxHeight,
            (rowCount() + 2) * RowHeight);
    if (height < bestHeight)
        resize(width, bestHeight);
}


// Slot functions

void PMeshTableWidget::setVisibility(int row, int col)
{
    if (col != VisibleColumn)
        return;
        
    bool visibility;
    Qt::CheckState state;
    
    if (item(row, col)->checkState() == Qt::Checked)
    {
        visibility = true;
        state = Qt::Checked;
    }
    else
    {
        visibility = false;
        state = Qt::Unchecked;
    }
        
    QList<QTableWidgetSelectionRange> list = selectedRanges();
    
    if (list.isEmpty() || list[0].rowCount() == 1)
        emit visibilityChanged(row, visibility);
    else
    {
        int top = list[0].topRow();
        int bot = list[0].bottomRow();
        for (int r = top; r <= bot; ++r)
        {
            item(r, col)->setCheckState(state);
            emit visibilityChanged(r, visibility);
        }
        emit visibilityChanged(row, visibility);
    }
    
    if (!list.isEmpty())  // Reset
        setRangeSelected(list[0], false);
}


void PMeshTableWidget::setColor(int row, int col)
{
    if (col != ColorColumn)
        return;
        
    QColor color = QColorDialog::getColor();
    if (!color.isValid())
        return;
        
    QTableWidgetItem *item = this->item(row, col);
    item->setBackground(QBrush(color));
    emit colorChanged(row, color);
}


void PMeshTableWidget::showTransparencyDialog(int row, int col)
{
    if (col != TransparencyColumn)
        return;
    
    transRow = row;
    QTableWidgetItem *item = this->item(row, col);
    float value = item->data(Qt::UserRole).toFloat();
    transSlider->setValue((int) (value * transMax));
    
    QRect geo = frameGeometry();
    transDialog->move(geo.right(), geo.top());
    transDialog->show();
}


void PMeshTableWidget::setTransparency(int value)
{
    float trans = (float) value / transMax;
    int c = (1.0 - trans) * 255;
        
    QColor color;
    color.setRgb(c, c, c);
        
    QTableWidgetItem *item = this->item(transRow, TransparencyColumn);
    item->setBackground(QBrush(color));
    item->setData(Qt::UserRole, QVariant(trans));
    emit transparencyChanged(transRow, trans);
}


void PMeshTableWidget::selectItem(int row, int col)
{
    if (col < NameColumn)
        return;
        
    emit itemSelected(row);
}
