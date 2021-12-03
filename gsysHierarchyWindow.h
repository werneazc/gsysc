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
      This class is used for visualization of the structure graphics
      of the underlying SystemC project.
      It is one of the main elements of gSysC.

 *****************************************************************************/



#ifndef GSYS_HIERWIN_H
#define GSYS_HIERWIN_H

#include "systemc.h"

#include <vector>
#include <cmath>
#include <stdio.h>
#include <string>

#include <QtCore/qvariant.h>
#include <QtCore/qrect.h>
#include <QtWidgets/qdialog.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qtooltip.h>
#include <QtWidgets/qwhatsthis.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qgroupbox.h>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGroupBox>

class gsysSignalViewer;
class gsysPort;
class gsysHierarchy;
class gsysConnection;
class gsysCanvasView;

// sectors for direction classification defined
#define N       1
#define NNW     2
#define NW      3
#define WNW     4
#define W       5
#define WSW     6
#define SW      7
#define SSW     8
#define S       9
#define SSE     10
#define SE      11
#define ESE     12
#define E       13
#define ENE     14
#define NE      15
#define NNE     16

using namespace std;

class gsysHierarchyWindow : public QDialog 
{
  friend class gsysHierarchyTree;
  friend class gsysCanvasView;
  friend class gsysConnection;
  friend class gsysPort;

  Q_OBJECT

  private:
  
  enum moduleType{PE, CHANNEL, MISC};
  vector< tuple <gsysHierarchy*, moduleType> > hierarchyList;
  vector<gsysConnection*> connList;        // Connections inside of this level
  vector<gsysConnection*> sideConnList;    // Connections to outer elements
  gsysHierarchyWindow( QWidget* parent, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0 );
  gsysHierarchy* parent;
  gsysHierarchy* ownHierarchy;
  int dimFactor;
  short getSector(QPoint *source, QPoint *dest);
  QPoint* normalize(QPoint *p);
  void drawSidePort(gsysPort* port, bool left, int nr, int abstand, int portHeight=21);
  void drawNetConns(gsysHierarchy* hier);
  void drawConnections();
  bool drawConnStep(gsysConnection* connection,QPoint *p1, QPoint *p2,int lfdNr);
  bool thisLevel(gsysHierarchy* hier);
  moduleType getModuleType(gsysHierarchy* hier);
 
  // values to be read from file 'gsysHViewer.conf'
  int sideMargin;
  int topMargin;
  int horizontalSpace;
  int verticalSpace;
  int moduleWidth;
  int moduleHeight;
  char* backgroundColor;
  char* moduleColor;
  char* moduleWithChild;
  char* textColor;
  char* inPort;
  char* outPort;
  char* inoutPort;
  char* mmHierConnColor;
  bool mmHierConnShow;
  bool mmSigPortShow;

  void readConfig();
 
  QPushButton *saveButton;
  QGroupBox* moveInfos;
  QLabel* labelKoordinaten;
  QLineEdit* coord;
  QLabel* labelTyp;
  QLineEdit* lineEditTyp;
  QLabel* labelWert;
  QLineEdit* lineEditWert;
  QHBoxLayout* moveInfosLayout;
  
  char* normalSignal;
  char* normalNode;
  char* activeSignal;
  char* changedSignal;
  char* activeChangedSig;

  ~gsysHierarchyWindow();
  gsysCanvasView *canvasView;
  bool isInitialized();
  void initializeWdw(gsysHierarchy* ownHier, vector<gsysHierarchy*> allHierarchies, vector<gsysConnection*> allConnections, bool toShow = true);
  gsysHierarchy* getOwnHier();

  public slots:

  void saveButton_clicked();

  protected:

  QVBoxLayout *hierWinLayout;

  protected slots:

  void languageChange();

};


#endif

