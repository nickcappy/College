// header files
#include "fileUpload.h"

void uploadConfigToFile( char *outputFile, PCB *headNode, Boolean logToFlag )
{
  // initialize variables
  FILE *fileAccessPtr;
  OpCodeType *currentLine;
  Boolean previousRun = False;
  double timer;
  char timeInStr[ 12 ];

  if( logToFlag == True )
  {
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
        accessTimer( LAP_TIMER, timeInStr );

        // print line at top
        fprintf(fileAccessPtr, "\n========================================\n" );

        // print title
        fprintf( fileAccessPtr, "Simulator Log File Header\n" );

        // use fprintf to copy lines to a file
        // calculate current time remaining in the current process
           // "time left in process"
        timer = accessTimer( LAP_TIMER, timeInStr );

        // Display OS Start
        fprintf( fileAccessPtr, "%f, OS: Simulator Start\n", timer);

        // set all processes to ready from new
        while ( headNode->nextNode != NULL )
        {
          // calculate current time remaining in the current process
             // "time left in process"
          timer = accessTimer( LAP_TIMER, timeInStr );

          // set process states to ready
          headNode->processState = READY_STATE;

          // displays which process is set to ready from new
          fprintf( fileAccessPtr,
                          "%f OS: Process %d set to READY state from NEW state\n",
                                            timer, headNode->pid );

          // gets the current time in the timer
          timer = accessTimer( LAP_TIMER, timeInStr );

          // display how much ms remaining in first process
          fprintf( fileAccessPtr, "%f OS: Process %d has %d msec remaining\n",
                                          timer, headNode->pid,
                                          headNode->msRemaining );

          // set from ready to running
          headNode->processState = RUNNING_STATE;

          // loop through the metadata till app end from pointer in pcb
          while( compareString(currentLine->strArg1, "end") != STR_EQ )
          {
            // check for cpu process
            if( compareString(currentLine->command, "cpu") == STR_EQ )
            {
             accessTimer(LAP_TIMER, timeInStr);
             fprintf( fileAccessPtr, "%s, Process %d, cpu process operation start\n",
                      timeInStr, headNode->pid );
             accessTimer(LAP_TIMER, timeInStr);
             fprintf( fileAccessPtr, "%s, Process %d, cpu process operation end\n",
                      timeInStr, headNode->pid );
            }

            // check for io process
            if( compareString(currentLine->command, "dev") == STR_EQ )
            {
            // check if in
            if ( compareString(currentLine->inOutArg, "in") == STR_EQ )
              {
              // check for hard drive, sound signal, video signal, keyboard
              if( compareString(currentLine->strArg1, "hard drive") == STR_EQ )
                {
                accessTimer(LAP_TIMER, timeInStr);
                fprintf( fileAccessPtr, "%s, Process %d, hard drive input operation start\n",
                                                       timeInStr, headNode->pid);

                accessTimer(LAP_TIMER, timeInStr);
                fprintf( fileAccessPtr, "%s, Process %d, hard drive input operation end\n",
                                                       timeInStr, headNode->pid);
                }

              if( compareString(currentLine->strArg1, "sound signal") == STR_EQ )
                 {
                 accessTimer(LAP_TIMER, timeInStr);
                 fprintf( fileAccessPtr, "%s, Process %d, sound signal input operation start\n",
                                                         timeInStr, headNode->pid);

                 accessTimer(LAP_TIMER, timeInStr);
                 fprintf( fileAccessPtr, "%s, Process %d, sound signal input operation end\n",
                                                         timeInStr, headNode->pid);
                 }

              if( compareString(currentLine->strArg1, "video signal") == STR_EQ )
                 {
                 accessTimer(LAP_TIMER, timeInStr);
                 fprintf( fileAccessPtr, "%s, Process %d, video signal input operation start\n",
                                                         timeInStr, headNode->pid);

                 accessTimer(LAP_TIMER, timeInStr);
                 fprintf( fileAccessPtr, "%s, Process %d, video signal input operation end\n",
                                                         timeInStr, headNode->pid);
                 }

              if( compareString(currentLine->strArg1, "keyboard") == STR_EQ )
                 {
                 accessTimer(LAP_TIMER, timeInStr);
                 fprintf( fileAccessPtr, "%s, Process %d, keyboard input operation start\n",
                                                         timeInStr, headNode->pid);

                 accessTimer(LAP_TIMER, timeInStr);
                 fprintf( fileAccessPtr, "%s, Process %d, keyboard input operation end\n",
                                                         timeInStr, headNode->pid);
                 }
              }
            // check for out
            if ( compareString(currentLine->inOutArg, "out") == STR_EQ )
              {
              // check for monitor, usb, video signal, sound signal, serial
              if( compareString(currentLine->strArg1, "monitor") == STR_EQ )
                 {
                 accessTimer(LAP_TIMER, timeInStr);
                 fprintf( fileAccessPtr, "%s, Process %d, monitor output operation start\n",
                                                         timeInStr, headNode->pid);

                 accessTimer(LAP_TIMER, timeInStr);
                 fprintf( fileAccessPtr, "%s, Process %d, monitor output operation end\n",
                                                         timeInStr, headNode->pid);
                 }

              if( compareString(currentLine->strArg1, "usb") == STR_EQ )
                 {
                 accessTimer(LAP_TIMER, timeInStr);
                 fprintf( fileAccessPtr, "%s, Process %d, usb output operation start\n",
                                                         timeInStr, headNode->pid);

                 accessTimer(LAP_TIMER, timeInStr);
                 fprintf( fileAccessPtr, "%s, Process %d, usb output operation end\n",
                                                         timeInStr, headNode->pid);
                 }

              if( compareString(currentLine->strArg1, "video signal") == STR_EQ )
                 {
                 accessTimer(LAP_TIMER, timeInStr);
                 fprintf( fileAccessPtr, "%s, Process %d, video signal output operation start\n",
                                                         timeInStr, headNode->pid);

                 accessTimer(LAP_TIMER, timeInStr);
                 fprintf( fileAccessPtr, "%s, Process %d, video signal output operation end\n",
                                                         timeInStr, headNode->pid);
                 }

              if( compareString(currentLine->strArg1, "sound signal") == STR_EQ )
                 {
                 accessTimer(LAP_TIMER, timeInStr);
                 fprintf( fileAccessPtr, "%s, Process %d, sound signal output operation start\n",
                                                         timeInStr, headNode->pid);

                 accessTimer(LAP_TIMER, timeInStr);
                 fprintf( fileAccessPtr, "%s, Process %d, sound signal output operation end\n",
                                                         timeInStr, headNode->pid);
                 }

              if( compareString(currentLine->strArg1, "serial") == STR_EQ )
                 {
                 accessTimer(LAP_TIMER, timeInStr);
                 fprintf( fileAccessPtr, "%s, Process %d, serial output operation start\n",
                                                         timeInStr, headNode->pid);

                 accessTimer(LAP_TIMER, timeInStr);
                 fprintf( fileAccessPtr, "%s, Process %d, serial output operation end\n",
                                                         timeInStr, headNode->pid);
                 }
              }
            }

            // new line for after the opcodes
            currentLine = currentLine -> nextNode;
          }

          // display end of simulation
          fprintf( fileAccessPtr, "\n%f, OS: Simulation end\n", timer);


          // close file
          fclose( fileAccessPtr );
          }
        }
      }
  }
}
