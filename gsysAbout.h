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
      This file provides an about-dialog giving information about
      the library. Therefore the library gets related to the
      institute of computer engineering of the university of Lübeck. 

 ***************************************************************************/


#ifndef GSYSABOUT_H
#define GSYSABOUT_H

#include <QtCore/qvariant.h>
#include <QtGui/qpixmap.h>
#include <QtGui/qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QLabel;

class gsysAbout : public QDialog
{
    friend class gsysMainGUI;

    Q_OBJECT

private:
    gsysAbout( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, Qt::WindowFlags fl = 0 );
    ~gsysAbout();

    QLabel* pixmapLabel1_2;
    QLabel* pixmapLabel1;
    QLabel* textLabel1;

protected:

protected slots:
    virtual void languageChange();
private:
    QPixmap image0;
    QPixmap image1;

};

#endif // GSYSABOUT_H
