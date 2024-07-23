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

#include "iooption.h"


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
IoOptionExcept::IoOptionExcept(int _num)
{
  num = _num;
}


/******************************************************************************
 *****************************************************************************/
IoOption::IoOption()
{
  try {

  }
  catch (...) {
    cerr << "ERROR : UNKNOWN : ";
    cerr << "IoOption::IoOption" << endl;
    throw;
  }
}


/******************************************************************************
 *****************************************************************************/
IoOption::~IoOption()
{
  try {

  }
  catch (...) {
    cerr << "ERROR : UNKNOWN : ";
    cerr << "IoOption::~IoOption" << endl;
    throw;
  }
}


/******************************************************************************
 *****************************************************************************/
void IoOption::addOption(const string &_name, const string &_shortcut,
                         const unsigned int &_maxNumParam)
{
  Option option;
  unsigned long i;
  
  try {
    /** check the option name ************************************************/
    if (_name.size() < 1) {
      throw IoOptionExcept(ERROR_MISSING_CHARS_IN_OPTION_NAME);
    }
  
    /** check the option shortcut ********************************************/
    if (_shortcut.size() < 1) {
      throw IoOptionExcept(ERROR_MISSING_CHARS_IN_OPTION_SHORTCUT);
    }
    if (1 < _shortcut.size()) {
      throw IoOptionExcept(ERROR_TOO_MANY_CHARS_IN_OPTION_SHORTCUT);
    }

    /** check if this option already exists **********************************/
    for (i = 0; i < optionList.size(); i++) {
      if ((optionList[i].name == _name) ||
          (optionList[i].shortcut == _shortcut)) {
        throw IoOptionExcept(ERROR_OPTION_ALREADY_EXISTS);
      }
    }
  
    /** set the new option ***************************************************/
    option.isSet = false;
    option.name = _name;
    option.shortcut = _shortcut;
    option.maxNumParam = _maxNumParam;

    /** append new option to the option list *********************************/
    optionList.push_back(option);    
  }
  catch (IoOptionExcept &_e) {
    if (_e.num == ERROR_MISSING_CHARS_IN_OPTION_NAME) {
      cerr << "ERROR : MISSING_CHARS_IN_OPTION_NAME : ";
      cerr << "option name = " << _name << " : ";
    }
    if (_e.num == ERROR_MISSING_CHARS_IN_OPTION_SHORTCUT) {
      cerr << "ERROR : MISSING_CHARS_IN_OPTION_SHORTCUT : ";
      cerr << "option shortcut = " << _shortcut << " : ";
    }
    if (_e.num == ERROR_TOO_MANY_CHARS_IN_OPTION_SHORTCUT) {
      cerr << "ERROR : TOO_MANY_CHARS_IN_OPTION_SHORTCUT : ";
      cerr << "option shortcut = " << _shortcut << " : ";
    }
    if (_e.num == ERROR_OPTION_ALREADY_EXISTS) {
      cerr << "ERROR : OPTION_ALREADY_EXISTS : ";
      cerr << "option = --" << _name << " or -" << _shortcut << " : ";
    }
    cerr << "IoOption::addOption" << endl;
    throw;
  }
  catch (bad_alloc &_e) {
    cerr << "ERROR : CAN_NOT_CREATE_OPTION : ";
    cerr << "IoOption::addOption" << endl;
    throw;
  }
  catch (...) {
    cerr << "ERROR : UNKNOWN : ";
    cerr << "IoOption::addOption" << endl;
    throw;
  }
}



/******************************************************************************
 *****************************************************************************/
void IoOption::extractOptions(int argc, char *argv[])
{
  string name;
  string shortcut;
  bool haveOption;
  unsigned long curArgv;
  unsigned long curOption;

  try {
    /** check for options and parameters *************************************/
    if (1 < argc) {

      /** make sure an option comes first ************************************/
      if (argv[1][0] != '-') {
        throw IoOptionExcept(ERROR_PARAMETER_BEFORE_OPTION);
      }

      /** get the options and parameters *************************************/
      for (curArgv = 1; curArgv < (unsigned long) argc; curArgv++) {

        /**********************************************************************
         * option name or option shortcut found
         *********************************************************************/
        if (argv[curArgv][0] == '-') {

          /********************************************************************
           * option name found, i.e. --option-name
           *******************************************************************/
          if (argv[curArgv][1] == '-') {

            /** the option name must have at least one character *************/
            if (argv[curArgv][2] == 0) {
              throw IoOptionExcept(ERROR_MISSING_CHARS_IN_OPTION_NAME);
            }

            /** copy the option name *****************************************/
            name.assign(argv[curArgv]);
            name.erase(0, 2);
          }
          
          /********************************************************************
           * option shoutcut found, i.e. -o where o is an option shortcut
           *******************************************************************/
          else {

            /** the option shortcut must have at least one character *********/
            if (argv[curArgv][1] == 0) {
              throw IoOptionExcept(ERROR_MISSING_CHARS_IN_OPTION_SHORTCUT);
            }

            /** the option shortcut may not have more than one character *****/
            if (argv[curArgv][2] != 0) {
              throw IoOptionExcept(ERROR_TOO_MANY_CHARS_IN_OPTION_SHORTCUT);
            }

            /** copy the option shortcut *************************************/
            shortcut = argv[curArgv][1];
          }

          /********************************************************************
           * check if this option is supported
           *******************************************************************/
          curOption = 0;
          haveOption = false;

          do {
            /** check for option name and shortcut ***************************/
            if ((optionList[curOption].name == name) ||
                (optionList[curOption].shortcut == shortcut)) {
            
              /** check if this option has already been set ******************/
              if (optionList[curOption].isSet) {
                throw IoOptionExcept(ERROR_OPTION_HAS_ALREADY_BEEN_SET);
              }

              /** mark this option as set ************************************/
              optionList[curOption].isSet = true;

              /** make sure the do-while-loop will be finished ***************/
              haveOption = true;
            }
            else {
              /** go on with the next option *********************************/
              curOption++;
            }
          } while ((! haveOption) && (curOption < optionList.size()));

          /** if we do not have an option the name and shortcut is unknown ***/
          if (! haveOption) {
            throw IoOptionExcept(ERROR_UNKNOWN_OPTION);
          }
        }
        
        /**********************************************************************
         * found a parameter related to an option
         *********************************************************************/
        else {

          /** copy the parameter *********************************************/
          optionList[curOption].paramList.push_back(argv[curArgv]);

          /** check number of parameters *************************************/
          if (optionList[curOption].maxNumParam <
              optionList[curOption].paramList.size()) {
            throw IoOptionExcept(ERROR_TOO_MANY_PARAMETERS);
          }
        }
      }
    }
  }
  catch (IoOptionExcept &_e) {
    if (_e.num == ERROR_PARAMETER_BEFORE_OPTION) {
      cerr << "ERROR : PARAMETER_BEFORE_OPTION : ";
      cerr << "parameter = " << argv[1] << " : ";
    }
    if (_e.num == ERROR_MISSING_CHARS_IN_OPTION_NAME) {
      cerr << "ERROR : MISSING_CHARS_IN_OPTION_NAME : ";
      cerr << "option = " << argv[curArgv] << " : ";
    }
    if (_e.num == ERROR_MISSING_CHARS_IN_OPTION_SHORTCUT) {
      cerr << "ERROR : MISSING_CHARS_IN_OPTION_SHORTCUT : ";
      cerr << "option = " << argv[curArgv] << " : ";
    }
    if (_e.num == ERROR_TOO_MANY_CHARS_IN_OPTION_SHORTCUT) {
      cerr << "ERROR : TOO_MANY_CHARS_IN_OPTION_SHORTCUT : ";
      cerr << "option = " << argv[curArgv] << " : ";
    }
    if (_e.num == ERROR_OPTION_HAS_ALREADY_BEEN_SET) {
      cerr << "ERROR : OPTION_HAS_ALREADY_BEEN_SET : ";
      cerr << "option = --" << optionList[curOption].name << " ";
      cerr << "or -" << optionList[curOption].shortcut << " : ";
    }
    if (_e.num == ERROR_UNKNOWN_OPTION) {
      cerr << "ERROR : UNKNOWN_OPTION : ";
      cerr << "option = " << argv[curArgv] << " : ";
    }
    if (_e.num == ERROR_TOO_MANY_PARAMETERS) {
      cerr << "ERROR : TOO_MANY_PARAMETERS : ";
      cerr << "option = --" << optionList[curOption].name << " ";
      cerr << "or -" << optionList[curOption].shortcut << " : ";
    }
    cerr << "IoOption::extractOptions" << endl;
    throw;
  }
  catch (...) {
    cerr << "ERROR : UNKNOWN : ";
    cerr << "IoOption::extractOptions" << endl;
    throw;
  }
}


/******************************************************************************
 *****************************************************************************/
bool IoOption::getParam(string &_param, const string &_optionName,
                        const unsigned long &_index)
{
  unsigned long i;

  try {
    /** try to find the specified option *************************************/
    for (i = 0; i < optionList.size(); i++) {
      if (optionList[i].name == _optionName) {
      
        /** check if that option was set *************************************/
        if (optionList[i].isSet) {
        
          /** check the given index against size of parameter list ***********/
          if (_index < optionList[i].paramList.size()) {
            _param = optionList[i].paramList[_index];
            return(true);
          }
          else {
            throw IoOptionExcept(ERROR_PARAMETER_INDEX_OUT_OF_RANGE);
          }
        }
        else {
          return(false);
        }
      }
    }
    throw IoOptionExcept(ERROR_UNKNOWN_OPTION);
  }
  catch (IoOptionExcept &_e) {
    if (_e.num == ERROR_PARAMETER_INDEX_OUT_OF_RANGE) {
      cerr << "ERROR : PARAMETER_INDEX_OUT_OF_RANGE : ";
      cerr << "only found " << optionList[i].paramList.size() << " ";
      cerr << "parameter(s) for option = --" << optionList[i].name << " ";
      cerr << "or -" << optionList[i].shortcut << " : ";
    }
    if (_e.num == ERROR_UNKNOWN_OPTION) {
      cerr << "ERROR : UNKNOWN_OPTION : ";
      cerr << "option = " << _optionName << " : ";
    }
    cerr << "IoOption::getParam" << endl;
    throw;
  }
  catch (...) {
    cerr << "ERROR : UNKNOWN : ";
    cerr << "IoOption::getParam" << endl;
    throw;
  }
}


/******************************************************************************
 *****************************************************************************/
bool IoOption::getParams(valarray<string> &_params, const string &_optionName)
{
  unsigned long i;
  unsigned long j;

  try {
    /** try to find the specified option *************************************/
    for (i = 0; i < optionList.size(); i++) {
      if (optionList[i].name == _optionName) {
      
        /** check if that option was set *************************************/
        if (optionList[i].isSet) {
          _params.resize(optionList[i].paramList.size());

          /** copy the strings ***********************************************/
          for (j = 0; j < _params.size(); j++) {
            _params[j].assign(optionList[i].paramList[j]);
          }
          return(true);
        }
        else {
          return(false);
        }
      }
    }
    throw IoOptionExcept(ERROR_UNKNOWN_OPTION);
  }
  catch (IoOptionExcept &_e) {
    if (_e.num == ERROR_UNKNOWN_OPTION) {
      cerr << "ERROR : UNKNOWN_OPTION : ";
      cerr << "option = " << _optionName << " : ";
    }
    cerr << "IoOption::getParamList" << endl;
    throw;
  }
  catch (...) {
    cerr << "ERROR : UNKNOWN : ";
    cerr << "IoOption::getParamList" << endl;
    throw;
  }
}


/**############################################################################
# NON MEMBER DEFINITIONS
#############################################################################*/


/**############################################################################
# END OF FILE
#############################################################################*/
