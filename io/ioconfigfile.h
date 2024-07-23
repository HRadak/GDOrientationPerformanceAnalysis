/**############################################################################
#
#
# Copyright (C) 2021 by Christian Scheunert
#
# Email: christian.scheunert@tu-dresden.de
#
#############################################################################*/

#ifndef __IOCONFIGFILE_H
#define __IOCONFIGFILE_H

/**############################################################################
# INCLUDES
#############################################################################*/

#include "iosectionkeyfile.h"

#include <iostream>
#include <string>
#include <valarray>
#include <vector>


/**############################################################################
# NAMES
#############################################################################*/

using namespace std;


/**############################################################################
# DEFINES
#############################################################################*/

/******************************************************************************
 * @ref IoConfigFileExcept specific constant numbers for errors thrown by
 * several functions of @ref IoConfigFile.
 * @see
 *****************************************************************************/
#define ERROR_COULD_NOT_CREATE_SECTION            1
#define ERROR_COULD_NOT_CREATE_KEY                2
#define ERROR_SECTION_ALREADY_EXISTS              3
#define ERROR_KEY_ALREADY_EXISTS                  4
#define ERROR_SECTION_DOES_NOT_EXIST              5
#define ERROR_KEY_DOES_NOT_EXIST                  6
#define ERROR_MISSING_EQUAL_INDICATOR             7
#define ERROR_MISSING_KEY_NAME                    8
#define ERROR_COULD_NOT_FIND_MULTI_VALUE_POSTFIX  9
#define ERROR_WRONG_NUMBER_OF_VALUES              10
#define ERROR_EMPTY_VALUE_STRING                  11


/******************************************************************************
 * This constant contains the characters that we check against to determine if
 * a line contains an assignment (key = value). Note that changing these from
 * their defaults ("=:") will affect the ability of IoConfigFile to read/write
 * to MS-Windows *.ini like configuration files.  Also, note that the first
 * character in this constant is the one that is used when writing the values
 * to the file. (EQUAL_INDICATORS[0])
 * @see
 *****************************************************************************/
const string EQUAL_INDICATORS = "=:";


/******************************************************************************
 * These constant values specify the format of a value list. The format will
 * be:
 * @li MULTI_VALUE_PREFIX value1 MULTI_VALUE_SEPARATOR
 *                           ... MULTI_VALUE_SEPARATOR
 *                        valueN MULTI_VALUE_POSTFIX
 * @see
 *****************************************************************************/
const string MULTI_VALUE_PREFIX    = "{";
const string MULTI_VALUE_SEPARATOR = ",";
const string MULTI_VALUE_POSTFIX   = "}";


/**############################################################################
# CLASS DECLARATIONS
#############################################################################*/

/******************************************************************************
 * A Exception class that refers to the @ref IoConfigFile class.
 * @author Christian M�ller <Christian.Mueller.Mailbox@web.de>
 * @version $Id: ioconfigfile.h,v 1.12 2004/03/01 11:45:58 muellerc Exp $
 * @see
 *****************************************************************************/
class IoConfigFileExcept : public IoFileExcept  {
public:
  /****************************************************************************
   * This construction function is used to set the number of exception.
   * @param _num Number of error that has been found.
   * @see
   ***************************************************************************/
  IoConfigFileExcept(int _num);
};


/******************************************************************************
 * The purpose of this class is to provide a simple, full featured means to
 * easily read/write persistent 'key = value' pairs from/to a standard
 * MS-Windows *.ini like configuration file, seperated by independant sections.
 * Sections may not have duplicate keys, although two or more sections can have
 * the same key. Simple support for comments is included. Each key, and each
 * section may have it's own multiline comment. In case there is an empty line
 * found between a comment and a section or key respectively, the comment will
 * be thrown away.
 *
 * An example of such a config file might look like this:
 *
 * @li ------------------------------------------------------------------------
 * @li empty section parameter = 635757
 * @li another_parameter = /path/to/my/home
 * @li ...
 * @li [SectionOfUserSettings]
 * @li Name = xyz
 * @li Date of Birth = 01/01/01
 * @li ...
 * @li #
 * @li # Settings unique to this server
 * @li #
 * @li [ServerSettingsSection]
 * @li Port = 1200
 * @li IP_Address = 127.0.0.1
 * @li MachineName = SNOOPY
 * @li ------------------------------------------------------------------------
 *
 * @author Christian M�ller <Christian.Mueller.Mailbox@web.de>
 * @version $Id: ioconfigfile.h,v 1.12 2004/03/01 11:45:58 muellerc Exp $
 * @see
 *****************************************************************************/
class IoConfigFile : public IoSectionKeyFile {
  /****************************************************************************
   * The Key class stores the definition of a key. A key is a named identifier
   * that is associated with a value. It may or may not have a comment.
   * All comments must precede the key on the line in the config file.
   * @author Christian M�ller <Christian.Mueller.Mailbox@web.de>
   * @version $Id: ioconfigfile.h,v 1.12 2004/03/01 11:45:58 muellerc Exp $
   * @see
   ***************************************************************************/
  class Key  {
  public:
    string name;
    string value;
    unsigned long emptyLines;
    vector<string> comments;
  };
  /****************************************************************************
   * This structure stores the definition of a section. A section contains any
   * number of keys (@ref Key), and may or may not have a comment. Like keys,
   * all comments must precede the section on the line in the config file.
   * @author Christian M�ller <Christian.Mueller.Mailbox@web.de>
   * @version $Id: ioconfigfile.h,v 1.12 2004/03/01 11:45:58 muellerc Exp $
   * @see
   ***************************************************************************/
  class Section {
  public:
    string name;
    unsigned long emptyLines;
    vector<string> comments;
    vector<Key> keyList;
  };
  /** tracks whether or not data has changed *********************************/
  bool changedData;
  /** a value that controls whether keys and section should be sorted or not */
  bool sortContent;
  /** list of sections *******************************************************/
  vector<Section> sectionList;
  /****************************************************************************
   * Given a section name, this function looks up the section within the list.
   * @return A pointer to that section if found, otherwise NULL.
   * @param _section A reference to the name of the @ref Section.
   * @see
   ***************************************************************************/
  Section *getSection(const string &_section);
  /****************************************************************************
   * Given a @ref Key _key and a @ref Section _section, this function looks up
   * the key.
   * @return A pointer to that key if found, otherwise NULL.
   * @param _key     A reference to the name of the @ref Key.
   * @param _section A reference to the name of the @ref Section.
   * @see
   ***************************************************************************/
  Key *getKey(const string &_key, const string &_section);
  /****************************************************************************
   * Given a @ref Section _section, this function creates a section specified.
   * @return A pointer to the section created, otherwise NULL.
   * @throws IoConfigFileExcept(ERROR_SECTION_ALREADY_EXISTS)
   *         IoConfigFileExcept(ERROR_CREATE_SECTION)
   * @param _section A reference to the name of the @ref Section.
   * @see
   ***************************************************************************/
  Section *createSection(const string &_section);
  /****************************************************************************
   * Given a @ref Key _key and a @ref Section _section, this function creates
   * the key within the section specified. In case the section does not exist,
   * it will be created as well.
   * @return A pointer to the key created, otherwise NULL.
   * @throws IoConfigFileExcept(ERROR_MISSING_KEY_NAME)
   *         IoConfigFileExcept(ERROR_KEY_ALREADY_EXISTS)
   *         IoConfigFileExcept(ERROR_CREATE_KEY)
   * @param _key     A reference to the name of the @ref Key.
   * @param _section A reference to the name of the @ref Section.
   * @see
   ***************************************************************************/
  Key *createKey(const string &_key, const string &_section);
  /****************************************************************************
   * MS-Windows *.ini like configuration files use a simple 'key = value'
   * paradigm to store its content. Given a 'key = value' string this function
   * tryes to extract the @ref Key.
   * @return A string including the @ref Key.
   * @throws IoConfigFileExcept(ERROR_MISSING_EQUAL_INDICATOR)
   * @param _string A reference to the 'key = value' string.
   * @see
   ***************************************************************************/
  string extractKey(const string &_string);
  /****************************************************************************
   * MS-Windows *.ini like configuration files use a simple 'key = value'
   * paradigm to store its content. Given a 'key = value' string this function
   * tryes to extract the value.
   * @return A string including the value.
   * @throws IoConfigFileExcept(ERROR_MISSING_EQUAL_INDICATOR)
   * @param _string A reference to the 'key = value' string.
   * @see
   ***************************************************************************/
  string extractValue(const string &_string);
  /****************************************************************************
   * This function is used to separate multiple values from a single value
   * string. If no @ref MULTI_VALUE_PREFIX is found the given input
   * @ref _valueString is assumed to be a single value. A multi-value set has
   * to have the following format, where white spaces in between are permitted.
   * @li ----------------------------------------------------------------------
   * @li key = MULTI_VALUE_PREFIX +
   * @li value_1 + MULTI_VALUE_SEPARATOR +
   * @li value_2 + MULTI_VALUE_SEPARATOR +
   * @li ...
   * @li value_N + MULTI_VALUE_POSTFIX
   * @li ----------------------------------------------------------------------
   * @throws IoConfigFileExcept(ERROR_MULTI_VALUE_POSTFIX_NOT_FOUND)
   * @param _values      A valarray to store the separated string values in.
   * @param _valueString A string that includes multiple values.
   * @see
   ***************************************************************************/
  void separateValues(valarray<string> &_values, const string &_valueString);
  /****************************************************************************
   * Given a @ref Section _section, this function will set the section comment
   * _comment to the section specified.
   * @throws IoConfigFileExcept(ERROR_SECTION_DOES_NOT_EXIST)
   * @param _comment A reference to the comment to be stored in a data file.
   * @param _section A reference to the name of the @ref Section.
   * @see
   ***************************************************************************/
  void setSectionComments(const vector<string> &_comments,
                          const unsigned long &_emptyLines,
                          const string &_section);
  /****************************************************************************
   * Given a @ref Key _key and a @ref Section _section, this function will set
   * the key comment _comment to the key within the section specified.
   * @throws IoConfigFileExcept(ERROR_KEY_DOES_NOT_EXIST)
   * @param _comment A reference to the comment to be stored in a data file.
   * @param _key     A reference to the name of the @ref Key.
   * @param _section A reference to the name of the @ref Section.
   * @see
   ***************************************************************************/
  void setKeyComments(const vector<string> &_comments,
                      const unsigned long &_emptyLines,
                      const string &_key, const string &_section = "");
  /****************************************************************************
   * Parses a string into a proper comment 'COMMENT_INDICATORS[0] _comment'.
   * @see
   ***************************************************************************/
  string &formatComment(string &_comment);
  /****************************************************************************
   * @see
   ***************************************************************************/
  virtual void setStringValues(const valarray<string> &_values,
                               const string &_key,
                               const string &_section);
  /****************************************************************************
   * @param _key     A reference to the name of the @ref Key.
   * @param _section A reference to the name of the @ref Section.
   * @see
   ***************************************************************************/
  virtual void getStringValues(valarray<string> &_values, const string &_key,
                               const string &_section,
                               const unsigned long &_size);
public:
  /****************************************************************************
   * In case the parameter reference @ref _fileName is different from "", this
   * contstructor will load the file specified and therefore call the function
   * @ref loadFile(...).
   * @param _fileName    Name of the MS-Windows *.ini like configuration file
   *                     to read data from.
   * @param _sortContent Decides whether the content read from file should be
   *                     sorted or not. If true the @ref save(...) function
   *                     will write the sorted content into a file.
   * @see
   ***************************************************************************/
  IoConfigFile(const string &_fileName = "", const bool &_sortContent = true);
  /****************************************************************************
   * Calls the function @ref saveFile(...) if any values have changed since
   * the last save.
   * @see
   ***************************************************************************/
  virtual ~IoConfigFile();
  /****************************************************************************
   * Attempts to load in the text file specified by _fileName. If successful
   * it will populate the @ref Section list with the 'key = value' pairs found
   * in the file. Note that comments are saved so that they can be rewritten
   * to the file later.
   * @param _fileName    Name of the MS-Windows *.ini like configuration file
   *                     to read data from.
   * @param _sortContent Decides whether the content read from file should be
   *                     sorted or not. If true the @ref save(...) function
   *                     will write the sorted content into a file.
   * @see
   ***************************************************************************/
  void loadFile(const string &_fileName, const bool &_sortContent = true);
  /****************************************************************************
   * Attempts to save the @ref Section list and their @ref Key list to the
   * file. Note that if loadFile(...) was never called (an @ref IoConfigFile
   * object was created with _fileName = ""), then you must set the fileName
   * variable using @ref setFileName(...) before calling saveFile().
   * @throws IoConfigFileExcept(ERROR_MISSING_FILE_NAME)
   *         IoConfigFileExcept(ERROR_OPEN_FILE)
   * @param _fileName Name of the MS-Windows *.ini like configuration file
   *                  to read data from.
   * @see
   ***************************************************************************/
  void saveFile(const string &_fileName = "");
  /****************************************************************************
   * Set's the fileName member variable.
   * @param _fileName    Name of the MS-Windows *.ini like configuration file
   *                     to read data from.
   * @see
   ***************************************************************************/
  virtual bool setFileName(const string &_fileName);
  /****************************************************************************
   * Resets the member variables to their defaults.
   * @see
   ***************************************************************************/
  void clear();
  /****************************************************************************
   * Given a @ref Section _section, this function looks up the section.
   * @return True if the section was found, otherwise false.
   * @param _key     A reference to the name of the @ref Key.
   * @param _section A reference to the name of the @ref Section.
   * @see
   ***************************************************************************/
  bool sectionExists(const string &_section);
  /****************************************************************************
   * Given a @ref Key _key and a @ref Section _section, this function looks up
   * the key.
   * @return True if the key was found, otherwise false.
   * @param _key     A reference to the name of the @ref Key.
   * @param _section A reference to the name of the @ref Section.
   * @see
   ***************************************************************************/
  bool keyExists(const string &_key, const string &_section);
  /****************************************************************************
   * Given a @ref Section _section, this function deletes the section
   * specified.
   * @throws IoConfigFileExcept(ERROR_SECTION_DOES_NOT_EXIST)
   * @return A pointer to the section created, otherwise NULL.
   * @param _section A reference to the name of the @ref Section.
   * @see
   ***************************************************************************/
  void deleteSection(const string &_section);
  /****************************************************************************
   * Given a @ref Key _key and a @ref Section _section, this function deletes
   * the key from the section specified.
   * @throws IoConfigFileExcept(ERROR_SECTION_DOES_NOT_EXIST)
   *         IoConfigFileExcept(ERROR_KEY_DOES_NOT_EXIST)
   * @param _key     A reference to the name of the @ref Key.
   * @param _section A reference to the name of the @ref Section.
   * @see
   ***************************************************************************/
  void deleteKey(const string &_key, const string &_section);
  /****************************************************************************
   * @param _section A reference to the name of the @ref Section.
   * @see
   ***************************************************************************/
  void deleteSectionComments(const string &_section);
  /****************************************************************************
   * @param _key     A reference to the name of the @ref Key.
   * @param _section A reference to the name of the @ref Section.
   * @see
   ***************************************************************************/
  void deleteKeyComments(const string &_key, const string &_section);
  /****************************************************************************
   * @param _section A reference to the name of the @ref Section.
   * @see
   ***************************************************************************/
  void addSectionComment(const string &_comment, const string &_section);
  /****************************************************************************
   * @param _key     A reference to the name of the @ref Key.
   * @param _section A reference to the name of the @ref Section.
   * @see
   ***************************************************************************/
  void addKeyComment(const string &_comment, const string &_key,
                     const string &_section);
};


/**############################################################################
# INLINE MEMBER DEFINITIONS
#############################################################################*/

/******************************************************************************
 *****************************************************************************/
inline bool IoConfigFile::setFileName(const string &_fileName)
{
  if (IoFile::setFileName(_fileName)) {
    changedData = true;
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

#endif /* __IOCONFIGFILE_H ***************************************************/
