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

#include "gsysBuffer.moc"

#include "gsysMain.h"
#include "gsysRegister.h"

#include <QtCore/qvariant.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qtooltip.h>
#include <QtGui/qwhatsthis.h>

/* 
 *  Constructs a gsysBuffer as a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
gsysBuffer::gsysBuffer( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "gsysBuffer" );
    pgList.clear();
    idList.clear();
    gboxList.clear();
    gbLayoutList.clear();

    gsysBufferLayout = new QVBoxLayout(this, 11, 6, "gsysBufferLayout");

    idList = (new gsysMain())->getRegModule()->getBufferIDs();
    vector<char*> nameList = (new gsysMain())->getRegModule()->getBufferNames();
    vector<bool> percList = (new gsysMain())->getRegModule()->getBufferPercs();
    for(int i=0; i<idList.size(); i++)
    {
      QGroupBox* groupBox1 = new QGroupBox( this, "groupBox1" );
      QVBoxLayout* groupBox1Layout = new QVBoxLayout(groupBox1,11,6,"gsysGroupBoxLayout");
      groupBox1Layout->setAlignment( Qt::AlignTop );

      QProgressBar* progressBar1 = new QProgressBar( groupBox1, "progressBar1" );
      
      QSpacerItem* spacer1 = new QSpacerItem(40,20,QSizePolicy::Minimum,QSizePolicy::Expanding);
      QSpacerItem* spacer2 = new QSpacerItem(40,20,QSizePolicy::Minimum,QSizePolicy::Expanding);
      groupBox1Layout->addItem( spacer1 );
      
      groupBox1Layout->addWidget( progressBar1 );
      gsysBufferLayout->addWidget( groupBox1 );
				   
      groupBox1Layout->addItem( spacer2 );
      
      gboxList.push_back(groupBox1);
      groupBox1->setTitle(nameList[i]);
      
      gbLayoutList.push_back(groupBox1Layout);
      groupBox1Layout->setAlignment( Qt::AlignTop );
      
      pgList.push_back(progressBar1);
      progressBar1->setTotalSteps(100);
      progressBar1->setProgress(0);
      progressBar1->setPercentageVisible(percList[i]);
    }

    languageChange();
    resize( QSize(150, 10).expandedTo(minimumSizeHint()) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
gsysBuffer::~gsysBuffer()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  get the index of the buffer variable with identification id
 */
int gsysBuffer::getBufferIndex(void* id)
{
  #ifdef DEBUG_GSYSC
  cout<<"\nContent of idList:"<<endl;
  for(int i=0; i<idList.size(); i++) cout<<"\t"<<i<<": "<<idList[i]<<endl;
  #endif
  for(int i=0; i<idList.size(); i++)
    if(idList[i]==id) return i;
  return -1;  
}

/*
 *  function to update the shown progressbar of buffer variable id
 *  new value is set with 'value' in relation to maximum value 'maxValue'
 */
void gsysBuffer::refreshBuffer(void* id, int value, int maxValue)
{
  #ifdef DEBUG_GSYSC
  cout<<"BUFFER: refreshBuffer called with parameters "<<id<<","<<value<<","<<maxValue<<";  getBufferIndex: "<<getBufferIndex(id)<<endl;
  #endif
  if(getBufferIndex(id)>=0)
  {
    QProgressBar* aktPg = pgList[getBufferIndex(id)];
    aktPg->setTotalSteps(maxValue);
    aktPg->setProgress(value);
    aktPg = 0;
  }  
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void gsysBuffer::languageChange()
{
    setCaption( tr( "Buffer variables" ) );
}

