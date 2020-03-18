#include "Viewport.h"

#include <QString>
#include <QMouseEvent>
#include <QDebug>

#include <manager/RenderManager.h>

Viewport::Viewport(QWidget * parent, Qt::WindowFlags f)
	: QLabel(parent, f),
	  last_point_pressed(QPoint(-1, -1))
{
	this->setFocusPolicy(Qt::ClickFocus);
}

Viewport::Viewport(const QString& text, QWidget* parent, Qt::WindowFlags f)
	: QLabel(text, parent, f),
	  last_point_pressed(QPoint(-1, -1))
{
	this->setFocusPolicy(Qt::ClickFocus);
}

void Viewport::mousePressEvent(QMouseEvent* ev)
{
	if(ev->button() == Qt::MouseButton::RightButton)
		last_point_pressed = ev->pos();
}

void Viewport::mouseReleaseEvent(QMouseEvent* ev)
{
	if(ev->button() == Qt::MouseButton::RightButton)
		last_point_pressed = QPoint(-1, -1);
}

void Viewport::mouseMoveEvent(QMouseEvent *ev)
{
	if(last_point_pressed.x() != -1 && last_point_pressed.y() != -1) {
		RenderManager::get_manager()->rotate_camera(last_point_pressed.x(),
													last_point_pressed.y(),
													ev->x(),
													ev->y());
		last_point_pressed = ev->pos();
		emit render();
	}
}

void Viewport::keyPressEvent(QKeyEvent* ev)
{
	if(ev->key() == Qt::Key_W) {
		RenderManager::get_manager()->move_camera(0, 0, -0.5f);
		emit render();
	}
	if(ev->key() == Qt::Key_A) {
		RenderManager::get_manager()->move_camera(-0.5f, 0, 0);
		emit render();
	}
	else if(ev->key() == Qt::Key_S) {
		RenderManager::get_manager()->move_camera(0, 0, 0.5f);
		emit render();
	}
	else if(ev->key() == Qt::Key_D) {
		RenderManager::get_manager()->move_camera(0.5f, 0, 0);
		emit render();
	}
	else if(ev->key() == Qt::Key_Q) {
		RenderManager::get_manager()->move_camera(0, 0.5f, 0);
		emit render();
	}
	else if(ev->key() == Qt::Key_E) {
		RenderManager::get_manager()->move_camera(0, -0.5f, 0);
		emit render();
	}
	else
		QLabel::keyPressEvent(ev);
}
