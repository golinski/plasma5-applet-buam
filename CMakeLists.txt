cmake_minimum_required(VERSION 3.5)
# set project name
set (PROJECT plasma_applet_buam)

# prepare
configure_file (metadata.desktop ${CMAKE_CURRENT_SOURCE_DIR}/plasmoid/metadata.desktop)

find_package (ECM 0.0.12 REQUIRED NO_MODULE)
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

find_package (Qt5 REQUIRED COMPONENTS Core Qml)
find_package (KF5 REQUIRED COMPONENTS Plasma)

include (KDEInstallDirs)
include (KDECMakeSettings)
include (KDECompilerSettings)

add_subdirectory (plugin)
plasma_install_package (plasmoid org.kde.plasma.private.buam)
