// header file
#include "configops.h"

/*
Function name: clearConfigData
Algorithm: frees all allocated memory for config data
Precondition: configData holds allocated data set
Postcondition: memory is freed, pointer is set to null
Exceptions: none
Notes: none
*/
ConfigDataType *clearConfigData( ConfigDataType *configData)
  {
    // check that config data pointer is not NULL
    if( configData != NULL )
    {
      // free data structure memory
        // function: free
      free( configData );

      // set config data to NULL
      configData = NULL;
    }

    // set config data pointer to null (returned as parameter)
    return NULL;
  }

/*
Function name: displayConfigData
Algorithm: diagnostic function to show config data output
Precondition: configData holds allocated data set
Postcondition: data is printed to screen
Exceptions: none
Notes: none
*/
void displayConfigData( ConfigDataType *configData )
  {
    // initialize function/ variables
    char displayString[ STD_STR_LEN ];

    // print lines of display
      // function: printf, configCodeToString (translates coded items)
    printf("Config File Display\n");
    printf("-------------------\n");
    printf("Version:              : %3.2f\n", configData->version);
    printf("Program file name     : %s\n", configData->metaDataFileName);
    configCodeToString( configData->cpuSchedCode, displayString);
    printf("CPU schedule selection: %s\n", displayString);
    printf("Quantum time          : %d\n", configData->quantumCycles);
    printf("Memory available      : %d\n", configData->memAvailable);
    printf("Process cycle rate    : %d\n", configData->procCycleRate);
    printf("I/O cycle rate        : %d\n", configData->ioCycleRate);
    configCodeToString( configData->logToCode, displayString);
    printf("Log to selection      : %s\n", displayString);
    printf("Log file name         : %s\n", configData->logToFileName);
  }

/*
Function name: getConfigData
Algorithm: opens file, acquires configuration data,
           returns pointer to data structure
Precondition: for correct operation, file is available, is formatted correctly,
              and has all configuration lines and data although
              the configuration lines are not required to be
              in a specific order
Postcondition: in correct operation,
               returns pointer in correct configuration data structure
Exceptions: correctly and appropriately (without program failure)
            responds to and reports file access failure,
            incorrectly formatted lead or end descriptors,
            incorrectly formattedprompt, data out of range,
            and incomplete file conditions
Notes: none
*/
Boolean getConfigData( char*fileName,
                                 ConfigDataType **configData, char *endStateMsg)
  {
  // initialze function/ variables

      // set constant number of data lines
      const int NUM_DATA_LINES = 9;

      // set read only constant
      const char READ_ONLY_FLAG[] = "r";

      // create pointer for data input
      ConfigDataType *tempData;

      // declare other variables
      FILE *fileAccessPtr;
      char dataBuffer[ MAX_STR_LEN ];
      int intData, dataLineCode, lineCtr = 0;
      double doubleData;
      Boolean dontStopAtNonPrintable = True;

    // set endStateMsg to success
      // function: copyString
    copyString( endStateMsg, "configuration file upload successful");

    // initialze config data pointer to success in case of return error
    *configData = NULL;

    // open file
      // function: fopen
    fileAccessPtr = fopen(fileName, READ_ONLY_FLAG);

    // check for file open failure
    if (fileAccessPtr == NULL)
    {
      // set end state message to config file access error
        // function: copyString
      copyString( endStateMsg, "Configuration file access error");

      // return file access failure
      return False;
    }

    // get first line, check for failure
      // function: getLineTo
    if( getLineTo(fileAccessPtr, MAX_STR_LEN, COLON,
      dataBuffer, IGNORE_LEADING_WS, dontStopAtNonPrintable ) != NO_ERR
        || compareString( dataBuffer, "Start Simulator Configuration File")
                                                                    != STR_EQ )
    {
      // close file access
        // function: fclose
      fclose(fileAccessPtr);

      // set end state message to corrupt leader line error
        // function: copyString
      copyString(endStateMsg, "Corrupt configuration leader line error");

      // return corrupt file data
      return False;
    }

    // create temporary pointer to configuration data structure
    tempData = (ConfigDataType *) malloc( sizeof( ConfigDataType ) );

    // loop to end of config data items
    while( lineCtr < NUM_DATA_LINES )
    {
      // get line leader, check for failure
      // function: getLineTo
      if( getLineTo( fileAccessPtr, MAX_STR_LEN, COLON,
        dataBuffer, IGNORE_LEADING_WS, dontStopAtNonPrintable ) != NO_ERR )
        {
        // free temp struct memory
          // function: free
        free( tempData );

        // close file access
          // function: fclose
        fclose( fileAccessPtr );

        // set end state message to line capture failure
          // function: copyString
        copyString( endStateMsg, "Configuration start line capture error" );

        // return incomplet efile error
        return False;
        }

      // find correct data line by number
        // function: getDataLineCode
      dataLineCode = getDataLineCode( dataBuffer );

      // check for data line found
      if( dataLineCode != CFG_CORRUPT_PROMPT_ERR )
      {
        // get value

          // check for version number (double value)
          if( dataLineCode == CFG_VERSION_CODE)
          {
            // get version number
              // function: fscanf
            fscanf( fileAccessPtr, "%lf", &doubleData );
          }
          // otherwise, check for metaData or LogTo file names
          //  or CPU Scheduling names (strings)
          else if( dataLineCode == CFG_MD_FILE_NAME_CODE
                    || dataLineCode == CFG_LOG_FILE_NAME_CODE
                      || dataLineCode == CFG_CPU_SCHED_CODE
                        || dataLineCode == CFG_LOG_TO_CODE )
          {
            // get string input
              // function: fscanf
            fscanf( fileAccessPtr, "%s", dataBuffer );
          }

          // otherwise, assume integer data
          else
          {
            // get integer input
              // function: fscanf
            fscanf( fileAccessPtr, "%d", &intData );
          }

        // check for data value in range
          // function: valueInRange
        if( valueInRange( dataLineCode, intData, doubleData, dataBuffer)
                                                                       == True )
          {
          // assign to data pointer depending in config item
            // function: getCpuSchedCode, getLogToCode
          switch ( dataLineCode )
            {
              case CFG_VERSION_CODE:
                tempData->version = doubleData;
                break;

              case CFG_MD_FILE_NAME_CODE:
                copyString( tempData->metaDataFileName, dataBuffer);
                break;

              case CFG_CPU_SCHED_CODE:
                tempData->cpuSchedCode = getCpuSchedCode( dataBuffer );
                break;

              case CFG_QUANT_CYCLES_CODE:
                tempData->quantumCycles = intData;
                break;

              case CFG_MEM_AVAILABLE_CODE:
                tempData->memAvailable = intData;
                break;

              case CFG_PROC_CYCLES_CODE:
                tempData->procCycleRate = intData;
                break;

              case CFG_IO_CYCLES_CODE:
                tempData->ioCycleRate = intData;
                break;

              case CFG_LOG_TO_CODE:
                tempData->logToCode = getLogToCode( dataBuffer );
                break;

              case CFG_LOG_FILE_NAME_CODE:
                copyString( tempData->logToFileName, dataBuffer );
                break;
            }
          }

        // otherwise, assume data value not in range
          else
          {
            // free temp struct memory
              // function: free
            free( tempData );

            // close file access
              // function: fclose
            fclose( fileAccessPtr );

            // set end state message to configuration data out of range
              // function: copyString
            copyString( endStateMsg, "Configuration item out of range" );

            // return data out of range
            return False;
          }
      }
      // otherwise, assume data line not found
      else
        {
        // free temp struct memory
          // function: free
        free( tempData );

        // close file access
          // function: fclose
        fclose( fileAccessPtr );

        // set end state message to corrupt prompt error
          // function: copyString
        copyString( endStateMsg, "Corrupted configuration prompt" );

        // return corrupt config file code
        return False;
        }

    // increment line counter
    lineCtr++;
  }
  // end master loop

  // acquire end of sim config string
    // function: getLineTo
    if( getLineTo(fileAccessPtr, MAX_STR_LEN, PERIOD,
      dataBuffer, IGNORE_LEADING_WS, dontStopAtNonPrintable ) != NO_ERR
        || compareString( dataBuffer, "End Simulator Configuration File")
                                                                    != STR_EQ )
    {
      // free temp struct memory
        // function: free
      free( tempData );

      // close file access
        // function: fclose
      fclose( fileAccessPtr );

      // set end state message to corrupt configuration end of line
        // function: copyString
      copyString( endStateMsg, "Configuration end line capture error" );

      // return corrupt file data
      return False;
    }

  // assign temporary pointer to parameter return pointer
  *configData = tempData;

  // close file access
    // function: fclose
  fclose( fileAccessPtr );

  // return no error code
  return True;
  }

/*
Function name: configCodeToString
Algorithm: utility function to support display
          of CPU schedulung or Log To code strings
Precondition: code variable holds constant value from ConfigDataCodes
              for item (e.g., CPU_SCHED_SPRT_P, LOGTO_FILE_CODE, etc.)
Postcondition: string parameter holds correct string
               associated with the given constant
Exceptions: none
Notes: none
*/
void configCodeToString( int code, char *outString)
  {
    // define array with eight items, and short (10) lengths
    char displayStrings[ 8 ][ 10 ] = { "SJF-N", "SRTF-P", "FCFS-P",
                                       "RR-P", "FCFS-N", "Monitor",
                                        "File", "Both" };

    // copy string to return parameters
      // function: copyString
    copyString( outString, displayStrings[ code ] );
  }

/*
Function name: getDataLineCode
Algorithm: tests stringn for one known leader strings, returns line number
           if string is correct, returns CFG_CORRUPT_DATA_ERR
           if string is not found
Precondition: dataBuffer is valid C-style string
Postcondition: returns line number of data item in terms of constant
               (e.g., CFG_VERSION_CODE, CFG_CPU_SCHED_CODE, etc.)
Exceptions: returns CFG_CORRUPT_FILE_ERR if string is not identified
Notes: none
*/
int getDataLineCode( char *dataBuffer)
  {
    // return appropriate code depending on prompt string provided
      // function: compareString
    if( compareString( dataBuffer, "Version/Phase" ) == STR_EQ)
    {
      return CFG_VERSION_CODE;
    }

    if( compareString( dataBuffer, "File Path" ) == STR_EQ)
    {
      return CFG_MD_FILE_NAME_CODE;
    }

    if( compareString( dataBuffer, "CPU Scheduling Code" ) == STR_EQ)
    {
      return CFG_CPU_SCHED_CODE;
    }

    if( compareString( dataBuffer, "Quantum Time (cycles)" ) == STR_EQ)
    {
      return CFG_QUANT_CYCLES_CODE;
    }

    if( compareString( dataBuffer, "Memory Available (KB)" ) == STR_EQ)
    {
      return CFG_MEM_AVAILABLE_CODE;
    }

    if( compareString( dataBuffer, "Processor Cycle Time (msec)" ) == STR_EQ)
    {
      return CFG_PROC_CYCLES_CODE;
    }

    if( compareString( dataBuffer, "I/O Cycle Time (msec)" ) == STR_EQ)
    {
      return CFG_IO_CYCLES_CODE;
    }

    if( compareString( dataBuffer, "Log To" ) == STR_EQ)
    {
      return CFG_LOG_TO_CODE;
    }

    if( compareString( dataBuffer, "Log File Path" ) == STR_EQ)
    {
      return CFG_LOG_FILE_NAME_CODE;
    }

    return CFG_CORRUPT_PROMPT_ERR;
  }

/*
Function name: valueInRange
Algorithm: tests one of three values (int, double, string) for being
           in specified range, depending on data code
           (i.e., specified config value)
Precondition: one o fthe three data values in valid
Postcondition: returns True if data is within specified parameters,
               False otherwise
Exceptions: metadata or logfile names are ignored and return True
Notes: none
*/
Boolean valueInRange( int lineCode, int intVal,
                                          double doubleVal, char *stringVal)
  {
    // initialze function/ variables
    Boolean result = True;
    char *tempStr;
    int strLen;

    // use in line code to identify prompt line
    switch( lineCode )
    {
      // for version code
      case CFG_VERSION_CODE:

        // check if limits of version code are exceeded
        if( doubleVal < 0.00 || doubleVal > 10.00 )
          {
          // set Boolean result to false
          result = False;
          }
        break;

      // for cpu scheduling code
      case CFG_CPU_SCHED_CODE:

        // create temporary string and set to lower case
          // function: getStringLength, malloc, setStrToLowerCase
        strLen = getStringLength( stringVal );
        tempStr = (char *) malloc( sizeof( strLen + 1 ) );
        setStrToLowerCase( tempStr, stringVal );

        // check for not finding one of the scheduling strings
          // function: compareString
        if (compareString( tempStr, "fcfs-n" ) != STR_EQ
            && compareString( tempStr, "sjf-n" ) != STR_EQ
            && compareString( tempStr, "srtf-p" ) != STR_EQ
            && compareString( tempStr, "fcfs-p" ) != STR_EQ
            && compareString( tempStr, "rr-p" ) != STR_EQ )
          {
            // set Boolean result to false
          result = False;
          }

        // free temp string memory
          // function: free
        free( tempStr );

        break;

        // for quantum cycles
        case CFG_QUANT_CYCLES_CODE:

          // check for quantum cycles exceeded
          if( intVal < 0 || intVal > 100 )
            {
              // set Boolean results to false
              result = False;
            }
          break;

        // for memory available
        case CFG_MEM_AVAILABLE_CODE:

          // check for available memory limits exceeded
          if( intVal < 1024 || intVal > 102400)
          {
            // set Boolean results to false
            result = False;
          }
          break;

        // for process cycles
        case CFG_PROC_CYCLES_CODE:

          // check for process cycles limit exceeded
          if( intVal < 1 || intVal > 100)
          {
            // set Boolean results to false
            result = False;
          }
          break;

        // for I/O cycles
        case CFG_IO_CYCLES_CODE:
          // check for I/O cycles limit exceeded
          if( intVal < 1 || intVal > 1000 )
          {
            // set Boolean results to false
            result = False;
          }
          break;

        // check for log to operation
        case CFG_LOG_TO_CODE:

          // create temporary string and set to lower case
            // function: getStringLength, malloc, setStrToLowerCase
          strLen = getStringLength( stringVal );
          tempStr = (char *) malloc( sizeof( strLen + 1 ) );
          setStrToLowerCase( tempStr, stringVal );

          // check for not finding one of the log to strings
            // function: compareString
          if( compareString( tempStr, "both" ) != STR_EQ
              && compareString( tempStr, "monitor" ) != STR_EQ
              && compareString( tempStr, "file" ) != STR_EQ )
          {
            // set Boolean to False
            result = False;
          }
          // free temp string memory
            // function: free
          free( tempStr );
          break;
    }

    // return results of limit analysis
    return result;
  }

/*
Function name: getCpuSchedCode
Algorithm: converts string data (e.g., "SJF-N", "SRTF-P")
           to constant code number to be stored as integer
Precondition: codeStr is a C-style string with one
              of the specified cpu scheduling operations
Postcondition: returns code representing scheduling actions
Exceptions: defaults to fcfs code
Notes: none
*/
ConfigDataCodes getCpuSchedCode( char *codeStr )
  {
    // initialze function/ variables

      // set up temporary string for testing
        // function: getStringLength, malloc
      int strLen = getStringLength( codeStr );
      char *tempStr = (char *) malloc( sizeof( strLen + 1 ) );

      // set default return to FCFS-N
      int returnVal = CPU_SCHED_FCFS_N_CODE;

    // set lower case for testing
      // function: setStrToLowerCase
    setStrToLowerCase( tempStr, codeStr );

    // check for SJF-N
      // function: compareString
    if( compareString( tempStr, "sjf-p" ) == STR_EQ )
    {
      // set return code to sfj
      returnVal = CPU_SCHED_SJF_N_CODE;
    }

    // check for SRTF-P
      // function: compareString
    if( compareString( tempStr, "srtf-p" ) == STR_EQ )
    {
      // set return code to srtf
      returnVal = CPU_SCHED_SRTF_P_CODE;
    }

    // check for FCFS-P
      // function: compareString
    if( compareString( tempStr, "fcfs-p" ) == STR_EQ )
    {
      // set return code to fcfs
      returnVal = CPU_SCHED_FCFS_P_CODE;
    }

    // check for RR-P
      // function: compareString
    if( compareString( tempStr, "rr-p" ) == STR_EQ )
    {
      // set return code to rr
      returnVal = CPU_SCHED_RR_P_CODE;
    }

    // free temp string memory
      // function: free
    free( tempStr );
    // return resulting value
    return returnVal;
  }

/*
Function name: getLogToCode
Algorithm: converts string data (e.g., "File", "Monitor)
           to constant code number to be stored as integer
Precondition: logToStr is a C-style string with one
              of the specified cpu scheduling operations
Postcondition: returns code representing log to actions
Exceptions: defaults to monitor code
Notes: none
*/
ConfigDataCodes getLogToCode( char *logToStr )
  {
    // initialze function/ variables

      // create temporary string
        // function: getStringLength, malloc
      int strLen = getStringLength( logToStr );
      char *tempStr = (char *) malloc( sizeof( strLen + 1) );

      // set default log to monitor
      int returnVal = LOGTO_MONITOR_CODE;

    // set temp string to lower case
      // function: setStrToLowerCase
    setStrToLowerCase( tempStr, logToStr );

    // check for BOTH
      // function: compareString
    if( compareString( tempStr, "both" ) == STR_EQ)
    {
      // set return value to both code
      returnVal = LOGTO_BOTH_CODE;
    }

    // check for FILE
      // function: compareString
    if( compareString( tempStr, "file" ) == STR_EQ)
    {
      // set return value to file code
      returnVal = LOGTO_FILE_CODE;
    }
    // free temp string memory
      // function: free
    free( tempStr );

    // return found code
    return returnVal;
  }
