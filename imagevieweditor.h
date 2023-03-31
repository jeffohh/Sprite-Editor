#ifndef IMAGEVIEWEDITOR_H
#define IMAGEVIEWEDITOR_H

#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QObject>
#include <QDebug>

class ImageViewEditor : public QGraphicsView
{
    Q_OBJECT

public:
    explicit ImageViewEditor(QWidget *parent = nullptr);
    void updatePixmap(QImage*);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent*) override;

private:
    double scale = 15; //Andy Tran: default 1500%
    bool pencil = false;
    bool eraser = false;

    //Ruini Tong for testing mouse movement
    int count = 0;

    QGraphicsPixmapItem* imageItem;

signals:
    void mouseDown(QPoint);
    void mouseMove(QPoint);
    void mouseRelease(QPoint);

    //Ruini Tong
    void mousePressed(bool pressed);
    void getColor(QPoint);
    //-------------------------------------------

};

#endif // IMAGEVIEWEDITOR_H
