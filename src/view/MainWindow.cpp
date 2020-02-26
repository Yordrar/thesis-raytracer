#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QPixmap>
#include <QLabel>

#include <fstream>

#include <math.h>
#include <math/Vector3.h>
#include <geometry/Sphere.h>

#define PI 3.14159265358979323846f

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	QLabel* label = ui->centralwidget->findChild<QLabel*>("label");

	float width = label->geometry().width();
	float height = label->geometry().height();
	float fovx = PI / 4.0f;
	float fovy = (height/width)*fovx;

	Sphere s1(Vector3(0, 0, -1), 0.5f);
	Sphere s2(Vector3(0, -100-s1.get_radius(), -1), 100);
	std::fstream image("image.ppm", std::fstream::out | std::fstream::trunc);
	image << "P3\n" << width << " " << height << "\n255\n";
	for(int j = 0; j < height; j++) {
		float camara_y = (2.0f*j-height)/height * tanf(fovy);
		for(int i = 0; i < width; i++) {
			float camara_x = (2.0f*i-width)/width * tanf(fovx);
			Ray r(Vector3(0, 0, 0), Vector3(camara_x, -camara_y, -1));
			Vector3 color;
			float t_minimo = FLT_MAX;
			Sphere s_intersectada;
			float t = s1.intersects(r);
			if(t > 0.0f && t < t_minimo) {
				t_minimo = t;
				s_intersectada = s1;
			}
			t = s2.intersects(r);
			if(t > 0.0f && t < t_minimo) {
				t_minimo = t;
				s_intersectada = s2;
			}
			if(t_minimo < FLT_MAX) {
				Vector3 normal = s_intersectada.get_normal(r.get_origin() + r.get_direction()*t_minimo);
				normal = (normal+Vector3(1.0f, 1.0f, 1.0f))*0.5;
				color = Vector3(normal.get_x(), normal.get_y(), normal.get_z());
			}
			image << int(color.get_x()*255.99f) << " " << int(color.get_y()*255.99f) << " " << int(color.get_z()*255.99f) << "\n";
		}
	}
	image.close();
	QPixmap pix("image.ppm");
	label->setPixmap(pix);
}

MainWindow::~MainWindow()
{
    delete ui;
}

