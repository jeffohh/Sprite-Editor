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

<<<<<<< HEAD
=======
//Andy Tran - Frames part
void Model::deletePressed(int deletedIndex){
    frameIndex--;
    frameList.erase(frameList.begin() + deletedIndex);
    //Delete if have more than one frame
    if(frameList.size() > 1){
        //Shift to the left
        if((deletedIndex - 1) >= 0){
            currentFrame = deletedIndex - 1;
        }else{ //Shift to the right
            currentFrame = deletedIndex;
        }

        canvas = frameList[currentFrame];


        emit deleteFrameWidget(&canvas, &frameList, currentFrame, deletedIndex);
    }
}

>>>>>>> local


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
    int newAlpha = 255*newAlphaSliderValue/10.0;;
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
//Duong
void Model::createNewCanvas(int width, int height){

    //Create an canvas with given width and height.
    canvas = QImage(width,height, QImage::Format_ARGB32);
    canvas.fill(Qt::white);

    //Andy Tran Edited
    initializeModel();
    emit newCanvasCreated();
}

//Andy Tran: I think we don't need use this one any more since I had combine your part and mine
//void Model::frameSelected(int index) {
//    currentFrame = index;
//    canvas = frameList[currentFrame];
//}

void Model::saveFile(const QString &filename)
{
    QJsonObject project;
    project["height"] = canvas.height();
    project["width"] = canvas.width();
    project["numberOfFrames"] = static_cast<int>(frameList.size());

    QJsonArray frames;

    for (size_t i = 0; i < frameList.size(); ++i) {
        QJsonArray frameData;

        QImage frame = frameList[i];
        for (int y = 0; y < frame.height(); ++y) {
            QJsonArray row;
            for (int x = 0; x < frame.width(); ++x) {
                QColor color = frame.pixelColor(x, y);
                QJsonArray pixel = {color.red(), color.green(), color.blue(), color.alpha()};
                row.append(pixel);
            }
            frameData.append(row);
        }

        QString frameName = QString("frame%1").arg(i);
        frames.append(QJsonObject({{frameName, frameData}}));
    }

    project["frames"] = frames;

    QFile file(filename);
    if (file.open(QIODevice::WriteOnly)) {
        QJsonDocument doc(project);
        file.write(doc.toJson());
    }
}

//Known bugs: Open File does not reset the frame when open. Open File does not display all frame when open (but will shows old frames if click news)
//Open File sometime does not create new canvas and frame when open (If one already exist, it does not close and exit out everything, but just added ontop of the old canvas)
void Model::openFile(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning("Failed to open the file for reading.");
        return;
    }

    QByteArray data = file.readAll();
    QJsonDocument doc(QJsonDocument::fromJson(data));
    QJsonObject project = doc.object();

    int height = project["height"].toInt();
    int width = project["width"].toInt();
    int numberOfFrames = project["numberOfFrames"].toInt();
    QJsonArray frames = project["frames"].toArray();

    frameList.clear();

    for (int i = 0; i < numberOfFrames; ++i) {
        QJsonObject frameObject = frames[i].toObject();
        QString frameName = QString("frame%1").arg(i);
        QJsonArray frameData = frameObject[frameName].toArray();

        QImage frame(width, height, QImage::Format_ARGB32);

        for (int y = 0; y < height; ++y) {
            QJsonArray row = frameData[y].toArray();
            for (int x = 0; x < width; ++x) {
                QJsonArray pixel = row[x].toArray();
                int r = pixel[0].toInt();
                int g = pixel[1].toInt();
                int b = pixel[2].toInt();
                int a = pixel[3].toInt();
                QColor color(r, g, b, a);
                frame.setPixelColor(x, y, color);
            }
        }

        frameList.push_back(frame);


    // Update the current canvas and frames
    canvas = frameList[currentFrame];
    emit updateCanvas(&canvas, &frameList, currentFrame);
}
}
