/*
** This file is part of gSysC.
**
** gSysC is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** gSysC is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with gSysC with the file ``LICENSE''; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/*****************************************************************************
    Author: Christian J. Eibl
    last modified: 2005-09-29
 *****************************************************************************
    Content:
      This class is the main class for registration and administration
      of project data. It provides collected information to all
      other classes.
      
 *****************************************************************************/



#ifndef GSYS_REGISTER_H
#define GSYS_REGISTER_H

#include "systemc.h"
#include <vector>

#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QListWidgetItem>

class gsysConnection;
class gsysHierarchy;
class gsysSimulator;
class gsysHierarchyTree;
class gsysMainGUI;
class gsysPort;
class gsysSignal;

using namespace std;

class gsysRegister
{
  friend class gsysMain;

  private:

  vector<gsysConnection*> connList;
  vector<gsysHierarchy*> hierarchyList;
  vector<gsysPort*> portList;
  vector<gsysPort*> notAssignedPorts;
  vector<gsysSignal*> notAssignedSignals;
  vector<QListWidget*> listViewElements;
  vector<void*> bufferIDList;
  vector<char*> bufferNameList;
  vector<bool> bufferPercList;
  int moduleInList(sc_module* module, vector<gsysHierarchy*> *list);
  int portInList(gsysPort* port, vector<gsysPort*> *list);
  int portExists(void* port, vector<gsysPort*> *list);
  int signalInList(void* signal, vector<gsysSignal*> *list);
  void regFinished();
  int connExists(gsysHierarchy* h1, gsysHierarchy* h2);
  gsysRegister();
  QListWidget *rootLVI;
  void insertLVI(QListWidget *parentLVI, gsysHierarchy* hier);


  public:
  
  ~gsysRegister();

  void registerModule(sc_module* modulePointer, char* name=0, sc_module* parent=0);
  void registerPort(void* realPort, sc_module* parentPointer,void* signal=0);
 
  void rename_port(void* realPort, char* name);
  void rename_sig(void* realSignal, char* name);

  void addBuffer(void* id, char* name, bool percUse=true);
  vector<void*> getBufferIDs();
  vector<bool> getBufferPercs();
  vector<char*> getBufferNames();
  void registerSignal(gsysSignal* sig, void* orig);
  void preRegPort(gsysPort* port, void* orig);
  vector<gsysHierarchy*> getHierarchyList();
  void saveAllPortValues();
  vector<gsysSignal*> getAllSignals();
  vector<gsysConnection*> getAllConns();
  void showChanges();
  void deHighlightAll();

};
					
#endif
