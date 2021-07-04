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
    QList<QGraphicsItem*> ceil = parentWindow->canvasView->scene()->items(rect);
    for(int i=0; i<ceil.count(); i++)
      if(ceil[i]->type() == (new QGraphicsLineItem())->type())
        if(((QGraphicsLineItem*)ceil[i])->pen().color().rgb() == col.rgb()) return true;
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
      QGraphicsRectItem* aktRect = 0;
      QGraphicsLineItem* aktLine = 0;
      for(int i=0; i<verlauf.size(); i++)
      {
	if(verlauf[i]->type() == (new QGraphicsLineItem(0, 0, 0, 0))->type())
	{
	  aktLine = (QGraphicsLineItem*) verlauf[i];
	  if(highlighted)
	  {
	    if(parentWindow!=0)
	    {
	      aktLine->setZValue(80);
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
	      aktLine->setZValue(60);
	      aktLine->setPen(QPen(QColor(parentWindow->activeSignal),3));
	    }
	    else
	    {
	      cerr << "ERROR: \t 'parentWindow' not set in connection " << this << endl;
	    }
	  }
	  aktLine->setActive(true);
	  aktLine->scene()->update();  
	}
	if(verlauf[i]->type() == (new QGraphicsRectItem(0,0,0,0))->type())
	{
	  aktRect = (QGraphicsRectItem*) verlauf[i];
	  if(parentWindow != 0)
	  {	  
  	    if(aktRect->pen().color().rgb() == QColor(parentWindow->changedSignal).rgb()) 
	    {
	      aktRect->setZValue(160);
	      aktRect->setBrush(QBrush(QColor(parentWindow->activeChangedSig)));
	      aktRect->setPen(QPen(QColor(parentWindow->activeChangedSig),3));
	    }
	    else
	    {
	      aktRect->setZValue(140);
	      aktRect->setBrush(QBrush(QColor(parentWindow->activeSignal)));
	      aktRect->setPen(QPen(QColor(parentWindow->activeSignal),3));
	    }
	  }  
	  else
	  {  
	    cerr << "ERROR: \t 'parentWindow' not set in connection " << this << endl;
	  }  
	  aktRect->setActive(true);
	  aktRect->scene()->update();  
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
	if(verlauf[i]->type() == (new QGraphicsLineItem(0,0,0,0))->type())
	{
	  QGraphicsLineItem* aktLine = (QGraphicsLineItem*) verlauf[i];
	  if(parentWindow != 0)
	    if(highlighted)
	    {
	      aktLine->setZValue(40);
	      aktLine->setPen(QPen(QColor(parentWindow->changedSignal),3));
	    }
	    else
	    {
	      aktLine->setZValue(20);
	      aktLine->setPen(QPen(QColor(parentWindow->normalSignal),3));
	    }
	  else  
	  {	  
	    cerr << "ERROR: \t 'parentWindow' not set in connection " << this << endl;
	  }
	  aktLine->setActive(true);
	  aktLine->scene()->update();  
	  aktLine = 0;
	}
	if(verlauf[i]->type() == (new QGraphicsRectItem(0,0,0,0))->type())
	{
	  QGraphicsRectItem* aktRect = (QGraphicsRectItem*) verlauf[i];
	  
	  if(parentWindow != 0)
	  {	  
	    // Check whether any signal is highlighted (through click) and/or changed
            QList<QGraphicsItem*> cil = parentWindow->canvasView->scene()->items(aktRect->rect());
	    bool rectActivated = false;
	    bool rectHighlighted = false;
            for(int i=0; i<cil.count(); i++)
	    {	    
              if(cil[i]->type() == (new QGraphicsLineItem(0,0,0,0))->type())
	      {	      
                if(((QGraphicsLineItem*)cil[i])->pen().color().rgb() == QColor(parentWindow->activeChangedSig).rgb()) 
		{
		  rectActivated = true;
		  rectHighlighted = true;
		  break;
		}
		else if(((QGraphicsLineItem*)cil[i])->pen().color().rgb() == QColor(parentWindow->activeSignal).rgb())
		     {
		       rectActivated = true;
		     }
		     else if(((QGraphicsLineItem*)cil[i])->pen().color().rgb() == QColor(parentWindow->changedSignal).rgb())
			  {
		            rectHighlighted = true;
			  }
	      }	
	    }  
	    if(rectActivated && rectHighlighted)
	    {
	      aktRect->setZValue(160);
	      aktRect->setBrush(QBrush(QColor(parentWindow->activeChangedSig)));
	      aktRect->setPen(QPen(QColor(parentWindow->activeChangedSig),3));
	    }
	    else
	      if(rectHighlighted)
	      {
		aktRect->setZValue(120);
		aktRect->setBrush(QBrush(QColor(parentWindow->changedSignal)));
	        aktRect->setPen(QPen(QColor(parentWindow->changedSignal),3));
	      }	
	      else
	        if(rectActivated)
	        {
		  aktRect->setZValue(140);
		  aktRect->setBrush(QBrush(QColor(parentWindow->activeSignal)));
	          aktRect->setPen(QPen(QColor(parentWindow->activeSignal),3));
	        }	
	        else
		{
		  aktRect->setZValue(100);
		  aktRect->setBrush(QBrush(QColor(parentWindow->normalNode)));
	          aktRect->setPen(QPen(QColor(parentWindow->normalNode),3));
	        } 
	  }
	  else
	  {	  
	    cerr << "ERROR: \t 'parentWindow' not set in connection " << this << endl;
	  }  
	  aktRect->setActive(true);
	  aktRect->scene()->update();  
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
      QGraphicsRectItem* aktRect = 0;
      QGraphicsLineItem* aktLine = 0;
      for(int i=0; i<verlauf.size(); i++)
      {
		if(verlauf[i]->type() == (new QGraphicsLineItem(0,0,0,0))->type())
		{
		  aktLine = (QGraphicsLineItem*) verlauf[i];
		  if (parentWindow != 0)
		  { 
		    if(activated)
		    {
		      aktLine->setZValue(80);
		      aktLine->setPen(QPen(QColor(parentWindow->activeChangedSig),3));
		    }
		    else
		    {
		      aktLine->setZValue(40);
		      aktLine->setPen(QPen(QColor(parentWindow->changedSignal),3));
		    }
		  }
		  else
		  {
		    cerr << "ERROR: \t 'parentWindow' not set in connection " << this << endl;
    	  }
		  aktLine->setActive(true);
		  aktLine->scene()->update();  
		}
		if(verlauf[i]->type() == (new QGraphicsRectItem(0,0,0,0))->type())
		{
		  aktRect = (QGraphicsRectItem*) verlauf[i];
		  #ifdef DEBUG_GSYSC
		  cout << "RECT(high) i=" << i << ";  Coords: (" << aktRect->rect().x() << "," << aktRect->rect().y() << ");  Color=" << aktRect->pen().color().rgb() << "  (activeSignal-color: " << QColor(parentWindow->activeSignal).rgb() << ")" << endl;
		  #endif
		  if(parentWindow != 0)
		  {
		    // Check whether any signal is highlighted (through click) and/or changed
    	    QList<QGraphicsItem*> cil = parentWindow->canvasView->scene()->items(aktRect->rect());
		    bool rectActivated = false;
		    bool rectHighlighted = false;
    	    for(int i=0; i<cil.count(); i++)
		    {	    
    	      if(cil[i]->type() == (new QGraphicsLineItem(0,0,0,0))->type())
		      {	      
    	        if(((QGraphicsLineItem*)cil[i])->pen().color().rgb() == QColor(parentWindow->activeChangedSig).rgb()) 
				{
			  		rectActivated = true;
			  		rectHighlighted = true;
			  		break;
			    }
				else if(((QGraphicsLineItem*)cil[i])->pen().color().rgb() == QColor(parentWindow->activeSignal).rgb())
			    {
			    	rectActivated = true;
			    }
			    else if(((QGraphicsLineItem*)cil[i])->pen().color().rgb() == QColor(parentWindow->changedSignal).rgb())
				{
			        rectHighlighted = true;
				}
		      }	
		    }   
		    if(rectActivated && rectHighlighted)
		    {
		      		aktRect->setZValue(160);
		      		aktRect->setBrush(QBrush(QColor(parentWindow->activeChangedSig)));
		      		aktRect->setPen(QPen(QColor(parentWindow->activeChangedSig),3));
		    }
		    else
		      if(rectHighlighted)
		      {
					aktRect->setZValue(120);
					aktRect->setBrush(QBrush(QColor(parentWindow->changedSignal)));
		        	aktRect->setPen(QPen(QColor(parentWindow->changedSignal),3));
		      }	
		    else
		        if(rectActivated)
		        {
			  		aktRect->setZValue(140);
			  		aktRect->setBrush(QBrush(QColor(parentWindow->activeSignal)));
		          	aktRect->setPen(QPen(QColor(parentWindow->activeSignal),3));
		        }	
		    else
				{
			  		aktRect->setZValue(100);
			  		aktRect->setBrush(QBrush(QColor(parentWindow->normalNode)));
		          	aktRect->setPen(QPen(QColor(parentWindow->normalNode),3));
		        } 
		  }
		  else
		  {
		    cerr << "ERROR: \t 'parentWindow' not set in connection " << this << endl;
		  }
		  aktRect->setActive(true);
		  aktRect->scene()->update();  
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
	if(verlauf[i]->type() == (new QGraphicsLineItem(0,0,0,0))->type())
	{
	  QGraphicsLineItem* aktLine = (QGraphicsLineItem*) verlauf[i];
	  if(parentWindow != 0 && aktLine != 0)
	  {
	    if(activated)
	    {
	      aktLine->setZValue(60);
	      aktLine->setPen(QPen(QColor(parentWindow->activeSignal),3));
	    }
	    else
	    {
	      aktLine->setZValue(20);
	      aktLine->setPen(QPen(QColor(parentWindow->normalSignal),3));
	    }
	    aktLine->setActive(true);
	    aktLine->scene()->update();  
	    aktLine = 0;
	  }
	  else
	  {
	    cerr << "ERROR: \t 'parentWindow' not set in connection " << this << endl;
	  }
	}
	if(verlauf[i]->type() == (new QGraphicsRectItem(0,0,0,0))->type())
	{
	  QGraphicsRectItem* aktRect = (QGraphicsRectItem*) verlauf[i];
	  #ifdef DEBUG_GSYSC
	  cout << "RECT(deHi) i=" << i << ";  Coords: (" << aktRect->rect().x() << "," << aktRect->rect().y() << ");  Color=" << aktRect->pen().color().rgb() << "  (activeChangedSig-color: " << QColor(parentWindow->activeChangedSig).rgb() << ")" << endl;
	  #endif
	  if(parentWindow != 0 && aktRect != 0)
	  {
	    if(verlauf[i]->pen().color().rgb() == QColor(parentWindow->activeChangedSig).rgb())
	    {
	      aktRect->setZValue(140);
	      aktRect->setBrush(QBrush(QColor(parentWindow->activeSignal)));
	      aktRect->setPen(QPen(QColor(parentWindow->activeSignal),3));
	    }
	    else
	    {
	      aktRect->setZValue(100);
	      aktRect->setBrush(QBrush(QColor(parentWindow->normalNode)));
	      aktRect->setPen(QPen(QColor(parentWindow->normalNode),3));
	    }
	    aktRect->setActive(true);
	    aktRect->scene()->update();  
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
   *   adds elemt powl to the drawn connection of the structure graphics
   *   That is because in this way gSysC can determine to which connection a
   *   clicked picture element belongs.
   */
  void gsysConnection::addTraceElem(QAbstractGraphicsShapeItem* powl)
  {
    verlauf.push_back(powl);
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

