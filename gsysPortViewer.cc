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

#include "gsysPortViewer.moc"
#include "gsysPortViewer.h"
#include "gsysPort.h"
#include "gsysMain.h"
#include "gsysSimulator.h"

/* 
 *  Constructs a gsysPortViewer as a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
gsysPortViewer::gsysPortViewer( QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl )
    : QDialog( parent, fl )

{
    setModal(modal);
    if ( !name ) setObjectName( "gsysPortViewer" );
    gsysPortViewerLayout = new QVBoxLayout( this ); 
    gsysPortViewerLayout->setObjectName("gsysPortViewerLayout");
    gsysPortViewerLayout->setSpacing(6);
    gsysPortViewerLayout->setMargin(11);

    portVec.clear();

    table1 = new QTableWidget( this );
    table1->setObjectName("table1");
    table1->setRowCount( 0 );
    table1->setColumnCount( 3 );
    table1->setHorizontalHeaderLabels(QStringList(QList<QString> {"Port name", "Value", "ID"}));
    table1->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table1->setSelectionMode((QTableWidget::SelectionMode) 2);	// SingleRow
    gsysPortViewerLayout->addWidget( table1 );

    frame4 = new QFrame( this );
    frame4->setObjectName("frame4");
    frame4->setFrameShape( QFrame::StyledPanel );
    frame4->setFrameShadow( QFrame::Raised );
    frame4Layout = new QHBoxLayout( frame4); 
    frame4Layout->setObjectName("frame4Layout");
    frame4Layout->setSpacing(6);
    frame4Layout->setMargin(11);


    comboBox1 = new QComboBox( frame4 );
    comboBox1->setObjectName("comboBox1");
    comboBox1->insertItem(0, tr("all") );
    frame4Layout->addWidget( comboBox1 );

    pushButton1 = new QPushButton( "pushButton1", frame4 );
    frame4Layout->addWidget( pushButton1 );
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum );
    frame4Layout->addItem( spacer );

    pushButton2 = new QPushButton( "pushButton2", frame4 );
    frame4Layout->addWidget( pushButton2 );
    gsysPortViewerLayout->addWidget( frame4 );
    languageChange();
    resize( QSize(409, 289).expandedTo(minimumSizeHint()) );

    // connect signals and slots
    connect( pushButton1, SIGNAL( clicked() ), this, SLOT( pushButton1_clicked() ) );
    connect( pushButton2, SIGNAL( clicked() ), this, SLOT( pushButton2_clicked() ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
gsysPortViewer::~gsysPortViewer()
{
  // no need to delete child widgets, Qt does it all for us
  this->hide();
  portVec.clear();
}

/*
 *   add the chosen port in the comboBox to the table of ports
 */
void gsysPortViewer::addPort(gsysPort* port)
{
  comboBox1->insertItem(0, port->getName());
  portVec.push_back(port);
}

/*
 *   refresh all port values in the table
 */
void gsysPortViewer::refresh()
{
  #ifdef DEBUG_GSYSC
  cout<<"Function refresh() called!"<<endl;
  #endif
  if(this->isVisible())
  {
    gsysPort* aktPort;
    for(int i=0; i<table1->rowCount(); i++)
    {
      aktPort = (gsysPort*) atoi(table1->item(i,2)->text().toStdString().c_str());
      table1->setItem(i,1,new QTableWidgetItem( QString( aktPort->getValue() ) ) );
      #ifdef DEBUG_GSYSC
      cout<<"REFRESH => Port "<<aktPort->getName()<<" ("<<aktPort<<") has value "<<aktPort->getValue()<<endl;
      #endif
    }
    aktPort = 0;
  }
  #ifdef DEBUG_GSYSC
  cout<<"Funktion refresh() finished!"<<endl;
  #endif
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void gsysPortViewer::languageChange()
{
    setWindowTitle( tr( "Port observation" ) );
    table1->setHorizontalHeaderLabels(QStringList(QList<QString> {"Port name", "Value"}));
    pushButton1->setText( tr( "&Add" ) );
    pushButton2->setText( tr( "&Remove" ) );
    comboBox1->setItemText( 0, tr("all") );
}


/*
 *   EventHandler of the add-button
 */
void gsysPortViewer::pushButton1_clicked()
{
  if(comboBox1->currentIndex() == 0)	// add all ports, if wanted
  {
    #ifdef DEBUG_GSYSC
    cout << "Add all items in the table. That means "<<portVec.size()<<" elements!"<<endl;
    #endif
    gsysPort* aktPort;
    int maxIdx = portVec.size();
    for(int idx=0; idx<maxIdx; idx++)
    {
      aktPort = portVec.front();
      comboBox1->removeItem(1);
      table1->setRowCount( table1->rowCount() + 1 );
      table1->setItem(table1->rowCount()-1, 0, new QTableWidgetItem( QString( aktPort->getName() ) ) );
      table1->setItem(table1->rowCount()-1, 1, new QTableWidgetItem( QString( aktPort->getValue() ) ) );
      table1->setItem(table1->rowCount()-1, 2, new QTableWidgetItem( QString( (new gsysMain())->getSimulator()->asChar((int64_t)aktPort))));
      portVec.erase(portVec.begin());
    }
    aktPort = 0;
  }
  else		// anderen Port hinzufügen
  {
    int idx = comboBox1->currentIndex();
    gsysPort* aktPort = portVec[idx-1];
    comboBox1->removeItem(idx);
    table1->setRowCount( table1->rowCount() + 1 );
    table1->setItem(table1->rowCount()-1, 0, new QTableWidgetItem( QString( aktPort->getName() ) ) );
    table1->setItem(table1->rowCount()-1, 1, new QTableWidgetItem( QString( aktPort->getValue() ) ) );
    table1->setItem(table1->rowCount()-1, 2, new QTableWidgetItem( QString( (new gsysMain())->getSimulator()->asChar((int64_t)aktPort) ) ) );
    portVec.erase(portVec.begin()+idx-1);
  }
}

/*
 *   EventHandler of the remove-button
 */
void gsysPortViewer::pushButton2_clicked()
{
  #ifdef DEBUG_GSYSC
  cout<<"Removal in row "<<table1->currentRow()<<";  Name: "<<(QString) table1->item(table1->currentRow(),0)<<";  ID: "<<(QString) table1->item(table1->currentRow(),2)<<endl;
  #endif
  gsysPort* aktPort;
  int addr = atoi(table1->item(table1->currentRow(),2)->text().toStdString().c_str());
  comboBox1->insertItem(0, table1->item(table1->currentRow(),0)->text());
  aktPort = (gsysPort*)addr;
  portVec.push_back(aktPort);
  table1->removeRow(table1->currentRow());
}

