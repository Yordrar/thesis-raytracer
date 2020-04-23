#pragma once

#include <QMainWindow>

#include <view/Viewport.h>
#include <view/Inspector.h>

#include <thread>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

	void resizeEvent(QResizeEvent* event);

private slots:
	void render_preview();
	void on_start_render_button_clicked();

	void on_cancel_render_button_clicked();

private:
    Ui::MainWindow *ui;
	Viewport* viewport;
	Inspector* inspector;
	QTimer* render_timer;
	void update_render_viewport();
	std::thread* render_worker = nullptr;
};

