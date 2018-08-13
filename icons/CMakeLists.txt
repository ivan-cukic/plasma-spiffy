# install (
#    FILES plasmavault.svgz plasmavault-error.svgz
#    DESTINATION ${PLASMA_DATA_INSTALL_DIR}/desktoptheme/default/icons/
#    )

include (ECMInstallIcons)

ecm_install_icons (
   ICONS
   hi22-apps-plasmacrisp.svgz
   hi32-apps-plasmacrisp.svgz
   hi64-apps-plasmacrisp.svgz
   DESTINATION ${ICON_INSTALL_DIR}
   )

