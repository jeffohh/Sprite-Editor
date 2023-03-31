#ifndef MODEL_H
#define MODEL_H

#include "qimage.h"
#include <QObject>

//Duong
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>

//Andy Tran
#include <QPixmap>
#include <QGraphicsPixmapItem>
using std::vector;

// global declaration so view can access too?
enum Tool {
    PENCIL,
    PICKER,
    ERASER,
    BUCKET
};

class Model : public QObject
{
    Q_OBJECT

public:
    explicit Model(QObject *parent = nullptr);

    // main canvas
    QImage canvas;

    //shared variable
    static int frameIndex;

public slots:
    void mouseDown(QPoint);
    //void mouseMove(QPoint);
    //void mouseUp(QPoint);

    //Andy Tran Added
    void changeTool(Tool currentTool);
    void mouseClicked(QGraphicsPixmapItem*, int);
    void initializeModel();
    void onAddFrame();

    //Tzhou: this should be combined with setToolColor, but need Renee's consent.
    /**
     * @brief Sets the paint color to a given color.
     *        This new color comes from a sinal in View.
     *        It emits a signal to view to update color picker panel.
     * @param newColor The new color to set to.
     */
    void paintColorChanged(const QColor newColor);//Tzhou, Ruini Tong
    void updateAlpha(int newAlphaSliderValue);

    //Duong
    void createNewCanvas(int width, int height);
//    void frameSelected(int index);
    void saveFile(const QString &filename);
    void openFile(const QString &filename);


    //Ruini Tong
    void mousePressed(bool pressed);
    void setPenSize(int size);
    void drawLine(QPoint posOne,QPoint posTwo);
    void getColor(QPoint pos);

private:
    Tool tool = PENCIL;
    QColor paintColor; //It is initialized in the constructor, uniform convention with the UI.

    //Ruini Tong
    QPoint posBegin;
    QPoint posEnd;
    bool isPos = false;
    QPoint newPoint;
    bool isPressed = false;
    int penSize = 1;

    //Andy Tran
    vector<QImage> frameList;
    int currentFrame = 0;

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
    void updateCanvas(QImage* canvas, vector<QImage>* list, int currentFrame);

    //Renee, Tzhou
    void updatePaintColor(QColor newColor);
    //Tzhou
    void updateAlphaSliderLabel(QString alphaSliderReading);
    void resetAlphaSlider(int max);
    //---------------------------------------------

    //Ruini Tong
    void newPosition(QPoint pos);

    //Duong
    void newCanvasCreated();



};

#endif // MODEL_H
