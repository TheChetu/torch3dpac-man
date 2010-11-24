#ifndef EVENT_H
#define EVENT_H

#include <global\globref.h>
#include <GL\glut.h>

extern vector<TLoc> lctn;			// Location Vector
extern vector<char> worldLayout;	// World Layout Storage
extern int currScore;				// The Current Score
extern float xpos;					// Player x-position
extern float zpos;					// Player z-position
extern CMD2Model Cloud;				// Cloud Player Model
extern CMD2Model Weapon;			// Cloud Weapon Model
extern int AniNum;					// Current Animation
extern long AniElapsed;				// Time Elapsed For Animation
extern int currLives;				// Current Player Lives

static bool EveInit = FALSE;		// Events Initialized Check
static bool Rewards[10];				// Rewards

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
		static bool Reward();
};

#endif