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

private slots:
	void on_start_render_button_clicked();

	void on_cancel_render_button_clicked();

private:
	Ui::RenderOptions *ui;
};

