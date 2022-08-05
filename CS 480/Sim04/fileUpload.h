#ifndef FILEUPLOAD_H
#define FILEUPLOAD_H

#include "configops.h"
#include "metadataops.h"
#include "simtimer.h"
#include "DataTypes.h"
#include "simulator.h"

extern const char WRITE_ONLY_FLAG[];

void uploadConfigToFile( char *outputFile, PCB *headNode, Boolean logToFlag );

#endif
