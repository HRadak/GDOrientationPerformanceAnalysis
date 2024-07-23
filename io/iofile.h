/**############################################################################
#
#
# Copyright (C) 2021 by Christian Scheunert
#
# Email: christian.scheunert@tu-dresden.de
#
#############################################################################*/

#ifndef __IOFILE_H
#define __IOFILE_H

/**############################################################################
# INCLUDES
#############################################################################*/

#include "convert.h"

#include <fstream>
#include <iostream>
#include <new>
#include <string>
#include <valarray>


/**############################################################################
# NAMES
#############################################################################*/

using namespace std;


/**############################################################################
# DEFINES
#############################################################################*/

/******************************************************************************
 * @ref IoFileExcept specific constant numbers for errors thrown by
 * several functions of @ref IoFile.
 * @see
 *****************************************************************************/
#define ERROR_COULD_NOT_OPEN_FILE      1
#define ERROR_COULD_NOT_READ_FILE      2
#define ERROR_MISSING_FILE_NAME        3
#define ERROR_TOO_MANY_CHARS_PER_LINE  4


/******************************************************************************
 * @see
 *****************************************************************************/
const unsigned long PERMITTED_CHARS_PER_LINE = 2048;
const unsigned long READ_MAX_CHARS_PER_LINE = 2050;


/******************************************************************************
 * This constant contains the characters that the @ref IoFile.trim(...)
 * function removes from the head and tail of strings.
 * @see
 *****************************************************************************/
const string WHITE_SPACE = " \t\n\r";


/******************************************************************************
 * This constant contains the characters that we check for to determine if a
 * line is a comment or not. Note that the first character in this constant is
 * the one used when writing comments to disk (if the comment does not allready
 * contain an indicator).
 * @see
 *****************************************************************************/
const string COMMENT_INDICATORS = "#;";


/**############################################################################
# CLASS DECLARATIONS
#############################################################################*/

/******************************************************************************
 * A Exception class that refers to the @ref IoFile class.
 * @see
 *****************************************************************************/
class IoFileExcept {
public:
  /** the number of error ****************************************************/
  int num;
  /****************************************************************************
   * This construction function is used to set the number of exception.
   * @param _num Number of error that has been found.
   * @see
   ***************************************************************************/
  IoFileExcept(int _num);
};


/******************************************************************************
 *
 * @see
 *****************************************************************************/
class IoFile {
  /** pionter to the filestream **********************************************/
  fstream *p_file;

protected:
  /** an object for conversion of values to strings and vice versa ***********/
  Convert convert;
  /** a string used for reading from and writing to the stream ***************/
  string line;
  /** the filename to write to or read from **********************************/
  string fileName;
  /** counter for the line number ********************************************/
  unsigned long lineNumber;
  /****************************************************************************
   * Trims @ref _trimChars from both sides of a string.
   * @param _string The string to be trimed at the left and right side.
   * @param _trimChars A number of chars to be trimed.
   * @see
   ***************************************************************************/
  void trim(string &_string, const string &_trimChars);
  /****************************************************************************
   * This function compares the strings @ref _string1 and @ref _string2 after
   * translation into their lowercase versions.
   * @return An integer greater than, equal to, or less than 0, according as
   *         @ref _string1 is lexicographically greater than, equal to, or
   *         less than @ref _string2.
   * @param _string1 A reference to the first string to be compared.
   * @param _string2 A reference to the second string to be compared.
   * @see
   ***************************************************************************/
  int compareNoCase(const string &_string1, const string &_string2);
  /***************************************************************************
   * This function is used to read a single line from a file. A single line
   * should not include more than 'MAX_CHARS_PER_ROW' characters. However,
   * the actual number of values read per line is 'READ_CHARS_PER_ROW'
   * and then checked if 'MAX_CHARS_PER_ROW' has been exceeded.
   * @throws IoFileExcept(ERROR_TOO_MANY_CHARS_PER_LINE)
   * @see
   ***************************************************************************/
  void readLine(string &_line);
  /****************************************************************************
   * @see
   ***************************************************************************/
  void write(const string &_string);
  /****************************************************************************
   * @see
   ***************************************************************************/
  void writeLine(const string &_line = "");
  /****************************************************************************
   * @see
   ***************************************************************************/
  void writeComment(const string &_string = "");
  /****************************************************************************
   * @see
   ***************************************************************************/
  void writeCommentLine(const string &_line = "");
  /****************************************************************************
   * @see
   ***************************************************************************/
  void writeSeparatorLine(const int _size = 80);
  /****************************************************************************
   * @see
   ***************************************************************************/
  bool endOfFile();
  /****************************************************************************
   * @see
   ***************************************************************************/
	void openFile(const string &_fileName = "",
                ios::openmode _openMode = ios::in | ios::out);
  /****************************************************************************
   * @see
   ***************************************************************************/
  void openFileForRead(const string &_fileName = "");
  /****************************************************************************
   * @see
   ***************************************************************************/
  void openFileForWrite(const string &_fileName = "");
  /****************************************************************************
   * @see
   ***************************************************************************/
  void openFileForAppend(const string &_fileName = "");
  /****************************************************************************
   * @see
   ***************************************************************************/
  void closeFile();

public:
  /****************************************************************************
   * @see
   ***************************************************************************/
  IoFile(const string &_fileName = "");
  /****************************************************************************
   * @see
   ***************************************************************************/
  virtual ~IoFile();
  /****************************************************************************
   * @see
   ***************************************************************************/
  virtual bool setFileName(const string &_fileName);
};


/**############################################################################
# INLINE MEMBER DEFINITIONS
#############################################################################*/

/******************************************************************************
 *****************************************************************************/
inline bool IoFile::endOfFile()
{
  return(p_file->eof());
}


/******************************************************************************
 *****************************************************************************/
inline bool IoFile::setFileName(const string &_fileName)
{
  if ((_fileName != "") && (compareNoCase(_fileName, fileName) != 0)) {
    fileName = _fileName;
    return(true);
  }
  return(false);
}


/**############################################################################
# TEMPLATE MEMBER DEFINITIONS
#############################################################################*/


/**############################################################################
# NON MEMBER DECLARATIONS
#############################################################################*/


/**############################################################################
# END OF FILE
#############################################################################*/

#endif /* __IOFILE_H *********************************************************/
