#ifndef CANVASFORM_H
#define CANVASFORM_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include "model.h"
#include<QSpinBox>
#include <QMessageBox>
#include<QInputDialog>
#include <QHBoxLayout>

class CanvasForm : public QDialog
{
    Q_OBJECT
public:
    explicit CanvasForm(QWidget *parent = nullptr, int newsize = 32, Action action = CREATE_NEW);
signals:
    void createNewCanvas(int);
    void resizeFrameList(int);

private slots:
    void handleSubmitButton();

private:
//    QLineEdit *widthInput;
//    QLineEdit *heightInput;
//    QPushButton *submitButton;
//    QLabel *widthLabel;
//    QLabel *heightLabel;

    QSpinBox* widthSpinBox = new QSpinBox(this);
    Action action;
    int newSize;
};

#endif // CANVASFORM_H
