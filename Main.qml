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

    component Title: RowLayout {
        property alias barColor: bar.color
        property alias text: text.text
        property alias color: text.color
        property int barHeight: 32

        implicitWidth: bar.implicitWidth + spacing + text.implicitWidth
        implicitHeight: Math.max(bar.implicitHeight, text.implicitHeight)

        id: root
        spacing: (1/3) * barHeight

        Rectangle {
            id: bar
            height: root.barHeight
            implicitWidth: (1/3) * height
            implicitHeight: height
        }

        Label {
            id: text
            font.pixelSize: root.barHeight
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

        background: Rectangle {
            color: '#ffffff'
        }

        TabButton {
            leftPadding: 14
            rightPadding: 14
            topPadding: 8
            bottomPadding: 8

            implicitWidth: leftPadding + contentItem.implicitWidth + rightPadding
            implicitHeight: topPadding + contentItem.implicitHeight + bottomPadding

            Component.onCompleted: {
                console.log('TabButton', 'Dashboard', 'width=', width, 'implicitWidth=', implicitWidth, 'contentWidth=', contentItem.width, 'contentImplicitWidth=', contentItem.implicitWidth)
            }

            onWidthChanged: {
                console.log('TabButton', 'Dashboard', 'width=', width, 'implicitWidth=', implicitWidth, 'contentWidth=', contentItem.width, 'contentImplicitWidth=', contentItem.implicitWidth)
            }

            contentItem: Title {
                text: qsTr('Dashboard')
                color: 'black'
                barColor: 'green'

                Component.onCompleted: {
                    console.log('Title', text, 'width=', width, 'implicitWidth=', implicitWidth)
                }

                onWidthChanged: {
                    console.log('Title', text, 'width=', width, 'implicitWidth=', implicitWidth)
                }
            }

            background: Rectangle {
                color: mainBar.currentIndex == 0 ? 'yellow' : 'white'
            }
        }

        TabButton {
            leftPadding: 14
            rightPadding: 14
            topPadding: 8
            bottomPadding: 8

            implicitWidth: leftPadding + contentItem.implicitWidth + rightPadding
            implicitHeight: topPadding + contentItem.implicitHeight + bottomPadding

            Component.onCompleted: {
                console.log('TabButton', 'To-do', 'width=', width, 'implicitWidth=', implicitWidth, 'contentWidth=', contentItem.width, 'contentImplicitWidth=', contentItem.implicitWidth)
            }

            onWidthChanged: {
                console.log('TabButton', 'To-do', 'width=', width, 'implicitWidth=', implicitWidth, 'contentWidth=', contentItem.width, 'contentImplicitWidth=', contentItem.implicitWidth)
            }

            contentItem: Title {
                text: qsTr('To-do')
                color: 'black'
                barColor: 'blue'

                Component.onCompleted: {
                    console.log('Title', text, 'width=', width, 'implicitWidth=', implicitWidth)
                }

                onWidthChanged: {
                    console.log('Title', text, 'width=', width, 'implicitWidth=', implicitWidth)
                }
            }

            background: Rectangle {
                color: mainBar.currentIndex == 1 ? 'yellow' : 'white'
            }
        }

        TabButton {
            leftPadding: 14
            rightPadding: 14
            topPadding: 8
            bottomPadding: 8

            implicitWidth: leftPadding + contentItem.implicitWidth + rightPadding
            implicitHeight: topPadding + contentItem.implicitHeight + bottomPadding

            Component.onCompleted: {
                console.log('TabButton', 'Calendar', 'width=', width, 'implicitWidth=', implicitWidth, 'contentWidth=', contentItem.width, 'contentImplicitWidth=', contentItem.implicitWidth)
            }

            onWidthChanged: {
                console.log('TabButton', 'Calendar', 'width=', width, 'implicitWidth=', implicitWidth, 'contentWidth=', contentItem.width, 'contentImplicitWidth=', contentItem.implicitWidth)
            }

            contentItem: Title {
                text: qsTr('Calendar')
                color: 'black'
                barColor: 'purple'

                Component.onCompleted: {
                    console.log('Title', text, 'width=', width, 'implicitWidth=', implicitWidth)
                }

                onWidthChanged: {
                    console.log('Title', text, 'width=', width, 'implicitWidth=', implicitWidth)
                }
            }

            background: Rectangle {
                color: mainBar.currentIndex == 2 ? 'yellow' : 'white'
            }
        }

        TabButton {
            leftPadding: 14
            rightPadding: 14
            topPadding: 8
            bottomPadding: 8

            implicitWidth: leftPadding + contentItem.implicitWidth + rightPadding
            implicitHeight: topPadding + contentItem.implicitHeight + bottomPadding

            Component.onCompleted: {
                console.log('TabButton', 'Settings', 'width=', width, 'implicitWidth=', implicitWidth, 'contentWidth=', contentItem.width, 'contentImplicitWidth=', contentItem.implicitWidth)
            }

            onWidthChanged: {
                console.log('TabButton', 'Settings', 'width=', width, 'implicitWidth=', implicitWidth, 'contentWidth=', contentItem.width, 'contentImplicitWidth=', contentItem.implicitWidth)
            }

            contentItem: Title {
                text: qsTr('Settings')
                color: 'black'
                barColor: 'orange'

                Component.onCompleted: {
                    console.log('Title', text, 'width=', width, 'implicitWidth=', implicitWidth)
                }

                onWidthChanged: {
                    console.log('Title', text, 'width=', width, 'implicitWidth=', implicitWidth)
                }
            }

            background: Rectangle {
                color: mainBar.currentIndex == 3 ? 'yellow' : 'white'
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
            }

            leftInset: 128
            rightInset: 128
            topInset: 128
            bottomInset: 128

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
                        color: 'black'
                        barColor: '#ff445c'
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

