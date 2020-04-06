#pragma once

#include <QWidget>

#include <geometry/Entity.h>

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

private:
	Ui::Inspector *ui;
	Entity* entity_selected = nullptr;
};

