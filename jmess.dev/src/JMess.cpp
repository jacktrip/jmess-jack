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


/**
 * ******************************************************************************
 * JMess.cpp
 * ******************************************************************************
 */
#include "JMess.h"


//-------------------------------------------------------------------------------
//JMess::JMess()
//-------------------------------------------------------------------------------
JMess::JMess()
{
  //Open a client connection to the JACK server.  Starting a
  //new server only to list its ports seems pointless, so we
  //specify JackNoStartServer. 
  client = jack_client_open ("lsp", JackNoStartServer, &status);
  if (client == NULL) {
    if (status & JackServerFailed) {
      cerr << "JACK server not running" << endl;
    } else {
      cerr << "jack_client_open() failed, " 
	   << "status = 0x%2.0x\n" << status << endl;
    }
    exit(1);
  }
}


//-------------------------------------------------------------------------------
//JMess::~JMess()
//-------------------------------------------------------------------------------
JMess::~JMess()
{
  jack_client_close(client);
}


//-------------------------------------------------------------------------------
//void JMess::writeOutput()
//-------------------------------------------------------------------------------
void JMess::writeOutput()
{
  //QVector<QVector<QString> > ConnectedPorts;
  QVector<QString> OutputInput(2);
  //ConnectedPorts = getConnectedPorts();

  this->setConnectedPorts();

  root = jmess_xml.createElement("jmess");
  for (QVector<QVector<QString> >::iterator it = ConnectedPorts.begin();
       it != ConnectedPorts.end(); ++it) {
    OutputInput = *it;
    //cout << "----------------------------------------------------" << endl;
    //cout << "Output ===> " <<qPrintable(OutputInput[0]) << endl;
    //cout << "Input ===> " <<qPrintable(OutputInput[1]) << endl;

    //Initialize XML elements
    connection = jmess_xml.createElement("connection");
    output = jmess_xml.createElement("output");
    input = jmess_xml.createElement("input");
    output_name = jmess_xml.createTextNode(OutputInput[0]);
    input_name = jmess_xml.createTextNode(OutputInput[1]);

    jmess_xml.appendChild(root);
    root.appendChild(connection);
    connection.appendChild(output);
    connection.appendChild(input);
    output.appendChild(output_name);
    input.appendChild(input_name);
  }

  QFile file("caca.xml");
  if (!file.open(QIODevice::WriteOnly)) {
    cerr << "Cannot open file for writing: "
	 << qPrintable(file.errorString()) << endl;
    exit(1);
    //return 1;
  }

  QTextStream out(&file);
  jmess_xml.save(out, Indent);
}


//-------------------------------------------------------------------------------
//void JMess::writeOutput()
//
//getConnectedPorts method is going to return a list of 
//ouput ports that have connections, i.e., that are connected to something
//-------------------------------------------------------------------------------
void JMess::setConnectedPorts()
{
  ConnectedPorts.clear();

  const char **ports, **connections; //vector of ports and connections
  QVector<QString> OutputInput(2); //helper variable

  //Get active output ports.
  ports = jack_get_ports (client, NULL, NULL, JackPortIsOutput);
  
  for (unsigned int out_i = 0; ports[out_i]; ++out_i) {
    if ((connections = jack_port_get_all_connections 
	 (client, jack_port_by_name(client, ports[out_i]))) != 0) {
      for (unsigned int in_i = 0; connections[in_i]; ++in_i) {
	OutputInput[0] = ports[out_i];
	//cout << "Output ===> " <<qPrintable(OutputInput[0]) << endl;
	OutputInput[1] = connections[in_i];
	//cout << "Input ===> " << qPrintable(OutputInput[1]) << endl;
	ConnectedPorts.append(OutputInput);
      }
    }
  }

  free(ports);
  //return ConnectedPorts;
}


//-------------------------------------------------------------------------------
//void JMess::disconnectAll()
//-------------------------------------------------------------------------------
void JMess::disconnectAll()
{
  QVector<QString> OutputInput(2);

  this->setConnectedPorts();

  for (QVector<QVector<QString> >::iterator it = ConnectedPorts.begin();
       it != ConnectedPorts.end(); ++it) {
    OutputInput = *it;
    //*********************************
    //TODO: Check success of disconnect
    //*********************************
    jack_disconnect(client, OutputInput[0].toAscii(), OutputInput[1].toAscii());
  }
}


//-------------------------------------------------------------------------------
//void JMess::parseXML()
//-------------------------------------------------------------------------------
int JMess::parseXML()
{
  PortsToConnect.clear();
  QString errorStr;
  int errorLine;
  int errorColumn;
  
  QFile file("input_test.xml");
  if (!file.open(QIODevice::ReadOnly)) {
    cerr << "Cannot open file for reading: "
	 << qPrintable(file.errorString()) << endl;
    return 1;
  }

  QDomDocument doc;
  if (!doc.setContent(&file, true, &errorStr, &errorLine,
		      &errorColumn)) {
    cerr << "===================================================\n" 
	 << "Error parsing XML input file:\n"
	 << "Parse error at line " << errorLine
	 << ", column " << errorColumn << "\n"
	 << qPrintable(errorStr) << "\n"
	 << "===================================================\n";
    return 1;
  }
  
  QDomElement jmess = doc.documentElement();
  if (jmess.tagName() != "jmess") {
    cerr << "Error: Root tag should be <jmess>: "
	 << qPrintable(root.tagName()) << endl;
    return 1;
  }


  QVector<QString> OutputInput(2);
  //First check for <connection> tag
  for(QDomNode n_cntn = jmess.firstChild(); 
      !n_cntn.isNull(); n_cntn = n_cntn.nextSibling()) {
    QDomElement cntn = n_cntn.toElement();
    if (cntn.tagName() == "connection") {
      //cout << qPrintable(cntn.tagName()) << endl;

      //Now check for ouput & input tag
      for(QDomNode n_sck = cntn.firstChild(); 
	  !n_sck.isNull(); n_sck = n_sck.nextSibling()) {
	QDomElement sck = n_sck.toElement();
	//cout << qPrintable(sck.tagName()) << endl;
	//cout << qPrintable(sck.text()) << endl;
	//cout << "===============================================" << endl;
	if (sck.tagName() == "output") {
	  //cout << "OUTPUT" << endl;
	  OutputInput[0] = sck.text();
	}
	else if (sck.tagName() == "input") {
	  //cout << "INPUT" << endl;
	  OutputInput[1] = sck.text();
	}
      }
      PortsToConnect.append(OutputInput);
      //cout << "SUPERCACUMEN" << endl;
      //cout << qPrintable(OutputInput[0]) << endl;
      //cout << qPrintable(OutputInput[1]) << endl;
    }
  }

  return 0;

}


//-------------------------------------------------------------------------------
//void JMess::connectPorts()
//-------------------------------------------------------------------------------
void JMess::connectPorts()
{
  QVector<QString> OutputInput(2);
  //cout << "this->parseXML();" << endl;
  this->parseXML();

  for (QVector<QVector<QString> >::iterator it = PortsToConnect.begin();
       it != PortsToConnect.end(); ++it) {
    OutputInput = *it;
    //*********************************
    //TODO: Check success of disconnect
    //*********************************
    //cout << qPrintable(OutputInput[0]) << endl;
    //cout << qPrintable(OutputInput[1]) << endl;
    jack_connect(client, OutputInput[0].toAscii(), OutputInput[1].toAscii());
  }

}








































  /*
   * Iterate thought all the ports (outputs) to get the connections 
   * to which (inputs) they are connected.
   * TODO:***************
   *  Output an array of strings with all the connections. Use qt string classes
   * ********************   
  */

  /*
  for (unsigned int i = 0; ports[i]; ++i) {
    //printf ("%s\n", ports[i]);
    if ((connections = jack_port_get_all_connections (client, jack_port_by_name(client, ports[i]))) != 0) {
      cout << "-------------------------" << endl;
      cout << "OUTPUT " << ports[i] << endl;
      //printf ("%s\n", ports[i]);
      for (unsigned int j = 0; connections[j]; j++) {
	cout << "IS CONNECTED TO INPUT " << connections[j] << endl;
	//printf ("   %s\n", connections[j]);
      }
      free (connections);
    } 
  }  
  */

  //caca = ports;
  /*
   * TODO:***************
   * Try to get just the ouput ports that are connected to something
   * in a smarter way than the previous section
   * ********************  
   */
  /*
  for (unsigned int i = 0; ports[i]; ++i) {
    cout <ck_port_connected (jack_port_by_name(client, ports[i]));
  }
  */


  
  /*
  cout << "SUPER CACA" << endl;
  for (unsigned int i = 0; ports[i]; ++i) {
    //cout << "LIST OF ACTIVE PORTS (OUPUTS): " << i << endl;
    //cout << ports[i] << endl;

    caca.append(ports[i]);
    cout << "mamukea   " << qPrintable(caca[i]) << endl;


    //caca[i] = "fdsjk fjsdklfjs klfsjdlfk s";
    //cout << qPrintable(caca.at(i)) << endl;
    //cout << "connected_ports[i]" << endl;
    //cout << caca.value(i) << endl;
  }
  return caca;
  */


  //QString cacumen = "cacument String";
  //cout << "FAULT!!!!! " << endl;
  //cout << qPrintable(cacumen) << endl;










