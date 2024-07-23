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

#include "iodatafile.h"


/**############################################################################
# DEFINES
#############################################################################*/


/**############################################################################
# LOCAL DEFINITIONS
#############################################################################*/


/**############################################################################
# MEMBER DEFINITIONS
#############################################################################*/

/******************************************************************************
 *****************************************************************************/
IoDataFileExcept::IoDataFileExcept(int _num)
: IoFileExcept(_num)
{

}


/******************************************************************************
 *****************************************************************************/
IoDataFile::IoDataFile(const string &_fileName)
: IoSectionKeyFile(_fileName)
{
  try {
  
  }
  catch (...) {
    cerr << "ERROR : UNKNOWN : ";
    cerr << "IoDataFile::IoDataFile" << endl;
    throw;
  }
}


/******************************************************************************
 *****************************************************************************/
IoDataFile::~IoDataFile()
{
  try {

  }
  catch (...) {
    cerr << "ERROR : UNKNOWN : ";
    cerr << "IoDataFile::~IoDataFile" << endl;
    throw;
  }
}


/******************************************************************************
******************************************************************************/
void IoDataFile::findSection(const string &_section)
{
  try {
    while (true) {
      if (endOfFile()) {
        throw IoDataFileExcept(ERROR_SECTION_NOT_FOUND);
      }
      readLine(line);
      if (string::npos != line.find(_section)) {
        return;
      }
    }
  }
  catch (IoDataFileExcept &_e) {
    if (_e.num == ERROR_SECTION_NOT_FOUND) {
      cerr << "ERROR : SECTION_NOT_FOUND : ";
      cerr << "section = " << _section << " : ";
    }
    cerr << "file = " << '"' << fileName << '"' << " : ";
    cerr << "IoDataFile::findSection" << endl;
    throw;
  }
  catch (...) {
    cerr << "ERROR : UNKNOWN : ";
    cerr << "IoDataFile::findSection" << endl;
    throw;
  }
}


/******************************************************************************
******************************************************************************/
void IoDataFile::findKey(const string &_key, const string &_section)
{
  try {
    /** find the section *****************************************************/
    if (_section != "") {
      findSection(_section);
    }
    
    /** find the key *********************************************************/
    while (true) {
      if (endOfFile()) {
        throw IoDataFileExcept(ERROR_KEY_NOT_FOUND);
      }
      readLine(line);
      if (string::npos != line.find(_key)) {
        return;
      }
      if (line.find(SECTION_PREFIX) == 0) {
        throw IoDataFileExcept(ERROR_KEY_NOT_FOUND);
      }      
    }
  }
  catch (IoDataFileExcept &_e) {
    if (_e.num == ERROR_KEY_NOT_FOUND) {
      cerr << "ERROR : KEY_NOT_FOUND : ";
      cerr << "section = " << _section << " : ";
      cerr << "key = " << _key << " : ";
    }
    cerr << "file = " << '"' << fileName << '"' << " : ";
    cerr << "IoDataFile::findKey" << endl;
    throw;
  }
  catch (...) {
    cerr << "ERROR : UNKNOWN : ";
    cerr << "IoDataFile::findKey" << endl;
    throw;
  }
}


/******************************************************************************
 *****************************************************************************/
void IoDataFile::setStringValues(const valarray<string> &_values,
                                 const string &_key, const string &_section)
{
  cerr << "ERROR : SETTING_VALUES_NOT_SUPPORTED_BY_NOW : ";
  cerr << "IoDataFile::setStringValues" << endl;
  throw;
}


/******************************************************************************
 *****************************************************************************/
void IoDataFile::getStringValues(valarray<string> &_values, const string &_key,
                                 const string &_section,
                                 const unsigned long &_size)
{
  string endKey;
  unsigned long i;

  try {
    /** open the file for reading ********************************************/
    openFileForRead();

    /** find the section and key *********************************************/
    findKey(convStartKeyName(_key), convSectionName(_section));

    /** resize the output array **********************************************/
    if (_size == 0) {
      _values.resize(1);
    }
    else {
      _values.resize(_size);
    }

    /** get the values from file *********************************************/
    i = 0;
    endKey = convEndKeyName(_key);
    do {
      if (endOfFile()) {
        throw IoDataFileExcept(ERROR_UNEXPECTED_END_OF_FILE);
      }
      readLine(line);
      if (string::npos != line.find(endKey)) {
        throw IoDataFileExcept(ERROR_NOT_ENOUGH_VALUES);
      }
      if (! line.empty()) {
        _values[i] = line;
        i++;
      }
    } while (i < _values.size());

    /** close the file *******************************************************/
    closeFile();
  }
  catch (IoDataFileExcept &_e) {
    if (_e.num == ERROR_UNEXPECTED_END_OF_FILE) {
      cerr << "ERROR : UNEXPECTED_END_OF_FILE : ";
      cerr << "section = " << convSectionName(_section) << " : ";
      cerr << "key = " << convStartKeyName(_key) << " ... " << endKey << " : ";
    }
    if (_e.num == ERROR_NOT_ENOUGH_VALUES) {
      cerr << "ERROR : NOT_ENOUGH_VALUES : ";
      cerr << "section = " << convSectionName(_section) << " : ";
      cerr << "key = " << convStartKeyName(_key) << " ... " << endKey << " : ";
    }
    cerr << "file = " << '"' << fileName << '"' << " : ";
    cerr << "IoDataFile::getStringValues" << endl;
    closeFile();
    throw;
  }
  catch (...) {
    cerr << "ERROR : UNKNOWN : ";
    cerr << "IoDataFile::getStringValues" << endl;
    closeFile();
    throw;
  }
}


/**############################################################################
# NON MEMBER DEFINITIONS
#############################################################################*/


/**############################################################################
# END OF FILE
#############################################################################*/










