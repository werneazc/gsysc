cmake_minimum_required(VERSION "3.10")

project(
  gSyscTestProject
  VERSION "0.1.0"
  DESCRIPTION "Test project for GsysC")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
                      ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

add_executable(
  gsysc_testproject consumer.cc fifo.cc
                    producer.cc main.cc)
target_compile_features(gsysc_testproject PRIVATE cxx_std_14)
target_link_libraries(
  gsysc_testproject PRIVATE gsysc ${SYSTEMC_LIBRARIES} Qt5::Core
                            Qt5::Gui Qt5::Widgets "pthread")
target_include_directories(
  gsysc_testproject PRIVATE ${SYSTEMC_INCLUDE_DIRS}
                            ${CMAKE_CURRENT_SOURCE_DIR} Qt5Widget_INCLUDE_DIR)
target_compile_definitions(gsysc_testproject PRIVATE SC_USE_SC_STRING_OLD)
