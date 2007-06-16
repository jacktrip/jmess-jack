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

#include "jmess.h"

using namespace std;

int main(int argc, char** argv)
{
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

  XMLWrite cacumen;
  cout << "cacumen" << endl;

  JackLSP testJackLSP;
  testJackLSP.getConnectedPorts();

  return 0;
}
