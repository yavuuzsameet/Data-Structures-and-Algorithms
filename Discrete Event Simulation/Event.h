#ifndef HACKNPAPER_EVENT_H
#define HACKNPAPER_EVENT_H

#include "string"
using namespace std;

/*
class of events.
*/
class Event {
public:
    float startTime;
    int whose, commitLength, deskNo;
    string type;

    Event(int _id, float eventTime, string _type, int comm, int deskid);

};

#endif //HACKNPAPER_EVENT_H
