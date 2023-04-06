#ifndef FRAMEVIEW_H
#define FRAMEVIEW_H

#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QObject>
#include <QMouseEvent>
#include <QDebug>

/**
 * @author AndyTran
 * @brief FrameView::FrameView A class represent the view of the frames
 * @param parent
 * Reviewed by Andy Duong. Andy Tran has follow all the Style Guides provided by us.
 */
class FrameView : public QGraphicsView
{
    Q_OBJECT

public:
    explicit FrameView(QWidget *parent = nullptr);

    /**
     * @brief FrameView::updatePixmap Update the image for this frame
     * @param image
     */
    void updatePixmap(QImage*);

    /**
     * @brief FrameView::updateIndex Update the index of this frame
     * @param newIndex New index wants to be updated
     */
    void updateIndex(int);

    /**
     * @brief FrameView::getIndex Get the current index of this frame
     * @return The current Index
     */
    int getIndex();
protected:
    /**
     * @brief FrameView::mousePressEvent Trigger whenever the left button is clicked.
     * Sending to the Model to handle the event
     * @param event
     */
    void mousePressEvent(QMouseEvent *event) override;

    /**
     * @brief FrameView::keyPressEvent Trigger whenever the delete button (Window) or back-space button (MacOS) is clicked.
     *  Sending to the Model to handle the event
     * @param event
     */
    void keyPressEvent(QKeyEvent *event) override;
private:
    QGraphicsPixmapItem* imageItem;
    int index;

signals:
    void mouseClicked(QGraphicsPixmapItem* frame, int frameIndex);
    void deletePressed(int index);
};

#endif // FRAMEVIEW_H
