/**############################################################################
#
#
# Copyright (C) 2021 by Christian Scheunert
#
# Email: christian.scheunert@tu-dresden.de
#
#############################################################################*/

#ifndef __IODATAFILE_H
#define __IODATAFILE_H


/**############################################################################
# INCLUDES
#############################################################################*/

#include "iosectionkeyfile.h"

#include <iostream>
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
 * @ref IoDataFileExcept specific constant numbers for errors thrown by
 * several functions of @ref IoDataFile.
 * @see
 *****************************************************************************/
#define ERROR_SECTION_NOT_FOUND       1
#define ERROR_KEY_NOT_FOUND           2
#define ERROR_UNEXPECTED_END_OF_FILE  3
#define ERROR_NOT_ENOUGH_VALUES       4


/******************************************************************************
 * @see
 *****************************************************************************/
const string KEY_START_PREFIX = "<";
const string KEY_END_PREFIX   = "</";
const string KEY_POSTFIX      = ">";


/**############################################################################
# CLASS DECLARATIONS
#############################################################################*/

/******************************************************************************
 * A Exception class that refers to the @ref IoDataFile class.
 * @author Christian M�ller <Christian.Mueller.Mailbox@web.de>
 * @version $Id: iodatafile.h,v 1.8 2003/06/20 11:45:08 muellerc Exp $
 * @see
 *****************************************************************************/
class IoDataFileExcept : public IoFileExcept  {
public:
  /****************************************************************************
   * This construction function is used to set the number of exception.
   * @param _num Number of error that has been found.
   * @see
   ***************************************************************************/
  IoDataFileExcept(int _num);
};


/******************************************************************************
 * The purpose of this class is to provide a simple, full featured means to
 * easily read persistent data from a text file having sections [...] and
 * keys. Sections may not have duplicate keys, although two or more
 * sections can have the same key. A key is actually a piar of a start tag
 * <...> and an end tag <<x>/...> where the data is placed in between. The end
 * tag is required to check whether the requested number of values is available
 * or not. Note, in the data file one value has to follow the other, each on a
 * single line. Empty lines between the start and end tags are permitted.
 *
 * Note, independent from the file formats, there is a major difference of
 * this class @ref IoDataFile to the @ref IoConfigFile class. @ref IoDataFile
 * does not read the data file while construction but if one of the
 * @ref getValues functions is called. This is because the file might include
 * thousands of values within different sections and keys, which might
 * not be used and therefore not be loaded. Thus take care, calling one of the
 * @ref getValues functions will probably consume a lot of time.
 *
 * An example of such a data file might look like this:
 * <Note, the braced x in the tags is only required for correct kdoc output>
 *
 * @li ------------------------------------------------------------------------
 * @li #
 * @li # Comments are permitted anywhere but between the key's start and
 * @li # end tag.
 * @li #
 * @li ...
 * @li [SIGNAL_SAMPLES_FOR_BINARY_MODULATION]
 * @li ...
 * @li <<x>SIGNAL_0>
 * @li 3.365763724323
 * @li 3.409806987964
 * @li ...
 * @li 0.768456043753
 * @li 0.703498729479
 * @li <<x>/SIGNAL_0>
 * @li ...
 * @li <<x>SIGNAL_1>
 * @li -3.365763724323
 * @li -3.409806987964
 * @li ...
 * @li -0.768456043753
 * @li -0.703498729479
 * @li <<x>/SIGNAL_1>
 * @li ...
 * @li [CODE_SYMBOLS]
 * @li 
 * @li <<x>PN_CODE_GF(31)>
 * @li 17
 * @li 3
 * @li ...
 * @li 29
 * @li 9
 * @li 11
 * @li <<x>/PN_CODE_GF(31)>
 * @li ...
 * @li <<x>GOLD_CODE_GF(2)>
 * @li 1
 * @li 0
 * @li ...
 * @li 1
 * @li 1
 * @li 0
 * @li <<x>/GOLD_CODE_GF(2)>
 * @li ------------------------------------------------------------------------
 *
 * @author Christian M�ller <Christian.Mueller.Mailbox@web.de>
 * @version $Id: iodatafile.h,v 1.8 2003/06/20 11:45:08 muellerc Exp $
 * @see
 *****************************************************************************/
class IoDataFile : public IoSectionKeyFile {
  /****************************************************************************
   * @see
   ***************************************************************************/
  string convStartKeyName(const string &_key);
  /****************************************************************************
   * @see
   ***************************************************************************/
  string convEndKeyName(const string &_key);
  /****************************************************************************
   * This function is used to find a section within the data file. Any section
   * within the data file to read has actually to have the format
   * "SECTION_PREFIX + _section + SECTION_POSTFIX".
   * @throws IoDataFileExcept(ERROR_SECTION_NOT_FOUND)
   * @param _section The section name to find.
   * @see
   ***************************************************************************/
  void findSection(const string &_section);
  /****************************************************************************
   * This function is used to find a key within the section and data file
   * specified. Any key is actually a piar of a start tag <...> and an end
   * tag <<x>/...> where the data is placed in between. The tags within the
   * data file to read have actually to have the format
   * "KEY_START_PREFIX + pattern + KEY_POSTFIX" respectively
   * "KEY_END_PREFIX + pattern + KEY_POSTFIX".
   * @throws IoDataFileExcept(ERROR_KEY_NOT_FOUND)
   * @param _section The name of section to find.
   * @param _key     The name of key to find.
   * @see
   ***************************************************************************/
  void findKey(const string &_key, const string &_section);
  /****************************************************************************
   * @see
   ***************************************************************************/
  virtual void setStringValues(const valarray<string> &_values,
                               const string &_key, const string &_section);
  /****************************************************************************
   * @see
   ***************************************************************************/
  virtual void getStringValues(valarray<string> &_values, const string &_key,
                               const string &_section,
                               const unsigned long &_size);
public:
  /****************************************************************************
   * @see
   ***************************************************************************/
  IoDataFile(const string &_fileName = "");
  /****************************************************************************
   * @see
   ***************************************************************************/
  virtual ~IoDataFile();
};


/**############################################################################
# INLINE MEMBER DEFINITIONS
#############################################################################*/

/******************************************************************************
 *****************************************************************************/
inline string IoDataFile::convStartKeyName(const string &_key)
{
  return(KEY_START_PREFIX + _key + KEY_POSTFIX);
}


/******************************************************************************
 *****************************************************************************/
inline string IoDataFile::convEndKeyName(const string &_key)
{
  return(KEY_END_PREFIX + _key + KEY_POSTFIX);
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

#endif /* __IODATAFILE_H *****************************************************/
