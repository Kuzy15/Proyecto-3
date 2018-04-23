#include "EF_Entities.h"
#include "Entity.h"
#include "Components.h"
#include "Scenes.h"
#include "Game.h"

//-------------------------------------------    GODS    -------------------------------------------//
#pragma region Gods
//Ra
Entity* createGodRa(std::string id, GameScene* s, Ogre::Vector3 iniPos){

	Entity* Ra = new Entity(id, s);

	//Mesh Render
	Ra->addComponent(new CMeshRender({2,2,0}, "Ra.mesh", Ra, s->getSceneManager(), { 1.0f, 1.0f, 1.0f }));

	//RigidBody
	float height = 6.0f;
	float width = 4.0f;
	Ra->addComponent(new CRigidBody(Ra, s->getGame()->getPhysicsWorld(), iniPos, height, width, RB_DYNAMIC, SH_PLAYER, MASK_PLAYER));

	//PlayerController
	Ra->addComponent(new CPlayerController(Ra, 0));
	//CollisionHandler
	Ra->addComponent(new CPlayerCollisionHandler(Ra));

	//LIfe
	Ra->addComponent(new CLife(Ra, 100.0f));
	//Jump
	float jumpForce = 75.0f;
	Ra->addComponent(new CPlayerJump(Ra, jumpForce));
	//Move
	float v = 0.8f;
	Ra->addComponent(new CPlayerMove(Ra, v));
	//Basic Attack


	return Ra;
}

#pragma endregion

#pragma region Bullets

Entity* createBulletRa(std::string id, GameScene* s, Ogre::Vector3 iniPos){

	Entity * bRa = new Entity(id, s);
	//Mesh Render
	bRa->addComponent(new CMeshRender({1,1,0},"Bullet_Ra", bRa, s->getSceneManager(), { 1.0f, 1.0f, 1.0f }));

	//RigidBody
	float weight = 0.0f;
	float heigth = 0.0f;
	Ogre::Vector3 pos;
	RigidBodyType rT;
	ShapeType sT;
	FilterMask fM;
	bRa->addComponent(new CRigidBody(bRa, s->getGame()->getPhysicsWorld(), pos, heigth, weight, rT, sT, fM));

	//Bullet
	bRa->addComponent(new CBullet(bRa, EB_RA,10.f,10.0f));

	return bRa;


}
#pragma endregion