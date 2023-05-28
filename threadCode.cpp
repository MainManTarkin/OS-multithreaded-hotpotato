#include "threadHead.hpp"

void *threadEntry(void *arg)
{
    //cast thread input arguments
    threadArgHolder *localThreadArg = (threadArgHolder *)arg;

    timespec potatoHoldTime = {

        .tv_sec = 0,
        .tv_nsec = 0

    };

    srand(time(0));//seed random number gen

    //while loop is kept going by thread state which by defualt is true
    while (localThreadArg->potatoC->geThreadState(localThreadArg->threadCountID))
    {   //set how long to hold a potato
        potatoHoldTime.tv_nsec = (rand() + 1000000) % 50000000;

        //aqcuire a potato
        SemWait(*localThreadArg->semophoreHolder);

        //increment the count and set holdPotato state
        localThreadArg->potatoC->incrementCount(localThreadArg->threadCountID);

        localThreadArg->potatoC->heldPotato(localThreadArg->threadCountID, true);

        //sleep for a random amount of time with potato
        if(nanosleep(&potatoHoldTime, &potatoHoldTime) == -1){

            //localThreadArg->potatoC->setThreadState(localThreadArg->threadCountID, false);

        }

        //give up potatoState
        localThreadArg->potatoC->heldPotato(localThreadArg->threadCountID, false);

        //release potato
        SemPost(*localThreadArg->semophoreHolder);

        if(nanosleep(&potatoHoldTime, &potatoHoldTime) == -1){

            //localThreadArg->potatoC->setThreadState(localThreadArg->threadCountID, false);

        }

    }

    return nullptr;
}