#include "RenderOptions.h"
#include "ui_RenderOptions.h"

#include "omp.h"

#include <manager/OptionsManager.h>
#include <manager/RenderManager.h>

#include <QFileDialog>

RenderOptions::RenderOptions(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::RenderOptions)
{
	ui->setupUi(this);

	ui->render_parameters_label->setStyleSheet("font-weight: bold");
	ui->camera_parameters_label->setStyleSheet("font-weight: bold");
	ui->ibl_label->setStyleSheet("font-weight: bold");

	QPixmap render_params_icon_pixmap(":/resource/render_params_icon.png");
	ui->render_params_icon->setPixmap(render_params_icon_pixmap.scaled(ui->render_params_icon->width(),
																	   ui->render_params_icon->height()));
	QPixmap camera_params_icon_pixmap(":/resource/camera_params_icon.png");
	ui->camera_params_icon->setPixmap(camera_params_icon_pixmap.scaled(ui->camera_params_icon->width(),
																	   ui->camera_params_icon->height()));
	QPixmap ibl_icon_pixmap(":/resource/illum_image_params.png");
	ui->ibl_icon->setPixmap(ibl_icon_pixmap.scaled(ui->ibl_icon->width(), ui->ibl_icon->height()));

	ui->threads->setValue(omp_get_max_threads());
	ui->threads->setMaximum(omp_get_max_threads());

	parameter_validator = new QDoubleValidator;
	parameter_validator->setNotation(QDoubleValidator::StandardNotation);
	parameter_validator->setLocale(QLocale(QLocale::C));
	ui->move_velocity->setText(QString::number(OptionsManager::get_manager()->getCamera_move_velocity()));
	ui->move_velocity->setValidator(parameter_validator);
	ui->zoom_velocity->setText(QString::number(OptionsManager::get_manager()->getCamera_zoom_velocity()));
	ui->zoom_velocity->setValidator(parameter_validator);
	ui->aperture->setText(QString::number(OptionsManager::get_manager()->getCamera_aperture()));
	ui->aperture->setValidator(parameter_validator);
	ui->focus_distance->setText(QString::number(OptionsManager::get_manager()->getCamera_focus_distance()));
	ui->focus_distance->setValidator(parameter_validator);

	color_validator = new QIntValidator(0, 255);
	Vector3 gradient_start_color = OptionsManager::get_manager()->get_gradient_start_color();
	ui->start_color_x->setText(QString::number(gradient_start_color.get_x()));
	ui->start_color_x->setValidator(color_validator);
	ui->start_color_y->setText(QString::number(gradient_start_color.get_y()));
	ui->start_color_y->setValidator(color_validator);
	ui->start_color_z->setText(QString::number(gradient_start_color.get_z()));
	ui->start_color_z->setValidator(color_validator);
	Vector3 gradient_end_color = OptionsManager::get_manager()->get_gradient_end_color();
	ui->end_color_x->setText(QString::number(gradient_end_color.get_x()));
	ui->end_color_x->setValidator(color_validator);
	ui->end_color_y->setText(QString::number(gradient_end_color.get_y()));
	ui->end_color_y->setValidator(color_validator);
	ui->end_color_z->setText(QString::number(gradient_end_color.get_z()));
	ui->end_color_z->setValidator(color_validator);

	ui->environment_view->setPixmap(QPixmap(":/resource/image_placeholder.png").scaled(ui->environment_view->width(),
																					   ui->environment_view->height()));
	QPixmap open_file_icon_pixmap(":/resource/open_file_icon.png");
	ui->open_environment->setIcon(QIcon(open_file_icon_pixmap));

	ui->render_progress->reset();
}

RenderOptions::~RenderOptions()
{
	delete ui;
}

void RenderOptions::update_render_progress(int samples_rendered)
{
	if(ui->render_progress->isTextVisible()) {
		ui->render_progress->setValue(samples_rendered);
	}
}

void RenderOptions::on_start_render_button_clicked()
{
	ui->start_render_button->setEnabled(false);
	ui->cancel_render_button->setEnabled(true);

	ui->render_progress->reset();
	ui->render_progress->setTextVisible(true);
	ui->render_progress->setMaximum(ui->width->text().toFloat() * ui->height->text().toFloat());

	emit start_render();
}

void RenderOptions::on_cancel_render_button_clicked()
{
	ui->start_render_button->setEnabled(true);
	ui->cancel_render_button->setEnabled(false);

	ui->render_progress->setTextVisible(false);
	ui->render_progress->reset();

	emit cancel_render();
}

void RenderOptions::line_edit_edited(QLineEdit* line_edit, const QString& text)
{
	QString input(text);
	int pos = 0;
	if(line_edit->validator()->validate(input, pos) == QValidator::State::Acceptable) {
		QPalette palette = line_edit->palette();
		palette.setColor(QPalette::Base, Qt::white);
		line_edit->setPalette(palette);
	}
	else {
		QPalette palette = line_edit->palette();
		palette.setColor(QPalette::Base, Qt::red);
		line_edit->setPalette(palette);
	}
}

void RenderOptions::on_move_velocity_editingFinished()
{
	OptionsManager::get_manager()->setCamera_move_velocity(ui->move_velocity->text().toFloat());
}

void RenderOptions::on_move_velocity_textEdited(const QString &arg1)
{
	line_edit_edited(ui->move_velocity, arg1);
}

void RenderOptions::on_zoom_velocity_editingFinished()
{
	OptionsManager::get_manager()->setCamera_zoom_velocity(ui->zoom_velocity->text().toFloat());
}

void RenderOptions::on_zoom_velocity_textEdited(const QString &arg1)
{
	line_edit_edited(ui->zoom_velocity, arg1);
}

void RenderOptions::on_aperture_editingFinished()
{
	OptionsManager::get_manager()->setCamera_aperture(ui->aperture->text().toFloat());
}

void RenderOptions::on_aperture_textEdited(const QString &arg1)
{
	line_edit_edited(ui->aperture, arg1);
}

void RenderOptions::on_focus_distance_editingFinished()
{
	OptionsManager::get_manager()->setCamera_focus_distance(ui->focus_distance->text().toFloat());
}

void RenderOptions::on_focus_distance_textEdited(const QString &arg1)
{
	line_edit_edited(ui->focus_distance, arg1);
}

void RenderOptions::on_start_color_x_editingFinished()
{
	OptionsManager::get_manager()->set_gradient_start_color(Vector3(ui->start_color_x->text().toFloat(),
																	ui->start_color_y->text().toFloat(),
																	ui->start_color_z->text().toFloat()));
}

void RenderOptions::on_start_color_x_textEdited(const QString &arg1)
{
	line_edit_edited(ui->start_color_x, arg1);
}

void RenderOptions::on_start_color_y_editingFinished()
{
	OptionsManager::get_manager()->set_gradient_start_color(Vector3(ui->start_color_x->text().toFloat(),
																	ui->start_color_y->text().toFloat(),
																	ui->start_color_z->text().toFloat()));
}

void RenderOptions::on_start_color_y_textEdited(const QString &arg1)
{
	line_edit_edited(ui->start_color_y, arg1);
}

void RenderOptions::on_start_color_z_editingFinished()
{
	OptionsManager::get_manager()->set_gradient_start_color(Vector3(ui->start_color_x->text().toFloat(),
																	ui->start_color_y->text().toFloat(),
																	ui->start_color_z->text().toFloat()));
}

void RenderOptions::on_start_color_z_textEdited(const QString &arg1)
{
	line_edit_edited(ui->start_color_z, arg1);
}

void RenderOptions::on_end_color_x_editingFinished()
{
	OptionsManager::get_manager()->set_gradient_end_color(Vector3(ui->end_color_x->text().toFloat(),
																  ui->end_color_y->text().toFloat(),
																  ui->end_color_z->text().toFloat()));
}

void RenderOptions::on_end_color_x_textEdited(const QString &arg1)
{
	line_edit_edited(ui->end_color_x, arg1);
}

void RenderOptions::on_end_color_y_editingFinished()
{
	OptionsManager::get_manager()->set_gradient_end_color(Vector3(ui->end_color_x->text().toFloat(),
																  ui->end_color_y->text().toFloat(),
																  ui->end_color_z->text().toFloat()));
}

void RenderOptions::on_end_color_y_textEdited(const QString &arg1)
{
	line_edit_edited(ui->end_color_y, arg1);
}

void RenderOptions::on_end_color_z_editingFinished()
{
	OptionsManager::get_manager()->set_gradient_end_color(Vector3(ui->end_color_x->text().toFloat(),
																  ui->end_color_y->text().toFloat(),
																  ui->end_color_z->text().toFloat()));
}

void RenderOptions::on_end_color_z_textEdited(const QString &arg1)
{
	line_edit_edited(ui->end_color_z, arg1);
}

void RenderOptions::on_gradient_toggled(bool checked)
{
	if(checked)
		OptionsManager::get_manager()->set_light_type(OptionsManager::IMAGE_BASED_LIGHT_TYPE::GRADIENT);
}

void RenderOptions::on_environment_map_toggled(bool checked)
{
	if(checked)
		OptionsManager::get_manager()->set_light_type(OptionsManager::IMAGE_BASED_LIGHT_TYPE::ENVIRONMENT_MAP);
}

void RenderOptions::on_open_environment_clicked()
{
	QString filename = QFileDialog::getOpenFileName(this,
											"Open Environment Map",
											"",
											"Image Files (*.png *.jpg *.bmp)");
	if(filename.isNull()) return;
	ui->environment_view->setPixmap(QPixmap::fromImage(QImage(filename).scaled(ui->environment_view->width(),
																			   ui->environment_view->height())));
	RenderManager::get_manager()->set_environment_map(filename);
}
