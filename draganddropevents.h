#ifndef DRAGANDDROPEVENTS_H
#define DRAGANDDROPEVENTS_H

#include<QGraphicsView>
#include<QLabel>
#include<QDragEnterEvent>
#include<QDragLeaveEvent>
#include<QDragMoveEvent>
#include<QDropEvent>
#include<QPixmap>
#include<QMouseEvent>
#include<QPoint>
#include<QByteArray>
#include<QDrag>
#include<QMimeData>
#include<QSize>

//tzhou
class dragAndDropEvents
{
public:
    dragAndDropEvents();
};

class DragAndDropLabel : public QLabel{
    Q_OBJECT
public:
   explicit DragAndDropLabel(QWidget *widget = nullptr);
private:
    void createDrag(const QPoint &pos, QWidget *widget );
protected:
    void mousePressEvent(QMouseEvent *event) override;
   using QLabel::QLabel;


};
class DragAndDropGraphicsView : public QGraphicsView{
    Q_OBJECT

public:
   explicit DragAndDropGraphicsView(QWidget *widget = nullptr);
   void dragEnterEvent(QDragEnterEvent* event) override;
   void dragLeaveEvent(QDragLeaveEvent* event) override;
   void dragMoveEvent(QDragMoveEvent* event) override;
   void dropEvent(QDropEvent* event) override;
   using QGraphicsView::QGraphicsView;

 signals:

    void itemDrop(QPixmap, QGraphicsView*);


};

#endif // DRAGANDDROPEVENTS_H
