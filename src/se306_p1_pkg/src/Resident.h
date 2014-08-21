#ifndef SE306P1_ACTOR_RESIDENT_H_DEFINED
#define SE306P1_ACTOR_RESIDENT_H_DEFINED

#include "Human.h"
#include <msg_pkg/Interaction.h>
#include "ros/ros.h"

<<<<<<< HEAD
// Typedef for dbl-precision printing in randomEventLoop()
typedef std::numeric_limits< double > dbl;
// Uncomment this to use Conor's very high-quality (but sometimes blocking) PRNG stream.
//#define USE_DEV_RANDOM

class Resident : public Human
{
public:
  virtual bool isLocked();
  virtual void lock();
  virtual void unlock();
  
protected:
  virtual void doInitialSetup();
  virtual void doExecuteLoop();
  static void interactionCallback(msg_pkg::Interaction msg);
  static float getRandom(float, float);
  void randomEventLoop();
  
private:
  bool lock_;

  // Demo paramters to graduall reduce levels
  int entertainment_count_;
  int socialness_count_;
  bool e_dropped_;
  bool s_dropped_;
  const static int WAIT_TIME = 50;
  bool e_replenished_;

	/* Randomness variables 
	int randomData;
	int myRandomInteger;
	size_t randomDataLen;
	*/

	// Delay measurement variables
	long long msAtPreviousLoop;
	float randNum;


#ifdef USE_DEV_RANDOM
	// Randomness variables	
	int randomData;
	int myRandomInteger;
	size_t randomDataLen;
#endif    

  // Levels (1 = critical, attention required; 
	// 5 = optimal, 'maximum')
  int socialness_level_;
  int entertainedness_level_;

	// Level publishers

  // Publisher for socialness
  ros::Publisher publisherSocialness;
  // Publisher for entertainedness
  ros::Publisher publisherEntertainedness;

	// Event subscribers

  // Subscriber for interactions
  ros::Subscriber subscriberInteraction;

  static int getNewLevel(int amount, int oldValue);
  void stopRobotSpinning();
};


#endif

