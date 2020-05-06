#include "RenderOptions.h"
#include "ui_RenderOptions.h"

#include <omp.h>

RenderOptions::RenderOptions(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::RenderOptions)
{
	ui->setupUi(this);

	ui->threads->setValue(omp_get_max_threads());
	ui->threads->setMaximum(omp_get_max_threads());
}

RenderOptions::~RenderOptions()
{
	delete ui;
}

void RenderOptions::on_start_render_button_clicked()
{
	ui->start_render_button->setEnabled(false);
	ui->cancel_render_button->setEnabled(true);
	emit start_render();
}

void RenderOptions::on_cancel_render_button_clicked()
{
	ui->start_render_button->setEnabled(true);
	ui->cancel_render_button->setEnabled(false);
	emit cancel_render();
}
