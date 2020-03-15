#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QPixmap>
#include <QLabel>
#include <QSpinBox>

#include <omp.h>

#include <manager/RenderManager.h>
#include <image/Framebuffer.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	viewport = new Viewport(this);
	viewport->setText("");
	viewport->setMouseTracking(true);
}

MainWindow::~MainWindow()
{
	delete viewport;
    delete ui;
}


void MainWindow::on_render_button_clicked()
{
	omp_set_num_threads(ui->centralwidget->findChild<QSpinBox*>("threads")->value());

	int width = ui->centralwidget->findChild<QSpinBox*>("width")->value();
	int height = ui->centralwidget->findChild<QSpinBox*>("height")->value();
	int n_samples = ui->centralwidget->findChild<QSpinBox*>("samples")->value();

	QLabel* label = viewport;//ui->centralwidget->findChild<QLabel*>("image");
	label->setGeometry(label->geometry().x(), label->geometry().y(), width, height);
	Framebuffer frame = RenderManager::get_manager()->render(width,
															 height,
															 n_samples);
	QImage image(width, height, QImage::Format_RGB888);
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			Vector3 color = frame.get_pixel_color(i, j);
			image.setPixelColor(i, j, QColor(static_cast<int>(color.get_x()),
											 static_cast<int>(color.get_y()),
											 static_cast<int>(color.get_z())));
		}
	}
	label->setPixmap(QPixmap::fromImage(image));
}
