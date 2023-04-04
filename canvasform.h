#ifndef CANVASFORM_H
#define CANVASFORM_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

class CanvasForm : public QDialog
{
    Q_OBJECT
public:
    explicit CanvasForm(QWidget *parent = nullptr);
signals:
    void createNewCanvas(int width, int height);

private slots:
    void handleSubmitButton();

private:
    QLineEdit *widthInput;
    QLineEdit *heightInput;
    QPushButton *submitButton;
    QLabel *widthLabel;
    QLabel *heightLabel;
};

#endif // CANVASFORM_H
