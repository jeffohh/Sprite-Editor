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

    //Ruini Tong cursor
    void pencilCursor();
    void eraserCursor();
    void pickerCursor();
    void bucketCursor();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent*) override;

private:
    double scale = 15; //Andy Tran: default 1500%

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
