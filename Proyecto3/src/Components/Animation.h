#ifndef ANIMATION_H
#define ANIMATION_H
#include "Components.h"

class Entity;
class Message;

//--------- ANIMATION COMPONENT ---------
class CAnimation : public GameComponent
{
public:

	CAnimation(Entity * father, Ogre::SceneManager * scnM, Ogre::SceneNode* child);
	~CAnimation();


	virtual void tick(float delta);
	virtual void getMessage(Message * m);

private:

	void changeAnim(Ogre::AnimationState* nextB, Ogre::AnimationState* nextT, bool loop, bool shoot);
	bool _air;
	bool isShooting;
	bool starting;
	Ogre::Entity * pOgreEnt;
	Ogre::SceneNode* pChild;
	//Animation pointers (Bot and Top)
	Ogre::AnimationState* idleBot;
	Ogre::AnimationState* moveBot;
	Ogre::AnimationState* jumpBot;
	Ogre::AnimationState* airBot;

	Ogre::AnimationState* idleTop;
	Ogre::AnimationState* moveTop;
	//Ogre::AnimationState* jumpTop;
	Ogre::AnimationState* airTop;
	Ogre::AnimationState* chargeTop;
	Ogre::AnimationState* shootTop;

	Ogre::AnimationState* start;
	//Current animation pointers

	Ogre::AnimationState* currentTop;
	Ogre::AnimationState* currentBot;

	Ogre::AnimationState* nextTop;
	Ogre::AnimationState* nextBot;

};

#endif