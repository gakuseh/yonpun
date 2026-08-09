import QtQuick 6.4
import QtQuick.Controls 6.4
import QtQuick.Layouts 6.4




ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Yonpun")
    color: "#e8e8e8"

    component Title: RowLayout {
        property alias barColor: bar.color
        property alias text: text.text
        property alias color: text.color
        property int barHeight: 32

        id: root
        spacing: (1/3) * barHeight

        Rectangle {
            id: bar
            height: root.barHeight
            width: (1/3) * height
        }

        Label {
            id: text
            font.pixelSize: root.barHeight
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
            color: "white"
            opacity: 0
            scale: 0.92
            anchors.centerIn: parent
            radius: 8

            Column { anchors.centerIn: parent; spacing: 8
                Text { text: "Dialog title"; font.bold: true }
                Text { text: "Body text goes here." }
            }
        }

        enter: Transition {
            ParallelAnimation {
                NumberAnimation { target: card; property: "opacity"; from: 0; to: 1; duration: 180; easing.type: Easing.OutCubic }
                NumberAnimation { target: card; property: "scale";   from: 0.92; to: 1.0; duration: 240; easing.type: Easing.OutBack }
            }
        }

        exit: Transition {
            ParallelAnimation {
                NumberAnimation { target: card; property: "opacity"; from: 1; to: 0; duration: 140; easing.type: Easing.InCubic }
                NumberAnimation { target: card; property: "scale";   from: 1.0; to: 0.96; duration: 140; easing.type: Easing.InCubic }
            }
        }
    }

    TabBar {
        id: mainBar
        width: parent.width

        background: Rectangle {
            color: "#ffffff"
        }

        TabButton {
            text: qsTr("Dashboard")
        }

        TabButton {
            text: qsTr("To-do")
        }

        TabButton {
            text: qsTr("Calendar")
        }

        TabButton {
            text: qsTr("Settings")
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
                color: "#ffffff"
                border.color: "#bbbbbb"
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
                        text: "To-do"
                        color: "black"
                        barColor: "#ff445c"
                    }

                    Item {
                        Layout.fillWidth: true
                    }
                    
                    RowLayout {
                        id: buttonWrapper

                        Button {
                            text: qsTr("Add")
                            onClicked: {
                                dlg.open()
                            }
                        }

                        Button {
                            text: qsTr("Add")
                            onClicked: {
                                dlg.open()
                            }
                        }
                    }                    
                }

                Label {
                    text: "hello world"
                }

                Label {
                    text: "hello world"
                }

                Label {
                    text: "hello world"
                }
            }
        }
    }
}

