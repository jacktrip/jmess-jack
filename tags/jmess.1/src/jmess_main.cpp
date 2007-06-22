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
#include <string>

#include "JMess.h"
#include "anyoption.h"

using namespace std;

void main_dialog( int argc, char* argv[] );

int main(int argc, char** argv)
{

  main_dialog( argc, argv ); 

  return 0;

}


void main_dialog( int argc, char* argv[] )
{
  
  //1. CREATE AN OBJECT
  AnyOption *opt = new AnyOption();
  
  //2. SET PREFERENCES
  //opt->noPOSIX(); /* do not check for POSIX style character options */
  opt->setVerbose(); /* print warnings about unknown options */
  opt->autoUsagePrint(true); /* print usage for bad options */
  
  //3. SET THE USAGE/HELP
  opt->addUsage("JMess: A simple utility so save your jack-audio mess.");
  opt->addUsage("Copyright (C) 2007 Juan-Pablo Caceres.");
  opt->addUsage("");
  opt->addUsage("Usage: " );
  opt->addUsage("--------------------------------------------" );
  opt->addUsage(" -h  --help                    Prints this help");
  opt->addUsage(" -c  --connect  inputfile.xml  Load the connections specified at inputfile.xml");
  opt->addUsage(" -s  --save  outputfile.xml    Save current connections in output.xml");
  opt->addUsage(" -d  --disconnectall           Disconnect all the connections");
  opt->addUsage("" );
  
  //4. SET THE OPTION STRINGS/CHARACTERS
  
  //for options that will be checked only on the command and line not in option/resource file
  opt->setCommandFlag("help", 'h'); //a flag (takes no argument) 
  opt->setCommandFlag( "disconnectall", 'd');
  opt->setCommandOption("connect", 'c'); // an option (takes an argument)
  opt->setCommandOption("save", 's');
  opt->setCommandFlag( "disconnectall", 'd');
  
  
  // 5. PROCESS THE COMMANDLINE

  //go through the command line and get the options
  opt->processCommandArgs( argc, argv );
  
  
  if( ! opt->hasOptions()) { //print usage if no options
    opt->printUsage();
    delete opt;
    return;
  }

  //6. GET THE VALUES
  if(opt->getFlag("help")) 
    opt->printUsage();

  //Create JMess Object for the following flags
  JMess jmessClient;

  if(opt->getFlag("disconnectall")) {
    //Confirm before disconnection
    string answer = "";
    while ((answer != "yes") && (answer != "no")) {
      cout << "Are you sure you want to disconnect all? (yes/no): ";
      cin >> answer;
    }
    if (answer == "yes") {
      jmessClient.disconnectAll();
    }
  }

  if(opt->getValue("connect") != NULL)
    jmessClient.connectPorts(opt->getValue("connect"));

  if(opt->getValue("save") != NULL)
    jmessClient.writeOutput(opt->getValue("save"));

  
  //8. DONE
  delete opt;
  
}
