#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QPixmap>
#include <QLabel>
#include <QSpinBox>
#include <QScrollArea>
#include <QTimer>

#include <omp.h>

#include <manager/RenderManager.h>
#include <image/Image.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	ui->centralwidget->findChild<QSpinBox*>("threads")->setValue(omp_get_max_threads());
	ui->centralwidget->findChild<QSpinBox*>("threads")->setMaximum(omp_get_max_threads());

	render_timer = new QTimer(this);
	render_timer->setInterval(500);
	render_timer->setSingleShot(false);
	connect(render_timer, &QTimer::timeout, this, &MainWindow::update_render_viewport);

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
	if(render_timer->isActive()) render_timer->stop();
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
	QMainWindow::resizeEvent(event);
	render_preview();
}

void MainWindow::render_preview()
{
	omp_set_num_threads(ui->centralwidget->findChild<QSpinBox*>("threads")->maximum());

	int width = this->size().width()-280;
	int height = this->size().height()-50;

	Image frame = RenderManager::get_manager()->render_preview(width, height);
	QImage image(width, height, QImage::Format_RGB888);
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			Vector3 color = frame.get_pixel_color(i, j) * 255;
			image.setPixelColor(i, j, QColor(static_cast<int>(color.get_x()),
											 static_cast<int>(color.get_y()),
											 static_cast<int>(color.get_z())));
		}
	}
	viewport->setPixmap(QPixmap::fromImage(image));
}


void render_worker_entry(int width, int height, int n_samples, int num_threads) {
	omp_set_num_threads(num_threads);

	RenderManager::get_manager()->render(width, height, n_samples);
}
void MainWindow::on_start_render_button_clicked()
{
	int width = ui->centralwidget->findChild<QSpinBox*>("width")->value();
	int height = ui->centralwidget->findChild<QSpinBox*>("height")->value();
	int n_samples = ui->centralwidget->findChild<QSpinBox*>("samples")->value();

	ui->centralwidget->findChild<QPushButton*>("start_render_button")->setEnabled(false);
	ui->centralwidget->findChild<QPushButton*>("cancel_render_button")->setEnabled(true);

	render_timer->start();
	if(render_worker) {
		RenderManager::get_manager()->finish_render();
		render_worker->join();
		delete render_worker;
	}
	render_worker = new std::thread(render_worker_entry, width, height, n_samples, ui->centralwidget->findChild<QSpinBox*>("threads")->value());
}

void MainWindow::update_render_viewport()
{
	Image* frame = RenderManager::get_manager()->get_rendered_image();
	if(frame == nullptr) {
		QImage image(0, 0, QImage::Format_RGB888);
		ui->centralwidget->findChild<QLabel*>("render_label")->setPixmap(QPixmap::fromImage(image));
	}
	else {
		int width = frame->get_width();
		int height = frame->get_height();
		QImage image(width, height, QImage::Format_RGB888);
		for (int j = 0; j < height; j++) {
			for (int i = 0; i < width; i++) {
				Vector3 color = frame->get_pixel_color(i, j) * 255;
				image.setPixelColor(i, j, QColor(static_cast<int>(color.get_x()),
												 static_cast<int>(color.get_y()),
												 static_cast<int>(color.get_z())));
			}
		}
		ui->centralwidget->findChild<QLabel*>("render_label")->setPixmap(QPixmap::fromImage(image));
	}

	if(RenderManager::get_manager()->is_render_finished()) {
		ui->centralwidget->findChild<QPushButton*>("start_render_button")->setEnabled(true);
		ui->centralwidget->findChild<QPushButton*>("cancel_render_button")->setEnabled(false);
		render_timer->stop();
	}
}

void MainWindow::on_cancel_render_button_clicked()
{
	render_timer->stop();

	RenderManager::get_manager()->finish_render();
	render_worker->join();
	delete render_worker;
	render_worker = nullptr;

	ui->centralwidget->findChild<QPushButton*>("start_render_button")->setEnabled(true);
	ui->centralwidget->findChild<QPushButton*>("cancel_render_button")->setEnabled(false);
}
