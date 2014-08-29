#include "Dog.h"
#include "keyinput/KeyboardListener.hpp"
#include <stdlib.h>
#include <time.h>

#include "PathPlanner.h"
#include "PathPlannerNode.h"

string Dog::getActorName()
{
  return "Dog";
}


void Dog::doInitialSetup()
{
		velLinear = 0.0;
    velRotational = 0.0;
}

void Dog::doExecuteLoop()
{
	KeyboardListener &keyboardListener = KeyboardListener::getInstance();
	velRotational = 0.0;
	velLinear = 0.0;
	
	if (keyboardListener.isKeyPressed(ups::KEY_W_CODE))
	{
		velLinear += 1.0;
	}
	
	if (keyboardListener.isKeyPressed(ups::KEY_S_CODE))
	{
		velLinear -= 1.0;
	}
	
	if (keyboardListener.isKeyPressed(ups::KEY_A_CODE))
	{
		velRotational += 1.0;
	}
	
	if (keyboardListener.isKeyPressed(ups::KEY_D_CODE))
	{
		velRotational -= 1.0;
	}
}

