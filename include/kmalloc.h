/*
 * kmalloc.h
 *
 *  Created on: Nov 18, 2011
 *      Author: mdesanti90
 */

#ifndef KMALLOC_H_
#define KMALLOC_H_

void * kmalloc(int pid);

void * krealloc(int pid);

void upPages(int pid);

void downPages(int pid);

uint checkStackPosition(unsigned int esp, int pid);

void notifyEndOfProcess(int pid);

#endif /* KMALLOC_H_ */
