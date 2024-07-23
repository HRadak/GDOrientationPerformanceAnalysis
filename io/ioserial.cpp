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

#include "ioserial.h"

/**############################################################################
 # GLOBALS
 #############################################################################*/

/**############################################################################
 # LOCAL DEFINITIONS
 #############################################################################*/

/**############################################################################
 # STATIC MEMBER DEFINITIONS
 #############################################################################*/

/**############################################################################
 # MEMBER DEFINITIONS
 #############################################################################*/

/******************************************************************************
 *****************************************************************************/
IoSerialExcept::IoSerialExcept(int _num) {
	num = _num;
}

/******************************************************************************
 *****************************************************************************/
IoSerial::IoSerial(IoConfigFile &_ioConf, const string &_confSection) {

	valarray<string> serialPortNames;
	size_t serialBaudRate;

	try {
		/** get config data from section _confSection ****************************/
		_ioConf.getValues(serialPortNames, "serialPortNames", _confSection);

		/** get config data from section _confSection ****************************/
		_ioConf.getValue(serialBaudRate, "serialBaudRate", _confSection);

		/** get config data from section _confSection ****************************/
		_ioConf.getValue(serialTimeout, "serialTimeout", _confSection);

		/**  *************************************/
		for (size_t i = 0; i < serialPortNames.size(); i++) {
			if (filesystem::exists(serialPortNames[i])) {
				cout << "INFO : opening serial port " << serialPortNames[i];
				serialPort.Open(serialPortNames[i]);
				if (serialPort.IsOpen()) {
					cout << " : done" << endl;
				} else {
					throw IoSerialExcept(
							IoSerialExcept::ERROR_CAN_NOT_OPEN_SERIAL_PORT);
				}
				break;
			}
			if (i == serialPortNames.size() - 1) {
				throw IoSerialExcept(IoSerialExcept::ERROR_SERIAL_PORT_NOT_FOUND);
			}
		}

		/**  *************************************/
		if (serialBaudRate == 50) {
			serialPort.SetBaudRate(BaudRate::BAUD_50);
		} else if (serialBaudRate == 75) {
			serialPort.SetBaudRate(BaudRate::BAUD_75);
		} else if (serialBaudRate == 110) {
			serialPort.SetBaudRate(BaudRate::BAUD_110);
		} else if (serialBaudRate == 134) {
			serialPort.SetBaudRate(BaudRate::BAUD_134);
		} else if (serialBaudRate == 150) {
			serialPort.SetBaudRate(BaudRate::BAUD_150);
		} else if (serialBaudRate == 200) {
			serialPort.SetBaudRate(BaudRate::BAUD_200);
		} else if (serialBaudRate == 300) {
			serialPort.SetBaudRate(BaudRate::BAUD_300);
		} else if (serialBaudRate == 600) {
			serialPort.SetBaudRate(BaudRate::BAUD_600);
		} else if (serialBaudRate == 1200) {
			serialPort.SetBaudRate(BaudRate::BAUD_1200);
		} else if (serialBaudRate == 1800) {
			serialPort.SetBaudRate(BaudRate::BAUD_1800);
		} else if (serialBaudRate == 2400) {
			serialPort.SetBaudRate(BaudRate::BAUD_2400);
		} else if (serialBaudRate == 4800) {
			serialPort.SetBaudRate(BaudRate::BAUD_4800);
		} else if (serialBaudRate == 9600) {
			serialPort.SetBaudRate(BaudRate::BAUD_9600);
		} else if (serialBaudRate == 19200) {
			serialPort.SetBaudRate(BaudRate::BAUD_19200);
		} else if (serialBaudRate == 38400) {
			serialPort.SetBaudRate(BaudRate::BAUD_38400);
		} else if (serialBaudRate == 57600) {
			serialPort.SetBaudRate(BaudRate::BAUD_57600);
		} else if (serialBaudRate == 115200) {
			serialPort.SetBaudRate(BaudRate::BAUD_115200);
		} else if (serialBaudRate == 230400) {
			serialPort.SetBaudRate(BaudRate::BAUD_230400);
		} else {
			throw IoSerialExcept(IoSerialExcept::ERROR_BAUDRATE_UNKOWN);
		}
	}

	catch (IoSerialExcept &_e) {
		if (_e.num == IoSerialExcept::ERROR_BAUDRATE_UNKOWN) {
			cerr << "ERROR : BAUDRATE_UNKOWN : ";
		} else if (_e.num == IoSerialExcept::ERROR_CAN_NOT_OPEN_SERIAL_PORT) {
			cerr << "ERROR : SERIAL_PORT_NOT_OPEN : ";
		} else if (_e.num == IoSerialExcept::ERROR_SERIAL_PORT_NOT_FOUND) {
			cerr << "ERROR : SERIAL_PORT_NOT_FOUND : ";
		}
		cerr << "IoSerial::IoSerial" << endl;
	}

	catch (...) {
		cerr << "ERROR : UNKNOWN : ";
		cerr << "IoSerial::IoSerial" << endl;
	}
}

/******************************************************************************
 *****************************************************************************/
IoSerial::~IoSerial() {

	try {
		serialPort.Close();
	}

	catch (...) {
		cerr << "ERROR : UNKNOWN : ";
		cerr << "IoSerial::~IoSerial" << endl;
	}
}

/**############################################################################
 # NON MEMBER DEFINITIONS
 #############################################################################*/

/**############################################################################
 # END OF FILE
 #############################################################################*/
