/*
Copyright (c) 2026 gakuseh

This file is part of Yonpun.

Yonpun is free software: you can redistribute it and/or modify it under the 
terms of the GNU General Public License as published by the Free Software 
Foundation, either version 3 of the License, or (at your option) any later 
version.

Yonpun is distributed in the hope that it will be useful, but WITHOUT ANY 
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A 
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with 
Yonpun. If not, see <https://www.gnu.org/licenses/>. 
*/

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