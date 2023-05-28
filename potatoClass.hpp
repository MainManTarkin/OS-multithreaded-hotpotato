#pragma once
#include <vector>


using namespace std;

/*

The potatoClass is used to manage the counting 
and holding which thread holds a potato.

also holds the state of the threads (if there still alive or not)

*/

struct potatoCounter{

    unsigned int counter = 0;
    bool isHeld = false;
    bool threadAlive = true;

};

class potatoClass
{
private:
    
  
    vector<potatoCounter> potatoCount;

    
public:
    potatoClass(int playerCount);
    
    void incrementCount(int threadIndex);
    
    int getCount(int vectorIndex);

    void heldPotato(int vectorIndex, bool stateToSet);

    void setThreadState(int vectorIndex, bool stateToSet);

    bool currentlyHoldsPotato(int vectorIndex);

    bool geThreadState(int vectorIndex);

};


