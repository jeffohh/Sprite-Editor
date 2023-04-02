#include "model.h"
#include "QtGui/qpainter.h"
#include "qdebug.h"

//Initialize static variable
int Model::frameIndex = 0;

Model::Model(QObject *parent)
    : QObject{parent}
    , canvas(32, 32, QImage::Format_ARGB32)
{
    canvas.fill(Qt::white);
    paintColor = Qt::black;
    initializeModel();
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

        emit deleteFrameWidget(&canvas, &frameList, currentFrame, deletedIndex);
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

    emit updateCanvas(&canvas, &frameList, currentFrame);
}

void Model::onAddFrame(){
    canvas = QImage(32, 32, QImage::Format_ARGB32);
    canvas.fill(Qt::white);
    frameList.push_back(canvas);
    currentFrame = frameList.size() - 1;
    emit updateCanvas(&canvas, &frameList, currentFrame);
}

void Model::initializeModel(){
    frameIndex = 0;
    currentFrame = 0;
    frameList.clear();
    frameList.push_back(canvas);

    emit updateCanvas(&canvas, &frameList, currentFrame);
    qDebug() << "initializeModel called";
}



// [=== TOOL SECTION ===] @Ruini
// --- Tool Input ---
void Model::mouseDown(QPoint pos) {

    if(isPressed){
        if(isPos){
            posEnd = pos;
            isPos = false;
        }else{
            posBegin = posEnd;
            isPos = true;
        }
    }else{
        posBegin = pos;
        posEnd = pos;
    }

    drawLine(posBegin,posEnd);

    //Andy Tran: update frameList and update view
    frameList[currentFrame] = canvas;
    emit updateCanvas(&canvas, &frameList, currentFrame);
}

void Model::mousePressed(bool pressed){
    isPressed = pressed;
}

// --- Tool Modify ---
void Model::drawLine(QPoint posOne,QPoint posTwo){
    QPainter painter(&canvas);
    QPen pen;
    pen.setWidth(penSize);

    switch (tool) {
    case PENCIL:
        pen.setColor(paintColor);
        break;
    case PICKER:
        return;
    case ERASER:
        pen.setColor(Qt::white);
        break;
    case BUCKET:
        return;
    default:
        break;
    }

    painter.setPen(pen);
    painter.drawLine(posOne.x(),posOne.y(),posTwo.x(),posTwo.y());

    emit updateCanvas(&canvas, &frameList, currentFrame);
}

void Model::fillColor(QColor originColor, QPoint pos){

//recursive method, crash sometimes because stack overflow
//    //if pos's color == the originColor, update that color
//    QColor pixelColor = canvas.pixelColor(pos);
//    if (pixelColor == originColor){
//        canvas.setPixelColor(pos, paintColor);
//        emit updateCanvas(&canvas, &frameList, currentFrame);
//    }else{//no more same color to fill
//        return;
//    }

//    //expand top
//    QPoint topPoint(pos.x(), pos.y()+1);
//    fillColor(originColor,topPoint);

//    //expand bottom
//    QPoint bottomPoint(pos.x(), pos.y()-1);
//    fillColor(originColor,bottomPoint);

//    //expand left
//    QPoint leftPoint(pos.x()-1, pos.y());
//    fillColor(originColor,leftPoint);

//    //expand right
//    QPoint rightPoint(pos.x()+1, pos.y());
//    fillColor(originColor,rightPoint);

    QList<QPoint> stack;
    stack <<pos;
    while(!stack.empty()){
        QPoint current = stack.takeLast();
        QColor pixelColor = canvas.pixelColor(current);
        if (pixelColor == originColor){
            canvas.setPixelColor(current, paintColor);
            emit updateCanvas(&canvas, &frameList, currentFrame);

            //    //expand top
                QPoint topPoint(current.x(), current.y()+1);
                stack<<topPoint;

            //    //expand bottom
                QPoint bottomPoint(current.x(), current.y()-1);
                stack<<bottomPoint;

            //    //expand left
                QPoint leftPoint(current.x()-1, current.y());
                stack<<leftPoint;

            //    //expand right
                QPoint rightPoint(current.x()+1, current.y());
                stack<<rightPoint;

        }
    }

}

void Model::getColor(QPoint pos){
    QColor pixelColor = canvas.pixelColor(pos);

    qDebug()<< pos<<" "<< pixelColor;

    switch (tool) {
    case PICKER:
        paintColorChanged(pixelColor);
        return;
    case BUCKET:
        fillColor(pixelColor, pos);
        return;
    default:
        break;
    }
}

// --- Tool Settings ---
void Model::changeTool(Tool currentTool){
    tool = currentTool;
    qDebug() << "changeTool called" << tool;
}

void Model::setPenSize(int size){
    penSize = size/10;
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
    canvas.fill(Qt::white);

    //Andy Tran Edited
    initializeModel();
    emit newCanvasCreated();
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

       // Set the current frame to the first frame
       currentFrame = 0;
       canvas = frameList[currentFrame];

       emit updateCanvas(&canvas, &frameList, currentFrame);
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
        qDebug()<<"selcted";
        //emit resetAlphaSlider(10);
    }

}


