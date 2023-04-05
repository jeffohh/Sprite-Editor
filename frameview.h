#ifndef FRAMEVIEW_H
#define FRAMEVIEW_H

#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QObject>
#include <QMouseEvent>
#include <QDebug>

//Andy Tran - Frames use - Global frameIndex
class FrameView : public QGraphicsView
{
    Q_OBJECT

public:
    explicit FrameView(QWidget *parent = nullptr);
    void updatePixmap(QImage*);
    void updateIndex(int);
    int getIndex();
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
private:
    QGraphicsPixmapItem* imageItem;
    int index;

signals:
    void mouseClicked(QGraphicsPixmapItem* frame, int frameIndex);
    void deletePressed(int index);
};

#endif // FRAMEVIEW_H
