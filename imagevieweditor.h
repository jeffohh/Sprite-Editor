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
    void pencilClicked();
    void eraserClicked();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    void wheelEvent(QWheelEvent*) override;

private:
    double scale = 1;
    bool pencil = false;
    bool eraser = false;

    QGraphicsPixmapItem* imageItem;

signals:
    void mouseDown(QPoint);
};

#endif // IMAGEVIEWEDITOR_H
