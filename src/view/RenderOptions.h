#pragma once

#include <QDialog>
#include <QDoubleValidator>
#include <QLineEdit>

namespace Ui {
class RenderOptions;
}

class RenderOptions : public QWidget
{
	Q_OBJECT

public:
	explicit RenderOptions(QWidget *parent = nullptr);
	~RenderOptions();

signals:
	void start_render();
	void cancel_render();

public slots:
	void update_render_progress(int samples_rendered);

private slots:
	void on_start_render_button_clicked();

	void on_cancel_render_button_clicked();

	void on_move_velocity_editingFinished();

	void on_move_velocity_textEdited(const QString &arg1);

	void on_zoom_velocity_editingFinished();

	void on_zoom_velocity_textEdited(const QString &arg1);

	void on_aperture_editingFinished();

	void on_aperture_textEdited(const QString &arg1);

	void on_focus_distance_editingFinished();

	void on_focus_distance_textEdited(const QString &arg1);

	void on_start_color_x_editingFinished();

	void on_start_color_x_textEdited(const QString &arg1);

	void on_start_color_y_editingFinished();

	void on_start_color_y_textEdited(const QString &arg1);

	void on_start_color_z_editingFinished();

	void on_start_color_z_textEdited(const QString &arg1);

	void on_end_color_x_editingFinished();

	void on_end_color_x_textEdited(const QString &arg1);

	void on_end_color_y_editingFinished();

	void on_end_color_y_textEdited(const QString &arg1);

	void on_end_color_z_editingFinished();

	void on_end_color_z_textEdited(const QString &arg1);

	void on_gradient_toggled(bool checked);

	void on_environment_map_toggled(bool checked);

	void on_open_environment_clicked();

private:
	Ui::RenderOptions *ui;
	QDoubleValidator* parameter_validator;
	QIntValidator* color_validator;

	void line_edit_edited(QLineEdit* line_edit, const QString& text);
};

