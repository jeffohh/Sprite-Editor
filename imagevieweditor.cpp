#include "imagevieweditor.h"
/*Code reviewed by Ruini Tong, Andy Duong, Tingting Zhou*/
ImageViewEditor::ImageViewEditor(QWidget *parent) :
    QGraphicsView(parent)
{

    imageItem = new QGraphicsPixmapItem();

    imagePreviewItem = new QGraphicsPixmapItem();
    imagePreviewItem->setParentItem(imageItem);

    // create cross pattern, apply it to RectItem's background
    QBrush cross(Qt::CrossPattern);
    QPen noPen(Qt::NoPen);

    rectBackground = new QGraphicsRectItem(imageItem->boundingRect());
    rectBackground->setBrush(cross);
    rectBackground->setPen(noPen);

    // the imageItem's Pixmap is now "parented" to "rectBackground"
    imageItem->setParentItem(rectBackground);

    // create a scene, add the "rectBackground" to the scene
    QGraphicsScene* scene = new QGraphicsScene();
    scene->addItem(rectBackground);
    this->setScene(scene);

    QPixmap pencilImage(":/images/icons/Pencil.PNG");
    pencilImage = pencilImage.scaled(32, 32, Qt::KeepAspectRatio);
    QCursor pencilCursor(pencilImage, 1,25);
    setCursor(pencilCursor);
}



// [=== CANVAS SECTION ===] @Jeffrey
void ImageViewEditor::updatePixmap(QImage* image) {
    imageItem->setPixmap(QPixmap::fromImage(*image));
    rectBackground->setRect(imageItem->boundingRect());

    setSceneRect(rectBackground->boundingRect());
    centerOn(rectBackground->boundingRect().center());
    setTransform(QTransform::fromScale(scale, scale));
}

void ImageViewEditor::updatePreviewPixmap(QImage* image) {
    imagePreviewItem->setPixmap(QPixmap::fromImage(*image));
}



// [=== INPUT SECTION ===] @Jeffrey
void ImageViewEditor::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        QPoint pos = mapToScene(event->position().toPoint()).toPoint();
        emit mouseDown(pos);
    }
}

void ImageViewEditor::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        //Andy Tran Edited
        QPoint pos = mapToScene(event->position().toPoint()).toPoint();
        emit mouseMove(pos);
    }
}

void ImageViewEditor::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        QPoint pos = mapToScene(event->position().toPoint()).toPoint();
        emit mouseRelease(pos);
    }

    event->ignore();
}

void ImageViewEditor::wheelEvent(QWheelEvent *event)
{
    // Zoom in or out based on the wheel delta
    if (event->angleDelta().y() > 0) {
        scale *= 1.2;
    } else {
        scale /= 1.2;
    }

    scale = std::max(0.2, std::min(scale, 160.0));

    // Set the new zoom level
    setTransform(QTransform().scale(scale, scale));
}

//change cursor image
void ImageViewEditor::pencilCursor(){

    QPixmap pencilImage(":/images/icons/Pencil.PNG");
    pencilImage = pencilImage.scaled(32, 32, Qt::KeepAspectRatio);
    QCursor pencilCursor(pencilImage, 1,25);
    setCursor(pencilCursor);
}

void ImageViewEditor::eraserCursor(){
    QPixmap eraserImage(":/images/icons/Eraser.PNG");
    eraserImage = eraserImage.scaled(32, 32, Qt::KeepAspectRatio);
    QCursor eraserCursor(eraserImage, 1,25);
    setCursor(eraserCursor);
}

void ImageViewEditor::pickerCursor(){
    QPixmap pickerImage(":/images/icons/Picker.PNG");
    pickerImage = pickerImage.scaled(32, 32, Qt::KeepAspectRatio);
    QCursor pickerCursor(pickerImage, 1,25);
    setCursor(pickerCursor);
}

void ImageViewEditor::bucketCursor(){
    QPixmap bucketImage(":/images/icons/Bucket.PNG");
    bucketImage = bucketImage.scaled(32, 32, Qt::KeepAspectRatio);
    QCursor bucketCursor(bucketImage, 1,1);
    setCursor(bucketCursor);
}


