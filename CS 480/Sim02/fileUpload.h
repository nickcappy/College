#ifndef FILEUPLOAD_H
#define FILEUPLOAD_H

#include "configops.h"
#include "metadataops.h"
#include "simtimer.h"
#include "DataTypes.h"
#include "simulator.h"



extern const char READ_ONLY_FLAG[];

void copyToFile( ConfigDataType* fileName, PCB *headNode,
                                                  OpCodeType *metaDataMstrPtr );
void uploadConfigToFile( char *outputFile, PCB *headNode,
                                                  OpCodeType *metaDataMstrPtr );

#endif
