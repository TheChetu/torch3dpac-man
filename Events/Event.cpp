#include "Events\Event.h"

bool Event::EInit()
{
	// Calculate Dot Centers
	zLoc theDots;
	for(unsigned int i = 0; i < worldLayout.size(); i++) {
		switch(worldLayout[i]) {
			case 'Z':
				theDots.x = lctn[i].x - 50;
				theDots.z = lctn[i].t - 50;
				theDots.num = i;
				dotpos.push_back(theDots);
				break;
			default:
				break;
		}
	}
	srand(time(0));
	EveInit = TRUE;
	return EveInit;
}

bool Event::CheckCollideDot()
{
	if(!EveInit)
		EInit();
	for(unsigned int i = 0; i < dotpos.size(); i++) {
		float checkZ = dotpos[i].z - zpos;
		float checkX = dotpos[i].x - xpos;
		if( (checkZ < 3.0f) && (checkZ > -3.0f) ) {
			if( (checkX < 3.0f) && (checkX > -3.0f) ) {
				DotsEaten++;
				AniNum = 2;
				AniElapsed = glutGet(GLUT_ELAPSED_TIME);
				Cloud.SetAnim(AniNum);
				ClWeapon.SetAnim(AniNum);
				// Collision Detected
				currScore += 100;	// Increase Score
				worldLayout[dotpos[i].num] = 'Y'; // Erase Dot
				dotpos.erase(dotpos.begin()+i);
				alSourcePlay(source[2]);
			}
		}
	}
	return TRUE;
}

bool Event::Reward()
{
	if((currScore == 10000) && (Rewards[0] == FALSE)) {
		currLives++;
		Rewards[0] = TRUE;
	}
	if((currScore == 30000) && (Rewards[1] == FALSE)) {
		currLives++;
		Rewards[1] = TRUE;
	}
	if((currScore == 100000) && (Rewards[2] == FALSE)) {
		currLives++;
		Rewards[2] = TRUE;
	}
	if((DotsEaten == 20) && (Rewards[3] == FALSE)) {
		if((rand() % 2) == 0)
			glEnable(GL_BLEND);
		else
			gEdible = TRUE;
	}

	return gEdible;
}

bool Event::CheckCollideGhosts()
{
	if(gEdible) {
		// if g1 - g4 collide set collision entity to dead
		if(/*Ghost1Collides*/TRUE) {
			gDeadAni1 = glutGet(GLUT_ELAPSED_TIME);
		}
	}
	else {
		// if g1 - g4 collide with pacman set pacman to dead
	}
	return TRUE;
}

void Event::MoveGhosts()
{
	for(int i = 0; i < 4; i++) {
		if((gLocs[i].xp - 50 - xpos) < 20.0f) {
			if((gLocs[i].zp - 50 - zpos) < 20.0f) {
				// Move Toward Pacman
				// Set Animations to 1 (GAniNum1 - GAniNum4)
			}
			else {
				// Move Randomly
			}
		}
		else {
			// Move Randomly
		}
	}
}

