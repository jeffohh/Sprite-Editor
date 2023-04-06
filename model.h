#ifndef MODEL_H
#define MODEL_H

#include "qimage.h"
#include <QObject>

//Duong
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>

//AndyTran
#include <QPixmap>
#include <QGraphicsPixmapItem>

//tzhou
#include<QGraphicsView>
#include<QMap>

//Ruini Tong
#include <QCursor>

using std::vector;

//Enum Global Declaration
enum Tool {
    PENCIL,
    PICKER,
    ERASER,
    BUCKET
};
enum Action{
    UPDATE,
    DELETE_FRAME,
    OPEN_FILE,
    CREATE_NEW,
    RESIZE,
    FPS_CHANGED
};

class Model : public QObject
{
    Q_OBJECT

public:
    explicit Model(QObject *parent = nullptr);

    //Main canvas
    QImage canvas;
    QImage mergeCanvas;

    //Shared variable
    static int frameIndex;

public slots:
    /**
     * @author Jeffery, Ruini Tong
     * @brief when the mouse is down, start drawing/erasing, pick current color or fill color
     */
    void mouseDown(QPoint);

    /**
     * @author Jeffery, Ruini Tong
     * @brief mouse move will only be triggered while the mouse is down, draw lines on canvas
     */
    void mouseMove(QPoint);
    void mouseRelease(QPoint);

    //Andy Tran
    /**
     * @brief Model::initializeModel Initilaize the model for the program to ready
     */
    void initializeModel();

    /**
     * @author Andy Tran
     * @brief changeTool change the tool using
     */
    void changeTool(Tool);

    /**
     * @author Andy Tran
     * @brief Model::mouseClicked Whenever the frame widget was clicked.
     * The FrameView class calls the Model to handle the event and send update to the View
     * @param frame The FrameView pixmap
     * @param frameIndex Current Index of the FrameView
     */
    void mouseClicked(QGraphicsPixmapItem*, int);

    /**
     * @author Andy Tran
     * @brief Model::onAddFrame Trigger whenever the Add Button is clicked.
     * Adding one more frame to the list and send update to the View
     */
    void onAddFrame();

    /**
     * @author Andy Tran
     * @brief Model::deletePressed Take an action whenever the Delete Button pressed
     * Action: Shift left or right the current frame
     * @param deletedIndex
     */
    void deletePressed(int);

    /**
     * @author Andy Tran
     * @brief Model::resizeFrameList: This method resized the frame to fit when a new canvas is created.
     * @param newSize:
     */
    void resizeFrameList(int);

    /**
     * @author Andy Tran
     * @brief changeFPS trigger an update to the View when the FPS changed
     */
    void changeFPS();

    /**
     * @brief Sets the paint color to a given color.
     *        This new color comes from a sinal in View.
     *        It emits a signal to view to update color picker panel.
     * @param newColor The new color to set to.
     */
    void paintColorChanged(const QColor newColor);//Tzhou, Ruini Tong
    void updateColorRelated(int newAlphaSliderValue);
    void updateCustomColor(QGraphicsView* view);
    void customColorIsSelected(QGraphicsView* view);

    //Duong
    void createNewCanvas(int);
    bool saveFile(const QString &filename);
    bool openFile(const QString &filename);

    /**
     * @author Ruini Tong
     * @brief change pen size to the value user selected
     * @param size
     */
    void setPenSize(int size);

    /**
     * @author Ruini Tong
     * @brief fill color of certain area
     * @param originColor in the position that was clicked
     * @param position that user clicked
     */
    void fillColor(QColor originColor, QPoint pos);

private:
    Tool tool = PENCIL;
    QColor paintColor; //It is initialized in the constructor, uniform convention with the UI.

    // Jeff
    void drawLine(QPoint, QPoint, QImage*, QPainter::CompositionMode);

    //Ruini Tong
    QPoint pixelCurrent;//use to keep track previous pixel position when mouseMove/mouseDown was called
    int penSize = 1;

    //Andy Tran
    vector<QImage> frameList;
    int currentFrame = 0;
    int canvasSize;

    //tzhou
    QMap <QGraphicsView*, QColor> customColors;

    //Duong
    int canvasWidth;
    int canvasHeight;

private slots:

signals:
    /**
     * @author AndyTran Jeff
     * @brief Sends canvas to View
     * Sends the canvas to the View, usually after an update has occured to
     * the canvas. The View should update so the user can see the changes
     * the Model has made.
     *
     * @param QImage        the canvas
     */
    void updateCanvas(QImage* canvas, vector<QImage>* list, int currentFrame, Action action, int newSize, int deletedIndex = -1);

    void updatePreviewCanvas(QImage* canvas);

    //Renee, Tzhou
    void updatePaintColor(QColor newColor);

    //Tzhou
    void updateAlphaSliderLabel(QString alphaSliderReading);
    void resetAlphaSlider(int newValue);

    //Andy Tran
    void deleteFrameWidget(int);

    //Duong
    void centerAndAutoZoom(int, int);
};

#endif // MODEL_H
