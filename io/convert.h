/**############################################################################
#
#
# Copyright (C) 2021 by Christian Scheunert
#
# Email: christian.scheunert@tu-dresden.de
#
#############################################################################*/

#ifndef __CONVERT_H
#define __CONVERT_H

/**############################################################################
 # INCLUDES
 #############################################################################*/

#include <cctype>
#include <iostream>
#include <string>
#include <sstream>

/**############################################################################
 # NAMES
 #############################################################################*/

using namespace std;

/**############################################################################
 # DEFINES
 #############################################################################*/

/******************************************************************************
 * @ref ConvertExcept specific constant numbers for errors thrown by
 * several functions of @ref Convert.
 * @see
 *****************************************************************************/
#define ERROR_NON_VALID_CHARS_IN_STRING  1
#define ERROR_CAN_NOT_CONVERT_TO_VALUE   2
#define ERROR_CAN_NOT_CONVERT_TO_STRING  3

/******************************************************************************
 * A number of permitted chars for different number formats.
 * @see
 *****************************************************************************/
#define PERMITTED_FLOAT_CHARS     "+-1234567890.eE"
#define PERMITTED_SIGNED_CHARS    "+-1234567890"
#define PERMITTED_UNSIGNED_CHARS  "+1234567890"

/**############################################################################
 # CLASS DECLARATIONS
 #############################################################################*/

/******************************************************************************
 * A Exception class that refers to the @ref Convert class.
 * @see
 *****************************************************************************/
class ConvertExcept {
public:
	/** the number of error ****************************************************/
	int num;
	/****************************************************************************
	 * This construction function is used to set the number of exception.
	 * @param _num Number of error that has been found.
	 * @see
	 ***************************************************************************/
	ConvertExcept(int _num);
};

/******************************************************************************
 * The @ref Convert class intends to convert values into strings and vice
 * versa.
 * @see
 *****************************************************************************/
class Convert {
	/** a string stream used for conversion from string to value ***************/
	istringstream inputStream;
	/** a string stream used for conversion from value to string ***************/
	ostringstream outputStream;
	/****************************************************************************
	 * Converts a value into a string.
	 * @param  _value The value to be converted into a string.
	 * @return A string corresponding to the value given with the parameter
	 *         @ref _value.
	 * @see
	 ***************************************************************************/
	template<class T> string valueToString(const T &_value);
	/****************************************************************************
	 * Tryes to convert a string into a value.
	 * @throws ConvertExcept(ERROR_CAN_NOT_CONVERT_TO_VALUE)
	 * @param _string The string to be converted into a value.
	 * @param _value  The variable to store the converted value in.
	 * @see
	 ***************************************************************************/
	template<class T> void stringToValue(T &_value, const string &_string);
public:
	/****************************************************************************
	 * The construction function of the 'Convert' class is used to set the flags
	 * for the conversion string-streams.
	 * @see
	 ***************************************************************************/
	Convert();
	/****************************************************************************
	 * The destruction function is just doing nothing.
	 * @see
	 ***************************************************************************/
	~Convert();
	/****************************************************************************
	 * @see
	 ***************************************************************************/
	ios::fmtflags setFormat(const ios::fmtflags &_format);
	/****************************************************************************
	 * Sets the width of the value string output.
	 * @return The previous format.
	 * @param  _width Specifies the total number digits to be written into the
	 *               string.
	 * @see
	 ***************************************************************************/
	streamsize setWidth(const streamsize _width = 0);
	/****************************************************************************
	 * Sets the precision of the double value string output.
	 * @return The previous precision.
	 * @param  _precision For values of floating type this sets the number of
	 *                   digits behind the decimal point. Default is 6.
	 * @see
	 ***************************************************************************/
	streamsize setPrecision(const streamsize _precision = 0);
	/****************************************************************************
	 * Coverts a string into its lower case version.
	 * @param  _string The string to be converted into lower case.
	 * @return A reference to the string given with the parameter @ref _string.
	 * @see
	 ***************************************************************************/
	string& toLowerCase(string &_string);
	/****************************************************************************
	 * Coverts a string into its upper case version.
	 * @param  _string The string to be converted into upper case.
	 * @return A reference to the string given with the parameter @ref _string.
	 * @see
	 ***************************************************************************/
	string& toUpperCase(string &_string);
	/****************************************************************************
	 * Converts a string into a string. Since the implementation is special
	 * for strings this function is actually only required for compatibility.
	 * @param  _value The value to be converted into a string.
	 * @return A string corresponding to the value given with the parameter
	 *         @ref _value.
	 * @see
	 ***************************************************************************/
	string toString(const string &_value);
	/****************************************************************************
	 * Converts a string into a bool value.
	 * @param  _value The bool value to be converted into a string.
	 * @return A string corresponding to the bool value given with
	 *         the parameter @ref _value.
	 * @see
	 ***************************************************************************/
	string toString(const bool &_value);
	/****************************************************************************
	 * Converts a string into a float value.
	 * @param  _value The float value to be converted into a string.
	 * @return A string corresponding to the float value given with
	 *         the parameter @ref _value.
	 * @see
	 ***************************************************************************/
	string toString(const float &_value);
	/****************************************************************************
	 * Converts a string into a double value.
	 * @param  _value The double value to be converted into a string.
	 * @return A string corresponding to the double value given with
	 *         the parameter @ref _value.
	 * @see
	 ***************************************************************************/
	string toString(const double &_value);
	/****************************************************************************
	 * Converts a string into a long double value.
	 * @param  _value The long double value to be converted into a string.
	 * @return A string corresponding to the long double value given with
	 *         the parameter @ref _value.
	 * @see
	 ***************************************************************************/
	string toString(const long double &_value);
	/****************************************************************************
	 * Converts a string into a char value.
	 * @param  _value The char value to be converted into a string.
	 * @return A string corresponding to the char value given with
	 *         the parameter @ref _value.
	 * @see
	 ***************************************************************************/
	string toString(const char &_value);
	/****************************************************************************
	 * Converts a string into a short value.
	 * @param  _value The short value to be converted into a string.
	 * @return A string corresponding to the short value given with
	 *         the parameter @ref _value.
	 * @see
	 ***************************************************************************/
	string toString(const short &_value);
	/****************************************************************************
	 * Converts a string into a int value.
	 * @param  _value The int value to be converted into a string.
	 * @return A string corresponding to the int value given with
	 *         the parameter @ref _value.
	 * @see
	 ***************************************************************************/
	string toString(const int &_value);
	/****************************************************************************
	 * Converts a string into a long value.
	 * @param  _value The long value to be converted into a string.
	 * @return A string corresponding to the long value given with
	 *         the parameter @ref _value.
	 * @see
	 ***************************************************************************/
	string toString(const long &_value);
	/****************************************************************************
	 * Converts a string into a unsigned char value.
	 * @param  _value The unsigned char value to be converted into a string.
	 * @return A string corresponding to the unsigned char value given with
	 *         the parameter @ref _value.
	 * @see
	 ***************************************************************************/
	string toString(const unsigned char &_value);
	/****************************************************************************
	 * Converts a string into a unsigned short value.
	 * @param  _value The unsigned short value to be converted into a string.
	 * @return A string corresponding to the unsigned short value given with
	 *         the parameter @ref _value.
	 * @see
	 ***************************************************************************/
	string toString(const unsigned short &_value);
	/****************************************************************************
	 * Converts a string into a unsigned int value.
	 * @param  _value The unsigned int value to be converted into a string.
	 * @return A string corresponding to the unsigned int value given with
	 *         the parameter @ref _value.
	 * @see
	 ***************************************************************************/
	string toString(const unsigned int &_value);
	/****************************************************************************
	 * Converts a string into a unsigned long value.
	 * @param  _value The unsigned long value to be converted into a string.
	 * @return A string corresponding to the unsigned long value given with
	 *         the parameter @ref _value.
	 * @see
	 ***************************************************************************/
	string toString(const unsigned long &_value);
	/****************************************************************************
	 * Tryes to convert a string into a string value.
	 * @param  _string The string to be converted into a string value.
	 * @return The converted string value.
	 * @throws ConvertExcept(ERROR_CAN_NOT_CONVERT_TO_BOOL)
	 * @see
	 ***************************************************************************/
	void toValue(string &_value, const string &_string);
	/****************************************************************************
	 * Tryes to convert a string into a bool value.
	 * @param  _string The string to be converted into a bool value.
	 * @return The converted bool value.
	 * @see
	 ***************************************************************************/
	void toValue(bool &_value, const string &_string);
	/****************************************************************************
	 * Tryes to convert a string into a float value.
	 * @param  _string The string to be converted into a float value.
	 * @return The converted float value.
	 * @throws ConvertExcept(ERROR_CAN_NOT_CONVERT_TO_VALUE)
	 * @see
	 ***************************************************************************/
	void toValue(float &_value, const string &_string);
	/****************************************************************************
	 * Tryes to convert a string into a double value.
	 * @param  _string The string to be converted into a double value.
	 * @return The converted double value.
	 * @see
	 ***************************************************************************/
	void toValue(double &_value, const string &_string);
	/****************************************************************************
	 * Tryes to convert a string into a long double value.
	 * @param  _string The string to be converted into a long double value.
	 * @return The converted long double value.
	 * @see
	 ***************************************************************************/
	void toValue(long double &_value, const string &_string);
	/****************************************************************************
	 * Tryes to convert a string into a char value.
	 * @param  _string The string to be converted into a char value.
	 * @return The converted char value.
	 * @see
	 ***************************************************************************/
	void toValue(char &_value, const string &_string);
	/****************************************************************************
	 * Tryes to convert a string into a short value.
	 * @param  _string The string to be converted into a short value.
	 * @return The converted short value.
	 * @see
	 ***************************************************************************/
	void toValue(short &_value, const string &_string);
	/****************************************************************************
	 * Tryes to convert a string into a int value.
	 * @param  _string The string to be converted into a int value.
	 * @return The converted int value.
	 * @see
	 ***************************************************************************/
	void toValue(int &_value, const string &_string);
	/****************************************************************************
	 * Tryes to convert a string into a long value.
	 * @param  _string The string to be converted into a long value.
	 * @return The converted long value.
	 * @see
	 ***************************************************************************/
	void toValue(long &_value, const string &_string);
	/****************************************************************************
	 * Tryes to convert a string into a unsigned char value.
	 * @param  _string The string to be converted into a unsigned char value.
	 * @return The converted unsigned char value.
	 * @see
	 ***************************************************************************/
	void toValue(unsigned char &_value, const string &_string);
	/****************************************************************************
	 * Tryes to convert a string into a unsigned short value.
	 * @param  _string The string to be converted into a unsigned short value.
	 * @return The converted unsigned short value.
	 * @see
	 ***************************************************************************/
	void toValue(unsigned short &_value, const string &_string);
	/****************************************************************************
	 * Tryes to convert a string into a unsigned int value.
	 * @param  _string The string to be converted into a unsigned int value.
	 * @return The converted unsigned int value.
	 * @see
	 ***************************************************************************/
	void toValue(unsigned int &_value, const string &_string);
	/****************************************************************************
	 * Tryes to convert a string into a unsigned long value.
	 * @param  _string The string to be converted into a unsigned long value.
	 * @return The converted unsigned long value.
	 * @see
	 ***************************************************************************/
	void toValue(unsigned long &_value, const string &_string);
};

/**############################################################################
 # INLINE MEMBER DEFINITIONS
 #############################################################################*/

/******************************************************************************
 *****************************************************************************/
inline ios::fmtflags Convert::setFormat(const ios::fmtflags &_format) {
	return (outputStream.flags(_format));
}

/******************************************************************************
 *****************************************************************************/
inline streamsize Convert::setWidth(const streamsize _width) {
	if (_width != 0) {
		return (outputStream.width(_width));
	} else {
		return (outputStream.width());
	}
}

/******************************************************************************
 *****************************************************************************/
inline streamsize Convert::setPrecision(const streamsize _precision) {
	if (_precision != 0) {
		return (outputStream.precision(_precision));
	} else {
		return (outputStream.precision());
	}
}

/******************************************************************************
 *****************************************************************************/
inline string& Convert::toLowerCase(string &_string) {
	unsigned long i;

	for (i = 0; i < _string.size(); i++) {
		_string[i] = (char) tolower((int) _string[i]);
	}
	return (_string);
}

/******************************************************************************
 *****************************************************************************/
inline string& Convert::toUpperCase(string &_string) {
	unsigned long i;

	for (i = 0; i < _string.size(); i++) {
		_string[i] = (char) toupper((int) _string[i]);
	}
	return (_string);
}

/**############################################################################
 # TEMPLATE MEMBER DEFINITIONS
 #############################################################################*/

/******************************************************************************
 *****************************************************************************/
template<class T>
string Convert::valueToString(const T &_value) {
	string tmpStr;

	try {
		outputStream.clear();
		outputStream.str("");
		outputStream << _value;
		tmpStr = outputStream.str();
		if (outputStream.bad() || outputStream.fail()) {
			throw ConvertExcept(ERROR_CAN_NOT_CONVERT_TO_STRING);
		}
		return (tmpStr);
	} catch (ConvertExcept &_e) {
		if (_e.num == ERROR_CAN_NOT_CONVERT_TO_STRING) {
			cerr << "ERROR : CAN_NOT_CONVERT_TO_STRING : ";
		}
		cerr << "valueToString(template)" << endl;
		throw;
	} catch (...) {
		cerr << "ERROR : UNKNOWN : ";
		cerr << "valueToString(template)" << endl;
		throw;
	}
}

/******************************************************************************
 *****************************************************************************/
template<class T>
void Convert::stringToValue(T &_value, const string &_string) {
	try {
		/** convert the string into a value **************************************/
		inputStream.clear();
		inputStream.str(_string);
		inputStream >> _value;
		if (inputStream.bad() || inputStream.fail()) {
			throw ConvertExcept(ERROR_CAN_NOT_CONVERT_TO_VALUE);
		}
	} catch (ConvertExcept &_e) {
		if (_e.num == ERROR_CAN_NOT_CONVERT_TO_VALUE) {
			cerr << "ERROR : CAN_NOT_CONVERT_TO_VALUE : ";
		}
		cerr << "stringToValue(template)" << endl;
		throw;
	} catch (...) {
		cerr << "ERROR : UNKNOWN : ";
		cerr << "stringToValue(template)" << endl;
		throw;
	}
}

/**############################################################################
 # NON MEMBER DECLARATIONS
 #############################################################################*/

/**############################################################################
 # END OF FILE
 #############################################################################*/

#endif /* __CONVERT_H ********************************************************/

