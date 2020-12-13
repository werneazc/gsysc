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

#include "gsysMainGUI.moc" 
#include "gsysMainGUI.h"
#include "gsysAbout.h"
#include "gsysSimulator.h"
#include "gsysPortViewer.h"
#include "gsysHierarchyTree.h"
#include "gsysBuffer.h"
#include "gsysMain.h"
#include "gsysRegister.h"

    /*
     *   open the main window
     */
    void gsysMainGUI::openMainWdw()
    {
      this->move(0,0);
      this->show();
    }

    /*
     *   open a simulation window
     *   initialize the maximum steps spinValue to 'steps'
     */
    void gsysMainGUI::startSimulator(int steps)
    {
      if(simulator==NULL) simulator=new gsysSimulator(this);
      this->move(0,0);
      this->show();
      simulator->move(this->width()+8,0);
      simulator->openSim(steps);
    }
    
    /*
     *   open the HierarchyTree window
     */
    void gsysMainGUI::startHierarchyTree()
    {
      if(hierTree==NULL) hierTree=new gsysHierarchyTree(this);
      this->move(0,0);
      this->show();
      hierTree->move(0,this->height()+27);
      hierTree->show();
    }

    /*
     *   open the PortViewer window
     */
    void gsysMainGUI::startPortViewer()
    {
      if(portViewer==NULL) portViewer=new gsysPortViewer(this);
      this->move(0,0);
      this->show();
      portViewer->move(this->width()+8+simulator->width()+8,0);
      portViewer->show();
    }
   
    /*
     *   open the About window
     */
    void gsysMainGUI::openAboutWdw()
    {
      if(aboutWdw==NULL) aboutWdw=new gsysAbout(this,"gSysC",true);
      aboutWdw->move(400,300);
      aboutWdw->show();
    }

    /*
     *   open the window for user-defined functionality
     */
    void gsysMainGUI::openBufferWdw()
    {
      createBufferWdw();
      bufferWdw->move(200,200);
      bufferWdw->show();
    }

    /*
     *  Destroys the object and frees any allocated resources
     */
    gsysMainGUI::~gsysMainGUI()
    {
      // no need to delete child widgets, Qt does it all for us
      this->hide();
      delete simulator; simulator = 0;
      delete hierTree; hierTree = 0;
      delete portViewer; portViewer = 0;
      delete aboutWdw; aboutWdw = 0;
      delete bufferWdw; bufferWdw = 0;
    }
      

    /*
     *  Constructs a gsysMain as a child of 'parent', with the
     *  name 'name' and widget flags set to 'f'.
     *
     *  The dialog will by default be modeless, unless you set 'modal' to
     *  TRUE to construct a modal dialog.
     */
    gsysMainGUI::gsysMainGUI( QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl )
           : QDialog( parent, name, modal, fl )
    {
      simulator = new gsysSimulator(this);
      hierTree = new gsysHierarchyTree(this);
      aboutWdw = 0;
      portViewer = NULL;
      bufferWdw = 0;
      
      if ( !name )
        setName( "gsysMain" );
      gsysMainGUILayout = new QVBoxLayout( this, 11, 6, "gsysMainGUILayout");

      pushButton4 = new QPushButton( this, "pushButton4" );
      gsysMainGUILayout->addWidget( pushButton4 );
      QSpacerItem* spacer = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Fixed );
      gsysMainGUILayout->addItem( spacer );

      pushButton5 = new QPushButton( this, "pushButton5" );
      gsysMainGUILayout->addWidget( pushButton5 );

      pushButton8 = new QPushButton( this, "pushButton8" );
      gsysMainGUILayout->addWidget( pushButton8 );

      pushButton9 = new QPushButton( this, "pushButton9" );
      gsysMainGUILayout->addWidget( pushButton9 );
      QSpacerItem* spacer_2 = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Fixed );
      gsysMainGUILayout->addItem( spacer_2 );
      if((new gsysMain())->getRegModule()->getBufferIDs().size() <= 0) pushButton9->setEnabled( false );

      pushButton6 = new QPushButton( this, "pushButton6" );
      gsysMainGUILayout->addWidget( pushButton6 );
      QSpacerItem* spacer_3 = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Expanding );
      gsysMainGUILayout->addItem( spacer_3 );
      languageChange();
      resize( QSize(245, 215).expandedTo(minimumSizeHint()) );

      // signals and slots connections
      connect( pushButton4, SIGNAL( clicked() ), this, SLOT( pushButton4_clicked() ) );
      connect( pushButton5, SIGNAL( clicked() ), this, SLOT( pushButton5_clicked() ) );
      connect( pushButton8, SIGNAL( clicked() ), this, SLOT( pushButton8_clicked() ) );
      connect( pushButton9, SIGNAL( clicked() ), this, SLOT( pushButton9_clicked() ) );
      connect( pushButton6, SIGNAL( clicked() ), this, SLOT( pushButton6_clicked() ) );
    }

    /* 
     *   get pointer to the PortViewer window
     */
    gsysPortViewer* gsysMainGUI::getPortViewer()
    {
      if(portViewer==NULL)
        portViewer = new gsysPortViewer(this);
      return portViewer;	
    }
															  
    /*
     *   EventHandler for startSimulator-Button
     */
    void gsysMainGUI::pushButton4_clicked()    // start simulator
    {
      startSimulator(-1);
    }
    
    /*
     *   EventHandler for startHierarchyViewer button
     */
    void gsysMainGUI::pushButton5_clicked()    // Hierarchy-Viewer
    {
      startHierarchyTree();
    }
    
    /*
     *   Event-Handler for startPortViewer-button
     */
    void gsysMainGUI::pushButton8_clicked()    // Port observation
    {
      startPortViewer();
    }
    
    /*
     *   EventHandler for startBuffer-button
     */
    void gsysMainGUI::pushButton9_clicked()    // Buffer variables
    {
      openBufferWdw();
    }
    
    /*
     *   EventHandler for the About-window button
     */
    void gsysMainGUI::pushButton6_clicked()    // About dialog
    {
      openAboutWdw();
    }

    /*
     *   create a simulator window if necessary and the return the pointer to it
     */
    gsysSimulator* gsysMainGUI::createSimulator()
    {
      if (simulator == 0) simulator = new gsysSimulator(this);
      return simulator;
    }
    
    /*
     *   create a HierarchyTree window if necessary and return a pointer to it
     */
    gsysHierarchyTree* gsysMainGUI::createHierTree()
    {
      if (hierTree == 0) hierTree = new gsysHierarchyTree(this);
      return hierTree;
    }

    /*
     *   create a Buffer-Variable window an return the pointer
     */
    gsysBuffer* gsysMainGUI::createBufferWdw()
    {
      if(bufferWdw==0) bufferWdw=new gsysBuffer(this);
      return bufferWdw;
    }
    

    /*
     *  Sets the strings of the subwidgets using the current
     *  language.
     */
    void gsysMainGUI::languageChange()
    {
      setCaption( tr( "gSysC" ) );
      pushButton4->setText( tr( "&Start simulator" ) );
      pushButton5->setText( tr( "&Hierarchy-Viewer" ) );
      pushButton8->setText( tr( "&Port-Viewer" ) );
      pushButton9->setText( tr( "&Buffer variables" ) );
      pushButton6->setText( tr( "About &gSysC" ) );
    }
			   

