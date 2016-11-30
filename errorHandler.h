/* 
 * File:            errorHandler.c
 * Project:         Computer Simulation - Phase 4
 * Submission Date: December 3, 2016
 * Author:          Jorge V. Vargas Jr. (0400283)
 * Lab UserId:	    jorge.vargas01
 *
 */

#ifndef ERRORHANDLER_H
#define ERRORHANDLER_H

void errorHndlg(char* comm, int errortype);
void warningHndlg(int warnType);
void setErrors(FILE* fp, char *psw);

#endif	// ERRORHANDLER_H