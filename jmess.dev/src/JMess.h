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
 * JMess.h
 * ******************************************************************************
 */
#ifndef __JMESS_H
#define __JMESS_H

#include <iostream>

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

class JMess {

public: 
  JMess();
  virtual ~JMess();

  void writeOutput();
  void setConnectedPorts();
  void disconnectAll();
  void parseXML();

private:
  jack_client_t *client; //dummy client to get ports
  //jack_status_t status;
  //const char **ports, **connections; //vector of ports and connections
  //const char **connected_ports;
  //QVector<QString> caca;

  QVector<QVector<QString> > ConnectedPorts;

  QDomDocument jmess_xml;
  QDomElement root;
  QDomElement connection;
  QDomElement output;
  QDomElement input;
  QDomText output_name;
  QDomText input_name;
};
#endif
