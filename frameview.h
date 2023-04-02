#ifndef FRAMEVIEW_H
#define FRAMEVIEW_H

#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QObject>
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
//    double scale = 15; //Andy Tran: default 1500%
    QGraphicsPixmapItem* imageItem;
    int index;

signals:
    void mouseClicked(QGraphicsPixmapItem* frame, int frameIndex);
    void deletePressed(int index);
};

#endif // FRAMEVIEW_H
