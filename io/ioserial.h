/**############################################################################
#
#
# Copyright (C) 2021 by Christian Scheunert
#
# Email: christian.scheunert@tu-dresden.de
#
#############################################################################*/

#ifndef __IOSERIAL_H
#define __IOSERIAL_H

/**############################################################################
 # INCLUDES
 #############################################################################*/

#include "convert.h"
#include "ioconfigfile.h"
#include <libserial/SerialPort.h>
#include "../tools/quaternion/quaternion.hpp"

#include <armadillo>
#include <filesystem>
#include <iostream>
#include <string>

/**############################################################################
 # NAMES
 #############################################################################*/

using namespace std;

using LibSerial::SerialPort;
using LibSerial::BaudRate;
using LibSerial::ReadTimeout;

/**############################################################################
 # DEFINES
 #############################################################################*/

/**############################################################################
 # CLASS DECLARATIONS
 #############################################################################*/

/******************************************************************************
 * A Exception class that refers to the @ref IoSerial class.
 *****************************************************************************/
class IoSerialExcept {
public:
	/** a list of known possible errors with class IoSerial *********************/
	enum {
		ERROR_BAUDRATE_UNKOWN,
		ERROR_CAN_NOT_OPEN_SERIAL_PORT,
		ERROR_SERIAL_PORT_NOT_FOUND,
		ERROR_SERIAL_PORT_TIMEOUT
	};

	/** the number of error ****************************************************/
	int num;

	/****************************************************************************
	 * This construction function is used to set the number of exception.
	 * @param _num Number of error that has been found.
	 ***************************************************************************/
	IoSerialExcept(int _num);
};

/******************************************************************************
 *****************************************************************************/
class IoSerial {

	/**  *************************************/
	SerialPort serialPort;

	/**  *************************************/
	size_t serialTimeout;

public:
	/****************************************************************************
	 ***************************************************************************/
	IoSerial(IoConfigFile &_ioConf, const string &_confSection = "IoSerial");

	/****************************************************************************
	 ***************************************************************************/
	~IoSerial();

	/****************************************************************************
	 ***************************************************************************/
	void readLine(string &_Id, string &_Acc, string &_Gyro, string &_Mag, string &_Q);

	/****************************************************************************
	 ***************************************************************************/
	void readLine(unsigned int &_Id, arma::vec &_Acc, arma::vec &_Gyro, arma::vec &_Mag, Quaternion &_Q);
};

/**############################################################################
 # INLINE MEMBER DEFINITIONS
 #############################################################################*/

/******************************************************************************
 *****************************************************************************/
inline void IoSerial::readLine(string &_Id, string &_Acc, string &_Gyro, string &_Mag, string &_Q) {

	unsigned count = 0;
	size_t startDelPos;
	size_t stopDelPos;
	string line;
	const string startDelAcc = "A";
	const string stopDelAcc = "G";
	const string startDelGyro = "G";
	const string stopDelGyro = "M";
	const string startDelMag = "M";
	const string stopDelMag = "Q";
	const string startDelQ = "Q";
	const string stopDelQ = "";

	try {

		do {
			if (count > 0) {
				cout << "INFO : serial data start delimiter '" <<  startDelAcc << "' not found" << endl;
			}
			serialPort.ReadLine(line, '\n', serialTimeout);
			startDelPos = line.find_first_of(startDelAcc);
			count++;
		}
		while(startDelPos == string::npos);

		_Id= line.substr(0,1);

		stopDelPos = line.find_last_of(stopDelAcc);
		_Acc = line.substr(startDelPos + 1, stopDelPos - startDelPos - 1);

		startDelPos = line.find_first_of(startDelGyro);
		stopDelPos = line.find_last_of(stopDelGyro);
		_Gyro = line.substr(startDelPos + 1, stopDelPos - startDelPos - 1);

		startDelPos = line.find_first_of(startDelMag);
		stopDelPos = line.find_last_of(stopDelMag);
		_Mag = line.substr(startDelPos + 1, stopDelPos - startDelPos - 1);

		startDelPos = line.find_first_of(startDelQ);
		stopDelPos = line.find_last_of(stopDelQ);
		_Q = line.substr(startDelPos + 1, stopDelPos - startDelPos - 1);
	}

	catch (const ReadTimeout&) {
		cerr << "ERROR : SERIAL_PORT_TIMEOUT : IoSerial::readLine(string)" << endl;
	}

	catch (...) {
		cerr << "ERROR : UNKNOWN : ";
		cerr << "IoSerial::readLine(string)" << endl;
	}
}

/******************************************************************************
 *****************************************************************************/
inline void IoSerial::readLine(unsigned int &_Id, arma::vec &_Acc, arma::vec &_Gyro, arma::vec &_Mag, Quaternion &_Q) {

	string acc, gyro, mag, q, id;

	try {
		readLine(id, acc, gyro, mag, q);
		_Id = std::stoi (id);
		_Acc = acc;
		_Gyro=gyro;
		_Mag = mag;
		_Q = q;
	}

	catch (...) {
		cerr << "ERROR : UNKNOWN : ";
		cerr << "IoSerial::readLine(arma::vec)" << endl;
	}
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

#endif /* __IOSERIAL_H *******************************************************/
