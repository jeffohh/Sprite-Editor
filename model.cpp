#include "model.h"
#include "QtGui/qpainter.h"
#include "qdebug.h"

//Initialize static variable
int Model::frameIndex = 0;

Model::Model(QObject *parent)
    : QObject{parent}
    , canvas(32, 32, QImage::Format_ARGB32)
{
    canvas.fill(Qt::transparent);
    paintColor = Qt::black;
    initializeModel();

    canvasRect = canvas.rect();// get canvas size
    canvasWidth = canvas.width();
    canvasHeight = canvas.height();
}

//Andy Tran - Frames part
void Model::deletePressed(int deletedIndex){
    //Delete if have more than one frame
    if(frameList.size() > 1){

        frameList.erase(frameList.begin() + deletedIndex);

        //Shift to the left
        if((deletedIndex - 1) >= 0){
            currentFrame = deletedIndex - 1;
            qDebug() << "Shift Left: " << currentFrame;
        }else{ //Shift to the right
            currentFrame = deletedIndex;
            qDebug() << "Shift right: " << currentFrame;
        }
        canvas = frameList[currentFrame];

        //Update static variable
        frameIndex--;

        emit updateCanvas(&canvas, &frameList, currentFrame, DELETE_FRAME, deletedIndex);
    }
}

// [=== FRAMES SECTION ===] @Andy Tran
void Model::mouseClicked(QGraphicsPixmapItem* frame, int frameIndex){
    this->currentFrame = frameIndex;

    qDebug() << "Model::mouseClicked "<<frameIndex;
    //Andy Tran - need to keep track of currentFrame -> update the canvas
    QPixmap pixmap = frame->pixmap();
    canvas = pixmap.toImage();

    frameList[currentFrame] = canvas;

    emit updateCanvas(&canvas, &frameList, currentFrame, UPDATE);
}

void Model::onAddFrame(){
    canvas = QImage(canvasWidth, canvasHeight, QImage::Format_ARGB32);
    canvas.fill(Qt::transparent);
    frameList.push_back(canvas);
    currentFrame = frameList.size() - 1;
    emit updateCanvas(&canvas, &frameList, currentFrame, UPDATE);

}

void Model::initializeModel(){
    frameIndex = 0;
    currentFrame = 0;
    frameList.clear();
    frameList.push_back(canvas);
//    emit updateCanvas(&canvas, &frameList, currentFrame, INIT);
}



// [=== TOOL SECTION ===] @Ruini
// --- Tool Input ---
void Model::mouseDown(QPoint pos) {
    pixelCurrent = pos;

    mergeCanvas = QImage(canvas.width(), canvas.height(), canvas.format());
    mergeCanvas.fill(Qt::transparent);
    //if (!canvasRect.contains(pos)) return; //if the pixel is out of bound, return

    QColor pixelColor = canvas.pixelColor(pos);

    switch (tool) {
    case PENCIL:
        drawLine(pixelCurrent, pos, &mergeCanvas, QPainter::CompositionMode_Source); // Jeffrey: draw "line" so it retains its Pen size
        //mergeCanvas.setPixelColor(pos, paintColor);
        break;
    case PICKER:
        paintColorChanged(pixelColor);
        return;
    case ERASER:
        return;
    case BUCKET:
        fillColor(pixelColor, pos);
        return;
    default:
        break;
    }

    //Andy Tran: update frameList and update view
    frameList[currentFrame] = canvas;
    emit updatePreviewCanvas(&mergeCanvas);
    emit updateCanvas(&canvas, &frameList, currentFrame, UPDATE);
}

void Model::mouseMove(QPoint pos) {

    canvasRect = canvas.rect();
    if (!canvasRect.contains(pos)) return; //if the pixel is out of bound, return

    switch (tool) {
    case PENCIL:
        drawLine(pixelCurrent, pos, &mergeCanvas, QPainter::CompositionMode_Source);
        break;
    case PICKER:
        return;
    case ERASER:
        drawLine(pixelCurrent, pos, &canvas, QPainter::CompositionMode_Clear);
        break;
    case BUCKET:
        return;
    default:
        break;
    }

    pixelCurrent = pos;

    frameList[currentFrame] = canvas;
    emit updatePreviewCanvas(&mergeCanvas);
    emit updateCanvas(&canvas, &frameList, currentFrame, UPDATE);

}

void Model::mouseRelease(QPoint) {
    QPainter painter(&canvas);
    painter.drawImage(0, 0, mergeCanvas);
    painter.end();

    mergeCanvas.fill(Qt::transparent);

    frameList[currentFrame] = canvas;
    emit updatePreviewCanvas(&mergeCanvas);
    emit updateCanvas(&canvas, &frameList, currentFrame, UPDATE);
}

// --- Tool Modify ---
void Model::drawLine(QPoint p1, QPoint p2, QImage* image, QPainter::CompositionMode composition) {
    QPainter painter(image);

    QPen pen;
    pen.setWidth(penSize);
    pen.setColor(paintColor);
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
    emit updateCanvas(&canvas, &frameList, currentFrame, UPDATE);

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
    emit updatePaintColor(paintColor);
    emit resetAlphaSlider(10);
}

//Tzhou
void Model::updateAlpha(int newAlphaSliderValue)
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

void Model::createNewCanvas(int width, int height){

    //Create an canvas with given width and height.
    canvas = QImage(width,height, QImage::Format_ARGB32);
    canvas.fill(Qt::transparent);


    // Store the current canvas width and height
    canvasWidth = width;
    canvasHeight = height;

    //Andy Tran Edited
    initializeModel();
    emit updateCanvas(&canvas, &frameList, currentFrame, CREATE_NEW);
    emit centerAndAutoZoom(width, height);
}


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
            QImage frame(width, height, QImage::Format_RGBA8888);

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

       qDebug() << "frameIndex: " << frameIndex;
       qDebug() << "frameList.size(): " << frameList.size();

       // Set the current frame to the first frame
       currentFrame = 0;
       canvas = frameList[currentFrame];

       emit updateCanvas(&canvas, &frameList, currentFrame, OPEN_FILE);
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
        emit updatePaintColor(paintColor);
        qDebug()<<"selected";
        //emit resetAlphaSlider(10);
    }

}


