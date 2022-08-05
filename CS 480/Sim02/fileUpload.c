// header files
#include "fileUpload.h"


void copyToFile( ConfigDataType* fileName, PCB *workingNode,
                                                  OpCodeType *metaDataMstrPtr )
{
  // check if file was was successfully opened
  if( fileName != NULL )
  {
    // display message that data is being copied to a file
    printf("\nSimulator running with output to file\n" );

    // display config data
    uploadConfigToFile( fileName->logToFileName, workingNode, metaDataMstrPtr );
  }
}


void uploadConfigToFile( char *outputFile, PCB *headNode,
                                                  OpCodeType *metaDataMstrPtr )
{
  // initialize variables
  FILE *fileAccessPtr;
  PCB *workingNode;
  Boolean previousRun = False;
  double timer;
  char timeInStr[ 12 ];

  workingNode = headNode;

  // checks to see if function was previously ran
  if( previousRun != True )
  {
    // set flag to True
    previousRun = True;

    // open write only file
    fileAccessPtr = fopen( outputFile, WRITE_ONLY_FLAG );

    // check to see if file upload was successful
    if( fileAccessPtr != NULL )
    {
      // run timer


      // print line at top
      fprintf( fileAccessPtr, "\n========================================\n" );

      // print title
      fprintf( fileAccessPtr, "Simulator Log File Header\n" );

      // use ffprintf to copy lines to a file
      // calculate current time remaining in the current process
         // "time left in process"
      timer = accessTimer( LAP_TIMER, timeInStr );

      // Display OS Start
      fprintf( fileAccessPtr, "%f, OS: Simulator Start\n", timer);

      // set all processes to ready from new
      while ( workingNode->nextNode != NULL )
      {
        // calculate current time remaining in the current process
           // "time left in process"
        timer = accessTimer( LAP_TIMER, timeInStr );

        // set process states to ready
        workingNode->processState = READY_STATE;

        // displays which process is set to ready from new
        fprintf( fileAccessPtr,
                        "%f OS: Process %d set to READY state from NEW state\n",
                                                      timer, workingNode->pid );

        // iterates to next node
        workingNode = workingNode->nextNode;

        // gets the current time in the timer
        timer = accessTimer( LAP_TIMER, timeInStr );

        // display how much ms remaining in first process
        fprintf( fileAccessPtr, "%f OS: Process %d has %d msec remaining\n",
                            timer, workingNode->pid, workingNode->msRemaining );

        // set from ready to running
        workingNode->processState = RUNNING_STATE;

        // loop through entire app start
        while( compareString( metaDataMstrPtr->command, "sys" ) != STR_EQ
                && compareString( metaDataMstrPtr->strArg1, "end" ) != STR_EQ)
        {
          // check if mem allocate
          if( compareString( metaDataMstrPtr->command, "mem" ) == STR_EQ )
          {
            fprintf( fileAccessPtr,
                "%f, Process: %d, attempting memory allocate request\n", timer,
                                                            workingNode->pid );
          }

          // check if cpu
          if( compareString( metaDataMstrPtr->command, "cpu" ) == STR_EQ )
          {
            fprintf( fileAccessPtr,
                              "%f, Process: %d, cpu process operation start\n",
                                                      timer, workingNode->pid );

            fprintf( fileAccessPtr,
                                "%f, Process: %d, cpu process operation end\n",
                                                      timer, workingNode->pid );
          }

          // check if dev
          if( compareString( metaDataMstrPtr->command, "dev" ) == STR_EQ )
          {
            // check if dev in
            if( compareString( metaDataMstrPtr->inOutArg, "in" ) == STR_EQ )
            {
              // checks for sound signal
              if( compareString( metaDataMstrPtr->strArg1, "sound signal" ) ==
                                                                        STR_EQ )
              {
                fprintf( fileAccessPtr,
                        "%f, Process: %d, sound signal input operation start\n",
                                                      timer, workingNode->pid );

                fprintf( fileAccessPtr,
                          "%f, Process: %d, sound signal input operation end\n",
                                                      timer, workingNode->pid );
              }

              // chekcs for hard drive
              if ( compareString( metaDataMstrPtr->strArg1, "hard drive" ) ==
                                                                        STR_EQ )
              {
                fprintf( fileAccessPtr,
                          "%f, Process: %d, hard drive input operation start\n",
                                                      timer, workingNode->pid );

                fprintf( fileAccessPtr,
                            "%f, Process: %d, hard drive input operation end\n",
                                                      timer, workingNode->pid );
              }

              // checks for video signal
              if ( compareString( metaDataMstrPtr->strArg1, "video signal" ) ==
                                                                        STR_EQ )
              {
                fprintf( fileAccessPtr,
                        "%f, Process: %d, video signal input operation start\n",
                                                      timer, workingNode->pid );

                fprintf( fileAccessPtr,
                          "%f, Process: %d, video signal input operation end\n",
                                                      timer, workingNode->pid );
              }
            }

            // check if dev out
            if( compareString( metaDataMstrPtr->inOutArg, "out" ) == STR_EQ )
            {
              // checks for monitor
              if( compareString( metaDataMstrPtr->strArg1, "monitor" ) ==
                                                                        STR_EQ )
              {
                fprintf( fileAccessPtr,
                            "%f, Process: %d, monitor output operation start\n",
                                                      timer, workingNode->pid );

                fprintf( fileAccessPtr,
                              "%f, Process: %d, monitor output operation end\n",
                                                      timer, workingNode->pid );
              }

              // checks for  usb
              if (compareString( metaDataMstrPtr->strArg1, "usb" ) == STR_EQ )
              {
                fprintf( fileAccessPtr,
                                "%f, Process: %d, usb output operation start\n",
                                                      timer, workingNode->pid );

                fprintf( fileAccessPtr,
                                  "%f, Process: %d, usb output operation end\n",
                                                      timer, workingNode->pid );
              }

              // checks for  serial
              if ( compareString( metaDataMstrPtr->strArg1, "serial" ) ==
                                                                        STR_EQ )
              {
                fprintf( fileAccessPtr,
                            "%f, Process: %d, serial output operation start\n",
                                                      timer, workingNode->pid );

                fprintf( fileAccessPtr,
                              "%f, Process: %d, serial output operation end\n",
                                                      timer, workingNode->pid );
              }
            }
          }

          // display current process has ended
          fprintf( fileAccessPtr, "%f OS: Process %d ended\n", timer,
                                                            workingNode->pid );

          // set current proccess to end state
          workingNode->processState = EXIT_STATE;

          // set current process to exit
          fprintf( fileAccessPtr, "%f OS: Process %d set to EXIT\n", timer,
                                                            workingNode->pid );

          // go to next process
          workingNode= workingNode->nextNode;
        }

        // display end of simulation
        fprintf( fileAccessPtr, "\n%f, OS: Simulation end\n", timer);


        // close file
        fclose( fileAccessPtr );
        }
      }
    }
  }
