#include "Viewport.h"

#include <QString>
#include <QMouseEvent>
#include <QDebug>

Viewport::Viewport(QWidget * parent, Qt::WindowFlags f)
	: QLabel(parent, f) {}

Viewport::Viewport(const QString& text, QWidget* parent, Qt::WindowFlags f)
	: QLabel(text, parent, f) {}

void Viewport::mousePressEvent(QMouseEvent* ev)
{
	QString x = QString::number(ev->x());
	QString y = QString::number(ev->y());
	qDebug() << x << "," << y;
}
