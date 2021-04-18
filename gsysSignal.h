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
      This class is the graphical pendent to the SystemC-own signals.
      Values of the corresponding SystemC signal are stored in it and 
      therefore can be demanded at any time.

 *****************************************************************************/



#ifndef GSYS_SIGNAL_H
#define GSYS_SIGNAL_H

#include "systemc.h"
#include <QtCore/qpoint.h>
#include <QtWidgets/QGraphicsScene>

#include <vector> 

class gsysConnection;
class gsysSignalViewer;
class gsysPort;
class gsysHierarchy;

using namespace std;


class gsysSignal 
{
  private:

  bool activated;
  bool highlighted;
  vector<gsysPort*> connPorts;
  vector<vector<QGraphicsItem*> > portAnbindungen;  // Lines of the single ports to the network nodes as CanvasPolygonalItem
  void* realSignal;
  gsysHierarchy* parentModule;
  gsysConnection* parentConn;
  char* name;
  char* oldValue;
  char* sigValue;
  gsysSignalViewer* sigViewWindow;


  public:
  gsysSignal(); 
  ~gsysSignal();
  void* getReal();
  void setReal(void* newSignal);
  void addPort(gsysPort* port);
  vector<gsysPort*> getPorts();
  bool isActivated();
  void setValue(const char* wert);
  char* getValue();
  void saveValue();
  bool sthChanged();
  void activate(gsysPort* p=0);
  void deactivate(gsysPort* p=0);
  void setName(const char* name);
  char* getName();
  void setParent(gsysHierarchy* parent);
  gsysHierarchy* getParent();
  void setConn(gsysConnection* conn);
  gsysConnection* getConn();
  QList<QGraphicsItem*> getCanvasItemList();
  gsysSignalViewer* getSigViewWindow();
  void highlightChanged();
  void deHighlight();
  
};



#endif
