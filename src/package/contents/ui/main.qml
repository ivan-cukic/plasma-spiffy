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

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.plasmoid 2.0
import org.kde.plasma.components 2.0 as PlasmaComponents
import org.kde.plasma.extras 2.0 as PlasmaX

import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.1 as QtQuickControlStyle
import QtQuick.Controls.Private 1.0 as QtQuickControlsPrivate

import org.kde.plasma.activityswitcher 1.0 as ActivitySwitcher


Item {
    id: root

    function droppedData(drop)
    {
        return drop.formats.map(function (mime) {
                return [ mime, drop.getDataAsString(mime) ];
            });
    }

    Plasmoid.hideOnWindowDeactivate: false

    Plasmoid.compactRepresentation: Item {
        PlasmaComponents.Highlight {
            id: dropMarker
            anchors.centerIn: iconApplet

            width: iconApplet.width + 2 * units.smallSpacing
            height: width

            visible: compactAppletDrop.containsDrag
        }

        PlasmaCore.IconItem {
            id: iconApplet
            source: "plasmacrisp"

            anchors.centerIn: parent
            width: units.iconSizeHints.panel
            height: width
        }

        MouseArea {
            anchors.fill: parent

            onClicked: plasmoid.expanded = !plasmoid.expanded
        }

        DropArea {
            id: compactAppletDrop

            anchors.fill: parent

            onDropped: {
                drop.acceptProposedAction();
                plasmoid.nativeInterface.createNoteFromData(droppedData(drop));
            }
        }
    }

    Layout.maximumWidth: 300

    Plasmoid.fullRepresentation: Item {
        Layout.maximumWidth: 300
        Layout.minimumHeight: 600

        NoteHeader {
            id: noteHeader

            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
            }
        }

        PlasmaComponents.TextArea {
            id: textDescription

            anchors {
                left: parent.left
                right: parent.right
                top: noteHeader.bottom
                bottom: listFiles.top

                topMargin: units.smallSpacing
                bottomMargin: units.smallSpacing
                leftMargin: - units.smallSpacing
                rightMargin: - units.smallSpacing
            }

            Component.onCompleted: {
                plasmoid.nativeInterface.registerDocument(textDescription.textDocument)
                plasmoid.nativeInterface.descriptionChanged.connect(
                    function() {
                        if (text != plasmoid.nativeInterface.description) {
                            text = plasmoid.nativeInterface.description;
                        }
                    }
                )
            }

            onTextChanged: plasmoid.nativeInterface.description = text

            style: QtQuickControlStyle.TextAreaStyle {
                id: style

                font: theme.defaultFont
                backgroundColor: "transparent"
                textColor: control.backgroundVisible ? theme.viewTextColor : PlasmaCore.ColorScope.textColor
                selectionColor: control.backgroundVisible ? theme.viewFocusColor : PlasmaCore.ColorScope.highlightColor
                selectedTextColor: control.backgroundVisible ? theme.viewHighlightedTextColor : PlasmaCore.ColorScope.highlightedTextColor

                frame: Rectangle {
                    color: Qt.rgba(0, 0, 0, 0.2)
                }
            }
        }

        ListView {
            id: listFiles

            anchors {
                left: parent.left
                right: parent.right
                bottom: buttons.top
            }

            clip: true
            model: plasmoid.nativeInterface.attachments

            height: 0

            onCountChanged: {
                height = Math.min(count, 3.5) * (units.iconSizeHints.panel + 2 * units.smallSpacing);
            }

            currentIndex: -1

            delegate: PlasmaComponents.ListItem {
                id: delegateRoot
                height: units.iconSizeHints.panel + 2 * units.smallSpacing
                width: parent.width

                Item {
                    anchors {
                        fill: parent
                    }

                    height: units.iconSizeHints.panel

                    PlasmaCore.IconItem {
                        anchors {
                            top: parent.top
                            left: parent.left
                            bottom: parent.bottom
                        }

                        id: itemIcon

                        source: model.icon
                        width:  parent.height
                        height: parent.height
                    }

                    PlasmaComponents.Label {
                        text: model.title
                        height: parent.height

                        anchors {
                            top: parent.top
                            bottom: parent.bottom
                            left: itemIcon.right
                            right: buttonActions.left
                            margins: units.smallSpacing
                        }

                        elide: Label.ElideRight

                        opacity: panelActions.visible ? 0.3 : 1.0
                        Behavior on opacity {
                            NumberAnimation { duration: units.longDuration }
                        }
                    }

                    PlasmaComponents.ToolButton {
                        id: buttonActions

                        anchors {
                            top: parent.top
                            right: parent.right
                            bottom: parent.bottom
                        }

                        // iconSource: "application-menu"
                        iconSource: "view-more-symbolic"
                        width:  parent.height
                        height: parent.height

                        onClicked: panelActions.visible = !panelActions.visible
                    }

                    PlasmaComponents.Highlight {
                        id: panelActions

                        visible: false

                        anchors {
                            top: parent.top
                            right: parent.right
                            bottom: parent.bottom
                        }

                        width: 4 * height

                        Row {
                            anchors.fill: parent

                            PlasmaComponents.ToolButton {
                                iconSource: "edit-link"
                                flat: false
                            }

                            PlasmaComponents.ToolButton {
                                iconSource: "edit-copy"
                                flat: false
                            }

                            PlasmaComponents.ToolButton {
                                iconSource: "list-remove"
                                onClicked: plasmoid.nativeInterface.removeAttachment(model.url)
                                flat: false
                            }
                        }
                    }
                }
            }
        }

        Item {
            id: buttons

            height: buttonCreate.height

            anchors {
                bottom: parent.bottom
                left: parent.left
                right: parent.right
            }

            PlasmaComponents.ToolButton {
                id: buttonActivity
                iconSource: "open-menu-symbolic"

                anchors {
                    left: parent.left
                }
            }

            PlasmaComponents.Button {
                id: buttonCreate
                text: i18n("Create")

                anchors {
                    right: buttonCancel.left
                    rightMargin: units.smallSpacing
                }
            }

            PlasmaComponents.Button {
                id: buttonCancel
                text: i18n("Cancel")

                anchors {
                    right: parent.right
                }
            }
        }

        PlasmaComponents.Highlight {
            anchors.fill: parent
            visible: mainAppletDrop.containsDrag
        }

        DropArea {
            id: mainAppletDrop

            anchors.fill: parent

            onDropped: {
                plasmoid.nativeInterface.createNoteFromData(droppedData(drop));
            }
        }
    }
}
