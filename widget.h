#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QThread>
#include "svmthread.h"
#define _Positive +1
#define _Negetive -1

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    int m_curType;

    SVMThread m_svmThread;

    void trainSVM(int kernelType=SVM::_Gaussian,double C=0.05, double Sigma=0.33, int P=3);

    void clearAll();
protected:
    QVector<QPointF> m_points;
    QVector<int> m_labels;

    QImage m_backImage;

    void paintEvent(QPaintEvent* e);
    void mousePressEvent(QMouseEvent* e);
    void mouseMoveEvent(QMouseEvent* e);
    void keyPressEvent(QKeyEvent* e);

public slots:
    void slot_threadDone();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
