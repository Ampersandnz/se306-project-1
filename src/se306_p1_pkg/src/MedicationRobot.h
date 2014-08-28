#ifndef SE306P1_ACTOR_ENTERTAINMENTROBOT_H_DEFINED
#define SE306P1_ACTOR_ENTERTAINMENTROBOT_H_DEFINED

#include "Robot.h"
#include <msg_pkg/Morale.h>

class MedicationRobot : public Robot
{
protected:
	virtual void doInitialSetup();
	virtual void doExecuteLoop();

	bool checkMedicationLevel();

	static void moraleCallback(msg_pkg::Morale msg);

	string getActorName();

	string getActorName();

	int8_t healthLevel;
	bool health;

	ros::Subscriber subscriberMorale;

	string residentName;
	int y;
	int x;
	bool first;
	bool first_call;
	bool returningHome;
	bool returningHome_first;
};

#endif
