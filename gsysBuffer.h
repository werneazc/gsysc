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
      This file provides buffer variable visualization over progree bars.
      All such bars are organized in GroupBoxes for giving visible 
      separation.

 ***************************************************************************/



#ifndef GSYSBUFFER_H
#define GSYSBUFFER_H

#include <QtCore/qvariant.h>
#include <QtGui/qdialog.h>
#include <QtGui/qwidget.h>
#include <QtGui/qgroupbox.h>
#include <QtWidgets/QProgressBar>

#include <vector>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QLabel;

using namespace std;

class gsysBuffer : public QDialog
{
    friend class gsysMain;
    friend class gsysMainGUI;
    
    Q_OBJECT

private:
    gsysBuffer( QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0 );
    ~gsysBuffer();
    
    // List of all ProgressBars
    vector<QProgressBar*> pgList;
    // List of all identification numbers of the Prog.Bars
    vector<void*> idList;
    // List of the GroupBoxes, in which bars are shown
    vector<QGroupBox*> gboxList;
    // List of the Layouts inside the GroupBoxes
    vector<QVBoxLayout*> gbLayoutList;

    // Function for refreshing the ProgressBars
    void refreshBuffer(void* id, int value, int maxValue);

protected:
 
    QVBoxLayout* gsysBufferLayout;

protected slots:
    virtual void languageChange();
private:

    int getBufferIndex(void* id);

};

#endif // GSYSBUFFER_H
