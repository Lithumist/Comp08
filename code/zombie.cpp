/*
	zombie.cpp

	Declares a a zombie class that can function on it's own.
	level.h includes zombie.h
*/

#include "zombie.h"


///
// Resets the zombie, like new
///
void zombie::reset(float xpos, float ypos, int MaxHp, int dmgStrength)
{
	x = xpos;
	y = ypos;
	maxHp = MaxHp;
	curHp = maxHp;
	dmgVal = dmgStrength;
	gotoTarget = false;
	tx = 0;
	ty = 0;

	attackTimer.restart();

	distanceThreshold = 300;

	cellZombie = NULL;

	SND_zombie.setBuffer(global::SNDBUF_zombie);
	SND_zombie.setLoop(false);
	SND_zombie.setMinDistance(96);
	SND_zombie.setAttenuation(5);


	SPR_left.setTexture(global::TXT_zombie_left);
	SPR_right.setTexture(global::TXT_zombie_right);



	currentSoundTimerMax = ut::random(5,30);// random between 5 and 30 seconds


	playerLeft = false;

	dead = false;
	proc = false;
}




///
// Updates the zombie
///
void zombie::step()
{
	if(dead)
		return;

	// Update sound position
	SND_zombie.setPosition(x,y,0);


	// Play sound if needed
	if(soundTimer.getElapsedTime().asSeconds() >= currentSoundTimerMax)
	{
		SND_zombie.play();
		currentSoundTimerMax = ut::random(5,30);// random between 5 and 30 seconds
		soundTimer.restart();
	}



	// Update playerLeft
	if(*global::flPlayerX+7 < x+7)
		playerLeft = true;
	else
		playerLeft = false;



	// Calculate the current distance to target
	float dist = ut::distanceBetween(x+6,y+6,*global::flPlayerX+8,*global::flPlayerY+8);



	// Update the gotoTarget flag
	if(dist <= distanceThreshold)
		gotoTarget = true;
	else
		gotoTarget = false;




	// Update the 2 vectors to target
	if(gotoTarget)
	{
		// non normalised
		vecToTarget.x = *global::flPlayerX - x;
		vecToTarget.y = *global::flPlayerY - y;

		// normalised

		vecToTargetNorm.x = vecToTarget.x;
		vecToTargetNorm.y = vecToTarget.y;

		float mg = sqrt(vecToTarget.x*vecToTarget.x + vecToTarget.y*vecToTarget.y); // this line looks like its wrong...
		vecToTargetNorm.x /= mg;
		vecToTargetNorm.y /= mg;

		if(vecToTargetNorm.x < 1 && vecToTargetNorm.x > 0)
			vecToTargetNorm.x = 1;
		if(vecToTargetNorm.x < 0 && vecToTargetNorm.x > -1)
			vecToTargetNorm.x = -1;

		if(vecToTargetNorm.y < 1 && vecToTargetNorm.y > 0)
			vecToTargetNorm.y = 1;
		if(vecToTargetNorm.y < 0 && vecToTargetNorm.y > -1)
			vecToTargetNorm.y = -1;
	}



	// Update the speed vector if needed
	if(gotoTarget)
	{
		float spdX, spdY;
		spdX = vecToTargetNorm.x;
		spdY = vecToTargetNorm.y;

		if(spdX > 0 && spdY == 0)
			spdX += 0.5;
		else if (spdX < 0 && spdY == 0)
			spdX -= 0.5;

		if(spdY > 0 && spdX == 0)
			spdY += 0.5;
		else if (spdY < 0 && spdX == 0)
			spdY -= 0.5;

		vecSpeed.x = spdX * C_ZOMBIE_STEP_SIZE;
		vecSpeed.y = spdY * C_ZOMBIE_STEP_SIZE;
	}
	else
	{
		vecSpeed.x = 0;
		vecSpeed.y = 0;
	}



	// Move the zombie
	x += vecSpeed.x * FpsCalc::GetInstance()->speedfactor;
	y += vecSpeed.y * FpsCalc::GetInstance()->speedfactor;







	// Check for collisions with walls
	if(cellDown->type == 1 || cellDown->type == 2 || cellDown->type == 3 || cellDown->type == 4)
	{
		sf::FloatRect rup;
		rup.left = cellDown->x*16;
		rup.top = cellDown->y*16;
		rup.width = 16;
		rup.height = 16;

		sf::FloatRect zomb;
		zomb.left = x;
		zomb.top = y;
		zomb.width = 14+1;
		zomb.height = 14+1;

		if(zomb.intersects(rup))
		{
			y -= vecSpeed.y * 1 * FpsCalc::GetInstance()->speedfactor;
		}
	}

	if(cellUp->type == 1 || cellUp->type == 2 || cellUp->type == 3 || cellUp->type == 4)
	{
		sf::FloatRect rup;
		rup.left = cellUp->x*16;
		rup.top = cellUp->y*16;
		rup.width = 16;
		rup.height = 16;

		sf::FloatRect zomb;
		zomb.left = x+1;
		zomb.top = y+1;
		zomb.width = 14;
		zomb.height = 14;

		if(zomb.intersects(rup))
		{
			y -= vecSpeed.y * 1 * FpsCalc::GetInstance()->speedfactor;
		}
	}

	if(cellLeft->type == 1 || cellLeft->type == 2 || cellLeft->type == 3 || cellLeft->type == 4)
	{
		sf::FloatRect rup;
		rup.left = cellLeft->x*16;
		rup.top = cellLeft->y*16;
		rup.width = 16;
		rup.height = 16;

		sf::FloatRect zomb;
		zomb.left = x+1;
		zomb.top = y+1;
		zomb.width = 14;
		zomb.height = 14;

		if(zomb.intersects(rup))
		{
			x -= vecSpeed.x * 1 * FpsCalc::GetInstance()->speedfactor;
		}
	}

	if(cellRight->type == 1 || cellRight->type == 2 || cellRight->type == 3 || cellRight->type == 4)
	{
		sf::FloatRect rup;
		rup.left = cellRight->x*16;
		rup.top = cellRight->y*16;
		rup.width = 16;
		rup.height = 16;

		sf::FloatRect zomb;
		zomb.left = x+1;
		zomb.top = y+1;
		zomb.width = 14;
		zomb.height = 14;

		if(zomb.intersects(rup))
		{
			x -= vecSpeed.x * 1 * FpsCalc::GetInstance()->speedfactor;
		}
	}


	


	if(cellUpLeft->type == 1 || cellUpLeft->type == 2 || cellUpLeft->type == 3 || cellUpLeft->type == 4)
	{
		sf::FloatRect rup;
		rup.left = cellUpLeft->x*16;
		rup.top = cellUpLeft->y*16;
		rup.width = 16;
		rup.height = 16;

		sf::FloatRect zomb;
		zomb.left = x+1;
		zomb.top = y+1;
		zomb.width = 14;
		zomb.height = 14;

		if(zomb.intersects(rup))
		{
			//if(vecSpeed.y != 0 || vecSpeed.y != 1 || vecSpeed.y != -1)
				//x -= vecSpeed.x * 1 * FpsCalc::GetInstance()->speedfactor;
			//if(vecSpeed.x != 0 || vecSpeed.x != 1 || vecSpeed.x != -1)
				//y -= vecSpeed.y * 1 * FpsCalc::GetInstance()->speedfactor;
		}
	}

	if(cellUpRight->type == 1 || cellUpRight->type == 2 || cellUpRight->type == 3 || cellUpRight->type == 4)
	{
		sf::FloatRect rup;
		rup.left = cellUpRight->x*16;
		rup.top = cellUpRight->y*16;
		rup.width = 16;
		rup.height = 16;

		sf::FloatRect zomb;
		zomb.left = x+1;
		zomb.top = y+1;
		zomb.width = 14;
		zomb.height = 14;

		if(zomb.intersects(rup))
		{
			//if(vecSpeed.y != 0 || vecSpeed.y != 1 || vecSpeed.y != -1)
				//x -= vecSpeed.x * 1 * FpsCalc::GetInstance()->speedfactor;
			//if(vecSpeed.x != 0 || vecSpeed.x != 1 || vecSpeed.x != -1)
				//y -= vecSpeed.y * 1 * FpsCalc::GetInstance()->speedfactor;
		}
	}

	if(cellDownLeft->type == 1 || cellDownLeft->type == 2 || cellDownLeft->type == 3 || cellDownLeft->type == 4)
	{
		sf::FloatRect rup;
		rup.left = cellDownLeft->x*16;
		rup.top = cellDownLeft->y*16;
		rup.width = 16;
		rup.height = 16;

		sf::FloatRect zomb;
		zomb.left = x+1;
		zomb.top = y+1;
		zomb.width = 14;
		zomb.height = 14;

		if(zomb.intersects(rup))
		{
			//if(vecSpeed.y != 0 || vecSpeed.y != 1 || vecSpeed.y != -1)
				//x -= vecSpeed.x * 1 * FpsCalc::GetInstance()->speedfactor;
			//if(vecSpeed.x != 0 || vecSpeed.x != 1 || vecSpeed.x != -1)
				//y -= vecSpeed.y * 1 * FpsCalc::GetInstance()->speedfactor;
		}
	}

	if(cellDownRight->type == 1 || cellDownRight->type == 2 || cellDownRight->type == 3 || cellDownRight->type == 4)
	{
		sf::FloatRect rup;
		rup.left = cellDownRight->x*16;
		rup.top = cellDownRight->y*16;
		rup.width = 16;
		rup.height = 16;

		sf::FloatRect zomb;
		zomb.left = x+1;
		zomb.top = y+1;
		zomb.width = 14;
		zomb.height = 14;

		if(zomb.intersects(rup))
		{
			//if(vecSpeed.y != 0 || vecSpeed.y != 1 || vecSpeed.y != -1)
				//x -= vecSpeed.x * 1 * FpsCalc::GetInstance()->speedfactor;
			//if(vecSpeed.x != 0 || vecSpeed.x != 1 || vecSpeed.x != -1)
				//y -= vecSpeed.y * 1 * FpsCalc::GetInstance()->speedfactor;
		}
	}







	// decide if the zombie should attack the target
	dist = ut::distanceBetween(x+8,y+8,*global::flPlayerX+8,*global::flPlayerY+8);
	if(gotoTarget)
	{
		if(dist <= 18 && attackTimer.getElapsedTime().asSeconds() >= 1)
		{
			// hit
			attackTimer.restart();
			global::zombieX = vecToTargetNorm.x;
			global::zombieY = vecToTargetNorm.y;
			global::zombieDmg = dmgVal;
			global::zombieHitPlayerFlag = true;
		}
	}


	// Check for collision with player
	sf::FloatRect fr,pr;
	fr.left = x;
	fr.top = y;
	fr.width = 16;
	fr.height = 16;

	pr.left = *global::flPlayerX;
	pr.top = *global::flPlayerY;
	pr.width = 16;
	pr.height = 16;
	if(fr.intersects(pr))
	{
		x -= vecSpeed.x * FpsCalc::GetInstance()->speedfactor;
		y -= vecSpeed.y * FpsCalc::GetInstance()->speedfactor;
	}

}


///
// Renders the zombie
///
void zombie::draw()
{
	if(dead)
		return;

	if(playerLeft)
	{
		SPR_left.setPosition(x,y);
		global::rwpWindow->draw(SPR_left);
	}
	else
	{
		SPR_right.setPosition(x,y);
		global::rwpWindow->draw(SPR_right);
	}
}



///
// Subtracts hp from the zombie and pushes it back from the attacker
///
void zombie::damage(sf::Vector2f vecToZombie, int dmgAmmount)
{
	std::cout << "zombie hit\n";

	// do damage
	curHp -= dmgAmmount;
	if(curHp <= 0)
	{
		dead = true;
	}


	sf::Vector2f vecNorm;
	// move back
	vecNorm.x = vecToZombie.x;
	vecNorm.y = vecToZombie.y;

	float mg = sqrt(vecNorm.x*vecNorm.x + vecNorm.y*vecNorm.y);
	vecNorm.x /= mg;
	vecNorm.y /= mg;

	if(vecNorm.x < 1 && vecNorm.x > 0)
		vecNorm.x = 1;
	if(vecNorm.x < 0 && vecNorm.x > -1)
		vecNorm.x = -1;

	if(vecNorm.y < 1 && vecNorm.y > 0)
		vecNorm.y = 1;
	if(vecNorm.y < 0 && vecNorm.y > -1)
		vecNorm.y = -1;


	x += vecNorm.x*4;
	y += vecNorm.y*4;
}