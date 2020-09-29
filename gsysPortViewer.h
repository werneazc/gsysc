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
      This class provides a window for the PortViewer. It is meant to offer
      runtime evaluation of port values for giving the user the ability
      of testing, whether the project works correctly.

 ***************************************************************************/

#ifndef GSYS_PORTVIEWER_H
#define GSYS_PORTVIEWER_H

#include <QtCore/qvariant.h>
#include <QtGui/qdialog.h>
#include <QtGui/qcombobox.h>
#include <QtGui/qframe.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qtable.h>
#include <QtGui/qlayout.h>
#include <QtGui/qtooltip.h>
#include <QtGui/qwhatsthis.h>

#include <vector>

using namespace std;

class gsysPort;

class gsysPortViewer : public QDialog
{
    friend class gsysMainGUI;
  
    Q_OBJECT

private:

    vector<gsysPort*> portVec;

public:
    gsysPortViewer( QWidget* parent, const char* name = 0, bool modal = FALSE, Qt::WindowFlags fl = 0 );
    ~gsysPortViewer();

    QTable* table1;
    QFrame* frame4;
    QComboBox* comboBox1;
    QPushButton* pushButton1;
    QPushButton* pushButton2;

    void addPort(gsysPort* port);
    void refresh();

public slots:
    void pushButton1_clicked();		// add port
    void pushButton2_clicked();		// remove port

protected:
    QVBoxLayout* gsysPortViewerLayout;
    QHBoxLayout* frame4Layout;

protected slots:
    virtual void languageChange();
};

#endif // GSYS_PORTVIEWER_H
