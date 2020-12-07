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

#include "gsysSignalViewer.moc"
#include "gsysSignalViewer.h"

/* 
 *  Constructs a gsysSignalViewer as a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
gsysSignalViewer::gsysSignalViewer( QWidget* parent, int portCount, const char* name, bool modal, Qt::WindowFlags fl )
    : QDialog( parent, fl )

{
    setModal(modal);
    if ( !name ) setObjectName( "gsysSignalViewer" );
    gsysSignalViewerLayout = new QVBoxLayout( this ); 
    gsysSignalViewerLayout->setObjectName("gsysSignalViewerLayout");
    gsysSignalViewerLayout->setSpacing(6);
    gsysSignalViewerLayout->setMargin(11);

    groupBoxList.clear();
    gridLayoutList.clear();
    textLabelList.clear();
    lineEditList.clear();

    for(int i=0; i<portCount; i++)
    {
      ostringstream ostr;
      ostr << (i+1) << ends;
      groupBoxList.push_back(new QGroupBox("groupbox",this));
      groupBoxList.back()->layout()->setSpacing( 6 );
      groupBoxList.back()->layout()->setMargin( 11 );
      groupBoxListLayout = new QGridLayout();
      groupBoxListLayout->addLayout(groupBoxList.back()->layout(),1,1);
      gridLayoutList.push_back(groupBoxListLayout);
      gridLayoutList.back()->setAlignment( Qt::AlignTop );
      textLabelList.push_back(new QLabel("textLabel",groupBoxList.back()));
      gridLayoutList.back()->addWidget(textLabelList.back(),0,0 );
      textLabelList.push_back(new QLabel("textLabel",groupBoxList.back()));
      gridLayoutList.back()->addWidget(textLabelList.back(),1,0);
      lineEditList.push_back(new QLineEdit("lineEdit",groupBoxList.back()));
      lineEditList.back()->setReadOnly(true);
      gridLayoutList.back()->addWidget(lineEditList.back(),0,1);
      lineEditList.push_back(new QLineEdit("lineEdit",groupBoxList.back()));
      lineEditList.back()->setReadOnly(true);
      gridLayoutList.back()->addWidget(lineEditList.back(),1,1);
      gsysSignalViewerLayout->addWidget(groupBoxList.back());
    }

    groupBox3 = new QGroupBox( "groupBox3", this );
    groupBox3->layout()->setSpacing( 6 );
    groupBox3->layout()->setMargin( 11 );
    groupBox3Layout = new QGridLayout();
    groupBox3Layout->addLayout(groupBox3->layout(), 1, 1);
    groupBox3Layout->setAlignment( Qt::AlignTop );

    textLabel3 = new QLabel( "textLabel3", groupBox3 );
    groupBox3Layout->addWidget( textLabel3, 0, 0 );

    textLabel4 = new QLabel( "textLabel4", groupBox3 );
    groupBox3Layout->addWidget( textLabel4, 1, 0 );

    lineEdit3 = new QLineEdit( "lineEdit3", groupBox3 );
    lineEdit3->setReadOnly(true);
    groupBox3Layout->addWidget( lineEdit3, 0, 1 );

    lineEdit4 = new QLineEdit( "lineEdit4", groupBox3 );
    lineEdit4->setReadOnly(true);
    groupBox3Layout->addWidget( lineEdit4, 1, 1 );
    gsysSignalViewerLayout->addWidget( groupBox3 );
    languageChange();
    resize( QSize(250, 264).expandedTo(minimumSizeHint()) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
gsysSignalViewer::~gsysSignalViewer()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void gsysSignalViewer::languageChange()
{
    setWindowTitle( tr( "Signal value" ) );
    for(int i=0; i<groupBoxList.size(); i++)
    {
      ostringstream ostr;
      ostr << (i+1) << ends;
      groupBoxList[i]->setTitle( tr( "Module" ).append(ostr.str().c_str()) );
    }  
    for(int i=0; i<textLabelList.size(); i++)
    {
      if(i % 2 == 0)   // i gerade
        textLabelList[i]->setText( tr( "Name:" ) );
      else	
        textLabelList[i]->setText( tr( "Port name:" ) );
    }
    groupBox3->setTitle( tr( "Signal" ) );
    textLabel3->setText( tr( "Name:" ) );
    textLabel4->setText( tr( "Value:" ) );
}

