// protect from multiple compiling
#ifndef METADATAOPS_H
#define METADATAOPS_H

// header files
#include "StringUtils.h"
#include "DataTypes.h"

// constants
// op code messages, starts at index 3 to allow for string error messages
typedef enum { MD_FILE_ACCESS_ERR = 3,
               MD_CORRUPT_DESCRIPTOR_ERR,
               OPCMD_ACCESS_ERR,
               CORRUPT_OPCMD_ERR,
               CORRUPT_OPCMD_ARG_ERR,
               UNBALANCED_START_END_ERR,
               COMPLETE_OPCMD_FOUND_MSG,
               LAST_OPCMD_FOUND_MSG } OpCodeMessages;

extern const int BAD_ARG_VAL;

// functions
OpCodeType *addNode( OpCodeType *localPtr, OpCodeType *newNode );
OpCodeType *clearMetaDataList( OpCodeType *localPtr );
void displayMetaData( OpCodeType *localPtr );
Boolean getMetaData( char *fileName,
                              OpCodeType **opCodeDataHead, char *endStateMsg );
int getOpCommand( FILE *filePtr, OpCodeType *inData );
int updateStartCount( int count, char *opString );
int updateEndCount( int count, char *opString );
Boolean verifyValidCommand( char *testCmd );
int getStringArg( char *strArg, char *inputStr, int index );
Boolean verifyFirstStringArg( char *strArg );
int getNumberArg( int *number, char *inputStr, int index );
int getCommand( char *cmd, char *inputStr, int index );
Boolean verifyFirstStringArg( char *strArg );
Boolean isDigit( char testChar );

#endif
