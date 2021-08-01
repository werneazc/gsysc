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

#include "gsysMain.h"

#include "gsysConnection.h"
#include "gsysHierarchy.h"
#include "gsysSimulator.h"
#include "gsysHierarchyTree.h"
#include "gsysMainGUI.h"
#include "gsysRegister.h"
#include "gsysBuffer.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
 *   thread function to simulate without getting all ressources 
 *   from the simulation controller frontend
 *   In this way it's possible to simulate and still have the possibility to use the frontends,
 *   e.g. for stopping some simulation process.
 */
pthread_mutex_t run_mutex;
void* startThread(void *stepsVoid)
{
  int steps = *((int*) stepsVoid);
  gsysSimulator* simWdw = (new gsysMain())->getSimulator();
  for (int i=1; i<=steps; i++)
  {
    if(simWdw->stopped())
    {
      pthread_exit(NULL);
    }
    pthread_mutex_lock(&run_mutex);
    simWdw->start(1);
    if (simWdw->autoStep->isChecked()) 
    {
      usleep( 100000 * simWdw->intervalSpin->value() );
    }
    pthread_mutex_unlock(&run_mutex);
    if (i==steps-1) simWdw->stop();
  }
  pthread_exit(NULL);
  simWdw = 0;
}
		      

  // global variables of the class gsysMain
  //   pointers to the main window and to the registration class
  gsysMainGUI * gsysMain::mainWindow = 0;
  gsysRegister * gsysMain::regObj = 0;

/* 
 *   constructor
 */
  gsysMain::gsysMain() 
  {
  }

/* 
 *   get pointer to the simulation controller
 *   if it does not exist then it will create it
 */
  gsysSimulator* gsysMain::getSimulator() 
  {
    if (mainWindow == 0) 
      return getMainWindow()->createSimulator();
    else
      return mainWindow->createSimulator();
  }

/*
 *   get registration object (gsysRegister object)
 */
  gsysRegister* gsysMain::getRegModule()
  {
    if (regObj == 0)
      regObj = new gsysRegister();
    return regObj;
  }
  
/*
 *   get pointer to the window with user-defined functionalities
 */
  gsysBuffer* gsysMain::getBufferWdw()
  {
    return getMainWindow()->createBufferWdw();
  }
  
/*
 *   get hierarchyTree window
 */
  gsysHierarchyTree* gsysMain::getHierarchyTree()
  {
    if (mainWindow == 0)
      return getMainWindow()->createHierTree();
    else
      return mainWindow->createHierTree();
  }
  
/*
 *   create and open main window, means start gSysC frontends
 */
  void gsysMain::createGUI()
  {
    if (mainWindow==0)
    {
      mainWindow = new gsysMainGUI();
      getRegModule()->regFinished();
    }
  }

/*
 *   method to start the simulation controller
 */
  void gsysMain::startSimulator(int steps)
  {
    createGUI();
    mainWindow->move(0,0);
    mainWindow->show();
  }
  
/*
 *   get pointer to the main window
 */
  gsysMainGUI* gsysMain::getMainWindow()
  { 
    createGUI();
    return mainWindow; 
  }
  
/*
 *   method to refresh the progressBar visualization
 *   'id' is the identification number of the variable to visualize
 *   'value' is the new value
 *   'maxValue' gives the new maximum of the progressbar
 */
  void gsysMain::refreshBuffer(void* id, int value, int maxValue)
  { 
    getBufferWdw()->refreshBuffer(id,value,maxValue);
  }
