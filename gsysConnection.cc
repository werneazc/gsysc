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
#include "gsysConnection.h"
#include "gsysHierarchyWindow.h"
#include "gsysHierarchy.h"
#include "gsysCanvasView.h"
#include "gsysRegister.h"
  
  /*
   *    constructor of this class
   */
  gsysConnection::gsysConnection(gsysHierarchy* hier1, gsysHierarchy* hier2) 
  {
    // initialize all global variables
    signalList.clear();
    verlauf.clear();
    anfang = hier1;
    ende = hier2;
    node1 = new QPoint();
    node2 = new QPoint();
    parentWindow = 0;
    activated = false;
    highlighted = false;
  }

  /*
   *   destructor of class
   */
  gsysConnection::~gsysConnection()
  {
    // free all global pointer variables and lists
    delete anfang; anfang =0;
    delete ende; ende = 0;
    delete node1; node1 = 0;
    delete node2; node2 = 0;
    delete parentWindow; parentWindow = 0;
    for(int i=0; i<signalList.size(); i++)
      { delete signalList[i]; signalList[i] = 0; }
    signalList.clear();
    verlauf.clear();
  }

  /*
   *   get pointer to first hierarchy that is connected
   *   (one side of the connection)
   */
  gsysHierarchy* gsysConnection::getHier1()
  {
    return anfang;
  }
  
  /*
   *   get pointer to second hierarchy that is connected
   *   (other side of the connection)
   */
  gsysHierarchy* gsysConnection::getHier2()
  {
    return ende;
  }
  
  /*
   *   read list of signals that are bundeled in the connection
   */
  vector<gsysSignal*> gsysConnection::getSignals()
  {
    return signalList;
  }
  
  /*
   *   add signal to the connection
   */
  void gsysConnection::addSignal(gsysSignal* signal)
  {
    signalList.push_back(signal);
  }
  
  /*
   *   method to test, whether the given rectangle rect has color col
   */
  bool gsysConnection::rectHasColor(QRect rect,QColor col)
  {
    QCanvasItemList cil = parentWindow->canvasView->canvas()->collisions(rect);
    for(int i=0; i<cil.count(); i++)
      if(cil[i]->rtti() == (new QCanvasLine(new QCanvas()))->rtti())
        if(((QCanvasLine*)cil[i])->brush().color().rgb() == col.rgb()) return true;
    return false;  
  }
  
  /*
   *   highlight the connection parts because the user clicked 
   *   a port or signal, that belongs to this connection
   */
  void gsysConnection::activate()
  {
    if(!activated)
    {
      QCanvasRectangle* aktRect = 0;
      QCanvasLine* aktLine = 0;
      for(int i=0; i<verlauf.size(); i++)
      {
	if(verlauf[i]->rtti() == (new QCanvasLine(new QCanvas()))->rtti())
	{
	  aktLine = (QCanvasLine*) verlauf[i];
	  if(highlighted)
	  {
	    if(parentWindow!=0)
	    {
	      aktLine->setZ(80);
	      aktLine->setBrush(QColor(parentWindow->activeChangedSig));
	      aktLine->setPen(QPen(QColor(parentWindow->activeChangedSig),3));
	    }
	    else
	    {
	      cerr << "ERROR: \t 'parentWindow' not set in connection " << this << endl;
	    }
	  }
	  else
	  {
	    if(parentWindow!=0)
	    {
	      aktLine->setZ(60);
	      aktLine->setBrush(QColor(parentWindow->activeSignal));
	      aktLine->setPen(QPen(QColor(parentWindow->activeSignal),3));
	    }
	    else
	    {
	      cerr << "ERROR: \t 'parentWindow' not set in connection " << this << endl;
	    }
	  }
	  aktLine->setActive(true);
	  aktLine->canvas()->update();  
	}
	if(verlauf[i]->rtti() == (new QCanvasRectangle(new QCanvas()))->rtti())
	{
	  aktRect = (QCanvasRectangle*) verlauf[i];
	  if(parentWindow != 0)
	  {	  
  	    if(aktRect->brush().color().rgb() == QColor(parentWindow->changedSignal).rgb()) 
	    {
	      aktRect->setZ(160);
	      aktRect->setBrush(QColor(parentWindow->activeChangedSig));
	      aktRect->setPen(QPen(QColor(parentWindow->activeChangedSig),3));
	    }
	    else
	    {
	      aktRect->setZ(140);
	      aktRect->setBrush(QColor(parentWindow->activeSignal));
	      aktRect->setPen(QPen(QColor(parentWindow->activeSignal),3));
	    }
	  }  
	  else
	  {  
	    cerr << "ERROR: \t 'parentWindow' not set in connection " << this << endl;
	  }  
	  aktRect->setActive(true);
	  aktRect->canvas()->update();  
	}
      }
      aktRect = 0;
      aktLine = 0;
      activated = true;
    }
  }

  /*
   *   de-highlight the connection parts
   */
  void gsysConnection::deactivate()
  {
    if(activated)
    {
      for(int i=0; i<verlauf.size(); i++)
      {
	if(verlauf[i]->rtti() == (new QCanvasLine(new QCanvas()))->rtti())
	{
	  QCanvasLine* aktLine = (QCanvasLine*) verlauf[i];
	  if(parentWindow != 0)
	    if(highlighted)
	    {
	      aktLine->setZ(40);
	      aktLine->setBrush(QColor(parentWindow->changedSignal));
	      aktLine->setPen(QPen(QColor(parentWindow->changedSignal),3));
	    }
	    else
	    {
	      aktLine->setZ(20);
	      aktLine->setBrush(QColor(parentWindow->normalSignal));
	      aktLine->setPen(QPen(QColor(parentWindow->normalSignal),3));
	    }
	  else  
	  {	  
	    cerr << "ERROR: \t 'parentWindow' not set in connection " << this << endl;
	  }
	  aktLine->setActive(true);
	  aktLine->canvas()->update();  
	  aktLine = 0;
	}
	if(verlauf[i]->rtti() == (new QCanvasRectangle(new QCanvas()))->rtti())
	{
	  QCanvasRectangle* aktRect = (QCanvasRectangle*) verlauf[i];
	  
	  if(parentWindow != 0)
	  {	  
	    // Check whether any signal is highlighted (through click) and/or changed
            QCanvasItemList cil = parentWindow->canvasView->canvas()->collisions(aktRect->rect());
	    bool rectActivated = false;
	    bool rectHighlighted = false;
            for(int i=0; i<cil.count(); i++)
	    {	    
              if(cil[i]->rtti() == (new QCanvasLine(new QCanvas()))->rtti())
	      {	      
                if(((QCanvasLine*)cil[i])->brush().color().rgb() == QColor(parentWindow->activeChangedSig).rgb()) 
		{
		  rectActivated = true;
		  rectHighlighted = true;
		  break;
		}
		else if(((QCanvasLine*)cil[i])->brush().color().rgb() == QColor(parentWindow->activeSignal).rgb())
		     {
		       rectActivated = true;
		     }
		     else if(((QCanvasLine*)cil[i])->brush().color().rgb() == QColor(parentWindow->changedSignal).rgb())
			  {
		            rectHighlighted = true;
			  }
	      }	
	    }  
	    if(rectActivated && rectHighlighted)
	    {
	      aktRect->setZ(160);
	      aktRect->setBrush(QColor(parentWindow->activeChangedSig));
	      aktRect->setPen(QPen(QColor(parentWindow->activeChangedSig),3));
	    }
	    else
	      if(rectHighlighted)
	      {
		aktRect->setZ(120);
	        aktRect->setBrush(QColor(parentWindow->changedSignal));
	        aktRect->setPen(QPen(QColor(parentWindow->changedSignal),3));
	      }	
	      else
	        if(rectActivated)
	        {
		  aktRect->setZ(140);
	          aktRect->setBrush(QColor(parentWindow->activeSignal));
	          aktRect->setPen(QPen(QColor(parentWindow->activeSignal),3));
	        }	
	        else
		{
		  aktRect->setZ(100);
	          aktRect->setBrush(QColor(parentWindow->normalNode));
	          aktRect->setPen(QPen(QColor(parentWindow->normalNode),3));
	        } 
	  }
	  else
	  {	  
	    cerr << "ERROR: \t 'parentWindow' not set in connection " << this << endl;
	  }  
	  aktRect->setActive(true);
	  aktRect->canvas()->update();  
	  aktRect = 0;
	}
      }
      activated = false;
    }
  }

  /*
   *   highlight the connection parts, because a signal value has changed
   */
  void gsysConnection::highlight()
  {
    if(!highlighted)
    {
      #ifdef DEBUG_GSYSC
      cout << "CONN-highlight in " << this << ": \t parentWindow=" << parentWindow << endl << endl;
      #endif
      QCanvasRectangle* aktRect = 0;
      QCanvasLine* aktLine = 0;
      for(int i=0; i<verlauf.size(); i++)
      {
	if(verlauf[i]->rtti() == (new QCanvasLine(new QCanvas()))->rtti())
	{
	  aktLine = (QCanvasLine*) verlauf[i];
	  if (parentWindow != 0)
	  { 
	    if(activated)
	    {
	      aktLine->setZ(80);
	      aktLine->setBrush(QColor(parentWindow->activeChangedSig));
	      aktLine->setPen(QPen(QColor(parentWindow->activeChangedSig),3));
	    }
	    else
	    {
	      aktLine->setZ(40);
	      aktLine->setBrush(QColor(parentWindow->changedSignal));
	      aktLine->setPen(QPen(QColor(parentWindow->changedSignal),3));
	    }
	  }
	  else
	  {
	    cerr << "ERROR: \t 'parentWindow' not set in connection " << this << endl;
          }
	  aktLine->setActive(true);
	  aktLine->canvas()->update();  
	}
	if(verlauf[i]->rtti() == (new QCanvasRectangle(new QCanvas()))->rtti())
	{
	  aktRect = (QCanvasRectangle*) verlauf[i];
	  #ifdef DEBUG_GSYSC
	  cout << "RECT(high) i=" << i << ";  Coords: (" << aktRect->rect().x() << "," << aktRect->rect().y() << ");  Color=" << aktRect->brush().color().rgb() << "  (activeSignal-color: " << QColor(parentWindow->activeSignal).rgb() << ")" << endl;
	  #endif
	  if(parentWindow != 0)
	  {
	    // Check whether any signal is highlighted (through click) and/or changed
            QCanvasItemList cil = parentWindow->canvasView->canvas()->collisions(aktRect->rect());
	    bool rectActivated = false;
	    bool rectHighlighted = false;
            for(int i=0; i<cil.count(); i++)
	    {	    
              if(cil[i]->rtti() == (new QCanvasLine(new QCanvas()))->rtti())
	      {	      
                if(((QCanvasLine*)cil[i])->brush().color().rgb() == QColor(parentWindow->activeChangedSig).rgb()) 
		{
		  rectActivated = true;
		  rectHighlighted = true;
		  break;
		}
		else if(((QCanvasLine*)cil[i])->brush().color().rgb() == QColor(parentWindow->activeSignal).rgb())
		     {
		       rectActivated = true;
		     }
		     else if(((QCanvasLine*)cil[i])->brush().color().rgb() == QColor(parentWindow->changedSignal).rgb())
			  {
		            rectHighlighted = true;
			  }
	      }	
	    }  
	    if(rectActivated && rectHighlighted)
	    {
	      aktRect->setZ(160);
	      aktRect->setBrush(QColor(parentWindow->activeChangedSig));
	      aktRect->setPen(QPen(QColor(parentWindow->activeChangedSig),3));
	    }
	    else
	      if(rectHighlighted)
	      {
		aktRect->setZ(120);
	        aktRect->setBrush(QColor(parentWindow->changedSignal));
	        aktRect->setPen(QPen(QColor(parentWindow->changedSignal),3));
	      }	
	      else
	        if(rectActivated)
	        {
		  aktRect->setZ(140);
	          aktRect->setBrush(QColor(parentWindow->activeSignal));
	          aktRect->setPen(QPen(QColor(parentWindow->activeSignal),3));
	        }	
	        else
		{
		  aktRect->setZ(100);
	          aktRect->setBrush(QColor(parentWindow->normalNode));
	          aktRect->setPen(QPen(QColor(parentWindow->normalNode),3));
	        } 
	  }
	  else
	  {
	    cerr << "ERROR: \t 'parentWindow' not set in connection " << this << endl;
	  }
	  aktRect->setActive(true);
	  aktRect->canvas()->update();  
	}
      }
      aktRect = 0;
      aktLine = 0;
      highlighted = true;
    }
  }

  /*
   *   de-highlight the connection parts, because no signal value of
   *   those signals, that are bundeled in this connection, has changed
   */
  void gsysConnection::deHighlight()
  {
    if(highlighted)
    {
      #ifdef DEBUG_GSYSC
      cout << "CONN-deHighlight in " << this << ": \t parentWindow=" << parentWindow << endl << endl;
      #endif
      for(int i=0; i<verlauf.size(); i++)
      {
	if(verlauf[i]->rtti() == (new QCanvasLine(new QCanvas()))->rtti())
	{
	  QCanvasLine* aktLine = (QCanvasLine*) verlauf[i];
	  if(parentWindow != 0 && aktLine != 0)
	  {
	    if(activated)
	    {
	      aktLine->setZ(60);
	      aktLine->setBrush(QColor(parentWindow->activeSignal));
	      aktLine->setPen(QPen(QColor(parentWindow->activeSignal),3));
	    }
	    else
	    {
	      aktLine->setZ(20);
	      aktLine->setBrush(QColor(parentWindow->normalSignal));
	      aktLine->setPen(QPen(QColor(parentWindow->normalSignal),3));
	    }
	    aktLine->setActive(true);
	    aktLine->canvas()->update();  
	    aktLine = 0;
	  }
	  else
	  {
	    cerr << "ERROR: \t 'parentWindow' not set in connection " << this << endl;
	  }
	}
	if(verlauf[i]->rtti() == (new QCanvasRectangle(new QCanvas()))->rtti())
	{
	  QCanvasRectangle* aktRect = (QCanvasRectangle*) verlauf[i];
	  #ifdef DEBUG_GSYSC
	  cout << "RECT(deHi) i=" << i << ";  Coords: (" << aktRect->rect().x() << "," << aktRect->rect().y() << ");  Color=" << aktRect->brush().color().rgb() << "  (activeChangedSig-color: " << QColor(parentWindow->activeChangedSig).rgb() << ")" << endl;
	  #endif
	  if(parentWindow != 0 && aktRect != 0)
	  {
	    if(verlauf[i]->brush().color().rgb() == QColor(parentWindow->activeChangedSig).rgb())
	    {
	      aktRect->setZ(140);
	      aktRect->setBrush(QColor(parentWindow->activeSignal));
	      aktRect->setPen(QPen(QColor(parentWindow->activeSignal),3));
	    }
	    else
	    {
	      aktRect->setZ(100);
	      aktRect->setBrush(QColor(parentWindow->normalNode));
	      aktRect->setPen(QPen(QColor(parentWindow->normalNode),3));
	    }
	    aktRect->setActive(true);
	    aktRect->canvas()->update();  
	    aktRect = 0;
	  }
	  else
	  {
	    cerr << "ERROR: \t 'parentWindow' not set in connection " << this << endl;
	  }
	}
      }
      highlighted = false;
    }
  }

  /*
   *   get node 1 (first rectangle in structure graphics) of the connection
   */
  QPoint* gsysConnection::getNode1()
  {
    return node1;
  }

  /*
   *   get node 2 (second rectangle in structure graphics) of the connection
   */
  QPoint* gsysConnection::getNode2()
  {
    return node2;
  }

  /*
   *   set node 1 (first rectangle in structure graphics) of the connection
   *   to the point, p points to
   */
  void gsysConnection::setNode1(QPoint* p)
  {
    node1 = p;
  }

  /*
   *   set node 2 (second rectangle in structure graphics) of the connection
   *   to the point, p points to
   */
  void gsysConnection::setNode2(QPoint* p)
  {
    node2 = p;
  }

  /*
   *   adds elemt pol to the drawn connection of the structure graphics
   *   That is because in this way gSysC can determine to which connection a
   *   clicked picture element belongs.
   */
  void gsysConnection::addTraceElem(QCanvasPolygonalItem* pol)
  {
    verlauf.push_back(pol);
  }
 
  /*
   *   method, to get a pointer to the parent window, that means a pointer
   *   to the window where the connection is drawn in the canvas field
   */
  gsysHierarchyWindow* gsysConnection::getParentWindow()
  {
    return parentWindow;
  }

  /*
   *   set the pointer to the parent window
   */
  void gsysConnection::setParentWindow(gsysHierarchyWindow* parWdw)
  {
    #ifdef DEBUG_GSYSC
    cout << "CONN-setParentWindow in " << this << ": \t parameter=" << parWdw << endl;
    #endif
    parentWindow = parWdw;
    #ifdef DEBUG_GSYSC
    cout << "CONN-setParentWindow in " << this << ": \t parentWindow after assignment=" << parentWindow << endl << endl;
    #endif
  }

