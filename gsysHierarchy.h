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
    last modified: 2005-01-31
 *****************************************************************************
    Content:
      This class contains all necessary properties for module
      appearance and behaviour.

 *****************************************************************************/



#ifndef GSYS_HIERARCHY_H
#define GSYS_HIERARCHY_H

#include "systemc.h"
#include <QtCore/qpoint.h>
#include <QtWidgets/QGraphicsScene>
#include <vector> 

using namespace std;

class gsysPort;
class gsysSignal;

class gsysHierarchy 
{
  
  public:
  
  gsysHierarchy(char* name=0); 
  ~gsysHierarchy();
  void addPort(gsysPort* port);
  
  bool addNPort(gsysSignal *sig,short initDir);
  bool addEPort(gsysSignal *sig,short initDir);
  bool addSPort(gsysSignal *sig,short initDir);
  bool addWPort(gsysSignal *sig,short initDir);
  
  bool addEPort(gsysSignal *sig, QPoint dest);
  bool addWPort(gsysSignal *sig, QPoint dest);
 
  void addLeftPort(gsysSignal* sig);
  void addRightPort(gsysSignal* sig);

  void sortSidePorts();

  vector<gsysPort*> getNPorts();
  vector<gsysPort*> getEPorts();
  vector<gsysPort*> getSPorts();
  vector<gsysPort*> getWPorts();
  
  vector<gsysHierarchy*> getAdjacentHier();
  vector<gsysPort*> getPorts();
  vector<gsysPort*> getLeftPorts();
  vector<gsysPort*> getRightPorts();
  vector<gsysHierarchy*> getChildren();
  void addChild(gsysHierarchy* child);
  void addAdjacentHier(gsysHierarchy* h);
  sc_module* getReal();
  void setReal(sc_module* realHier);
  void setName(char* name);
  char* getName();
  void setParent(gsysHierarchy* newParent);
  gsysHierarchy* getParent();
   enum moduleType{CHANNEL, INCOMING_CHANNEL, MISC, PE, VIRT_CHANNEL, SEPERATOR};
  
  void setCenterPoint(QPoint* cp);
  QPoint* getCenterPoint();

  void setHierRect(QGraphicsRectItem *hrect);
  QGraphicsRectItem *getHierRect();

  bool hasSidePort(gsysPort* p);
  moduleType getModuleType();

  private:

  vector<gsysPort*> portList;
  vector<gsysPort*> leftPorts;		// linke Portleiste
  vector<gsysPort*> rightPorts;		// rechte Portleiste
  vector<gsysPort*> portsN;
  vector<gsysPort*> portsE;
  vector<gsysPort*> portsS;
  vector<gsysPort*> portsW;
  vector<gsysHierarchy*> adjacentHier;
  
  QGraphicsRectItem *hierRect;
  
  char* name;
  vector<gsysHierarchy*> childList;
  gsysHierarchy* parent;
  sc_module* realHierarchy;
  int portExists(gsysPort* port);
  int childExists(gsysHierarchy* child);
  QPoint* centerPoint;
  moduleType type;

  moduleType determineModuleType();

};



#endif
