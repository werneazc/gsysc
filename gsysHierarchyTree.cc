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

#include "gsysHierarchyTree.moc"
#include "gsysHierarchy.h"
#include "gsysHierarchyWindow.h"
#include "gsysMain.h"
#include "gsysMainGUI.h"


  /*
   *   constructor of this class
   */
  gsysHierarchyTree::gsysHierarchyTree( QWidget* parent, const char* name, bool modal, WFlags fl ) : QDialog( parent, name, modal, fl )
  {
    windowOpen.clear();
    allHierarchies.clear();
    allConnections.clear();

    if ( !name ) setName("gsysHierarchyTree");
    gsysHierarchyTreeLayout = new QVBoxLayout( this, 11, 6, "gsysHierarchyTreeLayout");

    tree = new QListView(this,"tree");
    tree->setRootIsDecorated(true);
    tree->addColumn(tr("Module"));
    tree->addColumn(tr("Type"));
    tree->addColumn(tr("Address"));
    gsysHierarchyTreeLayout->addWidget( tree );
    openMod = new QPushButton( this, "openMod" );
    gsysHierarchyTreeLayout->addWidget( openMod );

    languageChanged();
    resize( QSize(260, 250).expandedTo(minimumSizeHint()) );

    // signals and slots connections
    connect( openMod, SIGNAL( clicked() ), this, SLOT( openMod_clicked() ) );
  }
 
  /*
   *   destructor
   */
  gsysHierarchyTree::~gsysHierarchyTree()
  {
    this->hide();
    windowOpen.clear();
    allHierarchies.clear();
    allConnections.clear();
    // Qt components get deleted by Qt
  }
 
  /*
   *   method to test, whether the module that hier points to is open
   */
  gsysHierarchyWindow* gsysHierarchyTree::isOpen(gsysHierarchy* hier)
  {
    for (int i=0; i<windowOpen.size(); i++)
    {
      if (hier==0) 
      {
        if (windowOpen[i]->getOwnHier() != 0) std::cout << "i: " << i << ";  OwnerHier: " << windowOpen[i]->getOwnHier()->getName() << ";  Parameter: 0"  << std::endl;
        else std::cout << "i: " << i << ";  OwnerHier: Root(NULL);  Parameter: 0" << std::endl;
      }
      else 
        if (windowOpen[i]->getOwnHier() != 0) std::cout << "i: " << i << ";  OwnerHier: " << windowOpen[i]->getOwnHier()->getName() << ";  Parameter: " << hier->getName() << std::endl;
        else std::cout << "i: " << i << ";  OwnerHier: Root(NULL);  Parameter: " << hier->getName() << std::endl;
      if (windowOpen[i]->getOwnHier() == hier)
        return windowOpen[i];
    }
    return 0;
  }
 
  /*
   *   method to open the hierarchy window of the module that 'hierarchy' points to
   */
  void gsysHierarchyTree::openWindow(gsysHierarchy *hierarchy)
  {
    #ifdef DEBUG_GSYSC
    cout<<"HierarchyTree called with parameter "<<hierarchy;
    if(hierarchy!=0) cout << ", also "<<hierarchy->getName();
    cout<<endl;
    #endif
    gsysHierarchyWindow* hierWdw = isOpen(hierarchy);
    bool isRoot = (hierarchy == 0);
    if (isRoot || hierarchy->getChildren().size()>0)
	if (hierWdw == 0)
	{
	  // Initialisieren und Zeichnen der Hierarchie-Ebene
	  hierWdw = new gsysHierarchyWindow(this);

          #ifdef DEBUG_GSYSC
	  if (isRoot) std::cout << "Node is /: " << isRoot << ";  Name of module: Root" << std::endl;
	  else std::cout << "Node is /: " << isRoot << ";  Name of module: " << hierarchy->getName() << std::endl;
	  #endif

	  hierWdw->setCaption(isRoot?"Root":hierarchy->getName());
	  if (!isRoot) hierWdw->initializeWdw(hierarchy,allHierarchies,allConnections,false);
	  else hierWdw->initializeWdw(0,allHierarchies,allConnections,false);
	  

	  // signalizing the end of creation
	  windowOpen.push_back(hierWdw);
	  hierWdw->move((new gsysMain())->getMainWindow()->width()+8,0);
	  hierWdw->show();
	  hierWdw->setFocus();   // **** won't bring the window on top (unfortunately!)
	}
	else
	{
	  hierWdw->show();
	  hierWdw->setFocus();   // **** won't bring the window on top (unfortunately!)
	}

    hierWdw = 0;
  }
  
  /*
   *   eventHandler for button click of openModule-Button
   */
  void gsysHierarchyTree::openMod_clicked()
  {
    QListViewItem* lvi = tree->currentItem();
    if (lvi != 0)
    {
      int addr = lvi->text(2).toInt();
      openWindow((gsysHierarchy*) addr);
    }

    lvi = 0;
  }

  /*
   *   method to adjust the used language to the language of the environment
   */
  void gsysHierarchyTree::languageChanged()
  {
    setCaption("HierarchyTree");
    openMod->setText( tr("&Open module") );
  }

  /*
   *   method to transfer the lists of hierarchies and connections from the gsysRegister-object to this class
   */
  void gsysHierarchyTree::setInfoVectors(vector<gsysHierarchy*> hier, vector<gsysConnection*> conn)
  {
    allHierarchies = hier;
    allConnections = conn;
  }

