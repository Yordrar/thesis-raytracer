#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QPixmap>
#include <QLabel>
#include <QSpinBox>
#include <QScrollArea>
#include <QTimer>
#include <QFileDialog>
#include <QMessageBox>
#include <QTabWidget>
#include <QWheelEvent>
#include <QDebug>

#include <omp.h>

#include <manager/RenderManager.h>
#include <manager/SceneManager.h>
#include <image/Image.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	ui->centralwidget->findChild<QSpinBox*>("threads")->setValue(omp_get_max_threads());
	ui->centralwidget->findChild<QSpinBox*>("threads")->setMaximum(omp_get_max_threads());

	ui->centralwidget->findChild<QScrollArea*>("scrollArea")->installEventFilter(this);
	ui->centralwidget->findChild<QLabel*>("render_label")->installEventFilter(this);

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

	connect(viewport, &Viewport::render_preview, this, &MainWindow::render_preview);
	connect(inspector, &Inspector::render_preview, this, &MainWindow::render_preview);
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

bool MainWindow::eventFilter(QObject* obj, QEvent* event)
{
	if (event->type() == QEvent::Wheel) {
		if(ui->centralwidget->findChild<QTabWidget*>("tabWidget")->currentIndex() == 1 && is_ctrl_pressed) {
			auto* label = ui->centralwidget->findChild<QLabel*>("render_label");
			if(!label->pixmap()->isNull()) {
				if(static_cast<QWheelEvent*>(event)->angleDelta().y() > 0) {
					render_viewport_scale += 0.01f;
				}
				else {
					render_viewport_scale -= 0.01f;
				}
				update_render_viewport();
				event->accept();
				return true;
			}
		}
	}
	else if(event->type() == QEvent::KeyPress) {
		if(static_cast<QKeyEvent*>(event)->key() == Qt::Key::Key_Control) {
			is_ctrl_pressed = true;

			event->accept();
			return true;
		}
	}
	else if(event->type() == QEvent::KeyRelease) {
		if(static_cast<QKeyEvent*>(event)->key() == Qt::Key::Key_Control) {
			is_ctrl_pressed = false;

			event->accept();
			return true;
		}
	}
	event->ignore();
	return false;
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
	render_viewport_scale = 1;

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
		image = image.scaled(image.width()*render_viewport_scale, image.height()*render_viewport_scale);
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

	update_render_viewport();
}

void MainWindow::on_actionNew_Scene_triggered()
{
	SceneManager::get_manager()->clear_scene();
	render_preview();
}

void MainWindow::on_actionOpen_Scene_triggered()
{
	QString filename = QFileDialog::getOpenFileName(this,
											"Open Scene File",
											"",
											"Scene and Mesh Files (*.obj *.fbx *.ply)");
	if(filename.isNull()) return;
	SceneManager::get_manager()->load_from_file(filename.toStdString());
	render_preview();
}

void MainWindow::on_actionAdd_from_File_triggered()
{
	QString filename = QFileDialog::getOpenFileName(this,
											"Open Scene File",
											"",
											"Scene and Mesh Files (*.obj *.fbx *.ply)");
	if(filename.isNull()) return;
	SceneManager::get_manager()->add_from_file(filename.toStdString());
	render_preview();
}

void MainWindow::on_actionExit_triggered()
{
	close();
}

void MainWindow::on_actionRender_Image_triggered()
{
	ui->centralwidget->findChild<QTabWidget*>("tabWidget")->setCurrentIndex(1);
	on_start_render_button_clicked();
}

void MainWindow::on_actionSave_Render_triggered()
{
	auto* render_pixmap = ui->centralwidget->findChild<QLabel*>("render_label")->pixmap();
	if(!render_pixmap) {
		QMessageBox::critical(this,
							 tr("Error"),
							 tr("You need to render an image before saving it."));
		return;
	}

	QString filename = QFileDialog::getSaveFileName(this,
													"Save Render",
													"untitled.png",
													"Portable Network Graphics (*.png);;"
													"Joint Photographic Experts Group (*.jpeg *.jpg);;"
													"Window Bitmap (*.bmp);;"
													"WebP (*.webp);;"
													"Portable Pixmap (*.ppm);;"
													"Portable Bitmap (*.pbm);;"
													"X11 Bitmap (*.xbm);;"
													"X11 Pixmap (*.xpm);;"
													"Tagged Image File Format (*.tiff)");
	if(filename.isNull()) return;

	bool successful = render_pixmap->save(filename);
	if(!successful) {
		QMessageBox::critical(this,
							 tr("Error"),
							 tr("There was an error while saving the render."));
	}
	else {
		QMessageBox::information(this,
								 tr("Success"),
								 tr("Render saved successfully."));
	}
}
