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

JMess::JMess()
{
  connection = jmess_xml.createElement("connection");
  output = jmess_xml.createElement("output");
  input = jmess_xml.createElement("input");
  output_name = jmess_xml.createTextNode("");
  input_name = jmess_xml.createTextNode("");
  cout << "cacumen CONSTR" << endl;
}


JMess::~JMess()
{

}


void JMess::writeOutput()
{

}

/**
 * JMess::getConnectedPorts method is going to return a list of 
 * ouput ports that have connections, i.e., that are connected to something
*/
void JMess::getConnectedPorts()
{
  /** 
   * Open a client connection to the JACK server.  Starting a
   * new server only to list its ports seems pointless, so we
   * specify JackNoStartServer. 
   */
  client = jack_client_open ("lsp", JackNoStartServer, &status);
  if (client == NULL) {
    if (status & JackServerFailed) {
      fprintf (stderr, "JACK server not running\n");
    } else {
      fprintf (stderr, "jack_client_open() failed, "
	       "status = 0x%2.0x\n", status);
    }
    //return 1;
  }

  /**
   * Get ports contains a list of all the ports active.
   * We only get ouput ports here.
   */
  ports = jack_get_ports (client, NULL, NULL, JackPortIsOutput);
  
  /**
   * Iterate thought all the ports (outputs) to get the connections 
   * to which (inputs) they are connected.
   * TODO:***************
   *  Output an array of strings with all the connections. Use qt string classes
   * ********************   
  */
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
}


