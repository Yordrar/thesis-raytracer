#pragma once

#include <QLabel>

class Viewport : public QLabel
{
	Q_OBJECT
public:
	Viewport(QWidget* parent = nullptr, Qt::WindowFlags f = nullptr);
	Viewport(const QString& text, QWidget* parent = nullptr, Qt::WindowFlags f = nullptr);

	void mousePressEvent(QMouseEvent* ev);

signals:
	void mousePressed(const QPoint&);
};

