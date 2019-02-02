#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    // initialize paramters
    m_svmThread.setParent(this);

    this->m_curType = _Positive;

    m_backImage = QImage(499,551,QImage::Format_ARGB32);
    m_backImage.fill(QColor(255,255,255));

    connect(&m_svmThread,SIGNAL(sig_threadDone()),this,SLOT(slot_threadDone()));
}

void Widget::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::HighQualityAntialiasing);
    QColor colors[3] = {QColor(255,0,0),QColor(0,255,0),QColor(0,0,255)};

    painter.drawImage(0,0,this->m_backImage);

    for(unsigned int i=0; i<m_points.size(); i++)
    {
        painter.setBrush(QBrush(colors[m_labels[i]+1]));

        painter.drawEllipse(m_points[i].toPoint(),3,3);
    }
}

void Widget::trainSVM(int kernelType, double C, double Sigma, int P)
{
    m_svmThread.m_svmEngine.clearSamples();

    m_svmThread.m_svmEngine.C = C;
    m_svmThread.m_svmEngine.sigma = Sigma;
    m_svmThread.m_svmEngine.p = P;
    m_svmThread.m_svmEngine.m_kernelType = (SVM::KernelType)kernelType;

    m_svmThread.m_backImage = QImage(width(),height(),QImage::Format_ARGB32);
    m_svmThread.m_backImage.fill(QColor(255,255,255,100));

    for(unsigned int i=0; i<m_points.size(); i++)
    {
        Sample tmp;
        tmp.fID.push_back(0);
        tmp.fID.push_back(1);
        tmp.x.push_back(m_points[i].x());
        tmp.x.push_back(m_points[i].y());
        tmp.y = m_labels[i];

        m_svmThread.m_svmEngine.pushOneSample(tmp);
    }

    m_svmThread.start();
}

void Widget::slot_threadDone()
{
    this->m_backImage = m_svmThread.m_backImage;

    update();
}

void Widget::clearAll()
{
    this->m_points.clear();
    this->m_labels.clear();

    m_svmThread.m_svmEngine.clearSamples();
    this->m_backImage = QImage(499,551,QImage::Format_ARGB32);
    this->m_backImage.fill(QColor(255,255,255,255));

    update();
}

void Widget::mousePressEvent(QMouseEvent *e)
{
    if(m_svmThread.isRunning())
        return;

    m_points.push_back(QPointF(e->pos()));
    m_labels.push_back(m_curType);

    this->update();
}

void Widget::mouseMoveEvent(QMouseEvent *e)
{

}

void Widget::keyPressEvent(QKeyEvent *e)
{

}



Widget::~Widget()
{
    delete ui;
}
