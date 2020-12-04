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
    fp = fopen("gsysHViewer.conf","r");
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
      sideMargin=130;
      topMargin=60;
      horizontalSpace=51;
      verticalSpace=51;
      moduleWidth=150;
      moduleHeight=120;
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
	  if(atoi(aktLine.substr(14,1).data()) > 0) { mmSigPortShow = (atoi((const char*) (aktLine.substr(11,4).data())) > 0); }
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
      sideMargin=130;
      topMargin=60;
      horizontalSpace=51;
      verticalSpace=51;
      moduleWidth=150;
      moduleHeight=120;

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
      moveInfos->layout()->setSpacing( 6 );
      moveInfos->layout()->setMargin( 11 );
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
      fd->setNameFilter("BMP (*.bmp *.BMP)");
      fd->setNameFilter("XBM (*.xbm *.XBM)");
      fd->setNameFilter("XPM (*.xpm *.XPM)");
      fd->setNameFilter("SVG (*.svg *.SVG)");
      fd->setNameFilter("PNM (*.pnm *.PNM)");
      fd->setNameFilter("PNG (*.png *.PNG)");
      fd->selectFile(tr("Save image"));
      if(fd->exec() == QDialog::Accepted)
      {
        QString filename = fd->getOpenFileName();
        if((fopen(filename.toLocal8Bit().constData(),"r")!=0 && QMessageBox::warning(this,"gSysC",tr("The chosen file does already exist.\nAre you sure you want to overwrite it?"),QMessageBox::Yes,QMessageBox::No)==QMessageBox::Yes) || fopen(filename.toLocal8Bit().constData(),"r")==0) 
	{
	  QString fileType = fd->selectedNameFilter();
	  if(fileType == "PNG (*.png *.PNG)") fileType="PNG";
	  if(fileType == "BMP (*.bmp *.BMP)") fileType="BMP";
	  if(fileType == "XBM (*.xbm *.XBM)") fileType="XBM";
	  if(fileType == "XPM (*.xpm *.XPM)") fileType="XPM";
	  if(fileType == "PNM (*.pnm *.PNM)") fileType="PNM";
	  if(fileType == "SVG (*.svg *.SVG)")
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
        if(allHierarchies[i]->getParent() == 0) hierarchyList.push_back(allHierarchies[i]);
    }
    else
    {
      hierarchyList = ownHierarchy->getChildren();
    }
    
    #ifdef DEBUG_GSYSC
    cout<<"    all modules of this level: "<<endl;
    for(int o=0; o<hierarchyList.size(); o++)
      cout<<"\t"<<hierarchyList[o]->getName()<<endl;
    #endif

    bool found;
    for (int i=0; i<allConnections.size(); i++)
    {
      #ifdef DEBUG_GSYSC
      std::cout << "Hier1: "<<allConnections[i]->getHier1()->getName()<<" ("<<allConnections[i]->getHier1()<<");  Hier2: "<<allConnections[i]->getHier2()->getName()<<" ("<<allConnections[i]->getHier2()<<")"<<std::endl;
      #endif
      found=false;
      if(thisLevel(allConnections[i]->getHier1()) && thisLevel(allConnections[i]->getHier2()))
      {
        #ifdef DEBUG_GSYSC
	cout<<allConnections[i]->getHier1()->getName()<<" AND "<<allConnections[i]->getHier2()->getName()<<" are in this level!"<<endl;
	#endif
	for(int u=0; u<connList.size(); u++)
	  if(connList[u]==allConnections[i]) found=true;
        if (!found) 
	{
	  #ifdef DEBUG_GSYSC
	  cout << "Address of this: " << this << endl;
	  #endif
	  allConnections[i]->setParentWindow(this);
	  connList.push_back(allConnections[i]);
	}  
      }
      else    	// at least one hierarchy is not in this level
      {
	if( (allConnections[i]->getHier1()==ownHierarchy && thisLevel(allConnections[i]->getHier2())) ||
	    (allConnections[i]->getHier2()==ownHierarchy && thisLevel(allConnections[i]->getHier1())) )
	{
          #ifdef DEBUG_GSYSC
	  cout<<allConnections[i]->getHier1()->getName()<<" OR "<<allConnections[i]->getHier2()->getName()<<" is in this level!"<<endl;
	  #endif
	  for(int u=0; u<sideConnList.size(); u++)
	    if(sideConnList[u]==allConnections[i]) found=true;
          if (!found) sideConnList.push_back(allConnections[i]);
	}
      }
    }

    // Drawing process and forwarding the results
    //     For simplicity X-Y-routing is used as placement strategy!!!
    dimFactor = (int) (ceil(sqrt((double) hierarchyList.size())));
    canvasView->resize(dimFactor*moduleWidth+2*sideMargin+(dimFactor-1)*horizontalSpace+10,dimFactor*moduleHeight+(dimFactor-1)*verticalSpace+2*topMargin+10);
    canvasView->scene()->setSceneRect(0, 0, dimFactor*moduleWidth+2*sideMargin+(dimFactor-1)*horizontalSpace,dimFactor*moduleHeight+(dimFactor-1)*verticalSpace+2*topMargin);
    QPalette palette;
    palette.setColor(canvasView->backgroundRole(), backgroundColor);
    canvasView->setPalette(palette);

    vector<QGraphicsRectItem*> modRect;
    vector<QGraphicsSimpleTextItem*> modText;
    QRectF textRect;
    int x=sideMargin;
    int y=topMargin;
    for (int i=0; i<hierarchyList.size(); i++)
    {
      modRect.push_back(new QGraphicsRectItem(0,0,moduleWidth,moduleHeight));
      hierarchyList[i]->setHierRect(modRect.back());
      modText.push_back(new QGraphicsSimpleTextItem());
      
      // draw module
      if(hierarchyList[i]->getChildren().size()>0)
      {
        modRect[i]->setBrush(QColor(moduleWithChild));
        modRect[i]->setPen(QPen(moduleWithChild));
      }
      else   		// no sub hierarchy
      {
        modRect[i]->setBrush(QColor(moduleColor));
        modRect[i]->setPen(QPen(QColor(moduleColor)));
      }
      modRect[i]->setX(x);
      modRect[i]->setY(y);
      modRect[i]->setZValue(210);
      modRect[i]->show();
      hierarchyList[i]->setCenterPoint(new QPoint(x+(int) ceil(0.5*(double)moduleWidth)-1,y+(int) ceil(0.5*(double)moduleHeight)-1));
      #ifdef DEBUG_GSYSC
      std::cout << "Module "<<hierarchyList[i]->getName()<<" has center point "<<hierarchyList[i]->getCenterPoint()->x()<<"/"<<hierarchyList[i]->getCenterPoint()->y()<<std::endl;
      #endif
      modText[i]->setText(hierarchyList[i]->getName());
      modText[i]->setBrush(QBrush(textColor));
      textRect = modText[i]->boundingRect();
      modText[i]->setX(x+(int) (0.5*(double)moduleWidth-0.5*(double)textRect.width()));
      modText[i]->setY(y+ (int) (0.5*(double)moduleHeight-0.5*(double)textRect.height()));
      modText[i]->setZValue(240);
      modText[i]->show(); 

      // Coordinates of the next module
      x = x+horizontalSpace+moduleWidth;
      if (x > sideMargin+(dimFactor-1)*moduleWidth+(dimFactor-1)*horizontalSpace) 
      {
        x = sideMargin;
	y = y+verticalSpace+moduleHeight; 
      }
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
    for (int co=0; co<connList.size(); co++)
    {
      steigung = getSector(normalize(connList[co]->getHier1()->getCenterPoint()),normalize(connList[co]->getHier2()->getCenterPoint()));
      sigList = connList[co]->getSignals();
      #ifdef DEBUG_GSYSC
      cout<<"Connection between "<<connList[co]->getHier1()->getName()<<" and "<<connList[co]->getHier2()->getName()<<" has the following signals:"<<endl;
      for(int si1=0; si1<sigList.size(); si1++)
        cout<<"\t"<<sigList[si1]->getName()<<endl;
      #endif
      if (steigung == N) 
      {
        #ifdef DEBUG_GSYSC
        std::cout<<connList[co]->getHier1()->getName()<<" and "<<connList[co]->getHier2()->getName()<<" lie to each other in sector \t"<<steigung<<std::endl;
	#endif
	if((connList[co]->getHier1()->getCenterPoint()->x() > sideMargin+(dimFactor-1)*(moduleWidth+horizontalSpace)) || 
	   (connList[co]->getHier1()->getCenterPoint()->x() - (int) ceil((double)moduleWidth*0.5) > sideMargin &&
	    connList[co]->getHier1()->getWPorts().size() + connList[co]->getHier2()->getWPorts().size() <
	    connList[co]->getHier1()->getEPorts().size() + connList[co]->getHier2()->getEPorts().size()))
	  lu = true;      // connection over left side
	else lu = false;  // connection over right side
	if(lu)
	{
	  connList[co]->setNode1(new QPoint(connList[co]->getHier1()->getCenterPoint()->x()-(int)ceil((double)moduleWidth*0.5)+1-(int)ceil((double)horizontalSpace*0.5),connList[co]->getHier1()->getCenterPoint()->y()-(int)ceil((double)moduleHeight*0.5)+1-(int)ceil((double)verticalSpace*0.5)));
	  connList[co]->setNode2(new QPoint(connList[co]->getHier2()->getCenterPoint()->x()-(int)ceil((double)moduleWidth*0.5)+1-(int)ceil((double)horizontalSpace*0.5),connList[co]->getHier2()->getCenterPoint()->y()+(int)floor((double)moduleHeight*0.5)+(int)ceil((double)verticalSpace*0.5)));
	}
	else // !lu
	{
	  connList[co]->setNode1(new QPoint(connList[co]->getHier1()->getCenterPoint()->x()+(int)floor((double)moduleWidth*0.5)+(int)ceil((double)horizontalSpace*0.5),connList[co]->getHier1()->getCenterPoint()->y()-(int)ceil((double)moduleHeight*0.5)+1-(int)ceil((double)verticalSpace*0.5)));
	  connList[co]->setNode2(new QPoint(connList[co]->getHier2()->getCenterPoint()->x()+(int)floor((double)moduleWidth*0.5)+(int)ceil((double)horizontalSpace*0.5),connList[co]->getHier2()->getCenterPoint()->y()+(int)floor((double)moduleHeight*0.5)+(int)ceil((double)verticalSpace*0.5)));
	}
	for(int si=0; si<sigList.size(); si++)
	{
	  if(lu)
	  {
	    if(connList[co]->getHier1()->getNPorts().size() < connList[co]->getHier1()->getWPorts().size())
	      connList[co]->getHier1()->addNPort(sigList[si],W);
	    else
	      if(connList[co]->getHier1()->getNPorts().size() > connList[co]->getHier1()->getWPorts().size())
	        connList[co]->getHier1()->addWPort(sigList[si],N);
	      else
	        if(connList[co]->getHier1()->getNPorts().size() == connList[co]->getHier1()->getWPorts().size())
		{
	          rand1 = rand()/RAND_MAX;
	          if(rand1 >= 0.5) connList[co]->getHier1()->addNPort(sigList[si],W);
	          else connList[co]->getHier1()->addWPort(sigList[si],N);
		}  
	    if(connList[co]->getHier2()->getSPorts().size() < connList[co]->getHier2()->getWPorts().size())
	      connList[co]->getHier2()->addSPort(sigList[si],W);
	    else
	      if(connList[co]->getHier2()->getSPorts().size() > connList[co]->getHier2()->getWPorts().size())
	        connList[co]->getHier2()->addWPort(sigList[si],S);
	      else
	        if(connList[co]->getHier2()->getSPorts().size() == connList[co]->getHier2()->getWPorts().size())
		{
	          rand1 = rand()/RAND_MAX;
	          if(rand1 >= 0.5) connList[co]->getHier2()->addSPort(sigList[si],W);
	          else connList[co]->getHier2()->addWPort(sigList[si],S);
	        }
	  }
	  else	// case: !lu	
	  {
	    if(connList[co]->getHier1()->getNPorts().size() < connList[co]->getHier1()->getEPorts().size())
	      connList[co]->getHier1()->addNPort(sigList[si],E);
	    else
	      if(connList[co]->getHier1()->getNPorts().size() > connList[co]->getHier1()->getEPorts().size())
	        connList[co]->getHier1()->addEPort(sigList[si],N);
	      else
	        if(connList[co]->getHier1()->getNPorts().size() == connList[co]->getHier1()->getEPorts().size())
		{
	          rand1 = rand()/RAND_MAX;
	          if(rand1 >= 0.5) connList[co]->getHier1()->addNPort(sigList[si],E);
	          else connList[co]->getHier1()->addEPort(sigList[si],N);
		}  
	    if(connList[co]->getHier2()->getSPorts().size() < connList[co]->getHier2()->getEPorts().size())
	      connList[co]->getHier2()->addSPort(sigList[si],E);
	    else
	      if(connList[co]->getHier2()->getSPorts().size() > connList[co]->getHier2()->getEPorts().size())
	        connList[co]->getHier2()->addEPort(sigList[si],S);
	      else
	        if(connList[co]->getHier2()->getSPorts().size() == connList[co]->getHier2()->getEPorts().size())
		{
	          rand1 = rand()/RAND_MAX;
	          if(rand1 >= 0.5) connList[co]->getHier2()->addSPort(sigList[si],E);
	          else connList[co]->getHier2()->addEPort(sigList[si],S);
	        }
	  }	
	}
      }	
      if (steigung == NNE || steigung == NE || steigung == ENE) 
      {
        #ifdef DEBUG_GSYSC
        std::cout<<connList[co]->getHier1()->getName()<<" and "<<connList[co]->getHier2()->getName()<<" lie to each other in sector \t"<<steigung<<std::endl;
	#endif
	connList[co]->setNode1(new QPoint(connList[co]->getHier1()->getCenterPoint()->x()+(int)floor((double)moduleWidth*0.5)+(int)ceil((double)horizontalSpace*0.5),connList[co]->getHier1()->getCenterPoint()->y()-(int)ceil((double)moduleHeight*0.5)+1-(int)ceil((double)verticalSpace*0.5)));
	connList[co]->setNode2(new QPoint(connList[co]->getHier2()->getCenterPoint()->x()-(int)ceil((double)moduleWidth*0.5)+1-(int)ceil((double)horizontalSpace*0.5),connList[co]->getHier2()->getCenterPoint()->y()+(int)floor((double)moduleHeight*0.5)+(int)ceil((double)verticalSpace*0.5)));
	for (int si=0; si<sigList.size(); si++)
	{
	    if(connList[co]->getHier1()->getNPorts().size() < connList[co]->getHier1()->getEPorts().size())
	      connList[co]->getHier1()->addNPort(sigList[si],E);
	    else
	      if(connList[co]->getHier1()->getNPorts().size() > connList[co]->getHier1()->getEPorts().size())
	        connList[co]->getHier1()->addEPort(sigList[si],N);
	      else
	        if(connList[co]->getHier1()->getNPorts().size() == connList[co]->getHier1()->getEPorts().size())
		{
	          rand1 = rand()/RAND_MAX;
	          if(rand1 >= 0.5) connList[co]->getHier1()->addNPort(sigList[si],E);
	          else connList[co]->getHier1()->addEPort(sigList[si],N);
		}  
	    if(connList[co]->getHier2()->getSPorts().size() < connList[co]->getHier2()->getWPorts().size())
	      connList[co]->getHier2()->addSPort(sigList[si],W);
	    else
	      if(connList[co]->getHier2()->getSPorts().size() > connList[co]->getHier2()->getWPorts().size())
	        connList[co]->getHier2()->addWPort(sigList[si],S);
	      else
	        if(connList[co]->getHier2()->getSPorts().size() == connList[co]->getHier2()->getWPorts().size())
		{
	          rand1 = rand()/RAND_MAX;
	          if(rand1 >= 0.5) connList[co]->getHier2()->addSPort(sigList[si],W);
	          else connList[co]->getHier2()->addWPort(sigList[si],S);
	        }
	}
      }	
      if (steigung == E) 
      {
        #ifdef DEBUG_GSYSC
        std::cout<<connList[co]->getHier1()->getName()<<" and "<<connList[co]->getHier2()->getName()<<" lie to each other in sector \t"<<steigung<<std::endl;
	#endif
	if(connList[co]->getHier1()->getNPorts().size() + connList[co]->getHier2()->getNPorts().size() <
	   connList[co]->getHier1()->getSPorts().size() + connList[co]->getHier2()->getSPorts().size())
	  lu = true;      // connection over upper side
	else lu = false;  // connection over lower side
	if(lu)
	{
	  connList[co]->setNode1(new QPoint(connList[co]->getHier1()->getCenterPoint()->x()+(int)floor((double)moduleWidth*0.5)+(int)ceil((double)horizontalSpace*0.5),connList[co]->getHier1()->getCenterPoint()->y()-(int)ceil((double)moduleHeight*0.5)+1-(int)ceil((double)verticalSpace*0.5)));
	  connList[co]->setNode2(new QPoint(connList[co]->getHier2()->getCenterPoint()->x()-(int)ceil((double)moduleWidth*0.5)+1-(int)ceil((double)horizontalSpace*0.5),connList[co]->getHier2()->getCenterPoint()->y()-(int)ceil((double)moduleHeight*0.5)+1-(int)ceil((double)verticalSpace*0.5)));
	}
	else // !lu
	{
	  connList[co]->setNode1(new QPoint(connList[co]->getHier1()->getCenterPoint()->x()+(int)floor((double)moduleWidth*0.5)+(int)ceil((double)horizontalSpace*0.5),connList[co]->getHier1()->getCenterPoint()->y()+(int)floor((double)moduleHeight*0.5)+(int)ceil((double)verticalSpace*0.5)));
	  connList[co]->setNode2(new QPoint(connList[co]->getHier2()->getCenterPoint()->x()-(int)ceil((double)moduleWidth*0.5)+1-(int)ceil((double)horizontalSpace*0.5),connList[co]->getHier2()->getCenterPoint()->y()+(int)floor((double)moduleHeight*0.5)+(int)ceil((double)verticalSpace*0.5)));
	}
	for(int si=0; si<sigList.size(); si++)
	{
	  if(lu)
	  {
	    if(connList[co]->getHier1()->getNPorts().size() < connList[co]->getHier1()->getEPorts().size())
	      connList[co]->getHier1()->addNPort(sigList[si],E);
	    else
	      if(connList[co]->getHier1()->getNPorts().size() > connList[co]->getHier1()->getEPorts().size())
	        connList[co]->getHier1()->addEPort(sigList[si],N);
	      else
	        if(connList[co]->getHier1()->getNPorts().size() == connList[co]->getHier1()->getEPorts().size())
		{
	          rand1 = rand()/RAND_MAX;
	          if(rand1 >= 0.5) connList[co]->getHier1()->addNPort(sigList[si],E);
	          else connList[co]->getHier1()->addEPort(sigList[si],N);
		}  
	    if(connList[co]->getHier2()->getNPorts().size() < connList[co]->getHier2()->getWPorts().size())
	      connList[co]->getHier2()->addNPort(sigList[si],W);
	    else
	      if(connList[co]->getHier2()->getNPorts().size() > connList[co]->getHier2()->getWPorts().size())
	        connList[co]->getHier2()->addWPort(sigList[si],N);
	      else
	        if(connList[co]->getHier2()->getNPorts().size() == connList[co]->getHier2()->getWPorts().size())
		{
	          rand1 = rand()/RAND_MAX;
	          if(rand1 >= 0.5) connList[co]->getHier2()->addNPort(sigList[si],W);
	          else connList[co]->getHier2()->addWPort(sigList[si],N);
	        }
	  }	
	  else	// case: !lu	
	  {
	    if(connList[co]->getHier1()->getSPorts().size() < connList[co]->getHier1()->getEPorts().size())
	      connList[co]->getHier1()->addSPort(sigList[si],E);
	    else
	      if(connList[co]->getHier1()->getSPorts().size() > connList[co]->getHier1()->getEPorts().size())
	        connList[co]->getHier1()->addEPort(sigList[si],S);
	      else
	        if(connList[co]->getHier1()->getSPorts().size() == connList[co]->getHier1()->getEPorts().size())
		{
	          rand1 = rand()/RAND_MAX;
	          if(rand1 >= 0.5) connList[co]->getHier1()->addSPort(sigList[si],E);
	          else connList[co]->getHier1()->addEPort(sigList[si],S);
		}  
	    if(connList[co]->getHier2()->getSPorts().size() < connList[co]->getHier2()->getWPorts().size())
	      connList[co]->getHier2()->addSPort(sigList[si],W);
	    else
	      if(connList[co]->getHier2()->getSPorts().size() > connList[co]->getHier2()->getWPorts().size())
	        connList[co]->getHier2()->addWPort(sigList[si],S);
	      else
	        if(connList[co]->getHier2()->getSPorts().size() == connList[co]->getHier2()->getWPorts().size())
		{
	          rand1 = rand()/RAND_MAX;
	          if(rand1 >= 0.5) connList[co]->getHier2()->addSPort(sigList[si],W);
	          else connList[co]->getHier2()->addWPort(sigList[si],S);
	        }
	  }	
	}
      }
      if (steigung == ESE || steigung == SE || steigung == SSE) 
      {
        #ifdef DEBUG_GSYSC
        std::cout<<connList[co]->getHier1()->getName()<<" and "<<connList[co]->getHier2()->getName()<<" lie to each other in sector \t"<<steigung<<std::endl;
	#endif
	connList[co]->setNode1(new QPoint(connList[co]->getHier1()->getCenterPoint()->x()+(int)floor((double)moduleWidth*0.5)+(int)ceil((double)horizontalSpace*0.5),connList[co]->getHier1()->getCenterPoint()->y()+(int)floor((double)moduleHeight*0.5)+(int)ceil((double)verticalSpace*0.5)));
	connList[co]->setNode2(new QPoint(connList[co]->getHier2()->getCenterPoint()->x()-(int)ceil((double)moduleWidth*0.5)+1-(int)ceil((double)horizontalSpace*0.5),connList[co]->getHier2()->getCenterPoint()->y()-(int)ceil((double)moduleHeight*0.5)+1-(int)ceil((double)verticalSpace*0.5)));
	for (int si=0; si<sigList.size(); si++)
	{
	    if(connList[co]->getHier1()->getSPorts().size() < connList[co]->getHier1()->getEPorts().size())
	      connList[co]->getHier1()->addSPort(sigList[si],E);
	    else
	      if(connList[co]->getHier1()->getSPorts().size() > connList[co]->getHier1()->getEPorts().size())
	        connList[co]->getHier1()->addEPort(sigList[si],S);
	      else
	        if(connList[co]->getHier1()->getSPorts().size() == connList[co]->getHier1()->getEPorts().size())
		{
	          rand1 = rand()/RAND_MAX;
	          if(rand1 >= 0.5) connList[co]->getHier1()->addSPort(sigList[si],E);
	          else connList[co]->getHier1()->addEPort(sigList[si],S);
		}  
	    if(connList[co]->getHier2()->getNPorts().size() < connList[co]->getHier2()->getWPorts().size())
	      connList[co]->getHier2()->addNPort(sigList[si],W);
	    else
	      if(connList[co]->getHier2()->getNPorts().size() > connList[co]->getHier2()->getWPorts().size())
	        connList[co]->getHier2()->addWPort(sigList[si],N);
	      else
	        if(connList[co]->getHier2()->getNPorts().size() == connList[co]->getHier2()->getWPorts().size())
		{
	          rand1 = rand()/RAND_MAX;
	          if(rand1 >= 0.5) connList[co]->getHier2()->addNPort(sigList[si],W);
	          else connList[co]->getHier2()->addWPort(sigList[si],N);
	        }
	}
      }
      if (steigung == S) 
      {
        #ifdef DEBUG_GSYSC
        std::cout<<connList[co]->getHier1()->getName()<<" and "<<connList[co]->getHier2()->getName()<<" lie to each other in sector \t"<<steigung<<std::endl;
	#endif
	if((connList[co]->getHier1()->getCenterPoint()->x() > sideMargin+(dimFactor-1)*(moduleWidth+horizontalSpace)) || 
	   (connList[co]->getHier1()->getCenterPoint()->x() - (int) ceil((double)moduleWidth*0.5) > sideMargin &&
	    connList[co]->getHier1()->getWPorts().size() + connList[co]->getHier2()->getWPorts().size() <
	    connList[co]->getHier1()->getEPorts().size() + connList[co]->getHier2()->getEPorts().size()))
	  lu = true;      // connection over left side
	else lu = false;  // connection over right side
	if(lu)
	{
	  connList[co]->setNode1(new QPoint(connList[co]->getHier1()->getCenterPoint()->x()-(int)ceil((double)moduleWidth*0.5)+1-(int)ceil((double)horizontalSpace*0.5),connList[co]->getHier1()->getCenterPoint()->y()+(int)floor((double)moduleHeight*0.5)+(int)ceil((double)verticalSpace*0.5)));
	  connList[co]->setNode2(new QPoint(connList[co]->getHier2()->getCenterPoint()->x()-(int)ceil((double)moduleWidth*0.5)+1-(int)ceil((double)horizontalSpace*0.5),connList[co]->getHier2()->getCenterPoint()->y()-(int)ceil((double)moduleHeight*0.5)+1-(int)ceil((double)verticalSpace*0.5)));
	}
	else // !lu
	{
	  connList[co]->setNode1(new QPoint(connList[co]->getHier1()->getCenterPoint()->x()+(int)floor((double)moduleWidth*0.5)+(int)ceil((double)horizontalSpace*0.5),connList[co]->getHier1()->getCenterPoint()->y()+(int)floor((double)moduleHeight*0.5)+(int)ceil((double)verticalSpace*0.5)));
	  connList[co]->setNode2(new QPoint(connList[co]->getHier2()->getCenterPoint()->x()+(int)floor((double)moduleWidth*0.5)+(int)ceil((double)horizontalSpace*0.5),connList[co]->getHier2()->getCenterPoint()->y()-(int)ceil((double)moduleHeight*0.5)+1-(int)ceil((double)verticalSpace*0.5)));
	}
	for(int si=0; si<sigList.size(); si++)
	{
	  if(lu)
	  {
	    if(connList[co]->getHier2()->getNPorts().size() < connList[co]->getHier2()->getWPorts().size())
	      connList[co]->getHier2()->addNPort(sigList[si],W);
	    else
	      if(connList[co]->getHier2()->getNPorts().size() > connList[co]->getHier2()->getWPorts().size())
	        connList[co]->getHier2()->addWPort(sigList[si],N);
	      else
	        if(connList[co]->getHier2()->getNPorts().size() == connList[co]->getHier2()->getWPorts().size())
		{
	          rand1 = rand()/RAND_MAX;
	          if(rand1 >= 0.5) connList[co]->getHier2()->addNPort(sigList[si],W);
	          else connList[co]->getHier2()->addWPort(sigList[si],N);
		}  
	    if(connList[co]->getHier1()->getSPorts().size() < connList[co]->getHier1()->getWPorts().size())
	      connList[co]->getHier1()->addSPort(sigList[si],W);
	    else
	      if(connList[co]->getHier1()->getSPorts().size() > connList[co]->getHier1()->getWPorts().size())
	        connList[co]->getHier1()->addWPort(sigList[si],S);
	      else
	        if(connList[co]->getHier1()->getSPorts().size() == connList[co]->getHier1()->getWPorts().size())
		{
	          rand1 = rand()/RAND_MAX;
	          if(rand1 >= 0.5) connList[co]->getHier1()->addSPort(sigList[si],W);
	          else connList[co]->getHier1()->addWPort(sigList[si],S);
	        }
	  }	
	  else	// case: !lu	
	  {
	    if(connList[co]->getHier2()->getNPorts().size() < connList[co]->getHier2()->getEPorts().size())
	      connList[co]->getHier2()->addNPort(sigList[si],E);
	    else
	      if(connList[co]->getHier2()->getNPorts().size() > connList[co]->getHier2()->getEPorts().size())
	        connList[co]->getHier2()->addEPort(sigList[si],N);
	      else
	        if(connList[co]->getHier2()->getNPorts().size() == connList[co]->getHier2()->getEPorts().size())
		{
	          rand1 = rand()/RAND_MAX;
	          if(rand1 >= 0.5) connList[co]->getHier2()->addNPort(sigList[si],E);
	          else connList[co]->getHier2()->addEPort(sigList[si],N);
		}  
	    if(connList[co]->getHier1()->getSPorts().size() < connList[co]->getHier1()->getEPorts().size())
	      connList[co]->getHier1()->addSPort(sigList[si],E);
	    else
	      if(connList[co]->getHier1()->getSPorts().size() > connList[co]->getHier1()->getEPorts().size())
	        connList[co]->getHier1()->addEPort(sigList[si],S);
	      else
	        if(connList[co]->getHier1()->getSPorts().size() == connList[co]->getHier1()->getEPorts().size())
		{
	          rand1 = rand()/RAND_MAX;
	          if(rand1 >= 0.5) connList[co]->getHier1()->addSPort(sigList[si],E);
	          else connList[co]->getHier1()->addEPort(sigList[si],S);
	        }
	  }	
	}
      }
      if (steigung == SSW || steigung == SW || steigung == WSW) 
      {
        #ifdef DEBUG_GSYSC
        std::cout<<connList[co]->getHier1()->getName()<<" and "<<connList[co]->getHier2()->getName()<<" lie to each other in sector \t"<<steigung<<std::endl;
	#endif
	connList[co]->setNode1(new QPoint(connList[co]->getHier1()->getCenterPoint()->x()-(int)ceil((double)moduleWidth*0.5)+1-(int)ceil((double)horizontalSpace*0.5),connList[co]->getHier1()->getCenterPoint()->y()+(int)floor((double)moduleHeight*0.5)+(int)ceil((double)verticalSpace*0.5)));
	connList[co]->setNode2(new QPoint(connList[co]->getHier2()->getCenterPoint()->x()+(int)floor((double)moduleWidth*0.5)+(int)ceil((double)horizontalSpace*0.5),connList[co]->getHier2()->getCenterPoint()->y()-(int)ceil((double)moduleHeight*0.5)+1-(int)ceil((double)verticalSpace*0.5)));
	for (int si=0; si<sigList.size(); si++)
	{
	    if(connList[co]->getHier1()->getSPorts().size() < connList[co]->getHier1()->getWPorts().size())
	      connList[co]->getHier1()->addSPort(sigList[si],W);
	    else
	      if(connList[co]->getHier1()->getSPorts().size() > connList[co]->getHier1()->getWPorts().size())
	        connList[co]->getHier1()->addWPort(sigList[si],S);
	      else
	        if(connList[co]->getHier1()->getSPorts().size() == connList[co]->getHier1()->getWPorts().size())
		{
	          rand1 = rand()/RAND_MAX;
	          if(rand1 >= 0.5) connList[co]->getHier1()->addSPort(sigList[si],W);
	          else connList[co]->getHier1()->addWPort(sigList[si],S);
		}  
	    if(connList[co]->getHier2()->getNPorts().size() < connList[co]->getHier2()->getEPorts().size())
	      connList[co]->getHier2()->addNPort(sigList[si],E);
	    else
	      if(connList[co]->getHier2()->getNPorts().size() > connList[co]->getHier2()->getEPorts().size())
	        connList[co]->getHier2()->addEPort(sigList[si],N);
	      else
	        if(connList[co]->getHier2()->getNPorts().size() == connList[co]->getHier2()->getEPorts().size())
		{
	          rand1 = rand()/RAND_MAX;
	          if(rand1 >= 0.5) connList[co]->getHier2()->addNPort(sigList[si],E);
	          else connList[co]->getHier2()->addEPort(sigList[si],N);
	        }
	}
      }	
      if (steigung == W) 
      {
        #ifdef DEBUG_GSYSC
        std::cout<<connList[co]->getHier1()->getName()<<" and "<<connList[co]->getHier2()->getName()<<" lie to each other in sector \t"<<steigung<<std::endl;
	#endif
	if(connList[co]->getHier1()->getNPorts().size() + connList[co]->getHier2()->getNPorts().size() <
	   connList[co]->getHier1()->getSPorts().size() + connList[co]->getHier2()->getSPorts().size())
	  lu = true;      // connection over upper side
	else lu = false;  // connection over lower side
	if(lu)
	{
	  connList[co]->setNode1(new QPoint(connList[co]->getHier1()->getCenterPoint()->x()-(int)ceil((double)moduleWidth*0.5)+1-(int)ceil((double)horizontalSpace*0.5),connList[co]->getHier1()->getCenterPoint()->y()-(int)ceil((double)moduleHeight*0.5)+1-(int)ceil((double)verticalSpace*0.5)));
	  connList[co]->setNode2(new QPoint(connList[co]->getHier2()->getCenterPoint()->x()+(int)floor((double)moduleWidth*0.5)+(int)ceil((double)horizontalSpace*0.5),connList[co]->getHier2()->getCenterPoint()->y()-(int)ceil((double)moduleHeight*0.5)+1-(int)ceil((double)verticalSpace*0.5)));
	}
	else // !lu
	{
	  connList[co]->setNode1(new QPoint(connList[co]->getHier1()->getCenterPoint()->x()-(int)ceil((double)moduleWidth*0.5)+1-(int)ceil((double)horizontalSpace*0.5),connList[co]->getHier1()->getCenterPoint()->y()+(int)floor((double)moduleHeight*0.5)+(int)ceil((double)verticalSpace*0.5)));
	  connList[co]->setNode2(new QPoint(connList[co]->getHier2()->getCenterPoint()->x()+(int)floor((double)moduleWidth*0.5)+(int)ceil((double)horizontalSpace*0.5),connList[co]->getHier2()->getCenterPoint()->y()+(int)floor((double)moduleHeight*0.5)+(int)ceil((double)verticalSpace*0.5)));
	}
	for(int si=0; si<sigList.size(); si++)
	{
	  if(lu)
	  {
	    if(connList[co]->getHier2()->getNPorts().size() < connList[co]->getHier2()->getEPorts().size())
	      connList[co]->getHier2()->addNPort(sigList[si],E);
	    else
	      if(connList[co]->getHier2()->getNPorts().size() > connList[co]->getHier2()->getEPorts().size())
	        connList[co]->getHier2()->addEPort(sigList[si],N);
	      else
	        if(connList[co]->getHier2()->getNPorts().size() == connList[co]->getHier2()->getEPorts().size())
		{
	          rand1 = rand()/RAND_MAX;
	          if(rand1 >= 0.5) connList[co]->getHier2()->addNPort(sigList[si],E);
	          else connList[co]->getHier2()->addEPort(sigList[si],N);
		}  
	    if(connList[co]->getHier1()->getNPorts().size() < connList[co]->getHier1()->getWPorts().size())
	      connList[co]->getHier1()->addNPort(sigList[si],W);
	    else
	      if(connList[co]->getHier1()->getNPorts().size() > connList[co]->getHier1()->getWPorts().size())
	        connList[co]->getHier1()->addWPort(sigList[si],N);
	      else
	        if(connList[co]->getHier1()->getNPorts().size() == connList[co]->getHier1()->getWPorts().size())
		{
	          rand1 = rand()/RAND_MAX;
	          if(rand1 >= 0.5) connList[co]->getHier1()->addNPort(sigList[si],W);
	          else connList[co]->getHier1()->addWPort(sigList[si],N);
	        }
	  }	
	  else	// case: !lu
	  {
	    if(connList[co]->getHier2()->getSPorts().size() < connList[co]->getHier2()->getEPorts().size())
	      connList[co]->getHier2()->addSPort(sigList[si],E);
	    else
	      if(connList[co]->getHier2()->getSPorts().size() > connList[co]->getHier2()->getEPorts().size())
	        connList[co]->getHier2()->addEPort(sigList[si],S);
	      else
	        if(connList[co]->getHier2()->getSPorts().size() == connList[co]->getHier2()->getEPorts().size())
		{
	          rand1 = rand()/RAND_MAX;
	          if(rand1 >= 0.5) connList[co]->getHier2()->addSPort(sigList[si],E);
	          else connList[co]->getHier2()->addEPort(sigList[si],S);
		}  
	    if(connList[co]->getHier1()->getSPorts().size() < connList[co]->getHier1()->getWPorts().size())
	      connList[co]->getHier1()->addSPort(sigList[si],W);
	    else
	      if(connList[co]->getHier1()->getSPorts().size() > connList[co]->getHier1()->getWPorts().size())
	        connList[co]->getHier1()->addWPort(sigList[si],S);
	      else
	        if(connList[co]->getHier1()->getSPorts().size() == connList[co]->getHier1()->getWPorts().size())
		{
	          rand1 = rand()/RAND_MAX;
	          if(rand1 >= 0.5) connList[co]->getHier1()->addSPort(sigList[si],W);
	          else connList[co]->getHier1()->addWPort(sigList[si],S);
	        }
	  }	
	}
      }
      if (steigung == WNW || steigung == NW || steigung == NNW) 
      {
        #ifdef DEBUG_GSYSC
        std::cout<<connList[co]->getHier1()->getName()<<" and "<<connList[co]->getHier2()->getName()<<" lie to each other in sector \t"<<steigung<<std::endl;
	#endif
	connList[co]->setNode1(new QPoint(connList[co]->getHier1()->getCenterPoint()->x()-(int)ceil((double)moduleWidth*0.5)+1-(int)ceil((double)horizontalSpace*0.5),connList[co]->getHier1()->getCenterPoint()->y()-(int)ceil((double)moduleHeight*0.5)+1-(int)ceil((double)verticalSpace*0.5)));
	connList[co]->setNode2(new QPoint(connList[co]->getHier2()->getCenterPoint()->x()+(int)floor((double)moduleWidth*0.5)+(int)ceil((double)horizontalSpace*0.5),connList[co]->getHier2()->getCenterPoint()->y()+(int)floor((double)moduleHeight*0.5)+(int)ceil((double)verticalSpace*0.5)));
	for (int si=0; si<sigList.size(); si++)
	{
	    if(connList[co]->getHier1()->getNPorts().size() < connList[co]->getHier1()->getWPorts().size())
	      connList[co]->getHier1()->addNPort(sigList[si],W);
	    else
	      if(connList[co]->getHier1()->getNPorts().size() > connList[co]->getHier1()->getWPorts().size())
	        connList[co]->getHier1()->addWPort(sigList[si],N);
	      else
	        if(connList[co]->getHier1()->getNPorts().size() == connList[co]->getHier1()->getWPorts().size())
		{
	          rand1 = rand()/RAND_MAX;
	          if(rand1 >= 0.5) connList[co]->getHier1()->addNPort(sigList[si],W);
	          else connList[co]->getHier1()->addWPort(sigList[si],N);
		}  
	    if(connList[co]->getHier2()->getSPorts().size() < connList[co]->getHier2()->getEPorts().size())
	      connList[co]->getHier2()->addSPort(sigList[si],E);
	    else
	      if(connList[co]->getHier2()->getSPorts().size() > connList[co]->getHier2()->getEPorts().size())
	        connList[co]->getHier2()->addEPort(sigList[si],S);
	      else
	        if(connList[co]->getHier2()->getSPorts().size() == connList[co]->getHier2()->getEPorts().size())
		{
	          rand1 = rand()/RAND_MAX;
	          if(rand1 >= 0.5) connList[co]->getHier2()->addSPort(sigList[si],E);
	          else connList[co]->getHier2()->addEPort(sigList[si],S);
	        }
	}
      }
    }


    // Port arrangements left and right for contact to the world outside ...
    if (ownHierarchy!=0) 
    {
      for (int i=0; i<sideConnList.size(); i++)
      {
        if(sideConnList[i]->getHier1()->getCenterPoint()->x() < sideMargin+moduleWidth ||
	   sideConnList[i]->getHier2()->getCenterPoint()->x() < sideMargin+moduleWidth)  // Modul auf linker Seite
	{
	  for(int o=0; o<sideConnList[i]->getSignals().size(); o++)
	  {
	    #ifdef DEBUG_GSYSC
	    cout<<"SideConnList.size()="<<sideConnList.size()<<",  sideConnList[i]->getSignals().size()="<<sideConnList[i]->getSignals().size()<<endl;
	    #endif
	    if(sideConnList[i]->getHier1()==ownHierarchy) 
	    {
	      sideConnList[i]->getHier2()->addWPort(sideConnList[i]->getSignals()[o],QPoint());
	      ownHierarchy->addLeftPort(sideConnList[i]->getSignals()[o]);
	      ownHierarchy->getLeftPorts()[ownHierarchy->getLeftPorts().size()-1]->hVar = sideConnList[i]->getHier2()->getCenterPoint()->y()+o+1;
	    }  
	    else  
	    {
	      sideConnList[i]->getHier1()->addWPort(sideConnList[i]->getSignals()[o],QPoint());
	      ownHierarchy->addLeftPort(sideConnList[i]->getSignals()[o]);
	      ownHierarchy->getLeftPorts()[ownHierarchy->getLeftPorts().size()-1]->hVar = sideConnList[i]->getHier1()->getCenterPoint()->y()+o+1;
	    }  
	  }
	}
	else	// Module on the right; all others should be connected through sideConnList (see above)
	{
	  for(int o=0; o<sideConnList[i]->getSignals().size(); o++)
	  {
	    if(sideConnList[i]->getHier1()==ownHierarchy) 
	    {
	      sideConnList[i]->getHier2()->addEPort(sideConnList[i]->getSignals()[o],QPoint());
	      ownHierarchy->addRightPort(sideConnList[i]->getSignals()[o]);
	      ownHierarchy->getRightPorts()[ownHierarchy->getRightPorts().size()-1]->hVar = sideConnList[i]->getHier2()->getCenterPoint()->y()+o+1;
	    }  
	    else  
	    {
	      sideConnList[i]->getHier1()->addEPort(sideConnList[i]->getSignals()[o],QPoint());
	      ownHierarchy->addRightPort(sideConnList[i]->getSignals()[o]);
	      ownHierarchy->getRightPorts()[ownHierarchy->getRightPorts().size()-1]->hVar = sideConnList[i]->getHier1()->getCenterPoint()->y()+o+1;
	    }  
	  }
	}
      }
      ownHierarchy->sortSidePorts();
    

      // draw port arrangements left and right ...
      int abstandY = (int) floor( (2*(double)topMargin+(double)dimFactor*(double)moduleHeight
      					+((double)dimFactor-1)*(double)verticalSpace 
      					- ownHierarchy->getLeftPorts().size() * 21) 
			  	  / (double)(ownHierarchy->getLeftPorts().size()+1) );      // distances between ports at equidistant distribution
      cout<<"AbstandY ist "<<abstandY<<";  Zähler: "<<(2*(double)topMargin+(double)dimFactor*(double)moduleHeight +((double)dimFactor-1)*(double)verticalSpace - ownHierarchy->getLeftPorts().size() * 21)<<" / Nenner: "<<(double)(ownHierarchy->getLeftPorts().size()+1)<<"    <->   getLeftPorts().size()="<<ownHierarchy->getLeftPorts().size()<<endl;		     
      for(int i=0; i<ownHierarchy->getLeftPorts().size(); i++)
      {
        cout<<"LeftPorts().size()="<<ownHierarchy->getLeftPorts().size()<<";  i="<<i<<endl;
	drawSidePort(ownHierarchy->getLeftPorts()[i],true,i,abstandY);
      }
    
      int donePortsL = 0;
      int donePortsR = 0;
      for(int i=0; i<hierarchyList.size(); i++)
      {
	if (i % dimFactor == 0)
	{
	  for (int o=0; o<hierarchyList[i]->getWPorts().size(); o++)
	    hierarchyList[i]->getWPorts()[o]->setDest(QPoint(50,(donePortsL+o+1)*abstandY+(donePortsL+o)*21+11));
	  donePortsL += hierarchyList[i]->getWPorts().size();
	}
	if (i % dimFactor == dimFactor-1)
	{
	  for (int o=0; o<hierarchyList[i]->getEPorts().size(); o++)
	    hierarchyList[i]->getEPorts()[o]->setDest(QPoint(canvasView->scene()->width()-51,(donePortsR+o+1)*abstandY+(donePortsR+o)*21+11));
	  donePortsR += hierarchyList[i]->getEPorts().size();
	}
      
      }
    }

    sigList.clear();

    for(int i=0; i<hierarchyList.size(); i++)
    {
      drawNetConns(hierarchyList[i]);
    }	

    drawConnections();

    if (toShow) this->show();
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
	polygons[polygons.size()-1]->setPen(QColor(inPort));
	polygons[polygons.size()-1]->setBrush(QColor(inPort));
	polygons[polygons.size()-1]->setZValue(220);
	polygons[polygons.size()-1]->show();
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
	polygons[polygons.size()-1]->setPen(QColor(outPort));
	polygons[polygons.size()-1]->setBrush(QColor(outPort));
	polygons[polygons.size()-1]->setZValue(220);
	polygons[polygons.size()-1]->show();
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
	polygons[polygons.size()-1]->setPen(QColor(inoutPort));
	polygons[polygons.size()-1]->setBrush(QColor(inoutPort));
	polygons[polygons.size()-1]->setZValue(220);
	polygons[polygons.size()-1]->show();
	ports[i]->addLineElem(polygons.back());
      }
      if(!ports[i]->isEndPort())
      {
        lines.push_back(new QGraphicsLineItem());
        lines.back()->setLine(x+3,y,x+3,y-((verticalSpace-16)/2));
        ports[i]->addLineElem((QAbstractGraphicsShapeItem*)lines.back());
        lines.back()->show();
        lines.push_back(new QGraphicsLineItem());
        if (ports[i]->getInitDir() == W) 
          lines.back()->setLine(x+3,y-((verticalSpace-16)/2),hier->getCenterPoint()->x()-(int)floor((double)moduleWidth*0.5)-(int)floor((double)horizontalSpace*0.5),y-((verticalSpace-16)/2));
        else	
          lines.back()->setLine(x+3,y-((verticalSpace-16)/2),hier->getCenterPoint()->x()+(int)ceil((double)moduleWidth*0.5)+(int)floor((double)horizontalSpace*0.5),y-((verticalSpace-16)/2));
        ports[i]->addLineElem((QAbstractGraphicsShapeItem*)lines.back());
        lines.back()->show();
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
	polygons[polygons.size()-1]->setPen(QColor(inPort));
	polygons[polygons.size()-1]->setBrush(QColor(inPort));
	polygons[polygons.size()-1]->setZValue(220);
	polygons[polygons.size()-1]->show();
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
	polygons[polygons.size()-1]->setPen(QColor(outPort));
	polygons[polygons.size()-1]->setBrush(QColor(outPort));
	polygons[polygons.size()-1]->setZValue(220);
	polygons[polygons.size()-1]->show();
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
	polygons[polygons.size()-1]->setPen(QColor(inoutPort));
	polygons[polygons.size()-1]->setBrush(QColor(inoutPort));
	polygons[polygons.size()-1]->setZValue(220);
	polygons[polygons.size()-1]->show();
	ports[i]->addLineElem(polygons.back());
      }
      
      if(!ports[i]->isEndPort())
      {
        lines.push_back(new QGraphicsLineItem());
        lines.back()->setLine(x+3,y,x+3,y+((verticalSpace-16)/2));
        ports[i]->addLineElem((QAbstractGraphicsShapeItem*)lines.back());
        lines.back()->show();
        lines.push_back(new QGraphicsLineItem());
        if (ports[i]->getInitDir() == W) 
          lines.back()->setLine(x+3,y+((verticalSpace-16)/2),hier->getCenterPoint()->x()-(int)floor((double)moduleWidth*0.5)-(int)floor((double)horizontalSpace*0.5),y+((verticalSpace-16)/2));
        else	
          lines.back()->setLine(x+3,y+((verticalSpace-16)/2),hier->getCenterPoint()->x()+(int)ceil((double)moduleWidth*0.5)+(int)floor((double)horizontalSpace*0.5),y+((verticalSpace-16)/2));
        ports[i]->addLineElem((QAbstractGraphicsShapeItem*)lines.back());
        lines.back()->show();
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
	polygons[polygons.size()-1]->setPen(QColor(inPort));
	polygons[polygons.size()-1]->setBrush(QColor(inPort));
	polygons[polygons.size()-1]->setZValue(220);
	polygons[polygons.size()-1]->show();
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
	polygons[polygons.size()-1]->setPen(QColor(outPort));
	polygons[polygons.size()-1]->setBrush(QColor(outPort));
	polygons[polygons.size()-1]->setZValue(220);
	polygons[polygons.size()-1]->show();
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
	polygons[polygons.size()-1]->setPen(QColor(inoutPort));
	polygons[polygons.size()-1]->setBrush(QColor(inoutPort));
	polygons[polygons.size()-1]->setZValue(220);
	polygons[polygons.size()-1]->show();
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
          lines.push_back(new QGraphicsLineItem());
          if (ports[i]->getInitDir() == N) 
  	  {
            lines.back()->setLine(x-((horizontalSpace-16)/2),y+3,x-((horizontalSpace-16)/2),hier->getCenterPoint()->y()-(int)floor((double)moduleHeight*0.5)-(int)floor((double)verticalSpace*0.5));
	    lines.back()->show();
	  }  
          else	
 	  {
            lines.back()->setLine(x-((horizontalSpace-16)/2),y+3,x-((horizontalSpace-16)/2),hier->getCenterPoint()->y()+(int)ceil((double)moduleHeight*0.5)+(int)floor((double)verticalSpace*0.5));
	    lines.back()->show();
	  }  
        }	
        else
        {
          lines.push_back(new QGraphicsLineItem());
	  lines.back()->setLine(x,y+3,ports[i]->getDest().x(),ports[i]->getDest().y());
	  lines.back()->show();
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
	polygons[polygons.size()-1]->setPen(QColor(inPort));
	polygons[polygons.size()-1]->setBrush(QColor(inPort));
	polygons[polygons.size()-1]->setZValue(220);
	polygons[polygons.size()-1]->show();
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
	polygons[polygons.size()-1]->setPen(QColor(outPort));
	polygons[polygons.size()-1]->setBrush(QColor(outPort));
	polygons[polygons.size()-1]->setZValue(220);
	polygons[polygons.size()-1]->show();
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
	polygons[polygons.size()-1]->setPen(QColor(inoutPort));
	polygons[polygons.size()-1]->setBrush(QColor(inoutPort));
	polygons[polygons.size()-1]->setZValue(220);
	polygons[polygons.size()-1]->show();
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
	  lines.push_back(new QGraphicsLineItem());
	  if (ports[i]->getInitDir() == N) 
	  {
	    lines.back()->setLine(x+((horizontalSpace-16)/2),y+3,x+((horizontalSpace-16)/2),hier->getCenterPoint()->y()-(int)floor((double)moduleHeight*0.5)-(int)floor((double)verticalSpace*0.5));
	    lines.back()->show();
	  }  
	  else	
	  {
	    lines.back()->setLine(x+((horizontalSpace-16)/2),y+3,x+((horizontalSpace-16)/2),hier->getCenterPoint()->y()+(int)ceil((double)moduleHeight*0.5)+(int)floor((double)verticalSpace*0.5));
	    lines.back()->show();
	  }  
	}	
	else
	{
	  lines.push_back(new QGraphicsLineItem());
	  lines.back()->setLine(x,y+3,ports[i]->getDest().x(),ports[i]->getDest().y());
	  lines.back()->show();
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
   *
   */
  bool gsysHierarchyWindow::thisLevel(gsysHierarchy* hier)
  {
    for(int i=0; i<hierarchyList.size(); i++)
      if(hierarchyList[i]==hier) return true;
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
    #ifdef DEBUG_GSYSC
    std::cout<<"Point 1:  "<<p1->x()<<"/"<<p1->y()<<";     Point 2:  "<<p2->x()<<"/"<<p2->y()<<std::endl;
    #endif
    if(lfdNr > 2*dimFactor) 
    {
      std::cerr<<"ERROR:  endless recursion threatens in function 'drawConnStep'!!!!!"<<std::endl;
      return false;
    }
    short steigung;
    QList<QGraphicsItem*> cil = canvasView->scene()->items(*p1);
    QGraphicsRectItem *nodeRect = 0;
    for(int o=0; o<cil.size(); o++)
    {	    
      if(cil[o]->type() == QGraphicsRectItem::Type) 
      {
	nodeRect = (QGraphicsRectItem*) cil[o];
      }	
    }  
    if(nodeRect==0)  
      nodeRect = new QGraphicsRectItem(p1->x()-11,p1->y()-11,24,24);
    nodeRect->setZValue(100);
    nodeRect->setPen(QColor(normalNode));
    nodeRect->setBrush(QColor(normalNode));
    nodeRect->show();
    connection->addTraceElem(nodeRect);
    if(*p1 == *p2) return true;
    else
    {
      steigung = getSector(p1,p2);
      QGraphicsLineItem *connLine = new QGraphicsLineItem();
      connLine->setZValue(20);
      connLine->setPen(QPen(QColor(normalSignal),3));
      if(steigung == N || steigung == NNW || steigung == NNE)
      {
        connLine->setLine(p1->x(),p1->y(),p1->x(),p1->y()-moduleHeight-verticalSpace);
	drawConnStep(connection,new QPoint(p1->x(),p1->y()-moduleHeight-verticalSpace),p2,lfdNr+1);
      }
      if(steigung == E || steigung == ENE || steigung == ESE)
      {
        connLine->setLine(p1->x(),p1->y(),p1->x()+moduleWidth+horizontalSpace,p1->y());
	drawConnStep(connection,new QPoint(p1->x()+moduleWidth+horizontalSpace,p1->y()),p2,lfdNr+1);
      }
      if(steigung == S || steigung == SSW || steigung == SSE)
      {
        connLine->setLine(p1->x(),p1->y(),p1->x(),p1->y()+moduleHeight+verticalSpace);
	drawConnStep(connection,new QPoint(p1->x(),p1->y()+moduleHeight+verticalSpace),p2,lfdNr+1);
      }
      if(steigung == W || steigung == WNW || steigung == WSW)
      {
        connLine->setLine(p1->x(),p1->y(),p1->x()-moduleWidth-horizontalSpace,p1->y());
	drawConnStep(connection,new QPoint(p1->x()-moduleWidth-horizontalSpace,p1->y()),p2,lfdNr+1);
      }
      if(steigung == NE)
      {
        if(floor(rand()/RAND_MAX+0.5) == 0.0)
	{
        connLine->setLine(p1->x(),p1->y(),p1->x(),p1->y()-moduleHeight-verticalSpace);
	drawConnStep(connection,new QPoint(p1->x(),p1->y()-moduleHeight-verticalSpace),p2,lfdNr+1);
	}
	else
	{
        connLine->setLine(p1->x(),p1->y(),p1->x()+moduleWidth+horizontalSpace,p1->y());
	drawConnStep(connection,new QPoint(p1->x()+moduleWidth+horizontalSpace,p1->y()),p2,lfdNr+1);
	}
      }
      if(steigung == NW)
      {
        if(floor(rand()/RAND_MAX+0.5) == 0.0)
	{
        connLine->setLine(p1->x(),p1->y(),p1->x(),p1->y()-moduleHeight-verticalSpace);
	drawConnStep(connection,new QPoint(p1->x(),p1->y()-moduleHeight-verticalSpace),p2,lfdNr+1);
	}
	else
	{
        connLine->setLine(p1->x(),p1->y(),p1->x()-moduleWidth-horizontalSpace,p1->y());
	drawConnStep(connection,new QPoint(p1->x()-moduleWidth-horizontalSpace,p1->y()),p2,lfdNr+1);
	}
      }
      if(steigung == SE)
      {
        if(floor(rand()/RAND_MAX+0.5) == 0.0)
	{
        connLine->setLine(p1->x(),p1->y(),p1->x(),p1->y()+moduleHeight+verticalSpace);
	drawConnStep(connection,new QPoint(p1->x(),p1->y()+moduleHeight+verticalSpace),p2,lfdNr+1);
	}
	else
	{
        connLine->setLine(p1->x(),p1->y(),p1->x()+moduleWidth+horizontalSpace,p1->y());
	drawConnStep(connection,new QPoint(p1->x()+moduleWidth+horizontalSpace,p1->y()),p2,lfdNr+1);
	}
      }
      if(steigung == SW)
      {
        if(floor(rand()/RAND_MAX+0.5) == 0.0)
	{
        connLine->setLine(p1->x(),p1->y(),p1->x(),p1->y()+moduleHeight+verticalSpace);
	drawConnStep(connection,new QPoint(p1->x(),p1->y()+moduleHeight+verticalSpace),p2,lfdNr+1);
	}
	else
	{
        connLine->setLine(p1->x(),p1->y(),p1->x()-moduleWidth-horizontalSpace,p1->y());
	drawConnStep(connection,new QPoint(p1->x()-moduleWidth-horizontalSpace,p1->y()),p2,lfdNr+1);
	}
      }
      connLine->show();
      connection->addTraceElem(new QGraphicsPathItem(connLine));               //(connLine);
      return true;
    }

    nodeRect = 0;
  }

  /*
   *   main method to draw connections
   */
  void gsysHierarchyWindow::drawConnections()
  {
    for(int i=0; i<connList.size(); i++)
    {
      drawConnStep(connList[i],connList[i]->getNode1(),connList[i]->getNode2(),1);
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
	polygon->setPen(QColor(inPort));
	polygon->setBrush(QColor(inPort));
	polygon->setZValue(220);
	text->setText(port->getName());
	text->setFont(smallerFont);
	text->setX(5);
	text->setY(y+4);
	text->setZValue(240);
	polygon->show();
	text->show();
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
	polygon->setPen(QColor(outPort));
	polygon->setBrush(QColor(outPort));
	polygon->setZValue(220);
	text->setText(port->getName());
	text->setFont(smallerFont);
	text->setX(5);
	text->setY(y+4);
	text->setZValue(240);
	polygon->show();
	text->show();
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
	polygon->setPen(QColor(inoutPort));
	polygon->setBrush(QColor(inoutPort));
	polygon->setZValue(220);
	text->setText(port->getName());
	text->setFont(smallerFont);
	text->setX(5);
	text->setY(y+4);
	text->setZValue(240);
	polygon->show();
	text->show();
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
	polygon->setPen(QColor(inPort));
	polygon->setBrush(QColor(inPort));
	polygon->setZValue(220);
	text->setText(port->getName());
	text->setFont(smallerFont);
	text->setX(5);
	text->setY(y+4);
	text->setZValue(240);
	polygon->show();
	text->show();
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
	polygon->setPen(QColor(outPort));
	polygon->setBrush(QColor(outPort));
	polygon->setZValue(220);
	text->setText(port->getName());
	text->setFont(smallerFont);
	text->setX(5);
	text->setY(y+4);
	text->setZValue(240);
	polygon->show();
	text->show();
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
	polygon->setPen(QColor(inoutPort));
	polygon->setBrush(QColor(inoutPort));
	polygon->setZValue(220);
	text->setText(port->getName());
	text->setFont(smallerFont);
	text->setX(5);
	text->setY(y+4);
	text->setZValue(240);
	polygon->show();
	text->show();
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
   *   get the sector in which the line between the points 'source' and 'dest'
   *   can be found. This means a sector of a circle divided into 16 parts.
   *   Horizontal, vertical and diagonal from the central point of view are respectively one direction,
   *   the sector between those direction give the rest of 8 sectors.
   *   The direction coding is given in the header file.
   */
  short gsysHierarchyWindow::getSector(QPoint *source, QPoint *dest)
  {
    if (*source == *dest) return 0;
    if (source->x() == dest->x())
      if (source->y() < dest->y()) return S;
      else return N;
    if (source->y() == dest->y())
      if (source->x() < dest->x()) return E;
      else return W;
    if (abs(source->x() - dest->x()) == abs(source->y() - dest->y()))
      if(source->x() < dest->x())
        if(source->y() < dest->y()) return SE;
	else return NE;
      else
        if(source->y() < dest->y()) return SW;
	else return NW;
    if (source->x() < dest->x())
      if (source->y() < dest->y())
        if (abs((source->y()-dest->y()) / (source->x()-dest->x())) > 1.0) return SSE;
	else return ESE;
      else
        if (abs((source->y()-dest->y()) / (source->x()-dest->x())) > 1.0) return NNE;
	else return ENE;
    else  
      if (source->y() < dest->y())
        if (abs((source->y()-dest->y()) / (source->x()-dest->x())) > 1.0) return SSW;
	else return WSW;
      else
        if (abs((source->y()-dest->y()) / (source->x()-dest->x())) > 1.0) return NNW;
	else return WNW;
    return -1;	
  }

