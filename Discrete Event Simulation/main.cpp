#include "Hacker.h"
#include "Event.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>
#include <iomanip>

using namespace std;

/*
compares events according to their time. if times are equal, than compare according to id.
a, b are events to be compared.
*/
class compareEvents{
public:
    bool operator()(Event* a, Event* b){
        if(a->startTime - b->startTime > 0.00001) return true; //>
        else if(abs(a->startTime - b->startTime) < 0.00001){   //=
            if(a->whose > b->whose) return true;
        }
        return false;
    }
};

/*
compares hackers according to their commits. if commits are equal, then time. if times are equal, then id.
a, b are hackers to be compared.
*/
class compareHackers{
public:
    bool operator()(Hacker* a, Hacker* other){
        if(a->nofValidCommits < other->nofValidCommits) return true;
        else if(a->nofValidCommits == other->nofValidCommits){
            if(a->arriveTimeToHoodie - other->arriveTimeToHoodie > 0.00001) return true; //>
            else if(abs(a->arriveTimeToHoodie - other->arriveTimeToHoodie) < 0.00001){   //=
                if(a->id > other->id) return true;
            }
        }
        return false;
    }
};

/*
class of desks, sticker desks and hoodie desks.
*/
class Desk{
public:
    float serviceTime;
    string type;
    int id;
    bool available; //checks if desk is available or not.

    Desk(int _id, float service, string _type){
        this->id = _id;
        this->type = _type;
        this->serviceTime = service;
        this->available = true;
    }
};

int main(int argc, char* argv[]) {

    float currentTime = 0; //start of the program. t=0.
    int maxLengthofSticker=0;
    int maxLengthofHoodie=0;
    int totalGifts=0; //number of total gifts in the program.

    float totalTurnaround = 0;

    ifstream input(argv[1]); //input file.

    vector<Hacker*> hackers; //container for hackers.
    priority_queue<Event*, vector<Event*>, compareEvents> events; //container for events.

    int nofHackers; //number of hackers throughout the simulation.
    input>>nofHackers;
    for(int i=0; i<nofHackers; i++){
        float arrivalTime;
        input>>arrivalTime;

        Hacker* h = new Hacker(i+1); //creates a pointer to hacker.
        hackers.push_back(h); //adds hacker to list.

        Event* arrival = new Event(i+1, arrivalTime, "arrival", 0, -1); //creates a pointer to arrival event.
        events.push(arrival); //adds event to list.
    }

    int nofCommitments; //number of commits throughout the simulation.
    input>>nofCommitments;
    for(int i=0; i<nofCommitments; i++){
        int id, commitLength;
        float commitTime;
        input>>id>>commitLength>>commitTime;

        Event* commitment = new Event(id, commitTime, "commit", commitLength, -1); //creates a pointer to commit event.
        events.push(commitment); //adds event to list.
    }

    int nofEntranceAttempts; //number of entrance attempts throughout the simulation.
    input>>nofEntranceAttempts;
    for(int i=0; i<nofEntranceAttempts; i++){
        int id;
        float attemptTime;
        input>>id>>attemptTime;

        Event* attempt = new Event(id, attemptTime, "entrance", 0, -1); //creates a pointer to attempt event.
        events.push(attempt); //adds event to list.
    }

    vector<Desk*> stickerDesks; //container for sticker desks.
    vector<Desk*> hoodieDesks; //container for hoodie desks.

    int nofStickerDesks; //number of sticker desks.
    input>>nofStickerDesks;
    for(int i=0; i<nofStickerDesks; i++){
        float serviceTime;
        input>>serviceTime;

        Desk* sticker = new Desk(i, serviceTime, "sticker"); //creates a pointer to sticker desk.
        stickerDesks.push_back(sticker); //adds desk to list.
    }

    int nofHoodieDesks; //number of hoodie desks.
    input>>nofHoodieDesks;
    for(int i=0; i<nofHoodieDesks; i++){
        float serviceTime;
        input>>serviceTime;

        Desk* hoodie = new Desk(i, serviceTime, "hoodie"); //creates a pointer to hoodie desk.
        hoodieDesks.push_back(hoodie); //adds desk to list.
    }

    queue<Hacker*> stickerQueue; //container for hackers in the sticker queue.
    priority_queue<Hacker*, vector<Hacker*>, compareHackers> hoodieQueue; //container for hackers in the hoodie queue.

    while(!events.empty()) {
        Event *a = events.top(); //event with a smallest time.

        currentTime = a->startTime; //progress time. t=event time.
        Hacker *hacker = hackers.at((a->whose) - 1); //hacker in action.

        /*
        if event type is arrival, mark hacker as arrived.
        */
        if (a->type == "arrival") {
            hacker->didArrive = true;
            hacker->canCommit = true;
        }

        /*
        if event type is commit, increment number of commits if hacker is capable of doing so.
        */
        if (a->type == "commit") {
            if (hacker->canCommit) {
                hacker->nofLinesChanged = a->commitLength; //states how many lines of code changed.
                hacker->totalLines += a->commitLength; //increments number of total lines of code.
                hacker->nofCommits++;
            
                if (a->commitLength >= 20) { //checks if it is enough.
                    hacker->nofValidCommits++;
                    
                    if (hacker->nofValidCommits >= 3 && hacker->ownedStickers <= 3) { //checks if hacker is capable of entering to sticker queue.
                        hacker->canEnter = true;
                    }
                }
            }
        }

        /*
        if event type is entrance, send hacker to sticker desk if any desk available. else put hacker in sticker queue.
        */
        if (a->type == "entrance") {
            if (hacker->didArrive) { //checks if hacker is in area.
               
                if(hacker->ownedStickers == 3) {
                    hacker->canEnter = false;
                    hacker->excessGifts++;
                }
                if (hacker->canEnter) { //checks if hacker is capable of entering.
                    bool isAvailable = false;
                    int availableDeskId;
                    for (int i = 0; i < nofStickerDesks; i++) { //finds available desk with smallest id.
                        if (stickerDesks[i]->available) {
                            isAvailable = true;
                            availableDeskId = i;
                            break;
                        }
                    }
                    if (isAvailable) { //if any desk available, hacker goes to the desk.
                        stickerDesks[availableDeskId]->available = false;
                        float serviceTime = stickerDesks[availableDeskId]->serviceTime;
                        hacker->arriveTimeToSticker = currentTime;
                        
                        Event *departure = new Event(hacker->id, currentTime + serviceTime, "departStickerDesk", 0, availableDeskId); //creates a pointer to departure event.
                        events.push(departure); //adds event to list.
                    } else { //if no desk available, hacker goes to queue.
                        
                        stickerQueue.push(hacker); //adds hacker to queue.
                        hacker->arriveTimeToSticker = currentTime;
                        
                        if (stickerQueue.size() > maxLengthofSticker) maxLengthofSticker = stickerQueue.size(); //if length of the queue exceeds its maximum, make it new maximum.
                    }
                }else{ //if hacker can not enter.
                    hacker->invalidAttempts++;
                
                }
            }else{ //if hacker did not arrive yet.
                hacker->invalidAttempts++;
            }
        }

        /*
        if event type is departure from sticker desk, makes hacker leave the desk and puts hacker in the hoodie queue or hoodie desk.
        */
        if (a->type == "departStickerDesk") {
            
            stickerDesks[a->deskNo]->available = true; //makes available the departuring desk.
            hacker->canCommit = false; //hacker can not commit code no longer since hacker is in queue.
            hacker->ownedStickers++; //hacker gets a sticker.
            
            totalGifts++;
            if (hacker->ownedStickers >= 3) hacker->canEnter = false; //if hacker has 3 sticker, hacker can no longer enter.

            bool isAvailable = false;
            int availableDeskId;
            for (int i = 0; i < nofHoodieDesks; i++) { //finds the desk with smallest id.
                if (hoodieDesks[i]->available) {
                    isAvailable = true;
                    availableDeskId = i;
                    break;
                }
            }
            if (isAvailable) { //if any desk available, sends hacker to the desk.
                
                hoodieDesks[availableDeskId]->available = false;
                float serviceTime = hoodieDesks[availableDeskId]->serviceTime;
                
                Event *departure = new Event(hacker->id, currentTime + serviceTime, "departHoodieDesk", 0, availableDeskId); //creates a pointer to departure event.
                events.push(departure); //adds event to list.
            } else { //if no desk available, puts hacker in queue.
                
                hacker->arriveTimeToHoodie = currentTime;
                hoodieQueue.push(hacker); //adds hacker to queue.
                
                if (hoodieQueue.size() > maxLengthofHoodie) maxLengthofHoodie = hoodieQueue.size(); //if length of the queue exceeds its maximum, make it new maximum.
            }
            if (!stickerQueue.empty()) { //if there is someone waiting in the queue, calls him/her as next.
                stickerDesks[a->deskNo]->available = false; //makes desk not available again.
                float serviceTime = stickerDesks[a->deskNo]->serviceTime;
                
                Hacker* next = stickerQueue.front(); //next hacker to proceed to the desk.
                next->waitingTimeSticker += currentTime - next->arriveTimeToSticker;
                stickerQueue.pop();
                
                Event *departure = new Event(next->id, currentTime + serviceTime, "departStickerDesk", 0, a->deskNo); //creates a pointer to departure event.
                events.push(departure); //adds event to list.
            }
        }

        /*
        if event type is departure from hoodie desk, makes hacker leave the desk.
        */
        if (a->type == "departHoodieDesk") {
            
            hacker->turnaround += currentTime - hacker->arriveTimeToSticker; //updates turnaround time.
            totalTurnaround+=currentTime - hacker->arriveTimeToSticker;

            hoodieDesks[a->deskNo]->available = true; //makes desk available.
            hacker->canCommit = true; //hacker can commit after leaving hoodie desk.
            hacker->ownedHoodies++; //hacker gets a sticker.
            
            totalGifts++;

            if (!hoodieQueue.empty()) { //if there is someone waiting in the queue, calls him/her as next.
                hoodieDesks[a->deskNo]->available = false; //makes desk not available again.
                float serviceTime = hoodieDesks[a->deskNo]->serviceTime;
                
                Hacker *next = hoodieQueue.top(); //next hacker to proceed to the desk.
                next->waitingTimeHoodie += currentTime - next->arriveTimeToHoodie;
                hoodieQueue.pop();
               
                Event *departure = new Event(next->id, currentTime + serviceTime, "departHoodieDesk", 0, a->deskNo); //creates a pointer to departure event.
                events.push(departure); //adds event to list.
            }
        }

        delete (a); //events deleted.
        events.pop();
    }

    ofstream out(argv[2]); //output file.

    float totalWaitingTime_Sticker = 0; 
    float totalWaitingTime_Hoodie = 0;
    int totalCodeCommits = 0;
    int totalLinesofCode = 0;
    int totalInvalidAttempts = 0;
    int totalExcessGifts = 0; //total invalid attempts due to excess number of gifts.
    int id_unlucky = -1; //id of the hacker who waits most in the queues.
    int id_lucky = -1; //id of the hacker who waits least in the queues.
    float maximum_wait = 0; 
    float minimum_wait = 9999999999999;

    for(int i=0; i<nofHackers; i++){ //iterates over hackers and updates necessary informations.
        Hacker* a = hackers.at(i);
        
        totalWaitingTime_Sticker += a->waitingTimeSticker;
        totalWaitingTime_Hoodie += a->waitingTimeHoodie;

        if((a->waitingTimeHoodie + a->waitingTimeSticker) - maximum_wait > 0.00001){ //determines the hacker who waits most.
            id_unlucky = i+1;
            maximum_wait = a->waitingTimeSticker + a->waitingTimeHoodie;
        }

        if(a->ownedStickers == 3){
            if((a->waitingTimeHoodie + a->waitingTimeSticker) - minimum_wait < -0.00001){ //determines the hacker who waits least.
                id_lucky = a->id;
                minimum_wait = a->waitingTimeSticker + a->waitingTimeHoodie;
            }
        }
       
        totalCodeCommits += a->nofCommits;
        totalLinesofCode += a->totalLines;
        totalInvalidAttempts += a->invalidAttempts;
        totalExcessGifts += a->excessGifts;

        delete (a); //hackers deleted.

    }

    /*
    prints informations to the file with wanted precisions.
    */
    out << maxLengthofSticker << endl;
    out << maxLengthofHoodie << endl;
    out << fixed << setprecision(3) << (float) (totalGifts/2) / (float) nofHackers << endl;
	out << fixed << setprecision(3) << totalWaitingTime_Sticker / (totalGifts/2) << endl;
    out << fixed << setprecision(3) << totalWaitingTime_Hoodie / (totalGifts/2) << endl;
    out << fixed << setprecision(3) << (float)totalCodeCommits / nofHackers << endl;
    out << fixed << setprecision(3) << (float)totalLinesofCode / totalCodeCommits << endl;
	out << totalTurnaround / (totalGifts/2) << endl;    out << totalInvalidAttempts - totalExcessGifts << endl;
    out << totalExcessGifts << endl;
    if(id_unlucky != -1) out << id_unlucky << " " << fixed << setprecision(3) << maximum_wait << endl;
    else out << 1 << " " << maximum_wait << endl;
    if(id_lucky != -1) out << id_lucky << " " << fixed << setprecision(3) << minimum_wait << endl;
    else out << id_lucky << " " << -1 <<  endl;
    out << fixed << setprecision(3) << currentTime << endl;

	return 0;
}