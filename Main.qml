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

import QtQuick 6.4
import QtQuick.Controls 6.4
import QtQuick.Layouts 6.4

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr('Yonpun')
    color: '#e8e8e8'

    component Title: Row {
        property alias color: bar.color
        property alias text: label.text

        spacing: 6

        Rectangle {
            id: bar
            width: 10
            height: 32
            anchors.verticalCenter: parent.verticalCenter
        }

        Label {
            id: label
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: 32
            color: 'black'
        }
    }

    component GreenButton: Button {
        property alias t: label.text

        id: root

        background: Rectangle {
            color: !root.down ? '#9bd65f' : '#89bc57'
            border.color: '#bbbbbb'
            border.width: 2
        }

        contentItem: Label {
            id: label
            color: 'black'
        }
    }

    component OrangeButton: Button {
        property alias t: label.text

        id: root

        background: Rectangle {
            color: !root.down ? '#ffba00' : '#d69d00'
            border.color: '#bbbbbb'
            border.width: 2
        }

        contentItem: Label {
            id: label
            color: 'black'
        }
    }

    component GrayButton: Button {
        property alias t: label.text

        id: root

        background: Rectangle {
            color: !root.down ? '#bbbbbb' : '#a0a0a0'
            border.color: '#bbbbbb'
            border.width: 2
        }

        contentItem: Label {
            id: label
            color: 'black'
        }
    }

    Dialog {
        id: dlg
        modal: true
        x: parent.width/2 - width/2
        y: parent.height/2 - height/2

        contentItem: Rectangle {
            id: card
            width: 320; height: 200
            color: 'white'
            opacity: 0
            scale: 0.92
            anchors.centerIn: parent
            radius: 8

            Column { anchors.centerIn: parent; spacing: 8
                Text { text: 'Dialog title'; font.bold: true }
                Text { text: 'Body text goes here.' }
            }
        }

        enter: Transition {
            ParallelAnimation {
                NumberAnimation { target: card; property: 'opacity'; from: 0; to: 1; duration: 180; easing.type: Easing.OutCubic }
                NumberAnimation { target: card; property: 'scale';   from: 0.92; to: 1.0; duration: 240; easing.type: Easing.OutBack }
            }
        }

        exit: Transition {
            ParallelAnimation {
                NumberAnimation { target: card; property: 'opacity'; from: 1; to: 0; duration: 140; easing.type: Easing.InCubic }
                NumberAnimation { target: card; property: 'scale';   from: 1.0; to: 0.96; duration: 140; easing.type: Easing.InCubic }
            }
        }
    }

    TabBar {
        id: mainBar
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        spacing: 8

        leftInset: -8
        rightInset: -8

        background: Rectangle {
            color: 'black'
            bottomLeftRadius: mainBar.leftInset * -1
            bottomRightRadius: mainBar.rightInset * -1
        }

        Repeater {
            model: [
                { index: 0, text: qsTr('Dashboard'), color: '#9bd65f' },
                { index: 1, text: qsTr('To-do'), color: '#ff445c' },
                { index: 2, text: qsTr('Calendar'), color: '#ffba00' },
            ]

            TabButton {
                required property var modelData
                id: tabButton
                leftPadding: 10
                rightPadding: 50
                width: implicitWidth

                contentItem: Row {
                    spacing: 16

                    Rectangle {
                        anchors.verticalCenter: parent.verticalCenter
                        color: tabButton.modelData.color
                        width: 25
                        height: 64
                    }

                    Column {
                        anchors.verticalCenter: parent.verticalCenter
                        Label {
                            text: tabButton.modelData.index                            
                            font.pixelSize: 32
                            color: 'black'
                        }

                        Label {
                            text: tabButton.modelData.text
                            font.pixelSize: 20
                            color: 'black'
                        }
                    }
                    
                }

                background: Rectangle {
                    color: if (mainBar.currentIndex == tabButton.modelData.index) {
                        'yellow'
                    } else if (tabButton.down) {
                        'lightgray'
                    } else {
                        'white'
                    }
                }
            }
        }

        TabButton {
            id: settingsButton
            topPadding: 20
            leftPadding: 10
            rightPadding: 50
            width: implicitWidth

            contentItem: Column {
                anchors.verticalCenter: parent.verticalCenter
                spacing: 8

                Label {
                    text: qsTr('⚙️')                           
                    font.pixelSize: 32
                    color: 'white'
                }

                Label {
                    text: qsTr('Settings')
                    font.pixelSize: 20
                    color: 'white'
                }
            }

            background: Rectangle {
                color: if (settingsButton.down) {
                    '#0c3c7f'
                } else {
                    '#1153af'
                }
            }
        }
    }



    StackLayout {
        id: mainStack
        anchors.top: mainBar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        currentIndex: mainBar.currentIndex
        Rectangle {
            color: 'teal'
            implicitWidth: 200
            implicitHeight: 200

            Row {
                Rectangle {
                    width: 100
                    height: 100
                    color: 'red'
                }

                Label {
                    text: 'hello world'
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }
        Frame {
            id: todoPage
            implicitWidth: 300
            implicitHeight: 200

            background: Rectangle {
                color: '#ffffff'
                border.color: '#bbbbbb'
                border.width: 1
                topLeftRadius: 32
                topRightRadius: 32
                bottomLeftRadius: 32
                bottomRightRadius: 32
            }

            leftInset: 64
            rightInset: 64
            topInset: 64
            bottomInset: 64

            leftPadding: leftInset + 32
            rightPadding: rightInset + 32
            topPadding: topInset + 32
            bottomPadding: bottomInset + 32

            contentItem: Column {

                spacing: 16

                RowLayout {
                    anchors.left: parent.left
                    anchors.right: parent.right

                    Title {
                        text: 'To-do'
                        color: '#ff445c'
                    }

                    Item {
                        Layout.fillWidth: true
                    }

                    RowLayout {
                        id: buttonWrapper
                        spacing: 15

                        GreenButton {
                            t: qsTr('Add task')

                            onClicked: {
                                dlg.open()
                            }
                        }

                        OrangeButton {
                            t: qsTr('Block out tasks')
                            onClicked: {
                                dlg.open()
                            }
                        }

                        GrayButton {
                            t: qsTr('Edit labels')
                        }
                    }
                }

                Label {
                    text: 'hello world'
                }

                Label {
                    text: 'hello world'
                }

                Label {
                    text: 'hello world'
                }
            }
        }
    }
}

