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

	QFont f("Arial", 10, QFont::Bold);
	ui->transform_label->setStyleSheet("font-weight: bold");
	ui->transform_label->setFont(f);
	ui->material_label->setStyleSheet("font-weight: bold");
	ui->material_label->setFont(f);
	ui->texture_label->setStyleSheet("font-weight: bold");
	ui->texture_label->setFont(f);
	ui->normal_label->setStyleSheet("font-weight: bold");
	ui->normal_label->setFont(f);

	QPixmap transform_icon_pixmap(":/resource/transform_icon.png");
	ui->transform_icon->setPixmap(transform_icon_pixmap.scaled(ui->transform_icon->width(), ui->transform_icon->height()));
	QPixmap material_icon_pixmap(":/resource/material_icon.png");
	ui->material_icon->setPixmap(material_icon_pixmap.scaled(ui->material_icon->width(), ui->material_icon->height()));
	QPixmap texture_map_icon_pixmap(":/resource/texture_map_icon.png");
	ui->texture_map_icon->setPixmap(texture_map_icon_pixmap.scaled(ui->texture_map_icon->width(), ui->texture_map_icon->height()));
	QPixmap normal_map_icon_pixmap(":/resource/normal_map_icon.png");
	ui->normal_map_icon->setPixmap(normal_map_icon_pixmap.scaled(ui->normal_map_icon->width(), ui->normal_map_icon->height()));

	QPixmap open_file_icon_pixmap(":/resource/open_file_icon.png");
	ui->open_texture->setIcon(QIcon(open_file_icon_pixmap));
	ui->open_normal->setIcon(QIcon(open_file_icon_pixmap));

	transform_validator = new QDoubleValidator;
	transform_validator->setNotation(QDoubleValidator::StandardNotation);
	ui->position_x->setValidator(transform_validator);
	ui->position_y->setValidator(transform_validator);
	ui->position_z->setValidator(transform_validator);
	ui->orientation_x->setValidator(transform_validator);
	ui->orientation_y->setValidator(transform_validator);
	ui->orientation_z->setValidator(transform_validator);

	material_parameter_validator = new QDoubleValidator(0, 1, 3);
	material_parameter_validator->setNotation(QDoubleValidator::StandardNotation);
	ui->roughness->setValidator(material_parameter_validator);
	ui->metallicity->setValidator(material_parameter_validator);
	ui->reflectance->setValidator(material_parameter_validator);
	ui->intensity->setValidator(transform_validator);
	ui->refraction->setValidator(transform_validator);

	albedo_validator = new QIntValidator(0, 255);
	ui->albedo_x->setValidator(albedo_validator);
	ui->albedo_y->setValidator(albedo_validator);
	ui->albedo_z->setValidator(albedo_validator);

	reload();
}

Inspector::~Inspector()
{
	delete transform_validator;
	delete material_parameter_validator;
	delete albedo_validator;
	delete ui;
}

void Inspector::reload()
{
	if(!RenderManager::get_manager()->is_there_entity_selected()) {
		ui->position_x->setEnabled(false);
		ui->position_x->setText("");
		ui->position_y->setEnabled(false);
		ui->position_y->setText("");
		ui->position_z->setEnabled(false);
		ui->position_z->setText("");
		ui->orientation_x->setEnabled(false);
		ui->orientation_x->setText("");
		ui->orientation_y->setEnabled(false);
		ui->orientation_y->setText("");
		ui->orientation_z->setEnabled(false);
		ui->orientation_z->setText("");

		ui->material_selector->setEnabled(false);
		ui->albedo_x->setEnabled(false);
		ui->albedo_x->setText("");
		ui->albedo_y->setEnabled(false);
		ui->albedo_y->setText("");
		ui->albedo_z->setEnabled(false);
		ui->albedo_z->setText("");
		ui->roughness->setEnabled(false);
		ui->roughness->setText("");
		ui->metallicity->setEnabled(false);
		ui->metallicity->setText("");
		ui->reflectance->setEnabled(false);
		ui->reflectance->setText("");
		ui->intensity->setEnabled(false);
		ui->intensity->setText("");
		ui->refraction->setEnabled(false);
		ui->refraction->setText("");

		QPixmap placeholder_image(":/resource/image_placeholder.png");
		ui->texture_view->setPixmap(placeholder_image.scaled(map_preview_size, map_preview_size));
		ui->normal_view->setPixmap(placeholder_image.scaled(map_preview_size, map_preview_size));
	}
	else {
		Entity* entity = RenderManager::get_manager()->get_entity_selected();

		ui->position_x->setEnabled(true);
		ui->position_y->setEnabled(true);
		ui->position_z->setEnabled(true);
		ui->orientation_x->setEnabled(true);
		ui->orientation_y->setEnabled(true);
		ui->orientation_z->setEnabled(true);

		Vector3 position = entity->get_position();
		ui->position_x->setText(QString::number(static_cast<double>(round(position.get_x()*1000)/1000.0f)));
		ui->position_y->setText(QString::number(static_cast<double>(round(position.get_y()*1000)/1000.0f)));
		ui->position_z->setText(QString::number(static_cast<double>(round(position.get_z()*1000)/1000.0f)));

		Vector3 orientation_euler = entity->get_rotation_euler();
		ui->orientation_x->setText(QString::number(static_cast<double>(orientation_euler.get_x())));
		ui->orientation_y->setText(QString::number(static_cast<double>(orientation_euler.get_y())));
		ui->orientation_z->setText(QString::number(static_cast<double>(orientation_euler.get_z())));

		auto material = RenderManager::get_manager()->get_material();
		if(material != RenderManager::MATERIAL_TYPE::NONE) {
			ui->material_selector->setEnabled(true);
			Vector3 albedo = RenderManager::get_manager()->get_albedo()*255;
			ui->albedo_x->setEnabled(true);
			ui->albedo_x->setText(QString::number(albedo.get_x()));
			ui->albedo_y->setEnabled(true);
			ui->albedo_y->setText(QString::number(albedo.get_y()));
			ui->albedo_z->setEnabled(true);
			ui->albedo_z->setText(QString::number(albedo.get_z()));
			ui->roughness->setEnabled(false);
			ui->roughness->setText("");
			ui->metallicity->setEnabled(false);
			ui->metallicity->setText("");
			ui->reflectance->setEnabled(false);
			ui->reflectance->setText("");
			ui->intensity->setEnabled(false);
			ui->intensity->setText("");
			ui->refraction->setEnabled(false);
			ui->refraction->setText("");

			switch(material) {
			case RenderManager::MATERIAL_TYPE::NONE:
				break;
			case RenderManager::MATERIAL_TYPE::LAMBERTIAN:
				ui->material_selector->setCurrentIndex(0);
				break;
			case RenderManager::MATERIAL_TYPE::BLINNPHONG:
				ui->material_selector->setCurrentIndex(1);
				ui->roughness->setEnabled(true);
				ui->roughness->setText(QString::number(RenderManager::get_manager()->get_roughness()));
				ui->reflectance->setEnabled(true);
				ui->reflectance->setText(QString::number(RenderManager::get_manager()->get_reflectance()));
				break;
			case RenderManager::MATERIAL_TYPE::METAL:
				ui->material_selector->setCurrentIndex(2);
				ui->roughness->setEnabled(true);
				ui->roughness->setText(QString::number(RenderManager::get_manager()->get_roughness()));
				ui->metallicity->setEnabled(true);
				ui->metallicity->setText(QString::number(RenderManager::get_manager()->get_metallicity()));
				break;
			case RenderManager::MATERIAL_TYPE::DIELECTRIC:
				ui->material_selector->setCurrentIndex(3);
				ui->roughness->setEnabled(true);
				ui->roughness->setText(QString::number(RenderManager::get_manager()->get_roughness()));
				ui->refraction->setEnabled(true);
				ui->refraction->setText(QString::number(RenderManager::get_manager()->get_refraction_index()));
				break;
			case RenderManager::MATERIAL_TYPE::EMISSIVE:
				ui->material_selector->setCurrentIndex(4);
				ui->intensity->setEnabled(true);
				ui->intensity->setText(QString::number(RenderManager::get_manager()->get_intensity()));
				break;
			case RenderManager::MATERIAL_TYPE::REFRACTIVE_LAMBERTIAN:
				ui->material_selector->setCurrentIndex(5);
				break;
			}
		}
		else {
			ui->material_selector->setEnabled(false);
			ui->albedo_x->setEnabled(false);
			ui->albedo_x->setText("");
			ui->albedo_y->setEnabled(false);
			ui->albedo_y->setText("");
			ui->albedo_z->setEnabled(false);
			ui->albedo_z->setText("");
			ui->roughness->setEnabled(false);
			ui->roughness->setText("");
			ui->metallicity->setEnabled(false);
			ui->metallicity->setText("");
			ui->reflectance->setEnabled(false);
			ui->reflectance->setText("");
			// This only handles emissive materials
			// If intensity is added to emitters this needs to check
			// whether or not the entity selected is an Emitter
			ui->intensity->setEnabled(false);
			ui->intensity->setText("");
			ui->refraction->setEnabled(false);
			ui->refraction->setText("");
		}

		Image* texture_map = RenderManager::get_manager()->get_texture_map();
		if(texture_map) {
			ui->texture_view->setPixmap(QPixmap::fromImage(image_to_qimage(texture_map).scaled(map_preview_size, map_preview_size)));
		}
		else {
			QPixmap placeholder_image(":/resource/image_placeholder.png");
			ui->texture_view->setPixmap(placeholder_image.scaled(map_preview_size, map_preview_size));
		}

		Image* normal_map = RenderManager::get_manager()->get_normal_map();
		if(normal_map) {
			ui->normal_view->setPixmap(QPixmap::fromImage(image_to_qimage(normal_map).scaled(map_preview_size, map_preview_size)));
		}
		else {
			QPixmap placeholder_image(":/resource/image_placeholder.png");
			ui->normal_view->setPixmap(placeholder_image.scaled(map_preview_size, map_preview_size));
		}
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
	case 4:
		RenderManager::get_manager()->set_material(RenderManager::MATERIAL_TYPE::EMISSIVE);
		break;
	case 5:
		RenderManager::get_manager()->set_material(RenderManager::MATERIAL_TYPE::REFRACTIVE_LAMBERTIAN);
		break;
	}

	reload();
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
	if(line_edit->validator()->validate(input, cero) == QValidator::State::Acceptable) {
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

QImage Inspector::image_to_qimage(Image* img) const
{
	if(img == nullptr) {
		QImage qimg(1, 1, QImage::Format_RGB888);
		qimg.fill(0);
		return qimg;
	}

	QImage qimg(img->get_width(), img->get_height(), QImage::Format_RGB888);
	for(int j = 0; j < qimg.height(); j++) {
		for(int i = 0; i < qimg.width(); i++) {
			Vector3 color = img->get_pixel_color(i, j) * 255;
			qimg.setPixelColor(i, j, QColor(static_cast<int>(color.get_x()),
											static_cast<int>(color.get_y()),
											static_cast<int>(color.get_z())));
		}
	}
	return qimg;
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

void Inspector::on_roughness_editingFinished()
{
	RenderManager::get_manager()->set_roughness(ui->roughness->text().toFloat());
}

void Inspector::on_roughness_textEdited(const QString &arg1)
{
	line_edit_edited(ui->roughness, arg1);
}

void Inspector::on_metallicity_editingFinished()
{
	RenderManager::get_manager()->set_metallicity(ui->metallicity->text().toFloat());
}

void Inspector::on_metallicity_textEdited(const QString &arg1)
{
	line_edit_edited(ui->metallicity, arg1);
}

void Inspector::on_reflectance_editingFinished()
{
	RenderManager::get_manager()->set_reflectance(ui->reflectance->text().toFloat());
}

void Inspector::on_reflectance_textEdited(const QString &arg1)
{
	line_edit_edited(ui->reflectance, arg1);
}

void Inspector::on_albedo_x_editingFinished()
{
	RenderManager::get_manager()->set_albedo(ui->albedo_x->text().toFloat(),
											 ui->albedo_y->text().toFloat(),
											 ui->albedo_z->text().toFloat());
}

void Inspector::on_albedo_x_textEdited(const QString &arg1)
{
	line_edit_edited(ui->albedo_x, arg1);
}

void Inspector::on_albedo_y_editingFinished()
{
	RenderManager::get_manager()->set_albedo(ui->albedo_x->text().toFloat(),
											 ui->albedo_y->text().toFloat(),
											 ui->albedo_z->text().toFloat());
}

void Inspector::on_albedo_y_textEdited(const QString &arg1)
{
	line_edit_edited(ui->albedo_y, arg1);
}

void Inspector::on_albedo_z_editingFinished()
{
	RenderManager::get_manager()->set_albedo(ui->albedo_x->text().toFloat(),
											 ui->albedo_y->text().toFloat(),
											 ui->albedo_z->text().toFloat());
}

void Inspector::on_albedo_z_textEdited(const QString &arg1)
{
	line_edit_edited(ui->albedo_z, arg1);
}

void Inspector::on_intensity_editingFinished()
{
	RenderManager::get_manager()->set_intensity(ui->intensity->text().toFloat());
}

void Inspector::on_intensity_textEdited(const QString &arg1)
{
	line_edit_edited(ui->intensity, arg1);
}

void Inspector::on_refraction_editingFinished()
{
	RenderManager::get_manager()->set_refraction_index(ui->refraction->text().toFloat());
}

void Inspector::on_refraction_textEdited(const QString &arg1)
{
	line_edit_edited(ui->intensity, arg1);
}
