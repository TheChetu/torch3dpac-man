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
				AniNum = 2;
				AniElapsed = glutGet(GLUT_ELAPSED_TIME);
				Cloud.SetAnim(AniNum);
				Weapon.SetAnim(AniNum);
				// Collision Detected
				currScore += 100;	// Increase Score
				worldLayout[dotpos[i].num] = 'Y'; // Erase Dot
				dotpos.erase(dotpos.begin()+i);
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

	return TRUE;
}