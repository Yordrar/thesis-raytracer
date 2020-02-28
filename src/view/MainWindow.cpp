#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QPixmap>
#include <QLabel>

#include <fstream>

#include <math.h>
#include <math/Vector3.h>
#include <geometry/Sphere.h>
#include <geometry/Scene.h>
#include <geometry/Camera.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	QLabel* label = ui->centralwidget->findChild<QLabel*>("label");

	Sphere s1(Vector3(0, 0, -1.0f), 0.5f);
	Sphere s2(Vector3(0, -100-s1.get_radius(), -1), 100);
	Camera c(Vector3(), Vector3(0, 0, -1), label->geometry().width(), label->geometry().height());
	Scene escena{&s1, &s2};
	escena.set_camera(c);

	label->setPixmap(escena.render());
}

MainWindow::~MainWindow()
{
    delete ui;
}

