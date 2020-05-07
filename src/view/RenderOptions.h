#pragma once

#include <QDialog>
#include <QDoubleValidator>

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

private:
	Ui::RenderOptions *ui;
};

