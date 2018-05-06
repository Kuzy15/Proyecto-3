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
	CMeshRender* cM = new CMeshRender({ 2, 2, 0 }, "Ra.mesh", Ra, s->getSceneManager(), { 1.0f, 1.0f, 1.0f }, { 0, 0, 0 });
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
	Ra->addComponent(new CPSkillVidar(Ra));
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
	AhPuch->addComponent(new CPlayerController(AhPuch, controllerId));
	//CollisionHandler
	AhPuch->addComponent(new CPlayerCollisionHandler(AhPuch));
	//Basic Attack
	AhPuch->addComponent(new CPlayerBasicAttack(AhPuch, 500.0f, EB_AHPUCH, iniPos, 2.0f));

	//LIfe
	AhPuch->addComponent(new CLife(AhPuch, 100.0f));
	//Jump
	float jumpForce = 150.0f;
	AhPuch->addComponent(new CPlayerJump(AhPuch, jumpForce));
	//Move
	float v = 8.0f;
	AhPuch->addComponent(new CPlayerMove(AhPuch, v));


	return AhPuch;
}


Entity* createGodZeus(std::string id, GameScene* s, Ogre::Vector3 iniPos, int controllerId){

	Entity* Zeus = new Entity("Player_" + std::to_string(controllerId), s);

	//Mesh Render
	CMeshRender* cM = new CMeshRender({ 2, 2, 0 }, "AhPuch.mesh", Zeus, s->getSceneManager(), { 1.0f, 1.0f, 1.0f }, { 0, 0, 0 });
	Zeus->addComponent(cM);

	//RigidBody
	Ogre::Vector3 size = cM->getSize();
	Zeus->addComponent(new CRigidBody(Zeus, s->getGame()->getPhysicsWorld(), iniPos, size.y, size.x, RB_DYNAMIC, SH_PLAYER, MASK_PLAYER));

	//PlayerController
	Zeus->addComponent(new CPlayerController(Zeus, controllerId));
	//CollisionHandler
	Zeus->addComponent(new CPlayerCollisionHandler(Zeus));
	//Basic Attack
	Zeus->addComponent(new CPlayerBasicAttack(Zeus, 1000.0f, EB_ZEUS, iniPos, 7.0f));

	//LIfe
	Zeus->addComponent(new CLife(Zeus, 100.0f));
	//Jump
	float jumpForce = 150.0f;
	Zeus->addComponent(new CPlayerJump(Zeus, jumpForce));
	//Move
	float v = 8.0f;
	Zeus->addComponent(new CPlayerMove(Zeus, v));


	return Zeus;
}



Entity* createGodHachiman(std::string id, GameScene* s, Ogre::Vector3 iniPos, int controllerId){

	Entity* Hachiman = new Entity("Player_" + std::to_string(controllerId), s);

	//Mesh Render
	CMeshRender* cM = new CMeshRender({ 2, 2, 0 }, "AhPuch.mesh", Hachiman, s->getSceneManager(), { 1.0f, 1.0f, 1.0f }, { 0, 0, 0 });
	Hachiman->addComponent(cM);

	//RigidBody
	Ogre::Vector3 size = cM->getSize();
	Hachiman->addComponent(new CRigidBody(Hachiman, s->getGame()->getPhysicsWorld(), iniPos, size.y, size.x, RB_DYNAMIC, SH_PLAYER, MASK_PLAYER));

	//PlayerController
	Hachiman->addComponent(new CPlayerController(Hachiman, controllerId));
	//CollisionHandler
	Hachiman->addComponent(new CPlayerCollisionHandler(Hachiman));
	//Basic Attack
	Hachiman->addComponent(new CPlayerBasicAttack(Hachiman, 750.0f, EB_HACHIMAN, iniPos, 4.0f));

	//LIfe
	Hachiman->addComponent(new CLife(Hachiman, 100.0f));
	//Jump
	float jumpForce = 150.0f;
	Hachiman->addComponent(new CPlayerJump(Hachiman, jumpForce));
	//Move
	float v = 8.0f;
	Hachiman->addComponent(new CPlayerMove(Hachiman, v));


	return Hachiman;
}


#pragma endregion

#pragma region Bullets

Entity* createBulletRa(std::string id, GameScene* s, Ogre::Vector3 iniPos, float angle, float damage){

	Entity * bRa = new Entity(id, s);
	//Mesh Render

	Ogre::Vector3 pos = iniPos;
	bRa->addComponent(new CMeshRender(pos, "balas/EnergyBall.mesh", bRa, s->getSceneManager(), { 1.0f, 1.0f, 1.0f }, {0,0,angle}));


	//RigidBody
	float weight = 1.0f;
	float heigth = 0.5f;
	bRa->addComponent(new CRigidBody(bRa, s->getGame()->getPhysicsWorld(), pos, heigth, weight, RB_DYNAMIC, SH_POLYGON, MASK_BULLET));

	//Bullet
	bRa->addComponent(new CBullet(bRa, EB_RA, damage, 400.0f));


	return bRa;


}

Entity* createBulletAhPuch(std::string id, GameScene* s, Ogre::Vector3 iniPos, float angle, float damage){

	Entity * bAhPuch = new Entity(id, s);
	//Mesh Render

	Ogre::Vector3 pos = iniPos;
	bAhPuch->addComponent(new CMeshRender(pos, "balas/Cerbatana.mesh", bAhPuch, s->getSceneManager(), { 0.25f, 1.0f, 0.25f }, { 0, 0,angle }));


	//RigidBody
	float weight = 1.0f;
	float heigth = 0.5f;
	bAhPuch->addComponent(new CRigidBody(bAhPuch, s->getGame()->getPhysicsWorld(), pos, heigth, weight, RB_DYNAMIC, SH_POLYGON, MASK_BULLET));

	//Bullet
	bAhPuch->addComponent(new CBullet(bAhPuch, EB_AHPUCH, damage, 700.0f));


	return bAhPuch;


}


Entity* createBulletZeus(std::string id, GameScene* s, Ogre::Vector3 iniPos, float angle, float damage){

	Entity * bAhPuch = new Entity(id, s);
	//Mesh Render

	Ogre::Vector3 pos = iniPos;
	bAhPuch->addComponent(new CMeshRender(pos, "balas/Ray.mesh", bAhPuch, s->getSceneManager(), { 1.0f, 1.0f, 1.0f }, { 0, 0, angle }));


	//RigidBody
	float weight = 1.0f;
	float heigth = 0.5f;
	bAhPuch->addComponent(new CRigidBody(bAhPuch, s->getGame()->getPhysicsWorld(), pos, heigth, weight, RB_DYNAMIC, SH_POLYGON, MASK_BULLET));

	//Bullet
	bAhPuch->addComponent(new CBullet(bAhPuch, EB_ZEUS, damage, 1000.0f));


	return bAhPuch;


}

Entity* createBulletHachiman(std::string id, GameScene* s, Ogre::Vector3 iniPos, float angle, float damage){

	Entity * bAhPuch = new Entity(id, s);
	//Mesh Render

	Ogre::Vector3 pos = iniPos;
	bAhPuch->addComponent(new CMeshRender(pos, "balas/Arrow.mesh", bAhPuch, s->getSceneManager(), { 1.0f, 0.5f, 1.0f }, { 0, 0, 90 }));


	//RigidBody
	float weight = 1.0f;
	float heigth = 0.5f;
	bAhPuch->addComponent(new CRigidBody(bAhPuch, s->getGame()->getPhysicsWorld(), pos, heigth, weight, RB_DYNAMIC, SH_POLYGON, MASK_BULLET));

	//Bullet
	bAhPuch->addComponent(new CBullet(bAhPuch, EB_HACHIMAN, damage, 700.0f));


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

	Entity * top = new Entity("ColliderTecho_01", s);
	top->addComponent(new CRigidBody(top, s->getGame()->getPhysicsWorld(), Ogre::Vector3(-100, 50, 0), 3, cM->getSize().x, RB_STATIC, SH_POLYGON, MASK_STATIC_TERRAIN));
	entities.push_back(top);

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
#pragma region Trees
	Entity * tree1 = new Entity("Tree_01", s);
	tree1->addComponent(new CMeshRender({ -50, -10, -30 }, "PineTree.mesh", tree1, s->getSceneManager(), { 3.0f, 5.0f, 3.0f }, { 0, 0, 0 }));
	entities.push_back(tree1);

	Entity * tree2 = new Entity("Tree_02", s);
	tree2->addComponent(new CMeshRender({ -60, -10, -20 }, "PineTree.mesh", tree2, s->getSceneManager(), { 2.0f, 2.0f, 2.0f }, { 0, 30, 0 }));
	entities.push_back(tree2);
	
	Entity * tree3 = new Entity("Tree_03", s);
	tree3->addComponent(new CMeshRender({ -10, -10, -40 }, "PineTree.mesh", tree3, s->getSceneManager(), { 3.0f, 4.0f, 3.0f }, { 0, 60, 0 }));
	entities.push_back(tree3);

	Entity * tree4 = new Entity("Tree_04", s);
	tree4->addComponent(new CMeshRender({ 5, -10, -20 }, "PineTree.mesh", tree4, s->getSceneManager(), { 3.0f, 4.0f, 3.0f }, { 0, 180, 0 }));
	entities.push_back(tree4);

	Entity * tree5 = new Entity("Tree_05", s);
	tree5->addComponent(new CMeshRender({ -70, -10, -30 }, "PineTree.mesh", tree5, s->getSceneManager(), { 3.0f, 3.0f, 3.0f }, { 0, 90, 0 }));
	entities.push_back(tree5);

	Entity * tree6 = new Entity("Tree_06", s);
	tree5->addComponent(new CMeshRender({ -77, -10, -10 }, "PineTree.mesh", tree6, s->getSceneManager(), { 2.0f, 2.0f, 2.0f }, { 0, 300, 0 }));
	entities.push_back(tree6);

#pragma endregion

	Entity * rock1 = new Entity("Rock_01", s);
	rock1->addComponent(new CMeshRender({ 0, -10, -25 }, "Rock.mesh", rock1, s->getSceneManager(), { 5.0f, 5.0f, 5.0f }, { 0, 300, 0 }));
	entities.push_back(rock1);

	Entity * rock2 = new Entity("Rock_02", s);
	rock2->addComponent(new CMeshRender({ 50, -10, -15 }, "Rock.mesh", rock2, s->getSceneManager(), { 10.0f, 10.0f, 10.0f }, { 0, 300, 0 }));
	entities.push_back(rock2);
	

	Entity * gate1 = new Entity("Gate_01", s);
	gate1->addComponent(new CMeshRender({ -100, -10, -10 }, "Gate.mesh", gate1, s->getSceneManager(), { 5.0f, 5.0f, 5.0f }, { 90, 180, 0 }));
	entities.push_back(gate1);


	return entities;

}

#pragma endregion