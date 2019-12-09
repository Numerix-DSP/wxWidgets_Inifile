@echo off

rem Batch file for creating SA application for Borland C++ V5.5.1 and wxWidgets > 2.6.0

rem Create the bakefile
bakefile -f borland project.bkl

rem Build the application
make -f makefile.bcc -DBUILD=release -DWX_DEBUG=0

echo Done ...
