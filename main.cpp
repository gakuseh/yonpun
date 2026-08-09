#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQuickControls2/QQuickStyle>
#include <QQmlContext>

#include "todomodel.h"

int main (int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("gakuseh");
    QCoreApplication::setOrganizationDomain("gakuseh.dev");
    QCoreApplication::setApplicationName("Yonpun");

    QGuiApplication app(argc, argv);

    QQuickStyle::setStyle("Basic"); // Eventually make custom style

    QQmlApplicationEngine engine;

    TodoModel todoModel;
    engine.rootContext()->setContextProperty("todoModel", &todoModel);

    todoModel.addTodo("Buy milk");

    engine.load("main.qml");
    return app.exec();
}