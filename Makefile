TARGET_ARCH = linux
CC     = g++
OPT    = #-O2
DEBUG  = -g #-DDEBUG_GSYSC 
#OTHER  = -Wall -Wno-deprecated
OTHER  = -Wno-deprecated -fpermissive -DSC_USE_SC_STRING_OLD
CFLAGS = $(OPT) $(OTHER) $(DEBUG)

## Variable that points to SystemC installation path
SYSTEMC = /opt/systemc-2.3.2
LIB_SYSTEMC=$(SYSTEMC)/lib64/libsystemc.a 

# other necessary path variables:
QT_DIR = /usr/lib64/qt3
INCDIR = -I$(SYSTEMC)/include -I$(QT_DIR)/include
LIBDIR = -L/usr/lib -L$(QT_DIR)/lib64


####################################################################

SRCS = gsysConnection.cc gsysHierarchy.cc gsysHierarchyTree.cc gsysHierarchyWindow.cc gsysRegister.cc \
	gsysMain.cc gsysMainGUI.cc gsysPort.cc gsysSignal.cc gsysSimulator.cc \
	gsysPortViewer.cc gsysSignalViewer.cc gsysCanvasView.cc gsysAbout.cc gsysBuffer.cc gsysReplaced.cc
OBJS = gsysConnection.o gsysHierarchy.o gsysHierarchyTree.o gsysHierarchyWindow.o gsysRegister.o \
	gsysMain.o gsysMainGUI.o gsysPort.o gsysSignal.o gsysSimulator.o \
	gsysPortViewer.o gsysSignalViewer.o gsysCanvasView.o gsysAbout.o gsysBuffer.o gsysReplaced.o
HEAD = gsysConnection.h gsysHierarchy.h gsysHierarchyTree.h gsysHierarchyWindow.h \
	gsysMain.h gsysMainGUI.h gsysPort.h gsysSignal.h gsysSimulator.h gsysc.h \
	gsysPortViewer.h gsysSignalViewer.h gsysCanvasView.h gsysAbout.h gsysBuffer.h gsysReplaced.h
MOC= gsysAbout.moc gsysBuffer.moc gsysCanvasView.moc gsysHierarchyTree.moc gsysHierarchyWindow.moc gsysMainGUI.moc \
	gsysPortViewer.moc gsysSignalViewer.moc gsysSimulator.moc

# libraries to link
EXTRA_LIBS = -lqt-mt -lpthread
LIBS   =  -lsystemc -lm $(EXTRA_LIBS) 

# rule to create everything
all: lib wrapper

# rule to create library package
lib: $(MOC) $(OBJS) $(HEAD) $(LIB_SYSTEMC)
	ar cru libgsysc.a $(OBJS)

# rule to create MetaObject-files; necessary for Qt
$(MOC): %.moc: %.h
	$(QT_DIR)/bin/moc $< -o $@

# rule to create object files
.cc.o: $(MOC) $(HEAD) 
	$(CC) $(CFLAGS) $(INCDIR) -c $<

# rule to compile wrapper
wrapper: gsysWrapper.cc
	$(CC) $(CFLAGS) $(INCDIR) gsysWrapper.cc -o wrapper

# delete all created files
.PHONY: clean
clean::
	rm -f $(OBJS) *~ $(EXE) core libgsysc.a *.moc wrapper

