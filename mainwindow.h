#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "periodictablescene.h"

#include <QMainWindow>
#include <QGraphicsView>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    PeriodicTableScene* scene_;
    QGraphicsView* view_;

protected:
    void resizeEvent(QResizeEvent *event);
    void leaveEvent(QEvent *event);

signals:
    void mouseLeaveEvent();
};
#endif // MAINWINDOW_H
