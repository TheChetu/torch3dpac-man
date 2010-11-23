#ifndef EVENT_H
#define EVENT_H

#include <global\globref.h>

extern vector<TLoc> lctn;			// Location Vector
extern vector<char> worldLayout;	// World Layout Storage
extern int currScore;				// The Current Score
extern float xpos;					// Player x-position
extern float zpos;					// Player z-position

static bool EveInit = FALSE;				// Events Initialized Check

typedef struct {
	int z;
	int x;
	int num;
} zLoc;

extern vector<zLoc> dotpos;				// Positioning of Dots

class Event {
	private:
		static bool EInit();
	public:
		static bool CheckCollideDot();
		static bool CheckCollideWall();
};

#endif