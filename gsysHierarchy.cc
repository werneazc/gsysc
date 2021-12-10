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

#include "gsysHierarchy.h"
#include "gsysPort.h"
#include "gsysSignal.h"
 
  /*
   *   constructor of this class
   */
  gsysHierarchy::gsysHierarchy(char* name) 
  {
    // initialize the global variables
    this->name = name;
    realHierarchy = 0;
    centerPoint = new QPoint();
    this->type = determineModuleType();
    portList.clear();
    portsN.clear();
    portsE.clear();
    portsS.clear();
    portsW.clear();
    leftPorts.clear();
    rightPorts.clear();
    
    childList.clear();
  }

  /*
   *   destructor
   */
  gsysHierarchy::~gsysHierarchy()
  {
    portList.clear();
    leftPorts.clear();
    rightPorts.clear();
    portsN.clear();
    portsE.clear();
    portsS.clear();
    portsW.clear();
    delete name; name = 0;
    childList.clear();
    delete parent; parent = 0;
    realHierarchy = 0;		// SystemC own destructor should do the rest
    delete centerPoint; centerPoint = 0;
  }
  
  /*
   *   method to check, whether port exists in the global port list
   */
  int gsysHierarchy::portExists(gsysPort* port)
  {
    for (int i=0; i<portList.size(); i++)
      if (portList[i] == port) return i;
    return -1;
  }
  
  /*
   *   add the port, the parameter points to,
   *   to the list of ports
   */
  void gsysHierarchy::addPort(gsysPort* port)
  {
    int it = portExists(port);
    if (it < 0) portList.push_back(port);
    #ifdef DEBUG_GSYSC
    std::cout<<"+++++++++++\nall ports (portList) of "<<this->getName()<<" ("<<this<<")"<<std::endl;
    for(int i=0; i<portList.size();i++)
      std::cout<<"\tPort:  "<<portList[i]->getName()<<" ("<<portList[i]<<")"<<std::endl;
    std::cout<<"+++++++++++"<<std::endl;  
    #endif
  }
  
  /*
   *   get the list of all known ports
   */
  vector<gsysPort*> gsysHierarchy::getPorts()
  {
    return portList;
  }
 
  /*
   *   add a port that is connected to the signal, sig points to,
   *   and that has this module as parent 
   *   to the upper side of the module
   *   initDir indicates, where to find the next node to connect to
   */
  bool gsysHierarchy::addNPort(gsysSignal *sig,short initDir)
  {
    vector<gsysPort*> sigPorts = sig->getPorts();
    #ifdef DEBUG_GSYSC
    std::cout<<"## addNPort -> Module:  "<<this->getName()<<" ("<<this<<")"<<std::endl;
    for(int o=0; o<sigPorts.size(); o++)
      std::cout<<"\tPort "<<o<<":  "<<sigPorts[o]->getName()<<" ("<<sigPorts[o]<<")"<<std::endl;
    #endif
    for(int i=0; i<sigPorts.size(); i++)
    {
      if (portExists(sigPorts[i])>=0)
      {
        if(sigPorts[i]->getParent()==this)
	      {
          if(portsN.size() == 0 || initDir > 9) portsN.push_back(sigPorts[i]);
	        else portsN.insert(portsN.begin(),1,sigPorts[i]);
	      }
	      sigPorts[i]->setInitDir(initDir);
	      #ifdef DEBUG_GSYSC
	      std::cout<<"\t\tThe port was found under number "<<i<<" ("<<sigPorts[i]->getName()<<";"<<sigPorts[i]<<") and included!"<<std::endl;
	      #endif
	      sigPorts.clear();
	      return true;
      }
    }
    sigPorts.clear();
    return false;
  }
 
  /*
   *   add a port that is connected to the signal, sig points to,
   *   and that has this module as parent 
   *   to the right side of the module
   *   initDir indicates, where to find the next node to connect to
   */
  bool gsysHierarchy::addEPort(gsysSignal *sig,short initDir)
  {
    vector<gsysPort*> sigPorts = sig->getPorts();
    #ifdef DEBUG_GSYSC
    std::cout<<"## addEPort -> Module:  "<<this->getName()<<" ("<<this<<"); Signal "<<sig->getName()<<"("<<sig<<")"<<std::endl;
    for(int o=0; o<sigPorts.size(); o++)
      std::cout<<"\tPort "<<o<<":  "<<sigPorts[o]->getName()<<" ("<<sigPorts[o]<<")"<<std::endl;
    #endif
    for(int i=0; i<sigPorts.size(); i++)
    {
      if (portExists(sigPorts[i])>=0)
      {
        if(sigPorts[i]->getParent()==this)
	      {
          if(portsE.size() == 0 || (initDir > 5 && initDir <= 13)) portsE.push_back(sigPorts[i]);
	        else portsE.insert(portsE.begin(),1,sigPorts[i]);
	      }
	      sigPorts[i]->setInitDir(initDir);
	      #ifdef DEBUG_GSYSC
	      std::cout<<"\t\tThe port was found under number "<<i<<" ("<<sigPorts[i]->getName()<<";"<<sigPorts[i]<<") and included!"<<std::endl;
	      #endif
	      sigPorts.clear();
	      return true;
      }
    }
    sigPorts.clear();
    return false;
  }
 
  /*
   *   add a port that is connected to the signal, sig points to,
   *   and that has this module as parent 
   *   to the lower side of the module
   *   initDir indicates, where to find the next node to connect to
   */
  bool gsysHierarchy::addSPort(gsysSignal *sig,short initDir)
  {
    vector<gsysPort*> sigPorts = sig->getPorts();
    #ifdef DEBUG_GSYSC
    std::cout<<"## addSPort -> Module:  "<<this->getName()<<" ("<<this<<")"<<std::endl;
    for(int o=0; o<sigPorts.size(); o++)
      std::cout<<"\tPort "<<o<<":  "<<sigPorts[o]->getName()<<" ("<<sigPorts[o]<<")"<<std::endl;
    #endif
    for(int i=0; i<sigPorts.size(); i++)
    {
      if (portExists(sigPorts[i])>=0)
      {
        if(sigPorts[i]->getParent()==this)
        {
	        if(portsS.size() == 0 || initDir > 9) portsS.push_back(sigPorts[i]);
	        else portsS.insert(portsS.begin(),1,sigPorts[i]);
	      }
	      sigPorts[i]->setInitDir(initDir);
	      #ifdef DEBUG_GSYSC
	      std::cout<<"\t\tThe port was found under number "<<i<<" ("<<sigPorts[i]->getName()<<";"<<sigPorts[i]<<") and included!"<<std::endl;
	      #endif
	      sigPorts.clear();
	      return true;
      }
    }
    sigPorts.clear();
    return false;
  }
 
  /*
   *   add a port that is connected to the signal, sig points to,
   *   and that has this module as parent 
   *   add a port to the left side of the module
   *   initDir indicates, where to find the next node to connect to
   */
  bool gsysHierarchy::addWPort(gsysSignal *sig,short initDir)
  {
    vector<gsysPort*> sigPorts = sig->getPorts();
    #ifdef DEBUG_GSYSC
    std::cout<<"## addWPort -> Module:  "<<this->getName()<<" ("<<this<<")"<<std::endl;
    for(int o=0; o<sigPorts.size(); o++)
      std::cout<<"\tPort "<<o<<":  "<<sigPorts[o]->getName()<<" ("<<sigPorts[o]<<")"<<std::endl;
    #endif
    for(int i=0; i<sigPorts.size(); i++)
    {
      if (portExists(sigPorts[i])>=0)
      {
        if(sigPorts[i]->getParent()==this)
	{
          if(portsW.size() == 0 || (initDir >= 5 && initDir < 13)) portsW.push_back(sigPorts[i]);
	        else portsW.insert(portsW.begin(),1,sigPorts[i]);
	      }
	      sigPorts[i]->setInitDir(initDir);
	      #ifdef DEBUG_GSYSC
	      std::cout<<"\t\tThe port was found under number "<<i<<" ("<<sigPorts[i]->getName()<<";"<<sigPorts[i]<<") and included!"<<std::endl;
	      #endif
	      sigPorts.clear();
	      return true;
      }
    }
    sigPorts.clear();
    return false;
  }

  /*
   *   add a port that is connected to the signal, sig points to,
   *   and that has this module as parent 
   *   to the right side of the module.
   *   dest points to that point on the canvas, where the connected signal has its other end 
   */
  bool gsysHierarchy::addEPort(gsysSignal *sig, QPoint dest)
  {
    for(int i=0; i<portList.size(); i++)
      for (int o=0; o<sig->getPorts().size(); o++)
        if(portList[i]==sig->getPorts()[o])
	      {
	        #ifdef DEBUG_GSYSC
	        cout<<"### addEPort(2) -> "<<portList[i]<<" ("<<portList[i]->getName()<<") == "<<sig->getPorts()[o]<<" ("<<sig->getPorts()[o]->getName()<<"), therefore to be added!"<<endl;
	        #endif
          if(portList[i]->getParent()==this)
	          portsE.push_back(portList[i]);
	        portList[i]->setDest(dest);
	        return true;
	      }
    return false;	
  }

  /*
   *   add a port that is connected to the signal, sig points to,
   *   and that has this module as parent 
   *   to the left side of the module.
   *   dest points to that point on the canvas, where the connected signal has its other end 
   */
  bool gsysHierarchy::addWPort(gsysSignal *sig, QPoint dest)
  {
    for(int i=0; i<portList.size(); i++)
      for (int o=0; o<sig->getPorts().size(); o++)
        if(portList[i]==sig->getPorts()[o])
	      {
	        #ifdef DEBUG_GSYSC
	        cout<<"### addWPort(2) -> "<<portList[i]<<" ("<<portList[i]->getName()<<") == "<<sig->getPorts()[o]<<" ("<<sig->getPorts()[o]->getName()<<"), therefore to be added!"<<endl;
	        #endif
          if(portList[i]->getParent()==this)
	          portsW.push_back(portList[i]);
	        portList[i]->setDest(dest);
	        return true;
	      }
    return false;	
  }

  /*
   *   add a port that is connected to signal, sig points to, 
   *   to the left side of the canvas.
   *   that ports symbolize a connection to the hierarchy above
   */
  void gsysHierarchy::addLeftPort(gsysSignal* sig)
  {
    for(int i=0; i<portList.size(); i++)
      for (int o=0; o<sig->getPorts().size(); o++)
        if(portList[i]==sig->getPorts()[o])
	      {
	        #ifdef DEBUG_GSYSC
	        cout<<"### addLeftPort -> "<<portList[i]<<" ("<<portList[i]->getName()<<") == "<<sig->getPorts()[o]<<" ("<<sig->getPorts()[o]->getName()<<"), therefore to be added!"<<endl;
	        #endif
          if(portList[i]->getParent()==this)
	          leftPorts.push_back(portList[i]);
	      }
  }

  /*
   *   add a port that is connected to signal, sig points to, 
   *   to the right side of the canvas.
   *   that ports symbolize a connection to the hierarchy above
   */
  void gsysHierarchy::addRightPort(gsysSignal* sig)
  {
    for(int i=0; i<portList.size(); i++)
      for (int o=0; o<sig->getPorts().size(); o++)
        if(portList[i]==sig->getPorts()[o])
	      {
	        #ifdef DEBUG_GSYSC
	        cout<<"### addRightPort -> "<<portList[i]<<" ("<<portList[i]->getName()<<") == "<<sig->getPorts()[o]<<" ("<<sig->getPorts()[o]->getName()<<"), therefore to be added!"<<endl;
	        #endif
                if(portList[i]->getParent()==this)
	          rightPorts.push_back(portList[i]);
	      }
  }
 
  /*
   *   get the list of all ports, that are placed at the left canvas side
   */
  vector<gsysPort*> gsysHierarchy::getLeftPorts()
  {
    return leftPorts;
  }

  /*
   *   get the list of all ports, that are placed at the right canvas side
   */
  vector<gsysPort*> gsysHierarchy::getRightPorts()
  {
    return rightPorts;
  }
 
  /*
   *   set the cental point of the drawn rectangle, that stands for this module
   */
  void gsysHierarchy::setCenterPoint(QPoint* cp)
  {
    centerPoint = cp;
  }

  /*
   *   get the cental point of the drawn rectangle, that stands for this module
   */
  QPoint* gsysHierarchy::getCenterPoint()
  {
    return centerPoint;
  }

  /*
   *   get a list of all children hierarchies (as pointers)
   */
  vector<gsysHierarchy*> gsysHierarchy::getChildren()
  {
    return childList;
  }
  
  /*
   *   method to check, whether the given child exists as child of this hierarchy module
   */
  int gsysHierarchy::childExists(gsysHierarchy* child)
  {
    for (int i=0; i<childList.size(); i++)
      if (childList[i] == child) return i;
    return -1;
  }
  
  /*
   *   add child to the list of children
   */
  void gsysHierarchy::addChild(gsysHierarchy* child)
  {
    int it = childExists(child);
    if (it < 0) childList.push_back(child);
  }
  
  /*
   *   get the corresponding sc_module object to this hierarchy module (as pointer)
   */
  sc_module* gsysHierarchy::getReal()
  {
    return realHierarchy;
  }
  
  /*
   *   set the pointer to the corresponding sc_module object
   */
  void gsysHierarchy::setReal(sc_module* realHier)
  {
    realHierarchy = realHier;
  }
  
  /*
   *   set name of this module
   */
  void gsysHierarchy::setName(char* name)
  {
    this->name = name;
  }

  /*
   *   get the name of this module
   */
  char* gsysHierarchy::getName()
  {
    return name;
  }

  /*
   *   set a pointer to the parent module
   */
  void gsysHierarchy::setParent(gsysHierarchy* newParent)
  {
    parent = newParent;
  }

  /*
   *   get a pointer to the parent module
   */
  gsysHierarchy* gsysHierarchy::getParent()
  {
    return parent;
  }

  /*
   *   get list of all ports that are placed on the upper side
   */
  vector<gsysPort*> gsysHierarchy::getNPorts()
  {
    return portsN;
  }

  /*
   *   get list of all ports that are placed on the right side
   */
  vector<gsysPort*> gsysHierarchy::getEPorts()
  {
    return portsE;
  }

  /*
   *   get list of all ports that are placed on the lower side
   */
  vector<gsysPort*> gsysHierarchy::getSPorts()
  {
    return portsS;
  }

  /*
   *   get list of all ports that are placed on the left side
   */
  vector<gsysPort*> gsysHierarchy::getWPorts()
  {
    return portsW;
  }

  /*
   *   sort the ports on the left and right side of the canvas area,
   *   so that no crossings of signals to module ports will appear
   */
  void gsysHierarchy::sortSidePorts()
  {
    vector<gsysPort*> newPorts;
    newPorts.clear();
    int idx = -1;
    int old = -1;
    int minFound;
    bool sthDone = false;

    do {
      minFound = 100000;
      sthDone = false;
      for (int i=0; i<leftPorts.size(); i++)
      {
        if(leftPorts[i]->hVar < minFound && leftPorts[i]->hVar > old)
	{
	  minFound = leftPorts[i]->hVar;
	  idx = i;
	  sthDone = true;
        }
      }
      if (sthDone) newPorts.push_back(leftPorts[idx]);
      old = minFound;
      idx = -1;
    } while(sthDone);
    leftPorts = newPorts;

    newPorts.clear();
    old = -1;
    do {
      minFound = 100000;
      sthDone = false;
      for (int i=0; i<rightPorts.size(); i++)
      {
        if(rightPorts[i]->hVar < minFound && rightPorts[i]->hVar > old)
	{
	  minFound = rightPorts[i]->hVar;
	  idx = i;
	  sthDone = true;
        }
      }
      if (sthDone) newPorts.push_back(rightPorts[idx]);
      old = minFound;
      idx = -1;
    } while(sthDone);
    rightPorts = newPorts;
  }
 

  /*
   *   set a pointer to the corresponding rectangle
   */
  void gsysHierarchy::setHierRect(QGraphicsRectItem *hrect)
  {
    hierRect = hrect;
  }

  /*
   *   get a pointer to the corresponding rectangle
   */
  QGraphicsRectItem *gsysHierarchy::getHierRect()
  {
    return hierRect;
  }

  /*
   *   method to test, whether this hierarchy module has side ports
   */
  bool gsysHierarchy::hasSidePort(gsysPort* p)
  {
    for(int i=0; i<leftPorts.size(); i++)
    {
      if(leftPorts[i]==p) return true;
    }
    for(int i=0; i<rightPorts.size(); i++)
    {
      if(rightPorts[i]==p) return true;
    }
    return false;
  }

  /*
   *   	Return the corresponding module type of a hierarchy element to identify  
   *	  its relative position in the window
   */
  gsysHierarchy::moduleType gsysHierarchy::determineModuleType()
  {
	string name = (string) getName();
	if (name.find("Channel"))
    if (name.find("Virtual"))
		  return moduleType::VIRT_CHANNEL;
    else 
		  return moduleType::CHANNEL;
	else if (name.find("PE"))
		return moduleType::PE;
	else 
		return moduleType::MISC;
  }

  /*
   *   	Return module type of the hierarchy
   */
  gsysHierarchy::moduleType gsysHierarchy::getModuleType()
  {
	  return type;
  }
