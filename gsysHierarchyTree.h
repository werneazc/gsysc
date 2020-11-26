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
      This class is the parent module of the single HierarchyViewer windows.
      Its main task is showing and managing the HierarchyViewer windows, and
      providing a textual overview over the underlying SystemC project.
      
 *****************************************************************************/



#ifndef GSYS_HIERTREE_H
#define GSYS_HIERTREE_H

#include "systemc.h"
class gsysHierarchy;
class gsysHierarchyWindow;
class gsysConnection;

#include <QtCore/qvariant.h>
#include <QtWidgets/qdialog.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qtooltip.h>
#include <QtWidgets/qwhatsthis.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qmessagebox.h>
#include <QtWidgets/QListWidget>

#include <vector>

using namespace std;

class gsysHierarchyTree : public QDialog
{
  friend class gsysMainGUI;
  friend class gsysRegister;
  friend class gsysSignal;
  friend class gsysSimulator;
  friend class gsysCanvasView;
  
  Q_OBJECT
  
  private:
 
  vector<gsysHierarchyWindow*> windowOpen;
  vector<gsysHierarchy*> allHierarchies;
  vector<gsysConnection*> allConnections;
  
  gsysHierarchyTree( QWidget* parent, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0 ); 

  gsysHierarchyWindow* isOpen(gsysHierarchy* hier);
  
  ~gsysHierarchyTree();

  void openWindow(gsysHierarchy *hierarchy);
  
  void setInfoVectors(vector<gsysHierarchy*> hier, vector<gsysConnection*> conn);

  QPushButton *openMod;
  QListWidget *tree;
  QSpacerItem *spacer;

  public slots:
  
  void openMod_clicked();

  protected:

  QVBoxLayout* gsysHierarchyTreeLayout;

  protected slots:

  void languageChanged();

};

#endif

