#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#include "threadHead.hpp"
#include "enumDefines.hpp"
#include "potatoClass.hpp"
#include "semp.hpp"

#define programFailed 1
#define programSuccess 0

using namespace std;

const char helpText[] =
    "--------------------------------------------------------------------------------\n"
    "Command: -n sets the player count. must be more then 1 and less then 33\n"
    "Command: -p set the potato count. must be more then 1 and less then player count\n"
    "--------------------------------------------------------------------------------\n";



struct commandArgHandler
{

    commandArgHandler() = default;
    u_char helpArgFlag = defualtHelpFlagVal;
    char playerCount = defualtPlayerCount;
    char potatoCount = defualtPotatoCountVal;
};

int handleArgs(int argcInput, char **argvInput, commandArgHandler &commandArgHandlerInput)
{
    int getOptReturn = 0;
    char *optargCapture = nullptr;

    while ((getOptReturn = getopt(argcInput, argvInput, "n:p:h")) != GetOptEOV)
    {

        switch (getOptReturn)
        {
        case 'h':

            commandArgHandlerInput.helpArgFlag = helpFlagSet;

            break;
        case 'n':
            optargCapture = optarg;
            commandArgHandlerInput.playerCount = (char)atoi(optargCapture);
            break;
        case 'p':
            optargCapture = optarg;
            commandArgHandlerInput.potatoCount = (char)atoi(optargCapture);
            break;
        default:
            return unknownArgGiven;
            break;
        }
    }

    return handlerSuccessReturn;
}

int checkArgHandler(commandArgHandler commandArgHandlerInput)
{

    if (commandArgHandlerInput.playerCount < minPlayerCount)
    {

        return playerCountLessThen1Err;
    }
    if (commandArgHandlerInput.playerCount > maxPlayerCount)
    {

        return playerCountMoreThen32Err;
    }

    if (commandArgHandlerInput.potatoCount < minPotatoCount)
    {

        return potatoCountLessThen1Err;
    }
    if (commandArgHandlerInput.potatoCount > commandArgHandlerInput.playerCount)
    {

        return PotatoCountMoreThenPlayersErr;
    }

    return argStructPass;
}

int parentLoop(commandArgHandler commandArgHandlerInput)
{
    int mainLoopStatus = programSuccess;

    //allocate all heap memory for threads and parent

    pthread_t *threadIDs = new pthread_t[commandArgHandlerInput.playerCount];

    potatoClass *potatoManager = new potatoClass(commandArgHandlerInput.playerCount);

    threadArgHolder *threadArgs = new threadArgHolder[commandArgHandlerInput.playerCount];

    semp *semoHolder = new semp;

    //used for calculating the variance
    uint32_t currentFetchedValue = 0;

    uint32_t largestValue = 0;

    uint32_t smallestValue = UINT32_MAX;

    //sleep for 0.25 of a sec
    timespec updateRest = {
        .tv_sec = 0,
        .tv_nsec = 400000000

    };

    //init the semophore to whatever the potato count is
    SemInit(*semoHolder, commandArgHandlerInput.potatoCount);


    for (size_t i = 0; i < (size_t)commandArgHandlerInput.playerCount && !mainLoopStatus; i++)
    {//init every thread based on player count setted

     //give every thread its known ID, the pointer to potatoManager, and the pointer to the semophoreHolder
     //the last two are shared between all threads
        threadArgs[i].threadCountID = i;
        threadArgs[i].potatoC = potatoManager;
        threadArgs[i].semophoreHolder = semoHolder;

        if (pthread_create(&threadIDs[i], nullptr, threadEntry, &threadArgs[i]))
        {

            perror("problem with pthread_create() in parentLoop(): ");
            mainLoopStatus = programFailed;
        }
    }

    //the main infinite loop (only ended by a control c or a change in mainLoopStatus)
    while (!mainLoopStatus)
    {

        for (size_t i = 0; i < (size_t)commandArgHandlerInput.playerCount; i++)
        {

            currentFetchedValue = potatoManager->getCount(i);

            //based on fetched value if larger then current known largest or smallest
            //set the variale accordinly
            if (currentFetchedValue > largestValue)
            {

                largestValue = currentFetchedValue;
            }

            if (currentFetchedValue < smallestValue)
            {

                smallestValue = currentFetchedValue;
            }
            //see if current thread holds a potato
            if (potatoManager->currentlyHoldsPotato(i))
            {

                cout << '*';
            }

            cout << currentFetchedValue << " ";
        }

        //calculate the variance
        cout << "variance: " << ((float)(largestValue - smallestValue) / (float)largestValue) * 100.0 << '\r' << flush;

        smallestValue = UINT32_MAX;

        //sleep for some time to slow down the updating of values
        if (nanosleep(&updateRest, &updateRest) == -1)
        {

            perror("failure point with nanosleep(): ");

            mainLoopStatus = programFailed;
        }

        //check to see if every thread is still alive
        for (size_t i = 0; i < (size_t)commandArgHandlerInput.playerCount; i++)
        {
            
            if(!potatoManager->geThreadState(i)){
                
                cerr << endl << "thread error: " << i << endl;

                mainLoopStatus = programFailed;

            }

        }
        
    }

    // clean up threads and deallocate memory
    for (size_t i = 0; i < (size_t)commandArgHandlerInput.playerCount; i++)
    {
        //kill the threadState of every thread so they will end
        for (size_t i = 0; i < (size_t)commandArgHandlerInput.playerCount; i++)
        {
            
            potatoManager->setThreadState(i,false);

        }

        //wait for every thread to join 
        if (pthread_join(threadIDs[i], nullptr))
        {

            perror("problem with pthread_join() in parentLoop(): ");
            mainLoopStatus = programFailed;
        }
    }

    delete[] threadIDs;

    delete potatoManager;

    delete semoHolder;

    delete[] threadArgs;

    return mainLoopStatus;
}

int main(int argc, char *argv[])
{

    int errReturnCode = 0;

    //function status if it ever goes above 0 ignore every function and reach end of program
    int mainFunctionReturnStatus = programSuccess;

    commandArgHandler programArguStruct;
    //handle user input

    if ((errReturnCode = handleArgs(argc, argv, programArguStruct)))
    {

        if (errReturnCode == unknownArgGiven)
        {

            mainFunctionReturnStatus = programFailed;
        }
    }

    //parse input to ensure nothing incorrect
    if ((errReturnCode = checkArgHandler(programArguStruct)) && !mainFunctionReturnStatus)
    {

        switch (errReturnCode)
        {
        case playerCountLessThen1Err:
            cerr << "ERROR: player count can not be less then 1" << endl;
            mainFunctionReturnStatus = programFailed;
            break;
        case playerCountMoreThen32Err:
            cerr << "ERROR: player count can not be more then 32 players" << endl;
            mainFunctionReturnStatus = programFailed;
            break;
        case potatoCountLessThen1Err:
            cerr << "ERROR: potato count can not be less then 1" << endl;
            mainFunctionReturnStatus = programFailed;
            break;
        case PotatoCountMoreThenPlayersErr:
            cerr << "ERROR: potato count can not be more then player count" << endl;
            mainFunctionReturnStatus = programFailed;
            break;
        }
    }

    //if help command was given print out and end program
    //other begin main program loop
    if (programArguStruct.helpArgFlag == helpFlagSet && !mainFunctionReturnStatus)
    {

        cout << helpText << endl;
        
    }
    else
    {

        if (parentLoop(programArguStruct) && !mainFunctionReturnStatus)
        {

            mainFunctionReturnStatus = programFailed;
        }
    }

    return mainFunctionReturnStatus;
}