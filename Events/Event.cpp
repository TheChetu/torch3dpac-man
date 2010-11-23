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
		if( (checkZ < 0.5f) && (checkZ > 0.5f) ) {
			if( (checkX < 0.5f) && (checkX > 0.5f) ) {
				// Collision Detected
				currScore += 100;	// Increase Score
				worldLayout[dotpos[i].num] = 'Y'; // Erase Dot
			}
		}
	}
	return TRUE;
}