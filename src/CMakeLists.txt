# TODO: adapt "org.kde.plasma" here & elsewhere if needed (see README)
add_definitions(-DTRANSLATION_DOMAIN=\"plasma_applet_org.kde.plasma.crisp\")

set(crisp_SRCS
    crisp.cpp
    EditorHighlight.cpp
    AttachmentModel.cpp
)

add_library(plasma_applet_crisp MODULE ${crisp_SRCS})

kcoreaddons_desktop_to_json(plasma_applet_crisp package/metadata.desktop SERVICE_TYPES plasma-applet.desktop)

target_link_libraries(plasma_applet_crisp
                      Qt5::Gui
                      Qt5::Quick
                      KF5::Plasma
                      KF5::I18n
                      KF5::FileMetaData
                      KF5::KIOCore
                      KF5::KIOWidgets
                      )


install(TARGETS plasma_applet_crisp DESTINATION ${KDE_INSTALL_PLUGINDIR}/plasma/applets)

plasma_install_package(package org.kde.plasma.crisp)
