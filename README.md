RTL2832U
=========

## Description

A frontendInterfaces compliant device for the RTL2832U usb dongle

## Dependencies

This device depends on the RTL SDR library, version 0.5.2

## Runtime Usage

Prior to launching this device, the LD_LIBRARY_PATH may need to be 
updated to include the librtlsdr libraries.
(i.e. export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib)

There is currently a bug where the RTL interface may not work when
the device debug level is set at 'DEBUG' or 'TRACE'.  If issues occur
and your debug level is at 'DEBUG' or 'TRACE', try and set the debug 
level as 'INFO'.

## REDHAWK Documentation

REDHAWK Website: [www.redhawkSDR.org](http://www.redhawksdr.org)

Overview and Getting Started Guide: [PDF](http://sourceforge.net/projects/redhawksdr/files/redhawk-doc/1.10.0/REDHAWK_Overview_v1.10.0.pdf/download "PDF") [HTML](http://redhawksdr.github.com/Documentation/gettingstarted/main.html "HTML")

Full REDHAWK Manual: [PDF](http://sourceforge.net/projects/redhawksdr/files/redhawk-doc/1.10.0/REDHAWK_Manual_v1.10.0.pdf/download "PDF") [HTML](http://redhawksdr.github.com/Documentation/main.html "HTML")

## Copyrights

This work is protected by Copyright. Please refer to the [Copyright File](COPYRIGHT) for updated copyright information.

## License

RTL2832U is licensed under the GNU General Public License (GPL).
