#include "model.h"
#include "QtGui/qpainter.h"
#include "qdebug.h"

Model::Model(QObject *parent)
    : QObject{parent}
    , canvas(32, 32, QImage::Format_ARGB32)
{
    canvas.fill(Qt::white);
    paintColor = Qt::black;
    frameList.push_back(canvas);

    //Testing purpose
    QImage frame1(32,32, QImage::Format_ARGB32);
    frame1.fill(Qt::red);
    frameList.push_back(frame1);

    //Testing purpose
    QImage frame2(32,32, QImage::Format_ARGB32);
    frame2.fill(Qt::blue);
    frameList.push_back(frame2);
}



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
    emit updateCanvas(&canvas, &frameList);

}

void Model::mousePressed(bool pressed){
    isPressed = pressed;
    //qDebug() << isPressed;
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
    case ERASER:
        pen.setColor(Qt::white);

        break;
    default:
        break;
    }

    painter.setPen(pen);
    painter.drawLine(posOne.x(),posOne.y(),posTwo.x(),posTwo.y());

    emit updateCanvas(&canvas, &frameList);
}

void Model::setPenSize(int size){
    penSize = size/10;
}


//void Model::mouseUp(QPoint) {

//}

void Model::changeTool(Tool currentTool){
    tool = currentTool;
    //qDebug() << "changeTool called";
}

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

void Model::createNewCanvas(int width, int height){

    //Create an canvas with given width and height.
    canvas = QImage(width,height, QImage::Format_ARGB32);
    canvas.fill(Qt::white);

    //Andy Tran: update frameList and update view
    frameList[currentFrame] = canvas;
    emit updateCanvas(&canvas, &frameList);
}
