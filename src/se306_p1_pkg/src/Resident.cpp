#include "Resident.h"
#include <string.h>
#ifdef USE_DEV_RANDOM
	#include <fcntl.h> // File access for RNG
#else
	#include <cstdlib>
	#include <ctime>
#endif
#include <msg_pkg/Interaction.h>
#include <msg_pkg/Socialness.h>
#include <msg_pkg/Entertainedness.h>
#include "Actor.h"
#include "ActorSpawner.h"

void Resident::doInitialSetup()
{
  velLinear = 0;
  velRotational = 0.0;

  // Initially the resident is not locked
  lock_ = false;

  // Set levels to maximum initially
  entertainedness_level_ = 5;
  socialness_level_ = 5;

  entertainment_count_ = 0;
  socialness_count_ = 0;
  e_dropped_ = false;
  e_replenished_ = false;

  // Set up a publishers
  publisherSocialness = nodeHandle->advertise<msg_pkg::Socialness>("socialness", 1000);
  publisherEntertainedness = nodeHandle->advertise<msg_pkg::Entertainedness>("entertainedness", 1000);

  // Set up subscribers
  subscriberInteraction = nodeHandle->subscribe("interaction", 1000, Resident::interactionCallback);

#ifndef USE_DEV_RANDOM
  // Seed the PRNG
  std::srand(std::time(NULL));
#endif
}

void Resident::doExecuteLoop()
{
	//PathPlannerNode *target = this->pathPlanner.getNode(&node4Name);
    //vector<PathPlannerNode*> path = this->pathPlanner.pathToNode(this->activeNode,target);
    //this->goToNode(path);
    
////////////////////////////////////////////////////
// _REMOVE (when randomness implementation complete) 

	if (entertainment_count_ >= WAIT_TIME && !e_dropped_)
	{
		Resident* residentInstance = dynamic_cast<Resident*>(ActorSpawner::getInstance().getActor());
		if(residentInstance->entertainedness_level_ <= 1)
		{
			// don't drop the value any more, it's being tended to or has been already
			e_dropped_ = true;
			//ROS_INFO("e_dropped changed");
		}
		else 
		{
			// reduce the level every 1000 counts
			residentInstance->entertainedness_level_--;
			//Create a socialness message to publish
			msg_pkg::Entertainedness entertainednessMessage;
			//Assign current socialness level to the message
			entertainednessMessage.level = residentInstance->entertainedness_level_;
			//Publish the message
			residentInstance->publisherEntertainedness.publish(entertainednessMessage);

		}
		entertainment_count_ = 0;
	}
	else if (entertainment_count_ < WAIT_TIME && !e_dropped_)
	{
		entertainment_count_++;
	}
	else if (e_replenished_ && (socialness_count_ >= WAIT_TIME) && !s_dropped_)
	{
		Resident* residentInstance = dynamic_cast<Resident*>(ActorSpawner::getInstance().getActor());
		if(residentInstance->socialness_level_ <= 1)
		{
			// don't drop the value any more, it's being tended to or has been already
			s_dropped_ = true;
		}
		else 
		{
			
			// reduce the level every 1000 counts
			residentInstance->socialness_level_--;
			//Create a socialness message to publish
			msg_pkg::Socialness socialnessMessage;
			//Assign current socialness level to the message
			socialnessMessage.level = residentInstance->socialness_level_;
			//Publish the message
			residentInstance->publisherSocialness.publish(socialnessMessage);
		}
		socialness_count_ = 0;
	}
	else if (e_replenished_ && (socialness_count_ < WAIT_TIME) && !s_dropped_)
	{
		socialness_count_++;
	}

// _REMOVE (when randomness implementation complete) 
////////////////////////////////////////////////////

	// Call random event for each iteration of the execution
	// loop (not sure if this should happen at start or end
	// of each loop; does this even matter?)
	randomEventLoop();
}

/*
 * A robot should not be able to interact with the resident if the resident is currently locked.
 */
bool Resident::isLocked()
{
  return lock_;
}

/*
 * Robots should only lock the resident if the resident is currently not locked
 */
void Resident::lock()
{
  lock_ = true;
}

/*
 * Robots should unlock the resident after interation * so that another robot may interact with the resident.
 */
void Resident::unlock()
{
  lock_ = false;
}

void Resident::interactionCallback(msg_pkg::Interaction msg)
{

  std::string attribute = msg.attribute;
  int amount = msg.amount;

  // Get the class instance
  Resident* residentInstance = dynamic_cast<Resident*>(ActorSpawner::getInstance().getActor());
  residentInstance->velRotational = 1.0; // Rotate to show is being interacted with

  if (attribute == "socialising")
  {
  	// Get new level
  	int newLevel = getNewLevel(amount, residentInstance->socialness_level_);
	// Update the residents socialness level
	residentInstance->socialness_level_ = newLevel;
	//Create a socialness message to publish
	msg_pkg::Socialness socialnessMessage;
	//Assign current socialness level to the message
	socialnessMessage.level = newLevel;

	if (newLevel == 5)
	{
		residentInstance->stopRobotSpinning();
	}

	//Publish the message
	residentInstance->publisherSocialness.publish(socialnessMessage);
  } 
  else if (attribute == "entertaining")
  {
	// Get new level
	int newLevel = getNewLevel(amount, residentInstance->entertainedness_level_);
	// Update the residents socialness level
	residentInstance->entertainedness_level_ = newLevel;
	//Create a socialness message to publish
	msg_pkg::Entertainedness entertainednessMessage;
	//Assign current socialness level to the message
	entertainednessMessage.level = newLevel;

	if (newLevel == 5)
	{
		residentInstance->stopRobotSpinning();
		residentInstance->e_replenished_ = true;
	}

	//Publish the message
	residentInstance->publisherEntertainedness.publish(entertainednessMessage);
  }
  // TODO: put others in when implemented
}

int Resident::getNewLevel(int amount, int oldLevel)
{
	int newLevel = std::min(amount + oldLevel, 5); // Can only have a maximum level of 5
	// Code to check it doesn't go below 1... just incase interactions can reduce levels at some point
	if (newLevel < 1)
	{
		newLevel = 1;
	}
	return newLevel;
}

void Resident::stopRobotSpinning()
{
  Resident* residentInstance = dynamic_cast<Resident*>(ActorSpawner::getInstance().getActor());
  residentInstance->velRotational = 0.0; // Stop rotation to show interaction finished
}






// Function called for each iteration of 'doExecuteLoop()'; emulates random
// human behaviours and needs
void Resident::randomEventLoop()
{
#ifdef USE_DEV_RANDOM
	//ROS_INFO("Calculating random event(s)...");
	

	// Access/dev/random in read-only for true, random 
	// number generation
	randomData = open("/dev/random", O_RDONLY);
	// Note: /dev/random blocks when it's empty, and it seems to become empty pretty quickly :(
	// It's probably higher quality than we need, but /dev/urandom sounds like it might be an okay compromise
	// It takes the numbers from /dev/random, but also generates new lower-quality pseudo-random numbers instead of blocking
	// I reckon we can get away with using C's rand(), which is just a simple linear congruential generator, but we're not storing passwords or anything.
	
	myRandomInteger;
	randomDataLen = 0;
	
	while (randomDataLen < sizeof myRandomInteger)
	{
		ssize_t randomResult = read(randomData, ((char*)&myRandomInteger) + randomDataLen, (sizeof myRandomInteger) - randomDataLen);

		// ssize_t type will return negative if an error occurs while
		// the random number is being generated and assigned to result
		if (randomResult < 0)
		{
			//ROS_INFO("Unable to read /dev/random, resorting to alternative RNG");
		}

		randomDataLen += randomResult;
	}

	close(randomData);
	//ROS_INFO("Random number generated: %d", randomDataLen);
	
#else // Use C's rand()

	long myRandomInteger = 0;
	
	// RAND_MAX is guaranteed to be >= 32767, but that's 15 bytes, so to cover 32 bytes completely, we'll need to do it in 3 stages
	for (int shiftVal = 0; shiftVal < 32; shiftVal += 15)
	{
		// Fill out myRandomInteger, 15 bits at a time
		myRandomInteger |= (std::rand() & 0x7fff) << shiftVal;
	}
	
	//ROS_INFO("Random number generated: %d", myRandomInteger);
#endif



	// Entertainedness drop
}
