/**############################################################################
#
#
# Copyright (C) 2021 by Christian Scheunert
#
# Email: christian.scheunert@tu-dresden.de
#
#############################################################################*/

#ifndef __IODIRECTORY_H
#define __IODIRECTORY_H

/**############################################################################
# INCLUDES
#############################################################################*/

#if defined WIN32
  #if defined __INTEL_COMPILER
    #include <direct.h>
    #include <sys/stat.h>
  #elif defined BORLAND_CPP_BUILDER_50
    #include <dir.h>
  #else
    #include <direct.h>
  #endif
#else
  #include <sys/stat.h>
#endif

#include <cstdio>
#include <iostream>
#include <string>


/**############################################################################
# NAMES
#############################################################################*/

using namespace std;


/**############################################################################
# DEFINES
#############################################################################*/

/******************************************************************************
 * @ref IoDirectoryExcept specific constant numbers for errors thrown by
 * several functions of @ref IoDirectory.
 * @see
 *****************************************************************************/
#define ERROR_COULD_NOT_CREATE_DIRECTORY  1
#define ERROR_COULD_NOT_RENAME_DIRECTORY  2


/**############################################################################
# CLASS DECLARATIONS
#############################################################################*/

/******************************************************************************
 * A Exception class that refers to the @ref IoDirectory class.
 * @author Christian M�ller <Christian.Mueller.Mailbox@web.de>
 * @version $Id: iodirectory.h,v 1.10 2004/02/26 15:53:37 muellerc Exp $
 * @see
 *****************************************************************************/
class IoDirectoryExcept {
public:
  /** the number of error ****************************************************/
  int num;
  /****************************************************************************
   * This construction function is used to set the number of exception.
   * @param _num Number of error that has been found.
   * @see
   ***************************************************************************/
  IoDirectoryExcept(int _num);
};


/******************************************************************************
 *
 * @author Christian M�ller <Christian.Mueller.Mailbox@web.de>
 * @version $Id: iodirectory.h,v 1.10 2004/02/26 15:53:37 muellerc Exp $
 * @see
 *****************************************************************************/
class IoDirectory  {
  string dirName;
public:
  /****************************************************************************
   * @see
   ***************************************************************************/
  IoDirectory(const string &_dirName = "");
  /****************************************************************************
   * @see
   ***************************************************************************/
  ~IoDirectory();
  /****************************************************************************
   * @see
   ***************************************************************************/
  void create(const string &_dirName = "");
  /****************************************************************************
   * @see
   ***************************************************************************/
  void rename(const string &_oldDirName, const string &_newDirName);
};



/**############################################################################
# INLINE MEMBER DEFINITIONS
#############################################################################*/


/**############################################################################
# TEMPLATE MEMBER DEFINITIONS
#############################################################################*/


/**############################################################################
# NON MEMBER DECLARATIONS
#############################################################################*/


/**############################################################################
# END OF FILE
#############################################################################*/

#endif /* __IODIRECTORY_H ****************************************************/
