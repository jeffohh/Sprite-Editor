#ifndef CANVASLABEL_H
#define CANVASLABEL_H

#include <QObject>
#include <QWidget>
#include <QLabel>

class canvasLabel:public QLabel
{
    Q_OBJECT

public:
    canvasLabel();

    void mousePressEvent(QMouseEvent *ev);
    canvasLabel(QWidget *parent = nullptr);

signals:

    //Ruini Tong
    void setcoordinate(int x, int y);
    //----------------------

};

#endif // CANVASLABEL_H
