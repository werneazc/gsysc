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

#include "gsysPort.h"
#include "gsysSignal.h"
#include "gsysConnection.h"
#include "gsysHierarchyWindow.h"
#include "gsysHierarchy.h"
#include "gsysRegister.h"

  /*
   *   constructor
   */
  gsysPort::gsysPort(short type) 
  {
    // initialize global variables
    portDirection = type;
    initialDirection = -1;
    endPort = false;
    connSignal = 0;
    realPort = 0;
    activated = false;
    highlighted = false;
  }

  /*
   *   destructor
   */
  gsysPort::~gsysPort()
  {
    // free pointer variables and lists
    delete connSignal; connSignal = 0;
    delete name; name = 0;
    delete parent; parent = 0;
    lines.clear();
    realPort = 0;   // SystemC must delete the port
  }
 
  /*
   *   set pointer to corresponding SystemC port
   */
  void gsysPort::setReal(void* real)
  {
    realPort = real;
  }

  /*
   *   get pointer to corresponding SystemC port
   */
  void* gsysPort::getReal()
  {
    return realPort;
  }
  
  /*
   *   get type of port (in- (=0), out- (=1) or inout-port (=2))
   */
  short gsysPort::getType() 
  {
    return portDirection;
  }
  
  /*
   *   set the initial direction to give information, where the next routing node can be found
   */
  void gsysPort::setInitDir(short dir)
  {
    initialDirection = dir;
  }

  /*
   *   get the initialil direction for routing
   */
  short gsysPort::getInitDir()
  {
    return initialDirection;
  }
  
  /*
   *   method to test, whether this port is not connected to any signal
   */
  bool gsysPort::isEndPort()
  {
    return endPort;
  }
  
  /*
   *   set varbiable 'endPort', which means the port is not connected to any signal
   */
  void gsysPort::setEndPort(bool newPort)
  {
    endPort = newPort;
  }
  
  /*
   *   set the value of the port
   */
  void gsysPort::setValue(char* myValue)
  {
    #ifdef DEBUG_GSYSC
    cout<<"-- Port("<<this<<")->setValue() of port '"<<name<<"' called with parameter "<<myValue<<std::endl;
    #endif
    if(connSignal!=0) connSignal->setValue(myValue);
  }

  /*
   *   get the port value
   */
  char* gsysPort::getValue()
  {
    #ifdef DEBUG_GSYSC
    if(connSignal!=0) 
      cout<<"oOo port("<<this<<")->getValue():  The connected signal("<<connSignal<<") has value '"<<connSignal->getValue()<<"'"<<endl;
    else
      cout<<"aAa port("<<this<<")->getValue():  No signal connected to this port!"<<endl;
    #endif
    if(connSignal!=0) return connSignal->getValue();
    else return (char*)">>UNDEFINED<<";
  }

  /*
   *   method to save the port value (forward to gsysSignal-object)
   */
  void gsysPort::saveValue()
  {
    if(connSignal!=0) connSignal->saveValue();
  }
  
  /*
   *   method to check, whether the port value has changed
   *   within the last simulation step
   */
  bool gsysPort::sthChanged()
  {
    if(connSignal!=0) return connSignal->sthChanged();
    return false;
  }

  /*
   *   set the port name
   */
  void gsysPort::setName(char* name)
  {
    this->name = name;
  }

  /*
   *   get the port name
   */
  char* gsysPort::getName()
  {
    return name;
  }

  /*
   *   set a pointer to the parent hierarchy module
   */
  void gsysPort::setParent(gsysHierarchy* newParent)
  {
    parent = newParent;
  }

  /*
   *   get a pointer to the parent module
   */
  gsysHierarchy* gsysPort::getParent()
  {
    return parent;
  }

  /*
   *   set the destination point for direct connection to a side port
   */
  void gsysPort::setDest(QPoint dest)
  {
    destination = dest;
  }

  /*
   *   get the destination port for connection to a side port
   *   (if point is 0/0, then no connection to side port is assumed)
   */
  QPoint gsysPort::getDest()
  {
    return destination;
  }

  /*
   *   get connected signal (as pointer)
   */
  gsysSignal* gsysPort::getSignal()
  {
    return connSignal;
  }

  /*
   *   set a pointer to the connected signal
   */
  void gsysPort::setSignal(gsysSignal* sig)
  {
    #ifdef DEBUG_GSYSC
    cout<<"  *** port "<<name<<"("<<this<<") gets connected to signal "<<sig->getName()<<"("<<sig<<")."<<endl;
    #endif
    connSignal = sig;
  }

  /*
   *   add an graphical element to the list of all drawn elements,
   *   that represent the port an its connection to the routing nodes
   */
  void gsysPort::addLineElem(QAbstractGraphicsShapeItem* line)
  {
    lines.push_back(line);
  }

  /*
   *   get the list of graphical representations
   */
  vector<QAbstractGraphicsShapeItem*> gsysPort::getCanvasItems()
  {
    return lines;
  }

  /*
   *   highlight the connected signal after the user clicked on this port
   */
  void gsysPort::activate()
  {
    if(!activated)
    {
      for(int i=0; i<lines.size(); i++)
        if(lines[i]->type() == QGraphicsLineItem::Type)
        {
          QGraphicsLineItem* aktLine = (QGraphicsLineItem*)lines[i];
	  if(highlighted)
	  {
	    aktLine->setZValue(80);
	    if(connSignal!=0 && connSignal->getConn()!=0 && connSignal->getConn()->getParentWindow())
	    {
	      aktLine->setPen(QColor(connSignal->getConn()->getParentWindow()->activeChangedSig));   // violet
	    }
	    else
	    {
	      aktLine->setPen(QColor("#FF00FF"));   // violet
	    }
	  }
	  else
	  {
	    aktLine->setZValue(60);
	    if(connSignal!=0 && connSignal->getConn()!=0 && connSignal->getConn()->getParentWindow())
	    {
	      aktLine->setPen(QColor(connSignal->getConn()->getParentWindow()->activeSignal));   // dark orange
	    }
	    else
	    {
	      aktLine->setPen(QColor("#FF7F00"));   // dark orange
	    }
	  }
	  aktLine->setActive(true);
	  aktLine->scene()->update();
	  aktLine = 0;
        }
      activated = true;
    }
  }

  /*
   *   de-highlight the connected signal
   */
  void gsysPort::deactivate()
  {
    if(activated)
    {
      for(int i=0; i<lines.size(); i++)
        if(lines[i]->type() == QGraphicsLineItem::Type)
        {
          QGraphicsLineItem* aktLine = (QGraphicsLineItem*)lines[i];
	  if(highlighted)
	  {
  	    aktLine->setZValue(40);
	    if(connSignal!=0 && connSignal->getConn()!=0 && connSignal->getConn()->getParentWindow())
	    {
	      aktLine->setPen(QColor(connSignal->getConn()->getParentWindow()->changedSignal));   // red
	    }
	    else
	    {
	      aktLine->setPen(QColor("#FF0000"));   // red
	    }
	  }
	  else
	  {
  	    aktLine->setZValue(20);
	    if(connSignal!=0 && connSignal->getConn()!=0 && connSignal->getConn()->getParentWindow())
	    {
	      aktLine->setPen(QColor(connSignal->getConn()->getParentWindow()->normalSignal));   // black
	    }
	    else
	    {
	      aktLine->setPen(QColor("#000000"));   // black
	    }
	  }
	  aktLine->setActive(true);
	  aktLine->scene()->update();
	  aktLine = 0;
        }
      activated = false;
    }
  }

  /*
   *   highlight the connected signal, because the port/signal-value has changed
   */
  void gsysPort::highlight()
  {
    if(!highlighted)
    {
      for(int i=0; i<lines.size(); i++)
        if(lines[i]->type() == QGraphicsLineItem::Type)
        {
          QGraphicsLineItem* aktLine = (QGraphicsLineItem*)lines[i];
	  if(activated)
	  {
	    aktLine->setZValue(80);
	    if(connSignal!=0 && connSignal->getConn()!=0 && connSignal->getConn()->getParentWindow())
	    {
	      aktLine->setPen(QColor(connSignal->getConn()->getParentWindow()->activeChangedSig));   // violet
	    }
	    else
	    {
	      aktLine->setPen(QColor("#FF00FF"));   // violet
	    }
	  }
	  else
	  {
	    aktLine->setZValue(40);
	    if(connSignal!=0 && connSignal->getConn()!=0 && connSignal->getConn()->getParentWindow())
	    {
	      aktLine->setPen(QColor(connSignal->getConn()->getParentWindow()->changedSignal));   // red
	    }
	    else
	    {
	      aktLine->setPen(QColor("#FF0000"));   // red
	    }
	  }
	  aktLine->setActive(true);
	  aktLine->scene()->update();
	  aktLine = 0;
        }
      highlighted = true;
    }	
  }

  /*
   *   de-highlight the connected signal
   */
  void gsysPort::deHighlight()
  {
    if(highlighted)
    {
      for(int i=0; i<lines.size(); i++)
        if(lines[i]->type() == QGraphicsLineItem::Type)
        {
          QGraphicsLineItem* aktLine = (QGraphicsLineItem*)lines[i];
	  if(activated)
	  {
  	    aktLine->setZValue(60);
	    if(connSignal!=0 && connSignal->getConn()!=0 && connSignal->getConn()->getParentWindow())
	    {
	      aktLine->setPen(QColor(connSignal->getConn()->getParentWindow()->activeSignal));   // orange
	    }
	    else
	    {
	      aktLine->setPen(QColor("#FF7F00"));   // orange
	    }
	  }
	  else
	  {
  	    aktLine->setZValue(20);
	    if(connSignal!=0 && connSignal->getConn()!=0 && connSignal->getConn()->getParentWindow())
	    {
	      aktLine->setPen(QColor(connSignal->getConn()->getParentWindow()->normalSignal));   // black
	    }
	    else
	    {
	      aktLine->setPen(QColor("#000000"));   // black
	    }
	  }
	  aktLine->setActive(true);
	  aktLine->scene()->update();
	  aktLine = 0;
        }
      highlighted = false;	
    }
  }
  

