#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "model.h"

//Added by Andy Tran
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QImage>
#include <QPixmap>
#include <QVector>
#include <QTimer>
using std::vector;
//----------------

//TZhou
#include<QColorDialog>
#include<QColor>
//-------------------


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(Model& model,QWidget *parent = nullptr);
    ~MainWindow();

    //Andy Tran
    void previewAnimation();
    //----------------------

public slots:
    void updateCanvas(QImage*); // jeff!!

    //Andy Tran
    void onTimerTimeout();

private:
    Ui::MainWindow *ui;

    //Andy Tran
    vector<QImage> frameList;
    QGraphicsScene* scene = new QGraphicsScene(this);
    QTimer *timer = new QTimer(this);
    int fps = 12;
    int frameDuration;
    //------------------------

    //TZhou
    QColor DEFAULT_PAINT_COLOR = Qt::black; // Default pen or brush color
    //----------------------

private slots:

    //-----------------TZhou: color picker area----------------------
    void setPaintColorView(QColor newColor);
    void on_changeColorBtn_clicked();
    //-------------------------------------------

    void on_fpsSlider_valueChanged(int value);

signals:

    //Renee
    // used by any color change
    void updateColor(QColor);

};
#endif // MAINWINDOW_H
