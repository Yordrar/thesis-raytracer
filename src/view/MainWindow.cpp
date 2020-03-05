#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QPixmap>
#include <QLabel>
#include <QSpinBox>

#include <fstream>

#include <manager/RenderManager.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
	ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_render_button_clicked()
{
	int width = ui->centralwidget->findChild<QSpinBox*>("width")->value();
	int height = ui->centralwidget->findChild<QSpinBox*>("height")->value();
	int n_samples = ui->centralwidget->findChild<QSpinBox*>("samples")->value();

	QLabel* label = ui->centralwidget->findChild<QLabel*>("image");
	label->setGeometry(label->geometry().x(), label->geometry().y(), width, height);
	label->setPixmap(RenderManager::get_manager()->render(width,
														  height,
														  n_samples));
}
