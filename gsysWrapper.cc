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
    last modified: 2005-09-29
 ****************************************************************************
    Content:
      This file provides a simple script for automatically adjusting
      an existing (and working!) systemc design to an usable gSysC
      design. This means, the system.h include is replaced by gsysc.h and
      the signals and ports get automatically registered to the gSysC classes.

 ***************************************************************************/


#include<iostream>
#include<fstream>
#include<cstring>
#include<string>
#include<vector>
#include <cstdio>
using namespace std;


//##########################################################################
//##         Structure declarations for storing scan results              ##
//##########################################################################

struct port_data {
  char* variableName;
  bool isPointer;
  bool isArray;
  char* arraySize;
  char* name;

  port_data(const char* varName="", bool isP=false, const char* n="") {
    variableName=(char*) malloc(strlen(varName)+1);
    strcpy(variableName,varName);
    variableName[strlen(varName)]='\0';
    isPointer=isP;
    isArray=false;
    name=(char*) malloc(strlen(n)+1);
    strcpy(name,n);
    name[strlen(n)]='\0';
  }

  void setArraySize(const char* size) {
    arraySize=(char*) malloc(strlen(size)+1);
    strcpy(arraySize,size);
    arraySize[strlen(size)]='\0';
  }

  void setVariableName(const char* name) {
    variableName=(char*) malloc(strlen(name)+1);
    strcpy(variableName,name);
    variableName[strlen(name)]='\0';
  }
};

struct signal_data {
  char* variableName;
  bool isPointer;
  bool isArray;
  char* arraySize;
  vector<string> portList;
  char* name;

  signal_data(const char* varName="", bool isP=false, const char* n="") {
    arraySize=0;
    setVariableName(varName);
    portList.clear();
    isArray=false;
    isPointer=isP;
    name=(char*) malloc(strlen(n)+1);
    strcpy(name,n);
    name[strlen(n)]='\0';
  }

  void setArraySize(const char* size) {
    arraySize=(char*) malloc(strlen(size)+1);
    if(arraySize==NULL) cerr << "ERROR: Could not allocate memory for array size delimiter '" << size << "'!" << endl;
    strcpy(arraySize,size);
    arraySize[strlen(size)]='\0';
  }

  void setVariableName(const char* name) {
    variableName=(char*) malloc(strlen(name)+1);
    if(variableName==NULL) cerr << "ERROR: Could not allocate memory for signal variable name '" << name << "'!" << endl;
    strcpy(variableName,name);
    variableName[strlen(name)]='\0';
  }
};

struct module_data {
  char* name;
  bool isMain;
  vector<port_data*> ports_vec;
  vector<signal_data*> signals_vec;

  module_data(const char* n=0) {
    ports_vec.clear();
    signals_vec.clear();
    isMain=false;
    if(n!=NULL) this->setName(n);
    else name=0;
  }

  void setName(const char* n) {
    name=(char*) malloc(strlen(n)+1);
    strcpy(name,n);
    name[strlen(n)]='\0';
  }
};

struct module_decl {
  char* variableName;
  char* givenName;
  module_data* moduleType;
  bool isPointer;
  bool isArray;
  char* arraySize;

  module_decl(const char* name=0) {
    if(name!=0) this->setVariableName(name);
    else variableName=0;
    givenName=0;
    moduleType=0;
    isPointer=false;
    isArray=false;
    arraySize=0;
  }

  void setVariableName(const char* n) {
    variableName=(char*) malloc(strlen(n)+1);
    strcpy(variableName,n);
    variableName[strlen(n)]='\0';
  }

  void setRealName(const char* n) {
    givenName=(char*) malloc(strlen(n)+1);
    strcpy(givenName,n);
    givenName[strlen(n)]='\0';
  }

  void setArraySize(const char* n) {
    arraySize=(char*) malloc(strlen(n)+1);
    strcpy(arraySize,n);
    arraySize[strlen(n)]='\0';
  }
};

struct file_data {
  char* fileName;
  char* includeFile;
  bool headerIncluded;
  bool incFileIncluded;
  int systemcIncludeLine;
  int CTORfirstLine;
  vector<module_decl> modules_vec;
  module_data* module;

  file_data(const char* file) {
    this->setFileName(file);
    systemcIncludeLine=-1;
    CTORfirstLine=-1;
    module=0;
    headerIncluded=false;
    incFileIncluded=false;
  }

  void setModule(module_data* mod) {
    module=mod;
  }

  void setFileName(const char* name) {
    fileName=(char*) malloc(strlen(name)+1);
    strcpy(fileName,name);
    fileName[strlen(name)]='\0';
  }

  void setIncludeFile(const char* name) {
    includeFile=(char*) malloc(strlen(name)+1);
    strcpy(includeFile,name);
    includeFile[strlen(name)]='\0';
  }
};


//##########################################################################
//##                   Global variable declarations                       ##
//##########################################################################

vector<port_data*> ports_vec;
vector<signal_data*> signals_vec;
vector<file_data*> files_vec;
file_data* tmp_file;
module_data* emptyModule;
char* filename;
string include_file;
bool starComment;
string zeile,workLine;
string varName;
string givenName;
int pos,pos1;
int lineNum;
bool isP;    // Pointer declaration


char ch;  /////////////////// Input for pause (debugging purposes)


//##########################################################################
//##                         Implementations                              ##
//##########################################################################

int main(int argc, char* argv[])
{
  if (argc == 1) {
    cout << "No file parameter found! \nThis program needs all files that have to be considered for further processing to be put into the parameter list:\n\t" << argv[0] << " file1 file2 file3 ...\n" << endl;
    exit(1);
  }
  else {
    files_vec.clear();
    cout << "##### Scanning Files..." << endl;
    for(int parameter=1; parameter<argc; parameter++) {
      cout << "\t" << argv[parameter] << endl;
      filename=argv[parameter];
      include_file=filename;
      include_file=include_file+".gsysc";

      // Working with the files
      ifstream* fp;
      fp->open(filename,ios::in);
      lineNum=0;
      ports_vec.clear();
      signals_vec.clear();
      starComment=false;
      if(fp!=nullptr) {
        tmp_file=new file_data(filename);
        tmp_file->setIncludeFile(include_file.c_str());
        while (getline(*fp,zeile)) {
          lineNum++;
          if(starComment==true && (pos=zeile.find("*/"))>=0) {
              zeile=zeile.erase(0,pos);
              starComment=false;
          }
          if(starComment) { continue; }  // abort loop if the whole line is part of a comment
          if((pos=zeile.find("//")) >= 0) { zeile=zeile.erase(pos); }
          while((pos=zeile.find("/*")) >= 0) {
            starComment=true;
            if((pos1=zeile.find("*/",pos)) >= 0) {
              zeile=zeile.erase(pos,pos1-pos+2);
              starComment=false;
            }
            else {
              zeile=zeile.erase(pos);
            }
          }
          if((pos=zeile.find_first_not_of(" ")) > 0) { zeile = zeile.erase(0,pos); }
          if((pos=zeile.find(".gsysc")) > 0 && (pos1=zeile.find("#include")) >=0) {
            tmp_file->incFileIncluded=true;
          }
          if((pos=zeile.find("gsysc.h")) > 0 && (pos1=zeile.find("#include")) >=0) {
            tmp_file->headerIncluded=true;
          }
          if((pos=zeile.find("systemc.h")) > 0 && (pos1=zeile.find("#include")) >=0) {
            tmp_file->systemcIncludeLine=lineNum;
          }
          if((pos=zeile.find("SC_MODULE")) >= 0) {
            varName=zeile.substr(0,zeile.find(")"));
            varName=varName.erase(0,varName.find("(")+1);
            module_data* tmp_module=new module_data(varName.c_str());
            tmp_file->setModule(tmp_module);
          }
	  if((pos=zeile.find("sc_main")) >= 0) {
	    varName="Main File";
	    module_data* main_module=new module_data(varName.c_str());
	    tmp_file->setModule(main_module);
	    tmp_file->module->isMain=true;
//	    cout << "DEBUG:    Main File found; bool=" << tmp_file->module->isMain << endl;
	  }
          if( (pos=zeile.find("SC_CTOR"))>=0 || ( tmp_file->module!=0 && tmp_file->module->name!=0 && (pos1=zeile.find((string) (tmp_file->module->name)))==zeile.find_first_not_of(" ") && pos1>=0)  ) 
	  {
/*            if((pos1=zeile.find(";",pos)) >= 0) {
              varName=zeile.substr(0,pos1);
            }
            else { varName=zeile; }
            varName=varName.erase(0,pos);
*/
            if((pos=zeile.find("{"))>0 && !(((pos1=zeile.find("}",pos))>0))) {
              tmp_file->CTORfirstLine=lineNum+1;
            }
            else { 
	      while (getline(*fp,zeile)) {
		lineNum++;
	        if ((pos=zeile.find("{"))>=0) break;
	      }
	      tmp_file->CTORfirstLine=lineNum+1; 
	    }         // CAVE: the CTOR is supposed to consist of brackets in different
                      // lines, i.e., the CTOR is not empty!
          }
          while((pos=zeile.find(";")) >= 0) {   // while another command can be found in this line
            workLine=zeile.substr(0,zeile.find(";"))+",";
            zeile=zeile.erase(0,zeile.find(";")+1);
            if((pos=workLine.find("sc_int"))>=0) {
              continue;
            }
//	    cout << "DEBUG: Workline 1:  " << workLine << endl;
            if((pos=workLine.find("sc_in"))>=0 || (pos=workLine.find("gsys_in"))>=0 ||
               (pos=workLine.find("sc_out"))>=0 || (pos=workLine.find("gsys_out"))>=0 ) {
              if(tmp_file->CTORfirstLine<lineNum+1) { tmp_file->CTORfirstLine=lineNum+1; }
              workLine=workLine.erase(0,workLine.find("_"));
              if((pos1=workLine.find(">"))>=0) { workLine=workLine.erase(0,pos1+1); }
              while((pos=workLine.find(","))>=0) {
                varName=workLine.substr(0,pos);
                workLine=workLine.erase(0,pos+1);
                if((pos=varName.find("*")) >=0) { isP=true; } else { isP=false; }   // declared as pointer?
                if(varName[0]=='_') {
                  if((pos1=varName.find(" "))>=0) { varName=varName.erase(0,pos1); }
                }
                if((pos1=varName.find_first_not_of(" >*"))>=0) {
                  varName=varName.erase(0,pos1);
                }
                if((pos=varName.find("("))>0) {
                  givenName=varName.substr(pos+2);
                  givenName=givenName.erase(givenName.size() -2);
                  varName=varName.erase(pos);
                }
                else { givenName=""; }
                port_data *tmp_port;
                if((pos=varName.find("["))>=0) {
                  tmp_port = new port_data((varName.substr(0,pos)).c_str(),isP,givenName.c_str());
                }
                else {
                  tmp_port = new port_data(varName.c_str(),isP,givenName.c_str());
                }
                if(tmp_port!=NULL) {
                  ports_vec.push_back(tmp_port);
                  if((pos=varName.find("["))>=0 && (pos1=varName.find("]"))>=0) {
                    tmp_port->isArray=true;
                    tmp_port->setArraySize( ((varName.erase(pos1)).erase(0,pos+1)).c_str() );
                  }
                }
                else { cerr << "Port " << varName << " could not be added to the list of ports!" << endl; }
                tmp_port=0;
              }
              continue;
            }
//	    cout << "DEBUG: Workline 2:  " << workLine << endl;
            if((pos=workLine.find("sc_signal"))>=0 || (pos=workLine.find("gsys_signal"))>=0) {
//	      cout << "DEBUG: Condition TRUE" << endl;
              if(tmp_file->CTORfirstLine<lineNum+1) { tmp_file->CTORfirstLine=lineNum+1; }
              workLine=workLine.erase(0,workLine.find("_"));
              if((pos1=workLine.find(">"))>=0) { workLine=workLine.erase(0,pos1+1); }
//	      cout << "DEBUG: Workline 3:  " << workLine << endl;
              while((pos=workLine.find(","))>=0) {
                varName=workLine.substr(0,pos);
                workLine=workLine.erase(0,pos+1);
//		cout << "DEBUG: varName 1:  " << varName << endl;
//	        cout << "DEBUG: Workline 4:  " << workLine << endl;
                if((pos=varName.find("*")) >=0) { isP=true; } else { isP=false; }   // declared as pointer?
                if(varName[0]=='_') {
                  if((pos1=varName.find(" "))>=0) { varName=varName.erase(0,pos1); }
                }
//		cout << "DEBUG: varName 2:  " << varName << endl;
                if((pos1=varName.find_first_not_of(" >*"))>=0) { varName=varName.erase(0,pos1); }
//		cout << "DEBUG: varName 3:  " << varName << endl;
                if((pos=varName.find("("))>0) {
                  givenName=varName.substr(pos+2);
                  givenName=givenName.erase(givenName.size() -2);
                  varName=varName.erase(pos);
                }
                else { givenName=""; }
//		cout << "DEBUG: varName 4:  " << varName << endl;
//		cout << "DEBUG: givenName:  " << givenName << endl;
                signal_data *tmp_signal;
                if((pos=varName.find("["))>=0) {
                  tmp_signal = new signal_data((varName.substr(0,pos)).c_str(),isP,givenName.c_str());
                }
                else {
                  tmp_signal = new signal_data(varName.c_str(),isP,givenName.c_str());
                }
                if(tmp_signal!=NULL) {
                  if((pos=varName.find("["))>=0 && (pos1=varName.find("]"))>=0) {
                    tmp_signal->isArray=true;
                    tmp_signal->setArraySize( ((varName.erase(varName.find("]"))).erase(0,pos+1)).c_str() );
                  }
//		  cout << "DEBUG: signals_vec Size (vorher): " << signals_vec.size() << endl;
                  signals_vec.push_back(tmp_signal);
//		  cout << "DEBUG: signals_vec Size (nachher): " << signals_vec.size() << endl;
		  //////DEBUG
//		  if(tmp_signal!=0) {
//		    cout << endl << "DEBUG:   tmp_signal = " << tmp_signal << endl;
//		    if(tmp_signal->variableName!=0) cout << "Name: " << (void*) tmp_signal->variableName << " - " << tmp_signal->variableName << endl;   //DEBUG
//		    if(tmp_signal->name!=0) cout << "GivenName: " << tmp_signal->name << endl;
//		    cout <<         "Array? " << tmp_signal->isArray << endl;
//		    if(tmp_signal->arraySize!=0) cout << "ArraySize: " << (void*) tmp_signal->arraySize << " - " << tmp_signal->arraySize << endl;
//		    cout <<	  "Pointer? " << tmp_signal->isPointer << endl << endl;
//		  }
//		  else
//		    cout << "DEBUG:   tmp_signal ist NULL !!!!" << endl;
                }
                else { cerr << "Signal " << varName << " could not be added to the list of signals!" << endl; }
                tmp_signal=0;
              }
              continue;
            }
//	    else cout << "DEBUG: Condition FALSE" << endl;
            if((pos=workLine.find("sc_start"))>=0) {
              if(tmp_file->CTORfirstLine<lineNum) { tmp_file->CTORfirstLine=lineNum; }
            }
          }
        }
        if(tmp_file!=NULL && tmp_file->module!=NULL) tmp_file->module->ports_vec=ports_vec;
        if(tmp_file!=NULL && tmp_file->module!=NULL) {
//	  cout << "DEBUG:  Schreibe signals_vec; size=" << signals_vec.size() << endl;
	  tmp_file->module->signals_vec=signals_vec;
//	  cout << "DEBUG:  Anschlie�end size in tmp_file: " << tmp_file->module->signals_vec.size() << endl;
	}
//	else cout << "DEBUG:  Fehler:   tmp_file ist NULL? " << (void*) tmp_file << "   ... oder tmp_file->module? " << (void*) tmp_file->module << endl;
        if(tmp_file->module==NULL) {
//	  cout << "DEBUG:  Kein Modul zur Datei zugewiesen." << endl;
	  emptyModule=new module_data();
          tmp_file->setModule(emptyModule);   // prevent NULL pointers
//	  cout << "DEBUG:   emptyModule: " << (void*) emptyModule << endl;
	  emptyModule=0;
	}
        files_vec.push_back(tmp_file);
        tmp_file=0;
        (*fp).close();
      }
      else {
        cout << "ERROR: File could not be opened!" << endl;
      }
    }

    // second run on input files
    cout << endl;
    cout << "##### Scanning for declarations in files... " << endl;
    for(int parameter=0; parameter < files_vec.size(); parameter++) {
      filename=(files_vec[parameter])->fileName;
      if((files_vec[parameter])->module!=NULL) {
        signals_vec=(files_vec[parameter])->module->signals_vec;
        ports_vec=(files_vec[parameter])->module->ports_vec;
      }
      fstream* fpin;
      fstream* fpout;
      fpin->open(filename,ios::in);
      fpout->open((string(filename)+".tmp").c_str(),ios::out);
      lineNum=-1;
      if(fpin!=nullptr && fpout!=nullptr) {
        cout << "\t" << filename << endl;
        if((files_vec[parameter])->systemcIncludeLine==1 && !(files_vec[parameter])->headerIncluded) {
          *fpout << "#include <gsysc.h>" << endl;
          getline(*fpin,zeile);
          lineNum++;
        }
        while(getline(*fpin,zeile)) {
          lineNum++;
          if((files_vec[parameter])->systemcIncludeLine>0 && !(files_vec[parameter])->headerIncluded) {
            if(lineNum+1 == (files_vec[parameter])->systemcIncludeLine) {        // Replace '#include "systemc.h"' with gsysc.h
              *fpout << "#include <gsysc.h>" << endl;
              getline(*fpin,zeile);
              lineNum++;
            }
          }
          if(!(files_vec[parameter])->incFileIncluded && (files_vec[parameter])->CTORfirstLine>0) {
            if(lineNum+1 == (files_vec[parameter])->CTORfirstLine) {
              *fpout << "  #include \"" << (files_vec[parameter])->includeFile << "\"" << endl;
            }
          }
          *fpout << zeile << endl;
          // Remove comments
          if(starComment && (pos=zeile.find("*/"))>=0) {
              zeile=zeile.erase(0,pos);
              starComment=false;
          }
          if(starComment) { continue; }  // abort loop if the whole line is part of a comment
          if((pos=zeile.find("//")) >= 0) { zeile=zeile.erase(pos); }
          while((pos=zeile.find("/*")) >= 0) {
            starComment=true;
            if((pos1=zeile.find("*/",pos)) >= 0) {
              zeile=zeile.erase(pos,pos1-pos+2);
              starComment=false;
            }
            else {
              zeile=zeile.erase(pos);
            }
          }
          if((pos=zeile.find_first_not_of(" ")) > 0) { zeile = zeile.erase(0,pos); }
          // search for declarations
          while((pos=zeile.find(";"))>=0) {
            workLine=zeile.substr(0,pos);
            zeile=zeile.erase(0,pos+1);
            // search for signals connected to some ports
            if(!((pos=workLine.find(","))>=0)) {
              for(int i=0; i<signals_vec.size(); i++) {
                if((pos1=workLine.find("("))>=0 && (pos1=(workLine.substr(workLine.find("("))).find((signals_vec[i])->variableName)) >= 0) {
                  if((pos1=workLine.find("("))>=0) { workLine = workLine.substr(0,pos1); }
                  if((pos1=workLine.find("=")) >= 0) { continue; }
                  ((signals_vec[i])->portList).push_back(workLine);
                  continue;
                }
              }
            }
            string variableType;
            module_data* typeModule=0;
            workLine=workLine.erase(0,workLine.find_first_not_of(" "));
            variableType=workLine.substr(0,workLine.find_first_of("* "));
            workLine=workLine.erase(0,workLine.find(" ")+1);

	    //cout << "--- WorkLine 1: " << workLine << endl;
            for(int i=0; i<files_vec.size(); i++) {
              if(i==parameter) { continue; }
              if((files_vec[i])->module!=NULL && (files_vec[i])->module->name!=NULL) {
                if(variableType.compare((files_vec[i])->module->name)==0) {
                  typeModule=(files_vec[i])->module;

                  // ensure loop termination:
                  workLine=workLine+",";
                  while((pos=workLine.find(","))>=0) {
	    //cout << "--- WorkLine 2: " << workLine << endl;
                    varName=workLine.substr(0,pos);
	    //cout << "--- varName 1: " << varName << endl;
                    workLine=workLine.erase(0,pos+1);
	    //cout << "--- WorkLine 3: " << workLine << endl;
                    if((pos=varName.find_first_not_of(" "))>0) varName=varName.erase(0,pos);
                    module_decl tempModule;
                    if((pos1=varName.find("=")) >= 0) { continue; }
                    if((pos1=varName.find("*"))>=0) {
                      varName=varName.erase(0,varName.find("*")+1);
                      tempModule.isPointer=true;
                      if((pos=varName.find_first_not_of(" "))>0) varName=varName.erase(0,pos);
                    }
	    //cout << "--- varName 2: " << varName << endl;
                    while(varName.at(varName.size()-1)==' ') varName=varName.erase(varName.size()-1,1);
                    if((pos1=varName.find("["))>=0) {
                      tempModule.isArray=true;
                      tempModule.setArraySize( ((varName.substr(0,varName.find("]"))).substr(varName.find("[")+1)).c_str() );


                      varName=varName.erase(varName.find("["));
                    }
	    //cout << "--- varName 3: " << varName << endl;
                    if((pos1=varName.find("("))>=0) {
                      givenName=(varName.substr(0,varName.find(")")-1)).substr(pos1+2);
                      varName=varName.erase(pos1);
                      tempModule.setRealName(givenName.c_str());
                    }
	    //cout << "--- varName 4: " << varName << endl;

                    tempModule.moduleType=typeModule;
                    tempModule.setVariableName(varName.c_str());
                    ((files_vec[parameter])->modules_vec).push_back(tempModule);
                    continue;
                  } //while
                } //if module found
              } //if no SEGFAULT
            } //for (all files)
          }
        }
        fpin->close();
        fpout->close();
        remove(filename);
        rename((string(filename)+".tmp").c_str(),filename);
      }
      else {
        cerr << "ERROR: '" << filename << "' (or temporary file '" << filename << ".tmp') could not be opened, although it was possible in a former step!\nSkip this file..." << endl;
      }
    }

    // create includable files
    cout << "\n##### Creating includable files ..." << endl;
    vector<module_decl> modules_vec;
    string port_string,port_module,port_port;
    for (int parameter=0; parameter < files_vec.size(); parameter++) {
      if((files_vec[parameter])->module!=NULL) {
//	cout << "DEBUG:        Modul-Adresse: " << (files_vec[parameter])->module << endl;
        signals_vec=(files_vec[parameter])->module->signals_vec;
        ports_vec=(files_vec[parameter])->module->ports_vec;
        modules_vec=(files_vec[parameter])->modules_vec;
      }
      else cout << "ERROR: The module of the currently processed file is NULL." << endl;
      cout << "\t" << (files_vec[parameter])->includeFile << " (ports: " << ports_vec.size() << "; signals: " << signals_vec.size() << "; modules: " << modules_vec.size() << ")" << endl;
      fstream* incfp;
      incfp->open((files_vec[parameter])->includeFile,ios::out);
      if(incfp != nullptr) {
	string incFileString=(files_vec[parameter])->includeFile;
	while ((pos=incFileString.find(".")) > 0) {
	  incFileString[pos]='_';
	}
        *incfp << "#ifndef " << incFileString << endl;
	*incfp << "#define " << incFileString << endl;
        if(ports_vec.size() > 0 || signals_vec.size() || modules_vec.size() > 0) {
          for(int i=0; i<ports_vec.size(); i++) {
            if((ports_vec[i])->isArray) {
              if(strlen((ports_vec[i])->variableName)>0) { *incfp << "for(int GSYSwrapper=0; GSYSwrapper < " << (ports_vec[i])->arraySize << "; GSYSwrapper++) {" << endl; }
            }
            if(strlen((ports_vec[i])->variableName)==0) {
              cout << "\tERROR: This port has no variable name. Skip further processing." << endl;
              continue;
            }

            *incfp << "REG_PORT((";
            if(!(ports_vec[i])->isPointer) { *incfp << "&"; }
            *incfp << (ports_vec[i])->variableName << ")";
            if((ports_vec[i])->isArray) { *incfp << " + GSYSwrapper"; }
            *incfp << ",this,0)" << endl;
            *incfp << "RENAME_PORT((";
            if(!(ports_vec[i])->isPointer) { *incfp << "&"; }
            *incfp << (ports_vec[i])->variableName << ")";
            if((ports_vec[i])->isArray) {
             *incfp << " + GSYSwrapper,\"";
            }
            else {
              *incfp << ",\"";
            }
            if(strlen((ports_vec[i])->name) > 0) {
              *incfp << (ports_vec[i])->name;
            }
            else {
              *incfp << (ports_vec[i])->variableName;
            }
            *incfp << "\")" << endl;

            if((ports_vec[i])->isArray) {
              *incfp << "}" << endl;
            }
          }
          for(int i=0; i<modules_vec.size(); i++) {
            if((modules_vec[i]).isArray) {
              *incfp << "for(int GSYSwrapper=0; GSYSwrapper<" << (modules_vec[i]).arraySize << "; GSYSwrapper++) {" << endl;
            }
            *incfp << "REG_MODULE(" << (modules_vec[i]).variableName;
            if((modules_vec[i]).isArray) { *incfp << "[GSYSwrapper]"; }
            *incfp << ",\"";
            if(modules_vec[i].givenName!=0) *incfp << modules_vec[i].givenName;
            else *incfp << (modules_vec[i]).moduleType->name;
            if((files_vec[parameter])->module->isMain) *incfp << "\",0)" << endl;
	    else *incfp << "\",this)" << endl;
            if((modules_vec[i]).isArray) {
              *incfp << "}" << endl;
            }
          }
          for(int i=0; i<signals_vec.size(); i++) {
            if((signals_vec[i])->isArray) {
              if(strlen((signals_vec[i])->variableName)>0) { *incfp << "for(int GSYSwrapper; GSYSwrapper < " << (signals_vec[i])->arraySize << "; GSYSwrapper++) {" << endl; }
            }
            if(strlen((signals_vec[i])->variableName)==0) {
              cout << "\tERROR: This signal has no variable name. Skip further processing." << endl;
              continue;
            }
            for(int o=0; o<((signals_vec[i])->portList).size(); o++) {
              *incfp << "REG_PORT(";
              port_string=((signals_vec[i])->portList)[o];

              if((pos=port_string.find("."))>=0 || (pos1=port_string.find("->"))>=0) {
                if(pos>=0) port_module=port_string.substr(0,pos);
                else port_module=port_string.substr(0,pos1);
                if((pos1=port_module.find("->"))>=0) {          // assume level differences of at most 1 between module and port
                  port_module=port_module.substr(0,pos);
                }
                port_port=port_string.substr(port_module.size());
                if((pos1=port_port.rfind("."))>=0) port_port=port_port.erase(0,pos1+1);
                if((pos1=port_port.rfind("->"))>=0) port_port=port_port.erase(0,pos1+2);
                while((pos1=port_module.find(" "))>=0) port_module=port_module.erase(pos1,1);
                while((pos1=port_port.find(" "))>=0) port_port=port_port.erase(pos1,1);

                for(int u=0; u<(files_vec[parameter])->modules_vec.size(); u++) {

                  if(port_module.compare((((files_vec[parameter])->modules_vec)[u]).variableName)==0) {
                    vector<port_data*> tempports = ((files_vec[parameter])->modules_vec)[u].moduleType->ports_vec;
                    for(int a=0; a<tempports.size(); a++) {

                      if(port_port.compare(tempports[a]->variableName)==0) {
                        if(!tempports[a]->isPointer) { *incfp << "&"; }
                      }
                    }
                  }
                }
              }
              *incfp << "(";
              *incfp << ((signals_vec[i])->portList)[o] << ")";
              if((signals_vec[i])->isArray) {
                *incfp << " + GSYSwrapper";
              }
              varName=((signals_vec[i])->portList)[o];
              if((pos=varName.find("->"))>=0) varName=varName.erase(pos);
              if((pos=varName.find("."))>=0) varName=varName.erase(pos);
              *incfp << "," << varName << ",";
              if(!(signals_vec[i])->isPointer) { *incfp << "&"; }
              *incfp << (signals_vec[i])->variableName << ")" << endl;
            }
            *incfp << "RENAME_SIGNAL((";
            if(!(signals_vec[i])->isPointer) { *incfp << "&"; }
            *incfp << (signals_vec[i])->variableName << ")";
            if((signals_vec[i])->isArray) {
              *incfp << " + GSYSwrapper,\"";
            }
            else {
              *incfp << ",\"";
            }
            if(strlen((signals_vec[i])->name) > 0) {
              *incfp << (signals_vec[i])->name;
            }
            else {
              *incfp << (signals_vec[i])->variableName;
            }
            *incfp << "\")" << endl;
            if((signals_vec[i])->isArray) {
              *incfp << "}" << endl;
            }
          }
        }
        else {
          cout << "\t\tWARNING: Nothing to include for this file." << endl;
        }
        *incfp << "#endif" << endl;
        incfp->close();
      }
      else {
        cout << "ERROR: Could not create this file!" << endl;
      }
    }
  }
}
