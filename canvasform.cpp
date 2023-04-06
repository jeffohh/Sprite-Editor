#include "canvasform.h"

/* Code reviewd by Tingting Zhou*/
CanvasForm::CanvasForm(QWidget *parent, int currentSize, Action action): QDialog(parent)
{
        this->action = action;

        this->setWindowTitle("Create New Sprite");
        if(action == RESIZE){
            this->setWindowTitle("Set Sprite Size");
        }

        QSpinBox* heightSpinBox = new QSpinBox(this);
        QPushButton* okButton = new QPushButton("OK", this);
        QPushButton* cancelButton = new QPushButton("Cancel", this);
        QHBoxLayout* buttonLayout = new QHBoxLayout();
        buttonLayout->addWidget(okButton);
        buttonLayout->addWidget(cancelButton);

        // Set the range, step size, and suffix for the spin boxes
        widthSpinBox->setRange(1, 500);
        widthSpinBox->setSingleStep(1);
        widthSpinBox->setSuffix(" px");
        heightSpinBox->setRange(1, 500);
        heightSpinBox->setSingleStep(1);
        heightSpinBox->setSuffix(" px");

        // Connect the valueChanged() signals of the spin boxes to each other
        connect(widthSpinBox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
                heightSpinBox, &QSpinBox::setValue);
        connect(heightSpinBox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
                widthSpinBox, &QSpinBox::setValue);

        // Set the width and height spin boxes to the same initial value
        int initialValue = currentSize;
        widthSpinBox->setValue(initialValue);
        heightSpinBox->setValue(initialValue);

        // Create a layout for the spin boxes
        QGridLayout* layout = new QGridLayout();
        layout->addWidget(new QLabel("Width:", this), 1, 0);
        layout->addWidget(widthSpinBox, 1, 1);
        layout->addWidget(new QLabel("Height:", this), 2, 0);
        layout->addWidget(heightSpinBox, 2, 1);
        layout->addLayout(buttonLayout, 3, 0, 1, 2);
        this->setLayout(layout);

        // Connect the buttons to slots
        connect(okButton, &QPushButton::clicked, this, &CanvasForm::handleSubmitButton);

        // Connect the buttons to slots
        connect(cancelButton, &QPushButton::clicked, this, [=]{
            close();
        });
}

/* Code reviewd by Tingting Zhou*/
void CanvasForm::handleSubmitButton(){
    this->newSize = widthSpinBox->value();
    if(newSize > 0){
        if(action == RESIZE)
            emit resizeFrameList(newSize);
        else if(action == CREATE_NEW){
            emit createNewCanvas(newSize);
        }
        close();
    }else{
        QMessageBox::warning(this, "Invalid input", "Please enter valid values for width and height.");
    }
}


