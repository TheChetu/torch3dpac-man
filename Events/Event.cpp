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
	// Get Ghost Pathing
	gMap *P = NULL;
	for(int i = 0; i < int(worldLayout.size()); i++) {
		P = new gMap;
		if(i >= 20) {
			if(worldLayout[i-20] != 'Z' && worldLayout[i-20] != 'X')
				P->up = 0;
			else
				P->up = 1;
		}	
		else {
			P->up = 0;
		}
		if(i <= (int(worldLayout.size()) - 20)) {
			if(worldLayout[i+20] != 'Z' && worldLayout[i+20] != 'X')
				P->down = 0;
			else
				P->down = 1;
		}
		else {
			P->down = 0;
		}
		if(i > 0) {
			if(worldLayout[i-1] != 'Z' && worldLayout[i-1] != 'X')
				P->left = 0;
			else
				P->left = 1;
		}
		else {
			P->left = 0;
		}
		if(i <= (int(worldLayout.size()) - 1)) {
			if(worldLayout[i+1] != 'Z' && worldLayout[i+1] != 'X')
				P->right = 0;
			else
				P->right = 1;
		}
		else {
			P->right = 1;
		}

		P->link = NULL;

		GhostMapSize++;
		
		if(GhostMapSize == 1) {
			Front = P;
			Rear = P;
		}
		else {//(GhostMapSize != 1)
			Rear->link = P;
			Rear = Rear->link;
		}

		free(P);
		
		Rear->xp = i % 20;
		Rear->zp = i / 20;
	}

	// Seed Random
	srand(time(0));
	EveInit = TRUE;
	return EveInit;
}

bool Event::CheckCollideDot()
{
	bool detected = FALSE;
	if(!EveInit)
		EInit();
	for(unsigned int i = 0; i < dotpos.size(); i++) {
		float checkZ = dotpos[i].z - zpos;
		float checkX = dotpos[i].x - xpos;
		if( (checkZ < 3.0f) && (checkZ > -3.0f) ) {
			if( (checkX < 3.0f) && (checkX > -3.0f) ) {
				detected = TRUE;
				DotsEaten++;
				AniNum = 2;
				AniElapsed = glutGet(GLUT_ELAPSED_TIME);
				Cloud.SetAnim(AniNum);
				ClWeapon.SetAnim(AniNum);
				// Collision Detected
				currScore += 100;	// Increase Score
				worldLayout[dotpos[i].num] = 'O'; // Erase Dot
				dotpos.erase(dotpos.begin()+i);
				alSourcePlay(source[2]);
			}
		}
	}
	return detected;
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
		reElapsed = glutGet(GLUT_ELAPSED_TIME);
		if((rand() % 2) == 0)
			blend = TRUE;
		else
			gEdible = TRUE;
	}

	return gEdible;
}

int Event::CheckCollideGhosts()
{
	int retvalue = 0;
	for(int i = 0; i < int(gLocs.size()); i++) {
		if( ((gLocs[i].xp - 50 - xpos) < 3.0f) && ((gLocs[i].xp - 50 - xpos) > -3.0f) ) {
			if( ((gLocs[i].zp - 50 - zpos) < 3.0f) && ((gLocs[i].zp - 50 - zpos) > -3.0f) ) {
				if(gEdible) {
					if(i == 0 && !g1Wait) {
						g1Wait = TRUE;
						gDead1 = TRUE;
						gDeadAni1 = glutGet(GLUT_ELAPSED_TIME);
						retvalue = 400;
					}
					if(i == 1) {
						g2Wait = TRUE;
						gDead2 = TRUE;
						gDeadAni2 = glutGet(GLUT_ELAPSED_TIME);
						retvalue = 400;
					}
					if(i == 2) {
						g3Wait = TRUE;
						gDead3 = TRUE;
						gDeadAni3 = glutGet(GLUT_ELAPSED_TIME);
						retvalue = 400;
					}
					if(i == 3) {
						g4Wait = TRUE;
						gDead4 = TRUE;
						gDeadAni4 = glutGet(GLUT_ELAPSED_TIME);
						retvalue = 400;
					}
				}
				else {
					pDead = TRUE;
					retvalue = -500;
				}
			}
		}
	}
	return retvalue;
}

void Event::MoveGhosts()
{
	gMap *P = NULL;
	int directions;
	bool l,r,u,d,locf;
		
	for(int i = 0; i < int(gLocs.size()); i++) {
		gLocs[i].xp -= (float)sin(gLocs[i].gheading*piover180) * 0.04f;
		gLocs[i].zp -= (float)cos(gLocs[i].gheading*piover180) * 0.04f;
		//if((floor(gLocs[i].xp) == gLocs[i].xp) && (floor(gLocs[i].zp) == gLocs[i].zp)) {
/*			directions = 0;
			l = r = u = d = locf = FALSE;
			for(P = Front; (P != NULL) && (locf == FALSE); P = P->link) {
				// Check current location
				if(P->xp == (floor(gLocs[i].xp / 5))) {
					if(P->zp == (floor(gLocs[i].zp / 5))) {
						locf = TRUE;
						// Choose random path
						if(P->left) {
							directions++;
							l = TRUE;
						}
						if(P->right) {
							directions++;
							r = TRUE;
						}
						if(P->up) {
							directions++;
							u = TRUE;
						}
						if(P->down) {
							directions++;
							d = TRUE;
						}
						
						directions = rand() % directions;
						switch(directions) {
							case 0:
								if(d)
									gLocs[i].gheading += 180.0f;
								else if(u)
									gLocs[i].gheading += 0.0f;
								else if(l)
									gLocs[i].gheading += 90.0f;
								else if(r)
									gLocs[i].gheading -= 90.0f;
								break;
							case 1:
								if(u)
									gLocs[i].gheading += 0.0f;
								else if(l)
									gLocs[i].gheading += 90.0f;
								else if(r)
									gLocs[i].gheading -= 90.0f;
								else if(d)
									gLocs[i].gheading += 180.0f;
								break;
							case 2:
								if(l)
									gLocs[i].gheading += 90.0f;
								else if(r)
									gLocs[i].gheading -= 90.0f;
								else if(d)
									gLocs[i].gheading += 180.0f;
								else if(u)
									gLocs[i].gheading += 0.0f;
							break;	
							case 3:
								if(r)
									gLocs[i].gheading -= 90.0f;
								else if(d)
									gLocs[i].gheading += 180.0f;
							else if(u)
									gLocs[i].gheading += 0.0f;
								else if(l)
									gLocs[i].gheading += 90.0f;
								break;
							default:
									if(d)
									gLocs[i].gheading += 180.0f;
								else if(u)
									gLocs[i].gheading += 0.0f;
								else if(l)
									gLocs[i].gheading += 90.0f;
								else if(r)
									gLocs[i].gheading -= 90.0f;
								break;
	
						}
						P = NULL;
					}
				}
			}*/
		//}
	}
}

