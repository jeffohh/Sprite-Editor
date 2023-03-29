#include "model.h"
#include "QtGui/qpainter.h"
#include "qdebug.h"

Model::Model(QObject *parent)
    : QObject{parent}
    , canvas(32, 32, QImage::Format_RGB32)
{
    canvas.fill(Qt::white);
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


    //qDebug() << "model: " <<pos;

    switch (tool) {

    case PENCIL:
        // set pixel at location
        canvas.setPixelColor(pos, toolColor);

        break;

    case ERASER:
        // set pixel at location
        canvas.setPixelColor(pos, Qt::white);

        break;

    default:
        break;

    }

    //Ruini's Edit:


    // update view
    emit updateCanvas(&canvas);
}

void Model::mousePressed(bool pressed){
    isPressed = pressed;
    qDebug() << isPressed;
}

void Model::drawLine(QPoint posOne,QPoint posTwo){

    QPainter painter(&canvas);
    QPen pen;
    pen.setWidth(penSize);
    if(tool == PENCIL){
        pen.setColor(toolColor);
    }else{
        pen.setColor(Qt::white);
    }

    painter.setPen(pen);
    painter.drawLine(posOne.x(),posOne.y(),posTwo.x(),posTwo.y());

    emit updateCanvas(&canvas);
}

void Model::setPenSize(int size){
    penSize = size/10;
}

void Model::setToolColor(const QColor newColor){
    toolColor = newColor;
}

void Model::changeTool(Tool currentTool){
    tool = currentTool;
    qDebug() << "changeTool called";
}

void Model::mouseMove(QPoint) {

}

void Model::mouseUp(QPoint) {

}
