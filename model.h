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

    //Ruini Tong
    void setToolColor(const QColor newColor);
    //Andy Tran Edited
//    void changeTool(Tool currentTool);
    //----------------------
    QImage canvas;

public slots:
    void mouseDown(QPoint);
    void mouseMove(QPoint);
    void mouseUp(QPoint);

    //Andy Tran Added
    void changeTool(Tool currentTool);

    //Ruini Tong
    void mousePressed(bool pressed);
    void setPenSize(int size);

private:
    Tool tool = ERASER;
    QColor toolColor = Qt::black;

    //Ruini Tong
    QPoint posBegin;
    QPoint posEnd;
    bool isPos = false;
    QPoint newPoint;
    bool isPressed = false;
    int penSize = 1;

private slots:
    void drawLine(QPoint posOne,QPoint posTwo);

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

    //Ruini Tong
    void newPosition(QPoint pos);

};

#endif // MODEL_H
