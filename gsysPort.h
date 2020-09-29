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
      This class has nothing to do with the SystemC version of ports, but
      represents the port for gSysC. In this class the port direction property, 
      i.e. type of port, are stored. Furthermore pointer to the connected 
      signal and information about how to connect to the next network node
      are stored.
      
 *****************************************************************************/



#ifndef GSYS_PORT_H
#define GSYS_PORT_H

#include "systemc.h"
#include <Qt/qpoint.h>
#include <Qt3Support/q3canvas.h>
#include <vector>

using namespace std;

class gsysSignal;
class gsysHierarchy;

class gsysPort 
{
  private:

  short portDirection;
  bool endPort;
  gsysSignal* connSignal;
  char* name;
  gsysHierarchy* parent;
  short initialDirection;
  QPoint destination;
  vector<Q3CanvasPolygonalItem*> lines;
  void *realPort;
  bool activated;
  bool highlighted;
  
  
  public:
 
  int hVar;    // auxiliary variables (temporary; not protected!)
 
  gsysPort(short type=0);   // 0: in-Port;  1: out-Port;  2: inout-Port 
  ~gsysPort();
  short getType();
  bool isEndPort();
  void setParent(gsysHierarchy* newParent);
  gsysHierarchy* getParent();
  void setEndPort(bool newPort=true);
  void setValue(char* value);
  char* getValue();
  void saveValue();
  bool sthChanged();
  void setName(char* name);
  char* getName();
  void setInitDir(short dir);
  short getInitDir();
  void setDest(QPoint dest);
  QPoint getDest();
  gsysSignal* getSignal();
  void setSignal(gsysSignal* sig);
  void addLineElem(Q3CanvasPolygonalItem* line);
  vector<Q3CanvasPolygonalItem*> getCanvasItems();
  void setReal(void* real);
  void* getReal();
  void activate();
  void deactivate();
  void highlight();
  void deHighlight();
  
};



#endif
