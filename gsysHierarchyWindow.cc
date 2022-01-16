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

#include "gsysHierarchyWindow.moc"
#include "gsysHierarchyWindow.h"
#include "gsysSignalViewer.h"
#include "gsysHierarchy.h"
#include "gsysConnection.h"
#include "gsysSignal.h"
#include "gsysPort.h"
#include "gsysCanvasView.h"
#include "gsysRegister.h"
#include "QtGui/qpicture.h"
#include "QtGui/qpainter.h"
#include <QtWidgets/qfiledialog.h>
#include <QtWidgets/qmessagebox.h>

  /*
   *   method to read the config file gsysHViewer.conf
   *   and set all recognized parameters with the settings in the file
   */
  void gsysHierarchyWindow::readConfig()
  {
    FILE *fp;
	std::string filePath = __FILE__;
	filePath.erase(filePath.find("gsysHierarchyWindow.cc"), 22);
	filePath.append("gsysHViewer.conf");
    fp = fopen(filePath.c_str(),"r");
    if(fp!=0)
    {
      backgroundColor=(char*)"#FFFFFF";
      moduleColor=(char*)"#DBDBDB";
      moduleWithChild=(char*)"#9999CC";
      normalSignal=(char*)"#000000";
      normalNode=(char*)"#000000";
      textColor=(char*)"#000000";
      activeSignal=(char*)"#FF7F00";
      changedSignal=(char*)"#FF0000";
      activeChangedSig=(char*)"#FF00FF";
      inPort=(char*)"#00DD00";
      outPort=(char*)"#0000FF";
      inoutPort=(char*)"#00DDFF";
      mmHierConnColor=(char*)"#FFFF00";
      mmHierConnShow = true;
      mmSigPortShow = true;
      sideMargin=30;
      topMargin=30;
      horizontalSpace=51;
      verticalSpace=51;
      moduleWidth=100;
      moduleHeight=80;
      verticalModuleWidth=(int) moduleWidth*0.5;
      seperatorHeight=2*moduleHeight + topMargin;	
      char zeile[50];
      while(!feof(fp))
      {
        fgets(zeile,50,fp);
        string aktLine(zeile);
	#ifdef DEBUG_GSYSC
	cout<<"Read config file: \n\tline='"<<zeile<<"', \n\tas string='"<<aktLine<<"'"<<endl;
	#endif
	if (aktLine.find(";") != string::npos) aktLine=aktLine.substr(0,aktLine.find(";"));
	#ifdef DEBUG_GSYSC
	cout << "shortened form of aktLine:  '"<<aktLine<<"'"<<endl;
	#endif
	if(aktLine.find("//") != string::npos)
	  aktLine = aktLine.substr(0,aktLine.find("//")-1);
	if(aktLine.substr(0,15).compare("backgroundColor") == 0) 
	  if(aktLine.substr(16,7).length() == 7) { backgroundColor = new char[8]; strncpy(backgroundColor,(char*) aktLine.substr(16,7).data(),7); backgroundColor[7]='\0'; }
	if(aktLine.substr(0,11).compare("moduleColor") == 0) 
	  if(aktLine.substr(12,7).length() == 7) { moduleColor = new char[8]; strncpy(moduleColor,(char*) aktLine.substr(12,7).data(),7); moduleColor[7]='\0'; }
	if(aktLine.substr(0,15).compare("moduleWithChild") == 0) 
	  if(aktLine.substr(16,7).length() == 7) { moduleWithChild = new char[8]; strncpy(moduleWithChild,(char*) aktLine.substr(16,7).data(),7); moduleWithChild[7]='\0'; }
	if(aktLine.substr(0,12).compare("normalSignal") == 0) 
	  if(aktLine.substr(13,7).length() == 7) { normalSignal = new char[8]; strncpy(normalSignal,(char*) aktLine.substr(13,7).data(),7); normalSignal[7]='\0'; }
	if(aktLine.substr(0,10).compare("normalNode") == 0) 
	  if(aktLine.substr(11,7).length() == 7) { normalNode = new char[8]; strncpy(normalNode,(char*) aktLine.substr(11,7).data(),7); normalNode[7]='\0'; }
	if(aktLine.substr(0,9).compare("textColor") == 0) 
	  if(aktLine.substr(10,7).length() == 7) { textColor = new char[8]; strncpy(textColor,(char*) aktLine.substr(10,7).data(),7); textColor[7]='\0'; }
	if(aktLine.substr(0,12).compare("activeSignal") == 0) 
	  if(aktLine.substr(13,7).length() == 7) { activeSignal = new char[8]; strncpy(activeSignal,(char*) aktLine.substr(13,7).data(),7); activeSignal[7]='\0'; }
	if(aktLine.substr(0,13).compare("changedSignal") == 0) 
	  if(aktLine.substr(14,7).length() == 7) { changedSignal = new char[8]; strncpy(changedSignal,(char*) aktLine.substr(14,7).data(),7); changedSignal[7]='\0'; }
	if(aktLine.substr(0,16).compare("activeChangedSig") == 0) 
	  if(aktLine.substr(17,7).length() == 7) { activeChangedSig = new char[8]; strncpy(activeChangedSig,(char*) aktLine.substr(17,7).data(),7); activeChangedSig[7]='\0'; }
	if(aktLine.substr(0,6).compare("inPort") == 0) 
	  if(aktLine.substr(7,7).length() == 7) { inPort = new char[8]; strncpy(inPort,(char*) aktLine.substr(7,7).data(),7); inPort[7]='\0'; }
	if(aktLine.substr(0,7).compare("outPort") == 0) 
	  if(aktLine.substr(8,7).length() == 7) { outPort = new char[8]; strncpy(outPort,(char*) aktLine.substr(8,7).data(),7); outPort[7]='\0'; }
	if(aktLine.substr(0,9).compare("inoutPort") == 0) 
	  if(aktLine.substr(10,7).length() == 7) { inoutPort = new char[8]; strncpy(inoutPort,(char*) aktLine.substr(10,7).data(),7); inoutPort[7]='\0'; }
	if(aktLine.substr(0,15).compare("mmHierConnColor") == 0) 
	  if(aktLine.substr(16,7).length() == 7) { mmHierConnColor = new char[8]; strncpy(mmHierConnColor,(char*) aktLine.substr(16,7).data(),7); mmHierConnColor[7]='\0'; }
	
	if(aktLine.substr(0,14).compare("mmHierConnShow") == 0) 
	  if(atoi(aktLine.substr(15,1).data()) > 0) { mmHierConnShow = (atoi((const char*) (aktLine.substr(15,1).data())) > 0); }
	if(aktLine.substr(0,13).compare("mmSigPortShow") == 0) 
	  if(atoi(aktLine.substr(14,1).data()) > 0) { mmSigPortShow = (atoi((const char*) (aktLine.substr(14,1).data())) > 0); }
	if(aktLine.substr(0,10).compare("sideMargin") == 0) 
	  if(atoi(aktLine.substr(11,4).data()) > 0) { sideMargin = atoi((const char*) (aktLine.substr(11,4).data())); }
	if(aktLine.substr(0,9).compare("topMargin") == 0) 
	  if(atoi(aktLine.substr(10,4).data()) > 0) { topMargin = atoi((const char*) (aktLine.substr(10,4).data())); }
	if(aktLine.substr(0,15).compare("horizontalSpace") == 0) 
	  if(atoi(aktLine.substr(16,4).data()) > 0) { 
	    horizontalSpace = atoi((const char*) (aktLine.substr(16,4).data())); 
	    if(horizontalSpace % 2 == 0) horizontalSpace++;
	  }
	if(aktLine.substr(0,13).compare("verticalSpace") == 0) 
	  if(atoi(aktLine.substr(14,4).data()) > 0) { 
	    verticalSpace = atoi((const char*) (aktLine.substr(14,4).data())); 
	    if(verticalSpace % 2 == 0) verticalSpace++;
	  }
	if(aktLine.substr(0,11).compare("moduleWidth") == 0) 
	  if(atoi(aktLine.substr(12,4).data()) > 0) { moduleWidth = atoi((const char*) (aktLine.substr(12,4).data())); }
	if(aktLine.substr(0,12).compare("moduleHeight") == 0) 
	  if(atoi(aktLine.substr(13,4).data()) > 0) { moduleHeight = atoi((const char*) (aktLine.substr(13,4).data())); }
      }
      fclose(fp);
    }
    else
    {
      backgroundColor=(char*)"#FFFFFF";
      moduleColor=(char*)"#DBDBDB";
      moduleWithChild=(char*)"#9999CC";
      normalSignal=(char*)"#000000";
      normalNode=(char*)"#000000";
      textColor=(char*)"#000000";
      activeSignal=(char*)"#FF7F00";
      changedSignal=(char*)"#FF0000";
      activeChangedSig=(char*)"#FF00FF";
      inPort=(char*)"#00DD00";
      outPort=(char*)"#0000FF";
      inoutPort=(char*)"#00DDFF";
      mmHierConnColor=(char*)"#FFFF00";
      mmHierConnShow = true;
      mmSigPortShow = true;
      sideMargin=30;
      topMargin=30;
      horizontalSpace=51;
      verticalSpace=51;
      moduleWidth=100;
      moduleHeight=80;
      verticalModuleWidth=(int) moduleWidth*0.5;
      seperatorHeight=2*moduleHeight + topMargin;	

      cerr<<"WARNING: The configuration file could not be opened. Probably it does not exist!"<<endl;
    }
    #ifdef DEBUG_GSYSC
    cout<<"Results of reading config file:"<<endl;
    cout<<"\tbgColor="<<backgroundColor<<endl;
    cout<<"\tmodColor="<<moduleColor<<endl;
    cout<<"\tmodWithCh="<<moduleWithChild<<endl;
    cout<<"\tnormSig="<<normalSignal<<endl;
    cout<<"\tnormNode="<<normalNode<<endl;
    cout<<"\ttext="<<textColor<<endl;
    cout<<"\tactive="<<activeSignal<<endl;
    cout<<"\tchanged="<<changedSignal<<endl;
    cout<<"\tactChanged="<<activeChangedSig<<endl;
    cout<<"\tIN="<<inPort<<endl;
    cout<<"\tOUT="<<outPort<<endl;
    cout<<"\tINOUT="<<inoutPort<<endl;
    cout<<"\tsideMargin="<<sideMargin<<endl;
    cout<<"\ttopMargin="<<topMargin<<endl;
    cout<<"\thorSpace="<<horizontalSpace<<endl;
    cout<<"\tvertSpace="<<verticalSpace<<endl;
    cout<<"\tmodWidth="<<moduleWidth<<endl;
    cout<<"\tmodHeight="<<moduleHeight<<endl;
    cout<<"\tmmSigPortShow="<<mmSigPortShow<<endl;
    cout<<"\tmmHierConnShow="<<mmHierConnShow<<endl;
    cout<<"\tmmHierConnColor="<<mmHierConnColor<<endl;
    #endif
    if(mmSigPortShow || mmHierConnShow) 
      { canvasView->setMouseTracking( true ); canvasView->viewport()->setMouseTracking( true ); }
    else 
      { canvasView->setMouseTracking( false ); canvasView->viewport()->setMouseTracking( false ); }
  }

  /*
   *   constructor
   */
  gsysHierarchyWindow::gsysHierarchyWindow( QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl )
  {
    hierarchyList.clear();
    connList.clear();
    sideConnList.clear();
    parent = 0;
    ownHierarchy = 0;
	maxPE = 0;

    canvasView = new gsysCanvasView(new QGraphicsScene(0,0,300,300),this,"Canvas");
    /* Size- and color properties for the structure graphic read out of the configuration file */
    readConfig();
    canvasView->setMM(mmSigPortShow, mmHierConnShow, mmHierConnColor, moduleColor, moduleWithChild);

    if (!name) 
    { 
      setObjectName("HierarchyWindow");
      QWidget::setWindowTitle( name );
    }  

    if(mmSigPortShow || mmHierConnShow)
    {
      moveInfos = new QGroupBox(this);
      moveInfosLayout = new QHBoxLayout( moveInfos );
      moveInfosLayout->layout()->setSpacing( 6 );
      moveInfosLayout->layout()->setMargin( 11 );
      moveInfosLayout->setAlignment( Qt::AlignTop );
      labelKoordinaten = new QLabel("labelKoordinaten", moveInfos);
      coord = new QLineEdit("coord", moveInfos);
      coord->setReadOnly(true);
      labelTyp = new QLabel("labelTyp", moveInfos);
      lineEditTyp = new QLineEdit("lineEditTyp", moveInfos);
      lineEditTyp->setReadOnly(true);
      labelWert = new QLabel("labelWert", moveInfos);
      lineEditWert = new QLineEdit("lineEditWert", moveInfos);
      lineEditWert->setReadOnly(true);
      coord->setText( "0/0" );
      lineEditTyp->setText("");
      lineEditWert->setText("");
      moveInfosLayout->addWidget( labelKoordinaten );
      moveInfosLayout->addWidget( coord );
      moveInfosLayout->addWidget( labelTyp );
      moveInfosLayout->addWidget( lineEditTyp );
      moveInfosLayout->addWidget( labelWert );
      moveInfosLayout->addWidget( lineEditWert );
    }
    
    saveButton = new QPushButton("saveButton", this);
    
    hierWinLayout = new QVBoxLayout(this);
    hierWinLayout->layout()->setSpacing( 6 );
    hierWinLayout->layout()->setMargin( 11 );
    hierWinLayout->addWidget(canvasView);
    hierWinLayout->addWidget(saveButton);

    if(mmHierConnShow || mmSigPortShow)
    { hierWinLayout->addWidget(moveInfos); }
    languageChange();

    // signals and slots connections
    connect( saveButton, SIGNAL( clicked() ), this, SLOT( saveButton_clicked() ) );
  }

  /*
   *   destructor
   */
  gsysHierarchyWindow::~gsysHierarchyWindow()
  {
    this->hide();
    hierarchyList.clear();
    connList.clear();
    sideConnList.clear();
    delete parent; parent = 0;
    delete ownHierarchy; ownHierarchy = 0;
    delete canvasView; canvasView = 0;
    delete backgroundColor; backgroundColor = 0;
    delete moduleColor; moduleColor = 0;
    delete moduleWithChild; moduleWithChild = 0;
    delete textColor; textColor = 0;
    delete normalSignal; normalSignal = 0;
    delete normalNode; normalNode = 0;
    delete activeSignal; activeSignal = 0;
    delete changedSignal; changedSignal = 0;
    delete activeChangedSig; activeChangedSig = 0;
    delete inPort; inPort = 0;
    delete outPort; outPort = 0;
    delete inoutPort; inoutPort = 0;
    delete mmHierConnColor; mmHierConnColor = 0;
    // Qt components get deleted by Qt
  }

  /*
   *   change language to the language of the environment
   */
  void gsysHierarchyWindow::languageChange()
  {
    saveButton->setText( tr("&Save image") );
    if(mmSigPortShow || mmHierConnShow)
    {
      moveInfos->setTitle("");
      labelKoordinaten->setText( tr("Coordinates: ") );
      labelTyp->setText( tr("Type: ") );
      labelWert->setText( tr("Value: ") );
    }
  }

  /*
   *   eventHandler for Button 'save graphics'
   */
  void gsysHierarchyWindow::saveButton_clicked()
  { 
    try {
      QFileDialog *fd = new QFileDialog(this,tr("Save file"));
      fd->setModal(true);
      fd->setFileMode(QFileDialog::AnyFile);
      fd->setViewMode(QFileDialog::List);
      fd->selectFile(tr("Save image"));
	  QString fileTypes = "PNG (*.png *.PNG);;BMP (*.bmp *.BMP);;XBM (*.xbm *.XBM);;XPM (*.xpm *.XPM);;PNM (*.pnm *.PNM);;SVG (*.svg *.SVG)";
	  QString filename = fd->getSaveFileName(this, tr("Save File"), "/Save image.png", fileTypes);
      if((fopen(filename.toLocal8Bit().constData(),"r")!=0 && QMessageBox::warning(this,"gSysC",tr("The chosen file does already exist.\nAre you sure you want to overwrite it?"),QMessageBox::Yes,QMessageBox::No)==QMessageBox::Yes) || fopen(filename.toLocal8Bit().constData(),"r")==0) 
	  {
		std::string type = filename.toStdString().substr(filename.size() - 3).c_str();
		std::transform(type.begin(), type.end(), type.begin(), [](unsigned char c){ return std::toupper(c); });
	  	QString fileType = tr(type.c_str());
	  	if(fileType == "SVG")
	  	{
	  	  cout<<"-- Saving structure graphics in file; SVG-format --"<<endl;
	  	  QPicture *pic = new QPicture();
	  	  QPainter qp;
	  	  qp.begin(pic);
	  	  canvasView->scene()->render(&qp, canvasView->scene()->sceneRect());
	  	  qp.end();
	  	  pic->save(filename,"svg");
	  	  cout<<"saving done."<<endl;
	  	  delete pic;
	  	  pic = 0;
	  	}
	  	else
	  	{
	  	  if(fileType=="PNG" ||
	  	     fileType=="BMP" ||
	  	     fileType=="XBM" ||
	  	     fileType=="XPM" ||
	  	     fileType=="PNM")
	  		{   
	  		  cout<<"-- Saving structure graphics in file; "<<fileType.toLocal8Bit().constData()<<"-format --"<<endl;
	  		  QPixmap *pm = new QPixmap();
	  		  *pm = QPixmap::grabWidget(canvasView->viewport());
	  		  if(pm==0) cout<<"Error: Cannot grab viewport!"<<endl;
	  		  pm->save(filename,fileType.toLocal8Bit().constData());
	  		  cout<<"saving done."<<endl;
	  		  delete pm;
	  		  pm = 0;
	  		}
	  		else QMessageBox::critical(this,"gSysC",tr("You have not given a valid file type. \n\n Please choose a file type of a valid image format!"),QMessageBox::Ok,0);
	  	}
	  }
      
      delete fd;
      fd = 0;
    }
    catch(int e) {
      QMessageBox::critical(this, "gSysC", QString(tr("An error occured.\n\nMaybe the file cannot be written, because you don't have sufficient file system privileges!\n\nThe program returns error code ")).append(e));
    }
  }
 
  /*
   *   get status, whether this class is initialized, i.e. the module
   *   this window should represent is set to the (mem)address of that module
   */
  bool gsysHierarchyWindow::isInitialized()
  {
    return ownHierarchy != 0;
  }

  /*
   *   method to normalize the locale relation of the drawn modules, i.e.
   *   errors because of the rectangular form of the modules are killed
   *   and modules can work with relations calculated with squares 
   */
  QPoint* gsysHierarchyWindow::normalize(QPoint *p)
  {
    QPoint *pn = new QPoint();
    #ifdef DEBUG_GSYSC
    if (p==0) std::cout<<"Normalize: The given point is NULL!!!!!"<<std::endl;
    else std::cout << "Point before normalizing:  "<<p->x()<<"/"<<p->y();
    #endif
    pn->setX((1+p->x()-((int) ceil(0.5*(double)moduleWidth)+sideMargin)) / (moduleWidth+horizontalSpace) +1);
    pn->setY((1+p->y()-((int) ceil(0.5*(double)moduleHeight)+topMargin)) / (moduleHeight+verticalSpace) +1);
    #ifdef DEBUG_GSYSC
    std::cout << "  ||  Point after normalizing:  "<<pn->x()<<"/"<<pn->y()<<std::endl;
    #endif
    return pn;
  }

  /*
   *   method to set all necessary variables to open window
   *   'ownHier' is a pointer to the represented hierarchy module
   *   'allHierarchies' is a list of all hierarchy modules
   *   'allConnections' is a list of all connections
   *   'toShow' is a boolean parameter to control, whether the hierarchyWindow should be shown immediately
   */
  void gsysHierarchyWindow::initializeWdw(gsysHierarchy* ownHier, vector<gsysHierarchy*> allHierarchies, vector<gsysConnection*> allConnections, bool toShow)
  {
    if (ownHier != 0) this->parent = ownHier->getParent();
    else this->parent = 0;
    ownHierarchy = ownHier;

    if (ownHierarchy==0)
    {
      for (int i=0; i<allHierarchies.size(); i++)
        if(allHierarchies[i]->getParent() == 0) 
		{
			pair<gsysHierarchy*, int> p (allHierarchies[i], hierarchyList.size());
			hierarchyList.insert(p);
		}
    }
    else
    {
	  vector<gsysHierarchy *> childList = ownHierarchy->getChildren();
      for (int i=0; i<childList.size(); i++)
		{
			pair<gsysHierarchy*, int> p (childList[i], hierarchyList.size());
			hierarchyList.insert(p);
		}
    }

	
    
    #ifdef DEBUG_GSYSC
    cout<<"    all modules of this level: "<<endl;
    for(int o=0; o<hierarchyList.size(); o++)
      cout <<"\t"<< get<0>(hierarchyList[o])->getName() <<endl;
    #endif

/************************************************************************
	Beginn der Bearbeitung der Connections zwischen den GUI Elementen.
*************************************************************************/
    for (int i=0; i<allConnections.size(); i++)
    {
	  allConnections[i]->getHier1()->addAdjacentHier(allConnections[i]->getHier2());
	  allConnections[i]->getHier2()->addAdjacentHier(allConnections[i]->getHier1());
      #ifdef DEBUG_GSYSC
      std::cout << "Hier1: " << allConnections[i]->getHier1()->getName()
	  			<< " ("<<allConnections[i]->getHier1()
				<< ");  Hier2: "<<allConnections[i]->getHier2()->getName()
				<< " ("<<allConnections[i]->getHier2()
				<< ")" << std::endl;
      #endif
      if(thisLevel(allConnections[i]->getHier1()) && thisLevel(allConnections[i]->getHier2()))
      {
		#ifdef DEBUG_GSYSC
		cout 	<< allConnections[i]->getHier1()->getName()
				<< " AND " << allConnections[i]->getHier2()->getName()
				<< " are in this level!" << endl;
		#endif

		if(connList.find(allConnections[i]->getHier1()) == connList.end())
		{
		  allConnections[i]->setParentWindow(this);
		  connList.insert(pair<gsysHierarchy*, vector<gsysConnection*>> (allConnections[i]->getHier1(), {allConnections[i]}));
		} else {
		  connList.at(allConnections[i]->getHier1()).push_back(allConnections[i]);
		}

		if(connList.find(allConnections[i]->getHier2()) == connList.end())
		{
		  allConnections[i]->setParentWindow(this);
		  connList.insert(pair<gsysHierarchy*, vector<gsysConnection*>> (allConnections[i]->getHier2(), {allConnections[i]}));
		} else {
		  connList.at(allConnections[i]->getHier2()).push_back(allConnections[i]);
		}

      }
      else    	
      {

/************************************************************************
	Hier ist wenigstens ein Verbindungsstück nicht in der GUI 
	enthalten.
*************************************************************************/		  
	    if( (allConnections[i]->getHier1()==ownHierarchy && thisLevel(allConnections[i]->getHier2())) ||
	        (allConnections[i]->getHier2()==ownHierarchy && thisLevel(allConnections[i]->getHier1())) )
	    {
        	#ifdef DEBUG_GSYSC
	    	cout 	<< allConnections[i]->getHier1()->getName()
	    	  		<< " OR "<<allConnections[i]->getHier2()->getName()
	    			<< " is in this level!" << endl;
	    	#endif
	    	for(int u=0; u<sideConnList.size(); u++)
        	if (!(sideConnList[u]==allConnections[i])) sideConnList.push_back(allConnections[i]);
	    }
      }
    }

/************************************************************************
	Erstellen einer Canvas für die GUI Elemente.
*************************************************************************/
	map<gsysHierarchy*, vector<gsysHierarchy*>> sortedChannelElems = sortChannels();
	map<gsysHierarchy*, vector<gsysHierarchy*>> sortedChannelSeperator = sortChannelSeperator(sortedChannelElems);
	map<gsysHierarchy*, vector<gsysHierarchy*>> sortedPESeperator = sortPeSeperator(sortedChannelElems);

    canvasView->resize((maxPE+1)*moduleWidth + (maxPE+2)*sideMargin, 
					   (sortedChannelElems.size()*2) * moduleHeight + (sortedChannelElems.size()*2) * topMargin);
    canvasView->scene()->setSceneRect(0, 0, (maxPE+1)*moduleWidth + (maxPE+2)*sideMargin,
											moduleHeight + (sortedChannelElems.size()*2) * moduleHeight + (sortedChannelElems.size()*2) * topMargin);
	canvasView->setAlignment(Qt::AlignTop|Qt::AlignLeft);
	QPalette palette;
    palette.setColor(canvasView->backgroundRole(), backgroundColor);
    canvasView->setPalette(palette);

    vector<QGraphicsRectItem*> modRect;			// Vector fuer die Module
    vector<QGraphicsSimpleTextItem*> modText;	// Vector fuer deren Text
    QRectF textRect;
    int x=sideMargin;
    int y=topMargin;
	int item = 0;
	int channelWidth = maxPE * moduleWidth; 
	int outerChannelHeight = (moduleHeight*2 + topMargin*2) * sortedChannelElems.size();


/************************************************************************
	Hier werden aktuell die Rechtecke, welche die eigentlichen 
	Module Hierarchyobjekte darstellen, erstellt und platziert.
*************************************************************************/

	for(int i = 0; i < outerChannel.size(); i++)
	{
		// Outer channel seperator
		drawModuleSquaresVertical(outerChannel[i], x, y, verticalModuleWidth, outerChannelHeight);

		// Coordinates of the outer channel seperator on the right side
		x += sideMargin*4 + channelWidth + verticalModuleWidth*2;
	}

	x=sideMargin*2 + verticalModuleWidth;

	for(map<gsysHierarchy*, vector<gsysHierarchy*>>::iterator iter = sortedChannelSeperator.begin(); iter != sortedChannelSeperator.end(); iter++)
	{
		// Channel seperator
		drawModuleSquaresVertical(iter->first, x, y, verticalModuleWidth, seperatorHeight);

		// Coordinates of the next channel seperator 
		y += seperatorHeight + topMargin;
	}

	x += verticalModuleWidth + 2*sideMargin + channelWidth;
	y = topMargin;

	for(map<gsysHierarchy*, vector<gsysHierarchy*>>::iterator iter = sortedPESeperator.begin(); iter != sortedPESeperator.end(); iter++)
	{	
		// PE seperator
		drawModuleSquaresVertical(iter->first, x, y, verticalModuleWidth, seperatorHeight);

		// Coordinates of the next PE seperator 
		y += seperatorHeight + topMargin;
	}

	x = 2*verticalModuleWidth + 3*sideMargin;
	y = topMargin;

	for(map<gsysHierarchy*, vector<gsysHierarchy*>>::iterator iter = sortedChannelElems.begin(); iter != sortedChannelElems.end(); iter++)
	{
		// Channel
		drawModuleSquaresHorizontal(iter->first, x, y, channelWidth, moduleHeight);

		// Coordinates of the next PE row
		y += moduleHeight + topMargin;

		// PE's
		for(int i = 0; i < iter->second.size(); i++)
		{
		  drawModuleSquaresHorizontal(iter->second[i], x, y, moduleWidth, moduleHeight);

		  // Coordinates of the next PE
		  x += moduleWidth + sideMargin;
		}

		// Coordinates of the next channel
		y += moduleHeight + topMargin;
		x = 2*verticalModuleWidth + 3*sideMargin;
	}	
		
	modRect.clear();
    modText.clear();

    // Port placement
    #ifdef DEBUG_GSYSC
    std::cout << "\tSize of ConnList" << connList.size() << std::endl;
    #endif
    short steigung = 0;
    double rand1;
    vector<gsysSignal*> sigList;
    bool lu = true;

/************************************************************************
	Erstellen der Verbindungsports an den jeweiligen Modulen.
*************************************************************************/

	// Coordinates of the bottom left corner of the incoming channel
	x = 2*verticalModuleWidth + 3*sideMargin;
	y = topMargin + moduleHeight;
	drawModulePorts(sortedChannelElems, gsysHierarchy::moduleType::INCOMING_CHANNEL, x, y);

/************************************************************************
	Methode zum zeichnen der letztendlichen Verbindungslinien 
	in der GUI. 
*************************************************************************/
    drawConnections();

    if (toShow) this->show();
  }

  void gsysHierarchyWindow::drawModulePorts(map<gsysHierarchy*, vector<gsysHierarchy*>> elems, gsysHierarchy::moduleType k, int x, int y)
  {
	if(k == gsysHierarchy::moduleType::INCOMING_CHANNEL) 
	{
	  for(map<gsysHierarchy*, vector<gsysHierarchy*>>::iterator iter = elems.begin(); iter != elems.end(); iter++)
		for(int i = 0; i < iter->second.size(); i++)							// Go through the PE
		  {
		    for(int j = 0; j < connList.at(iter->first).size(); j++)				// Go through the connections
		      if(connList.at(iter->first)[j]->getHier1() == iter->second[i])		// Found the connection to the PE here...
		      {
		  	  connList.at(iter->first)[j]->setNode1(new QPoint((int) (x += moduleWidth*0.3), y));
		      }
		      else if(connList.at(iter->first)[j]->getHier2() == iter->second[i])	// ...or here
		      {
		  	  connList.at(iter->first)[j]->setNode2(new QPoint((int) (x += moduleWidth*0.3), y));
		      }
			x += (int) (moduleWidth*0.4) + sideMargin;
		  }
	}
  }

  void gsysHierarchyWindow::drawModuleSquaresHorizontal(gsysHierarchy* elem, int x, int y, int moduleWidth, int moduleHeight)
  {
	modRect.push_back(new QGraphicsRectItem(0, 0, moduleWidth, moduleHeight));
	elem->setHierRect(modRect.back());
	modText.push_back(new QGraphicsSimpleTextItem());

	// draw module
    if(elem->getChildren().size()>0)
    {
    	modRect[item]->setBrush(QBrush(QColor(moduleWithChild)));
    	modRect[item]->setPen(QPen(QColor(moduleWithChild)));
    }
    else   		// no sub hierarchy
    {
    	modRect[item]->setBrush(QBrush(QColor(moduleColor)));
    	modRect[item]->setPen(QPen(QColor(moduleColor)));
    }
	modRect[item]->setX(x);
    modRect[item]->setY(y);
    modRect[item]->setZValue(210);
    modRect[item]->show();
	elem->setCenterPoint(new QPoint(x+(int) ceil(0.5*(double)moduleWidth)-1,y+(int) ceil(0.5*(double)moduleHeight)-1));
	#ifdef DEBUG_GSYSC
    	std::cout << "Module " << elem->getName()
		<<" has center point " << elem->getCenterPoint()->x() 
		<< "/" << elem->getCenterPoint()->y() << std::endl;
    #endif
	modText[item]->setText(elem->getName());
    modText[item]->setBrush(QBrush(QColor(textColor)));
    textRect = modText[item]->boundingRect();
    modText[item]->setX(x+(int) (0.5*(double)moduleWidth-0.5*(double)textRect.width()));
    modText[item]->setY(y+ (int) (0.5*(double)moduleHeight-0.5*(double)textRect.height()));
    modText[item]->setZValue(240);
    modText[item]->show(); 
			
	canvasView->scene()->addItem(modText[item]);	
	canvasView->scene()->addItem(modRect[item]);
	item++;
  }

  void gsysHierarchyWindow::drawModuleSquaresVertical(gsysHierarchy* elem, int x, int y, int moduleWidth, int moduleHeight)
  {
	modRect.push_back(new QGraphicsRectItem(0, 0, moduleWidth, moduleHeight));
	elem->setHierRect(modRect.back());
	modText.push_back(new QGraphicsSimpleTextItem());

	// draw module
    if(elem->getChildren().size()>0)
    {
    	modRect[item]->setBrush(QBrush(QColor(moduleWithChild)));
    	modRect[item]->setPen(QPen(QColor(moduleWithChild)));
    }
    else   		// no sub hierarchy
    {
    	modRect[item]->setBrush(QBrush(QColor(moduleColor)));
    	modRect[item]->setPen(QPen(QColor(moduleColor)));
    }
	modRect[item]->setX(x);
    modRect[item]->setY(y);
    modRect[item]->setZValue(210);
    modRect[item]->show();
	elem->setCenterPoint(new QPoint(x+(int) ceil(0.5*(double)verticalModuleWidth)-1,y+(int) ceil(0.5*(double)seperatorHeight)-1));
	#ifdef DEBUG_GSYSC
    	std::cout << "Module " << elem->getName()
		<<" has center point " << elem->getCenterPoint()->x() 
		<< "/" << elem->getCenterPoint()->y() << std::endl;
    #endif
	modText[item]->setText(elem->getName());
	modText[item]->setRotation(90);
    modText[item]->setBrush(QBrush(QColor(textColor)));
    textRect = modText[item]->boundingRect();
    modText[item]->setX(x+ sideMargin);
    modText[item]->setY(y+ topMargin);
    modText[item]->setZValue(240);
    modText[item]->show(); 
			
	canvasView->scene()->addItem(modText[item]);	
	canvasView->scene()->addItem(modRect[item]);
	item++;
  }

  /*
   *   draw all ports and their connection to the nodes of the built network
   */
  void gsysHierarchyWindow::drawNetConns(gsysHierarchy* hier)
  {
    #ifdef DEBUG_GSYSC
    std::cout<<"\n#####\nHierarchyWindow.drawNetConns for "<<hier->getName()<<":\n"<<"\tPorts:  N->"<<hier->getNPorts().size()<<"  E->"<<hier->getEPorts().size()<<"  S->"<<hier->getSPorts().size()<<"  W->"<<hier->getWPorts().size()<<std::endl;
    #endif
    vector<QGraphicsLineItem*> lines;
    vector<QGraphicsPolygonItem*> polygons;
    int abstand;
    int x = hier->getCenterPoint()->x() - (int) ceil((double)moduleWidth*0.5) +1;
    int y = hier->getCenterPoint()->y() - (int) ceil((double)moduleHeight*0.5) +1;
    vector<gsysPort*> ports = hier->getNPorts();
    abstand = (int) floor(((double)moduleWidth-ports.size()*7.0) / ((double) ports.size()+1.0));
    if(abstand<0) { if(abstand<=-7) abstand=-6; x-=abstand; }

    #ifdef DEBUG_GSYSC
    std::cout<<"-----  N-Ports  -----"<<std::endl;
    #endif
    for(int i=0; i<ports.size(); i++)
    {
      x += abstand;

      #ifdef DEBUG_GSYSC
      std::cout<<"Port-Nr "<<i<<" with name "<<ports[i]->getName()<<" is of type "<<ports[i]->getType()<<", x:"<<x<<", y:"<<y<<" || initDir: "<<ports[i]->getInitDir()<<",  dest: "<<ports[i]->getDest().x()<<"/"<<ports[i]->getDest().y()<<",  Dest is set? "<<!ports[i]->getDest().isNull()<<std::endl;
      #endif
      if(ports[i]->getType() == 0)   	// IN-Port
      {
        polygons.push_back(new QGraphicsPolygonItem());
		QPolygon pa(5);
		pa[0] = QPoint(x,y);
		pa[1] = QPoint(x+6,y);
		pa[2] = QPoint(x+6,y+7);
		pa[3] = QPoint(x+3,y+9);
		pa[4] = QPoint(x,y+7);
		polygons[polygons.size()-1]->setPolygon(pa);
		polygons[polygons.size()-1]->setPen(QPen(QColor(inPort)));
		polygons[polygons.size()-1]->setBrush(QBrush(QColor(inPort)));
		polygons[polygons.size()-1]->setZValue(220);
		polygons[polygons.size()-1]->show();
		canvasView->scene()->addItem(polygons[polygons.size()-1]);
		ports[i]->addLineElem(polygons.back());
      }
      if(ports[i]->getType() == 1)   	// OUT-Port
      {
        polygons.push_back(new QGraphicsPolygonItem());
		QPolygon pa(5);
		pa[0] = QPoint(x+3,y);
		pa[1] = QPoint(x+6,y+2);
		pa[2] = QPoint(x+6,y+9);
		pa[3] = QPoint(x,y+9);
		pa[4] = QPoint(x,y+2);
		polygons[polygons.size()-1]->setPolygon(pa);
		polygons[polygons.size()-1]->setPen(QPen(QColor(outPort)));
		polygons[polygons.size()-1]->setBrush(QBrush(QColor(outPort)));
		polygons[polygons.size()-1]->setZValue(220);
		polygons[polygons.size()-1]->show();
		canvasView->scene()->addItem(polygons[polygons.size()-1]);
		ports[i]->addLineElem(polygons.back());
      }
      if(ports[i]->getType() == 2)   	// INOUT-Port
      {
        polygons.push_back(new QGraphicsPolygonItem());
	QPolygon pa(6);
		pa[0] = QPoint(x+3,y);
		pa[1] = QPoint(x+6,y+2);
		pa[2] = QPoint(x+6,y+7);
		pa[3] = QPoint(x+3,y+9);
		pa[4] = QPoint(x,y+7); 
		pa[5] = QPoint(x,y+2);
		polygons[polygons.size()-1]->setPolygon(pa);
		polygons[polygons.size()-1]->setPen(QPen(QColor(inoutPort)));
		polygons[polygons.size()-1]->setBrush(QBrush(QColor(inoutPort)));
		polygons[polygons.size()-1]->setZValue(220);
		polygons[polygons.size()-1]->show();
		canvasView->scene()->addItem(polygons[polygons.size()-1]);
		ports[i]->addLineElem(polygons.back());
      }
      if(!ports[i]->isEndPort())
      {
        lines.push_back(new QGraphicsLineItem());
        lines.back()->setLine(x+3,y,x+3,y-((verticalSpace-16)/2));
        ports[i]->addLineElem((QAbstractGraphicsShapeItem*)lines.back());
        lines.back()->show();
		canvasView->scene()->addItem(lines.back());
        lines.push_back(new QGraphicsLineItem());
        if (ports[i]->getInitDir() == W) 
          lines.back()->setLine(x+3,y-((verticalSpace-16)/2),hier->getCenterPoint()->x()-(int)floor((double)moduleWidth*0.5)-(int)floor((double)horizontalSpace*0.5),y-((verticalSpace-16)/2));
        else	
          lines.back()->setLine(x+3,y-((verticalSpace-16)/2),hier->getCenterPoint()->x()+(int)ceil((double)moduleWidth*0.5)+(int)floor((double)horizontalSpace*0.5),y-((verticalSpace-16)/2));
        ports[i]->addLineElem((QAbstractGraphicsShapeItem*)lines.back());
        lines.back()->show();
		canvasView->scene()->addItem(lines.back());
      }	
      x += 7;	
    }
    
    x = hier->getCenterPoint()->x() - (int) ceil((double)moduleWidth*0.5)+1;
    y = hier->getCenterPoint()->y() + (int) floor((double)moduleHeight*0.5);;
    ports = hier->getSPorts();
    abstand = (int) floor(((double)moduleWidth-ports.size()*7.0) / ((double) ports.size()+1.0));
    if(abstand<0) { if(abstand<=-7) abstand=-6; x-=abstand; }

    #ifdef DEBUG_GSYSC
    std::cout<<"-----  S-Ports  -----"<<std::endl;
    #endif
    for(int i=0; i<ports.size(); i++)
    {
      x += abstand;

      #ifdef DEBUG_GSYSC
      std::cout<<"Port-Nr "<<i<<" with name "<<ports[i]->getName()<<" is of type "<<ports[i]->getType()<<", x:"<<x<<", y:"<<y<<" || initDir: "<<ports[i]->getInitDir()<<",  dest: "<<ports[i]->getDest().x()<<"/"<<ports[i]->getDest().y()<<",  Dest is set? "<<!ports[i]->getDest().isNull()<<std::endl;
      #endif
      if(ports[i]->getType() == 0)   	// IN-Port
      {
        polygons.push_back(new QGraphicsPolygonItem());
		QPolygon pa(5);
		pa[0] = QPoint(x,y);
		pa[1] = QPoint(x+6,y);
		pa[2] = QPoint(x+6,y-7);
		pa[3] = QPoint(x+3,y-9);
		pa[4] = QPoint(x,y-7);
		polygons[polygons.size()-1]->setPolygon(pa);
		polygons[polygons.size()-1]->setPen(QPen(QColor(inPort)));
		polygons[polygons.size()-1]->setBrush(QBrush(QColor(inPort)));
		polygons[polygons.size()-1]->setZValue(220);
		polygons[polygons.size()-1]->show();
		canvasView->scene()->addItem(polygons[polygons.size()-1]);
		ports[i]->addLineElem(polygons.back());
      }
      if(ports[i]->getType() == 1)   	// OUT-Port
      {
        polygons.push_back(new QGraphicsPolygonItem());
		QPolygon pa(5);
		pa[0] = QPoint(x+3,y);
		pa[1] = QPoint(x+6,y-2);
		pa[2] = QPoint(x+6,y-9);
		pa[3] = QPoint(x,y-9);
		pa[4] = QPoint(x,y-2);
		polygons[polygons.size()-1]->setPolygon(pa);
		polygons[polygons.size()-1]->setPen(QPen(QColor(outPort)));
		polygons[polygons.size()-1]->setBrush(QBrush(QColor(outPort)));
		polygons[polygons.size()-1]->setZValue(220);
		polygons[polygons.size()-1]->show();
		canvasView->scene()->addItem(polygons[polygons.size()-1]);
		ports[i]->addLineElem(polygons.back());
      }
      if(ports[i]->getType() == 2)   	// INOUT-Port
      {
        polygons.push_back(new QGraphicsPolygonItem());
		QPolygon pa(6);
		pa[0] = QPoint(x+3,y);
		pa[1] = QPoint(x+6,y-2);
		pa[2] = QPoint(x+6,y-7);
		pa[3] = QPoint(x+3,y-9);
		pa[4] = QPoint(x,y-7); 
		pa[5] = QPoint(x,y-2);
		polygons[polygons.size()-1]->setPolygon(pa);
		polygons[polygons.size()-1]->setPen(QPen(QColor(inoutPort)));
		polygons[polygons.size()-1]->setBrush(QBrush(QColor(inoutPort)));
		polygons[polygons.size()-1]->setZValue(220);
		polygons[polygons.size()-1]->show();
		canvasView->scene()->addItem(polygons[polygons.size()-1]);
		ports[i]->addLineElem(polygons.back());
      }
      
      if(!ports[i]->isEndPort())
      {
        lines.push_back(new QGraphicsLineItem());
        lines.back()->setLine(x+3,y,x+3,y+((verticalSpace-16)/2));
        ports[i]->addLineElem((QAbstractGraphicsShapeItem*)lines.back());
        lines.back()->show();
		canvasView->scene()->addItem(lines.back());
        lines.push_back(new QGraphicsLineItem());
        if (ports[i]->getInitDir() == W) 
          lines.back()->setLine(x+3,y+((verticalSpace-16)/2),hier->getCenterPoint()->x()-(int)floor((double)moduleWidth*0.5)-(int)floor((double)horizontalSpace*0.5),y+((verticalSpace-16)/2));
        else	
          lines.back()->setLine(x+3,y+((verticalSpace-16)/2),hier->getCenterPoint()->x()+(int)ceil((double)moduleWidth*0.5)+(int)floor((double)horizontalSpace*0.5),y+((verticalSpace-16)/2));
        ports[i]->addLineElem((QAbstractGraphicsShapeItem*)lines.back());
        lines.back()->show();
		canvasView->scene()->addItem(lines.back());
      }	
      x += 7;	
    }
    
    x = hier->getCenterPoint()->x() - (int) ceil((double)moduleWidth*0.5) +1;
    y = hier->getCenterPoint()->y() - (int) ceil((double)moduleHeight*0.5) +1;
    ports = hier->getWPorts();
    abstand = (int) floor(((double) moduleHeight-ports.size()*7.0) / ((double) ports.size()+1.0));
    if(abstand<0) { if(abstand<=-7) abstand=-6; y-=abstand; }

    #ifdef DEBUG_GSYSC
    std::cout<<"-----  W-Ports  -----"<<std::endl;
    #endif
    for(int i=0; i<ports.size(); i++)
    {
      y += abstand;

      #ifdef DEBUG_GSYSC
      std::cout<<"Port-Nr "<<i<<" with name "<<ports[i]->getName()<<" is of type "<<ports[i]->getType()<<", x:"<<x<<", y:"<<y<<" || initDir: "<<ports[i]->getInitDir()<<",  dest: "<<ports[i]->getDest().x()<<"/"<<ports[i]->getDest().y()<<",  Dest is set? "<<!ports[i]->getDest().isNull()<<std::endl;
      #endif
      if(ports[i]->getType() == 0)   	// IN-Port
      {
        polygons.push_back(new QGraphicsPolygonItem());
		QPolygon pa(5);
		pa[0] = QPoint(x,y);
		pa[1] = QPoint(x+7,y);
		pa[2] = QPoint(x+9,y+3);
		pa[3] = QPoint(x+7,y+6);
		pa[4] = QPoint(x,y+6);
		polygons[polygons.size()-1]->setPolygon(pa);
		polygons[polygons.size()-1]->setPen(QPen(QColor(inPort)));
		polygons[polygons.size()-1]->setBrush(QBrush(QColor(inPort)));
		polygons[polygons.size()-1]->setZValue(220);
		polygons[polygons.size()-1]->show();
		canvasView->scene()->addItem(polygons[polygons.size()-1]);
		ports[i]->addLineElem(polygons.back());
      }
      if(ports[i]->getType() == 1)   	// OUT-Port
      {
        polygons.push_back(new QGraphicsPolygonItem());
		QPolygon pa(5);
		pa[0] = QPoint(x,y+3);
		pa[1] = QPoint(x+2,y);
		pa[2] = QPoint(x+9,y);
		pa[3] = QPoint(x+9,y+6);
		pa[4] = QPoint(x+2,y+6);
		polygons[polygons.size()-1]->setPolygon(pa);
		polygons[polygons.size()-1]->setPen(QPen(QColor(outPort)));
		polygons[polygons.size()-1]->setBrush(QBrush(QColor(outPort)));
		polygons[polygons.size()-1]->setZValue(220);
		polygons[polygons.size()-1]->show();
		canvasView->scene()->addItem(polygons[polygons.size()-1]);
		ports[i]->addLineElem(polygons.back());
      }
      if(ports[i]->getType() == 2)   	// INOUT-Port
      {
        polygons.push_back(new QGraphicsPolygonItem());
		QPolygon pa(6);
		pa[0] = QPoint(x,y+3);
		pa[1] = QPoint(x+2,y);
		pa[2] = QPoint(x+7,y);
		pa[3] = QPoint(x+9,y+3);
		pa[4] = QPoint(x+7,y+6); 
		pa[5] = QPoint(x+2,y+6);
		polygons[polygons.size()-1]->setPolygon(pa);
		polygons[polygons.size()-1]->setPen(QPen(QColor(inoutPort)));
		polygons[polygons.size()-1]->setBrush(QBrush(QColor(inoutPort)));
		polygons[polygons.size()-1]->setZValue(220);
		polygons[polygons.size()-1]->show();
		canvasView->scene()->addItem(polygons[polygons.size()-1]);
		ports[i]->addLineElem(polygons.back());
      }
      if(! ports[i]->isEndPort())
      {
        if(ports[i]->getDest().isNull())
        {
          lines.push_back(new QGraphicsLineItem());
          lines.back()->setLine(x,y+3,x-((horizontalSpace-16)/2),y+3);
          ports[i]->addLineElem((QAbstractGraphicsShapeItem*)lines.back());
	  	  lines.back()->show();
		  canvasView->scene()->addItem(lines.back());
          lines.push_back(new QGraphicsLineItem());
          if (ports[i]->getInitDir() == N) 
  	  	  {
            lines.back()->setLine(x-((horizontalSpace-16)/2),y+3,x-((horizontalSpace-16)/2),hier->getCenterPoint()->y()-(int)floor((double)moduleHeight*0.5)-(int)floor((double)verticalSpace*0.5));
	    	lines.back()->show();
			canvasView->scene()->addItem(lines.back());
	      }  
          else	
 	      {
            lines.back()->setLine(x-((horizontalSpace-16)/2),y+3,x-((horizontalSpace-16)/2),hier->getCenterPoint()->y()+(int)ceil((double)moduleHeight*0.5)+(int)floor((double)verticalSpace*0.5));
	    	lines.back()->show();
			canvasView->scene()->addItem(lines.back());
	  	  }  
        }	
        else
        {
          lines.push_back(new QGraphicsLineItem());
	  	  lines.back()->setLine(x,y+3,ports[i]->getDest().x(),ports[i]->getDest().y());
	  	  lines.back()->show();
		  canvasView->scene()->addItem(lines.back());
        }
        ports[i]->addLineElem((QAbstractGraphicsShapeItem*)lines.back());
      }
      y += 7;	
    }
    
    x = hier->getCenterPoint()->x() + (int) floor((double)moduleWidth*0.5);
    y = hier->getCenterPoint()->y() - (int) ceil((double)moduleHeight*0.5) +1;
    ports = hier->getEPorts();
    abstand = (int) floor(((double) moduleHeight-ports.size()*7.0) / ((double) ports.size()+1.0));
    if(abstand<0) { if(abstand<=-7) abstand=-6; y-=abstand; }

    #ifdef DEBUG_GSYSC
    std::cout<<"-----  E-Ports  -----"<<std::endl;
    #endif
    for(int i=0; i<ports.size(); i++)
    {
      y += abstand;
      
      #ifdef DEBUG_GSYSC
      std::cout<<"Port-Nr "<<i<<" with name "<<ports[i]->getName()<<" is of type "<<ports[i]->getType()<<", x:"<<x<<", y:"<<y<<" || initDir: "<<ports[i]->getInitDir()<<",  dest: "<<ports[i]->getDest().x()<<"/"<<ports[i]->getDest().y()<<",  Dest is set? "<<!ports[i]->getDest().isNull()<<std::endl;
      #endif
      if(ports[i]->getType() == 0)   	// IN-Port
      {
        polygons.push_back(new QGraphicsPolygonItem());
		QPolygon pa(5);
		pa[0] = QPoint(x,y);
		pa[1] = QPoint(x-7,y);
		pa[2] = QPoint(x-9,y+3);
		pa[3] = QPoint(x-7,y+6);
		pa[4] = QPoint(x,y+6);
		polygons[polygons.size()-1]->setPolygon(pa);
		polygons[polygons.size()-1]->setPen(QPen(QColor(inPort)));
		polygons[polygons.size()-1]->setBrush(QBrush(QColor(inPort)));
		polygons[polygons.size()-1]->setZValue(220);
		polygons[polygons.size()-1]->show();
		canvasView->scene()->addItem(polygons[polygons.size()-1]);
		ports[i]->addLineElem(polygons.back());
      }
      if(ports[i]->getType() == 1)   	// OUT-Port
      {
        polygons.push_back(new QGraphicsPolygonItem());
		QPolygon pa(5);
		pa[0] = QPoint(x,y+3);
		pa[1] = QPoint(x-2,y);
		pa[2] = QPoint(x-9,y);
		pa[3] = QPoint(x-9,y+6);
		pa[4] = QPoint(x-2,y+6);
		polygons[polygons.size()-1]->setPolygon(pa);
		polygons[polygons.size()-1]->setPen(QPen(QColor(outPort)));
		polygons[polygons.size()-1]->setBrush(QBrush(QColor(outPort)));
		polygons[polygons.size()-1]->setZValue(220);
		polygons[polygons.size()-1]->show();
		canvasView->scene()->addItem(polygons[polygons.size()-1]);
		ports[i]->addLineElem(polygons.back());
      }
      if(ports[i]->getType() == 2)   	// INOUT-Port
      {
        polygons.push_back(new QGraphicsPolygonItem());
		QPolygon pa(6);
		pa[0] = QPoint(x,y+3);
		pa[1] = QPoint(x-2,y);
		pa[2] = QPoint(x-7,y);
		pa[3] = QPoint(x-9,y+3);
		pa[4] = QPoint(x-7,y+6); 
		pa[5] = QPoint(x-2,y+6);
		polygons[polygons.size()-1]->setPolygon(pa);
		polygons[polygons.size()-1]->setPen(QPen(QColor(inoutPort)));
		polygons[polygons.size()-1]->setBrush(QBrush(QColor(inoutPort)));
		polygons[polygons.size()-1]->setZValue(220);
		polygons[polygons.size()-1]->show();
		canvasView->scene()->addItem(polygons[polygons.size()-1]);
		ports[i]->addLineElem(polygons.back());
      }

      if(!ports[i]->isEndPort())
      {
	if(ports[i]->getDest().isNull())
	{
	  lines.push_back(new QGraphicsLineItem());
	  lines.back()->setLine(x,y+3,x+((horizontalSpace-16)/2),y+3);
	  ports[i]->addLineElem((QAbstractGraphicsShapeItem*)lines.back());
	  lines.back()->show();
	  canvasView->scene()->addItem(lines.back());
	  lines.push_back(new QGraphicsLineItem());
	  if (ports[i]->getInitDir() == N) 
	  {
	    lines.back()->setLine(x+((horizontalSpace-16)/2),y+3,x+((horizontalSpace-16)/2),hier->getCenterPoint()->y()-(int)floor((double)moduleHeight*0.5)-(int)floor((double)verticalSpace*0.5));
	    lines.back()->show();
		canvasView->scene()->addItem(lines.back());
	  }  
	  else	
	  {
	    lines.back()->setLine(x+((horizontalSpace-16)/2),y+3,x+((horizontalSpace-16)/2),hier->getCenterPoint()->y()+(int)ceil((double)moduleHeight*0.5)+(int)floor((double)verticalSpace*0.5));
	    lines.back()->show();
		canvasView->scene()->addItem(lines.back());
	  }  
	}	
	else
	{
	  lines.push_back(new QGraphicsLineItem());
	  lines.back()->setLine(x,y+3,ports[i]->getDest().x(),ports[i]->getDest().y());
	  lines.back()->show();
	  canvasView->scene()->addItem(lines.back());
	}
	ports[i]->addLineElem((QAbstractGraphicsShapeItem*)lines.back());
      }
      y += 7;	
    }

    lines.clear();
    polygons.clear();
    ports.clear();
  }

  /*
   *	check, whether a hierarchy is on this level
   *	'hier' is the hierarchy element to check   
   */
  bool gsysHierarchyWindow::thisLevel(gsysHierarchy* hier)
  {
	for(map<gsysHierarchy*, int>::iterator iter = hierarchyList.begin(); iter != hierarchyList.end(); iter++)
		if(iter->first == hier)
			return true;
    return false;
  }

  /*
   *   draw the way of the connections through the network
   *   'connection' is a pointer to the connection, that should be drawn
   *   'p1', 'p2' are start and stop points of the (remaining way of the) connection
   *   'lfdNr' is a counter to prevent infinite loops
   */
  bool gsysHierarchyWindow::drawConnStep(gsysConnection* connection, QPoint *p1, QPoint *p2,int lfdNr)
  {
	  return true;
  }

  /*
   *   main method to draw connections
   */
  void gsysHierarchyWindow::drawConnections()
  {
    for(int i=0; i<connList.size(); i++)
    {
      //drawConnStep(connList[i],connList[i]->getNode1(),connList[i]->getNode2(),1);
    }
  }

  /*
   *   method to draw the side ports
   */
  void gsysHierarchyWindow::drawSidePort(gsysPort* port, bool left, int nr, int abstand, int portHeight)
  {
    #ifdef DEBUG_GSYSC
    cout << "drawSidePort left="<<left<<" with number="<<nr<<" and distance "<<abstand<<endl;
    #endif
    int x=0;
    int y=0;
    if (!left) x=2*sideMargin+dimFactor*moduleWidth+(dimFactor-1)*horizontalSpace-1;
    if(abstand<0) { if(abstand<=-21) abstand=-20; y-=abstand; }
    y = (nr+1)*abstand+nr*portHeight;
 
    QFont smallerFont;
    smallerFont.setPointSize(8);
    QGraphicsPolygonItem *polygon = new QGraphicsPolygonItem();
    QGraphicsSimpleTextItem *text = new QGraphicsSimpleTextItem();
    if (left)
    {
      if (port->getType() == 0)
      {
        QPolygon pa(5);
	pa[0] = QPoint(x,y);
	pa[1] = QPoint(x+40,y);
	pa[2] = QPoint(x+50,y+11);
	pa[3] = QPoint(x+40,y+21);
	pa[4] = QPoint(x,y+21);
	polygon->setPolygon(pa);
	polygon->setPen(QPen(QColor(inPort)));
	polygon->setBrush(QBrush(QColor(inPort)));
	polygon->setZValue(220);
	text->setText(port->getName());
	text->setFont(smallerFont);
	text->setX(5);
	text->setY(y+4);
	text->setZValue(240);
	polygon->show();
	text->show();
	canvasView->scene()->addItem(polygon);
	canvasView->scene()->addItem(text);
      }
      if (port->getType() == 1)
      {
        QPolygon pa(5);
	pa[0] = QPoint(x,y+11);
	pa[1] = QPoint(x+10,y);
	pa[2] = QPoint(x+50,y);
	pa[3] = QPoint(x+50,y+21);
	pa[4] = QPoint(x+10,y+21);
	polygon->setPolygon(pa);
	polygon->setPen(QPen(QColor(outPort)));
	polygon->setBrush(QBrush(QColor(outPort)));
	polygon->setZValue(220);
	text->setText(port->getName());
	text->setFont(smallerFont);
	text->setX(5);
	text->setY(y+4);
	text->setZValue(240);
	polygon->show();
	text->show();
	canvasView->scene()->addItem(polygon);
	canvasView->scene()->addItem(text);
      }
      if (port->getType() == 2)
      {
        QPolygon pa(6);
	pa[0] = QPoint(x,y+11);
	pa[1] = QPoint(x+10,y);
	pa[2] = QPoint(x+40,y);
	pa[3] = QPoint(x+50,y+11);
	pa[4] = QPoint(x+40,y+21);
	pa[5] = QPoint(x+10,y+21);
	polygon->setPolygon(pa);
	polygon->setPen(QPen(QColor(inoutPort)));
	polygon->setBrush(QBrush(QColor(inoutPort)));
	polygon->setZValue(220);
	text->setText(port->getName());
	text->setFont(smallerFont);
	text->setX(5);
	text->setY(y+4);
	text->setZValue(240);
	polygon->show();
	text->show();
	canvasView->scene()->addItem(polygon);
	canvasView->scene()->addItem(text);
      }
    }
    else
    {
      if (port->getType() == 0)
      {
        QPolygon pa(5);
	pa[0] = QPoint(x,y);
	pa[1] = QPoint(x-40,y);
	pa[2] = QPoint(x-50,y+11);
	pa[3] = QPoint(x-40,y+21);
	pa[4] = QPoint(x,y+21);
	polygon->setPolygon(pa);
	polygon->setPen(QPen(QColor(inPort)));
	polygon->setBrush(QBrush(QColor(inPort)));
	polygon->setZValue(220);
	text->setText(port->getName());
	text->setFont(smallerFont);
	text->setX(5);
	text->setY(y+4);
	text->setZValue(240);
	polygon->show();
	text->show();
	canvasView->scene()->addItem(polygon);
	canvasView->scene()->addItem(text);
      }
      if (port->getType() == 1)
      {
        QPolygon pa(5);
	pa[0] = QPoint(x,y+11);
	pa[1] = QPoint(x-10,y);
	pa[2] = QPoint(x-50,y);
	pa[3] = QPoint(x-50,y+21);
	pa[4] = QPoint(x-10,y+21);
	polygon->setPolygon(pa);
	polygon->setPen(QPen(QColor(outPort)));
	polygon->setBrush(QBrush(QColor(outPort)));
	polygon->setZValue(220);
	text->setText(port->getName());
	text->setFont(smallerFont);
	text->setX(5);
	text->setY(y+4);
	text->setZValue(240);
	polygon->show();
	text->show();
	canvasView->scene()->addItem(polygon);
	canvasView->scene()->addItem(text);
      }
      if (port->getType() == 2)
      {
        QPolygon pa(6);
	pa[0] = QPoint(x,y+11);
	pa[1] = QPoint(x-10,y);
	pa[2] = QPoint(x-40,y);
	pa[3] = QPoint(x-50,y+11);
	pa[4] = QPoint(x-40,y+21);
	pa[5] = QPoint(x-10,y+21);
	polygon->setPolygon(pa);
	polygon->setPen(QPen(QColor(inoutPort)));
	polygon->setBrush(QBrush(QColor(inoutPort)));
	polygon->setZValue(220);
	text->setText(port->getName());
	text->setFont(smallerFont);
	text->setX(5);
	text->setY(y+4);
	text->setZValue(240);
	polygon->show();
	text->show();
	canvasView->scene()->addItem(polygon);
	canvasView->scene()->addItem(text);
      }
    }
    polygon = 0;
  }
  
  /*
   *   get hierarchy module that is drawn in this window
   */
  gsysHierarchy* gsysHierarchyWindow::getOwnHier()
  {
    return ownHierarchy;
  }  

  /*
   *   	Sort channel and PE elements according to their relative position 
   *	in the GUI.
   */
  map<gsysHierarchy*, vector<gsysHierarchy*>> gsysHierarchyWindow::sortChannels()
  {
	  map<gsysHierarchy*, vector<gsysHierarchy*>> sortList = {};
	  vector<gsysHierarchy*> channels = {};
	  vector<gsysHierarchy*> PE = {};
	  vector<gsysHierarchy*> sortedPE = {};
	  gsysHierarchy* currentChannel;
	  gsysHierarchy* adjSeperator = NULL;
	  int workOnElems = 0;
	  int levelPE = 0;

	  // Filter the relevant items first and pick the incoming channel as first elem in sortList
	  for(map<gsysHierarchy*, int>::iterator iter = hierarchyList.begin(); iter != hierarchyList.end(); iter++)
	  {
		  if(iter->first->getModuleType() == gsysHierarchy::moduleType::INCOMING_CHANNEL)
		  {
			  sortList.insert(pair<gsysHierarchy*, vector<gsysHierarchy*>> (iter->first, {}));
			  currentChannel = iter->first;	// Start at the incoming channel
		  } 
		  else if(iter->first->getModuleType() == gsysHierarchy::moduleType::CHANNEL)
		  {
			channels.push_back(iter->first);
			workOnElems++;
		  } 
		  else if(iter->first->getModuleType() == gsysHierarchy::moduleType::VIRT_CHANNEL)
		  {
			outerChannel.push_back(iter->first);	//Put this channel at the beginning
			std::reverse(outerChannel.begin(), outerChannel.end());
		  }
		  else if(iter->first->getModuleType() == gsysHierarchy::moduleType::PROC_CHANNEL)
		  {
			outerChannel.push_back(iter->first);
		  }
		  else if(iter->first->getModuleType() == gsysHierarchy::moduleType::PE)
		  {
			PE.push_back(iter->first);
			workOnElems++;
		  }
	  }

	  // Process every element 
	  while(workOnElems > 0)
	  {

		// Find PE's which are connected to the currentChannel
	  	for(const auto &item : PE)	
	  	{
		  levelPE++;
		  for(const auto &neighbour : item->getAdjacentHier())	
		  {
		    if(neighbour == currentChannel) // Is the PE connected to currentChannel
		    {
			  // Check for a connection to a seperator
			  //if(sortedPE.size() == 0)
			  //{
			  for(int i = 0; i < item->getAdjacentHier().size(); i++)
			  {
			    if(item->getAdjacentHier()[i]->getModuleType() == gsysHierarchy::moduleType::SEPERATOR)
			    {
			  	  adjSeperator = item->getAdjacentHier()[i];
			  	  break;
			    }
			  }
			  //}

		  	  if(adjSeperator != NULL) 
		  	  { 
				sortedPE.push_back(item); 
			  	adjSeperator = NULL;
			  } 
		  	  else 
		  	  { sortList.at(currentChannel).push_back(item); }
			  workOnElems--;
		  	  break;
		    }
		  }
		  maxPE = (levelPE > maxPE) ? levelPE : maxPE;
	  	}

		// Sort the PE's
		for(int i = 0; i < sortList.at(currentChannel).size(); i++)	
		{	
		  for(const auto &elem : sortedPE[i]->getAdjacentHier())
		  {
			if(elem->getModuleType() == gsysHierarchy::moduleType::PE &&
			   std::find(sortedPE.begin(), sortedPE.end(), elem) == sortedPE.end())
			{
			  sortedPE.push_back(elem);
			  break;
			}
		  }
		}

		std::reverse(sortedPE.begin(), sortedPE.end());
		sortList.at(currentChannel) = sortedPE;

		// Search for the next level channel
		for(const auto &item : sortList.at(currentChannel)[0]->getAdjacentHier()) 
		{
		  if(item->getModuleType() == gsysHierarchy::moduleType::CHANNEL &&
		     currentChannel != item)
		  {
			  currentChannel = item;
			  sortList.insert(pair<gsysHierarchy*, vector<gsysHierarchy*>> (currentChannel, {}));
			  workOnElems--;
			  levelPE = 0;
			  break;
		  }
		}
	  }
	  return sortList;
  }
  
  /*
   *   	Sort channel configuration seperator elements according to their relative position in the GUI.
   */
  map<gsysHierarchy*, vector<gsysHierarchy*>> gsysHierarchyWindow::sortChannelSeperator(map<gsysHierarchy*, vector<gsysHierarchy*>> sortedChannelElems)
  {
	  map<gsysHierarchy*, vector<gsysHierarchy*>> seperatorAndChannel = {};

	  for(map<gsysHierarchy*, vector<gsysHierarchy*>>::iterator iter = sortedChannelElems.begin(); iter != sortedChannelElems.end(); iter++)
	  {
		// Find the seperator of the channel
	    for(const auto &item : iter->first->getAdjacentHier())
		{
		  if(item->getModuleType() == gsysHierarchy::moduleType::SEPERATOR &&
		  	 seperatorAndChannel.find(item) == seperatorAndChannel.end())
		  {
			// Add a new seperator
		  	seperatorAndChannel.insert(pair<gsysHierarchy*, vector<gsysHierarchy*>> (item, {iter->first}));
			break;
		  } 
		  else 
		  {
			// Add the channel to an existing seperator
			//seperatorAndChannel.at(item).push_back(iter->first);
			break;
		  }
		}
	  }
	  return seperatorAndChannel;
  }

  /*
   *   	Sort PE configuration seperator elements according to their relative position in the GUI.
   */
  map<gsysHierarchy*, vector<gsysHierarchy*>> gsysHierarchyWindow::sortPeSeperator(map<gsysHierarchy*, vector<gsysHierarchy*>> sortedChannelElems)
  {
	  gsysHierarchy* rPE;	//Rightmost PE, which is usually connected to a seperator
	  map<gsysHierarchy*, vector<gsysHierarchy*>> seperatorAndPE = {};

	  for(map<gsysHierarchy*, vector<gsysHierarchy*>>::iterator iter = sortedChannelElems.begin(); iter != sortedChannelElems.end(); iter++)
	  {
		rPE = iter->second[iter->second.size() - 1]; 
		// Find the seperator of the PE 
		for(const auto &item : rPE->getAdjacentHier())
		{
		  if(item->getModuleType() == gsysHierarchy::moduleType::SEPERATOR &&
		  	 seperatorAndPE.find(item) == seperatorAndPE.end())
			{
			  // Add a new seperator
   			  seperatorAndPE.insert(pair<gsysHierarchy*, vector<gsysHierarchy*>> (item, {rPE}));
   			  break;
			}
			else 
			{
			  // Add the PE to an existing seperator
			  seperatorAndPE.at(item).push_back(rPE);
			  break;
			}
		}
	  }
	  return seperatorAndPE;
  }