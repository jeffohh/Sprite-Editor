#ifndef IMAGEVIEWEDITOR_H
#define IMAGEVIEWEDITOR_H

#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QObject>
#include <QDebug>
#include <QMouseEvent>

class ImageViewEditor : public QGraphicsView
{
    Q_OBJECT

public:
    explicit ImageViewEditor(QWidget *parent = nullptr);
    void updatePixmap(QImage* image);
    void updatePreviewPixmap(QImage* image);

    //Ruini Tong cursor
    void pencilCursor();
    void eraserCursor();
    void pickerCursor();
    void bucketCursor();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    //Scale default 1580%
    double scale = 15.8;

    QGraphicsPixmapItem* imageItem;
    QGraphicsPixmapItem* imagePreviewItem;
    QGraphicsRectItem* rectBackground;

signals:
    void mouseDown(QPoint pos);
    void mouseMove(QPoint pos);
    void mouseRelease(QPoint pos);
};

#endif // IMAGEVIEWEDITOR_H
