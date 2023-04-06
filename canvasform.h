#ifndef CANVASFORM_H
#define CANVASFORM_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include "model.h"
#include <QSpinBox>
#include <QMessageBox>
#include <QInputDialog>
#include <QHBoxLayout>

/**
 * @author: Andy Duong
 * @brief CanvasForm::CanvasForm: This method handle create a new canvas with all required widgets.
 * @param parent
 * @param currentSize
 * @param action
 */
class CanvasForm : public QDialog
{
    Q_OBJECT
public:
    explicit CanvasForm(QWidget *parent = nullptr, int newsize = 32, Action action = CREATE_NEW);
signals:
    void createNewCanvas(int);
    void resizeFrameList(int);

private slots:
    /**
     * @author Andy Tran
     * @brief CanvasForm::handleSubmitButton: This method handle when the user click "Submit" on the form.
     */
    void handleSubmitButton();

private:
    QSpinBox* widthSpinBox = new QSpinBox(this);
    Action action;
    int newSize;
};

#endif // CANVASFORM_H
