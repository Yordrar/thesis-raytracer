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

	number_validator = new QDoubleValidator;
	ui->position_x->setValidator(number_validator);
	ui->position_y->setValidator(number_validator);
	ui->position_z->setValidator(number_validator);
	ui->orientation_x->setValidator(number_validator);
	ui->orientation_y->setValidator(number_validator);
	ui->orientation_z->setValidator(number_validator);

	reload();
}

Inspector::~Inspector()
{
	delete ui;
}

void Inspector::reload()
{
	if(!RenderManager::get_manager()->is_there_entity_selected()) {
		ui->position_x->setText("");
		ui->position_y->setText("");
		ui->position_z->setText("");
		ui->orientation_x->setText("");
		ui->orientation_y->setText("");
		ui->orientation_z->setText("");

		ui->position_x->setEnabled(false);
		ui->position_y->setEnabled(false);
		ui->position_z->setEnabled(false);
		ui->orientation_x->setEnabled(false);
		ui->orientation_y->setEnabled(false);
		ui->orientation_z->setEnabled(false);

		ui->material_selector->setEnabled(false);

		QImage default_image(map_preview_size, map_preview_size, QImage::Format_BGR888);
		default_image.fill(0);
		ui->texture_view->setPixmap(QPixmap::fromImage(default_image));
		ui->normal_view->setPixmap(QPixmap::fromImage(default_image));
	}
	else {
		Entity* entity = RenderManager::get_manager()->get_entity_selected();

		ui->position_x->setEnabled(true);
		ui->position_y->setEnabled(true);
		ui->position_z->setEnabled(true);
		ui->orientation_x->setEnabled(true);
		ui->orientation_y->setEnabled(true);
		ui->orientation_z->setEnabled(true);

		ui->material_selector->setEnabled(true);

		Vector3 position = entity->get_position();
		ui->position_x->setText(QString::number(static_cast<double>(round(position.get_x()*1000)/1000.0f)));
		ui->position_y->setText(QString::number(static_cast<double>(round(position.get_y()*1000)/1000.0f)));
		ui->position_z->setText(QString::number(static_cast<double>(round(position.get_z()*1000)/1000.0f)));

		Vector3 orientation_euler = entity->get_rotation_euler();
		ui->orientation_x->setText(QString::number(static_cast<double>(orientation_euler.get_x())));
		ui->orientation_y->setText(QString::number(static_cast<double>(orientation_euler.get_y())));
		ui->orientation_z->setText(QString::number(static_cast<double>(orientation_euler.get_z())));

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
	if(filename.isNull()) return;
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
	if(filename.isNull()) return;
	ui->normal_view->setPixmap(QPixmap::fromImage(QImage(filename).scaled(map_preview_size, map_preview_size)));
	RenderManager::get_manager()->set_normal_map(filename);
}

void Inspector::on_material_selector_currentIndexChanged(int index)
{
	if(!RenderManager::get_manager()->is_there_entity_selected()) return;

	switch(index) {
	case 0:
		RenderManager::get_manager()->set_material(RenderManager::MATERIAL_TYPE::LAMBERTIAN);
		break;
	case 1:
		RenderManager::get_manager()->set_material(RenderManager::MATERIAL_TYPE::BLINNPHONG);
		break;
	case 2:
		RenderManager::get_manager()->set_material(RenderManager::MATERIAL_TYPE::METAL);
		break;
	case 3:
		RenderManager::get_manager()->set_material(RenderManager::MATERIAL_TYPE::DIELECTRIC);
		break;
	}
}


void Inspector::position_changed(float x, float y, float z)
{
	bool position_changed = RenderManager::get_manager()->set_position_entity_selected(x, y, z);
	if(position_changed) emit render_preview();
}

void Inspector::orientation_changed(float euler_x, float euler_y, float euler_z) {
	bool orientation_changed = RenderManager::get_manager()->set_orientation_entity_selected(euler_x,
																							 euler_y,
																							 euler_z);
	if(orientation_changed) emit render_preview();
}

int cero = 0;
void Inspector::line_edit_edited(QLineEdit* line_edit, const QString& text)
{
	QString input(text);
	if(number_validator->validate(input, cero) == QValidator::State::Acceptable) {
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

void Inspector::on_orientation_x_editingFinished()
{
	orientation_changed(ui->orientation_x->text().toFloat(),
						ui->orientation_y->text().toFloat(),
						ui->orientation_z->text().toFloat());
}

void Inspector::on_orientation_x_textEdited(const QString& arg1)
{
	line_edit_edited(ui->orientation_x, arg1);
}

void Inspector::on_orientation_y_editingFinished()
{
	orientation_changed(ui->orientation_x->text().toFloat(),
						ui->orientation_y->text().toFloat(),
						ui->orientation_z->text().toFloat());
}

void Inspector::on_orientation_y_textEdited(const QString &arg1)
{
	line_edit_edited(ui->orientation_y, arg1);
}

void Inspector::on_orientation_z_editingFinished()
{
	orientation_changed(ui->orientation_x->text().toFloat(),
						ui->orientation_y->text().toFloat(),
						ui->orientation_z->text().toFloat());
}

void Inspector::on_orientation_z_textEdited(const QString &arg1)
{
	line_edit_edited(ui->orientation_z, arg1);
}

void Inspector::on_position_x_editingFinished()
{
	position_changed(ui->position_x->text().toFloat(),
					 ui->position_y->text().toFloat(),
					 ui->position_z->text().toFloat());
}

void Inspector::on_position_x_textEdited(const QString &arg1)
{
	line_edit_edited(ui->position_x, arg1);
}


void Inspector::on_position_y_editingFinished()
{
	position_changed(ui->position_x->text().toFloat(),
					 ui->position_y->text().toFloat(),
					 ui->position_z->text().toFloat());
}

void Inspector::on_position_y_textEdited(const QString &arg1)
{
	line_edit_edited(ui->position_y, arg1);
}

void Inspector::on_position_z_editingFinished()
{
	position_changed(ui->position_x->text().toFloat(),
					 ui->position_y->text().toFloat(),
					 ui->position_z->text().toFloat());
}

void Inspector::on_position_z_textEdited(const QString &arg1)
{
	line_edit_edited(ui->position_z, arg1);
}
