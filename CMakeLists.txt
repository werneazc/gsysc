cmake_minimum_required(VERSION 3.10)

project(gSysC
    VERSION "0.1.2"
    DESCRIPTION "Visualize SystemC Hierarchical Composition"
)

OPTION(STATIC_CHECK "Use clang-tidy to check compiled files." OFF)

IF(STATIC_CHECK)
    set(CMAKE_CXX_CLANG_TIDY clang-tidy -checks=*,-google-*,-android-*,-openmp-*,-zircon-*,)
ENDIF()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)
find_package(SystemC REQUIRED)
find_package(Threads REQUIRED)

set(CMAKE_AUTOMOC ON)
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED )

add_library(gsysc STATIC
    "gsysConnection.cc" "gsysHierarchy.cc" "gsysHierarchyTree.cc" "gsysHierarchyWindow.cc" "gsysRegister.cc"
    "gsysMain.cc" "gsysMainGUI.cc" "gsysPort.cc" "gsysSignal.cc" "gsysSimulator.cc"
    "gsysPortViewer.cc" "gsysSignalViewer.cc" "gsysCanvasView.cc" "gsysAbout.cc" "gsysBuffer.cc" "gsysReplaced.cc"
)
target_compile_definitions(gsysc
    PRIVATE SC_USE_SC_STRING_OLD
)
target_compile_options(gsysc
    PRIVATE "-Wno-deprecated" "-fpermissive" "-fPIC"
)
target_include_directories(gsysc
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE ${SYSTEMC_INCLUDE_DIRS}
    PRIVATE Qt5Widget_INCLUDE_DIR
)

target_link_libraries(gsysc
    PRIVATE Qt5::Gui Qt5::Core Qt5::Widgets
    PRIVATE "${SYSTEMC_LIBRARIES}"
    PRIVATE Threads::Threads
)
#####################################################################
IF(INCLUDE_TESTS)
    add_subdirectory(test)
ENDIF(INCLUDE_TESTS)
