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
    QImage mergeCanvas; // this canvas will merge with the main canvas, mainly for alpha blending

    //Shared variable
    static int frameIndex;

public slots:
    /**
     * @author Jeffrey Le, Ruini Tong
     * @brief Called when the ImageViewEditor is clicked.
     *
     * When the Left Mouse Button is down, the View sends a signal to the Model of
     * the mouse's position on the scene. This method delegates tasks to other methods that
     * draw, erase, color pick, and fill areas of the canvas.
     *
     * @param QPoint    the mouse position on scene
     */
    void mouseDown(QPoint pos);

    /**
     * @author Jeffrey Le, Ruini Tong
     * @brief Called when the ImageViewEditor is clicked and mouse is moving.
     *
     * When the Left Mouse Button is moving, the View sends a signal to the Model of
     * the mouse's position on the scene. This method delegates tasks to other methods that
     * draw and erase lines on the canvas.
     *
     * @param QPoint    the mouse position on scene
     */
    void mouseMove(QPoint pos);

    /**
     * @author Jeffrey Le, Ruini Tong
     * @brief Called when the mouse is released.
     *
     * When the Left Mouse Button is released, the View sends a signal to the Model of
     * the mouse's position on the scene. This method considers the end of a brush stroke,
     * merging alphas with the main canvas.
     *
     * @param QPoint    the mouse position on scene
     */
    void mouseRelease(QPoint pos);


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

    /**
     * @author Andy Duong
     * @brief createNewCanvas: this method handle when user want to create a new canvas using File -> New.
     */
    void createNewCanvas(int);

    /**
     * @author Andy Duong
     * @brief saveFile: this method handle when user want to save a file using File -> Save
     * @param filename
     * @return bool, true if we sucessfully save, false other wise.
     */
    bool saveFile(const QString &filename);

    /**
     * @author Andy Duong
     * @brief openFile: this method handle when user want to save a file using File -> Open
     * @param filename
     * @return bool, true if we sucessfully open, false other wise.
     */
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

    /**
     * @author Jeffrey Le, Ruini Tong
     * @brief Creates a line between two points on the passed image.
     *
     * Creates a line between two points on the passed QImage. The line takes into account
     * the paintColor and penSize. The passed CompositionMode determines how that line
     * is effecting the canvas, either adding or erasing.
     *
     * @param QPoint            the first position
     * @param QPoint            the second position
     * @param QImage*           the image being edited
     * @param CompositionMode   the composition mode
     */
    void drawLine(QPoint p1, QPoint p2, QImage* image, QPainter::CompositionMode composition);

    /**
     * @author Andy Tran
     * @brief Model::initializeModel Initilaize the model for the program to ready
     */
    void initializeModel();

    //Ruini Tong
    QPoint pixelCurrent;//use to keep track previous pixel position when mouseMove/mouseDown was called
    int penSize = 1;

    //Andy Tran
    vector<QImage> frameList;
    int currentFrame = 0;
    int canvasSize;

    //Tingting Zhou
    QMap <QGraphicsView*, QColor> customColors;

    //Duong
    int canvasWidth;
    int canvasHeight;

signals:
    /**
     * @author Andy Tran, Jeffrey Le
     * @brief Sends canvas to View
     *
     * Sends the canvas to the View, usually after an update has occured to
     * the canvas. The View should update so the user can see the changes
     * the Model has made.
     *
     * @param QImage*       the canvas
     */
    void updateCanvas(QImage* canvas, vector<QImage>* list, int currentFrame, Action action, int newSize, int deletedIndex = -1);

    /**
     * @author Jeffrey Le
     * @brief Sends mergeCanvas to View
     *
     * Sends the mergeCanvas to the View, usually after an update has occured to
     * the mergeCanvas. Shows a preview of lines before they are applies to the
     * main canvas.
     *
     * @param QImage*       the canvas
     */
    void updatePreviewCanvas(QImage* canvas);

    /**
     * @authors Tingting Zhou, Ruini Tong
     * @brief updatePaintColor Updates the current paint color
     * @param newColor The color to set to
     */
    void updatePaintColor(QColor newColor);

    /**
     * @author Tingting Zhou
     * @brief updateAlphaSliderLabel Updates the alpha slider readings.
     * @param alphaSliderReading Alpha Slider readings.
     */
    void updateAlphaSliderLabel(QString alphaSliderReading);

    /**
     * @author Tingting Zhou
     * @brief resetAlphaSlider Set the alpha slider to a new value
     * @param newValue The new value to set to
     */
    void resetAlphaSlider(int newValue);

    //Andy Tran
    void deleteFrameWidget(int);

};

#endif // MODEL_H
