#include "Hacker.h"
#include "Event.h"
#include <iostream>

using namespace std;

/*
creates event with given id, time, type, commit length, and desk id.
*/
Event::Event(int _id, float eventTime, string _type, int comm, int deskid) {

    this->whose = _id;
    this->type = _type;
    this->startTime = eventTime;
    this->commitLength = comm;
    this->deskNo = deskid;

}