/**
 * @author Tingting Zhou
 *  Reviewed by Andy Duong. Tingting has follow all the Style Guides provided by us.
 **/

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

/**
 * @brief The dragAndDropEvents class This class is a class for custom QLabel and QGraphicsView for Drag and Drop effect
 */
class dragAndDropEvents
{
public:
    dragAndDropEvents();
};

/**
 * @brief The DragAndDropLabel class Custom QLabel class
 */
class DragAndDropLabel : public QLabel{
    Q_OBJECT

public:

    /**
    * @brief DragAndDropLabel Constructor
    * @param widget QWidget
    */
   explicit DragAndDropLabel(QWidget *widget = nullptr);

private:

    /**
     * @brief createDrag Creates a drag event.
     * @param pos Position of the mouse
     * @param widget The widget owns this drag
     */
    void createDrag(const QPoint &pos, QWidget *widget );
protected:

    /**
     * @brief mousePressEvent Mouse event for dragging.
     * @param event QMouseEvent
     */
    void mousePressEvent(QMouseEvent *event) override;
    using QLabel::QLabel;

};

/**
 * @brief The DragAndDropGraphicsView class Custom QGraphicsView class
 */
class DragAndDropGraphicsView : public QGraphicsView{
    Q_OBJECT

public:

    /**
    * @brief DragAndDropGraphicsView Constructor
    * @param widget QWidget
    */
   explicit DragAndDropGraphicsView(QWidget *widget = nullptr);

    /**
    * @brief dragEnterEvent Drag enters a QGraphicsView event
    * @param event QDragEnterEvent
    */
   void dragEnterEvent(QDragEnterEvent* event) override;

   /**
    * @brief dragLeaveEvent Drag leaves a QGraphicsView event
    * @param event QDragLeaveEvent
    */
   void dragLeaveEvent(QDragLeaveEvent* event) override;

   /**
    * @brief dragMoveEvent Drag moves event
    * @param event QDragMoveEvent
    */
   void dragMoveEvent(QDragMoveEvent* event) override;

   /**
    * @brief dropEvent A drop event
    * @param event QDropEvent
    */
   void dropEvent(QDropEvent* event) override;

   /**
    * @brief mousePressEvent Mouse press event
    * @param event QMouseEvent
    */
   void mousePressEvent(QMouseEvent *event) override;
   using QGraphicsView::QGraphicsView;

 signals:

   /**
     * @brief itemDrop Item is dropped to a QGraphicsView
     */
    void itemDrop(QPixmap, QGraphicsView*);

    /**
     * @brief customColorIsSelected A saved custom color is selected
     */
    void customColorIsSelected(QGraphicsView*);

};

#endif // DRAGANDDROPEVENTS_H
