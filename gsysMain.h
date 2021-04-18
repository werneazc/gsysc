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
      This class is the main class of gSysC. It contains pointer to all
      important classes. For saving memory all members are declared static,
      since gsysMain is created several times, but always refers to the 
      same objects.

 *****************************************************************************/



#ifndef GSYS_MAIN_H
#define GSYS_MAIN_H

#include "systemc.h"

#include <QtWidgets/qlistview.h>
#include <vector>

class gsysConnection;
class gsysHierarchy;
class gsysSimulator;
class gsysHierarchyTree;
class gsysMainGUI;
class gsysRegister;
class gsysBuffer;

class gsysMain
{
  private:

  static gsysMainGUI *mainWindow;
  static gsysRegister *regObj;

  public:
  gsysMain();
  static gsysHierarchyTree* getHierarchyTree();
  static gsysMainGUI* getMainWindow();
  static gsysSimulator* getSimulator();
  static gsysRegister* getRegModule();
  static gsysBuffer* getBufferWdw();
  static void createGUI();
  static void startSimulator(int steps);
  static void refreshBuffer(void* id, int value, int maxValue);
};
		      
		      
#endif
