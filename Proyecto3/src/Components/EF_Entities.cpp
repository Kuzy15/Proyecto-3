#include "EF_Entities.h"
#include "Entity.h"
#include "Components.h"
#include "Scenes.h"
#include "Game.h"
#include "DebugNew.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//-------------------------------------------    GODS    -------------------------------------------//
#pragma region Gods
//Ra
Entity* createGodRa(std::string id, GameScene* s, Ogre::Vector3 iniPos){
	
	Entity* Ra = new Entity("Ra", s);

	//Mesh Render
	CMeshRender* cM = new CMeshRender({ 2, 2, 0 }, "Ra.mesh", Ra, s->getSceneManager(), { 1.0f, 1.0f, 1.0f }, { 0, 90, 0 });
	Ra->addComponent(cM);

	//RigidBody
	Ogre::Vector3 size = cM->getSize();
	Ra->addComponent(new CRigidBody(Ra, s->getGame()->getPhysicsWorld(), iniPos, size.y, size.x, RB_DYNAMIC, SH_PLAYER, MASK_PLAYER));

	//PlayerController
	Ra->addComponent(new CPlayerController(Ra, 0));
	//CollisionHandler
	Ra->addComponent(new CPlayerCollisionHandler(Ra));
	//Basic Attack
	Ra->addComponent(new CPlayerBasicAttack(Ra, 1000.0f, EB_RA,iniPos,10.0f));

	//LIfe
	Ra->addComponent(new CLife(Ra, 100.0f));
	//Jump
	float jumpForce = 150.0f;
	Ra->addComponent(new CPlayerJump(Ra, jumpForce));
	//Move
	float v = 8.0f;
	Ra->addComponent(new CPlayerMove(Ra, v));
	//Ra->addComponent(new CPSkillHades(Ra, 10.0f, 10.0f));
	//Ra->addComponent(new CPSkillHermes(Ra, 10.0f, 10.0f));
	//Ra->addComponent(new CPSkillUll(Ra,0));
	Ra->addComponent(new CShuHeaddress(Ra, 0));






	
	


	return Ra;
}

Entity* createGodAhPuch(std::string id, GameScene* s, Ogre::Vector3 iniPos){

	Entity* AhPuch = new Entity("AhPuch", s);

	//Mesh Render
	CMeshRender* cM = new CMeshRender({ 2, 2, 0 }, "AhPuch.mesh", AhPuch, s->getSceneManager(), { 1.0f, 1.0f, 1.0f }, { 0, 0, 0 });
	AhPuch->addComponent(cM);

	//RigidBody
	Ogre::Vector3 size = cM->getSize();
	AhPuch->addComponent(new CRigidBody(AhPuch, s->getGame()->getPhysicsWorld(), iniPos, size.y, size.x, RB_DYNAMIC, SH_PLAYER, MASK_PLAYER));

	//PlayerController
	AhPuch->addComponent(new CPlayerController(AhPuch, 1));
	//CollisionHandler
	AhPuch->addComponent(new CPlayerCollisionHandler(AhPuch));
	//Basic Attack
	AhPuch->addComponent(new CPlayerBasicAttack(AhPuch, 1000.0f, EB_AHPUCH, iniPos, 10.0f));

	//LIfe
	AhPuch->addComponent(new CLife(AhPuch, 100.0f));
	//Jump
	float jumpForce = 75.0f;
	AhPuch->addComponent(new CPlayerJump(AhPuch, jumpForce));
	//Move
	float v = 5.0f;
	AhPuch->addComponent(new CPlayerMove(AhPuch, v));


	return AhPuch;
}


#pragma endregion

#pragma region Bullets

Entity* createBulletRa(std::string id, GameScene* s, Ogre::Vector3 iniPos, float angle, float damage){

	Entity * bRa = new Entity(id, s);
	//Mesh Render

	Ogre::Vector3 pos = iniPos;
	bRa->addComponent(new CMeshRender(pos, "WoodPallet.mesh", bRa, s->getSceneManager(), { 0.4f, 0.4f, 0.4f }, {0,0,angle}));


	//RigidBody
	float weight = 1.0f;
	float heigth = 0.5f;
	bRa->addComponent(new CRigidBody(bRa, s->getGame()->getPhysicsWorld(), pos, heigth, weight, RB_DYNAMIC, SH_POLYGON, MASK_BULLET));

	//Bullet
	bRa->addComponent(new CBullet(bRa, EB_RA, damage, 500.0f));


	return bRa;


}

Entity* createBulletAhPuch(std::string id, GameScene* s, Ogre::Vector3 iniPos, float angle, float damage){

	Entity * bAhPuch = new Entity(id, s);
	//Mesh Render

	Ogre::Vector3 pos = iniPos;
	bAhPuch->addComponent(new CMeshRender(pos, "fish.mesh", bAhPuch, s->getSceneManager(), { 0.1f, 0.1f, 0.1f }, {0,0,angle}));


	//RigidBody
	float weight = 1.0f;
	float heigth = 0.5f;
	bAhPuch->addComponent(new CRigidBody(bAhPuch, s->getGame()->getPhysicsWorld(), pos, heigth, weight, RB_DYNAMIC, SH_POLYGON, MASK_BULLET));

	//Bullet
	bAhPuch->addComponent(new CBullet(bAhPuch, EB_AHPUCH, damage, 20.0f));


	return bAhPuch;


}

#pragma endregion