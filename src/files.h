#ifndef FILES_H
#define FILES_H

void readFile(char* buffer, char* filename, int* sectorsRead);
void writeFile(char* buffer, char* filename, int numberOfSectors);

#endif

