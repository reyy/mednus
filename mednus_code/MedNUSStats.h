#ifndef MEDNUSSTATS_H
#define MEDNUSSTATS_H

// General
#include <QWidget>
#include <QLabel>
#include <QVector>
#include <QString>
#include <QMessageBox>
#include <QTimer>
#include <QDir>
#include <QFileInfo>
#include <QDateTime>

// For Graphics
#include <QGridLayout>
#include <QRadioButton>
#include <QButtonGroup>
#include <QPushButton>
#include <QScrollArea>
#include <QScrollBar>
#include <QResizeEvent>

// For JSON
#include <QFile>
#include <QByteArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

// For debugging
#include <QTextStream>
#include <QDebug>

class MedNUSStats : public QWidget
{
    Q_OBJECT
public:
    explicit MedNUSStats(QString filename, QWidget *parent = 0);
    ~MedNUSStats();
};


#endif // MEDNUSSTATS_H
