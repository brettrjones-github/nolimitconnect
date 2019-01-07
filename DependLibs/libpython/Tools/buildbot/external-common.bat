@rem Common file shared between external.bat and external-amd64.bat.  Responsible for
@rem fetching external components into the root\externals directory.

if "%SVNROOT%"=="" set SVNROOT=http://svn.python.org/projects/external/

if not exist externals mkdir externals
cd externals

@rem XXX: If you need to force the buildbots to start from a fresh environment, uncomment
@rem the following, check it in, then check it out, comment it out, then check it back in.
@rem if exist bzip2-1.0.6 rd /s/q bzip2-1.0.6
@rem if exist tcltk rd /s/q tcltk
@rem if exist tcltk64 rd /s/q tcltk64
@rem if exist tcl8.4.12 rd /s/q tcl8.4.12
@rem if exist tcl8.4.16 rd /s/q tcl8.4.16
@rem if exist tcl-8.4.18.1 rd /s/q tcl-8.4.18.1
@rem if exist tcl-8.5.2.1 rd /s/q tcl-8.5.2.1
@rem if exist tcl-8.5.15.0 rd /s/q tcl-8.5.15.0
@rem if exist tk8.4.12 rd /s/q tk8.4.12
@rem if exist tk8.4.16 rd /s/q tk8.4.16
@rem if exist tk-8.4.18.1 rd /s/q tk-8.4.18.1
@rem if exist tk-8.5.2.0 rd /s/q tk-8.5.2.0
@rem if exist tk-8.5.2.1 rd /s/q tk-8.5.2.1
@rem if exist tk-8.5.15.0 rd /s/q tk-8.5.15.0
@rem if exist tix-8.4.3.5 rd /s/q tix-8.4.3.5
@rem if exist db-4.4.20 rd /s/q db-4.4.20
@rem if exist db-4.7.25.0 rd /s/q db-4.7.25.0
@rem if exist openssl-0.9.8y rd /s/q openssl-0.9.8y
@rem if exist openssl-1.0.1f rd /s/q openssl-1.0.1f
@rem if exist openssl-1.0.1g rd /s/q openssl-1.0.1g
@rem if exist openssl-1.0.1h rd /s/q openssl-1.0.1h
@rem if exist openssl-1.0.1i rd /s/q openssl-1.0.1i
@rem if exist openssl-1.0.1j rd /s/q openssl-1.0.1j
@rem if exist openssl-1.0.1l rd /s/q openssl-1.0.1l
@rem if exist openssl-1.0.2a rd /s/q openssl-1.0.2a
@rem if exist openssl-1.0.2b rd /s/q openssl-1.0.2b
@rem if exist openssl-1.0.2o rd /s/q openssl-1.0.2o
@rem if exist sqlite-3.8.10 rd /s/q sqlite-3.8.10

@rem bzip
@rem if not exist ..\..\bzip2-1.0.6\NUL (
@rem    rd /s/q bzip2-1.0.6
@rem   svn export %SVNROOT%bzip2-1.0.6
@rem )

@rem Berkeley DB
@rem if exist ..\..\db-4.7.25.0 rd /s/q db-4.7.25.0
@rem if not exist db-4.7.25.0 svn export %SVNROOT%db-4.7.25.0

@rem NASM, for OpenSSL build
@rem if exist nasm-2.11.06 rd /s/q nasm-2.11.06
@rem if not exist ..\..\nasm-2.13.01\NUL svn export %SVNROOT%nasm-2.13.01

@rem OpenSSL
@rem if exist ..\..\openssl-1.0.2b\NUL rd /s/q openssl-1.0.2b
@rem if not exist openssl-1.0.2b/ svn export %SVNROOT%openssl-1.0.2b

@rem tcl/tk
@rem if not exist ..\..\tcl-8.5.2.1\NUL (
@rem rd /s/q tcltk tcltk64 tcl-8.5.2.1 tcl-8.5.2.1
@rem svn export %SVNROOT%tcl-8.5.2.1
@rem )

@rem if not exist ..\..\tk-8.5.2.0\NUL ( 
@rem svn export %SVNROOT%tk-8.5.2.0
@rem )

@rem if not exist ..\..\tix-8.4.3.5\NUL ( 
@rem svn export %SVNROOT%tix-8.4.3.5
@rem )

@rem sqlite3
@rem if not exist ..\..\sqlite-3.8.10\NUL (
@rem   rd /s/q sqlite-3.8.10
@rem   svn export %SVNROOT%sqlite-3.8.10
@rem )

@rem change back to root Python
cd ../