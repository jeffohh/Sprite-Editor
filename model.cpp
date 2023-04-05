#include "model.h"
#include "QtGui/qpainter.h"
#include "qdebug.h"

//Initialize static variable
int Model::frameIndex = 0;

Model::Model(QObject *parent)
    : QObject{parent}
    , canvas(32, 32, QImage::Format_ARGB32)
{
    canvasSize = 32;
    canvas.fill(Qt::transparent);
    paintColor = Qt::black;
    initializeModel();

    canvasRect = canvas.rect();// get canvas size
    canvasWidth = canvas.width();
    canvasHeight = canvas.height();
}

// [=== INITIAL SECTION ===] @Andy Tran
/**
 * @brief Model::initializeModel Initilaize the model for the program to ready
 */
void Model::initializeModel(){
    frameIndex = 0;
    currentFrame = 0;
    frameList.clear();
    frameList.push_back(canvas);
}

// [=== FRAMES SECTION ===] @Andy Tran
/**
 * @brief Model::deletePressed Take an action whenever the Delete Button pressed
 * Action: Shift left or right the current frame
 * @param deletedIndex
 */
void Model::deletePressed(int deletedIndex){
    //Delete if have more than one frame
    if(frameList.size() > 1){
        //Delete the frame by deletedIndex
        frameList.erase(frameList.begin() + deletedIndex);

        //Shift to the left
        if((deletedIndex - 1) >= 0){
            currentFrame = deletedIndex - 1;
        }else{ //Shift to the right
            currentFrame = deletedIndex;
        }

        //Update canvas to current frame
        canvas = frameList[currentFrame];

        //Update static variable
        frameIndex--;

        emit updateCanvas(&canvas, &frameList, currentFrame, DELETE_FRAME, canvasSize, deletedIndex);
    }
}

/**
 * @brief Model::mouseClicked Whenever the frame widget was clicked.
 * The FrameView class calls the Model to handle the event and send update to the View
 * @param frame The FrameView pixmap
 * @param frameIndex Current Index of the FrameView
 */
void Model::mouseClicked(QGraphicsPixmapItem* frame, int frameIndex){
    //Update current frame
    this->currentFrame = frameIndex;

    //Update canvas to new pixmap
    QPixmap pixmap = frame->pixmap();
    canvas =  pixmap.toImage().convertToFormat(QImage::Format_ARGB32);

    //Update the Frame List with new canvas
    frameList[currentFrame] = canvas;

    //Send signals to the View to handle the update
    emit updateCanvas(&canvas, &frameList, currentFrame, UPDATE, canvasSize);
}

/**
 * @brief Model::onAddFrame Trigger whenever the Add Button is clicked.
 * Adding one more frame to the list and send update to the View
 */
void Model::onAddFrame(){
    //Create new blank canvas and send signals to the view to update
    canvas = QImage(canvasSize, canvasSize, QImage::Format_ARGB32);
    canvas.fill(Qt::transparent);
    frameList.push_back(canvas);
    currentFrame = frameList.size() - 1;
    emit updateCanvas(&canvas, &frameList, currentFrame, UPDATE, canvasSize);

}

// [=== TOOL SECTION ===] @Ruini
// --- Tool Input ---
void Model::mouseDown(QPoint pos) {
    pixelCurrent = pos;

    mergeCanvas = QImage(canvas.width(), canvas.height(), canvas.format());
    mergeCanvas.fill(Qt::transparent);
    //if (!canvasRect.contains(pos)) return; //if the pixel is out of bound, return

    QColor pixelColor = canvas.pixelColor(pos);
    //paintColor = pixelColor + paintColor;

    //int red = pixelColor.red() + paintColor.red();
    //int green = pixelColor.green() + paintColor.green();
    //int blue = pixelColor.blue() + paintColor.blue();
    //int alpha = (pixelColor.alpha() + paintColor.alpha()) / 2;

    // Create the new color with the calculated values
    //QColor paintColor = QColor::fromRgbF(red / 510.0, green / 510.0, blue / 510.0, alpha / 255.0);

    switch (tool) {
    case PENCIL:
        drawLine(pixelCurrent, pos, &mergeCanvas, QPainter::CompositionMode_Source); // Jeffrey: draw "line" so it retains its Pen size
        break;
    case ERASER:
        drawLine(pixelCurrent, pos, &canvas, QPainter::CompositionMode_Clear);
        break;
    case PICKER:
        paintColorChanged(pixelColor);
        return;
    case BUCKET:
        fillColor(pixelColor, pos);
        return;
    default:
        return; // Jeffrey: return by default, cleaner code :-)
    }

    //Andy Tran: update frameList and update view
    frameList[currentFrame] = canvas;
    emit updatePreviewCanvas(&mergeCanvas);
    emit updateCanvas(&canvas, &frameList, currentFrame, UPDATE, canvasSize);
}

void Model::mouseMove(QPoint pos) {

    canvasRect = canvas.rect(); // Jeffrey: is canvasRect a needed variable? we could call canvas.rect() instead
    if (!canvasRect.contains(pos)) return; //if the pixel is out of bound, return

    switch (tool) {
    case PENCIL:
        drawLine(pixelCurrent, pos, &mergeCanvas, QPainter::CompositionMode_Source);
        break;
    case ERASER:
        drawLine(pixelCurrent, pos, &canvas, QPainter::CompositionMode_Clear);
        break;
    default:
        return; // Jeffrey: just return by default, other tools don't matter!
    }

    pixelCurrent = pos;

    frameList[currentFrame] = canvas;
    emit updatePreviewCanvas(&mergeCanvas);
    emit updateCanvas(&canvas, &frameList, currentFrame, UPDATE, canvasSize);

}

void Model::mouseRelease(QPoint) {
    QPainter painter(&canvas);
    painter.drawImage(0, 0, mergeCanvas);
    painter.end();

    mergeCanvas.fill(Qt::transparent);

    frameList[currentFrame] = canvas;
    emit updatePreviewCanvas(&mergeCanvas);
    emit updateCanvas(&canvas, &frameList, currentFrame, UPDATE, canvasSize);
}

// --- Tool Modify ---
void Model::drawLine(QPoint p1, QPoint p2, QImage* image, QPainter::CompositionMode composition) {
    QPen pen; // Jeffrey: this can be a private member, potentially replacing penSize and paintColor
    pen.setWidth(penSize);
    pen.setColor(paintColor);

    QPainter painter(image);
    painter.setPen(pen);
    painter.setCompositionMode(composition);

    painter.drawLine(p1.x(),p1.y(),p2.x(),p2.y());
}

void Model::fillColor(QColor originColor, QPoint pos){

    //if the canvas color is the same as paintColor, don't need to fill
    QColor canvasColor = canvas.pixelColor(pos);
    double threshold = 0.1;
    qDebug()<<canvasColor;
    qDebug()<<paintColor;

    double aDiff = abs(canvasColor.alpha() - paintColor.alpha());
    double rDiff = abs(canvasColor.red() - paintColor.red());
    double gDiff = abs(canvasColor.green() - paintColor.green());
    double bDiff = abs(canvasColor.blue() - paintColor.blue());

    // check if the difference is within the threshold
    if ((aDiff <= threshold)&&(rDiff <= threshold) && (gDiff <= threshold) && (bDiff <= threshold)) {
        qDebug() <<"return";
        return;
    }

    QList<QPoint> stack;
    stack <<pos;
    while(!stack.empty()){
        QPoint current = stack.takeLast();
        if (canvasRect.contains(current)) { // check if pixel is out of bound
            QColor pixelColor = canvas.pixelColor(current); //get pixel color
            if (pixelColor == originColor){
                canvas.setPixelColor(current, paintColor);

                //expand top
                QPoint topPoint(current.x(), current.y()+1);
                stack<<topPoint;

                //expand bottom
                QPoint bottomPoint(current.x(), current.y()-1);
                stack<<bottomPoint;

                //expand left
                QPoint leftPoint(current.x()-1, current.y());
                stack<<leftPoint;

                //expand right
                QPoint rightPoint(current.x()+1, current.y());
                stack<<rightPoint;

            }
        }
    }
    frameList[currentFrame] = canvas;
    emit updateCanvas(&canvas, &frameList, currentFrame, UPDATE, canvasSize);

}

// --- Tool Settings ---
void Model::changeTool(Tool currentTool){
    tool = currentTool;
    qDebug() << "changeTool called" << tool;
}

void Model::setPenSize(int size){
    penSize = size/5;
}



// [=== COLOR SECTION ===] @Tzhou @Ruini
void Model::paintColorChanged(QColor newColor)
{
    paintColor=newColor;
    int alphaSliderValue = paintColor.alpha()/255.0*10;
    updateColorRelated(alphaSliderValue);
    emit resetAlphaSlider(alphaSliderValue);
}

//Tzhou
void Model::updateColorRelated(int newAlphaSliderValue)
{
    int newAlpha = 255*newAlphaSliderValue/10.0;
    paintColor.setAlpha(newAlpha);
    QString alphaSliderReading = QString::number(newAlphaSliderValue/10.0, 'f', 1);

    emit updatePaintColor(paintColor);
    emit updateAlphaSliderLabel(alphaSliderReading);
    qDebug() << "paint: "<<paintColor.red()<<" "<<
                paintColor.green()<<" "
               <<paintColor.blue()<<" "
              <<paintColor.alpha();
}


// [=== CANVAS SECTION ===] @Duong @Andy Tran
/**
 * @author Andy Tran
 * @brief Model::resizeFrameList: This method resized the frame to fit when a new canvas is created.
 * @param newSize:
 */
void Model::resizeFrameList(int newSize){

    //This one worked
    qDebug() << "resizeFrameList called " << newSize;
    for (unsigned int i = 0; i < frameList.size(); i++) {
        // Create a new QImage of size new_size
        QImage newImage(newSize, newSize, QImage::Format_ARGB32);

        newImage.fill(Qt::transparent);

        // Calculate the center position of the new image
        int center_x = (newSize - canvasSize) / 2;
        int center_y = (newSize - canvasSize) / 2;

        // Draw the original QImage onto the new QImage at the center position
        QPainter painter(&newImage);
        painter.drawImage(center_x, center_y, frameList[i]);
        painter.end();
        frameList[i] = newImage;
    }

    canvas = frameList[currentFrame];
    canvasSize = newSize;
    emit updateCanvas(&canvas, &frameList, currentFrame, RESIZE, canvasSize);
}

/**
 * @brief Model::createNewCanvas: This method is used to create a new canvas with a given size.
 * @param newSize
 */
void Model::createNewCanvas(int newSize){

    //AndyTran edited:
    canvasSize = newSize;

    //Create an canvas with given width and height.
    canvas = QImage(canvasSize,canvasSize, QImage::Format_ARGB32);
    canvas.fill(Qt::transparent);

    //Andy Tran Edited
    initializeModel();
    canvasSize = newSize;
    emit updateCanvas(&canvas, &frameList, currentFrame, CREATE_NEW, canvasSize);

}

/**
 * @brief Model::saveFile: This method is used to create a JSON file with height, width, number of frames and all the frame, using QJason classes.
 * @param filename
 * @return true if this save sucessfully, false otherwise.
 */
bool Model::saveFile(const QString &filename)
{
    QJsonObject project;
        project["height"] = canvas.height();
        project["width"] = canvas.width();
        project["numberOfFrames"] = static_cast<int>(frameList.size());

    QJsonArray framesArray;
        for (size_t frameIndex = 0; frameIndex < frameList.size(); ++frameIndex) {
            const QImage &frame = frameList[frameIndex];
            QJsonArray frameArray;

            for (int row = 0; row < canvas.height(); ++row) {
                QJsonArray rowArray;
                for (int col = 0; col < canvas.width(); ++col) {
                    QColor pixel = frame.pixelColor(col, row);
                    QJsonArray pixelArray{
                        pixel.red(),
                        pixel.green(),
                        pixel.blue(),
                        pixel.alpha()
                    };
                    rowArray.append(pixelArray);
                }
                frameArray.append(rowArray);
            }

            framesArray.append(frameArray);
        }

        project["frames"] = framesArray;

        QJsonDocument doc(project);
        QFile file(filename);
        if (!file.open(QIODevice::WriteOnly)) {
            return false;
        }

        file.write(doc.toJson());
        return true;
    }

/**
 * @brief Model::openFIle: This method is used read through a JSON file with height, width, number of frames and all the frame, using QJason classes and create
 * new canvas with those information.
 * @param filename
 * @return true if this open sucessfully, false otherwise.
 */
bool Model::openFile(const QString &filename)
{
    QFile file(filename);
        if (!file.open(QIODevice::ReadOnly)) {
            return false;
        }

        QByteArray data = file.readAll();
        QJsonDocument doc(QJsonDocument::fromJson(data));
        QJsonObject project = doc.object();

        int height = project["height"].toInt();
        int width = project["width"].toInt();

        int numberOfFrames = project["numberOfFrames"].toInt();       
        QJsonArray framesArray = project["frames"].toArray();

        //Andy Tran editted
        Model::frameIndex = 0;

        frameList.clear();
        frameList.reserve(static_cast<size_t>(numberOfFrames));

        for (int frameIndex = 0; frameIndex < numberOfFrames; ++frameIndex) {
            QJsonArray frameArray = framesArray[frameIndex].toArray();
            QImage frame(width, height, QImage::Format_ARGB32);

            for (int row = 0; row < height; ++row) {
                QJsonArray rowArray = frameArray[row].toArray();

                for (int col = 0; col < width; ++col) {
                    QJsonArray pixelArray = rowArray[col].toArray();
                    QColor pixel(
                        pixelArray[0].toInt(),
                        pixelArray[1].toInt(),
                        pixelArray[2].toInt(),
                        pixelArray[3].toInt()
                    );
                    frame.setPixelColor(col, row, pixel);
                }
            }

            frameList.push_back(frame);
        }

       // Set the current frame to the first frame
       currentFrame = 0;
       canvas = frameList[currentFrame];

       emit updateCanvas(&canvas, &frameList, currentFrame, OPEN_FILE, canvasSize);
       return true;
}

//---------------------Extra Feature------------
//Tzhou
void Model::updateCustomColor(QGraphicsView *view)
{
    if(customColors.contains(view)){
        customColors[view] = paintColor;
    }
    else
        customColors.insert(view,paintColor);
}

//Tzhou
void Model::customColorIsSelected(QGraphicsView* view)
{
    if(customColors.contains(view)){
        paintColor=customColors[view];
        int alphaSliderValue = paintColor.alpha()/255.0*10;
        updateColorRelated(alphaSliderValue);
        emit resetAlphaSlider(alphaSliderValue);

    }

}


