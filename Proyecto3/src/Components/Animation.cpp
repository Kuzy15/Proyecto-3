#include "Animation.h"
#include "Entity.h"

#pragma region Animation Component
CAnimation::CAnimation(Entity * father, Ogre::SceneManager * scnM, Ogre::SceneNode* child) : GameComponent(CMP_ANIMATION, father){
	pOgreEnt = scnM->getEntity(father->getID());
	pChild = child;
	isShooting = false;
	Ogre::Skeleton* sk = pOgreEnt->getSkeleton();

	idleBot = pOgreEnt->getAnimationState("idleBot");
	moveBot = pOgreEnt->getAnimationState("moveBot");
	jumpBot = pOgreEnt->getAnimationState("jumpBot");
	airBot = pOgreEnt->getAnimationState("airBot");

	idleTop = pOgreEnt->getAnimationState("idleTop");
	moveTop = pOgreEnt->getAnimationState("moveTop");
	//jumpTop = pOgreEnt->getAnimationState("Jump Top");
	airTop = pOgreEnt->getAnimationState("airTop");
	//chargeTop = pOgreEnt->getAnimationState("chargeTop");
	shootTop = pOgreEnt->getAnimationState("shootTop");
	start = pOgreEnt->getAnimationState("start");

	start->setLoop(false);
	idleBot->setLoop(true);
	moveBot->setLoop(true);
	jumpBot->setLoop(false);
	airBot->setLoop(true);

	idleTop->setLoop(true);
	moveTop->setLoop(true);
	//jumpTop->setLoop(false);
	airTop->setLoop(true);
	//chargeTop->setLoop(false);
	shootTop->setLoop(false);
	//Initial rotation
	pChild->setOrientation(Ogre::Quaternion(Ogre::Degree(-90), Ogre::Vector3(0, 1, 0)));


	currentBot = idleBot;
	currentBot->setEnabled(true);
	currentTop = idleTop;
	currentTop->setEnabled(true);
	nextBot = nullptr;
	nextTop = nullptr;
	starting = false;

}
CAnimation::~CAnimation(){
}


void CAnimation::tick(float delta){

	//std::cout << currentBot->getAnimationName() << std::endl;
	//std::cout << currentTop->getAnimationName() << std::endl;

	/*if (nextBot != nullptr){
	currentBot = nextBot;
	nextBot = nullptr;
	}
	*/

	if (currentTop->getLength() == currentTop->getTimePosition()){
		currentTop->setEnabled(false);
		if (nextTop != nullptr){
			nextTop->setTimePosition(0);
			nextTop->setEnabled(true);
			currentTop = nextTop;
		}
		isShooting = false;
		starting = false;
	}

	if (currentBot->getLength() == currentBot->getTimePosition()){
		currentBot->setEnabled(false);
		if (nextBot != nullptr){
			nextBot->setTimePosition(0);
			nextBot->setEnabled(true);
			currentBot = nextBot;
		}

	}


	if (starting){
		currentTop->addTime(delta);
	}
	else{
		currentTop->addTime(delta * 3);
		currentBot->addTime(delta * 3);
	}

}
void CAnimation::getMessage(Message * m){
	MPlayerMoveX* mMoveX;
	float angle;
	float dir;
	switch (m->getType())
	{
	case MSG_PLAYER_MOVE_X:
		mMoveX = static_cast<MPlayerMoveX*>(m);


		if (!isShooting){
			dir = mMoveX->GetValue();
			angle = pChild->getOrientation().getYaw().valueDegrees();

			if (dir != 0){
				if (dir < 0){
					pChild->setOrientation(Ogre::Quaternion(Ogre::Degree(-angle), Ogre::Vector3(0, 1, 0)));
					pChild->setOrientation(Ogre::Quaternion(Ogre::Degree(-90), Ogre::Vector3(0, 1, 0)));
				}
				else{
					pChild->setOrientation(Ogre::Quaternion(Ogre::Degree(-angle), Ogre::Vector3(0, 1, 0)));
					pChild->setOrientation(Ogre::Quaternion(Ogre::Degree(90), Ogre::Vector3(0, 1, 0)));

				}
			}
		}



		if (mMoveX->GetValue() != 0){
			if (_air)
				changeAnim(airBot, airTop, false, false);
			else
				changeAnim(moveBot, moveTop, true, false);

		}
		else{
			changeAnim(idleBot, idleTop, true, false);
		}
		break;

	case MSG_RIGIDBODY_JUMP:
		changeAnim(jumpBot, airTop, false, false);
		_air = true;
		break;

	case MSG_COLLISION_TERRAIN:
		_air = false;
		break;

	case MSG_SHOT:

		isShooting = true;
		angle = pChild->getOrientation().getYaw().valueDegrees();

		dir = static_cast<MPlayerShot*>(m)->getXValue();
		if (dir != 0){
			if (dir < 0){
				pChild->setOrientation(Ogre::Quaternion(Ogre::Degree(-angle), Ogre::Vector3(0, 1, 0)));
				pChild->setOrientation(Ogre::Quaternion(Ogre::Degree(-90), Ogre::Vector3(0, 1, 0)));
			}
			else{
				pChild->setOrientation(Ogre::Quaternion(Ogre::Degree(-angle), Ogre::Vector3(0, 1, 0)));
				pChild->setOrientation(Ogre::Quaternion(Ogre::Degree(90), Ogre::Vector3(0, 1, 0)));

			}
		}
		changeAnim(currentBot, shootTop, false, true);

		break;
	default:
		break;
	}
}
void CAnimation::changeAnim(Ogre::AnimationState* nextB, Ogre::AnimationState* nextT, bool loop, bool shoot){

	if (nextT != nullptr){
		if (currentTop->getLoop()){
			if (nextT != currentTop){
				currentTop->setEnabled(false);
				nextT->setTimePosition(0);
				nextT->setEnabled(true);
				nextT->setLoop(loop);
				currentTop = nextT;
			}
		}
		else{

			if (shoot){
				if (nextT != currentTop){
					currentTop->setEnabled(false);
					nextT->setTimePosition(0);
					nextT->setEnabled(true);
					nextT->setLoop(loop);
					currentTop = nextT;
				}
			}
			else{
				nextTop = nextT;
				nextTop->setLoop(loop);

			}

		}
	}

	if (nextB != nullptr){

		if (currentBot->getLoop()){
			if (nextB != currentBot){
				currentBot->setEnabled(false);
				nextB->setTimePosition(0);
				nextB->setEnabled(true);
				nextB->setLoop(loop);
				currentBot = nextB;
			}
		}
		else{

			nextBot = nextB;
			nextBot->setLoop(loop);

		}
	}





}
#pragma endregion
