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
	y = xpos;
	maxHp = MaxHp;
	dmgVal = dmgStrength;
	gotoTarget = false;
	tx = 0;
	ty = 0;

	attackTimer.restart();

	distanceThreshold = 300;
}




///
// Updates the zombie
///
void zombie::step()
{

	// Calculate the current distance to target
	float dist = ut::distanceBetween(x+8,y+8,*global::flPlayerX+8,*global::flPlayerY+8);



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

		float mg = sqrt(x*x + y*y);
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
		vecSpeed.x = (vecToTargetNorm.x) * C_ZOMBIE_STEP_SIZE;
		vecSpeed.y = (vecToTargetNorm.y) * C_ZOMBIE_STEP_SIZE;
	}
	else
	{
		vecSpeed.x = 0;
		vecSpeed.y = 0;
	}



	// Move the zombie
	x += vecSpeed.x * FpsCalc::GetInstance()->speedfactor;
	y += vecSpeed.y * FpsCalc::GetInstance()->speedfactor;



	// decide if the zombie should attack the target
	dist = ut::distanceBetween(x+8,y+8,*global::flPlayerX+8,*global::flPlayerY+8);
	if(gotoTarget)
	{
		if(dist <= 16 && attackTimer.getElapsedTime().asSeconds() >= 1)
		{
			// hit
			attackTimer.restart();
			global::zombieX = vecToTarget.x;
			global::zombieY = vecToTarget.y;
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
	sf::RectangleShape rty;
	rty.setPosition(x,y);
	rty.setSize(sf::Vector2f(16,16));
	rty.setFillColor(sf::Color(100,255,100));

	global::rwpWindow->draw(rty);
}



///
// Subtracts hp from the zombie and pushes it back from the attacker
///
void zombie::damage(sf::Vector2f vecToZombie, int dmgAmmount)
{
}