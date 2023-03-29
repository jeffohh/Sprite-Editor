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

    QImage canvas;

public slots:
    void mouseDown(QPoint);
    void mouseMove(QPoint);
    void mouseUp(QPoint);

    //Andy Tran Added
    void changeTool(Tool currentTool);

    //Tzhou: this should be combined with setToolColor, but need Renee's consent.
    /**
     * @brief Sets the paint color to a given color.
     *        This new color comes from a sinal in View.
     *        It emits a signal to view to update color picker panel.
     * @param newColor The new color to set to.
     */
    void updatePaintColor(const QColor newColor);
    void updateAlpha(int newAlphaSliderValue);

private:
    Tool tool = PENCIL;
    QColor toolColor = Qt::black;

    //Tzhou: this should be combined with setToolColor, but need Renee's consent.
    QColor paintColor; //It is initialized in the constructor, uniform convention with the UI.

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

    //Tzhou
    void updateColorPickerPanel(QColor newColor);
    void updateAlphaSliderLabel(QString alphaSliderReading);
    void resetAlphaSlider(int max);
};

#endif // MODEL_H
