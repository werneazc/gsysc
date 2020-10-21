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

#include "gsysRegister.h"

#include "gsysMain.h"
#include "gsysMainGUI.h"
#include "gsysConnection.h"
#include "gsysHierarchy.h"
#include "gsysSimulator.h"
#include "gsysHierarchyTree.h"
#include "gsysSignal.h"
#include "gsysPort.h"
#include "gsysPortViewer.h"
#include "Qt3Support/q3canvas.h"

  /*
   *   constructor
   */
  gsysRegister::gsysRegister() 
  {
    connList.clear();
    hierarchyList.clear();
    portList.clear();
    notAssignedPorts.clear();
    notAssignedSignals.clear();
    listViewElements.clear();
  }

  /*
   *   destructor
   */
  gsysRegister::~gsysRegister()
  {
    for(int i=0; i<connList.size(); i++)
      { delete connList[i]; connList[i] = 0; }
    for(int i=0; i<hierarchyList.size(); i++)
      { delete hierarchyList[i]; hierarchyList[i] = 0; }
    for(int i=0; i<portList.size(); i++)
      { delete portList[i]; portList[i] = 0; }
    for(int i=0; i<notAssignedSignals.size(); i++)
      { delete notAssignedSignals[i]; notAssignedSignals[i] = 0; }
    for(int i=0; i<notAssignedPorts.size(); i++)
      { delete notAssignedPorts[i]; notAssignedPorts[i] = 0; }
    for(int i=0; i<listViewElements.size(); i++)
      { delete listViewElements[i]; listViewElements[i] = 0; }
    connList.clear();
    hierarchyList.clear();
    portList.clear();
    notAssignedPorts.clear();
    notAssignedSignals.clear();
    listViewElements.clear();
    delete rootLVI; rootLVI = 0;
  }
  
  /*
   *   method to check whether a connection between the hierarchies h1 and h2 is already created
   *   if it exists and is in the list of connection, the index will be returned
   */
  int gsysRegister::connExists(gsysHierarchy* h1,gsysHierarchy* h2)
  {
    #ifdef DEBUG_GSYSC
    std::cout<<"----------\nconnExists("<<h1->getName()<<" ["<<h1<<"], "<<h2->getName()<<" ["<<h2<<"]) called:"<<std::endl;
    #endif
    for (int i=0; i<connList.size(); i++)
    {
      #ifdef DEBUG_GSYSC
      std::cout<<"\t"<<i<<"->  Hier1: "<<connList[i]->getHier1()<<",  Hier2: "<<connList[i]->getHier2()<<std::endl;
      #endif
      if ((connList[i]->getHier1() == h1 && connList[i]->getHier2() == h2) ||
          (connList[i]->getHier1() == h2 && connList[i]->getHier2() == h1))
        { 
	  #ifdef DEBUG_GSYSC
	  std::cout<<"\t\tReturn result:  "<<i<<std::endl;
	  #endif
	  return i;
	}
    }	
    #ifdef DEBUG_GSYSC
    std::cout<<"\t\tReturn result:  -1"<<std::endl;
    #endif
    return -1;	
  }
 
 
  // recursive function for insertion of the hierarchies according to their parent-/child-structure
  // WARNING: It is assumed, that the Root node is already inserted!           =>  hier!=0 (always!)
  void gsysRegister::insertLVI(Q3ListView *parentLVI, gsysHierarchy* hier)
  {
    gsysMain* mainWdw = new gsysMain();
    Q3ListView* lvi = (Q3ListView*) new Q3ListViewItem(parentLVI,hier->getName(),"Modul",mainWdw->getSimulator()->asChar((int) hier));
    listViewElements.push_back( lvi );
    
    int chSize = hier->getChildren().size();
    for (int i=0; i<chSize; i++)
      insertLVI(lvi,(hier->getChildren())[i]);
    
    mainWdw = 0;
    lvi = 0;
  }


  /*
   *   method that is started after the registration process of the programmer
   *   and before starting the simulation.
   *   It has to create all missing relation pointers between the registered elements
   *   and prepare the used lists for usage in gSysC.
   */
  void gsysRegister::regFinished()
  {
    /***** assign signals to the connections *****/
    int portCount = 0;
    int it = 0;
    vector<gsysSignal*>::iterator tempIterator = notAssignedSignals.begin();
    int nasSize = notAssignedSignals.size();
    for (int i=nasSize-1; i>=0; i--)
    {
      portCount = notAssignedSignals[i]->getPorts().size();
      #ifdef DEBUG_GSYSC
      std::cout << "Signal '" << notAssignedSignals[i]->getName() << "' ("<< notAssignedSignals[i]<<"; reell: "<<notAssignedSignals[i]->getReal()<<") hat " << portCount<<" Ports." << std::endl;
      #endif
      if (portCount == 2)
      {
        it = connExists(notAssignedSignals[i]->getPorts()[0]->getParent(),notAssignedSignals[i]->getPorts()[1]->getParent());
	if (it >= 0) 
	{ 
	  connList[it]->addSignal(notAssignedSignals[i]);
	  notAssignedSignals[i]->setConn(connList[it]);    // create back reference
	}
	else
	{
	  connList.push_back(new gsysConnection(notAssignedSignals[i]->getPorts()[0]->getParent(),notAssignedSignals[i]->getPorts()[1]->getParent()));
	  connList[connList.size()-1]->addSignal(notAssignedSignals[i]);
	  notAssignedSignals[i]->setConn(connList[connList.size()-1]);
	}
	notAssignedSignals.erase(tempIterator+i);
	#ifdef DEBUG_GSYSC
	std::cout<<"State after i="<<i<<":"<<std::endl;
	for(int i1=0; i1<notAssignedSignals.size(); i1++) 
	  std::cout<<notAssignedSignals[i1]->getName()<<" ("<<notAssignedSignals[i1]<<")"<<std::endl;
	#endif
      }
      else 
        if (portCount > 2)
        {
	  for (int o=0; o<portCount-1; o++)
	    for (int u=o+1; u<portCount; u++)
	    {
	      it = connExists(notAssignedSignals[i]->getPorts()[o]->getParent(),notAssignedSignals[i]->getPorts()[u]->getParent());
	      if (it >= 0)
	      {
                connList[it]->addSignal(notAssignedSignals[i]);
	        notAssignedSignals[i]->setConn(connList[it]);    // create back reference
              }
              else
              {
                connList.push_back(new gsysConnection(notAssignedSignals[i]->getPorts()[o]->getParent(),notAssignedSignals[i]->getPorts()[u]->getParent()));
	        connList[connList.size()-1]->addSignal(notAssignedSignals[i]);
	        notAssignedSignals[i]->setConn(connList[connList.size()-1]);
              }
	    }
          notAssignedSignals.erase(tempIterator+i);
        }
    }

    /***** complete references *****/
    for (int i=0; i<hierarchyList.size(); i++)
    {
      gsysHierarchy* parentHier = hierarchyList[i]->getParent();
      if (parentHier != 0) parentHier->addChild(hierarchyList[i]);
      parentHier = 0;
    }

    for(int i=0; i<connList.size(); i++)
      for(int o=0; o<connList[i]->getSignals().size(); o++)
        for(int u=0; u<connList[i]->getSignals()[o]->getPorts().size(); u++)
          if(connList[i]->getHier1()==connList[i]->getHier2()->getParent())
	  {
	    if(connList[i]->getSignals()[o]->getPorts()[u]->getParent()==connList[i]->getHier2()) 
	      connList[i]->getSignals()[o]->getPorts()[u]->setSignal(connList[i]->getSignals()[o]);
	  }  
	  else
            if(connList[i]->getHier2()==connList[i]->getHier1()->getParent())
	    {
	      if(connList[i]->getSignals()[o]->getPorts()[u]->getParent()==connList[i]->getHier1()) 
	        connList[i]->getSignals()[o]->getPorts()[u]->setSignal(connList[i]->getSignals()[o]);
	    }
	    else
	      connList[i]->getSignals()[o]->getPorts()[u]->setSignal(connList[i]->getSignals()[o]);

    #ifdef DEBUG_GSYSC
    for(int i=0; i<hierarchyList.size(); i++)
    {
      std::cout << "Module: " << hierarchyList[i]->getName() << " (" << hierarchyList[i] << "); " << std::endl;
      if(hierarchyList[i]->getParent()!=0) std::cout << "\tparent module: "<<hierarchyList[i]->getParent()->getName() << " (" << hierarchyList[i]->getParent() << ")" << std::endl;
      else std::cout << "\tparent module: (" << hierarchyList[i]->getParent() << ")" << std::endl;
      for(int o=0; o<hierarchyList[i]->getChildren().size(); o++)
        std::cout << "\tchild module: "<<hierarchyList[i]->getChildren()[o]->getName() << " ("<< hierarchyList[i]->getChildren()[o]<<")" << std::endl;
    }  
    #endif



    /***** forward values to the graphical elements *****/
    vector<gsysPort*> ports;
    gsysMain* mainWdw = new gsysMain();
    gsysPortViewer* pv = mainWdw->getMainWindow()->getPortViewer();
    rootLVI = (Q3ListView*) new Q3ListViewItem(mainWdw->getHierarchyTree()->tree,"/","Root","0");
    listViewElements.push_back( rootLVI );
    
    for (int i=0; i<hierarchyList.size(); i++)
    {
      ports = hierarchyList[i]->getPorts();
      for (int o=0; o<ports.size(); o++)
      {
        mainWdw->getSimulator()->addPort(ports[o]);
	pv->addPort(ports[o]);
      }
      if(hierarchyList[i]->getParent()==0) insertLVI(rootLVI,hierarchyList[i]);
    }
    
    mainWdw->getHierarchyTree()->setInfoVectors(hierarchyList,connList);

    mainWdw = 0;
    pv = 0;
    rootLVI = 0;

    /***** renew parent of the signals for repairing errors through wrong registration order *****/
    for(int i=0; i<connList.size(); i++)
      for(int o=0; o<connList[i]->getSignals().size(); o++)
      {
        connList[i]->getSignals()[o]->setParent(connList[i]->getSignals()[o]->getPorts()[0]->getParent()->getParent());
      }
        

    /***** output of results or warnings *****/
    if (notAssignedPorts.size() > 0)
    {
      std::cout << "\n##########################################################" << std::endl;
      std::cout <<   "##                Not-assignable ports                  ##" << std::endl;
      std::cout <<   "##########################################################" << std::endl;
      for (int i=0; i<notAssignedPorts.size(); i++)
      {
        std::cout << notAssignedPorts[i]->getName() << std::endl;
      }
    }
    if (notAssignedSignals.size() > 0)
    {
      std::cout << "\n##########################################################" << std::endl;
      std::cout <<   "##               Not-assignable signals                 ##" << std::endl;
      std::cout <<   "##########################################################" << std::endl;
      for (int i=0; i<notAssignedSignals.size(); i++)
      {
        std::cout << notAssignedSignals[i]->getName() << std::endl;
      }
    }
    if (hierarchyList.size() > 0)
    {
      std::cout << "\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
      std::cout <<   "++                 registered modules                   ++" << std::endl;
      std::cout <<   "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
      for (int i=0; i<hierarchyList.size(); i++)
      {
        std::cout << hierarchyList[i]->getName() << "  ("<< hierarchyList[i] << ")" << std::endl;
      }
    }
    if (connList.size() > 0)
    {
      std::cout << "\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
      std::cout <<   "++              registered connections                  ++" << std::endl;
      std::cout <<   "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
      for (int i=0; i<connList.size(); i++)
      {
        std::cout <<"Connection:  "<< connList[i]->getHier1()->getName()<<"->"<<connList[i]->getHier2()->getName() << "  (" << connList[i] <<")" <<std::endl;
	for(int o=0; o<connList[i]->getSignals().size(); o++)
	{
	  std::cout << "\tSignal:  " <<connList[i]->getSignals()[o]->getName() <<"  ("<<connList[i]->getSignals()[o]<<"); Parent: "<<connList[i]->getSignals()[o]->getParent()<<std::endl;
	  for(int u=0; u<connList[i]->getSignals()[o]->getPorts().size(); u++)
	  {
	    std::cout << "\t\tPort:  " <<connList[i]->getSignals()[o]->getPorts()[u]->getName() <<"  ("<<connList[i]->getSignals()[o]->getPorts()[u]<<")"<<std::endl;
	  }
	}
      }
    }
    saveAllPortValues();
  }
  
  /*
   *   method to check, whether the module 'module' exists in the list 'list'
   *   if found, the index will be returned
   */
  int gsysRegister::moduleInList(sc_module* module, vector<gsysHierarchy*> *list)
  {
    for (int i=0; i<list->size(); i++)
    {
      if ((*list)[i]->getReal() == module) 
        return i;
    }	
    return -1;   // if the module was not found ... 
  }
  
  /*
   *   method to check, whether the port 'port' is in list 'list'
   *   if found, the index will be returned
   */
  int gsysRegister::portInList(gsysPort* port, vector<gsysPort*> *list)
  {
    for (int i=0; i<list->size(); i++)
      if ((gsysPort*) ((*list)[i]) == port) 
        return i;
    return -1;   // if the modules was not found ... 
  }

  /*
   *   method to test, whether the (SystemC) port 'port' exists in list 'list'
   *   if found, the index will be returned
   */
  int gsysRegister::portExists(void* port, vector<gsysPort*> *list)
  {
    for(int i=0; i<list->size(); i++)
      if(port == ((*list)[i])->getReal())
        return i;
    return -1;	
  }
  
  /*
   *   method to test, whether the (SystemC) signal 'signal' exists in list 'list'
   *   if found, the index will be returned
   */
  int gsysRegister::signalInList(void* signal, vector<gsysSignal*> *list)
  {
    for (int i=0; i<list->size(); i++)
      if (((gsysSignal*) ((*list)[i]))->getReal() == signal) 
        return i;
    return -1;   // if the modules was not found ... 
  }
  
  /*
   *   method to register the module
   *   'modulePointer' is the original SystemC module
   *   'name' is the name to set for the module
   *   'parent' is the module that stand above of the module
   */
  void gsysRegister::registerModule(sc_module* modulePointer, char* name, sc_module* parent)
  {
    #ifdef DEBUG_GSYSC
    std::cout << "registerModule("<<modulePointer<<","<<name<<","<<parent<<")" << std::endl;
    #endif
    
    int it = moduleInList(modulePointer,&hierarchyList);
    if (it < 0)   // module cannot be found in the list
    {
      gsysHierarchy *newHier = new gsysHierarchy(name);
      newHier->setReal(modulePointer);
      int itx = moduleInList(parent,&hierarchyList);
      if (itx >= 0) newHier->setParent(hierarchyList[itx]);
      else 
      {
        if(parent!=0)
	{
	  gsysHierarchy* parentM = new gsysHierarchy((char*)"unknownParent");
	  parentM->setReal(parent);
	  hierarchyList.push_back(parentM);
	  newHier->setParent(parentM);
	  parentM=0;
	}
	else newHier->setParent(0);
      }	
      hierarchyList.push_back(newHier);
      newHier = 0;
    }
    else
    {
      if(name!=0)  // if the name has to be set
        ((gsysHierarchy*) (hierarchyList[it]))->setName(name);
      if(parent!=0)
      {
        int itx = moduleInList(parent,&hierarchyList);
        if (itx >=0)
          ((gsysHierarchy*) (hierarchyList[it]))->setParent(hierarchyList[itx]);
	else 
	{
	  if(parent!=0)
	  {
	    gsysHierarchy* parentM = new gsysHierarchy((char*)"unknownParent");
	    parentM->setReal(parent);
	    hierarchyList.push_back(parentM);
	    hierarchyList[it]->setParent(parentM);
	    parentM=0;
	  }
	  else hierarchyList[it]->setParent(0);
	}	
      }	
    }	
  }
  
  /*
   *   method to register a port
   *   'realPort' is a pointer to the corresponding SystemC port
   *   'parentPointer' is the pointer to the parental module
   *   'signal' is the connected signal
   */
  void gsysRegister::registerPort(void* realPort, sc_module* parentPointer, void* signal)
  {
    #ifdef DEBUG_GSYSC
    std::cout << "registerPort("<<realPort<<","<<parentPointer<<","<<signal<<")"<<std::endl;
    #endif
 
    int pit = portExists(realPort,&portList);
    gsysPort* port;
    if(pit>=0) port=portList[pit];
    else 
    {
      std::cerr<<"ERROR: The port '" << realPort << "' should be already created, but cannot be found!"<<std::endl;
      port=new gsysPort(2);
      port->setName((char*)"NoName");
      preRegPort(port,realPort);
      std::cerr<<"\tPort is created due to reasons of consistency (as INOUT-Port)!"<<std::endl;
    }  
 
    int it=-1;
    
    if (signal == 0) port->setEndPort(true); 
    else port->setEndPort(false);

    it = moduleInList(parentPointer,&hierarchyList);
    if (parentPointer==0) { notAssignedPorts.push_back(port); }
    else
    {
      if (it >= 0)
      {
        port->setParent(hierarchyList[it]);
        hierarchyList[it]->addPort(port);
      }
      else
      {
        registerModule(parentPointer,(char*)"unknownMod");
	port->setParent(hierarchyList.back());
	hierarchyList.back()->addPort(port);
      }
    }
    it = moduleInList(parentPointer,&hierarchyList);
    #ifdef DEBUG_GSYSC
    cout<<" RegisterPort: ";
    if(it<0) cout<<" parent module is not registered yet!"<<endl;
    else cout<<" the parent module was found"<<endl;
    cout<<"  ==> Modul ";
    if(hierarchyList[it]!=0) cout<<hierarchyList[it]->getName()<<" ("<<hierarchyList[it]<<"; parent: "<<hierarchyList[it]->getParent();
    else cout<<"Root (0)";
    cout<<endl;
    #endif
    int cit = signalInList(signal,&notAssignedSignals);
    if (cit >= 0)
    {
      if (it >= 0 && hierarchyList[it]!=0) notAssignedSignals[cit]->setParent(hierarchyList[it]->getParent());
      else 
      {
        notAssignedSignals[cit]->setParent(0);
      }
      notAssignedSignals[cit]->addPort(port);
    }
    else
    {
      gsysSignal* newSig = new gsysSignal();
      newSig->setReal(signal);
      if (it >= 0) newSig->setParent(hierarchyList[it]);
      else newSig->setParent(0);
      newSig->addPort(port);
      notAssignedSignals.push_back(newSig);
      newSig = 0;
    }
    
    port = 0;
  }
 
  /*
   *   method to register signal (only called at declaration of signal 'orig')
   *   'sig' is a pointer to the already created gsysSignal object
   *   'orig' is a pointer to the corresponding SystemC signal
   *   THIS IS NOT THE REGISTRATION OUT OF THE MODULE CONSTRUCTOR 
   */
  void gsysRegister::registerSignal(gsysSignal* sig, void* orig)
  {
    #ifdef DEBUG_GSYSC
    cout<<"Signal: "<<sig<<",  Original signal: "<<orig<<endl;
    #endif
    int sit = signalInList(orig,&notAssignedSignals);
    if(sit<0)
    {
      sig->setReal(orig);
      notAssignedSignals.push_back(sig);
    }
  }

  /*
   *   method to register port (only called at declaration of port 'orig')
   *   'port' is a pointer to the already created gsysPort object
   *   'orig' is a pointer to the corresponding SystemC port
   *   THIS IS NOT THE REGISTRATION OUT OF THE MODULE CONSTRUCTOR 
   */
  void gsysRegister::preRegPort(gsysPort* port, void* orig)
  {
    #ifdef DEBUG_GSYSC
    cout<<"Port: "<<(void*) port<<",  Original port: "<<orig<<endl;
    #endif
    int pit = portExists(orig,&portList);
    if(pit<0)
    {
      port->setReal(orig);
      portList.push_back(port);
    }
  }

  /*
   *   get a list of all registered hierarchy modules
   */
  vector<gsysHierarchy*> gsysRegister::getHierarchyList()
  {
    return hierarchyList;
  }

  /*
   *   initiate a savage of all port values
   *   this is necessary, when a difference to the last simulation step
   *   should be tested
   */
  void gsysRegister::saveAllPortValues()
  {
    for(int i=0; i<hierarchyList.size(); i++)
      for(int o=0; o<hierarchyList[i]->getPorts().size(); o++)
      {
        hierarchyList[i]->getPorts()[o]->saveValue();
      }
  }

  /*
   *   get a list of all registered signals
   */
  vector<gsysSignal*> gsysRegister::getAllSignals()
  {
    vector<gsysSignal*> aktList;
    aktList.clear();
    for(int i=0; i<connList.size(); i++)
      for(int o=0; o<connList[i]->getSignals().size(); o++)
        aktList.push_back(connList[i]->getSignals()[o]);
    return aktList;	
  }

  /*
   *   highlight all changed signals
   */
  void gsysRegister::showChanges()
  {
    for(int i=0; i<connList.size(); i++)
      for(int o=0; o<connList[i]->getSignals().size(); o++)
      {
        connList[i]->getSignals()[o]->highlightChanged();
      }	
    saveAllPortValues();  
  }
 
  /*
   *   de-highlight all signals
   */
  void gsysRegister::deHighlightAll()
  {
    for(int i=0; i<connList.size(); i++)
      for(int o=0; o<connList[i]->getSignals().size(); o++)
      {
        connList[i]->getSignals()[o]->deHighlight();
      }	
  }

  /*
   *   give a new name to the port with SystemC port 'realPort'
   */
  void gsysRegister::rename_port(void* realPort, char* name)
  {
    int pit = portExists(realPort,&portList);
    if(pit>=0)
      portList[pit]->setName(name);
  }

  /*
   *   give a new name to the signal with SystemC signal 'realSignal'
   */
  void gsysRegister::rename_sig(void* realSignal, char* name)
  {
    int sit = signalInList(realSignal,&notAssignedSignals);
    if(sit>=0)
      notAssignedSignals[sit]->setName(name);
  }
 
  /*
   *   add a buffer variable, to show on the bufferWindow
   *   'id' is an unified identifier
   *   'name' is the name of this variable (appears as caption in the bufferWindow)
   *   'percUse' indicated, whether a percentage number should be shown
   */
  void gsysRegister::addBuffer(void* id, char* name, bool percUse)
  {
    bool found=false;
    for(int i=0; i<bufferIDList.size(); i++)
      if(bufferIDList[i]==id) found=true;
    if(!found)
    {
      bufferIDList.push_back(id);
      bufferNameList.push_back(name);
      bufferPercList.push_back(percUse);
    }
  }

  /*
   *   get a list of the IDs of all registered buffer variables 
   */
  vector<void*> gsysRegister::getBufferIDs()
  {
    return bufferIDList;
  }

  /*
   *   get a list of the names of all registered buffer variables
   */
  vector<char*> gsysRegister::getBufferNames()
  {
    return bufferNameList;
  }

  /*
   *   get a list of the percUse-flags of all buffer variables
   */
  vector<bool> gsysRegister::getBufferPercs()
  {
    return bufferPercList;
  }

  /*
   *   get a list of all known connections
   */
  vector<gsysConnection*> gsysRegister::getAllConns()
  {
    return connList;
  }
  
