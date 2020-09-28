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
      This class is a collection of gsysSignal objects.
      A connection is a point-to-point link between two modules.
      Main idea for doing this is, that for an eventually later 
      reimplementation using more intelligent structure placement, 
      first only the connections have to be considered. The more
      detailed signal placement is done in a second step.

 *****************************************************************************/



#ifndef GSYS_CONN_H
#define GSYS_CONN_H

#include "systemc.h"
#include <q3canvas.h>
#include <vector> 

using namespace std;

class gsysHierarchy;
class gsysHierarchyWindow;
class gsysSignal;

class gsysConnection 
{
  private:
  
  vector<gsysSignal*> signalList;
  vector<Q3CanvasPolygonalItem*> verlauf;
  gsysHierarchy* anfang;
  gsysHierarchy* ende;
  QPoint* node1;
  QPoint* node2;
  gsysHierarchyWindow* parentWindow;
  bool activated;
  bool highlighted;
  bool rectHasColor(QRect rect, QColor col);

  
  public:
 
  gsysHierarchy* getHier1();
  gsysHierarchy* getHier2();
  QPoint* getNode1();
  QPoint* getNode2();
  void setNode1(QPoint* p);
  void setNode2(QPoint* p);
  gsysConnection(gsysHierarchy* hier1, gsysHierarchy* hier2); 
  ~gsysConnection();
  vector<gsysSignal*> getSignals();
  void addSignal(gsysSignal* signal);
  void addTraceElem(Q3CanvasPolygonalItem* pol);
  void activate();
  void deactivate();
  void highlight();
  void deHighlight();
  gsysHierarchyWindow* getParentWindow();
  void setParentWindow(gsysHierarchyWindow* parWdw);
};

#endif
