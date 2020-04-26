#include "Viewport.h"

#include <QString>
#include <QMouseEvent>
#include <QDebug>

#include <manager/RenderManager.h>
#include <manager/SceneManager.h>

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
	else if(ev->button() == Qt::MouseButton::LeftButton) {
		RenderManager::get_manager()->make_selection(static_cast<int>(ev->localPos().x()),
													 static_cast<int>(ev->localPos().y()));
		emit entity_selected_changed();
		emit render_preview();
	}
	else
		QLabel::mousePressEvent(ev);
}

void Viewport::mouseReleaseEvent(QMouseEvent* ev)
{
	if(ev->button() == Qt::MouseButton::RightButton)
		last_point_pressed = QPoint(-1, -1);
	else
		QLabel::mouseReleaseEvent(ev);
}

void Viewport::mouseMoveEvent(QMouseEvent *ev)
{
	if(last_point_pressed.x() != -1 && last_point_pressed.y() != -1) {
		SceneManager::get_manager()->rotate_camera(last_point_pressed.x(),
													last_point_pressed.y(),
													ev->x(),
													ev->y());
		last_point_pressed = ev->pos();
		emit render_preview();
	}
	else
		QLabel::mouseMoveEvent(ev);
}

void Viewport::keyPressEvent(QKeyEvent* ev)
{
	if(ev->key() == Qt::Key_W) {
		SceneManager::get_manager()->move_camera(SceneManager::MOVE_DIRECTION::FRONT);
		emit render_preview();
	}
	else if(ev->key() == Qt::Key_A) {
		SceneManager::get_manager()->move_camera(SceneManager::MOVE_DIRECTION::LEFT);
		emit render_preview();
	}
	else if(ev->key() == Qt::Key_S) {
		SceneManager::get_manager()->move_camera(SceneManager::MOVE_DIRECTION::BACK);
		emit render_preview();
	}
	else if(ev->key() == Qt::Key_D) {
		SceneManager::get_manager()->move_camera(SceneManager::MOVE_DIRECTION::RIGHT);
		emit render_preview();
	}
	else if(ev->key() == Qt::Key_Q) {
		SceneManager::get_manager()->move_camera(SceneManager::MOVE_DIRECTION::UP);
		emit render_preview();
	}
	else if(ev->key() == Qt::Key_E) {
		SceneManager::get_manager()->move_camera(SceneManager::MOVE_DIRECTION::DOWN);
		emit render_preview();
	}
	else
		QLabel::keyPressEvent(ev);
}
