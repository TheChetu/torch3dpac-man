#include "Events\Event.h"

bool Event::EInit()
{

	// Calculate Dot Centers
	zLoc theDots;

	wall currentWall;

	for(unsigned int i = 0; i < worldLayout.size(); i++) {
		switch(worldLayout[i]) {
			case 'Z':
				theDots.x = lctn[i].x - 50;
				theDots.z = lctn[i].t - 50;
				theDots.num = i;
				dotpos.push_back(theDots);
				break;

			case 'T':
				currentWall.minx = lctn[i].x - 50.0f;
				currentWall.maxx = currentWall.minx + 5.0f;
				currentWall.minz = lctn[i].t - 50.0f;
				currentWall.maxz = currentWall.minz;
				currentWall.type = 0;
				fileWalls(currentWall);
				break;

			case 'B':
				currentWall.minx = lctn[i].x - 50.0f;
				currentWall.maxx = currentWall.minx + 5.0f;
				currentWall.minz = lctn[i].t - 50.0f;
				currentWall.maxz = currentWall.minz;
				currentWall.type = 0;
				fileWalls(currentWall);
				break;

			case 'L':
				currentWall.minx = lctn[i].x - 50.0f;
				currentWall.maxx = currentWall.minx;
				currentWall.minz = lctn[i].t - 50.0f;
				currentWall.maxz = currentWall.minz + 5.0f;
				currentWall.type = 1;
				fileWalls(currentWall);
				break;

			case 'R':
				currentWall.minx = lctn[i].x - 45.0f;
				currentWall.maxx = currentWall.minx;
				currentWall.minz = lctn[i].t - 50.0f;
				currentWall.maxz = currentWall.minz + 5.0f;
				currentWall.type = 1;
				fileWalls(currentWall);
				break;

			case 'D':
				if(worldLayout[i+1] == 'Z' || worldLayout[i+1] == 'L')
				{
					currentWall.minx = lctn[i].x - 45.0f;
					currentWall.maxx = currentWall.minx;
					currentWall.minz = lctn[i].t - 50.0f;
					currentWall.maxz = currentWall.minz + 5.0f;
					currentWall.type = 1;
					fileWalls(currentWall);

					currentWall.minx = lctn[i].x - 50.0f;
					currentWall.maxx = currentWall.minx + 5.0f;
					currentWall.minz = lctn[i].t - 50.0f;
					currentWall.maxz = currentWall.minz;
					currentWall.type = 0;
					fileWalls(currentWall);
				}

				else
				{
					currentWall.minx = lctn[i].x - 50.0f;
					currentWall.maxx = currentWall.minx;
					currentWall.minz = lctn[i].t - 50.0f;
					currentWall.maxz = currentWall.minz + 5.0f;
					currentWall.type = 1;
					fileWalls(currentWall);

					currentWall.minx = lctn[i].x - 50.0f;
					currentWall.maxx = currentWall.minx + 5.0f;
					currentWall.minz = lctn[i].t - 50.0f;
					currentWall.maxz = currentWall.minz;
					currentWall.type = 0;
					fileWalls(currentWall);
				}
				break;

			case 'U':
				if(worldLayout[i+1] == 'Z' || worldLayout[i+1] == 'Y' || worldLayout[i+1] == 'L')
				{
					currentWall.minx = lctn[i].x - 45.0f;
					currentWall.maxx = currentWall.minx;
					currentWall.minz = lctn[i].t - 55.0f;
					currentWall.maxz = currentWall.minz + 5.0f;
					currentWall.type = 1;
					fileWalls(currentWall);

					currentWall.minx = lctn[i].x - 50.0f;
					currentWall.maxx = currentWall.minx + 5.0f;
					currentWall.minz = lctn[i].t - 50.0f;
					currentWall.maxz = currentWall.minz;
					currentWall.type = 0;
					fileWalls(currentWall);
				}

				else
				{
					currentWall.minx = lctn[i].x - 50.0f;
					currentWall.maxx = currentWall.minx;
					currentWall.minz = lctn[i].t - 55.0f;
					currentWall.maxz = currentWall.minz + 5.0f;
					currentWall.type = 1;
					fileWalls(currentWall);

					currentWall.minx = lctn[i].x - 50.0f;
					currentWall.maxx = currentWall.minx + 5.0f;
					currentWall.minz = lctn[i].t - 50.0f;
					currentWall.maxz = currentWall.minz;
					currentWall.type = 0;
					fileWalls(currentWall);
				}

				break;

			default:
				break;
		}
	}

	// Get Ghost Pathing
	gMap P;
	for(int i = 0; i < int(worldLayout.size()); i++) {
		if(i >= 20) {
			if((worldLayout[i-20] != 'Z') && (worldLayout[i-20] != 'X'))
				P.up = 0;
			else
				P.up = 1;
		}	
		else {
			P.up = 0;
		}
		if(i <= (int(worldLayout.size()) - 20)) {
			if(worldLayout[i+20] != 'Z' && worldLayout[i+20] != 'X')
				P.down = 0;
			else
				P.down = 1;
		}
		else {
			P.down = 0;
		}
		if(i > 0) {
			if((worldLayout[i-1] != 'Z') && (worldLayout[i-1] != 'X'))
				P.left = 0;
			else
				P.left = 1;
		}
		else {
			P.left = 0;
		}
		if(i <= (int(worldLayout.size()) - 1)) {
			if((worldLayout[i+1] != 'Z') && (worldLayout[i+1] != 'X'))
				P.right = 0;
			else
				P.right = 1;
		}
		else {
			P.right = 1;
		}
		
		GhostMapSize++;
		
		P.xp = i % 20 * 5;
		P.zp = i / 20 * 5;

		gMapSet.push_back(P);
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
				if(worldLayout[dotpos[i].num] == 'Z')
					worldLayout[dotpos[i].num] = 'O'; // Erase Dot
				dotpos.erase(dotpos.begin()+i);
				alSourcePlay(source[2]);
			}
		}
	}
	return detected;
}

bool Event::CheckCollideWall(qwall currentQ)
{
	bool detected = false;
	if(!EveInit)
		EInit();

	//check x walls
	for(int i = 0; i < int(currentQ.xwalls.size()); i++)
	{
		if(xpos > currentQ.xwalls[i].minx && xpos < currentQ.xwalls[i].maxx)	//inside wall range
		{
			if(zpos > currentQ.xwalls[i].minz) //at the front of the wall
			{
				if (zpos - (float)cos(heading*piover180) < currentQ.xwalls[i].minz)
				{
					zpos += ((float)cos(heading*piover180) * (3.5f / FPS));
					detected = true;
				}
			}

			else if(zpos < currentQ.xwalls[i].minz) //at the back of the wall
			{
				if (zpos - (float)cos(heading*piover180) > currentQ.xwalls[i].minz)
				{
					zpos += ((float)cos(heading*piover180) * (3.5f / FPS));
					detected = true;
				}
			}
		}
	}

	//check z walls
	for(int j = 0; j < int(currentQ.zwalls.size()); j++)
	{
		if(zpos > currentQ.zwalls[j].minz && zpos < currentQ.zwalls[j].maxz)	//inside wall range
		{
			if(xpos > currentQ.zwalls[j].minx) //at the front of the wall
			{
				if (xpos - (float)sin(heading*piover180) < currentQ.zwalls[j].minx)
				{
					xpos += ((float)sin(heading*piover180) * (3.5f / FPS));
					detected = true;
				}
			}

			else if(xpos < currentQ.zwalls[j].minx) //at the back of the wall
			{
				if (xpos - (float)sin(heading*piover180) > currentQ.zwalls[j].minx)
				{
					xpos += ((float)sin(heading*piover180) * (3.5f / FPS));
					detected = true;
				}
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
			blend = TRUE;
			gEdible = TRUE;
			Rewards[3] = TRUE;
	}
	if((DotsEaten == 60) && (Rewards[4] == FALSE)) {
		reElapsed = glutGet(GLUT_ELAPSED_TIME);
			blend = TRUE;
			gEdible = TRUE;
			Rewards[4] = TRUE;
	}
	if((DotsEaten == 110) && (Rewards[5] == FALSE)) {
		reElapsed = glutGet(GLUT_ELAPSED_TIME);
			blend = TRUE;
			gEdible = TRUE;
			Rewards[5] = TRUE;
	}
	if((DotsEaten == 170) && (Rewards[6] == FALSE)) {
		reElapsed = glutGet(GLUT_ELAPSED_TIME);
			blend = TRUE;
			gEdible = TRUE;
			Rewards[6] = TRUE;
	}



	return gEdible;
}

int Event::CheckCollideGhosts()
{
	int retvalue = 0;
	for(int i = 0; i < int(gLocs.size()); i++) {
		if( ((gLocs[i].xp - 50 - xpos) < 4.0f) && ((gLocs[i].xp - 50 - xpos) > -4.0f) ) {
			if( ((gLocs[i].zp - 50 - zpos) < 4.0f) && ((gLocs[i].zp - 50 - zpos) > -4.0f) ) {
				if(gEdible) {
					if(i == 0 && !g1Wait) {
						g1Wait = TRUE;
						gDead1 = TRUE;
						gDeadAni1 = glutGet(GLUT_ELAPSED_TIME);
						retvalue = 400;
					}
					if(i == 1 && !g2Wait) {
						g2Wait = TRUE;
						gDead2 = TRUE;
						gDeadAni2 = glutGet(GLUT_ELAPSED_TIME);
						retvalue = 400;
					}
					if(i == 2 && !g3Wait) {
						g3Wait = TRUE;
						gDead3 = TRUE;
						gDeadAni3 = glutGet(GLUT_ELAPSED_TIME);
						retvalue = 400;
					}
					if(i == 3 && !g4Wait) {
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
	int directions;
	bool l,r,u,d,locf;
	
	if(((elapsed() - lastMove) / 1000) >= 2) {
		lastMove = elapsed();
		for(int i = 0; i < int(gLocs.size()); i++) {
			// Change gWait States
			if(g1Wait && !gEdible)
				g1Wait = FALSE;
			if(g2Wait && !gEdible)
				g2Wait = FALSE;
			if(g3Wait && !gEdible)
				g3Wait = FALSE;
			if(g4Wait && !gEdible)
				g4Wait = FALSE;

			/* Print For Debugging */
			if(dbug) {
				char* prn = NULL;
				prn = (char *)malloc(100);
				sprintf(prn,"Ghost %d at xpos %4.4f zpos %4.4f", i, gLocs[i].xp, gLocs[i].zp);
				PrintToLog(prn);
			}

		
			directions = 0;
			l = r = u = d = locf = FALSE;
			for(int j = 0; (j < int(gMapSet.size())) && (locf == FALSE); j++) {
				// Check current location
				if(gMapSet[j].xp == (gLocs[i].xp)) {
					if(gMapSet[j].zp == (gLocs[i].zp)) {
						locf = TRUE;
						// Choose random path
						if(gMapSet[j].left) {
							directions++;
							l = TRUE;
						}
						if(gMapSet[j].right) {
							directions++;
							r = TRUE;
						}
						if(gMapSet[j].up) {
							directions++;
							u = TRUE;
						}
						if(gMapSet[j].down) {
							directions++;
							d = TRUE;
						}
						
						directions = rand() % 4;
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
					}
				}
			}

			// Move only if not waiting for respawn
			if(i == 0 && !g1Wait) {
				gLocs[0].xp -= (float)sin(gLocs[i].gheading*piover180) * 5.0f;
				gLocs[0].zp -= (float)cos(gLocs[i].gheading*piover180) * 5.0f;
			}
			if(i == 1 && !g2Wait) {
				gLocs[1].xp -= (float)sin(gLocs[i].gheading*piover180) * 5.0f;
				gLocs[1].zp -= (float)cos(gLocs[i].gheading*piover180) * 5.0f;
			}
			if(i == 2 && !g3Wait) {
				gLocs[2].xp -= (float)sin(gLocs[i].gheading*piover180) * 5.0f;
				gLocs[2].zp -= (float)cos(gLocs[i].gheading*piover180) * 5.0f;
			}
			if(i == 3 && !g4Wait) {
				gLocs[3].xp -= (float)sin(gLocs[i].gheading*piover180) * 5.0f;
				gLocs[3].zp -= (float)cos(gLocs[i].gheading*piover180) * 5.0f;
			}
			// Check Position Outside Range of Stage
			if(gLocs[i].xp > 100.0f || gLocs[i].xp < 0.0f)
				gLocs[i].xp = 50.0f;
			if(gLocs[i].zp > 100.0f || gLocs[i].zp < 0.0f)
				gLocs[i].zp = 50.0f;

			gLocs[i].xp = floor(gLocs[i].xp);
			gLocs[i].zp = floor(gLocs[i].zp);

		}
	}
}

void Event::fileWalls(wall currentWall)
{
	if(currentWall.minz <= 0.0f)
	{
		if(currentWall.minx <= 0.0f)
		{
			if(currentWall.type == 0)
				q1.xwalls.push_back(currentWall);

			else if(currentWall.type == 1)
				q1.zwalls.push_back(currentWall);
		}

		else
		{
			if(currentWall.type == 0)
				q2.xwalls.push_back(currentWall);

			else if(currentWall.type == 1)
				q2.zwalls.push_back(currentWall);
		}
	}

	else	//currentWall.minz > 0
	{
		if(currentWall.minx <= 0.0f)
		{
			if(currentWall.type == 0)
				q3.xwalls.push_back(currentWall);

			else if(currentWall.type == 1)
				q3.zwalls.push_back(currentWall);
		}

		else
		{
			if(currentWall.type == 0)
				q4.xwalls.push_back(currentWall);

			else if(currentWall.type == 1)
				q4.zwalls.push_back(currentWall);
		}
	}
}

qwall Event::currentQ()
{
		if(zpos <= 0.0f)
	{
		if(xpos <= 0.0f)
		{
			return q1;
		}

		else
		{
			return q2;
		}
	}

	else	//zpos > 0
	{
		if(xpos <= 0.0f)
		{
			return q3;
		}

		else
		{
			return q4;
		}
	}
}