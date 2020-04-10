#pragma once

#include <QWidget>

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

public slots:
	void reload(Entity* entity);

private slots:
	void on_open_texture_clicked();

	void on_open_normal_clicked();

	void on_comboBox_currentIndexChanged(int index);

private:
	Ui::Inspector *ui;
	unsigned int map_preview_size;
};

