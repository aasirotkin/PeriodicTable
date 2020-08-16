#include "mainwindow.h"

#include <QApplication>
#include <QScreen>
#include <QResizeEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    const QRect rect = QApplication::screens().first()->geometry();
    const int three_quarter_width = 0.75 * rect.width();
    const int three_quarter_height = 0.75 * rect.height();

    scene_ = new PeriodicTableScene(this);

    view_ = new QGraphicsView(scene_, this);
    view_->setRenderHint(QPainter::Antialiasing, true);
    view_->setMouseTracking(true);
    view_->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    view_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view_->setFrameStyle(QFrame::NoFrame);

    setCentralWidget(view_);
    resize(three_quarter_width, three_quarter_height);
}

MainWindow::~MainWindow()
{
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    const int width = event->size().width();
    const int height = event->size().height();
    view_->setSceneRect(0.0, 0.0, width, height);
    scene_->scaleItems(width, height);
    QMainWindow::resizeEvent(event);
}

void MainWindow::leaveEvent(QEvent *event)
{
    scene_->mouseLeaveEvent();
    QMainWindow::leaveEvent(event);
}

