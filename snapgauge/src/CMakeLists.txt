set( snapgauge_SRCS
    main.cpp
    mainwindow.cpp
    mainwidget.cpp
    infowidget.cpp
    settingswidget.cpp
    hexeditwidget.cpp
 ) 

kde4_add_ui_files(snapgauge_SRCS
    mainwidget.ui
    infowidget.ui
    settingswidget.ui
 ) 

kde4_add_app_icon(snapgauge_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/../data/icons/hi*-app-snapgauge.png" )

kde4_add_executable(snapgauge ${snapgauge_SRCS})
target_link_libraries(snapgauge ${KDE4_KDECORE_LIBS} ${KDE4_KDEUI_LIBS} snap )

install( TARGETS snapgauge ${INSTALL_TARGETS_DEFAULT_ARGS} )
install( FILES snapgauge.desktop DESTINATION ${XDG_APPS_INSTALL_DIR} )
install( FILES snapgaugeui.rc DESTINATION  ${DATA_INSTALL_DIR}/snapgauge )