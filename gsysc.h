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
      This file defines all necessary macros to simplify the 
      usage of gSysC for the programmer. 
 
 *****************************************************************************/


#ifndef GSYSC_H
#define GSYSC_H

#include "systemc.h"


/*****************************************************************************
  include gSysC-files
 *****************************************************************************/
#include "gsysMain.h"
#include "gsysSimulator.h"
#include "gsysMainGUI.h"
#include "gsysBuffer.h"

#ifndef GSYS_DO_NOT_USE
#include "gsysReplaced.h"
#endif

#include <Qt/qapplication.h>
#include <Qt/qtranslator.h>
#include <Qt/qtextcodec.h>



/*****************************************************************************
 
                            Macro Definitions
 
 *****************************************************************************/

/*############################################################################
                     Interface macros (functional)
 ############################################################################*/

#ifndef GSYS_DO_NOT_USE

  #define gsys_start(stepcount) \
	QApplication app(argc,argv); \
	QTranslator translator( 0 ); \
	translator.load( QString("gsysc_") + QTextCodec::locale(), "."); \
	app.installTranslator( &translator ); \
	gsysMainGUI* wdw = (new gsysMain())->getMainWindow(); \
	app.setMainWidget(wdw); \
	wdw->show(); \
	wdw->startSimulator(stepcount); \
	app.exec();
  #define sc_start(stepcount) \
	QApplication app(argc,argv); \
	QTranslator translator( 0 ); \
	translator.load( QString("gsysc_") + QTextCodec::locale(), "."); \
	app.installTranslator( &translator ); \
	gsysMainGUI* wdw = (new gsysMain())->getMainWindow(); \
	app.setMainWidget(wdw); \
	wdw->show(); \
	wdw->startSimulator(stepcount); \
	app.exec();
  #define REAL_sc_start(stepcount) sc_start(stepcount)	
  #define REG_IN_PORT(port,module,signal) \
	(new gsysMain())->getRegModule()->registerPort(port,module,signal);
  #define REG_OUT_PORT(port,module,signal) \
	(new gsysMain())->getRegModule()->registerPort(port,module,signal);
  #define REG_INOUT_PORT(port,module,signal) \
	(new gsysMain())->getRegModule()->registerPort(port,module,signal);
  #define REG_PORT(port,module,signal) \
	(new gsysMain())->getRegModule()->registerPort(port,module,signal);
  #define REG_MODULE(module,name,parent) \
	(new gsysMain())->getRegModule()->registerModule(module,name,parent);
  #define RENAME_PORT(renameObject,name) \
  	(new gsysMain())->getRegModule()->rename_port(renameObject,name);
  #define RENAME_SIGNAL(renameObject,name) \
  	(new gsysMain())->getRegModule()->rename_sig(renameObject,name);
  #define gsys_open \
	QApplication app(argc,argv); \
	gsysMainGUI* wdw = (new gsysMain())->getMainWindow(); \
	app.setMainWidget(wdw); \
	wdw->show(); \
  	wdw->openMainWdw(); \
	app.exec();
  #define gsys_addBuffer(id,name) \
  	(new gsysMain())->getRegModule()->addBuffer(id,name,true);
  #define gsys_addBufferN(id,name) \
  	(new gsysMain())->getRegModule()->addBuffer(id,name,false);
  #define gsys_refreshBuffer(id,akt,max) \
  	(new gsysMain())->refreshBuffer(id,akt,max);

#else	

  #define gsys_start(stepcount) sc_start(stepcount)
  #define sc_start(stepcount) sc_start(stepcount)
  #define REAL_sc_start(stepcount) sc_start(stepcount)	
  #define REG_IN_PORT(port,module,signal) ; 
  #define REG_OUT_PORT(port,module,signal) ; 
  #define REG_INOUT_PORT(port,module,signal) ; 
  #define REG_PORT(port,module,signal) ; 
  #define REG_MODULE(module,name,parent) ; 
  #define RENAME_PORT(renameObject,name) ;
  #define RENAME_SIGNAL(renameObject,name) ;
  #define gsys_open ;
  #define gsys_addBuffer(id,name) ;
  #define gsys_addBufferN(id,name) ;
  #define gsys_refreshBuffer(id,akt,max) ;

#endif  



/*############################################################################
                     Interface Macros (non-functional)
 ############################################################################*/
 
#define GSYS_MODULE(parameter) SC_MODULE(parameter)
#define GSYS_CTOR(parameter) SC_CTOR(parameter)
#define GSYS_METHOD(parameter) SC_METHOD(parameter)
#define GSYS_THREAD(parameter) SC_THREAD(parameter)
#define GSYS_CTHREAD(par1,par2) SC_CTHREAD(par1,par2)


#ifndef GSYS_DO_NOT_USE
    #define gsys_in_clk 	gsys_in<bool> 
    #define gsys_inout_clk 	gsys_inout<bool> 
    #define gsys_out_clk 	gsys_out<bool> 
    #define sc_in_clk 		gsys_in<bool> 
    #define sc_inout_clk 	gsys_inout<bool> 
    #define sc_out_clk 		gsys_out<bool> 
    #define sc_inout		gsys_inout
    #define sc_in		gsys_in
    #define sc_out		gsys_out
    #define sc_signal 		gsys_signal
#else
    #define gsys_inout  	sc_inout
    #define gsys_in		sc_in
    #define gsys_out		sc_out
    #define gsys_inout_clk	sc_inout_clk
    #define gsys_in_clk		sc_in_clk
    #define gsys_out_clk	sc_out_clk
    #define gsys_signal 	sc_signal
#endif
  
#endif
