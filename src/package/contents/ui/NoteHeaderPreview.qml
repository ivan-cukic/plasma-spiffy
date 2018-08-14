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

import org.kde.kquickcontrolsaddons 2.0 as KQuickControlsAddons

import org.kde.plasma.plasmoid 2.0
import org.kde.plasma.components 2.0 as PlasmaComponents

import QtGraphicalEffects 1.0 as Effects

Item {
    id: noteHeaderPreviewRoot

    property int currentIndex: -1
    property variant currentPixmap: null

    function nextPreview() {
        currentIndex++;
        if (currentIndex >= noteHeaderPreviewRepeater.count) currentIndex = 0;
        console.log("Next preview is: " + currentIndex);
    }

    function previousPreview() {
        currentIndex--;
        if (currentIndex < 0) currentIndex = noteHeaderPreviewRepeater.count - 1;
        console.log("Previous preview is: " + currentIndex);
    }

    Repeater {
        id: noteHeaderPreviewRepeater

        model: plasmoid.nativeInterface.attachments.previewsModel

        onCountChanged: {
            if (count == 0) {
                currentIndex = -1;
                currentPixmap = null;
            } else if (currentIndex == -1) {
                currentIndex = 0;
            }
        }

        delegate: Item {
            property alias pixmap: itemPreview.pixmap

            anchors.fill: noteHeaderPreviewRoot

            visible: noteHeaderPreviewRoot.currentIndex == model.index
            onVisibleChanged: {
                if (visible) {
                    noteHeaderPreviewRoot.currentPixmap = model.preview
                }
            }

            KQuickControlsAddons.QPixmapItem {
                id: itemPreviewBlur

                anchors.centerIn: parent

                width: parent.width
                height: parent.height

                fillMode: KQuickControlsAddons.QPixmapItem.PreserveAspectCrop

                pixmap: itemPreview.pixmap
            }

            Effects.FastBlur {
                anchors.fill: itemPreviewBlur
                source: itemPreviewBlur
                radius: 32
            }

            Rectangle {
                color: "black"
                opacity: 0.5
                anchors.fill: parent
            }

            KQuickControlsAddons.QPixmapItem {
                id: itemPreview

                anchors {
                    centerIn: parent
                }

                width: itemThumbnail.width
                height: itemThumbnail.height

                fillMode: KQuickControlsAddons.QPixmapItem.PreserveAspectFit

                pixmap: model.preview
            }
        }
    }

    Row {
        anchors {
            top: parent.top
            right: parent.right
        }

        visible: noteHeaderPreviewRepeater.count > 1

        PlasmaComponents.ToolButton {
            iconSource: "go-previous-symbolic"
            onClicked: noteHeaderPreviewRoot.previousPreview()
        }

        PlasmaComponents.ToolButton {
            iconSource: "go-next-symbolic"
            onClicked: noteHeaderPreviewRoot.nextPreview()
        }
    }
}
