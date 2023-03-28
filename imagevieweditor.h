#ifndef IMAGEVIEWEDITOR_H
#define IMAGEVIEWEDITOR_H

#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QObject>
#include <QDebug>
#include "model.h"

class ImageViewEditor : public QGraphicsView
{
    Q_OBJECT

public:
    explicit ImageViewEditor(QWidget *parent = nullptr);

    void updatePixmap(QImage*);
    void pencilClicked();
    void eraserClicked();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    void wheelEvent(QWheelEvent*) override;

private:
    double scale = 15; //Andy Tran: default 1500%
    bool pencil = false;
    bool eraser = false;

    QGraphicsPixmapItem* imageItem;

signals:
    void mouseDown(QPoint);
    void mouseMove(QPoint);
    void mouseRelease(QPoint);

    void changeTool(Tool);
};

#endif // IMAGEVIEWEDITOR_H
