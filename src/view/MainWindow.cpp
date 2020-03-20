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

	QLayout* preview = ui->centralwidget->findChild<QLayout*>("preview_layout");
	viewport = new Viewport(this);
	preview->addWidget(viewport);
	viewport->setText("");
	viewport->setMouseTracking(true);
	viewport->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	connect(viewport, &Viewport::render, this, &MainWindow::render_preview);

	render_preview();
}

MainWindow::~MainWindow()
{
	delete viewport;
	delete ui;
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
	render_preview();
}

void MainWindow::render_preview()
{
	omp_set_num_threads(ui->centralwidget->findChild<QSpinBox*>("threads")->value());

	int width = viewport->geometry().width();
	int height = viewport->geometry().height();

	Framebuffer frame = RenderManager::get_manager()->render_preview(width, height);
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

void MainWindow::on_render_button_clicked()
{
	omp_set_num_threads(ui->centralwidget->findChild<QSpinBox*>("threads")->value());

	int width = ui->centralwidget->findChild<QSpinBox*>("width")->value();
	int height = ui->centralwidget->findChild<QSpinBox*>("height")->value();
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
	ui->centralwidget->findChild<QLabel*>("render_label")->setPixmap(QPixmap::fromImage(image));
}
