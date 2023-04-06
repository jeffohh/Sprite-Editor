#include "helpmenu.h"
#include "ui_helpmenu.h"

/*Code reviewed by Jeffery Le*/
helpmenu::helpmenu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::helpmenu)
{
    ui->setupUi(this);
}

helpmenu::~helpmenu()
{
    delete ui;
}
