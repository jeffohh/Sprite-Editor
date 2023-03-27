#ifndef MODEL_H
#define MODEL_H

#include "qimage.h"
#include <QObject>

// global declaration so view can access too?
enum Tool {
    PENCIL,
    BRUSH,
    ERASER,
    BUCKET
};

class Model : public QObject
{
    Q_OBJECT

public:
    explicit Model(QObject *parent = nullptr);

    void setToolColor(const QColor newColor);
    void changeTool(Tool currentTool);

    QImage canvas;

public slots:
    void mouseDown(QPoint);
    void mouseMove(QPoint);
    void mouseUp(QPoint);

private:
    Tool tool = PENCIL;
    QColor toolColor = Qt::black;

private slots:


signals:
    /**
     * @brief Sends canvas to View
     *
     * Sends the canvas to the View, usually after an update has occured to
     * the canvas. The View should update so the user can see the changes
     * the Model has made.
     *
     * @param QImage        the canvas
     */
    void updateCanvas(QImage* canvas);

};

#endif // MODEL_H
