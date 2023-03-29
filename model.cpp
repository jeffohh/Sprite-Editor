#include "model.h"
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
    switch (tool) {
    case PENCIL:
        // set pixel at location
        canvas.setPixelColor(pos, paintColor);
        break;
    case ERASER:
        canvas.setPixelColor(pos, Qt::white);
        break;
    default:
        break;
    }

    //Andy Tran: update frameList and update view
    frameList[currentFrame] = canvas;
    emit updateCanvas(&canvas, &frameList);
}

// we
void Model::mouseMove(QPoint pos) {
    switch (tool) {

    case PENCIL:
        canvas.setPixelColor(pos, paintColor);
        break;
    case ERASER:
        canvas.setPixelColor(pos, Qt::white);
        break;
    default:
        break;
    }

    //Andy Tran: update frameList and update view
    frameList[currentFrame] = canvas;
    emit updateCanvas(&canvas, &frameList);
}

void Model::mouseUp(QPoint) {

}

void Model::changeTool(Tool currentTool){
    tool = currentTool;
    qDebug() << "changeTool called";
}

//Renee, Tzhou
void Model::updatePaintColor(QColor newColor)
{
    paintColor=newColor;
    emit updateColorPickerPanel(paintColor);
    emit resetAlphaSlider(10);
}

//Tzhou
void Model::updateAlpha(int newAlphaSliderValue)
{
    int newAlpha = 255*newAlphaSliderValue/10.0;;
    paintColor.setAlpha(newAlpha);
    QString alphaSliderReading = QString::number(newAlphaSliderValue/10.0, 'f', 1);

    emit updateColorPickerPanel(paintColor);
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
