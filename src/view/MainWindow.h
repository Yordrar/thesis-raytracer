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

	void resizeEvent(QResizeEvent* event) override;

	bool eventFilter(QObject *obj, QEvent *event) override;

signals:
	void update_render_progress(int samples_rendered);

private slots:
	void render_preview();
	void start_render();
	void cancel_render();
	void update_render_viewport();

	void on_actionNew_Scene_triggered();

	void on_actionOpen_Scene_triggered();

	void on_actionAdd_from_File_triggered();

	void on_actionExit_triggered();

	void on_actionRender_Image_triggered();

	void on_actionSave_Render_triggered();

private:
    Ui::MainWindow *ui;
	Viewport* viewport;
	Inspector* inspector;
	QTimer* render_timer;
	std::thread* render_worker = nullptr;
	float render_viewport_scale = 1;
	bool is_ctrl_pressed = false;
};

