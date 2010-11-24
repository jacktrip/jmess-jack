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
#include <getopt.h> // for command line parsing

#include "JMess.h"
//#include "anyoption.h"

using namespace std;

void main_dialog( int argc, char* argv[] );
void printUsage();
string version  = "1.0.1";


//*******************************************************************************
int main(int argc, char** argv)
{
  main_dialog( argc, argv ); 
  return 0;
}


//*******************************************************************************
void main_dialog( int argc, char* argv[] )
{
  
  // If no command arguments are given, print instructions
  if(argc == 1) {
    printUsage();
    std::exit(0);
  }

  //Create JMess Object for the following flags
  JMess jmessClient;

  // Usage example at:
  // http://www.gnu.org/software/libc/manual/html_node/Getopt-Long-Option-Example.html#Getopt-Long-Option-Example
  // options descriptor
  //----------------------------------------------------------------------------
  static struct option longopts[] = {
    // These options don't set a flag.
    { "disconnectall", no_argument, NULL,  'd' },
    { "connect", required_argument, NULL, 'c' },
    { "save", required_argument, NULL,  's' },
    { "DisconnectAll", no_argument, NULL,  'D' },
    { "version", no_argument, NULL, 'v' }, // Version Number
    { "help", no_argument, NULL, 'h' }, // Print Help
    { NULL, 0, NULL, 0 }
  };


  // Parse Command Line Arguments
  //----------------------------------------------------------------------------
  /// \todo Specify mandatory arguments
  string answer = "";
  int ch;
  while ( (ch = getopt_long(argc, argv,
                            "dc:s:Dvh", longopts, NULL)) != -1 )
    switch (ch) {
    case 'd':
    //-------------------------------------------------------
    //Confirm before disconnection
    while ((answer != "yes") && (answer != "no")) {
      cout << "Are you sure you want to disconnect all? (yes/no): ";
      cin >> answer;
    }
    if (answer == "yes") {
      jmessClient.disconnectAll();
    }
    break;
    case 'c':
    //-------------------------------------------------------
    jmessClient.connectPorts(optarg);
    break;
    case 's':
    //-------------------------------------------------------
    jmessClient.writeOutput( optarg );
    break;
    case 'D':
    //-------------------------------------------------------
    jmessClient.disconnectAll();
    break;
    case 'v':
    //-------------------------------------------------------
    cout << "JMess VERSION: " << version << endl;
    cout << "Copyright (c) 2007-2010 Juan-Pablo Caceres." << endl;
    cout << "SoundWIRE group at CCRMA, Stanford University" << endl;
    cout << "" << endl;
    std::exit(0);
    break;
    case 'h':
    //-------------------------------------------------------
    printUsage();
    break;
  }

  // Warn user if undefined options where entered
  //----------------------------------------------------------------------------
  if (optind < argc) {
    cout << "------------------------------------------------------" << endl;
    cout << "WARINING: The following entered options have no effect" << endl;
    cout << "          They will be ignored!" << endl;
    cout << "          Type jmess to see options." << endl;
    for( ; optind < argc; optind++) {
      printf("argument: %s\n", argv[optind]);
    }
    cout << "------------------------------------------------------" << endl;
  }
}


//*******************************************************************************
void printUsage()
{
  cout << "" << endl;
  cout << "JMess: A simple utility so save your jack-audio mess." << endl;
  cout << "Copyright (C) 2007-2010 Juan-Pablo Caceres." << endl;
  cout << "VERSION: " << version << endl;
  cout << "" << endl;
  cout << "Usage: " << endl;
  cout << "--------------------------------------------" << endl;
  cout << " -h  --help                    Prints this help" << endl;
  cout << " -c  --connect  inputfile.xml  Load the connections specified at inputfile.xml" << endl;
  cout << " -s  --save  outputfile.xml    Save current connections in output.xml" << endl;
  cout << " -d  --disconnectall           Disconnect all the connections" << endl;
  cout << " -D  --DisconnectAll           Disconnect all the connections without confirmation" << endl;
  cout << "" << endl;
}
