#include "model.h"

Model::Model(QObject *parent)
    : QObject{parent}
    , canvas(32, 32, QImage::Format_RGB32)
{
    canvas.fill(Qt::red);
}


void Model::mouseDown(QPoint pos) {

    switch (tool) {

    case PENCIL:
        // set pixel at location
        canvas.setPixelColor(pos, toolColor);

        // update view
        emit updateCanvas(&canvas);

        break;

    default:
        break;

    }
}

void Model::mouseMove(QPoint) {

}

void Model::mouseUp(QPoint) {

}
