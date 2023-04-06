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

    /**
     * @author Jeffrey Le
     * @brief Updates the Pixmap to the Model's main canvas.
     *
     * Updates the Pixmap of the graphical view to the Model's main canvas.
     * Resizes Pixmap's background and centers the graphical view.
     *
     * @param QImage*       the main canvas
     */
    void updatePixmap(QImage* image);

    /**
     * @author Jeffrey Le
     * @brief Updates the Pixmap to the Model's merging canvas.
     *
     * Updates the Pixmap of the graphical view to the Model's merging canvas.
     *
     * @param QImage*       the merging canvas
     */
    void updatePreviewPixmap(QImage* image);

    //Ruini Tong cursor
    void pencilCursor();
    void eraserCursor();
    void pickerCursor();
    void bucketCursor();

protected:
    /**
     * @author Jeffrey Le
     * @brief Called when mouse button is pressed in widget.
     *
     * Filters only Left Mouse Button clicks.
     * Calcluates the QPoint of where the user clicked on the graphical view,
     * relative to the items in the graphical scene. Emits them to the Model.
     *
     * @param QMouseEvent*      the pointer to the mouse event
     */
    void mousePressEvent(QMouseEvent *event) override;

    /**
     * @author Jeffrey Le
     * @brief Called when mouse button is pressed and moving in widget.
     *
     * Filters only Left Mouse Button moves.
     * Calcluates the QPoint of where the user clicked on the graphical view,
     * relative to the items in the graphical scene. Emits them to the Model.
     *
     * @param QMouseEvent*      the pointer to the mouse event
     */
    void mouseMoveEvent(QMouseEvent *event) override;

    /**
     * @author Jeffrey Le
     * @brief Called when mouse button is released.
     *
     * Filters only Left Mouse Button releases.
     * Calcluates the QPoint of where the user clicked on the graphical view,
     * relative to the items in the graphical scene. Emits them to the Model.
     *
     * @param QMouseEvent*      the pointer to the mouse event
     */
    void mouseReleaseEvent(QMouseEvent *event) override;

    /**
     * @author Jeffrey Le
     * @brief Called when mouse wheel inputs.
     *
     * Called when the mouse wheel inputs, increasing or decreasing the scale amount.
     *
     * The scale is used to transform the graphical view, making it graphical scene objects
     * bigger and smaller. The scale amount is clamped to limit how far or close the user can view.
     *
     * @param QWheelEvent*      the pointer to the mouse wheel event
     */
    void wheelEvent(QWheelEvent *event) override;

private:
    //Scale default 1580%
    double scale = 15.8;

    QGraphicsPixmapItem* imageItem; // represents main canvas
    QGraphicsPixmapItem* imagePreviewItem; // represent merging canvas
    QGraphicsRectItem* rectBackground; // represents background grid, canvas bounds

signals:
    /**
     * @author Jeffrey Le
     * @brief Fires the mouse position when the LMB is down.
     *
     * Used to emit the mouse position at which a click has occured on the
     * graphical scene.
     *
     * @param QPoint    the mouse position on scene
     */
    void mouseDown(QPoint pos);

    /**
     * @author Jeffrey Le
     * @brief Fires the mouse position when the LMB is down and moving.
     *
     * Used to emit the mouse position at which mouse movement has occured on the
     * graphical scene.
     *
     * @param QPoint    the mouse position on scene
     */
    void mouseMove(QPoint pos);

    /**
     * @author Jeffrey Le
     * @brief Fires the mouse position when the LMB is release.
     *
     * Used to emit the mouse position at which a release has occured.
     *
     * @param QPoint    the mouse position on scene
     */
    void mouseRelease(QPoint pos);
};

#endif // IMAGEVIEWEDITOR_H
