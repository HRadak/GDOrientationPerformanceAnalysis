/**############################################################################
#
#
# Copyright (C) 2021 by Christian Scheunert
#
# Email: christian.scheunert@tu-dresden.de
#
#############################################################################*/

/**############################################################################
# INCLUDES
#############################################################################*/

#include "iodirectory.h"


/**############################################################################
# GLOBALS
#############################################################################*/


/**############################################################################
# LOCAL DEFINITIONS
#############################################################################*/


/**############################################################################
# MEMBER DEFINITIONS
#############################################################################*/

/******************************************************************************
 *****************************************************************************/
IoDirectoryExcept::IoDirectoryExcept(int _num)
{
  num = _num;
}


/******************************************************************************
 *****************************************************************************/
IoDirectory::IoDirectory(const string &_dirName)
{
  try {
    dirName = _dirName;
  }
  catch (...) {
    cerr << "ERROR : UNKNOWN : ";
    cerr << "IoDirectory::IoDirectory" << endl;
    throw;
  }
}


/******************************************************************************
 *****************************************************************************/
IoDirectory::~IoDirectory()
{
  try {

  }
  catch (...) {
    cerr << "ERROR : UNKNOWN : ";
    cerr << "IoDirectory::~IoDirectory" << endl;
    throw;
  }
}


/******************************************************************************
 *****************************************************************************/
void IoDirectory::create(const string &_dirName)
{
  string tmpDirName;

  try {
    /** check the given directory name ***************************************/
    if (_dirName == "") {
      if (dirName == "") {
        throw IoDirectoryExcept(ERROR_COULD_NOT_CREATE_DIRECTORY);
      }
      else {
        tmpDirName = dirName;
      }
    }
    else {
      tmpDirName = _dirName;
    }

    /** create the directory *************************************************/

#if defined WIN32
    if (mkdir(tmpDirName.c_str())) {
      throw IoDirectoryExcept(ERROR_COULD_NOT_CREATE_DIRECTORY);
    }

#else
    if (mkdir(tmpDirName.c_str(), (S_IRWXU | S_IRGRP | S_IXGRP))) {
      throw IoDirectoryExcept(ERROR_COULD_NOT_CREATE_DIRECTORY);
    }
#endif

  }
  catch (IoDirectoryExcept e) {
    if (e.num == ERROR_COULD_NOT_CREATE_DIRECTORY) {
      cerr << "ERROR : COULD_NOT_CREATE_DIRECTORY : ";
      cerr << "directory name = " << tmpDirName << " : ";
    }
    cerr << "IoDirectory::create" << endl;
    throw;
  }
  catch (...) {
    cerr << "ERROR : UNKNOWN : ";
    cerr << "IoDirectory::create" << endl;
    throw;
  }
}


/******************************************************************************
 *****************************************************************************/
void IoDirectory::rename(const string &_oldDirName, const string &_newDirName)
{
  string x,y;

  try {
    if (::rename(_oldDirName.c_str(), _newDirName.c_str())) {
      throw IoDirectoryExcept(ERROR_COULD_NOT_RENAME_DIRECTORY);
    }
  }
  catch (IoDirectoryExcept e) {
    if (e.num == ERROR_COULD_NOT_RENAME_DIRECTORY) {
      cerr << "ERROR : COULD_NOT_RENAME_DIRECTORY : ";
      cerr << "old directory name = " << _oldDirName << " , ";
      cerr << "new directory name = " << _newDirName << " : ";
    }
    cerr << "IoDirectory::rename" << endl;
    throw;
  }
  catch (...) {
    cerr << "ERROR : UNKNOWN : ";
    cerr << "IoDirectory::rename" << endl;
    throw;
  }
}


/**############################################################################
# NON MEMBER DEFINITIONS
#############################################################################*/


/**############################################################################
# END OF FILE
#############################################################################*/
