/**############################################################################
#
#
# Copyright (C) 2021 by Christian Scheunert
#
# Email: christian.scheunert@tu-dresden.de
#
#############################################################################*/

#ifndef __IOSECTIONKEYFILE_H
#define __IOSECTIONKEYFILE_H

/**############################################################################
# INCLUDES
#############################################################################*/

#include "iofile.h"

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
 * @ref IoSectionKeyFileExcept specific constant numbers for errors thrown by
 * several functions of @ref IoSectionKeyFile.
 * @see
 *****************************************************************************/
#define ERROR_NOT_ENOUGH_OUTPUT_VALUES  1


/******************************************************************************
 * Characters framing a section name.
 * @see
 *****************************************************************************/
const string SECTION_PREFIX  = "[";
const string SECTION_POSTFIX = "]";


/**############################################################################
# CLASS DECLARATIONS
#############################################################################*/

/******************************************************************************
 * A Exception class that refers to the @ref IoSectionKeyFile class.
 * @author Christian M�ller <Christian.Mueller.Mailbox@web.de>
 * @version $Id: iosectionkeyfile.h,v 1.5 2003/06/20 11:45:09 muellerc Exp $
 * @see
 *****************************************************************************/
class IoSectionKeyFileExcept : public IoFileExcept  {
public:
  /****************************************************************************
   * This construction function is used to set the number of exception.
   * @param _num Number of error that has been found.
   * @see
   ***************************************************************************/
  IoSectionKeyFileExcept(int _num);
};


/******************************************************************************
 *
 * @author Christian M�ller <muellerc@ifn.et.tu-dresden.de>
 * @version $Id: iosectionkeyfile.h,v 1.5 2003/06/20 11:45:09 muellerc Exp $
 * @see
 *****************************************************************************/
class IoSectionKeyFile : public IoFile  {
protected:
  /** an array required for converting values into strings and vice versa ****/
  valarray<string> stringValues;
  /****************************************************************************
   * @param _section A reference to the name of the @ref Section.
   * @see
   ***************************************************************************/
  string convSectionName(const string &_section);
  /****************************************************************************
   * @see
   ***************************************************************************/
  virtual void setStringValues(const valarray<string> &_values,
                               const string &_key,
                               const string &_section) = 0;
  /****************************************************************************
   * @see
   ***************************************************************************/
  virtual void getStringValues(valarray<string> &_values, const string &_key,
                               const string &_section,
                               const unsigned long &_size) = 0;  
public:
  /****************************************************************************
   * @see
   ***************************************************************************/
  IoSectionKeyFile(const string &_fileName);
  /****************************************************************************
   * @see
   ***************************************************************************/
  virtual ~IoSectionKeyFile();
  /****************************************************************************
   * Given references to a value _value, a @ref Key _key and a @ref Section
   * _section, this function will attempt to locate the key within the given
   * section. If it finds the key it changes the keys value to the new value.
   * If it does not locate the key, it will create a new key with the proper
   * value and place it in the section requested. In case it does not locate
   * the section requested, it will create a new section.
   * @throws IoConfigFileExcept(ERROR_EMPTY_VALUE_STRING)
   * @param _value   A reference to the value to be stored in a data file.
   * @param _key     A reference to the name of the @ref Key.
   * @param _section A reference to the name of the @ref Section.
   * @see
   ***************************************************************************/
  template<class T> void setValue(const T &_value, const string &_key,
                                  const string &_section = "");
  /****************************************************************************
   * Given references to a value array _values, a @ref Key _key and a
   * @ref Section _section, this function will attempt to locate the key
   * within the given section. If it finds the key it changes the keys value
   * to the new value array. If it does not locate the key, it will create a
   * new key with the proper value array and place it in the section requested.
   * In case it does not locate the section requested, it will create a new
   * section.
   * @param _values  A reference to the value array to be stored in a data
   *                 file.
   * @param _key     A reference to the name of the @ref Key.
   * @param _section A reference to the name of the @ref Section.
   * @see
   ***************************************************************************/
  template<class T> void setValues(const valarray<T> &_values,
                                   const string &_key,
                                   const string &_section = "");
  /****************************************************************************
   * Tryes to return a string value specified by references to a
   * @ref Key _key and a @ref Section _section. The first parameter reference
   * @ref _value is actually used to return the result. This is because of
   * tracking the data type when a function @ref getValue(...) is called.
   * @throws IoConfigFileExcept(ERROR_KEY_DOES_NOT_EXIST)
   * @param _value   A reference to the variable to store the result in.
   * @param _key     A reference to the name of the @ref Key.
   * @param _section A reference to the name of the @ref Section.
   * @see
   ***************************************************************************/
  template<class T> void getValue(T &_value, const string &_key,
                                  const string &_section = "");
  /****************************************************************************
   * This function tryes to obtain a number of _size values from the key _key
   * within the section _section given. If it finds the key and section, and
   * the file includes the requested number of values, the values will be
   * stored in the array _values. If _values does not have the right size it
   * will be resized by this function to finally have a size of _size.
   * @param _values  A reference to an array to store the values in that
   *                 have been loaded from the file @ref fileName.
   * @param _key     A reference to the name of the key to read from.
   * @param _section A reference to the name of the section to read from.
   * @param _size    A reference to the number of values to read.
   * @see
   ***************************************************************************/
  template<class T> void getValues(valarray<T> &_values, const string &_key,
                                   const string &_section = "",
                                   const unsigned long &_size = 0);
};


/**############################################################################
# INLINE MEMBER DEFINITIONS
#############################################################################*/

/******************************************************************************
 *****************************************************************************/
inline string IoSectionKeyFile::convSectionName(const string &_section)
{
  return(SECTION_PREFIX + _section + SECTION_POSTFIX);
}


/**############################################################################
# TEMPLATE MEMBER DEFINITIONS
#############################################################################*/

/******************************************************************************
 *****************************************************************************/
template<class T>
void IoSectionKeyFile::setValue(const T &_value, const string &_key,
                                const string &_section)
{
  try {
    /** convert the values to strings ****************************************/
    stringValues.resize(1);
    stringValues[0] = convert.toString(_value);

    /** set the strings into the file ****************************************/
    setStringValues(stringValues, _key, _section);
  }
  catch (...) {
    cerr << "ERROR : UNKNOWN : ";
    cerr << "IoSectionKeyFile::setValue(template)" << endl;
    throw;
  }
}


/******************************************************************************
 *****************************************************************************/
template<class T>
void IoSectionKeyFile::setValues(const valarray<T> &_values,
                                 const string &_key, const string &_section)
{
  unsigned long i;

  try {
    /** check if we have got at least one value ******************************/
    if (_values.size() < 1) {
      throw IoSectionKeyFileExcept(ERROR_NOT_ENOUGH_OUTPUT_VALUES);
    }

    /** convert the values to strings ****************************************/
    stringValues.resize(_values.size());
    for (i = 0;  i < stringValues.size(); i++) {
       stringValues[i] = convert.toString(_values[i]);
    }

    /** set the strings into the file ****************************************/
    setStringValues(stringValues, _key, _section);
  }
  catch (IoSectionKeyFileExcept &_e) {
    if (_e.num == ERROR_NOT_ENOUGH_OUTPUT_VALUES) {
      cerr << "ERROR : NOT_ENOUGH_OUTPUT_VALUES : ";
    }
    cerr << "IoSectionKeyFile::setValues(template)" << endl;
    throw;
  }
  catch (...) {
    cerr << "ERROR : UNKNOWN : ";
    cerr << "IoSectionKeyFile::setValues(template)" << endl;
    throw;
  }
}


/******************************************************************************
 *****************************************************************************/
template<class T>
void IoSectionKeyFile::getValue(T &_value, const string &_key,
                                const string &_section)
{
  try {
    /** get the values as strings ********************************************/
    getStringValues(stringValues, _key, _section, 1);

    /** convert the values to the desired type *******************************/
    convert.toValue(_value, stringValues[0]);
  }
  catch (...) {
    cerr << "ERROR : UNKNOWN : ";
    cerr << "IoSectionKeyFile::getValue(template)" << endl;
    throw;
  }
}


/******************************************************************************
 *****************************************************************************/
template<class T>
void IoSectionKeyFile::getValues(valarray<T> &_values, const string &_key,
                                 const string &_section,
                                 const unsigned long &_size)
{
  unsigned long i;

  try {
    /** get the values as strings ********************************************/
    getStringValues(stringValues, _key, _section, _size);

    /** convert the values to the desired type *******************************/
    _values.resize(stringValues.size());
    for (i = 0;  i < _values.size(); i++) {
       convert.toValue(_values[i], stringValues[i]);
    }
  }
  catch (...) {
    cerr << "ERROR : UNKNOWN : ";
    cerr << "IoSectionKeyFile::getValues(template)" << endl;
    throw;
  }
}


/**############################################################################
# NON MEMBER DECLARATIONS
#############################################################################*/

 
/**############################################################################
# END OF FILE
#############################################################################*/

#endif /* __IOSECTIONKEYFILE_H ***********************************************/
