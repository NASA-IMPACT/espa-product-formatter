/*****************************************************************************
FILE: error_handler.c
  
PURPOSE: Contains functions for error handling.

PROJECT:  Land Satellites Data System Science Research and Development (LSRD)
at the USGS EROS

LICENSE TYPE:  NASA Open Source Agreement Version 1.3

HISTORY:
Date         Programmer       Reason
----------   --------------   -------------------------------------
12/12/2013   Gail Schmidt     Original development

NOTES:
*****************************************************************************/

#include "error_handler.h"
#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>

/******************************************************************************
MODULE:  error_handler

PURPOSE:  Prints the error/warning message.

RETURN VALUE:
Type = None

HISTORY:
Date         Programmer       Reason
----------   --------------   -------------------------------------
12/12/2013   Gail Schmidt     Original development

NOTES:
******************************************************************************/

void print_trace (void)
{
  void *array[20];
  size_t size;
  char **strings;
  size_t i;

  size = backtrace (array, 20);
  strings = backtrace_symbols (array, size);

  printf ("Obtained %zd stack frames.\n", size);

  for (i = 0; i < size; i++)
     printf ("%s\n", strings[i]);

  free (strings);
}

void error_handler
(
    bool error_flag,  /* I: true for errors, false for warnings */
    char *module,     /* I: calling module name */
    char *errmsg      /* I: error/warning message to be printed, without
                            ending EOL */
)
{
    if (error_flag)
    {
        printf ("Error: %s : %s\n\n", module, errmsg);
        print_trace ();
    }
    else
        printf ("Warning: %s : %s\n", module, errmsg);
}
