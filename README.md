# JMess - A utility to save your audio connections (mess) #

JMess can save an XML file with all the current [Jack Audio](http://jackaudio.org/) connections. This same file can be loaded to connect everything again. The XML file can also be edited. It also also has the option to disconnect all the clients.

## Download and Installation ##
There is a Linux and a Mac OS X version of JMees. To install just download, uncompress, and follow the instrunctions on `INSTALL.txt`.

## Using JMess ##

To save connections on Jack, use for example [QjackCtl](http://qjackctl.sourceforge.net/) to make the connections you need. To save them, just type in the command line:

`jmess -s filename.xml`

This will save your connections in a file filename.xml. This is an XML file that you can edit, if you want.

To load your connections from a file, just type:

`jmess -c filename.xml`

Note that if you have connections running, the command above is going to connect what was saved on the filename.xml, keeping the connections that you have. If you want to get exactly what was saved on filename.xml, use the flag -d to disonnect all:

`jmess -d -c filename.xml`

If you just want to disconnect all from the command line:

`jmess -d`

For a complete list of options, just type:

`jmess`
