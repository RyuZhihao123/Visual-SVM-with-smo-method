#ifndef SVMTHREAD_H
#define SVMTHREAD_H
#include "svm.h"
#include <QImage>
#include <QThread>

class SVMThread : public QThread
{
    Q_OBJECT

public:
    SVM m_svmEngine;
    QImage m_backImage;

protected:

    void run();

signals:
    void sig_threadDone();
};

#endif // SVMTHREAD_H
