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
	    : QDialog( parent, fl )
  {
    setModal(modal);
    if ( !name ) setObjectName( "gsysSimulator" );
    gsysSimulatorLayout = new QVBoxLayout( this );
    gsysSimulatorLayout->setObjectName("gsysSimulatorLayout");
    gsysSimulatorLayout->setSpacing(6);
    gsysSimulatorLayout->setMargin(11);
    
    
    /** Initialization block **/
    aktStep = 0;
    #ifdef DEBUG_GSYSC
    cout<<"Address of 'aktStep' is "<<&aktStep<<endl;
    #endif
    toStop = false;
    hierTree = 0;
	
    groupBox1 = new QGroupBox( "groupBox1", this );
    groupLayout1 = new QGridLayout();
    groupLayout1->addLayout(groupBox1->layout(), 0, 0);
    groupLayout1->setAlignment( Qt::AlignTop );

    textLabel1 = new QLabel( "textLabel1" );
    groupLayout1->addWidget( textLabel1, 0, 0 );

    stepSpin = new QSpinBox();
    stepSpin->setObjectName( "stepSpin" );
    stepSpin->setMaximum( 1000 );
    stepSpin->setMinimum( 1 );
    groupLayout1->addWidget( stepSpin, 0, 1 );

    textLabel2 = new QLabel( "textLabel2" );
    groupLayout1->addWidget( textLabel2, 0, 2 );

    maxSpin = new QSpinBox();
    stepSpin->setObjectName( "maxSpin" );
    maxSpin->setMaximum( 999999999 );
    maxSpin->setMinimum( 1 );
    maxSpin->setValue( 1000 );
    groupLayout1->addWidget( maxSpin, 0, 3 );

    groupBox1->setLayout(groupLayout1);
    groupBox1->layout()->setSpacing( 6 );
    groupBox1->layout()->setMargin( 11 );
    gsysSimulatorLayout->addWidget( groupBox1 );

    groupBox2 = new QGroupBox( "groupBox2", this );
    groupLayout2 = new QGridLayout();
    groupLayout2->addLayout(groupBox2->layout(), 0, 0);
    groupLayout2->setAlignment( Qt::AlignTop );

    stepButton = new QPushButton( "stepButton" );
    groupLayout2->addWidget( stepButton, 0, 0 );
    
    startButton = new QPushButton( "startButton" );
    groupLayout2->addWidget( startButton, 0, 1 );

    stopButton = new QPushButton( "stopButton" );
    groupLayout2->addWidget( stopButton, 0, 2 );

    groupBox2->setLayout(groupLayout2);
    groupBox2->layout()->setSpacing( 6 );
    groupBox2->layout()->setMargin( 11 );
    gsysSimulatorLayout->addWidget( groupBox2 );

    groupBox3 = new QGroupBox( "groupBox3", this );
    groupBox3->setLayout(new QVBoxLayout(groupBox3));
    groupBox3->layout()->setSpacing( 6 );
    groupBox3->layout()->setMargin( 11 );

    useViewer = new QCheckBox( "useViewer", groupBox3 );
    useViewer->setChecked( false );
    groupBox3->layout()->addWidget( useViewer );
						
    autoStep = new QCheckBox( "autoStep", groupBox3 );
    groupBox3->layout()->addWidget( autoStep );

    groupBox4 = new QGroupBox( "groupBox4", groupBox3 );
    groupBox4->setEnabled( false );
    groupBox4->setLayout(new QVBoxLayout(groupBox4));
    groupBox4->layout()->setSpacing( 6 );
    groupBox4->layout()->setMargin( 11 );

    textLabel3 = new QLabel( "textLabel3", groupBox4 );
    textLabel3_2 = new QLabel( "textLabel3_2", groupBox4 );
    textLabel3_2->setText(" [1/10 s]");
    groupBox4->layout()->addWidget( textLabel3 );
    intervalSpin = new QSpinBox( groupBox4 );
    intervalSpin->setObjectName( "intervalSpin" );
    intervalSpin->setEnabled( false );
    intervalSpin->setValue(10);
    intervalSpin->setMinimum( 1 );
    
    groupBox4->layout()->addWidget( intervalSpin );
    groupBox4->layout()->addWidget( textLabel3_2 );
    groupBox3->layout()->addWidget( groupBox4 );
    gsysSimulatorLayout->addWidget( groupBox3 );

    useWatcher = new QCheckBox( "useWatcher", groupBox3 );
    groupBox3->layout()->addWidget( useWatcher );

    groupBox5 = new QGroupBox( "groupBox5", groupBox3 );
    groupBox5->setEnabled( false );
    groupBox5->setLayout(new QVBoxLayout(groupBox5));
    groupBox5->layout()->setSpacing( 6 );
    groupBox5->layout()->setMargin( 11 );

    watchVariable = new QComboBox( groupBox5 );
    watchVariable->setObjectName("watchVariable");

    watchVarPolicy = new QSizePolicy( (QSizePolicy::Policy)1, (QSizePolicy::Policy)0 );
    watchVarPolicy->setHorizontalStretch( 0 );
    watchVarPolicy->setVerticalStretch( 0 );
    watchVarPolicy->setHeightForWidth( watchVariable->sizePolicy().hasHeightForWidth() );
    
    watchVariable->setSizePolicy( *watchVarPolicy );
    watchVariable->setMinimumSize( QSize( 0, 0 ) );
    groupBox5->layout()->addWidget( watchVariable );

    textLabel1_2 = new QLabel( "textLabel1_2", groupBox5 );
    groupBox5->layout()->addWidget( textLabel1_2 );

    watchValue = new QLineEdit( "watchValue", groupBox5 );
    groupBox5->layout()->addWidget( watchValue );
    groupBox3->layout()->addWidget( groupBox5 );
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
      stepSpin->setMaximum(steps);
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
      (new gsysMain())->getRegModule()->saveAllPortValues();
      sc_start(timeValue, unit);
      aktStep += timeValue;   
      if(autoStep->isChecked())
      {
        setWindowTitle( tr( "Simulator - steps done: " ).append(asChar(aktStep)) );
	      (new gsysMain())->getMainWindow()->getPortViewer()->refresh();
        if(useViewer->isChecked())
          (new gsysMain())->getRegModule()->showChanges();
      }	
      
      if (useWatcher->isChecked())   // use port observation
      {
        if (strcmp(watchPorts[watchVariable->currentIndex()]->getValue(),watchValue->text().toLocal8Bit().data()) == 0)
	      {
	        toStop = true;
	        hardStop = true;
          setWindowTitle( tr( "Simulator - steps done: " ).append(asChar(aktStep)) );
	        (new gsysMain())->getMainWindow()->getPortViewer()->refresh();
          (new gsysMain())->getRegModule()->showChanges();
          if(useViewer->isChecked())
            (new gsysMain())->getRegModule()->showChanges();
	      }  
      }
    }
    if (haveToStop() && !hardStop)
    {
      // do value-Save for being able to detect changes in the last step
      (new gsysMain())->getRegModule()->saveAllPortValues();
      sc_start(timeValue, unit);
      aktStep += timeValue;    
      hardStop = true;
      setWindowTitle( tr( "Simulator - steps done: " ).append(asChar(aktStep)) );
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
    toStop = true;
    hardStop = false;
    start(stepSpin->value());
    if (aktStep > maxSpin->value()) { maxSpin->setValue(aktStep); }
  }

  /*
   *   method to add the port, 'port' points to, 
   *   to the list of ports, that can be watched for equality with
   *   the value in the text field for the option 'abort simulation if ...'
   */
  void gsysSimulator::addPort(gsysPort* port)
  {
    watchPorts.push_back(port);
    watchVariable->insertItem(0, port->getName());
  }

  /*
   *   method to clear the comboBox watchPorts of all port names
   */
  void gsysSimulator::clearPorts()
  {
    watchPorts.clear();
  }

  /*
   *   method to check, if the simulation has been stopped
   */
  bool gsysSimulator::stopped()
  {
    return toStop || hardStop;
  }

  /*
   *  Sets the strings of the subwidgets using the current
   *  language.
   */
  void gsysSimulator::languageChange()
  {
    setWindowTitle( tr( "Simulator - steps done: " ).append(asChar(aktStep)) );
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

