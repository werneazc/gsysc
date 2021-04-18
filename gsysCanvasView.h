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

/**************************************************************************
    Author: Christian J. Eibl
    last modified: 2005-01-31
 **************************************************************************
    Content:
      This file is a subclass of the Qt implemented class QCanvasView. 
      With deriving this class, overloading and additional 
      implementation of functions like mousePressEvent is
      possible. Hence event handling for the mouse can be realized.

 *************************************************************************/


#include <iostream>
#include <vector>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsView>
#include <QtGui/QMouseEvent>

#include <stdio.h>
#include <sstream>

class gsysHierarchy;
class gsysPort;

using namespace std;

class gsysCanvasView : public QGraphicsView
{
  // Objects can only be instatiated over friend classes
  friend class gsysHierarchyWindow;
  
  Q_OBJECT

  private:

  bool foundAndOpenedHier(QPoint p);
  gsysHierarchy *connShowHierarchy;
  bool mmSigPortShow;
  bool mmHierConnShow;
  char* moduleColor;
  char* moduleWithChild;
  char* mmHierConnColor;
  int sidePortExists(vector<gsysPort*> pl, int destNr);
  
  gsysCanvasView(QGraphicsScene* viewing, QWidget* parent, const char* name=0, Qt::WindowFlags f=0);
  ~gsysCanvasView();
  char* asChar(int zahl);
  void setMM(bool sigShow, bool hcShow, char* hcColor, char* modColor, char* modWithColor);

  protected:
  // functions for deriving in order to get MouseEvent handling
  void mousePressEvent(QMouseEvent* e);
  void mouseMoveEvent(QMouseEvent* e);
};

