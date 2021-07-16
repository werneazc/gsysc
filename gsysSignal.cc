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

#include "gsysSignal.h"
#include "gsysMain.h"
#include "gsysMainGUI.h"
#include "gsysRegister.h"
#include "gsysConnection.h"
#include "gsysSignalViewer.h"
#include "gsysPort.h"
#include "gsysHierarchy.h"
#include "gsysHierarchyTree.h"

  /*
   *   constructor
   */
  gsysSignal::gsysSignal() 
  {
    activated = false;
    highlighted = false;
    connPorts.clear();
    portAnbindungen.clear();
    realSignal = 0;
    parentModule = 0;
    parentConn = 0;
    name = (char*)"";
    oldValue = 0;
    sigValue = 0;
    sigViewWindow = 0;
  }

  /*
   *   destructor
   */
  gsysSignal::~gsysSignal()
  {
    for(int i=0; i<connPorts.size(); i++)
      { delete connPorts[i]; connPorts[i] = 0; }
    connPorts.clear();
    portAnbindungen.clear();
    realSignal = 0;
    delete parentModule; parentModule = 0;
    delete parentConn; parentConn = 0;
    delete name; name = 0;
    delete oldValue; oldValue = 0;
    delete sigValue; sigValue = 0;
    delete sigViewWindow; sigViewWindow = 0;
  }
  
  /*
   *   method to test, whether this signal is highlighted because of user click
   */
  bool gsysSignal::isActivated()
  {
    return activated;
  }
  
  /*
   *   get the signal value
   */
  char* gsysSignal::getValue()
  {
    if (this->sigValue == NULL)
    {
      #ifdef DEBUG_GSYS
      cout<<"gsysSignal("<<this<<")->getValue() == '' (undefined)"<<endl;
      #endif
      return((char*)">>UNDEFINED<<");
    }  
    #ifdef DEBUG_GSYSC
    cout<<"gsysSignal("<<this<<")->getValue() (sigValue->"<<&(this->sigValue)<<") gives value '"<<this->sigValue<<"'"<<endl;
    #endif
    return this->sigValue;
  }
  
  /*  
   *   set the signal value
   */
  void gsysSignal::setValue(const char* wert)
  {
    if (this->sigValue != NULL)
    	{ delete this->sigValue; this->sigValue = NULL; }
    this->sigValue = new char[strlen(wert)+1];
    strcpy(this->sigValue, wert);
    
    #ifdef DEBUG_GSYSC
    cout<<"Signal("<<this<<")->setValue() (sigValue->"<<&(this->sigValue)<<"):\tgiven parameter: '"<<wert<<"', internal after assignment: '"<<this->sigValue<<"'"<<endl;
    #endif
  }

  /*
   *   save the value in another variable
   */
  void gsysSignal::saveValue()
  {
    if (oldValue != NULL)
    	{ delete oldValue; oldValue = NULL; }
    #ifdef DEBUG_GSYSC
    cout<<"  saveValue called"<<endl;
    #endif
    if(sigValue!=NULL)
    {
      oldValue = new char[strlen(sigValue)+1];
      strcpy(oldValue, sigValue);
    }
  }

  /*
   *   method to check, whether the value was changed in the last simulation step
   */
  bool gsysSignal::sthChanged()
  {
    if (oldValue!=NULL && sigValue!=NULL)
      return (strcmp(oldValue,sigValue)!=0);
    return false;  
  }
  
  /*
   *   hightlight the signal because of user click
   */
  void gsysSignal::activate(gsysPort* p)
  {
    // graphisches Aktivieren ...
    #ifdef DEBUG_GSYSC
    cout<<"\tSignal "<<name<<"("<<this<<") should be activated!"<<endl;
    #endif
    if(!activated)
    {
      // deactivate other signals
      vector<gsysSignal*> sigList = (new gsysMain())->getRegModule()->getAllSignals();
      for(int i=0; i<sigList.size(); i++)
        if(sigList[i]->isActivated()) sigList[i]->deactivate(p);
      // activate current signal
      for(int i=0; i<connPorts.size(); i++)
        if(connPorts[i]!=p) connPorts[i]->activate();
      parentConn->activate();	
      activated = true;
      getSigViewWindow();		// let sigViewWindow be created
      for(int i=0; i<connPorts.size(); i++)
      {
        sigViewWindow->lineEditList[2*i]->setText(connPorts[i]->getParent()->getName());		// Module name
        sigViewWindow->lineEditList[2*i+1]->setText(connPorts[i]->getName());			// Module port
      }
      sigViewWindow->lineEdit3->setText(this->getName());			// Signal name
      sigViewWindow->lineEdit4->setText(this->getValue());			// Signal value
      sigViewWindow->move(0,(new gsysMain())->getMainWindow()->geometry().y()+28);
      sigViewWindow->show();
      sigList.clear();
    }
  }
  
  /*
   *   de-highlight the signal
   */
  void gsysSignal::deactivate(gsysPort* p)
  {
    // graphical deactivation ...
    #ifdef DEBUG_GSYSC
    cout<<"\tDas Signal "<<name<<"("<<this<<") soll deaktiviert werden!"<<endl;
    #endif
    if(activated)
    {
      // activate current signal
      for(int i=0; i<connPorts.size(); i++)
        if(connPorts[i]!=p) connPorts[i]->deactivate();
      parentConn->deactivate();	
      activated = false;
      if(sigViewWindow!=0) sigViewWindow->hide();
    }
  }

  /*
   *   highlight the signal, because the value has changed
   */
  void gsysSignal::highlightChanged()
  {
    #ifdef DEBUG_GSYSC
    cout<<"\tSignal "<<name<<" should be highlighted: "<<sthChanged()<<"; !highlighted="<< !highlighted<<endl;
    #endif
    if(sthChanged() && !highlighted)
    {
      #ifdef DEBUG_GSYSC
      cout<<"\t\tIF-condition fulfilled..."<<endl;
      #endif
      (new gsysMain())->getHierarchyTree()->openWindow(this->getParent());
      // activate current signal 
      for(int i=0; i<connPorts.size(); i++)
        connPorts[i]->highlight();
      parentConn->highlight();	
      highlighted = true;
    }
    else
      if(highlighted && !sthChanged())
      {
        #ifdef DEBUG_GSYSC
        cout<<"\t\tIF-condition NOT fulfilled..."<<endl;
        #endif
	      // Remove highlighting of the current signal
	      for(int i=0; i<connPorts.size(); i++)
	        connPorts[i]->deHighlight();
	      parentConn->deHighlight();	
	      highlighted = false;
      }  
    if(sigViewWindow!=0) {
      sigViewWindow->lineEdit4->setText(this->getValue());			// Signal value
    }
  }

  /*
   *   de-highlight the signal
   */
  void gsysSignal::deHighlight()
  {
    for(int i=0; i<connPorts.size(); i++)
      connPorts[i]->deHighlight();
    parentConn->deHighlight();	
    highlighted = false;
  }
  
  /*
   *   set name of signal
   */
  void gsysSignal::setName(const char* name)
  {
    this->name = (char*)name;
  }

  /*
   *   get name of signal
   */
  char* gsysSignal::getName()
  {
    return name;
  }

  /*
   *   get a pointer to the corresponding SystemC signal
   */
  void* gsysSignal::getReal()
  {
    return realSignal;
  }

  /*
   *   set the pointer to the SystemC signal
   */
  void gsysSignal::setReal(void* newSignal)
  {
    realSignal = newSignal;
  }

  /*
   *   set a pointer to the parental hierarchy module
   */
  void gsysSignal::setParent(gsysHierarchy* parent)
  {
    parentModule = parent;
  }

  /*
   *   get pointer to the parent module
   */
  gsysHierarchy* gsysSignal::getParent()
  {
    return parentModule;
  }
  
  /*
   *   add a port to the list of ports the signal is connected to
   */
  void gsysSignal::addPort(gsysPort* port)
  {
    connPorts.push_back(port);
  }
 
  /*
   *   get a list of all ports that are connected to this signal
   */
  vector<gsysPort*> gsysSignal::getPorts()
  {
    return connPorts;
  }

  /*
   *   set a pointer to the connection, this signal belongs to
   */
  void gsysSignal::setConn(gsysConnection* conn)
  {
    parentConn = conn;
  }

  /*
   *   get the connection this signal belongs to
   */
  gsysConnection* gsysSignal::getConn()
  {
    return parentConn;
  }

  /*
   *   get list of canvas items that represent this signal
   */
  QList<QGraphicsItem*> gsysSignal::getCanvasItemList()
  {
    QList<QGraphicsItem*> qcil;
    qcil.clear();
    for(int i=0; i<connPorts.size(); i++)
      for(int o=0; o<connPorts[i]->getCanvasItems().size(); o++)
        qcil.push_back((QGraphicsItem*)connPorts[i]->getCanvasItems()[o]);
    return qcil;
  }

  /*
   *   get the signalViewer window that belongs to this signal
   *   it contains and shows the signal properties
   */
  gsysSignalViewer* gsysSignal::getSigViewWindow()
  {
    if(sigViewWindow==0) sigViewWindow=new gsysSignalViewer((QWidget*) parentConn->getParentWindow(),(int) connPorts.size());
    return sigViewWindow;
  }

