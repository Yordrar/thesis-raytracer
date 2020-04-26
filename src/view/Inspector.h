#pragma once

#include <QDoubleValidator>
#include <QWidget>
#include <QLineEdit>

#include <geometry/Entity.h>

#include <image/Image.h>

namespace Ui {
class Inspector;
}

class Inspector : public QWidget
{
	Q_OBJECT

public:
	explicit Inspector(QWidget *parent = nullptr);
	~Inspector();

signals:
	void render_preview();

public slots:
	void reload();

private slots:
	void on_open_texture_clicked();

	void on_open_normal_clicked();

	void on_material_selector_currentIndexChanged(int index);

	void on_orientation_x_editingFinished();
	void on_orientation_x_textEdited(const QString &arg1);

	void on_orientation_y_editingFinished();

	void on_orientation_y_textEdited(const QString &arg1);

	void on_orientation_z_editingFinished();

	void on_orientation_z_textEdited(const QString &arg1);

	void on_position_x_editingFinished();

	void on_position_x_textEdited(const QString &arg1);

	void on_position_y_editingFinished();

	void on_position_y_textEdited(const QString &arg1);

	void on_position_z_editingFinished();

	void on_position_z_textEdited(const QString &arg1);

private:
	Ui::Inspector *ui;
	unsigned int map_preview_size;
	QDoubleValidator* number_validator;

	void position_changed(float x, float y, float z);
	void orientation_changed(float euler_x, float euler_y, float euler_z);
	void line_edit_edited(QLineEdit* line_edit, const QString& text);
};

