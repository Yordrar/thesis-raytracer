#include "view/MainWindow.h"

#include <QApplication>
#include <QStyleFactory>

#include <iostream>

int main(int argc, char *argv[])
{
	for(QString s : QStyleFactory::keys())
		std::cout << s.toStdString() << std::endl;
	QApplication app(argc, argv);
	QApplication::setStyle("windowsvista");
    MainWindow w;
    w.show();
	return app.exec();
}
