cmake_minimum_required(VERSION 3.14)

project(gSysC
    VERSION "0.1.0"
    DESCRIPTION "Visualize SystemC Hierarchical Composition"
)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)
find_package(SystemC REQUIRED)
message("SystemC Library: ${SYSTEMC_LIBRARY}")
message("SystemC Include Dir: ${SYSTEMC_INCLUDE_DIR}")

set(CMAKE_AUTOMOC ON)
find_package(Qt4 REQUIRED)

add_library(gsysc STATIC
    "gsysConnection.cc" "gsysHierarchy.cc" "gsysHierarchyTree.cc" "gsysHierarchyWindow.cc" "gsysRegister.cc"
    "gsysMain.cc" "gsysMainGUI.cc" "gsysPort.cc" "gsysSignal.cc" "gsysSimulator.cc" 
    "gsysPortViewer.cc" "gsysSignalViewer.cc" "gsysCanvasView.cc" "gsysAbout.cc" "gsysBuffer.cc" "gsysReplaced.cc"
)
target_compile_definitions(gsysc
    PRIVATE SC_USE_SC_STRING_OLD
)
target_compile_options(gsysc
    PRIVATE "-Wno-deprecated" "-fpermissive"
)
target_include_directories(gsysc
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE ${SYSTEMC_INCLUDE_DIR}
)
target_link_libraries(gsysc
    PRIVATE Qt4::QtGui Qt4::QtCore Qt4::Qt3Support
    PRIVATE "${SYSTEMC_LIBRARY}" 
    PRIVATE "lpthread"
)
#####################################################################
