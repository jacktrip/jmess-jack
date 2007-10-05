Summary: A utility to save your audio connections (mess)
Name: jmess
Version: 1
Release: 1%{?dist}
License: GPL
Group: Applications/Multimedia
URL: http://ccrma.stanford.edu/groups/soundwire/software/jmess/download/jmess.1.tar.gz
Source0: http://ccrma.stanford.edu/groups/soundwire/software/jmess/download/jmess.%{version}.tar.gz
BuildRoot: %{_tmppath}/%{name}.%{version}-%{release}-root-%(%{__id_u} -n)
Packager: Juan-Pablo Caceres
Vendor: Planet CCRMA
Distribution: Planet CCRMA

BuildRequires: qt4-devel
BuildRequires: jack-audio-connection-kit-devel alsa-lib-devel

%description
JMess can save an XML file with all the current jack connections.
This same file can be loaded to connect everything again. The XML
file can also be edited. It also also has the option to disconnect 
all the clients.

%prep
%setup -q -n jmess.%{version}

%build
cd src
./m
make

%install
%{__rm} -rf %{buildroot}
%{__mkdir} -p %{buildroot}%{_bindir}
%{__install} -m 0755 src/jmess %{buildroot}%{_bindir}/jmess

%clean
%{__rm} -rf %{buildroot}


%files
%defattr(-,root,root,-)
%doc INSTALL.txt
%{_bindir}/jmess


%changelog
* Fri Oct 05 2007 Juan-Pablo Caceres <jcaceres@ccrma.stanford.edu> - 1-1
- initial build.
