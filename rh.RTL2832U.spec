#
# This file is protected by Copyright. Please refer to the COPYRIGHT file
# distributed with this source distribution.
#
# This file is part of RTL2832U Device.
#
# RTL2832U Device is free software: you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the
# Free Software Foundation, either version 3 of the License, or (at your
# option) any later version.
#
# RTL2832U Device is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
# for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see http://www.gnu.org/licenses/.
#

# By default, the RPM will install to the standard REDHAWK SDR root location (/var/redhawk/sdr)
# You can override this at install time using --prefix /new/sdr/root when invoking rpm (preferred method, if you must)
%{!?_sdrroot: %define _sdrroot /var/redhawk/sdr}
%define _prefix %{_sdrroot}
Prefix:         %{_prefix}

# Point install paths to locations within our target SDR root
%define _sysconfdir    %{_prefix}/etc
%define _localstatedir %{_prefix}/var
%define _mandir        %{_prefix}/man
%define _infodir       %{_prefix}/info

Name:           rh.RTL2832U
Version:        2.0.2
Release:        1%{?dist}
Summary:        Device %{name}

Group:          REDHAWK/Devices
License:        GPLv3+
Source0:        %{name}-%{version}.tar.gz
BuildRoot:      %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

BuildRequires:  redhawk-devel >= 2.0
Requires:       redhawk >= 2.0

# Interface requirements
BuildRequires:  frontendInterfaces >= 2.2 bulkioInterfaces >= 2.0
Requires:       frontendInterfaces >= 2.2 bulkioInterfaces >= 2.0

# RTL requirements
BuildRequires:  libusb1-devel >= 1.0.0
BuildRequires:  librtlsdr >= 0.5.2
Requires:       librtlsdr >= 0.5.2

Obsoletes:      RTL2832U < 2.0.0


%description
Device %{name}
 * Commit: __REVISION__
 * Source Date/Time: __DATETIME__


%prep
%setup -q


%build
# Implementation cpp
pushd cpp
./reconf
%define _bindir %{_prefix}/dev/devices/rh/RTL2832U/cpp
%configure
make %{?_smp_mflags}
popd


%install
rm -rf $RPM_BUILD_ROOT
# Implementation cpp
pushd cpp
%define _bindir %{_prefix}/dev/devices/rh/RTL2832U/cpp
make install DESTDIR=$RPM_BUILD_ROOT
popd


%clean
rm -rf $RPM_BUILD_ROOT


%files
%defattr(-,redhawk,redhawk,-)
%dir %{_prefix}/dev/devices/rh/RTL2832U
%{_prefix}/dev/devices/rh/RTL2832U/nodeconfig.py
%{_prefix}/dev/devices/rh/RTL2832U/nodeconfig.pyc
%{_prefix}/dev/devices/rh/RTL2832U/nodeconfig.pyo
%{_prefix}/dev/devices/rh/RTL2832U/RTL2832U.scd.xml
%{_prefix}/dev/devices/rh/RTL2832U/RTL2832U.prf.xml
%{_prefix}/dev/devices/rh/RTL2832U/RTL2832U.spd.xml
%{_prefix}/dev/devices/rh/RTL2832U/cpp

