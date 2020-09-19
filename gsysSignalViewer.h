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

/****************************************************************************
    Author: Christian J. Eibl
    last modified: 2005-01-31
 ****************************************************************************
    Content:
      This class provides displaying of signal properties,
      if the user clicked on that signal in the HierarchyViewer.

 ***************************************************************************/

#ifndef GSYS_SIGNALVIEWER_H
#define GSYS_SIGNALVIEWER_H

#include <qvariant.h>
#include <qdialog.h>

#include <qvariant.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <stdio.h>
#include <sstream>
#include <vector>
using namespace std;

class gsysSignalViewer : public QDialog
{
    friend class gsysSignal;
    Q_OBJECT

private:
    gsysSignalViewer( QWidget* parent, int portCount=2, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~gsysSignalViewer();

    vector<QGroupBox*> groupBoxList;
    vector<QGridLayout*> gridLayoutList;
    vector<QLabel*> textLabelList;
    vector<QLineEdit*> lineEditList;
		    
    QGroupBox* groupBox3;
    QLabel* textLabel3;
    QLabel* textLabel4;
    QLineEdit* lineEdit3;
    QLineEdit* lineEdit4;

protected:
    QVBoxLayout* gsysSignalViewerLayout;
    QGridLayout* groupBox3Layout;

protected slots:
    virtual void languageChange();
};

#endif // GSYS_SIGNALVIEWER_H
