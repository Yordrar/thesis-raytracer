#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QPixmap>
#include <QLabel>
#include <QSpinBox>
#include <QGridLayout>

#include <omp.h>

#include <manager/RenderManager.h>
#include <image/Framebuffer.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	QLayout* layout = ui->centralwidget->findChild<QLayout*>("preview_layout");
	viewport = new Viewport(this);
	layout->addWidget(viewport);
	viewport->setText("");
	viewport->setMouseTracking(true);
	viewport->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	viewport->setGeometry(viewport->geometry().x(),
						  viewport->geometry().y()+ui->menubar->height(),
						  800,
						  600);
	connect(viewport, &Viewport::render, this, &MainWindow::on_render_button_clicked);
	viewport->updateGeometry();

	on_render_button_clicked();
}

MainWindow::~MainWindow()
{
	delete viewport;
    delete ui;
}

void MainWindow::on_render_button_clicked()
{
	omp_set_num_threads(ui->centralwidget->findChild<QSpinBox*>("threads")->value());

	QLayout* layout = ui->centralwidget->findChild<QLayout*>("preview_layout");
	int width = viewport->geometry().width();//ui->centralwidget->findChild<QSpinBox*>("width")->value();
	int height = viewport->geometry().height();//ui->centralwidget->findChild<QSpinBox*>("height")->value();
	int n_samples = ui->centralwidget->findChild<QSpinBox*>("samples")->value();

	Framebuffer frame = RenderManager::get_manager()->render(width, height, n_samples);
	QImage image(width, height, QImage::Format_RGB888);
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			Vector3 color = frame.get_pixel_color(i, j);
			image.setPixelColor(i, j, QColor(static_cast<int>(color.get_x()),
											 static_cast<int>(color.get_y()),
											 static_cast<int>(color.get_z())));
		}
	}
	viewport->setPixmap(QPixmap::fromImage(image));
}
