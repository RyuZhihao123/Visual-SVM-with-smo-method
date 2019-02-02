#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("基于SMO的SVM可视化小程序 —— RyuZhihao123");
    m_widget = new Widget(ui->centralWidget);

    connect(ui->btnChangeColor,SIGNAL(clicked(bool)),this,SLOT(slot_btnChangeColor()));
    connect(ui->btnSVM,SIGNAL(clicked(bool)),this,SLOT(slot_btnSVM()));
    connect(&(m_widget->m_svmThread),SIGNAL(sig_threadDone()),this,SLOT(slot_btnThreadDone()));
    connect(ui->btnClear,SIGNAL(clicked(bool)),this,SLOT(slot_btnClear()));

    ui->lblColor->setText("<font color=blue>Blue(+1)</font>");
}

void MainWindow::resizeEvent(QResizeEvent *e)
{
    ui->groupBox->move(ui->centralWidget->width()-ui->groupBox->width(),0);
    m_widget->resize(ui->centralWidget->width()-ui->groupBox->width(),ui->centralWidget->height());
}

void MainWindow::slot_btnChangeColor()
{
    if(m_widget->m_curType == _Positive)
    {
        m_widget->m_curType = _Negetive;
        ui->lblColor->setText("<font color=red >Red(-1)</font>");
    }
    else
    {
        m_widget->m_curType = _Positive;
        ui->lblColor->setText("<font color=blue>Blue(+1)</font>");
    }
}

void MainWindow::slot_btnSVM()
{
    if(m_widget->m_svmThread.isRunning())
        return;

    ui->lblState->setText("开始训练（高斯核训练约<font color=red>6-10</font>秒）");

    SVM::KernelType type;
    if(ui->cbxKernelType->currentIndex()==0)
        type = SVM::_linear;
    if(ui->cbxKernelType->currentIndex()==1)
        type = SVM::_Gaussian;
    m_widget->trainSVM(type, ui->spinC->value(),ui->spinSigma->value());

    // set UI unabled;
    ui->btnChangeColor->setDisabled(true);
    ui->btnSVM->setDisabled(true);
    ui->cbxKernelType->setDisabled(true);
}

void MainWindow::slot_btnClear()
{
    m_widget->clearAll();
}

void MainWindow::slot_btnThreadDone()
{
    ui->lblState->setText("完成");

    // set UI abled;
    ui->btnChangeColor->setDisabled(false);
    ui->btnSVM->setDisabled(false);
    ui->cbxKernelType->setDisabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}
