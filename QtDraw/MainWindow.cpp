#include "MainWindow.h"
MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
{
	this->setAcceptDrops(true);
	this->setMouseTracking(true);
	ui.setupUi(this);
	initWindow();
	connentEvent();
}

MainWindow::~MainWindow()
{}
void MainWindow::initWindow()
{
	ui.layout->setContentsMargins(0, 0, 0, 0);
	ui.layout->setSpacing(0);
	viewer = new ImageViewer(this->width(), this->height());
	ui.layout->addWidget(viewer);
	setMinimumSize(300, 150);
	//int h = this->height();
}
void MainWindow::connentEvent()
{
}
