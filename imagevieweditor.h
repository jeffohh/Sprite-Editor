#ifndef IMAGEVIEWEDITOR_H
#define IMAGEVIEWEDITOR_H

#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QObject>

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
    double scale = 1;

    QGraphicsPixmapItem* imageItem;

signals:
    void mouseDown(QPoint);
};

#endif // IMAGEVIEWEDITOR_H
