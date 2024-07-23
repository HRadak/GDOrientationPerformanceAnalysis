/**############################################################################
#
#
# Copyright (C) 2021 by Christian Scheunert
#
# Email: christian.scheunert@tu-dresden.de
#
#############################################################################*/

#ifndef __SENSE_H
#define __SENSE_H


/**#############################################################################
 # INCLUDES
 #############################################################################*/

#include "control.h"
#include "io/convert.h"
#include "io/iooption.h"

#include <cstdlib>
#include <iostream>
#include <string>
#include <valarray>


/**#############################################################################
 # NAMES
 #############################################################################*/

using namespace std;


/**############################################################################
 # DEFINES
 #############################################################################*/

/*******************************************************************************
 ******************************************************************************/
const string DEFAULT_CONF_FILE_SUFFIX = "cfg";
const string DEFAULT_CONF_FILE_BASE_NAME = "sense";
const string DEFAULT_CONF_FILE_NAME = DEFAULT_CONF_FILE_BASE_NAME + "."
		+ DEFAULT_CONF_FILE_SUFFIX;

/*******************************************************************************
 ******************************************************************************/
const string OPTION_NAME_CONF_FILE = "config-file";
const string OPTION_SHORTCUT_CONF_FILE = "c";
const string OPTION_REPETITIONS = "repetitions";
const string OPTION_SHORTCUT_REPETITIONS = "r";


/**#############################################################################
 # CLASS DECLARATIONS
 #############################################################################*/

/*******************************************************************************
 * A Exception class that refers to the @ref sense main function.
 ******************************************************************************/
class senseExcept {
public:
	/** a list of known possible errors with class sense ************************/
	//enum {};
	/** the number of error *****************************************************/
	int num;

	/*****************************************************************************
	 * This construction function is used to set the number of exception.
	 * @param _num Number of error that has been found.
	 ****************************************************************************/
	senseExcept(int _num);
};

/*******************************************************************************
 * A class of command line options used with the @ref sense main function.
 ******************************************************************************/
class SenseOptions {
	/**   *******************************/
	Convert convert;

	/**   *******************************/
	IoOption optionList;

	/**   *******************************/
	valarray<string> confFileNames;

	/**   *******************************/
	size_t repetitions;

	/**   *******************************/
	string optionString;

public:
	/*****************************************************************************
	 ****************************************************************************/
	SenseOptions(int argc, char *argv[]) {
		/** add the options supported *********************************************/
		optionList.addOption(OPTION_NAME_CONF_FILE, OPTION_SHORTCUT_CONF_FILE, 99);
		optionList.addOption(OPTION_REPETITIONS, OPTION_SHORTCUT_REPETITIONS, 1);

		/** extract the options from the command line *****************************/
		optionList.extractOptions(argc, argv);

		/** get all configuration file names in case that option was set **********/
		if (!optionList.getParams(confFileNames, OPTION_NAME_CONF_FILE)) {
			confFileNames.resize(1);
			confFileNames[0] = DEFAULT_CONF_FILE_NAME;
		}

		if (optionList.getParam(optionString, OPTION_REPETITIONS)) {
			convert.toValue(repetitions, optionString);
		} else {
			repetitions = 1;
		}

	}

	/*****************************************************************************
	 ****************************************************************************/
	~SenseOptions() {
	}

	/*****************************************************************************
	 ****************************************************************************/
	size_t getNumRepetitions() {
		return (repetitions);
	}

	/*****************************************************************************
	 ****************************************************************************/
	size_t getNumConfFiles() {
		return ((size_t) confFileNames.size());
	}

	/*****************************************************************************
	 ****************************************************************************/
	string getConfFileName(size_t _num) {
		return (confFileNames[_num]);
	}

	/*****************************************************************************
	 ****************************************************************************/
	string getOptionInfo(size_t _num, size_t _rep) {
		string info = "INFO : using config file " + to_string(_num + 1) + " ("
				+ confFileNames[_num] + ")";
		if (repetitions > 1) {
			return (info + " : repetition " + to_string(_rep + 1));
		} else {
			return (info);
		}
	}
};

/**#############################################################################
 # INLINE MEMBER DEFINITIONS
 #############################################################################*/

/**#############################################################################
 # TEMPLATE MEMBER DEFINITIONS
 #############################################################################*/

/**#############################################################################
 # NON MEMBER DECLARATIONS
 #############################################################################*/

/**#############################################################################
 # END OF FILE
 #############################################################################*/

#endif /** __SENSE_H **********************************************************/
