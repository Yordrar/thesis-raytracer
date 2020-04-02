#pragma once

#include <QLabel>

#include <geometry/Entity.h>

class Viewport : public QLabel
{
	Q_OBJECT
public:
	Viewport(QWidget* parent = nullptr, Qt::WindowFlags f = nullptr);
	Viewport(const QString& text, QWidget* parent = nullptr, Qt::WindowFlags f = nullptr);

	void mousePressEvent(QMouseEvent* ev) override;
	void mouseReleaseEvent(QMouseEvent* ev) override;
	void mouseMoveEvent(QMouseEvent *ev) override;

	void keyPressEvent(QKeyEvent *ev) override;

signals:
	void render();
	void entity_selected_changed(Entity* entity);

private:
	QPoint last_point_pressed;
};

