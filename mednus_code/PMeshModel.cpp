/* PMeshModel.cpp

   Model class for meshes

   Copyright 2012, 2103, 2014 National University of Singapore
   Author: Leow Wee Kheng
*/


#include "PMeshModel.h"
#include "vtkProperty.h"



PMeshModel::PMeshModel()
{
}


PMeshModel::~PMeshModel()
{
}


void PMeshModel::append(QString name, QString source, vtkPolyData *data)
{
    PMeshPart part;
    part.name = name;
    part.source = source;
    part.data = data;
    part.normals = vtkPolyDataNormals::New();
    part.normals->SetInputData(data);
    part.mapper = vtkPolyDataMapper::New();
    part.mapper->SetInputConnection(part.normals->GetOutputPort());
    part.mapper->Update();
    part.actor = vtkActor::New();
    part.actor->SetMapper(part.mapper);
    meshList.append(part);
}


void PMeshModel::clear()
{
    for (int i = 0; i < meshList.size(); ++i)
    {
        meshList[i].normals->Delete();
        meshList[i].mapper->Delete();
        meshList[i].actor->Delete();
    }
    meshList.clear();
}


bool PMeshModel::isEmpty() const
{
    return meshList.isEmpty();
}


int PMeshModel::size() const
{
    return meshList.size();
}


QString PMeshModel::getName(int pos) const
{
    if (pos >= 0 && pos < meshList.size())
        return meshList[pos].name;
    else
        return QString();
}


QString PMeshModel::getSource(int pos) const
{
    if (pos >= 0 && pos < meshList.size())
        return meshList[pos].source;
    else
        return QString();
}


vtkPolyData *PMeshModel::getData(int pos) const
{
    if (pos >= 0 && pos < meshList.size())
        return meshList[pos].data;
    else
        return NULL;
}


vtkActor *PMeshModel::getActor(int pos) const
{
    if (pos >= 0 && pos < meshList.size())
        return meshList[pos].actor;
    else
        return NULL;
}


bool PMeshModel::getVisibility(int pos) const
{
    if (pos >= 0 && pos < meshList.size())
        return (meshList[pos].actor->GetVisibility() == 1 ? true : false);
    else
        return false;
}


QColor PMeshModel::getColor(int pos) const
{
    if (pos < 0 || pos >= meshList.size())
       return QColor();
       
    double *rgb = meshList[pos].actor->GetProperty()->GetColor();
    QColor color(int(rgb[0] * 255), int(rgb[1] * 255), int(rgb[2] * 255));
    return color;
}


float PMeshModel::getTransparency(int pos) const
{
    if (pos >= 0 && pos < meshList.size())
        return 1.0 - meshList[pos].actor->GetProperty()->GetOpacity();
    else
        return 0.0;
}

void PMeshModel::setName(int pos, QString name)
{
    if (pos >= 0 && pos < meshList.size())
        meshList[pos].name = name;
}


void PMeshModel::setVisibility(int pos, bool visible)
{
    if (pos >= 0 && pos < meshList.size())
        meshList[pos].actor->SetVisibility(visible ? 1 : 0);
}


void PMeshModel::setColor(int pos, QColor color)
{
    if (pos < 0 || pos >= meshList.size())
       return;
    
    double red = color.red() / 255.0;
    double green = color.green() / 255.0;
    double blue = color.blue() / 255.0;
    meshList[pos].actor->GetProperty()->SetColor(red, green, blue);
}


void PMeshModel::setTransparency(int pos, float value)
{
    if (pos >= 0 && pos < meshList.size())
        meshList[pos].actor->GetProperty()->SetOpacity(1.0 - value);
}


void PMeshModel::removeAt(int pos)
{
   if (pos < 0 || pos >= meshList.size())
       return;

    meshList[pos].normals->Delete();
    meshList[pos].mapper->Delete();
    meshList[pos].actor->Delete();
    meshList.removeAt(pos);
}
