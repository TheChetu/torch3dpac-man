#ifndef EVENT_H
#define EVENT_H

#include <global/globref.h>

extern vector<TLoc> lctn;			// Location Vector
extern vector<TLoc> dotpos;			// Positioning of Dots
extern vector<char> worldLayout;	// World Layout Storage
extern int currScore;				// The Current Score

bool EveInit = FALSE;						// Events Initialized Check

class Event {
	private:
		bool EInit();
	public:
		static bool CheckCollideDot();
		static bool CheckCollideWall();
};

#endif