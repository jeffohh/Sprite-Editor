#include "canvasform.h"
#include <QVBoxLayout>
#include <QMessageBox>

CanvasForm::CanvasForm(QWidget *parent): QDialog(parent)
{
        widthInput = new QLineEdit(this);
        heightInput = new QLineEdit(this);
        widthLabel = new QLabel("Width", this);
        heightLabel = new QLabel("Height", this);
        submitButton = new QPushButton("Submit", this);

        QVBoxLayout *layout = new QVBoxLayout();
        layout->addWidget(widthLabel);
        layout->addWidget(widthInput);

        layout->addWidget(heightLabel);
        layout->addWidget(heightInput);

        layout->addWidget(submitButton);

        setLayout(layout);

        connect(submitButton, &QPushButton::clicked, this, &CanvasForm::handleSubmitButton);
}

void CanvasForm::handleSubmitButton(){
    bool widthOK, heightOK;
    int width = widthInput->text().toInt(&widthOK);
    int height = heightInput->text().toInt(&heightOK);

    if(widthOK && heightOK && width > 0 && height > 0){
        emit canvasSizeChanged(width, height);
        close();
    } else{
         QMessageBox::warning(this, "Invalid input", "Please enter valid values for width and height.");
    }
}


