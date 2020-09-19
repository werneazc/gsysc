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

#include "gsysCanvasView.moc"
#include "gsysMain.h"
#include "gsysPort.h"
#include "gsysSignal.h"
#include "gsysRegister.h"
#include "gsysConnection.h"
#include "gsysHierarchy.h"
#include "gsysHierarchyWindow.h"
#include "gsysHierarchyTree.h"


/*
 *  Constructs an object of this class
 */
gsysCanvasView::gsysCanvasView( QCanvas* c, QWidget* parent, const char* name, WFlags f) : 
    QCanvasView(c,parent,name,f)  
    {
      // initialize all global variables
      connShowHierarchy = 0; 
      mmSigPortShow = true;
      mmHierConnShow = false;
      moduleColor = 0;
      moduleWithChild = 0;
      mmHierConnColor = 0;
    }

/*
 *  destructor of this class
 *  nothing has to be done here; Qt frees its widgets itself
 */
gsysCanvasView::~gsysCanvasView() {}

/*
 *  function to check, whether the hierarchy, to which the point p belongs,
 *  is opened
 *    if not then open it
 */
bool gsysCanvasView::foundAndOpenedHier(QPoint p)
{
  vector<gsysHierarchy*> hierList = ((gsysHierarchyWindow*) parentWidget())->hierarchyList;
  for(int i=0; i<hierList.size(); i++)
  {
    #ifdef DEBUG_GSYSC
    cout<<"--++-- point "<<p.x()<<"/"<<p.y()<<":  x-area "<<hierList[i]->getCenterPoint()->x()-(int)ceil((double)((gsysHierarchyWindow*)parentWidget())->moduleWidth*0.5)+1<<"->"<<hierList[i]->getCenterPoint()->x()+(int)floor((double)((gsysHierarchyWindow*)parentWidget())->moduleWidth*0.5)<<";  y-area "<<hierList[i]->getCenterPoint()->y()-(int)ceil((double)((gsysHierarchyWindow*)parentWidget())->moduleWidth*0.5)+1<<"->"<<hierList[i]->getCenterPoint()->y()+(int)floor((double)((gsysHierarchyWindow*)parentWidget())->moduleHeight*0.5)<<endl;
    #endif
    if(hierList[i]->getCenterPoint()->x()-(int)ceil((double)((gsysHierarchyWindow*)parentWidget())->moduleWidth*0.5)+1 < p.x()   &&
       hierList[i]->getCenterPoint()->x()+(int)floor((double)((gsysHierarchyWindow*)parentWidget())->moduleWidth*0.5) > p.x()   &&
       hierList[i]->getCenterPoint()->y()-(int)ceil((double)((gsysHierarchyWindow*)parentWidget())->moduleHeight*0.5)+1 < p.y()   &&
       hierList[i]->getCenterPoint()->y()+(int)floor((double)((gsysHierarchyWindow*)parentWidget())->moduleHeight*0.5) > p.y())
       {
         #ifdef DEBUG_GSYSC
	 cout<<"-+- the point "<<p.x()<<"/"<<p.y()<<" lies in module "<<hierList[i]->getName()<<endl;
	 #endif
         (new gsysMain())->getHierarchyTree()->openWindow(hierList[i]);
	 return true;
       }	 
  }
  return false;
}

/*
 *   function to translate an int-value to a c-string
 */
char* gsysCanvasView::asChar(int zahl)
{
  ostringstream ostr;
  ostr << zahl << ends;
  return (char*) ostr.str().c_str();
}

/*
 *   Event-hadler for Mouse-Press-Events
 *   it checks, whether something is below the cursor, which can be highlighted
 *   if so, it controls the highlighting job
 */
void gsysCanvasView::contentsMousePressEvent(QMouseEvent* e)
{
  if(e->button()==LeftButton)
  {
    QPoint p = e->pos();
    QCanvasItemList l=canvas()->collisions(QRect(p.x()-2,p.y()-2,5,5));
    
    // Something found? else leave function!
    if(l.size()==0) return;

    if(l.size()>=1 && l.front()->rtti()==QCanvasText::RTTI) l.erase(l.begin());
    QCanvasItemList lines;
    QCanvasItemList polygons;
    lines.clear();
    polygons.clear();
    for (QCanvasItemList::Iterator it=l.begin(); it!=l.end(); ++it) {
      if ( (*it)->rtti() == QCanvasLine::RTTI ) {
	lines.push_back(*it);
      }
      if ( (*it)->rtti() == QCanvasPolygon::RTTI ) {
	polygons.push_back(*it);
      }
    }
    #ifdef DEBUG_GSYSC
    std::cout << "in a square of 5x5 point from "<<p.x()<<"/"<<p.y()<<" there are "<<lines.size()<<" lines and "<<polygons.size()<<" polygony found!"<<std::endl;
    #endif
    // hit 2 lines of the same signal?
    if(lines.size()==2)
    {
      QCanvasLine* line0 = (QCanvasLine*) lines[0];
      QCanvasLine* line1 = (QCanvasLine*) lines[1];
      if((line0->startPoint()==line1->startPoint() || 
	  line0->startPoint()==line1->endPoint()   ||
	  line0->endPoint()==line1->startPoint()   ||
	  line0->endPoint()==line1->endPoint())    &&
	 line0->startPoint().x()+line0->endPoint().x() != line1->startPoint().x()+line1->endPoint().x()  &&
	 line0->startPoint().y()+line0->endPoint().y() != line1->startPoint().y()+line1->endPoint().y())
	      lines.erase(++lines.begin());   // zweites Element löschen, da überflüssig
      line0 = 0;
      line1 = 0;
    }
    // processing the found items
    if(lines.size()==1 || polygons.size()==1)
    {
      vector<gsysSignal*> sigList;
      sigList = (new gsysMain())->getRegModule()->getAllSignals();
      for(int i=0; i<sigList.size(); i++)
      {
	QCanvasItemList cil = sigList[i]->getCanvasItemList();
	if((lines.size()==1 && cil.contains(lines[0])) ||
	   (polygons.size()==1 && cil.contains(polygons[0])))
	    {
	      #ifdef DEBUG_GSYSC
	      cout<<"OwnHierarchy: "<<((gsysHierarchyWindow*) parentWidget())->getOwnHier()<<endl;
	      cout<<"Signal-Parent: "<<sigList[i]->getParent()<<endl;
	      #endif
	      if( ((gsysHierarchyWindow*) parentWidget())->getOwnHier() == sigList[i]->getParent() )
	      {
		int sidePortNr=-1;
		int destNr=-1;
		for(int k=0; k<sigList[i]->getPorts().size(); k++) 
		  if(!sigList[i]->getPorts()[k]->getDest().isNull()) destNr=k;
		  
		if(destNr>=0 && (sidePortNr=sidePortExists(sigList[i]->getPorts(),destNr))>=0)
		{
		  #ifdef DEBUG_GSYSC
		  cout<<"--- Signal "<<sigList[i]<<" has "<<sigList[i]->getPorts().size()<<" Ports; Port"<<destNr<<" has Dest"<<endl;
		  #endif
		  if(!sigList[i]->isActivated()) sigList[i]->activate(sigList[i]->getPorts()[sidePortNr]);
		  else sigList[i]->deactivate(sigList[i]->getPorts()[sidePortNr]);
		}
		else
		{
		  #ifdef DEBUG_GSYSC
		  cout<<"--- Signal "<<sigList[i]<<" has "<<sigList[i]->getPorts().size()<<" Ports; Port"<<destNr<<" has Dest"<<endl;
		  #endif
		  if(!sigList[i]->isActivated()) sigList[i]->activate();
		  else sigList[i]->deactivate();
		}
	      }
	    }  
      }
      sigList.clear();
    }
    else
    {
      lines.clear();
      polygons.clear();
      l=canvas()->collisions(QRect(p.x()-1,p.y()-1,3,3));
      if(l.size()>=1 && l.front()->rtti()==QCanvasText::RTTI) l.erase(l.begin());
      for (QCanvasItemList::Iterator it=l.begin(); it!=l.end(); ++it) {
	if ( (*it)->rtti() == QCanvasLine::RTTI ) {
	  lines.push_back(*it);
	}
	if ( (*it)->rtti() == QCanvasPolygon::RTTI ) {
	  polygons.push_back(*it);
	}
      }
      #ifdef DEBUG_GSYSC
      std::cout << "in a square of 3x3 point from "<<p.x()<<"/"<<p.y()<<" there are "<<lines.size()<<" lines and "<<polygons.size()<<" polygons found!"<<std::endl;
      #endif
      // hit 2 lines of the same signal?
      if(lines.size()==2)
      {
	QCanvasLine* line0 = (QCanvasLine*) lines[0];
	QCanvasLine* line1 = (QCanvasLine*) lines[1];
	if((line0->startPoint()==line1->startPoint() || 
	    line0->startPoint()==line1->endPoint()   ||
	    line0->endPoint()==line1->startPoint()   ||
	    line0->endPoint()==line1->endPoint())    &&
	   line0->startPoint().x()+line0->endPoint().x() != line1->startPoint().x()+line1->endPoint().x()  &&
	   line0->startPoint().y()+line0->endPoint().y() != line1->startPoint().y()+line1->endPoint().y())
		lines.erase(++lines.begin());   // delete second element, because needless
	line0 = 0;
	line1 = 0;
      }
      if(lines.size()==1 || polygons.size()==1)
      {
	vector<gsysSignal*> sigList = (new gsysMain())->getRegModule()->getAllSignals();
	for(int i=0; i<sigList.size(); i++)
	{
	  QCanvasItemList cil = sigList[i]->getCanvasItemList();
	  if((lines.size()==1 && cil.contains(lines[0])) ||
	     (polygons.size()==1 && cil.contains(polygons[0])))
	      {
		if( ((gsysHierarchyWindow*) parentWidget())->getOwnHier() == sigList[i]->getParent() ) 
		{
		  int sidePortNr=-1;
		  int destNr=-1;
		  for(int k=0; k<sigList[i]->getPorts().size(); k++) 
		    if(!sigList[i]->getPorts()[k]->getDest().isNull()) destNr=k;
		    
		  if(destNr>=0 && (sidePortNr=sidePortExists(sigList[i]->getPorts(),destNr))>=0)
		  {
		    #ifdef DEBUG_GSYSC
		    cout<<"--- Signal "<<sigList[i]<<" has "<<sigList[i]->getPorts().size()<<" Ports; Port"<<destNr<<" has Dest"<<endl;
		    #endif
		    if(!sigList[i]->isActivated()) sigList[i]->activate(sigList[i]->getPorts()[sidePortNr]);
		    else sigList[i]->deactivate(sigList[i]->getPorts()[sidePortNr]);
		  }
		  else
		  {
		    #ifdef DEBUG_GSYSC
		    cout<<"--- Signal "<<sigList[i]<<" has "<<sigList[i]->getPorts().size()<<" Ports; Port"<<destNr<<" has Dest"<<endl;
		    #endif
		    if(!sigList[i]->isActivated()) sigList[i]->activate();
		    else sigList[i]->deactivate();
		  }
		}  
	      }	
	}
	sigList.clear();
      } 
      else
      {
	lines.clear();
	polygons.clear();
	l=canvas()->collisions(p);
	if(l.size()>=1 && l.front()->rtti()==QCanvasText::RTTI) l.erase(l.begin());
	for (QCanvasItemList::Iterator it=l.begin(); it!=l.end(); ++it) {
	  if ( (*it)->rtti() == QCanvasLine::RTTI ) {
	    lines.push_back(*it);
	  }
	  if ( (*it)->rtti() == QCanvasPolygon::RTTI ) {
	    polygons.push_back(*it);
	  }
	}
	#ifdef DEBUG_GSYSC
	std::cout << "In point "<<p.x()<<"/"<<p.y()<<" there are "<<lines.size()<<" lines and "<<polygons.size()<<" polygons found!"<<std::endl;
	#endif
	// hit 2 lines of the same signal?
	if(lines.size()==2)
	{
	  QCanvasLine* line0 = (QCanvasLine*) lines[0];
	  QCanvasLine* line1 = (QCanvasLine*) lines[1];
	  if((line0->startPoint()==line1->startPoint() || 
	      line0->startPoint()==line1->endPoint()   ||
	      line0->endPoint()==line1->startPoint()   ||
	      line0->endPoint()==line1->endPoint())    &&
	     line0->startPoint().x()+line0->endPoint().x() != line1->startPoint().x()+line1->endPoint().x()  &&
	     line0->startPoint().y()+line0->endPoint().y() != line1->startPoint().y()+line1->endPoint().y())
		  lines.erase(++lines.begin());   // delete second element, because needless
	  line0 = 0;
	  line1 = 0;
	}
	if(lines.size()==1 || polygons.size()==1)
	{
	  vector<gsysSignal*> sigList = (new gsysMain())->getRegModule()->getAllSignals();
	  for(int i=0; i<sigList.size(); i++)
	  {
	    QCanvasItemList cil = sigList[i]->getCanvasItemList();
	    if((lines.size()==1 && cil.contains(lines[0])) ||
	       (polygons.size()==1 && cil.contains(polygons[0])))
	        {
		  if( ((gsysHierarchyWindow*) parentWidget())->getOwnHier() == sigList[i]->getParent() ) 
		  {
		    int sidePortNr=-1;
		    int destNr=-1;
		    for(int k=0; k<sigList[i]->getPorts().size(); k++) 
		      if(!sigList[i]->getPorts()[k]->getDest().isNull()) destNr=k;
		      
		    if(destNr>=0 && (sidePortNr=sidePortExists(sigList[i]->getPorts(),destNr))>=0)
		    {
		      #ifdef DEBUG_GSYSC
		      cout<<"--- Signal "<<sigList[i]<<" has "<<sigList[i]->getPorts().size()<<" Ports; Port"<<destNr<<" has Dest"<<endl;
		      #endif
		      if(!sigList[i]->isActivated()) sigList[i]->activate(sigList[i]->getPorts()[sidePortNr]);
		      else sigList[i]->deactivate(sigList[i]->getPorts()[sidePortNr]);
		    }
		    else
		    {
		      #ifdef DEBUG_GSYSC
		      cout<<"--- Signal "<<sigList[i]<<" has "<<sigList[i]->getPorts().size()<<" Ports; Port"<<destNr<<" has Dest"<<endl;
		      #endif
		      if(!sigList[i]->isActivated()) sigList[i]->activate();
		      else sigList[i]->deactivate();
		    }
		  }  
		}  
	  }
	  sigList.clear();
	} 
      }
    }
    
    if(l.size()>=1 && l.front()->rtti()==QCanvasText::RTTI) l.erase(l.begin());
    // if a hierarchy having children is clicked, then decent in lower hierarchy level
    if(l.size()==1 && l.front()->rtti()==QCanvasRectangle::RTTI)
    {
      if(foundAndOpenedHier(p))
	return;
    }
  }  
}

/*
 *   function to test, whether a (side)port exists, that gives a connection to the above module
 *   that point is not allowed to have the index destNr
 *   if a port exists in the list pl, the index will be returned, else return -1
 */
int gsysCanvasView::sidePortExists(vector<gsysPort*> pl, int destNr)
{
  for(int i=0; i<pl.size(); i++)
  {
    if(i!=destNr && pl[i]->getSignal()!=0 && pl[i]->getSignal()->getParent()!=0)
      if(pl[i]->getSignal()->getParent()!=0 && pl[i]->getSignal()->getParent()->hasSidePort(pl[i])) return i;
  }
  return -1;
}

/*
 *   EventHandler for MouseMove-Events
 *   It tests, whether the cursor is above a module, then 
 *   (if mmHierConnShow is set to true) it highlights all connected modules;
 *   if the cursor is above a port or signal (and mmSigPortShow is set) 
 *   it gives information about the port-/signal-name and -value on bottom of the window
 */
void gsysCanvasView::contentsMouseMoveEvent(QMouseEvent *e)
{
  #ifdef DEBUG_GSYSC
  cout<<"mouseMoveEvent called. Mouse is on position "<<e->x()<<"/"<<e->y()<<endl;
  #endif
  if(mmHierConnShow || mmSigPortShow)
  {
    ((gsysHierarchyWindow*) parentWidget())->lineEditTyp->setText( "" );
    ((gsysHierarchyWindow*) parentWidget())->lineEditWert->setText( "" );
    QPoint p = e->pos();
    ostringstream ostr;
    ostr << p.x() << "/" << p.y() << ends;
    ((gsysHierarchyWindow*) parentWidget())->coord->setText(QString(ostr.str().c_str()));
    QCanvasItemList l=canvas()->collisions(p);
    if(l.size()>=1 && l.front()->rtti()==QCanvasText::RTTI) l.erase(l.begin());
    if(mmSigPortShow && l.size()>=1 && l.front()->rtti()==QCanvasPolygon::RTTI)
    {
      vector<gsysSignal*> sigList = (new gsysMain())->getRegModule()->getAllSignals();
      QCanvasItemList cil;
      for(int i=0; i<sigList.size(); i++)
      {
        cil.clear();
        for(int o=0; o<sigList[i]->getPorts().size(); o++)
	{
	  for(int u=0; u<sigList[i]->getPorts()[o]->getCanvasItems().size(); u++)
	    cil.push_back(sigList[i]->getPorts()[o]->getCanvasItems()[u]);
	  if(cil.contains(l.front()))
	  {
            ((gsysHierarchyWindow*) parentWidget())->lineEditTyp->setText( tr("Port: ").append(sigList[i]->getPorts()[o]->getName()) );
            ((gsysHierarchyWindow*) parentWidget())->lineEditWert->setText( sigList[i]->getPorts()[o]->getValue() );
	  }  
	}
      }
    }
    if(mmHierConnShow && l.size()>=1 && l.front()->rtti()==QCanvasRectangle::RTTI)
    {
      vector<gsysHierarchy*> hierList = (new gsysMain())->getRegModule()->getHierarchyList();
      for(int i=0; i<hierList.size(); i++)
      {
        if(hierList[i]->getHierRect()==l.front())
	{
          ((gsysHierarchyWindow*) parentWidget())->lineEditTyp->setText( tr("Module:") );
          ((gsysHierarchyWindow*) parentWidget())->lineEditWert->setText( "" );
	  connShowHierarchy = hierList[i];
	  hierList[i]->getHierRect()->setBrush(QColor(mmHierConnColor));
	  vector<gsysConnection*> conns = (new gsysMain())->getRegModule()->getAllConns();
	  for(int o=0; o<conns.size(); o++)
	  {
	    if(conns[o]!=0 && conns[o]->getHier1()!=0 && conns[o]->getHier2()!=0 && conns[o]->getHier1()->getHierRect()!=0 && conns[o]->getHier2()->getHierRect()!=0)
	    {
	      if(conns[o]->getHier1()==hierList[i])
	      {
		if((conns[o]->getHier2()!=0 && conns[o]->getHier2()->getParent() != hierList[i])  ||
		   (hierList[i]!=0 && conns[o]->getHier2() != hierList[i]->getParent()))
		  {
		    conns[o]->getHier2()->getHierRect()->setBrush(QColor(mmHierConnColor));
		    conns[o]->getHier2()->getHierRect()->setPen(QPen(QColor(mmHierConnColor)));
		  }  
	      }
	      else 
		if(conns[o]->getHier2()==hierList[i])
		{
		  if((conns[o]->getHier1()!=0 && conns[o]->getHier1()->getParent() != hierList[i])  ||
		     (hierList[i]!=0 && conns[o]->getHier1() != hierList[i]->getParent()))
		    {
		      conns[o]->getHier1()->getHierRect()->setBrush(QColor(mmHierConnColor));
		      conns[o]->getHier1()->getHierRect()->setPen(QPen(QColor(mmHierConnColor)));
		    }  
		}
	    }
	  }
	  conns.clear();
	}
      }
    }
    else
    {
      if(connShowHierarchy!=0)
      {
        // de-highlight ...
	if(connShowHierarchy->getChildren().size()<=0) 
	  connShowHierarchy->getHierRect()->setBrush(QColor(moduleColor));
	else
	  connShowHierarchy->getHierRect()->setBrush(QColor(moduleWithChild));
	vector<gsysConnection*> conns = (new gsysMain())->getRegModule()->getAllConns();
	for(int o=0; o<conns.size(); o++)
	{
	  if(conns[o]!=0 && conns[o]->getHier1()!=0 && conns[o]->getHier2()!=0 && conns[o]->getHier1()->getHierRect()!=0 && conns[o]->getHier2()->getHierRect()!=0)
	  {
	    if(conns[o]->getHier1()==connShowHierarchy)
	    {
	      if((conns[o]->getHier2()!=0 && conns[o]->getHier2()->getParent() != connShowHierarchy)  ||
		 (connShowHierarchy!=0 && conns[o]->getHier2() != connShowHierarchy->getParent()))
		{
		  if(conns[o]->getHier2()->getChildren().size()<=0)
		  {
		    conns[o]->getHier2()->getHierRect()->setBrush(QColor(moduleColor));
		    conns[o]->getHier2()->getHierRect()->setPen(QPen(QColor(moduleColor)));
		  }
		  else
		  {
		    conns[o]->getHier2()->getHierRect()->setBrush(QColor(moduleWithChild));
		    conns[o]->getHier2()->getHierRect()->setPen(QPen(QColor(moduleWithChild)));
		  }
		}  
	    }
	    else 
	      if(conns[o]->getHier2()==connShowHierarchy)
	      {
		if((conns[o]->getHier1()!=0 && conns[o]->getHier1()->getParent() != connShowHierarchy)  ||
		   (connShowHierarchy!=0 && conns[o]->getHier1() != connShowHierarchy->getParent()))
		  {
		    if(conns[o]->getHier1()->getChildren().size()<=0)
		    {
		      conns[o]->getHier1()->getHierRect()->setBrush(QColor(moduleColor));
		      conns[o]->getHier1()->getHierRect()->setPen(QPen(QColor(moduleColor)));
		    }
		    else
		    {
		      conns[o]->getHier1()->getHierRect()->setBrush(QColor(moduleWithChild));
		      conns[o]->getHier1()->getHierRect()->setPen(QPen(QColor(moduleWithChild)));
		    }
		  }  
	      }
	  }
	}
	conns.clear();
      }
    }
    canvas()->update();
  }  
}

/*
 *   method to set the boolean variables and colors for highlighting after reading the config file gsysHViewer.conf
 *   this method should only be called by an gsysHierarchyWindow class
 *	sigShow:  show signal and port values
 *	hvShow :  highlight all connected modules if mouse is over a module
 *	hcColor:  color for highlighting of connected modules
 *	modColor: normal color for modules whithout sub-hierarchy
 *	modWithColor: normal color for modules with sub-hierarchy
 */
void gsysCanvasView::setMM(bool sigShow, bool hcShow, char* hcColor, char* modColor, char* modWithColor)
{
  mmSigPortShow = sigShow;
  mmHierConnShow = hcShow;
  if(moduleColor==0) moduleColor=new char[8];
  strcpy(moduleColor,modColor);
  if(moduleWithChild==0) moduleWithChild=new char[8];
  strcpy(moduleWithChild,modWithColor);
  if(mmHierConnColor==0) mmHierConnColor=new char[8];
  strcpy(mmHierConnColor,hcColor);
}


