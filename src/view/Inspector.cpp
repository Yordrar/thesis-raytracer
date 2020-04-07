#include "Inspector.h"
#include "ui_Inspector.h"

#include <QPushButton>
#include <QPixmap>
#include <QFileDialog>

#include <manager/RenderManager.h>

Inspector::Inspector(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::Inspector)
{
	ui->setupUi(this);

	map_preview_size = ui->texture_view->width();

	QPixmap icon_pixmap(":/resource/open_file_icon.png");
	ui->open_texture->setIcon(QIcon(icon_pixmap));
	ui->open_normal->setIcon(QIcon(icon_pixmap));

	QImage default_image(map_preview_size, map_preview_size, QImage::Format_BGR888);
	default_image.fill(0);
	ui->texture_view->setPixmap(QPixmap::fromImage(default_image));
	ui->normal_view->setPixmap(QPixmap::fromImage(default_image));
}

Inspector::~Inspector()
{
	delete ui;
}

void Inspector::reload(Entity* entity)
{
	if(!RenderManager::get_manager()->is_there_entity_selected()) {
		ui->position_x->setText("");
		ui->position_y->setText("");
		ui->position_z->setText("");
		QImage default_image(map_preview_size, map_preview_size, QImage::Format_BGR888);
		default_image.fill(0);
		ui->texture_view->setPixmap(QPixmap::fromImage(default_image));
		ui->normal_view->setPixmap(QPixmap::fromImage(default_image));
	}
	else {
		Vector3 position = entity->get_position();
		ui->position_x->setText(QString::number(static_cast<double>(position.get_x())));
		ui->position_y->setText(QString::number(static_cast<double>(position.get_y())));
		ui->position_z->setText(QString::number(static_cast<double>(position.get_z())));
		ui->texture_view->setPixmap(QPixmap::fromImage(RenderManager::get_manager()->get_texture_map().scaled(map_preview_size, map_preview_size)));
		ui->normal_view->setPixmap(QPixmap::fromImage(RenderManager::get_manager()->get_normal_map().scaled(map_preview_size, map_preview_size)));
	}
}

void Inspector::on_open_texture_clicked()
{
	if(!RenderManager::get_manager()->is_there_entity_selected()) return;

	QString filename = QFileDialog::getOpenFileName(this,
											"Open Texture Map",
											"",
											"Image Files (*.png *.jpg *.bmp)");
	ui->texture_view->setPixmap(QPixmap::fromImage(QImage(filename).scaled(map_preview_size, map_preview_size)));
	RenderManager::get_manager()->set_texture_map(filename);
}

void Inspector::on_open_normal_clicked()
{
	if(!RenderManager::get_manager()->is_there_entity_selected()) return;

	QString filename = QFileDialog::getOpenFileName(this,
											"Open Normal Map",
											"",
											"Image Files (*.png *.jpg *.bmp)");
	ui->normal_view->setPixmap(QPixmap::fromImage(QImage(filename).scaled(map_preview_size, map_preview_size)));
	RenderManager::get_manager()->set_normal_map(filename);
}
