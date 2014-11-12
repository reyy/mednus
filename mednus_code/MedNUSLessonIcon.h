#ifndef MedNUSLessonIcon_H
#define MedNUSLessonIcon_H

#include <QLabel>
#include <QMouseEvent>

class MedNUSLessonIcon : public QWidget
{
    Q_OBJECT
private:
    bool _selected;
    QLabel *_highlight;
    QLabel *_icon;
    QLabel *_text;
    QPixmap _directory;    
    QString _path;
    QString _filename;
    QWidget *_parent;

    float _x;
    float _y;

public:
     MedNUSLessonIcon(QString path, QPixmap directory, QWidget *parent);
    ~MedNUSLessonIcon();
     void updatePosition(float packageX, float packageY, float x, float y);
     void setHighlight(bool status);
     void setSelected(bool value);
     void setVisible(bool value);

protected:
    void mousePressEvent ( QMouseEvent * event ) ;

signals:
    void emitOpenFile(QString, QString, int);
public slots:

};

#endif // MedNUSLessonIcon_H
