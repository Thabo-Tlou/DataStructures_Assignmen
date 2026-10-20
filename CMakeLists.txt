cmake_minimum_required(VERSION 3.19)
project(DataStructures_Assignmen LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(DataStructures_Assignmen
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    hashsetopen.h
    hashsetclosed.h
    dijkstra.h
    hashsetopen.cpp
    hashsetclosed.cpp
    dijkstra.cpp
)

target_link_libraries(DataStructures_Assignmen
    PRIVATE
        Qt::Core
        Qt::Widgets
)

install(TARGETS DataStructures_Assignmen
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET DataStructures_Assignmen
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
