#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QPixmap>
#include <QLabel>

#include <fstream>

#include <manager/RenderManager.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	QLabel* label = ui->centralwidget->findChild<QLabel*>("label");

	label->setPixmap(RenderManager::get_manager()->render(label->width(),
														  label->height()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

