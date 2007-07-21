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
 * JMess.h
 */

#ifndef __JMESS_H
#define __JMESS_H

#include <iostream>
#include <string>
#include <errno.h>

#include <QIODevice>
#include <QString>
#include <QVector>
#include <QtXml>
#include <QXmlSimpleReader>
#include <QXmlInputSource>
#include <QXmlContentHandler>

#include <jack/jack.h>

using namespace std;

const int Indent = 2;

//-------------------------------------------------------------------------------
/*! \brief Class to save and load all jack client connections.
 *
 * Saves an XML file with all the current jack connections. This same file can
 * be loaded to connect evrything again. The XML file can also be edited.
 *
 * Has also an option to disconnect all the clients.
 */
//-------------------------------------------------------------------------------
class JMess {

public: 
  JMess();
  virtual ~JMess();

  void disconnectAll();
  void writeOutput(QString xmlOutFile);
  void connectPorts(QString xmlInFile);

private:
  void setConnectedPorts();
  int parseXML(QString xmlInFile);

  jack_client_t *mClient; //Class client
  jack_status_t mStatus; //Class client status

  //Vectors of Connected Ports and Ports to connects
  //This are a matrix (Nx2) of string like this:
  //OuputPort1 InputPort1
  // ...
  //OuputPortN InputPortN
  QVector<QVector<QString> > mConnectedPorts;
  QVector<QVector<QString> > mPortsToConnect;
};
#endif
