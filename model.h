#ifndef MODEL_H
#define MODEL_H

#include <QObject>

// global declaration so view can access too?
enum Tool {
    PENCIL,
    BRUSH,
    ERASER,
    BUCKET
};

class Model : public QObject
{
    Q_OBJECT

public:
    explicit Model(QObject *parent = nullptr);

public slots:


private:


private slots:


signals:


};

#endif // MODEL_H
