#include "Inspector.h"
#include "ui_Inspector.h"

Inspector::Inspector(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::Inspector)
{
	ui->setupUi(this);
}

Inspector::~Inspector()
{
	delete ui;
}

void Inspector::reload(Entity* entity)
{
	if(entity == nullptr) {
		ui->position_x->setText("");
		ui->position_y->setText("");
		ui->position_z->setText("");
	}
	else {
		Vector3 position = entity->get_position();
		ui->position_x->setText(QString::number(static_cast<double>(position.get_x())));
		ui->position_y->setText(QString::number(static_cast<double>(position.get_y())));
		ui->position_z->setText(QString::number(static_cast<double>(position.get_z())));
	}
}
