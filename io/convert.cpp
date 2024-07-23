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

#include "convert.h"

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
ConvertExcept::ConvertExcept(int _num) {
	num = _num;
}

/******************************************************************************
 *****************************************************************************/
Convert::Convert() {
	try {

	} catch (...) {
		cerr << "ERROR : UNKNOWN : ";
		cerr << "Convert::Convert" << endl;
		throw;
	}
}

/******************************************************************************
 *****************************************************************************/
Convert::~Convert() {
	try {

	} catch (...) {
		cerr << "ERROR : UNKNOWN : ";
		cerr << "Convert::~Convert" << endl;
		throw;
	}
}

/******************************************************************************
 *****************************************************************************/
string Convert::toString(const string &_value) {
	string value = _value;
	try {
		if (ios::left & outputStream.flags()) {
			while (value.size() < (unsigned long) outputStream.width()) {
				value += ((char) outputStream.fill());
			}
		} else {
			while (value.size() < (unsigned long) outputStream.width()) {
				value = ((char) outputStream.fill()) + value;
			}
		}
		return (value);
	} catch (...) {
		cerr << "ERROR : UNKNOWN : ";
		cerr << "Convert::toString(string)" << endl;
		throw;
	}
}

/******************************************************************************
 *****************************************************************************/
string Convert::toString(const bool &_value) {
	try {
		if (_value == true) {
			return ((string) "true");
		} else {
			return ((string) "false");
		}
	} catch (...) {
		cerr << "ERROR : UNKNOWN : ";
		cerr << "Convert::toString(bool)" << endl;
		throw;
	}
}

/******************************************************************************
 *****************************************************************************/
string Convert::toString(const float &_value) {
	try {
		/** convert the value into a string **************************************/
		return (valueToString(_value));
	} catch (...) {
		cerr << "ERROR : UNKNOWN : ";
		cerr << "Convert::toString(float)" << endl;
		throw;
	}
}

/******************************************************************************
 *****************************************************************************/
string Convert::toString(const double &_value) {
	try {
		/** convert the value into a string **************************************/
		return (valueToString(_value));
	} catch (...) {
		cerr << "ERROR : UNKNOWN : ";
		cerr << "Convert::toString(double)" << endl;
		throw;
	}
}

/******************************************************************************
 *****************************************************************************/
string Convert::toString(const long double &_value) {
	try {
		/** convert the value into a string **************************************/
		return (valueToString(_value));
	} catch (...) {
		cerr << "ERROR : UNKNOWN : ";
		cerr << "Convert::toString(long double)" << endl;
		throw;
	}
}

/******************************************************************************
 *****************************************************************************/
string Convert::toString(const char &_value) {
	try {
		/** convert the value into a string **************************************/
		return (valueToString(_value));
	} catch (...) {
		cerr << "ERROR : UNKNOWN : ";
		cerr << "Convert::toString(char)" << endl;
		throw;
	}
}

/******************************************************************************
 *****************************************************************************/
string Convert::toString(const short &_value) {
	try {
		/** convert the value into a string **************************************/
		return (valueToString(_value));
	} catch (...) {
		cerr << "ERROR : UNKNOWN : ";
		cerr << "Convert::toString(short)" << endl;
		throw;
	}
}

/******************************************************************************
 *****************************************************************************/
string Convert::toString(const int &_value) {
	try {
		/** convert the value into a string **************************************/
		return (valueToString(_value));
	} catch (...) {
		cerr << "ERROR : UNKNOWN : ";
		cerr << "Convert::toString(int)" << endl;
		throw;
	}
}

/******************************************************************************
 *****************************************************************************/
string Convert::toString(const long &_value) {
	try {
		/** convert the value into a string **************************************/
		return (valueToString(_value));
	} catch (...) {
		cerr << "ERROR : UNKNOWN : ";
		cerr << "Convert::toString(long)" << endl;
		throw;
	}
}

/******************************************************************************
 *****************************************************************************/
string Convert::toString(const unsigned char &_value) {
	try {
		/** convert the value into a string **************************************/
		return (valueToString(_value));
	} catch (...) {
		cerr << "ERROR : UNKNOWN : ";
		cerr << "Convert::toString(unsigned char)" << endl;
		throw;
	}
}

/******************************************************************************
 *****************************************************************************/
string Convert::toString(const unsigned short &_value) {
	try {
		/** convert the value into a string **************************************/
		return (valueToString(_value));
	} catch (...) {
		cerr << "ERROR : UNKNOWN : ";
		cerr << "Convert::toString(unsigned short)" << endl;
		throw;
	}
}

/******************************************************************************
 *****************************************************************************/
string Convert::toString(const unsigned int &_value) {
	try {
		/** convert the value into a string **************************************/
		return (valueToString(_value));
	} catch (...) {
		cerr << "ERROR : UNKNOWN : ";
		cerr << "Convert::toString(unsigned int)" << endl;
		throw;
	}
}

/******************************************************************************
 *****************************************************************************/
string Convert::toString(const unsigned long &_value) {
	try {
		/** convert the value into a string **************************************/
		return (valueToString(_value));
	} catch (...) {
		cerr << "ERROR : UNKNOWN : ";
		cerr << "Convert::toString(unsigned long)" << endl;
		throw;
	}
}

/******************************************************************************
 *****************************************************************************/
void Convert::toValue(string &_value, const string &_string) {
	try {
		_value = _string;
	} catch (...) {
		cerr << "ERROR : UNKNOWN : ";
		cerr << "Convert::toValue(string)" << endl;
		throw;
	}
}

/******************************************************************************
 *****************************************************************************/
void Convert::toValue(bool &_value, const string &_string) {
	string tmpStr;

	try {
		tmpStr = _string;
		toLowerCase(tmpStr);
		if ((tmpStr == "1") || (tmpStr == "true") || (tmpStr == "yes")) {
			_value = true;
		} else if ((tmpStr == "0") || (tmpStr == "false") || (tmpStr == "no")) {
			_value = false;
		} else {
			throw ConvertExcept(ERROR_CAN_NOT_CONVERT_TO_VALUE);
		}
	} catch (ConvertExcept &_e) {
		if (_e.num == ERROR_CAN_NOT_CONVERT_TO_VALUE) {
			cerr << "ERROR : CAN_NOT_CONVERT_TO_VALUE : ";
		}
		cerr << "Convert::toValue(bool)" << endl;
		throw;
	} catch (...) {
		cerr << "ERROR : UNKNOWN : ";
		cerr << "Convert::toValue(bool)" << endl;
		throw;
	}
}

/******************************************************************************
 *****************************************************************************/
void Convert::toValue(float &_value, const string &_string) {
	try {
		/** check the string against the permitted chars *************************/
		if (string::npos != _string.find_first_not_of(PERMITTED_FLOAT_CHARS)) {
			throw ConvertExcept(ERROR_NON_VALID_CHARS_IN_STRING);
		}

		/** convert the string into a value **************************************/
		stringToValue(_value, _string);
	} catch (ConvertExcept &_e) {
		if (_e.num == ERROR_NON_VALID_CHARS_IN_STRING) {
			cerr << "ERROR : NON_VALID_CHARS_IN_STRING : ";
			cerr << "string = " << _string << " : ";
		}
		cerr << "Convert::toValue(float)" << endl;
		throw;
	} catch (...) {
		cerr << "ERROR : UNKNOWN : ";
		cerr << "Convert::toValue(float)" << endl;
		throw;
	}
}

/******************************************************************************
 *****************************************************************************/
void Convert::toValue(double &_value, const string &_string) {
	try {
		/** check the string against the permitted chars *************************/
		if (string::npos != _string.find_first_not_of(PERMITTED_FLOAT_CHARS)) {
			throw ConvertExcept(ERROR_NON_VALID_CHARS_IN_STRING);
		}

		/** convert the string into a value **************************************/
		stringToValue(_value, _string);
	} catch (ConvertExcept &_e) {
		if (_e.num == ERROR_NON_VALID_CHARS_IN_STRING) {
			cerr << "ERROR : NON_VALID_CHARS_IN_STRING : ";
			cerr << "string = " << _string << " : ";
		}
		cerr << "Convert::toValue(double)" << endl;
		throw;
	} catch (...) {
		cerr << "ERROR : UNKNOWN : ";
		cerr << "Convert::toValue(double)" << endl;
		throw;
	}
}

/******************************************************************************
 *****************************************************************************/
void Convert::toValue(long double &_value, const string &_string) {
	try {
		/** check the string against the permitted chars *************************/
		if (string::npos != _string.find_first_not_of(PERMITTED_FLOAT_CHARS)) {
			throw ConvertExcept(ERROR_NON_VALID_CHARS_IN_STRING);
		}

		/** convert the string into a value **************************************/
		stringToValue(_value, _string);
	} catch (ConvertExcept &_e) {
		if (_e.num == ERROR_NON_VALID_CHARS_IN_STRING) {
			cerr << "ERROR : NON_VALID_CHARS_IN_STRING : ";
			cerr << "string = " << _string << " : ";
		}
		cerr << "Convert::toValue(long double)" << endl;
		throw;
	} catch (...) {
		cerr << "ERROR : UNKNOWN : ";
		cerr << "Convert::toValue(long double)" << endl;
		throw;
	}
}

/******************************************************************************
 *****************************************************************************/
void Convert::toValue(char &_value, const string &_string) {
	try {
		/** check the string against the permitted chars *************************/
		if (string::npos != _string.find_first_not_of(PERMITTED_SIGNED_CHARS)) {
			throw ConvertExcept(ERROR_NON_VALID_CHARS_IN_STRING);
		}

		/** convert the string into a value **************************************/
		stringToValue(_value, _string);
	} catch (ConvertExcept &_e) {
		if (_e.num == ERROR_NON_VALID_CHARS_IN_STRING) {
			cerr << "ERROR : NON_VALID_CHARS_IN_STRING : ";
			cerr << "string = " << _string << " : ";
		}
		cerr << "Convert::toValue(char)" << endl;
		throw;
	} catch (...) {
		cerr << "ERROR : UNKNOWN : ";
		cerr << "Convert::toValue(char)" << endl;
		throw;
	}
}

/******************************************************************************
 *****************************************************************************/
void Convert::toValue(short &_value, const string &_string) {
	try {
		/** check the string against the permitted chars *************************/
		if (string::npos != _string.find_first_not_of(PERMITTED_SIGNED_CHARS)) {
			throw ConvertExcept(ERROR_NON_VALID_CHARS_IN_STRING);
		}

		/** convert the string into a value **************************************/
		stringToValue(_value, _string);
	} catch (ConvertExcept &_e) {
		if (_e.num == ERROR_NON_VALID_CHARS_IN_STRING) {
			cerr << "ERROR : NON_VALID_CHARS_IN_STRING : ";
			cerr << "string = " << _string << " : ";
		}
		cerr << "Convert::toValue(short)" << endl;
		throw;
	} catch (...) {
		cerr << "ERROR : UNKNOWN : ";
		cerr << "Convert::toValue(short)" << endl;
		throw;
	}
}

/******************************************************************************
 *****************************************************************************/
void Convert::toValue(int &_value, const string &_string) {
	try {
		/** check the string against the permitted chars *************************/
		if (string::npos != _string.find_first_not_of(PERMITTED_SIGNED_CHARS)) {
			throw ConvertExcept(ERROR_NON_VALID_CHARS_IN_STRING);
		}

		/** convert the string into a value **************************************/
		stringToValue(_value, _string);
	} catch (ConvertExcept &_e) {
		if (_e.num == ERROR_NON_VALID_CHARS_IN_STRING) {
			cerr << "ERROR : NON_VALID_CHARS_IN_STRING : ";
			cerr << "string = " << _string << " : ";
		}
		cerr << "Convert::toValue(int)" << endl;
		throw;
	} catch (...) {
		cerr << "ERROR : UNKNOWN : ";
		cerr << "Convert::toValue(int)" << endl;
		throw;
	}
}

/******************************************************************************
 *****************************************************************************/
void Convert::toValue(long &_value, const string &_string) {
	try {
		/** check the string against the permitted chars *************************/
		if (string::npos != _string.find_first_not_of(PERMITTED_SIGNED_CHARS)) {
			throw ConvertExcept(ERROR_NON_VALID_CHARS_IN_STRING);
		}

		/** convert the string into a value **************************************/
		stringToValue(_value, _string);
	} catch (ConvertExcept &_e) {
		if (_e.num == ERROR_NON_VALID_CHARS_IN_STRING) {
			cerr << "ERROR : NON_VALID_CHARS_IN_STRING : ";
			cerr << "string = " << _string << " : ";
		}
		cerr << "Convert::toValue(long)" << endl;
		throw;
	} catch (...) {
		cerr << "ERROR : UNKNOWN : ";
		cerr << "Convert::toValue(long)" << endl;
		throw;
	}
}

/******************************************************************************
 *****************************************************************************/
void Convert::toValue(unsigned char &_value, const string &_string) {
	try {
		/** check the string against the permitted chars *************************/
		if (string::npos != _string.find_first_not_of(PERMITTED_UNSIGNED_CHARS)) {
			throw ConvertExcept(ERROR_NON_VALID_CHARS_IN_STRING);
		}

		/** convert the string into a value **************************************/
		stringToValue(_value, _string);
	} catch (ConvertExcept &_e) {
		if (_e.num == ERROR_NON_VALID_CHARS_IN_STRING) {
			cerr << "ERROR : NON_VALID_CHARS_IN_STRING : ";
			cerr << "string = " << _string << " : ";
		}
		cerr << "Convert::toValue(unsigned char)" << endl;
		throw;
	} catch (...) {
		cerr << "ERROR : UNKNOWN : ";
		cerr << "Convert::toValue(unsigned char)" << endl;
		throw;
	}
}

/******************************************************************************
 *****************************************************************************/
void Convert::toValue(unsigned short &_value, const string &_string) {
	try {
		/** check the string against the permitted chars *************************/
		if (string::npos != _string.find_first_not_of(PERMITTED_UNSIGNED_CHARS)) {
			throw ConvertExcept(ERROR_NON_VALID_CHARS_IN_STRING);
		}

		/** convert the string into a value **************************************/
		stringToValue(_value, _string);
	} catch (ConvertExcept &_e) {
		if (_e.num == ERROR_NON_VALID_CHARS_IN_STRING) {
			cerr << "ERROR : NON_VALID_CHARS_IN_STRING : ";
			cerr << "string = " << _string << " : ";
		}
		cerr << "Convert::toValue(unsigned short)" << endl;
		throw;
	} catch (...) {
		cerr << "ERROR : UNKNOWN : ";
		cerr << "Convert::toValue(unsigned short)" << endl;
		throw;
	}
}

/******************************************************************************
 *****************************************************************************/
void Convert::toValue(unsigned int &_value, const string &_string) {
	try {
		/** check the string against the permitted chars *************************/
		if (string::npos != _string.find_first_not_of(PERMITTED_UNSIGNED_CHARS)) {
			throw ConvertExcept(ERROR_NON_VALID_CHARS_IN_STRING);
		}

		/** convert the string into a value **************************************/
		stringToValue(_value, _string);
	} catch (ConvertExcept &_e) {
		if (_e.num == ERROR_NON_VALID_CHARS_IN_STRING) {
			cerr << "ERROR : NON_VALID_CHARS_IN_STRING : ";
			cerr << "string = " << _string << " : ";
		}
		cerr << "Convert::toValue(unsigned int)" << endl;
		throw;
	} catch (...) {
		cerr << "ERROR : UNKNOWN : ";
		cerr << "Convert::toValue(unsigned int)" << endl;
		throw;
	}
}

/******************************************************************************
 *****************************************************************************/
void Convert::toValue(unsigned long &_value, const string &_string) {
	try {
		/** check the string against the permitted chars *************************/
		if (string::npos != _string.find_first_not_of(PERMITTED_UNSIGNED_CHARS)) {
			throw ConvertExcept(ERROR_NON_VALID_CHARS_IN_STRING);
		}

		/** convert the string into a value **************************************/
		stringToValue(_value, _string);
	} catch (ConvertExcept &_e) {
		if (_e.num == ERROR_NON_VALID_CHARS_IN_STRING) {
			cerr << "ERROR : NON_VALID_CHARS_IN_STRING : ";
			cerr << "string = " << _string << " : ";
		}
		cerr << "Convert::toValue(unsigned long)" << endl;
		throw;
	} catch (...) {
		cerr << "ERROR : UNKNOWN : ";
		cerr << "Convert::toValue(unsigned long)" << endl;
		throw;
	}
}

/**############################################################################
 # NON MEMBER DEFINITIONS
 #############################################################################*/

/**############################################################################
 # END OF FILE
 #############################################################################*/
