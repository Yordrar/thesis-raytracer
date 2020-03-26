#pragma once

#include <QWidget>

namespace Ui {
class Inspector;
}

class Inspector : public QWidget
{
	Q_OBJECT

public:
	explicit Inspector(QWidget *parent = nullptr);
	~Inspector();

private:
	Ui::Inspector *ui;
};

