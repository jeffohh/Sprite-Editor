#include "model.h"
#include "QtGui/qpainter.h"
#include "qdebug.h"


Model::Model(QObject *parent)
    : QObject{parent}
    , canvas(32, 32, QImage::Format_ARGB32)
{
    canvas.fill(Qt::white);
    paintColor = Qt::black;
    initializeFrames();
}

//Andy Tran - Frames part
void Model::onAddFrame(){
    qDebug() << "onAddFrame called";
    canvas = QImage(32, 32, QImage::Format_ARGB32);
    canvas.fill(Qt::white);
    frameList.push_back(canvas);
    currentFrame++;
    emit updateCanvas(&canvas, &frameList, currentFrame);
}

void Model::initializeFrames(){
    frameList.clear();
    QImage scaledCanvas= canvas.scaled(previewSize, Qt::KeepAspectRatio, Qt::FastTransformation);
    frameList.push_back(scaledCanvas);
}

//void Model::updateFrames(){
//    frameList[currentFrame] = canvas;
//}
//-------------------------------------------------------

//Ruini
//----------------------Tool part -------------------------
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

void Model::mousePressed(bool pressed, QGraphicsPixmapItem* frame){
    isPressed = pressed;

    //Andy Tran - need to keep track of currentFrame -> update the canvas
//    QPixmap pixmap = frame->pixmap();
//    canvas = pixmap.toImage();

}

void Model::drawLine(QPoint posOne,QPoint posTwo){

    //qDebug() << tool;
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

void Model::setPenSize(int size){
    penSize = size/10;
}

//void Model::mouseUp(QPoint) {

//}

void Model::changeTool(Tool currentTool){
    tool = currentTool;
    qDebug() << "changeTool called" << tool;
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

//----------------------Color part -------------------------

//Renee, Tzhou
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

//Duong
void Model::createNewCanvas(int width, int height){

    //Create an canvas with given width and height.
    canvas = QImage(width,height, QImage::Format_ARGB32);
    canvas.fill(Qt::white);

    //Andy Tran: update frameList and update view
    currentFrame = 0;
    initializeFrames();
    emit updateCanvas(&canvas, &frameList, currentFrame);

    emit newCanvasCreated();
}

void Model::frameSelected(int index) {
    currentFrame = index;
    canvas = frameList[currentFrame];
}
