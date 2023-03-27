#include "model.h"
#include "qdebug.h"

Model::Model(QObject *parent)
    : QObject{parent}
    , canvas(32, 32, QImage::Format_RGB32)
{
    canvas.fill(Qt::white);
}


void Model::mouseDown(QPoint pos) {

    switch (tool) {

    case PENCIL:
        // set pixel at location
        canvas.setPixelColor(pos, toolColor);

        // update view
        emit updateCanvas(&canvas);

        break;

    case ERASER:
        // set pixel at location
        canvas.setPixelColor(pos, Qt::white);

        // update view
        emit updateCanvas(&canvas);

        break;

    default:
        break;

    }
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
