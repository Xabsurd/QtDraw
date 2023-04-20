#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"
#include <QPainter>
#include <qpen.h>
#include <QMouseEvent>
#include <QHBoxLayout>
#include <QDragEnterEvent>
#include <QMimeData>
#include "ImageViewer.h"
//#include "CCalculate.h"
class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow();

private:
	void initWindow();
	void connentEvent();
private:
	Ui::MainWindowClass ui;
	ImageViewer *viewer;
};
