#pragma once

#include <QMainWindow>

#include <view/Viewport.h>

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
	void on_render_button_clicked();

private:
    Ui::MainWindow *ui;
	Viewport* viewport;
};

