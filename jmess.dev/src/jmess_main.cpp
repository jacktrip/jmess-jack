/*
    JMess: A simple utility so save your jack-audio mess.

    Copyright (C) 2007 Juan-Pablo Caceres.
    
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/


#include <iostream>
#include <QtXml>
#include <QIODevice>
//#include <QIODevice>

#include "JMess.h"

#include "anyoption.h"

using namespace std;

void main_dialog( int argc, char* argv[] );

int main(int argc, char** argv)
{
  
  main_dialog( argc, argv ); 


  cout << "testJackLSP" << endl;
  JMess testJackLSP;
  //cout << "writeOutput" << endl;
  //testJackLSP.writeOutput();
  //cout << "disconnectAll" << endl;
  //testJackLSP.disconnectAll();
  //cout << "parseXML" << endl;
  //testJackLSP.parseXML();
  cout << "connectPorts" << endl;
  testJackLSP.connectPorts();
 

    /*
  QDomDocument jmess_xml;
  QDomElement connection = jmess_xml.createElement("connection");
  QDomElement output = jmess_xml.createElement("output");
  QDomElement input = jmess_xml.createElement("input");
  QDomText output_name = jmess_xml.createTextNode("jack:out11111111");
  QDomText input_name = jmess_xml.createTextNode("jack:in1111111");
  
  jmess_xml.appendChild(connection);
  connection.appendChild(output);
  connection.appendChild(input);
  output.appendChild(output_name);
  input.appendChild(input_name);
  */
  

  /*
  const int Indent = 2;



  QDomDocument jmess_xml;
  QDomElement connection = jmess_xml.createElement("connection");
  QDomElement output = jmess_xml.createElement("output");
  QDomElement input = jmess_xml.createElement("input");
  QDomText output_name = jmess_xml.createTextNode("jack:out11111111");
  QDomText input_name = jmess_xml.createTextNode("jack:in1111111");
  
  jmess_xml.appendChild(connection);
  connection.appendChild(output);
  connection.appendChild(input);
  output.appendChild(output_name);
  input.appendChild(input_name);


  connection = jmess_xml.createElement("connection");
  output = jmess_xml.createElement("output");
  input = jmess_xml.createElement("input");
  output_name = jmess_xml.createTextNode("jack:out222");
  input_name = jmess_xml.createTextNode("jack:in2");
  
  jmess_xml.appendChild(connection);
  connection.appendChild(output);
  connection.appendChild(input);
  output.appendChild(output_name);
  input.appendChild(input_name);



  QFile file("caca.xml");
  if (!file.open(QIODevice::WriteOnly)) {
    cerr << "Cannot open file for writing: "
	 << qPrintable(file.errorString()) << endl;
    return 1;
  }

  cout << "caca.xml" << endl;
  QTextStream out(&file);
  jmess_xml.save(out, Indent);

  JMess cacumen;
  cout << "cacumen" << endl;
  */

  return 0;
}


void
main_dialog( int argc, char* argv[] )
{
  
  /* 1. CREATE AN OBJECT */
  AnyOption *opt = new AnyOption();
  
  /* 2. SET PREFERENCES  */
  //opt->noPOSIX(); /* do not check for POSIX style character options */
  //opt->setVerbose(); /* print warnings about unknown options */
  //opt->autoUsagePrint(true); /* print usage for bad options */
  
  /* 3. SET THE USAGE/HELP   */
  opt->addUsage( "" );
  opt->addUsage( "Usage: " );
  opt->addUsage( "" );
  opt->addUsage( " -h  --help  		Prints this help " );
  opt->addUsage( " -s  --size 42 	        Image Size " );
  opt->addUsage( " -z  --zip  		Compress Image " );
  opt->addUsage( " -c   			convert Image " );
  opt->addUsage( "     --name image.jpg	Image Name " );
  opt->addUsage( "" );
  
  /* 4. SET THE OPTION STRINGS/CHARACTERS */
  
  /* by default all  options  will be checked on the command line and from option/resource file */
  opt->setFlag(  "help", 'h' );   /* a flag (takes no argument), supporting long and short form */ 
  opt->setOption(  "size", 's' ); /* an option (takes an argument), supporting long and short form */
  opt->setOption(  "name" );      /* an option (takes an argument), supporting only long form */
  opt->setFlag( 'c' );            /* a flag (takes no argument), supporting only short form */
  
  /* for options that will be checked only on the command and line not in option/resource file */
  opt->setCommandFlag(  "zip" , 'z'); /* a flag (takes no argument), supporting long and short form */
  
  /* for options that will be checked only from the option/resource file */
  opt->setFileOption(  "title" ); /* an option (takes an argument), supporting only long form */
  
  /* 5. PROCESS THE COMMANDLINE AND RESOURCE FILE */
  
  /* read options from a  option/resource file with ':' separated opttions or flags, one per line */
  opt->processFile( "/home/user/.options" );  
  /* go through the command line and get the options  */
  opt->processCommandArgs( argc, argv );
  
  if( ! opt->hasOptions()) { /* print usage if no options */
    opt->printUsage();
    delete opt;
    return;
  }
  
  /* 6. GET THE VALUES */
  if( opt->getFlag( "help" ) || opt->getFlag( 'h' ) ) 
    opt->printUsage();
  if( opt->getValue( 's' ) != NULL  || opt->getValue( "size" ) != NULL  )
    cout << "size = " << opt->getValue( 's' ) << endl ;
  if( opt->getValue( "name" ) != NULL )
    cout << "name = " << opt->getValue( "name" ) << endl ;
  if( opt->getValue( "title" ) != NULL )
    cout << "title = " << opt->getValue( "title" ) << endl ;
  if( opt->getFlag( 'c' ) )  
    cout << "c = flag set " << endl ;
  if( opt->getFlag( 'z' ) || opt->getFlag( "zip" ) )  
    cout << "zip = flag set " << endl ;
  cout << endl ;
  
  /* 7. GET THE ACTUAL ARGUMENTS AFTER THE OPTIONS */
  for( int i = 0 ; i < opt->getArgc() ; i++ ){
    cout << "arg = " <<  opt->getArgv( i ) << endl ;
  }
  
  /* 8. DONE */
  delete opt;
  
}
