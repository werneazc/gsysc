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
      This class contains the front end of the simulation controller.
      It represents the main feature of gSysC. Many other classe depent on it.

 *****************************************************************************/



#ifndef GSYS_SIMULATOR_H
#define GSYS_SIMULATOR_H

#include "systemc.h"

class gsysSimulator;
class gsysConnection;
class gsysMain;
class gsysHierarchyTree;
class gsysRegister;
class gsysPort;

#include <QtCore/qvariant.h>
#include <QtWidgets/qdialog.h>
#include <QtWidgets/qcheckbox.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qspinbox.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qtooltip.h>
#include <QtWidgets/qwhatsthis.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qcombobox.h>
#include <QtWidgets/QGroupBox>

#include <stdio.h>
#include <sstream>
#include <vector> 

using namespace std;

extern pthread_mutex_t run_mutex;
extern void* startThread(void *stepsVoid);


class gsysSimulator : public QDialog
{
  friend class gsysMainGUI;

  Q_OBJECT

public:

  QGroupBox* groupBox1;
  QLabel* textLabel1;
  QSpinBox* stepSpin;
  QLabel* textLabel2;
  QSpinBox* maxSpin;
  QGroupBox* groupBox2;
  QPushButton* stepButton;
  QPushButton* startButton;
  QPushButton* stopButton;
  QGroupBox* groupBox3;
  QCheckBox* useViewer;
  QCheckBox* autoStep;
  QGroupBox* groupBox4;
  QLabel* textLabel3;
  QLabel* textLabel3_2;
  QSpinBox* intervalSpin;
  QGroupBox* groupBox5;
  QCheckBox* useWatcher;
  QComboBox* watchVariable;
  QLabel* textLabel1_2;
  QLineEdit* watchValue;

private:
  
  int aktStep;
  bool toStop;
  bool hardStop;
  gsysHierarchyTree* hierTree;
  vector<gsysPort*> watchPorts;

  gsysSimulator(); 

  /*
   *  Constructs a gsysSimulator as a child of 'parent', with the
   *  name 'name' and widget flags set to 'f'.
   *
   *  The dialog will by default be modeless, unless you set 'modal' to
   *  TRUE to construct a modal dialog.
   */
  gsysSimulator( QWidget* parent, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0 );
  
  bool haveToStop();

public:

  /*
  *  Destroys the object and frees any allocated resources
  */
  ~gsysSimulator();
      
  char* asChar(int nr);
  
  void openSim(int steps=1000);
  void start(int stepCount=1, sc_core::sc_time_unit unit = sc_core::SC_NS);
  void stop();
  void addPort(gsysPort* port);
  void clearPorts();
  
public slots:
  void autoStep_stateChanged( int );
  void useWatcher_stateChanged( int );
  void startButton_clicked();
  void stopButton_clicked();
  void stepButton_clicked();
  void maxSpin_valueChanged( int );
  void useViewer_stateChanged( int );
  

protected:
  QVBoxLayout* gsysSimulatorLayout;
  QHBoxLayout* groupBox1Layout;
  QHBoxLayout* groupBox2Layout;
  QVBoxLayout* groupBox3Layout;
  QHBoxLayout* groupBox4Layout;
  QHBoxLayout* groupBox5Layout;

protected slots:
  /*
   *  Sets the strings of the subwidgets using the current
   *  language.
   */
  void languageChange();
							 
};



#endif
