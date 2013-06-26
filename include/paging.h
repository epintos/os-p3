#ifndef PAGING_H_
#define PAGING_H_

void setUpPaging(void);

unsigned int alignEntry(unsigned int entry);

void createPage(void* addr, int perms);

#endif
