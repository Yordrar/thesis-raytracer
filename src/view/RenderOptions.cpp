#include "RenderOptions.h"
#include "ui_RenderOptions.h"

#include <omp.h>

RenderOptions::RenderOptions(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::RenderOptions)
{
	ui->setupUi(this);

	ui->render_parameters_label->setStyleSheet("font-weight: bold");
	ui->camera_parameters_label->setStyleSheet("font-weight: bold");
	ui->ibl_label->setStyleSheet("font-weight: bold");

	QPixmap render_params_icon_pixmap(":/resource/render_params_icon.png");
	ui->render_params_icon->setPixmap(render_params_icon_pixmap.scaled(ui->render_params_icon->width(), ui->render_params_icon->height()));
	QPixmap camera_params_icon_pixmap(":/resource/camera_params_icon.png");
	ui->camera_params_icon->setPixmap(camera_params_icon_pixmap.scaled(ui->camera_params_icon->width(), ui->camera_params_icon->height()));
	QPixmap ibl_icon_pixmap(":/resource/illum_image_params.png");
	ui->ibl_icon->setPixmap(ibl_icon_pixmap.scaled(ui->ibl_icon->width(), ui->ibl_icon->height()));

	ui->threads->setValue(omp_get_max_threads());
	ui->threads->setMaximum(omp_get_max_threads());

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
