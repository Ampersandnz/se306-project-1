#include "Caregiver.h"
#include <stdlib.h>
#include <time.h>
#include <string>

#include "PathPlanner.h"
#include "PathPlannerNode.h"

string Caregiver::getActorName()
{
    return "Caregiver";
}

void Caregiver::caring()
{
    if ((hour > 6 && hour < 21) || (locked))
    {
        if (!homeVisit)
        {
            homeVisit = true;
            movingToResident = true;
            // Move to resident
            
        }
        if (movingToResident)
        {
            if (eating)
            {
                if (hungerLevel == 5)
                {
                    stopResponse("eating");
                    eating = false;
                    locked = false;
                }
                else
                {
                    locked = true;
                    if (y == 40)
                    {
                        doResponse("eating");
                        y = 0;
                    }
                    else
                    {
                        y ++;
                    }
                }
            }
            else if (showering)
            {
                // First, moving to bathroom
                if (hygieneLevel == 5)
                {
                    stopResponse("showering");
                    showering = false;
                    locked = false;
                    // Finally, moving back to resident
                }
                else
                {
                    locked = true;
                    if (y == 40)
                    {
                        doResponse("showering");
                        y = 0;
                        // Showering
                    }
                    else
                    {
                        y ++;
                    }
                }
            }
            else if (exercising)
            {
                if (fitnessLevel == 5)
                {
                    stopResponse("exercising");
                    exercising = false;
                    locked = false;
                }
                else
                {
                    locked = true;
                    if (y == 40)
                    {
                        doResponse("exercising");
                        y = 0;
                    }
                    else
                    {
                        y ++;
                    }
                }
            }
            else if (entertaining)
            {
                if (moraleLevel == 5)
                {
                    stopResponse("entertaining");
                    entertaining = false;
                    locked = false;
                }
                else
                {
                    locked = true;
                    if (y == 40)
                    {
                        doResponse("entertaining");
                        y = 0;
                    }
                    else
                    {
                        y ++;
                    }
                }
            }
            else if (socialising)
            {
                if (socialnessLevel == 5)
                {
                    stopResponse("socialising");
                    socialising = false;
                    locked = false;
                }
                else
                {
                    locked = true;
                    if (y == 40)
                    {
                        doResponse("socialising");
                        y = 0;
                    }
                    else
                    {
                        y ++;
                    }
                }
            }
        }
    }
    else
    {
        if (homeVisit)
        {
            homeVisit = false;
            // Back to initial location
        }
    }
}

void Caregiver::doInitialSetup()
{
    velLinear = 0.0;
    velRotational = 0.0;
    nodename = ros::this_node::getName();

    std::size_t found12 = nodename.find_first_of("12");
    std::size_t found13 = nodename.find_first_of("13");
    
    if (found12 != std::string::npos)
    {
        caregiverId = 1;
    }
    else if (found13 != std::string::npos)
    {
        caregiverId = 2;
    }
    else
    {
        //YOU BROKE THE BUILD!
    	ROS_INFO("THIS IS BROKEN! CAREGIVERS SHOULD BE KEPT TO ROSNODE12 AND ROSNODE13");
    	ROS_INFO("MY NAME IS %s", nodename.c_str());
        caregiverId = 0;
        exit(EXIT_FAILURE);
    }

    subscriberFitness = nodeHandle->subscribe("fiteness", 1000, Caregiver::fitnessCallback);
    subscriberHunger = nodeHandle->subscribe("hunger", 1000, Caregiver::hungerCallback);
    subscriberHygiene = nodeHandle->subscribe("hygiene", 1000, Caregiver::hygieneCallback);
    subscriberMorale = nodeHandle->subscribe("morale", 1000, Caregiver::moraleCallback);
    subscriberSocialness = nodeHandle->subscribe("socialness", 1000, Caregiver::socialnessCallback);
    subscriberTime = nodeHandle->subscribe("time", 1000, Caregiver::timeCallback);
    subscriberTelephone = nodeHandle->subscribe("telephone", 1000, Caregiver::telephoneCallback);

    homeVisit = false;
    movingToResident = false;
    odd = true;
    hour = 0;
    y = 0;

    fitnessLevel = 5;
    hungerLevel = 5;
    hygieneLevel = 5;
    moraleLevel = 5;
    socialnessLevel = 5;

    exercising = false;
    eating = false;
    showering = false;
    entertaining = false;
    socialising = false;

    locked = false;
}

void Caregiver::doExecuteLoop()
{
    if (caregiverId == 1)
    {
        if (odd)
        {
            caring();
        }
    }
    else
    {
        if (!odd)
        {
            caring();
        }
    }
}

void Caregiver::fitnessCallback(msg_pkg::Fitness msg)
{
    Caregiver* temp = dynamic_cast<Caregiver*> (ActorSpawner::getInstance().getActor());
    temp->fitnessLevel = msg.level;
    if (!temp->checkFitnessLevel())
    {
        temp->exercising = true;
    }
}

void Caregiver::hungerCallback(msg_pkg::Hunger msg)
{
    Caregiver* temp = dynamic_cast<Caregiver*> (ActorSpawner::getInstance().getActor());
    temp->hungerLevel = msg.level;
    if (!temp->checkHungerLevel())
    {
        temp->eating = true;
    }
}

void Caregiver::hygieneCallback(msg_pkg::Hygiene msg)
{
    Caregiver* temp = dynamic_cast<Caregiver*> (ActorSpawner::getInstance().getActor());
    temp->hygieneLevel = msg.level;
    if (!temp->checkHygieneLevel())
    {
        temp->showering = true;
    }
}

void Caregiver::moraleCallback(msg_pkg::Morale msg)
{
    Caregiver* temp = dynamic_cast<Caregiver*> (ActorSpawner::getInstance().getActor());
    temp->moraleLevel = msg.level;
    if (!temp->checkMoraleLevel())
    {
        temp->entertaining = true;
    }
}

void Caregiver::socialnessCallback(msg_pkg::Socialness msg)
{
    Caregiver* temp = dynamic_cast<Caregiver*> (ActorSpawner::getInstance().getActor());
    temp->socialnessLevel = msg.level;
    if (!temp->checkSocialnessLevel())
    {
        temp->socialising = true;
    }
}

void Caregiver::timeCallback(msg_pkg::Time msg)
{
    Caregiver* temp = dynamic_cast<Caregiver*> (ActorSpawner::getInstance().getActor());

    temp-> hour = msg.hour;

    if (msg.day % 2 != 0)
    {
        temp->odd = true;
    }
    else
    {
        temp->odd = false;
    }
}

void Caregiver::telephoneCallback(msg_pkg::Telephone msg)
{
}

bool Caregiver::checkFitnessLevel()
{
    if (fitnessLevel >= 2)
    {
        return true;
    }
    return false;
}
bool Caregiver::checkHungerLevel()
{
    if (hungerLevel >= 2)
    {
        return true;
    }
    return false;
}
bool Caregiver::checkHygieneLevel()
{
    if (hygieneLevel >= 2)
    {
        return true;
    }
    return false;
}
bool Caregiver::checkMoraleLevel()
{
    if (moraleLevel >= 2)
    {
        return true;
    }
    return false;
}
bool Caregiver::checkSocialnessLevel()
{
    if (socialnessLevel >= 2)
    {
        return true;
    }
    return false;
}