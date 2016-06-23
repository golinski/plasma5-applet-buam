set (PLUGIN_NAME buamplugin)

add_definitions (${Qt5Core_DEFINITIONS} ${Qt5Qml_DEFINITIONS})
set (Qt_INCLUDE ${Qt5Core_INCLUDE_DIRS}
                ${Qt5Qml_INCLUDE_DIRS})
set (Qt_LIBRARIES ${Qt5Core_LIBRARIES}
                  ${Qt5Qml_LIBRARIES})

include_directories (${CMAKE_SOURCE_DIR}
                     ${CMAKE_BINARY_DIR}
                     ${CMAKE_CURRENT_BINARY_DIR}
                     ${CMAKE_CURRENT_BINARY_DIR}/../
                     ${Qt_INCLUDE})

file (GLOB_RECURSE SUBPROJECT_SOURCE *.cpp)

add_library (${PLUGIN_NAME} SHARED ${SUBPROJECT_SOURCE})
target_link_libraries (${PLUGIN_NAME} ${Qt_LIBRARIES} )

install (TARGETS ${PLUGIN_NAME} DESTINATION ${QML_INSTALL_DIR}/org/kde/plasma/private/buam)
install (FILES qmldir DESTINATION ${QML_INSTALL_DIR}/org/kde/plasma/private/buam)
