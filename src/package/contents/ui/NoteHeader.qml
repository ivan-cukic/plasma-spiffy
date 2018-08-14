/*
 *   Copyright (C) 2018 Ivan Čukić <ivan.cukic(at)kde.org>
 *
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Lesser General Public
 *   License as published by the Free Software Foundation; either
 *   version 2.1 of the License, or (at your option) version 3, or any
 *   later version accepted by the membership of KDE e.V. (or its
 *   successor approved by the membership of KDE e.V.), which shall
 *   act as a proxy defined in Section 6 of version 3 of the license.
 *
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *   Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public
 *   License along with this library.
 *   If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick 2.1
import QtQuick.Layouts 1.1

import org.kde.plasma.plasmoid 2.0
import org.kde.plasma.components 2.0 as PlasmaComponents

import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.1 as QtQuickControlStyle


Item {
    id: root

    height: width / 4 * 3

    anchors.margins: - units.smallSpacing

    Image {
        id: itemThumbnail

        anchors.fill: parent

        Layout.maximumWidth: 300
        Layout.maximumHeight: 300

        source: "../images/default-preview.png"
        fillMode: Image.PreserveAspectCrop
        clip: true
    }

    NoteHeaderPreview {
        anchors.fill: itemThumbnail
    }

    Rectangle {
        anchors {
            bottom: itemThumbnail.bottom
            right: itemThumbnail.right
            left: itemThumbnail.left
        }

        height: 64
        opacity: 0.7

        gradient: Gradient {
            GradientStop { position: 1.0; color: "black" }
            GradientStop { position: 0.5; color: Qt.rgba(0, 0, 0, 0.75) }
            GradientStop { position: 0.0; color: Qt.rgba(0, 0, 0, 0) }
        }
    }

    PlasmaComponents.TextField {
        id: textTitle

        anchors {
            bottom: itemThumbnail.bottom
            right: itemThumbnail.right
            left: itemThumbnail.left
        }

        height: 32

        Component.onCompleted: {
            plasmoid.nativeInterface.titleChanged.connect(
                function() {
                    if (text != plasmoid.nativeInterface.title) {
                        text = plasmoid.nativeInterface.title
                    }
                }
            )
        }

        onTextChanged: plasmoid.nativeInterface.title = text

        style: QtQuickControlStyle.TextFieldStyle {
            id: styleRoot

            textColor: Qt.rgba(1.0, 1.0, 1.0, 1.0)
            font.pixelSize: 20

            selectionColor: theme.highlightColor
            selectedTextColor: theme.viewHighlightedTextColor

            background: Item {
            }
        }
    }


}
