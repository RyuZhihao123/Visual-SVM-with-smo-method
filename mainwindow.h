#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "widget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    Widget* m_widget;

protected:
    void resizeEvent(QResizeEvent* e);

private slots:
    void slot_btnChangeColor();
    void slot_btnSVM();
    void slot_btnThreadDone();
    void slot_btnClear();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
