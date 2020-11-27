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

#include "gsysSimulator.moc"
#include "gsysSimulator.h"
#include "gsysMainGUI.h"  
#include "gsysPortViewer.h"
#include "gsysConnection.h"
#include "gsysHierarchyTree.h"
#include "gsysRegister.h"
#include "gsysMain.h"
#include "gsysPort.h"

  /*
   *  Constructs a gsysSimulator as a child of 'parent', with the
   *  name 'name' and widget flags set to 'f'.
   *
   *  The dialog will by default be modeless, unless you set 'modal' to
   *  TRUE to construct a modal dialog.
   */
  gsysSimulator::gsysSimulator( QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl )
	    : QDialog( parent, name, modal, fl )
  {
    if ( !name )
       setName( "gsysSimulator" );
    gsysSimulatorLayout = new QVBoxLayout( this, 11, 6, "gsysSimulatorLayout");
    
    
    /** Initialization block **/
    aktStep = 0;
    #ifdef DEBUG_GSYSC
    cout<<"Address of 'aktStep' is "<<&aktStep<<endl;
    #endif
    toStop = false;
    hierTree = 0;
	

    groupBox1 = new QGroupBox( this, "groupBox1" );
    groupBox1->setColumnLayout(0, Qt::Vertical );
    groupBox1->layout()->setSpacing( 6 );
    groupBox1->layout()->setMargin( 11 );
    groupBox1Layout = new QHBoxLayout( groupBox1->layout() );
    groupBox1Layout->setAlignment( Qt::AlignTop );
    QSpacerItem* spacer = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox1Layout->addItem( spacer );

    textLabel1 = new QLabel( groupBox1, "textLabel1" );
    groupBox1Layout->addWidget( textLabel1 );

    stepSpin = new QSpinBox( groupBox1, "stepSpin" );
    stepSpin->setMaxValue( 1000 );
    stepSpin->setMinValue( 1 );
    groupBox1Layout->addWidget( stepSpin );
    QSpacerItem* spacer_2 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox1Layout->addItem( spacer_2 );

    textLabel2 = new QLabel( groupBox1, "textLabel2" );
    groupBox1Layout->addWidget( textLabel2 );

    maxSpin = new QSpinBox( groupBox1, "maxSpin" );
    maxSpin->setMaxValue( 999999999 );
    maxSpin->setMinValue( 1 );
    maxSpin->setValue( 1000 );
    groupBox1Layout->addWidget( maxSpin );
    QSpacerItem* spacer_3 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox1Layout->addItem( spacer_3 );
    gsysSimulatorLayout->addWidget( groupBox1 );

    groupBox2 = new QGroupBox( this, "groupBox2" );
    groupBox2->setColumnLayout(0, Qt::Vertical );
    groupBox2->layout()->setSpacing( 6 );
    groupBox2->layout()->setMargin( 11 );
    groupBox2Layout = new QHBoxLayout( groupBox2->layout() );
    groupBox2Layout->setAlignment( Qt::AlignTop );
    QSpacerItem* spacer_4 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox2Layout->addItem( spacer_4 );

    stepButton = new QPushButton( groupBox2, "stepButton" );
    groupBox2Layout->addWidget( stepButton );
    
    QSpacerItem* spacer_9_2 = new QSpacerItem( 8, 40, QSizePolicy::Fixed, QSizePolicy::Fixed );
    groupBox2Layout->addItem( spacer_9_2 );

    startButton = new QPushButton( groupBox2, "startButton" );
    groupBox2Layout->addWidget( startButton );

    stopButton = new QPushButton( groupBox2, "stopButton" );
    groupBox2Layout->addWidget( stopButton );
    QSpacerItem* spacer_5 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox2Layout->addItem( spacer_5 );
    gsysSimulatorLayout->addWidget( groupBox2 );

    groupBox3 = new QGroupBox( this, "groupBox3" );
    groupBox3->setColumnLayout(0, Qt::Vertical );
    groupBox3->layout()->setSpacing( 6 );
    groupBox3->layout()->setMargin( 11 );
    groupBox3Layout = new QVBoxLayout( groupBox3->layout() );
    groupBox3Layout->setAlignment( Qt::AlignTop );

    useViewer = new QCheckBox( groupBox3, "useViewer" );
    useViewer->setChecked( FALSE );
    groupBox3Layout->addWidget( useViewer );
						
    autoStep = new QCheckBox( groupBox3, "autoStep" );
    groupBox3Layout->addWidget( autoStep );

    groupBox4 = new QGroupBox( groupBox3, "groupBox4" );
    groupBox4->setEnabled( FALSE );
    groupBox4->setColumnLayout(0, Qt::Vertical );
    groupBox4->layout()->setSpacing( 6 );
    groupBox4->layout()->setMargin( 11 );
    groupBox4Layout = new QHBoxLayout( groupBox4->layout() );
    groupBox4Layout->setAlignment( Qt::AlignTop );
    QSpacerItem* spacer_6 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox4Layout->addItem( spacer_6 );

    textLabel3 = new QLabel( groupBox4, "textLabel3" );
    textLabel3_2 = new QLabel( groupBox4, "textLabel3_2" );
    textLabel3_2->setText(" [1/10 s]");
    groupBox4Layout->addWidget( textLabel3 );
    groupBox4Layout->addItem( spacer_9_2 );
    intervalSpin = new QSpinBox( groupBox4, "intervalSpin" );
    intervalSpin->setEnabled( FALSE );
    intervalSpin->setValue(10);
    intervalSpin->setMinValue( 1 );
    
    groupBox4Layout->addWidget( intervalSpin );
    groupBox4Layout->addWidget( textLabel3_2 );
    QSpacerItem* spacer_7 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox4Layout->addItem( spacer_7 );
    groupBox3Layout->addWidget( groupBox4 );
    gsysSimulatorLayout->addWidget( groupBox3 );
    QSpacerItem* spacer_8 = new QSpacerItem( 20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding );
    gsysSimulatorLayout->addItem( spacer_8 );
   

    useWatcher = new QCheckBox( groupBox3, "useWatcher" );
    groupBox3Layout->addWidget( useWatcher );

    groupBox5 = new QGroupBox( groupBox3, "groupBox5" );
    groupBox5->setEnabled( FALSE );
    groupBox5->setColumnLayout(0, Qt::Vertical );
    groupBox5->layout()->setSpacing( 6 );
    groupBox5->layout()->setMargin( 11 );
    groupBox5Layout = new QHBoxLayout( groupBox5->layout() );
    groupBox5Layout->setAlignment( Qt::AlignTop );
    QSpacerItem* spacer_9 = new QSpacerItem( 3, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox5Layout->addItem( spacer_9 );

    watchVariable = new QComboBox( FALSE, groupBox5, "watchVariable" );
    watchVariable->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)0, 0, 0, watchVariable->sizePolicy().hasHeightForWidth() ) );
    watchVariable->setMinimumSize( QSize( 0, 0 ) );
    groupBox5Layout->addWidget( watchVariable );

    textLabel1_2 = new QLabel( groupBox5, "textLabel1_2" );
    groupBox5Layout->addWidget( textLabel1_2 );

    watchValue = new QLineEdit( groupBox5, "watchValue" );
    groupBox5Layout->addWidget( watchValue );
    QSpacerItem* spacer_10 = new QSpacerItem( 3, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox5Layout->addItem( spacer_10 );
    groupBox3Layout->addWidget( groupBox5 );
    QSpacerItem* spacer_11 = new QSpacerItem( 20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding );
    gsysSimulatorLayout->addItem( spacer_11 );

   
    languageChange();
    resize( QSize(323, 350).expandedTo(minimumSizeHint()) );

    // signals and slots connections
    connect( autoStep, SIGNAL( stateChanged(int) ), this, SLOT( autoStep_stateChanged(int) ) );
    connect( useWatcher, SIGNAL( stateChanged(int) ), this, SLOT( useWatcher_stateChanged(int) ) );
    connect( startButton, SIGNAL( clicked() ), this, SLOT( startButton_clicked() ) );
    connect( stopButton, SIGNAL( clicked() ), this, SLOT( stopButton_clicked() ) );
    connect( stepButton, SIGNAL( clicked() ), this, SLOT( stepButton_clicked() ) );
    connect( maxSpin, SIGNAL( valueChanged(int) ), this, SLOT( maxSpin_valueChanged(int) ) );
    connect( useViewer, SIGNAL( stateChanged(int) ), this, SLOT( useViewer_stateChanged(int) ) );

    pthread_mutex_init(&run_mutex,NULL); // = PTHREAD_MUTEX_INITIALIZER;
  }

  /*
   *   method to check, whether the stop button was pressed and the simulation
   *   has to be stopped
   */
  bool gsysSimulator::haveToStop()
  {
    return toStop;
  }

  /*
  *  Destroys the object and frees any allocated resources
  */
  gsysSimulator::~gsysSimulator()
  {
    // no need to delete child widgets, Qt does it all for us
    delete hierTree; hierTree = 0;
    watchPorts.clear();
  }

  /*
   *   EventHandler for the checkbox 'use HierarchyViewer'
   */
  void gsysSimulator::useViewer_stateChanged( int )
  {
    if(!useViewer->isChecked())
      (new gsysMain())->getRegModule()->deHighlightAll();
  }

  /*
   *   method to open the simulation window,
   *   initializes the maxValue SpinButton to steps
   */
  void gsysSimulator::openSim(int steps)
  {
    if (steps>0)
    {
      maxSpin->setValue(steps);
      stepSpin->setMaxValue(steps);
    }
    this->show();
  }

  /*
   *   method to start a simulation process over stepCount simulation steps
   */
  void gsysSimulator::start(int timeValue, sc_core::sc_time_unit unit)
  {
    if (!haveToStop() && timeValue>0)
    {
      sc_start(timeValue, unit);
      aktStep += timeValue;   
      if(autoStep->isChecked())
      {
        setCaption( tr( "Simulator - steps done: " ).append(asChar(aktStep)) );
	(new gsysMain())->getMainWindow()->getPortViewer()->refresh();
        if(useViewer->isChecked())
          (new gsysMain())->getRegModule()->showChanges();
      }	
      if (useWatcher->isChecked())   // use port observation
      {
        if (strcmp(watchPorts[watchVariable->currentItem()]->getValue(),watchValue->text()) == 0)
	{
	  toStop = true;
	  hardStop = true;
          setCaption( tr( "Simulator - steps done: " ).append(asChar(aktStep)) );
	  (new gsysMain())->getMainWindow()->getPortViewer()->refresh();
          if(useViewer->isChecked())
            (new gsysMain())->getRegModule()->showChanges();
	}  
      }
    }
    if (haveToStop() && !hardStop)
    {
      // do value-Save for being able to detect changes in the last step
      (new gsysMain())->getRegModule()->saveAllPortValues();
      aktStep++;   
      hardStop = true;
      setCaption( tr( "Simulator - steps done: " ).append(asChar(aktStep)) );
      (new gsysMain())->getMainWindow()->getPortViewer()->refresh();
      if(useViewer->isChecked())
        (new gsysMain())->getRegModule()->showChanges();
    }
  }

  /*
   *   stops the simulation, means, the toStop variable is set and 
   *   the next time it is evaluated, the simulation stops
   */
  void gsysSimulator::stop()
  {
    toStop = true;
  }

  /*
   *   EventHandler for changes on the spinButton for maximum step
   */
  void gsysSimulator::maxSpin_valueChanged( int )
  {
    if (aktStep + stepSpin->value() > maxSpin->value())
    {
      stepSpin->setValue(maxSpin->value() - aktStep);
    }
  }

  /*
   *   EventHandler for changes on the checkbox for 'use AutoStep'
   */
  void gsysSimulator::autoStep_stateChanged( int )
  {
    if (autoStep->isChecked())
    {
      groupBox4->setEnabled(true);
      textLabel3->setEnabled(true);
      intervalSpin->setEnabled(true);
    }
    else
    {
      groupBox4->setEnabled(false);
    }
  }

  /*
   *   EventHandler for the checkbox 'abort simulation if ...'
   */
  void gsysSimulator::useWatcher_stateChanged( int )
  {
    if (useWatcher->isChecked())
    {
      groupBox5->setEnabled(true);
    }
    else
    {
      groupBox5->setEnabled(false);
    }
  }

  /*
   *   EventHandler for button click on the start button
   */
  void gsysSimulator::startButton_clicked()
  {
    int steps = maxSpin->value() - aktStep;
    toStop = false;
    hardStop = false;
    pthread_t tid;
    pthread_create(&tid, NULL, startThread, &steps);
  }

  /*
   *   EventHandler for button click on the stop button
   */
  void gsysSimulator::stopButton_clicked()
  {
    stop();
  }
    
  /*
   *   method to translate the given int value 'nr' to a c_string
   */
  char* gsysSimulator::asChar(int nr)
  {
    ostringstream ostr;
    ostr << nr << ends;
    return (char*) ostr.str().c_str();
  }

  /*
   *   EventHandler for button click of 'single step' button
   */
  void gsysSimulator::stepButton_clicked()
  {
    toStop = (stepSpin->value() == 1);
    hardStop = false;
    start(stepSpin->value());
    if (aktStep > maxSpin->value()) { maxSpin->setValue(aktStep); }
    setCaption( tr( "Simulator - steps done: " ).append(asChar(aktStep)) );
    (new gsysMain())->getMainWindow()->getPortViewer()->refresh();
  }

  /*
   *   method to add the port, 'port' points to, 
   *   to the list of ports, that can be watched for equality with
   *   the value in the text field for the option 'abort simulation if ...'
   */
  void gsysSimulator::addPort(gsysPort* port)
  {
    watchPorts.push_back(port);
    watchVariable->insertItem(port->getName());
  }

  /*
   *   method to clear the comboBox watchPorts of all port names
   */
  void gsysSimulator::clearPorts()
  {
    watchPorts.clear();
  }

  /*
   *  Sets the strings of the subwidgets using the current
   *  language.
   */
  void gsysSimulator::languageChange()
  {
    setCaption( tr( "Simulator - steps done: " ).append(asChar(aktStep)) );
    groupBox1->setTitle( tr( "Iteration interval", "important values..." ) );
    textLabel1->setText( tr( "Number of steps" ) );
    textLabel2->setText( tr( "Maximum" ) );
    groupBox2->setTitle( tr( "Controlling", "Simulation control" ) );
    stepButton->setText( tr( "S&ingle step" ) );
    startButton->setText( tr( "&Start" ) );
    stopButton->setText( tr( "S&top" ) );
    groupBox3->setTitle( tr( "Options", "additional preferences" ) );
    useViewer->setText( tr( "Use &Hierarchy-Viewer" ) );
    autoStep->setText( tr( "&Automated steps" ) );
    groupBox4->setTitle( QString::null );
    textLabel3->setText( tr( "Time interval" ) );
    useWatcher->setText( tr( "A&bort simulation, if ..." ) );
    groupBox5->setTitle( QString::null );
    textLabel1_2->setText( "=" );
 }

