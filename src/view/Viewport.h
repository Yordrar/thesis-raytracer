#pragma once

#include <QLabel>

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

private:
	QPoint last_point_pressed;
};

