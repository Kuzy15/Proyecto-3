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
Entity* createGodRa(std::string id, GameScene* s, Ogre::Vector3 iniPos, int controllerId){
	
	Entity* Ra = new Entity("Player_" + std::to_string(controllerId), s);

	//Mesh Render
	CMeshRender* cM = new CMeshRender({ 2, 2, 0 }, "Ra.mesh", Ra, s->getSceneManager(), { 1.0f, 1.0f, 1.0f }, { 0, 90, 0 });
	Ra->addComponent(cM);

	//RigidBody
	Ogre::Vector3 size = cM->getSize();
	Ra->addComponent(new CRigidBody(Ra, s->getGame()->getPhysicsWorld(), iniPos, size.y, size.x, RB_DYNAMIC, SH_PLAYER, MASK_PLAYER));

	//PlayerController
	Ra->addComponent(new CPlayerController(Ra, controllerId));
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
	Ra->addComponent(new CShuHeaddress(Ra, controllerId));






	
	


	return Ra;
}

Entity* createGodAhPuch(std::string id, GameScene* s, Ogre::Vector3 iniPos, int controllerId){

	Entity* AhPuch = new Entity("Player_" + std::to_string(controllerId), s);

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

#pragma region Stages

std::vector<Entity*> createStageTemple(GameScene* s){

	std::vector<Entity*> entities;

	s->getSceneManager()->setAmbientLight(Ogre::ColourValue(.5, .5, .5));

	Ogre::Light* light = s->getSceneManager()->createLight("MainLight");
	light->setPosition(20, 80, 50);

	Ogre::Viewport* vp = nullptr;
	Entity * cam = new Entity("Camera1", s);
	cam->addComponent(new CActionCamera(cam, s->getSceneManager(), vp));
	entities.push_back(cam);

	Entity * e1 = new Entity("Suelo_01", s);
	CMeshRender* cM = new CMeshRender({ 45, -20, -30 }, "suelo.mesh", e1, s->getSceneManager(), { 100.0f, 100.0f, 100.0f }, { 0, -90.0f, 0 });
	e1->addComponent(cM);
	entities.push_back(e1);

	Entity * Suelo = new Entity("ColliderSuelo_01", s);
	Suelo->addComponent(new CRigidBody(Suelo, s->getGame()->getPhysicsWorld(), Ogre::Vector3(-100, -15, 0), 3, cM->getSize().x, RB_STATIC, SH_POLYGON, MASK_STATIC_TERRAIN));
	//Suelo->addComponent(new CMeshRender(Ogre::Vector3(0, 0, 0), "Barrel.mesh", Suelo, scnMgr));
	entities.push_back(Suelo);

	Entity * porton = new Entity("Porton_01", s);
	porton->addComponent(new CMeshRender({ 0, -10, -10 }, "porton.mesh", porton, s->getSceneManager(), { 1.0f, 1.0f, 1.0f }, { 0, 50, 180 }));
	entities.push_back(porton);

	Entity * rightEdge = new Entity("ColliderLimite_Dcho", s);
	rightEdge->addComponent(new CRigidBody(rightEdge, s->getGame()->getPhysicsWorld(), Ogre::Vector3(55, -15, 0), 50, 2, RB_STATIC, SH_POLYGON, MASK_STATIC_TERRAIN));
	entities.push_back(rightEdge);

	Entity * temple = new Entity("Templo_01", s);
	temple->addComponent(new CMeshRender({ -30, -2, -20 }, "templo.mesh", temple, s->getSceneManager(), { 10.0f, 10.0f, 10.0f }, { 0, 300, 0 }));
	entities.push_back(temple);

	Entity * portonCollider = new Entity("ColliderPorton_01", s);
	portonCollider->addComponent(new CRigidBody(portonCollider, s->getGame()->getPhysicsWorld(), Ogre::Vector3(-2, 2, 0), 2, 15, RB_STATIC, SH_POLYGON, MASK_STATIC_TERRAIN));
	entities.push_back(portonCollider);

	return entities;

}

#pragma endregion