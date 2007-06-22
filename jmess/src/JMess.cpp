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


/*
 * JMess.cpp
 */

#include "JMess.h"


//-------------------------------------------------------------------------------
/*! \brief Constructs a JMess object that has a jack client.
 *
 */
//-------------------------------------------------------------------------------
JMess::JMess()
{
  //Open a client connection to the JACK server.  Starting a
  //new server only to list its ports seems pointless, so we
  //specify JackNoStartServer. 
  mClient = jack_client_open ("lsp", JackNoStartServer, &mStatus);
  if (mClient == NULL) {
    if (mStatus & JackServerFailed) {
      cerr << "JACK server not running" << endl;
    } else {
      cerr << "jack_client_open() failed, " 
	   << "status = 0x%2.0x\n" << mStatus << endl;
    }
    exit(1);
  }
}


//-------------------------------------------------------------------------------
/*! \brief Distructor closes the jmess jack audio client.
 *
 */
//-------------------------------------------------------------------------------
JMess::~JMess()
{
  if (jack_client_close(mClient))
    cerr << "ERROR: Could not close the hidden jmess jack client." << endl;
}


//-------------------------------------------------------------------------------
/*! \brief Write an XML file with the name specified at xmlOutFile.
 *
 */
//-------------------------------------------------------------------------------
void JMess::writeOutput(QString xmlOutFile)
{
  QDomDocument jmess_xml;   QDomElement root;
  QDomElement connection;   QDomElement output;
  QDomElement input;        QDomText output_name;
  QDomText input_name;
  
  QVector<QString> OutputInput(2);

  this->setConnectedPorts();

  root = jmess_xml.createElement("jmess");
  for (QVector<QVector<QString> >::iterator it = mConnectedPorts.begin();
       it != mConnectedPorts.end(); ++it) {
    OutputInput = *it;
    //cout << "Output ===> " <<qPrintable(OutputInput[0]) << endl;
    //cout << "Input ===> " <<qPrintable(OutputInput[1]) << endl;

    //Initialize XML elements
    connection = jmess_xml.createElement("connection");
    output = jmess_xml.createElement("output");
    input = jmess_xml.createElement("input");
    output_name = jmess_xml.createTextNode(OutputInput[0]);
    input_name = jmess_xml.createTextNode(OutputInput[1]);

    jmess_xml.appendChild(root);      root.appendChild(connection);
    connection.appendChild(output);   connection.appendChild(input);
    output.appendChild(output_name);  input.appendChild(input_name);
  }

  //Write output file
  QFile file(xmlOutFile);
  string answer = "";
  //Check for existing file first, and confirm before overwriting
  if (file.exists()) {
    while ((answer != "yes") && (answer != "no")) {
      cout << "WARNING: The File " <<qPrintable(xmlOutFile)
	   << " exists. Do you want to overwrite it? (yes/no): ";
      cin >> answer;
    }
  }
  else {
    answer = "yes";
  }

  if (answer == "yes") {
    if (!file.open(QIODevice::WriteOnly)) {
      cerr << "Cannot open file for writing: "
	   << qPrintable(file.errorString()) << endl;
      exit(1);
    }
    
    QTextStream out(&file);
    jmess_xml.save(out, Indent);
    cout << qPrintable(xmlOutFile) << " written." << endl;
  }
}
  

//-------------------------------------------------------------------------------
/*! \brief Set list of ouput ports that have connections.
 *
 */
//-------------------------------------------------------------------------------
void JMess::setConnectedPorts()
{
  mConnectedPorts.clear();

  const char **ports, **connections; //vector of ports and connections
  QVector<QString> OutputInput(2); //helper variable

  //Get active output ports.
  ports = jack_get_ports (mClient, NULL, NULL, JackPortIsOutput);
  
  for (unsigned int out_i = 0; ports[out_i]; ++out_i) {
    if ((connections = jack_port_get_all_connections 
	 (mClient, jack_port_by_name(mClient, ports[out_i]))) != 0) {
      for (unsigned int in_i = 0; connections[in_i]; ++in_i) {
	OutputInput[0] = ports[out_i];
	//cout << "Output ===> " <<qPrintable(OutputInput[0]) << endl;
	OutputInput[1] = connections[in_i];
	//cout << "Input ===> " << qPrintable(OutputInput[1]) << endl;
	mConnectedPorts.append(OutputInput);
      }
    }
  }

  free(ports);
}


//-------------------------------------------------------------------------------
/*! \brief Disconnect all the clients.
 *
 */
//-------------------------------------------------------------------------------
void JMess::disconnectAll()
{
  QVector<QString> OutputInput(2);
  
  this->setConnectedPorts();
  
  for (QVector<QVector<QString> >::iterator it = mConnectedPorts.begin();
       it != mConnectedPorts.end(); ++it) {
    OutputInput = *it;
    
    if (jack_disconnect(mClient, OutputInput[0].toAscii(), OutputInput[1].toAscii())) {
      cerr << "WARNING: port: " << qPrintable(OutputInput[0])
	   << "and port: " << qPrintable(OutputInput[1])
	   << " could not be disconnected.\n";
    }
  }

}


//-------------------------------------------------------------------------------
/*! \brief Parse the XML input file.
 *
 * Returns 0 on success, or 1 if the file has an incorrect format or cannot 
 * read the file.
 */
//-------------------------------------------------------------------------------
int JMess::parseXML(QString xmlInFile)
{
  mPortsToConnect.clear();
  QString errorStr;
  int errorLine;
  int errorColumn;
  
  QFile file(xmlInFile);
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
	 << qPrintable(jmess.tagName()) << endl;
    return 1;
  }


  QVector<QString> OutputInput(2);
  //First check for <connection> tag
  for(QDomNode n_cntn = jmess.firstChild(); 
      !n_cntn.isNull(); n_cntn = n_cntn.nextSibling()) {
    QDomElement cntn = n_cntn.toElement();
    if (cntn.tagName() == "connection") {
      //Now check for ouput & input tag
      for(QDomNode n_sck = cntn.firstChild(); 
	  !n_sck.isNull(); n_sck = n_sck.nextSibling()) {
	QDomElement sck = n_sck.toElement();
	//cout << qPrintable(sck.tagName()) << endl;
	//cout << qPrintable(sck.text()) << endl;
	if (sck.tagName() == "output") {
	  OutputInput[0] = sck.text();
	}
	else if (sck.tagName() == "input") {
	  OutputInput[1] = sck.text();
	}
      }
      mPortsToConnect.append(OutputInput);
    }
  }

  return 0;
  
}


//-------------------------------------------------------------------------------
/*! \brief Connect ports specified in input XML file xmlInFile
 *
 */
//-------------------------------------------------------------------------------
void JMess::connectPorts(QString xmlInFile)
{
  QVector<QString> OutputInput(2);

  if ( !(this->parseXML(xmlInFile)) ) {  
    for (QVector<QVector<QString> >::iterator it = mPortsToConnect.begin();
	 it != mPortsToConnect.end(); ++it) {
      OutputInput = *it;

      if (jack_connect(mClient, OutputInput[0].toAscii(), OutputInput[1].toAscii())) {
	//Display a warining only if the error is not because the ports are already
	//connected, in case the program doesn't display anyting.
	if (EEXIST != 
	    jack_connect(mClient, OutputInput[0].toAscii(), OutputInput[1].toAscii())) {
	  cerr << "WARNING: port: " << qPrintable(OutputInput[0])
	       << "and port: " << qPrintable(OutputInput[1])
	       << " could not be connected.\n";
	}
      }
    }
  }

}
