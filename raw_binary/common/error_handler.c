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
#include <sys/wait.h>
#include <unistd.h>
#include <sys/prctl.h>

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

void print_trace() {
    char pid_buf[30];
    sprintf(pid_buf, "%d", getpid());
    char name_buf[512];
    name_buf[readlink("/proc/self/exe", name_buf, 511)]=0;
    prctl(PR_SET_PTRACER, PR_SET_PTRACER_ANY, 0, 0, 0);
    int child_pid = fork();
    if (!child_pid) {
        dup2(2,1); // redirect output to stderr - edit: unnecessary?
        execl("/usr/bin/gdb", "gdb", "--batch", "-n", "-ex", "thread", "-ex", "bt", name_buf, pid_buf, NULL);
        abort(); /* If gdb failed to start */
    } else {
        waitpid(child_pid,NULL,0);
    }
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
        print_trace ();
        printf ("Error: %s : %s\n\n", module, errmsg);
    }
    else
        printf ("Warning: %s : %s\n", module, errmsg);
}
