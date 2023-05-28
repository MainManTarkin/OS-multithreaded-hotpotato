#include "semp.hpp"

void SemInit(semp &s, int32_t initial_value)
{

    s.sempValue = initial_value;

    if (pthread_mutex_init(&s.semMutex, nullptr))
    {

        perror("problem in SemInit() with pthread_mutex_init(): ");
        exit(1);
    }

    if (pthread_cond_init(&s.semCondVar, nullptr))
    {

        perror("problem in SemInit() with pthread_cond_init(): ");
        exit(1);
    }
}

void SemPost(semp &s)
{

    pthread_mutex_lock(&s.semMutex);
    s.sempValue++;
    pthread_cond_signal(&s.semCondVar);

    pthread_mutex_unlock(&s.semMutex);
}

void SemWait(semp &s)
{

    pthread_mutex_lock(&s.semMutex);
    
    while (s.sempValue < 0)
    {

        pthread_cond_wait(&s.semCondVar, &s.semMutex);
    }
    s.sempValue--;
    pthread_mutex_unlock(&s.semMutex);
}