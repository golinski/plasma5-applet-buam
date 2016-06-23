/* plasma5-applet-buam -- Books from your university libray
 * Copyright © 2016 Michał Goliński
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>. */

import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2
import QtQuick.Layouts 1.3
import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents
import org.kde.plasma.extras 2.0 as PlasmaExtras
import org.kde.plasma.private.buam 1.0 as Buam


Item {
    Layout.minimumWidth: 250
    Layout.preferredWidth: 250
    Layout.minimumHeight: 100
    Layout.preferredHeight: 350

    property bool refreshing: false

    function refresh() {
        if (plasmoid.configuration.cardNumber.length > 0 && plasmoid.configuration.peselNumber.length > 0) {
            refreshing = true
            buam.setCardNumber(plasmoid.configuration.cardNumber)
            buam.setPeselNumber(plasmoid.configuration.peselNumber)
            buam.updateBookList()
        }
    }

    Buam.Buam {
        id: buam
        
        onBooksDownloaded: {
            refreshing = false
            bookModel.clear()
            var booksList = buam.getBooksList()
            booksList.forEach(function (book) {
                bookModel.append({
                    "title": book.title,
                    "link": book.link,
                    "diff": book.diff,
                    "date": book.date
                })
            })
            refreshTimer.restart()
        }
    }

    Timer {
        id: refreshTimer
        interval: 21600000 //6 hours
        repeat: true
        running: true
        onTriggered: {
            refresh()
        }
    }

    Button {
        anchors.left: parent.left
        anchors.right: parent.right
        id: refreshButton
        iconName: "view-refresh"
        text: "Refresh"
        height: 40
        style: ButtonStyle {
            Text {
                horizontalAlignment: Text.AlignHCenter
                text: control.text
            }
        }
        onClicked: {
            refresh()
        }
    }

    PlasmaExtras.ScrollArea {
        id: scrollArea;
        anchors.fill: parent
        anchors.topMargin: refreshButton.height
        ListView {
            id: mainList
            focus: true
            boundsBehavior: Flickable.StopAtBounds
            model: ListModel {
                id: bookModel
            }
            delegate: PlasmaComponents.ListItem {
                height: 40
                RowLayout{
                    anchors.fill: parent
                    spacing: 6
                    PlasmaComponents.Label {
                        anchors.left: parent.left
                        height: implicitHeight
                        Layout.fillWidth: true
                        Layout.minimumWidth: 100
                        Layout.preferredWidth: 100
                        Layout.maximumWidth: 1000
                        elide: Text.ElideRight
                        text: model.title
                        color: (model.diff < 10) ? "#FF0000" : "#000000"
                    }
                    PlasmaComponents.Label {
                        //Layout.fillWidth: true
                        Layout.minimumWidth: 50
                        Layout.maximumWidth: 50
                        anchors.right: parent.right
                        height: implicitHeight
                        horizontalAlignment: Text.AlignRight
                        elide: Text.ElideRight
                        text: model.diff
                        color: (model.diff < 10) ? "#FF0000" : "#000000"
                    }
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: Qt.openUrlExternally(model.link)
                }
            }
        }
    }
    PlasmaComponents.BusyIndicator {
        anchors.centerIn: parent
        anchors.verticalCenterOffset: refreshButton.height/2
        //whilst the model is loading, stay visible
        //we use opacity rather than visible to force an animation
        opacity: refreshing ? 1 : 0
        Behavior on opacity {
            PropertyAnimation {
                //this comes from PlasmaCore
                duration: units.shortDuration
            }
        }
    }
    Component.onCompleted: {
        refresh()
    }
}

