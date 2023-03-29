#include "model.h"
#include "qdebug.h"

Model::Model(QObject *parent)
    : QObject{parent}
    , canvas(32, 32, QImage::Format_ARGB32)
{
    canvas.fill(Qt::white);
    paintColor = Qt::black;
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
        canvas.setPixelColor(pos, Qt::white);
        emit updateCanvas(&canvas);
        break;

    default:
        break;

    }
}

// we
void Model::mouseMove(QPoint pos) {
    switch (tool) {

    case PENCIL:
        canvas.setPixelColor(pos, toolColor);
        emit updateCanvas(&canvas);
        break;

    case ERASER:
        canvas.setPixelColor(pos, Qt::white);
        emit updateCanvas(&canvas);
        break;

    default:
        break;

    }
}

void Model::mouseUp(QPoint) {

}



void Model::setToolColor(const QColor newColor){
    toolColor = newColor;
}

void Model::changeTool(Tool currentTool){
    tool = currentTool;
    qDebug() << "changeTool called";
}

void Model::updatePaintColor(QColor newColor)
{
    paintColor=newColor;
    emit updateColorPickerPanel(paintColor);
    emit resetAlphaSlider(10);
}

void Model::updateAlpha(int newAlphaSliderValue)
{
    int newAlpha = 255*newAlphaSliderValue/10.0;;
    paintColor.setAlpha(newAlpha);
    QString alphaSliderReading = QString::number(newAlphaSliderValue/10.0, 'f', 1);

    emit updateColorPickerPanel(paintColor);
    emit updateAlphaSliderLabel(alphaSliderReading);

    toolColor=paintColor;// this could be removed after combining paintColor.
    qDebug() << "paint: "<<paintColor.red()<<" "<<
                paintColor.green()<<" "
               <<paintColor.blue()<<" "
              <<paintColor.alpha();


}

