#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QPixmap>
#include <QLabel>
#include <QSpinBox>
#include <QScrollArea>

#include <omp.h>

#include <manager/RenderManager.h>
#include <image/Image.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	inspector = new Inspector();
	ui->centralwidget->findChild<QScrollArea*>("inspector")->setWidget(inspector);

	QLayout* preview = ui->centralwidget->findChild<QLayout*>("preview_layout");
	viewport = new Viewport(this);
	preview->addWidget(viewport);
	viewport->setText("");
	viewport->setMouseTracking(true);
	viewport->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	connect(viewport, &Viewport::render, this, &MainWindow::render_preview);
	connect(viewport, &Viewport::entity_selected_changed, inspector, &Inspector::reload);

	render_preview();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
	QMainWindow::resizeEvent(event);
	render_preview();
}

void MainWindow::render_preview()
{
	omp_set_num_threads(ui->centralwidget->findChild<QSpinBox*>("threads")->value());

	int width = this->size().width()-280;
	int height = this->size().height()-50;

	Image frame = RenderManager::get_manager()->render_preview(width, height);
	QImage image(width, height, QImage::Format_RGB888);
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			Vector3 color = frame.get_pixel_color(i, j) * 255.99f;
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

	Image frame = RenderManager::get_manager()->render(width, height, n_samples);
	QImage image(width, height, QImage::Format_RGB888);
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			Vector3 color = frame.get_pixel_color(i, j) * 255.99f;
			image.setPixelColor(i, j, QColor(static_cast<int>(color.get_x()),
											 static_cast<int>(color.get_y()),
											 static_cast<int>(color.get_z())));
		}
	}
	ui->centralwidget->findChild<QLabel*>("render_label")->setPixmap(QPixmap::fromImage(image));
}
