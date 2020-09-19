cmake_minimum_required(VERSION 3.10)

project(gSysC)

set(CC "g++")
set(DEBUG "-g")
set(CMAKE_CXX_FLAGS "-Wno-deprecated" "-fpermissive" "-DSC_USE_SC_STRING_OLD" ${DEBUG})

# Variable that points to SystemC installation path
set(SYSTEMC "/opt/systemc-2.3.2")
set(LIB_SYSTEMC "${SYSTEMC}/lib64/libsystemc.a") 

# Other necessary path variables:
set(QT_DIR "/usr/lib64/qt3")
set(INCDIR "-I${SYSTEMC}/include" "-I${QT_DIR}/include")
set(LIBDIR "-L/usr/lib" "-L${QT_DIR}/lib64")

set(LIB "libgsysc.a")
find_program(AR "ar")


#####################################################################

set(SRCS "gsysConnection.cc" "gsysHierarchy.cc" "gsysHierarchyTree.cc" "gsysHierarchyWindow.cc" "gsysRegister.cc"  
    "gsysMain.cc" "gsysMainGUI.cc" "gsysPort.cc" "gsysSignal.cc" "gsysSimulator.cc" 
    "gsysPortViewer.cc" "gsysSignalViewer.cc" "gsysCanvasView.cc" "gsysAbout.cc" "gsysBuffer.cc" "gsysReplaced.cc")

set(OBJS "gsysConnection.o" "gsysHierarchy.o" "gsysHierarchyTree.o" "gsysHierarchyWindow.o" "gsysRegister.o" 
    "gsysMain.o" "gsysMainGUI.o" "gsysPort.o" "gsysSignal.o" "gsysSimulator.o" 
    "gsysPortViewer.o" "gsysSignalViewer.o" "gsysCanvasView.o" "gsysAbout.o" "gsysBuffer.o" "gsysReplaced.o")

set(MOC "gsysAbout" "gsysBuffer" "gsysCanvasView" "gsysHierarchyTree" "gsysHierarchyWindow" "gsysMainGUI" 
    "gsysPortViewer" "gsysSignalViewer" "gsysSimulator")

## Create MetaObject-files; necessary for Qt
foreach(moc_file ${MOC})
    execute_process(
        COMMAND ${QT_DIR}/bin/moc ${moc_file}.h -o ${moc_file}.moc 
    )
endforeach()

# Create object files
foreach(cc_file ${SRCS})
    execute_process(
        COMMAND ${CC} ${CMAKE_CXX_FLAGS} ${INCDIR} -c ${cc_file}
    )
endforeach()

# Compile wrapper
execute_process(
    COMMAND ${CC} ${CMAKE_CXX_FLAGS} ${INCDIR} gsysWrapper.cc -o wrapper
)

# Create library package
execute_process(
    COMMAND ${AR} cr ${LIB} ${OBJS}
)