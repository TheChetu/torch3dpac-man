#ifndef EVENT_H
#define EVENT_H

#include <global\globref.h>

extern vector<TLoc> lctn;			// Location Vector
extern vector<char> worldLayout;	// World Layout Storage
extern int currScore;				// The Current Score
extern float xpos;					// Player x-position
extern float zpos;					// Player z-position
extern CMD2Model Cloud;				// Cloud Player Model
extern CMD2Model ClWeapon;			// Cloud Weapon Model
extern int AniNum;					// Current Animation
extern long AniElapsed;				// Time Elapsed For Animation
extern long gDeadAni1;				// Time Elapsed For Ghost Animation
extern long gDeadAni2;				// Time Elapsed For Ghost Animation
extern long gDeadAni3;				// Time Elapsed For Ghost Animation
extern long gDeadAni4;				// Time Elapsed For Ghost Animation
extern int GAniNum1;				// Ghost 1 Animation
extern int GAniNum2;				// Ghost 2 Animation
extern int GAniNum3;				// Ghost 3 Animation
extern int GAniNum4;				// Ghost 4 Animation
extern int currLives;				// Current Player Lives
extern bool pDead;					// PacMan Dead?
extern bool	gDead1;					// Ghost 1 Dead?
extern bool	gDead2;					// Ghost 2 Dead?
extern bool	gDead3;					// Ghost 3 Dead?
extern bool	gDead4;					// Ghost 4 Dead?
extern bool	gEdible;				// Ghosts Edible?
extern int currLevel;				// Current Level Number
extern bool	levelCom;				// Level Completion
extern bool	levelStr;				// Level Started
extern bool blend;					// Blending Enabled
extern long reElapsed;				// Reward Elapsed Time
extern GLfloat sGauge;				// Sprint Gauge
extern double FPS;					// Frames Per Second
extern bool dbug;					// Debug Logging

extern const float piover180;		// PI/180 for angle calculations

extern bool gEdible;				// Ghosts Edible?

static bool EveInit = FALSE;		// Events Initialized Check
static bool Rewards[10];			// Rewards
static int DotsEaten = 0;			// Number of Dots Eaten
extern ALuint source[5];			// Audio Sources
static long lastMove = 0;				// Holds Ghosts Last Move Time;

// Wait for Ghost Edible timer to end.
static bool g1Wait = FALSE;
static bool g2Wait = FALSE;
static bool g3Wait = FALSE;
static bool g4Wait = FALSE;

//extern void PrintToLog(const char* prnErr);

typedef struct {
	int z;
	int x;
	int num;
} zLoc;

struct gMap {
	int xp;
	int zp;
	int right;
	int left;
	int up;
	int down;
};

typedef gMap gMap;
static int GhostMapSize = 0;

extern void PrintToLog(const char* prnErr);
extern vector<zLoc> dotpos;				// Positioning of Dots
extern vector<GhP> gLocs;				// Ghost Positions
extern vector<gMap> gMapSet;			// Ghost Map
extern float heading;					//direction
extern qwall q1, q2, q3, q4;

class Event {
	private:
		static bool EInit();
	public:
		static bool CheckCollideDot();
		static bool CheckCollideWall(qwall currentQ);
		static bool Reward();
		static int CheckCollideGhosts();
		static void	MoveGhosts();
		static void fileWalls(wall currentWall);
		static qwall currentQ();
};

#endif