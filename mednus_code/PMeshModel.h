/* PMeshModel.h

   Model class for meshes

   Copyright 2012, 2103, 2014 National University of Singapore
   Author: Leow Wee Kheng
*/


#ifndef PMESHMODEL_H
#define PMESHMODEL_H

#include <QObject>
#include <QColor>
#include <QList>
#include "vtkPolyData.h"
#include "vtkPolyDataNormals.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"


class PMeshPart
{
    friend class PMeshModel;
    QString name;
    QString source;
    vtkPolyData *data;
    vtkPolyDataNormals *normals; // construct internally
    vtkPolyDataMapper *mapper; // construct internally
    vtkActor *actor; // construct internally
};


class PMeshModel: public QObject
{
    Q_OBJECT

public:
    PMeshModel();
    ~PMeshModel();
    
    void append(QString name, QString source, vtkPolyData *data);
    void clear();
    bool isEmpty() const;
    int size() const;
    
    QString getName(int pos) const;
    QString getSource(int pos) const;
    vtkPolyData *getData(int pos) const;
    vtkActor *getActor(int pos) const;
    bool getVisibility(int pos) const;
    QColor getColor(int pos) const;
    float getTransparency(int pos) const;

    
public slots:
    void setName(int pos, QString name);
    void setVisibility(int pos, bool visible);
    void setColor(int pos, QColor color);
    void setTransparency(int pos, float value);
    void removeAt(int pos);
    
protected:
    QList<PMeshPart> meshList;
};

#endif
