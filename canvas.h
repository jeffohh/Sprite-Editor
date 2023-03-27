#ifndef CANVAS_H
#define CANVAS_H

#include <QGraphicsView>
#include <QObject>
#include <QWidget>

class canvas : public QGraphicsView
{
    Q_OBJECT
public:
public:
    explicit canvas(QWidget *parent = nullptr);

signals:
    void viewClicked();

protected:
    void mousePressEvent(QMouseEvent *event) override;
};

#endif // CANVAS_H
