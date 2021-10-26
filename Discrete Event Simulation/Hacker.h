#ifndef HACKNPAPER_HACKER_H
#define HACKNPAPER_HACKER_H

/*
class of hackers.
*/
class Hacker {
public:
    int id, ownedHoodies, ownedStickers, nofLinesChanged, totalLines, nofCommits, nofValidCommits, invalidAttempts, excessGifts;
    bool didArrive, canEnter, canCommit;
    float arriveTimeToHoodie, arriveTimeToSticker, waitingTimeSticker, waitingTimeHoodie, turnaround;

    Hacker(int _id);

};

#endif //HACKNPAPER_HACKER_H

