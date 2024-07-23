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

#include "iofile.h"


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
IoFileExcept::IoFileExcept(int _num)
{
  num = _num;
}


/******************************************************************************
 *****************************************************************************/
IoFile::IoFile(const string &_fileName)
{
  try {
    lineNumber = 0;
  	p_file = 0;

    if (_fileName != "") {
      fileName = _fileName;
    }
  }

  catch (...) {
    cerr << "ERROR : UNKNOWN : ";
    cerr << "IoFile::IoFile" << endl;
    throw;
  }
}


/******************************************************************************
 *****************************************************************************/
IoFile::~IoFile()
{
  try {
    /** */
    closeFile();
  }
  catch (...) {
    cerr << "ERROR : UNKNOWN : ";
    cerr << "IoFile::~IoFile" << endl;
    throw;
  }
}


/******************************************************************************
 *****************************************************************************/
void IoFile::trim(string &_string, const string &_trimChars)
{
  int charPos;
  int trimCharPos;

  /** trim left **************************************************************/
  charPos = _string.find_first_not_of(_trimChars);
  if (charPos > 0)  _string.erase(0, charPos);
  /** trim right *************************************************************/
  charPos = _string.find_last_not_of(_trimChars);
  trimCharPos = _string.find_last_of(_trimChars);
  if ((charPos < trimCharPos) && (-1 < trimCharPos)) {
    _string.erase(charPos + 1, _string.size());
  }
}


/******************************************************************************
 *****************************************************************************/
int IoFile::compareNoCase(const string &_string1, const string &_string2)
{
  string string1 = _string1;
  string string2 = _string2;
  convert.toLowerCase(string1);
  convert.toLowerCase(string2);
  return(string1.compare(string2));
}


/******************************************************************************
******************************************************************************/
void IoFile::readLine(string &_line)
{
  char tmpLine[READ_MAX_CHARS_PER_LINE];

  try {
    /** check if operation failed ********************************************/
    if (p_file->bad() || p_file->fail()) {
      throw IoFileExcept(ERROR_COULD_NOT_READ_FILE);
    }

    /** get a new line from file *********************************************/
#ifdef BORLAND_CPP_50
    p_file->get(tmpLine, READ_MAX_CHARS_PER_LINE);
    p_file->get();
#else
    p_file->getline(tmpLine, READ_MAX_CHARS_PER_LINE);
#endif


    _line = tmpLine;
    trim(_line, WHITE_SPACE);

    /** update the line number counter ***************************************/
    lineNumber++;

    /** throw if the there are too many characters within that row ***********/
    if (PERMITTED_CHARS_PER_LINE < _line.size()) {
      throw IoFileExcept(ERROR_TOO_MANY_CHARS_PER_LINE);
    }
  }
  catch (IoFileExcept &_e) {
    if (_e.num == ERROR_COULD_NOT_READ_FILE) {
      cerr << "ERROR : COULD_NOT_READ_FILE : ";
      cerr << "lineNumber = " << lineNumber << " : ";
    }
    if (_e.num == ERROR_TOO_MANY_CHARS_PER_LINE) {
      cerr << "ERROR : TOO_MANY_CHARS_PER_LINE : ";
      cerr << "lineNumber = " << lineNumber << " : ";
    }
    cerr << "IoFile::readLine" << endl;
    throw;
  }
  catch (...) {
    cerr << "ERROR : UNKNOWN : ";
    cerr << "IoFile::readLine" << endl;
    throw;
  }
}


/******************************************************************************
 *****************************************************************************/
void IoFile::write(const string &_string)
{
  try {
    (*p_file) << _string;
  }
  catch (...) {
    cerr << "ERROR : UNKNOWN : ";
    cerr << "IoFile::write" << endl;
    throw;
  }
}


/******************************************************************************
 *****************************************************************************/
void IoFile::writeLine(const string &_line)
{
  try {
    write(_line);
    (*p_file) << endl;
    p_file->flush();
  }
  catch (...) {
    cerr << "ERROR : UNKNOWN : ";
    cerr << "IoFile::writeLine" << endl;
    throw;
  }
}


/******************************************************************************
 *****************************************************************************/
void IoFile::writeComment(const string &_string)
{
  try {
    (*p_file) << COMMENT_INDICATORS.substr(0, 1);
    if (0 < _string.size()) {
      (*p_file) << " " << _string;
    }
  }
  catch (...) {
    cerr << "ERROR : UNKNOWN : ";
    cerr << "IoFile::writeComment" << endl;
    throw;
  }
}


/******************************************************************************
 *****************************************************************************/
void IoFile::writeCommentLine(const string &_line)
{
  try {
    writeComment(_line);
    (*p_file) << endl;
    p_file->flush();
  }
  catch (...) {
    cerr << "ERROR : UNKNOWN : ";
    cerr << "IoFile::writeCommentLine" << endl;
    throw;
  }
}

/******************************************************************************
 *****************************************************************************/
void IoFile::writeSeparatorLine(const int _size)
{
  try {
    for (auto i = 0; i < _size; i++) {
      (*p_file) << COMMENT_INDICATORS[0];
    }
    (*p_file) << endl;
    p_file->flush();
  }
  catch (...) {
    cerr << "ERROR : UNKNOWN : ";
    cerr << "IoFile::write" << endl;
    throw;
  }
}


/******************************************************************************
 *****************************************************************************/
void IoFile::openFile(const string &_fileName, ios::openmode _openMode)
{
  try {
    /** ***/
    if (p_file) {
      closeFile();
    }

    /** */
    p_file = new fstream;

    /** try to open the file *************************************************/
    if (_fileName != "") {
      p_file->open(_fileName.c_str(), _openMode);
    }
    else if (fileName != "") {
      p_file->open(fileName.c_str(), _openMode);
    }
    else {
      throw IoFileExcept(ERROR_MISSING_FILE_NAME);
    }

    /** */
    if (! p_file->is_open()) {
      throw IoFileExcept(ERROR_COULD_NOT_OPEN_FILE);
    }

    /** reset the line number counter ****************************************/
    lineNumber = 0;
  }
  catch (IoFileExcept &_e) {
    if (_e.num == ERROR_MISSING_FILE_NAME) {
      cerr << "ERROR : MISSING_FILE_NAME : ";
    }
    if (_e.num == ERROR_COULD_NOT_OPEN_FILE) {
      cerr << "ERROR : COULD_NOT_OPEN_FILE : ";
      cerr << "fileName = " << '"' << fileName << '"' << " : ";
    }
    cerr << "IoFile::openFile" << endl;
    throw;
  }
  catch (...) {
    cerr << "ERROR : UNKNOWN : ";
    cerr << "IoFile::openFile" << endl;
    throw;
  }
}


/******************************************************************************
 *****************************************************************************/
void IoFile::openFileForRead(const string &_fileName)
{
  try {
    openFile(_fileName, ios::in);
  }
  catch (...) {
    cerr << "ERROR : UNKNOWN : ";
    cerr << "IoFile::openFileForRead" << endl;
    throw;
  }
}


/******************************************************************************
 *****************************************************************************/
void IoFile::openFileForWrite(const string &_fileName)
{
  try {
    openFile(_fileName, ios::out | ios::trunc);
  }
  catch (...) {
    cerr << "ERROR : UNKNOWN : ";
    cerr << "IoFile::openFileForWrite" << endl;
    throw;
  }
}


/******************************************************************************
 *****************************************************************************/
void IoFile::openFileForAppend(const string &_fileName)
{
  try {
    openFile(_fileName, ios::out | ios::app);
  }
  catch (...) {
    cerr << "ERROR : UNKNOWN : ";
    cerr << "IoFile::openFileForWrite" << endl;
    throw;
  }
}

/******************************************************************************
 *****************************************************************************/
void IoFile::closeFile()
{
  try {
    if (p_file) {
      if (p_file->is_open()) {
        p_file->close();
      }
      delete p_file;
      p_file = 0;
    }
  }
  catch (...) {
    cerr << "ERROR : UNKNOWN : ";
    cerr << "IoFile::closeFile" << endl;
    throw;
  }
}


/**############################################################################
# NON MEMBER DEFINITIONS
#############################################################################*/


/**############################################################################
# END OF FILE
#############################################################################*/
