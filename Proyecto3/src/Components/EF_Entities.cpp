#include "EF_Entities.h"
#include "Entity.h"
#include "Components.h"
#include "Scenes.h"
#include "Game.h"
#include "DebugNew.h"
#include "EntityFactory.h"
#include "RenderComponents.h"
#include "Animation.h"
#include "Camera.h"
#include "PlayerComponents.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const float JUMPFORCE = 75.0f;
const float VELOCITY = 8.0f;
const float HEIGHT = 6.0f;
const float WIDTH = 2.5f;
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

	Ra->addComponent(new CRigidBody(Ra, Game::getInstance()->getPhysicsWorld(), iniPos, HEIGHT, WIDTH, 0.0f, RB_DYNAMIC, SH_PLAYER, MASK_PLAYER,controllerId));

	//CameraFollow
	Ra->addComponent(new CCameraFollow(Ra));


	//PlayerController
	Ra->addComponent(new CPlayerController(Ra, controllerId));
	//CollisionHandler
	Ra->addComponent(new CPlayerCollisionHandler(Ra));
	//Basic Attack
	Ra->addComponent(new CPlayerBasicAttack(Ra, 800.0f, EB_RA,iniPos,10.0f));

	//LIfe
	Ra->addComponent(new CLife(Ra, 100.0f));
	//Jump

	Ra->addComponent(new CPlayerJump(Ra, JUMPFORCE));
	//Move

	Ra->addComponent(new CPlayerMove(Ra, VELOCITY));

	//Ra->addComponent(new CPSkillHermes(Ra, 10.0f, 10.0f));
	//Ra->addComponent(new CPSkillUll(Ra));
	//Ra->addComponent(new CPSkillVidar(Ra));
	//Ra->addComponent(new CShuHeaddress(Ra, controllerId));
	//Ra->addComponent(EntityFactory::getInstance().createAbility(CMP_HERA_RUNE,Ra, controllerId));
	Ra->addComponent(new CParticleRender({ 0, 0, 0 }, id + "sandRa", "sandRa", Ra, s->getSceneManager(), { Ogre::Real(1.0f), Ogre::Real(1.0f), Ogre::Real(1.0f) }, { 0, 0, 0 }));

	//Animation
	Ra->addComponent(new CAnimation(Ra, s->getSceneManager(), cM->getChildNode()));

	return Ra;
}

Entity* createGodAhPuch(std::string id, GameScene* s, Ogre::Vector3 iniPos, int controllerId){

	Entity* AhPuch = new Entity("Player_" + std::to_string(controllerId), s);

	//Mesh Render
	CMeshRender* cM = new CMeshRender({ 2, 2, 0 }, "AhPuch.mesh", AhPuch, s->getSceneManager(), { 1.0f, 1.0f, 1.0f }, { 0, 0, 0 });
	AhPuch->addComponent(cM);

	//RigidBody
	Ogre::Vector3 size = cM->getSize();

	AhPuch->addComponent(new CRigidBody(AhPuch, Game::getInstance()->getPhysicsWorld(), iniPos, HEIGHT, WIDTH, 0.0f, RB_DYNAMIC, SH_PLAYER, MASK_PLAYER, controllerId));

	//CameraFollow
	AhPuch->addComponent(new CCameraFollow(AhPuch));


	//PlayerController
	AhPuch->addComponent(new CPlayerController(AhPuch, controllerId));
	//CollisionHandler
	AhPuch->addComponent(new CPlayerCollisionHandler(AhPuch));
	//Basic Attack
	AhPuch->addComponent(new CPlayerBasicAttack(AhPuch, 300.0f, EB_AHPUCH, iniPos, 2.0f));

	//LIfe
	AhPuch->addComponent(new CLife(AhPuch, 100.0f));
	//Jump
	AhPuch->addComponent(new CPlayerJump(AhPuch, JUMPFORCE));
	//Move
	AhPuch->addComponent(new CPlayerMove(AhPuch, VELOCITY));

	AhPuch->addComponent(new CAnimation(AhPuch, s->getSceneManager(), cM->getChildNode()));

	AhPuch->addComponent(new CParticleRender({ 0, 0, 0 }, id + "poisonAhPuch", "poisonAhPuch", AhPuch, s->getSceneManager(), { Ogre::Real(1.0f), Ogre::Real(1.0f), Ogre::Real(1.0f) }, { 0, 0, 0 }));
	AhPuch->addComponent(new CParticleRender({ 0, 0, 0 }, id + "pB1AhPuch", "pB1AhPuch", AhPuch, s->getSceneManager(), { Ogre::Real(1.0f), Ogre::Real(1.0f), Ogre::Real(1.0f) }, { 0, 0, 0 }));
	AhPuch->addComponent(new CParticleRender({ 0, 0, 0 }, id + "pB2AhPuch", "pB2AhPuch", AhPuch, s->getSceneManager(), { Ogre::Real(1.0f), Ogre::Real(1.0f), Ogre::Real(1.0f) }, { 0, 0, 0 }));


	return AhPuch;
}


Entity* createGodZeus(std::string id, GameScene* s, Ogre::Vector3 iniPos, int controllerId){

	Entity* Zeus = new Entity("Player_" + std::to_string(controllerId), s);

	//Mesh Render
	CMeshRender* cM = new CMeshRender({ 2, 2, 0 }, "Zeus.mesh", Zeus, s->getSceneManager(), { 1.0f, 1.0f, 1.0f }, { 0, 0, 0});
	Zeus->addComponent(cM);

	//RigidBody
	Ogre::Vector3 size = cM->getSize();

	Zeus->addComponent(new CRigidBody(Zeus, Game::getInstance()->getPhysicsWorld(), iniPos, HEIGHT, WIDTH,0.0f, RB_DYNAMIC, SH_PLAYER, MASK_PLAYER,controllerId));

	//CameraFollow
	Zeus->addComponent(new CCameraFollow(Zeus));


	//PlayerController
	Zeus->addComponent(new CPlayerController(Zeus, controllerId));
	//CollisionHandler
	Zeus->addComponent(new CPlayerCollisionHandler(Zeus));
	//Basic Attack
	Zeus->addComponent(new CPlayerBasicAttack(Zeus, 800.0f, EB_ZEUS, iniPos, 7.0f));

	//LIfe
	Zeus->addComponent(new CLife(Zeus, 100.0f));
	//Jump
	Zeus->addComponent(new CPlayerJump(Zeus, JUMPFORCE));
	//Move
	Zeus->addComponent(new CPlayerMove(Zeus, VELOCITY));
	//Zeus->addComponent(new CHerisMark(Zeus, controllerId));

	Zeus->addComponent(new CAnimation(Zeus, s->getSceneManager(), cM->getChildNode()));


	Zeus->addComponent(new CParticleRender({ 0, 0, 0 }, id + "LZeus1", "LZeus1", Zeus, s->getSceneManager(), { Ogre::Real(1.0f), Ogre::Real(1.0f), Ogre::Real(1.0f) }, { 0, 0, 0 }));
	Zeus->addComponent(new CParticleRender({ 0, 0, 0 }, id + "LZeus2", "LZeus2", Zeus, s->getSceneManager(), { Ogre::Real(1.0f), Ogre::Real(1.0f), Ogre::Real(1.0f) }, { 0, 0, 0 }));
	Zeus->addComponent(new CParticleRender({ 0, 0, 0 }, id + "LZeus3", "LZeus3", Zeus, s->getSceneManager(), { Ogre::Real(1.0f), Ogre::Real(1.0f), Ogre::Real(1.0f) }, { 0, 0, 0 }));
	Zeus->addComponent(new CParticleRender({ 0, 0, 0 }, id + "LZeus4", "LZeus4", Zeus, s->getSceneManager(), { Ogre::Real(1.0f), Ogre::Real(1.0f), Ogre::Real(1.0f) }, { 0, 0, 0 }));
	Zeus->addComponent(new CParticleRender({ 0, 0, 0 }, id + "LZeus5", "LZeus5", Zeus, s->getSceneManager(), { Ogre::Real(1.0f), Ogre::Real(1.0f), Ogre::Real(1.0f) }, { 0, 0, 0 }));
	Zeus->addComponent(new CParticleRender({ 0, 0, 0 }, id + "LZeus6", "LZeus6", Zeus, s->getSceneManager(), { Ogre::Real(1.0f), Ogre::Real(1.0f), Ogre::Real(1.0f) }, { 0, 0, 0 }));

	//Zeus->addComponent(new CPSkillVidar(Zeus));


	return Zeus;
}



Entity* createGodHachiman(std::string id, GameScene* s, Ogre::Vector3 iniPos, int controllerId){

	Entity* Hachiman = new Entity("Player_" + std::to_string(controllerId), s);

	//Mesh Render
	CMeshRender* cM = new CMeshRender({ 2, 2, 0 }, "Hachiman.mesh", Hachiman, s->getSceneManager(), { 1.0f, 1.0f, 1.0f }, { 0, 0, 0 });
	Hachiman->addComponent(cM);

	//RigidBody
	Ogre::Vector3 size = cM->getSize();

	Hachiman->addComponent(new CRigidBody(Hachiman, Game::getInstance()->getPhysicsWorld(), iniPos, HEIGHT, WIDTH, 0.0f, RB_DYNAMIC, SH_PLAYER, MASK_PLAYER, controllerId));

	//CameraFollow
	Hachiman->addComponent(new CCameraFollow(Hachiman));

	//PlayerController
	Hachiman->addComponent(new CPlayerController(Hachiman, controllerId));
	//CollisionHandler
	Hachiman->addComponent(new CPlayerCollisionHandler(Hachiman));
	//Basic Attack
	Hachiman->addComponent(new CPlayerBasicAttack(Hachiman, 550.0f, EB_HACHIMAN, iniPos, 4.0f));

	//LIfe
	Hachiman->addComponent(new CLife(Hachiman, 100.0f));
	//Jump
	Hachiman->addComponent(new CPlayerJump(Hachiman, JUMPFORCE));
	//Move
	Hachiman->addComponent(new CPlayerMove(Hachiman, VELOCITY));

	Hachiman->addComponent(new CAnimation(Hachiman, s->getSceneManager(), cM->getChildNode()));


	Hachiman->addComponent(new CParticleRender({ 0, 0, 0 }, id + "windHachimanI", "windHachimanI", Hachiman, s->getSceneManager(), { Ogre::Real(1.0f), Ogre::Real(1.0f), Ogre::Real(1.0f) }, { 0, 0, 0 }));
	Hachiman->addComponent(new CParticleRender({ 0, 0, 0 }, id + "windHachimanC", "windHachimanC", Hachiman, s->getSceneManager(), { Ogre::Real(1.0f), Ogre::Real(1.0f), Ogre::Real(1.0f) }, { 0, 0, 0 }));
	Hachiman->addComponent(new CParticleRender({ 0, 0, 0 }, id + "windHachimanD", "windHachimanD", Hachiman, s->getSceneManager(), { Ogre::Real(1.0f), Ogre::Real(1.0f), Ogre::Real(1.0f) }, { 0, 0, 0 }));




	return Hachiman;
}


#pragma endregion

#pragma region Bullets

Entity* createBulletRa(std::string id, GameScene* s, Ogre::Vector3 iniPos, float angle, float damage, std::string iD){

	Entity * bRa = new Entity(id, s);
	//Mesh Render

	Ogre::Vector3 pos = iniPos;
	CMeshRender* cM = new CMeshRender(pos, "balas/energyBall.mesh", bRa, s->getSceneManager(), { 1.0f, 1.0f, 1.0f }, { 0, 0, 0 });
	bRa->addComponent(cM);


	//RigidBody
	float weight = cM->getSize().x;
	float heigth = cM->getSize().y;

	if (iD == "Player_0")
		bRa->addComponent(new CRigidBody(bRa, s->getGame()->getPhysicsWorld(), pos, heigth, weight, angle, RB_DYNAMIC, SH_POLYGON, MASK_BULLET_0,-1));
	else
		bRa->addComponent(new CRigidBody(bRa, s->getGame()->getPhysicsWorld(), pos, heigth, weight, angle, RB_DYNAMIC, SH_POLYGON, MASK_BULLET_1, -1));



	//Bullet
	bRa->addComponent(new CBullet(bRa, EB_RA, damage, 400.0f));
	bRa->addComponent(new CParticleRender({ 0, 1000, 0 }, id + "ballRa", "ballRa", bRa, s->getSceneManager(), { Ogre::Real(0.1f), Ogre::Real(0.1f), Ogre::Real(0.1f) }, { 0, 0, 0 }));


	return bRa;


}

Entity* createBulletAhPuch(std::string id, GameScene* s, Ogre::Vector3 iniPos, float angle, float damage, std::string iD){

	Entity * bAhPuch = new Entity(id, s);
	//Mesh Render

	Ogre::Vector3 pos = iniPos;
	CMeshRender* cM = new CMeshRender(pos, "balas/Cerbatana.mesh", bAhPuch, s->getSceneManager(), { 0.25f, 1.0f, 0.25f }, { 0, 0, 0 });
	bAhPuch->addComponent(cM);


	//RigidBody
	float weight = cM->getSize().x;
	float heigth = cM->getSize().y;

	if (iD == "Player_0")
		bAhPuch->addComponent(new CRigidBody(bAhPuch, s->getGame()->getPhysicsWorld(), pos, heigth, weight, angle, RB_DYNAMIC, SH_POLYGON, MASK_BULLET_0, -1));
	else
		bAhPuch->addComponent(new CRigidBody(bAhPuch, s->getGame()->getPhysicsWorld(), pos, heigth, weight, angle, RB_DYNAMIC, SH_POLYGON, MASK_BULLET_1, -1));


	//Bullet
	bAhPuch->addComponent(new CBullet(bAhPuch, EB_AHPUCH, damage, 700.0f));
	//bAhPuch->addComponent(new CParticleRender({ 0, 1000, 0 }, id + "pCer1AhPuch", "pCer1AhPuch", bAhPuch, s->getSceneManager(), { Ogre::Real(1.0f), Ogre::Real(1.0f), Ogre::Real(1.0f) }, { 0, 0, angle }));

	return bAhPuch;


}


Entity* createBulletZeus(std::string id, GameScene* s, Ogre::Vector3 iniPos, float angle, float damage, std::string iD){

	Entity * bZeus = new Entity(id, s);
	//Mesh Render

	Ogre::Vector3 pos = iniPos;
	CMeshRender* cM = new CMeshRender(pos, "balas/Ray.mesh", bZeus, s->getSceneManager(), { 0.35f, 0.5f, 0.5f }, { 0, 0,0 });
	bZeus->addComponent(cM);


	//RigidBody
	float weight = cM->getSize().x;
	float heigth = cM->getSize().y;

	if (iD == "Player_0")
		bZeus->addComponent(new CRigidBody(bZeus, s->getGame()->getPhysicsWorld(), pos, heigth, weight, angle, RB_DYNAMIC, SH_POLYGON, MASK_BULLET_0, -1));

	else
		bZeus->addComponent(new CRigidBody(bZeus, s->getGame()->getPhysicsWorld(), pos, heigth, weight, angle, RB_DYNAMIC, SH_POLYGON, MASK_BULLET_1, -1));



	//Bullet
	bZeus->addComponent(new CBullet(bZeus, EB_ZEUS, damage, 1000.0f));
	/*bZeus->addComponent(new CParticleRender({ 0, 0, -1000 }, id + "LightningZeus1", "LightningZeus1", bZeus, s->getSceneManager(), { Ogre::Real(1.0f), Ogre::Real(1.0f), Ogre::Real(1.0f) }, { 0, 0, angle }));
	bZeus->addComponent(new CParticleRender({ 0, 0, -1000 }, id + "LightningZeus2", "LightningZeus2", bZeus, s->getSceneManager(), { Ogre::Real(1.0f), Ogre::Real(1.0f), Ogre::Real(1.0f) }, { 0, 0, angle }));
	bZeus->addComponent(new CParticleRender({ 0, 0, -1000 }, id + "LightningZeus3", "LightningZeus3", bZeus, s->getSceneManager(), { Ogre::Real(1.0f), Ogre::Real(1.0f), Ogre::Real(1.0f) }, { 0, 0, angle }));*/


	// Estela
	//bZeus->addComponent(new CRibbonTrailRender({ 0, 0, pos.z }, id + "trail", "Smoke", bZeus, s->getSceneManager(), { Ogre::Real(0.01f), Ogre::Real(0.01f), Ogre::Real(0.01f) }, { 0, 0, angle }));

	return bZeus;


}

Entity* createBulletHachiman(std::string id, GameScene* s, Ogre::Vector3 iniPos, float angle, float damage, std::string iD){

	Entity * bHachiman = new Entity(id, s);
	//Mesh Render

	Ogre::Vector3 pos = iniPos;
	CMeshRender* cM = new CMeshRender(pos, "balas/Arrow.mesh", bHachiman, s->getSceneManager(), { 0.7f, 0.4f, 1.0f }, { 0, 0, 0 });
	bHachiman->addComponent(cM);


	//RigidBody
	float weight = cM->getSize().x;
	float heigth = cM->getSize().y;

	if (iD == "Player_0")
		bHachiman->addComponent(new CRigidBody(bHachiman, s->getGame()->getPhysicsWorld(), pos, heigth, weight, angle, RB_DYNAMIC, SH_POLYGON, MASK_BULLET_0, -1));
	else
		bHachiman->addComponent(new CRigidBody(bHachiman, s->getGame()->getPhysicsWorld(), pos, heigth, weight, angle, RB_DYNAMIC, SH_POLYGON, MASK_BULLET_1, -1));

	//Bullet
	bHachiman->addComponent(new CBullet(bHachiman, EB_HACHIMAN, damage, 700.0f));
	bHachiman->addComponent(new CParticleRender({ 0, 0, -1000 }, id + "wind", "wind", bHachiman, s->getSceneManager(), { Ogre::Real(1.0f), Ogre::Real(1.0f), Ogre::Real(1.0f) }, { 0, 0, angle }));



	return bHachiman;


}

#pragma endregion

#pragma region Stages

std::vector<Entity*>* createStageTemple(GameScene* s, std::vector<Entity*> &deleteable){

	std::vector<Entity*>* entities = new std::vector<Entity*>();

	s->getSceneManager()->setAmbientLight(Ogre::ColourValue(.5, .5, .5));

	Ogre::Light* light = s->getSceneManager()->createLight("MainLight");
	light->setPosition(20, 80, 50);

	


	
	Entity * e1 = new Entity("Suelo_01", s);
	CMeshRender* cM = new CMeshRender({ 45, -20, -30 }, "suelo.mesh", e1, s->getSceneManager(), { 100.0f, 100.0f, 100.0f }, { 0, -90.0f, 0 });
	e1->addComponent(cM);
	deleteable.push_back(e1);

	Entity * Suelo = new Entity("ColliderSuelo_01", s);
	Suelo->addComponent(new CRigidBody(Suelo, s->getGame()->getPhysicsWorld(), Ogre::Vector3(-100, -15, 0), 3, cM->getSize().x, 0, RB_STATIC, SH_POLYGON, MASK_STATIC_TERRAIN, -1));
	entities->push_back(Suelo);

	Entity * top = new Entity("ColliderTecho_01", s);
	top->addComponent(new CRigidBody(top, s->getGame()->getPhysicsWorld(), Ogre::Vector3(-100, 50, 0), 3, cM->getSize().x, 0, RB_STATIC, SH_POLYGON, MASK_STATIC_TERRAIN, -1));
	entities->push_back(top);

	Entity * porton = new Entity("Porton_01", s);
	porton->addComponent(new CMeshRender({ 0, -10, -20 }, "porton.mesh", porton, s->getSceneManager(), { 1.0f, 1.0f, 1.0f }, { 0, 50, 180 }));
	deleteable.push_back(porton);

	Entity * rightEdge = new Entity("ColliderLimite_Dcho", s);
	rightEdge->addComponent(new CRigidBody(rightEdge, s->getGame()->getPhysicsWorld(), Ogre::Vector3(55, -15, 0), 200, 2, 0, RB_STATIC, SH_POLYGON, MASK_STATIC_TERRAIN, -1));
	entities->push_back(rightEdge);

	Entity * leftEdge = new Entity("ColliderLimite_Izqdo", s);
	leftEdge->addComponent(new CRigidBody(leftEdge, s->getGame()->getPhysicsWorld(), Ogre::Vector3(-100, -15, 0), 200, 2, 0, RB_STATIC, SH_POLYGON, MASK_STATIC_TERRAIN, -1));
	entities->push_back(leftEdge);

	Entity * temple = new Entity("Templo_01", s);
	temple->addComponent(new CMeshRender({ -30, -2, -30 }, "templo.mesh", temple, s->getSceneManager(), { 10.0f, 10.0f, 10.0f }, { 0, 300, 0 }));
	deleteable.push_back(temple);

#pragma region Trees
	Entity * tree1 = new Entity("Tree_01", s);
	tree1->addComponent(new CMeshRender({ -50, -12, -30 }, "Arbol1.mesh", tree1, s->getSceneManager(), { 3.0f, 3.0f, 3.0f }, { 0, 0, 0 }));
	deleteable.push_back(tree1);

	Entity * tree2 = new Entity("Tree_02", s);
	tree2->addComponent(new CMeshRender({ -60, -12, -20 }, "ArbolRosa2.mesh", tree2, s->getSceneManager(), { 4.0f, 4.0f, 4.0f }, { 90, 0, 0 }));
	deleteable.push_back(tree2);

	Entity * tree3 = new Entity("Tree_03", s);
	tree3->addComponent(new CMeshRender({ -10, -12, -40 }, "Arbol2.mesh", tree3, s->getSceneManager(), { 3.0f, 4.0f, 3.0f }, { 0, 60, 0 }));
	deleteable.push_back(tree3);

	Entity * tree4 = new Entity("Tree_04", s);
	tree4->addComponent(new CMeshRender({ 5, -12, -20 }, "Arbol1.mesh", tree4, s->getSceneManager(), { 3.0f, 4.0f, 3.0f }, { 0, 180, 0 }));
	deleteable.push_back(tree4);

	Entity * tree5 = new Entity("Tree_05", s);
	tree5->addComponent(new CMeshRender({ -70, -12, -30 }, "Arbol2.mesh", tree5, s->getSceneManager(), { 3.0f, 3.0f, 3.0f }, { 0, 0, 0 }));
	deleteable.push_back(tree5);

	Entity * tree6 = new Entity("Tree_06", s);
	tree5->addComponent(new CMeshRender({ -77, -12, -10 }, "Arbol2.mesh", tree6, s->getSceneManager(), { 4.0f, 4.0f, 4.0f }, { 0, 300, 0 }));
	deleteable.push_back(tree6);

	Entity * tree7 = new Entity("Tree_07", s);
	tree7->addComponent(new CMeshRender({ 35, -15, -30 }, "Arbol2.mesh", tree7, s->getSceneManager(), { 3.0f, 3.0f, 3.0f }, { 0, 90, 0 }));
	deleteable.push_back(tree7);

	Entity * tree8 = new Entity("Tree_08", s);
	tree8->addComponent(new CMeshRender({ 20, -14, -20 }, "ArbolRosa2.mesh", tree8, s->getSceneManager(), { 5.0f, 5.0f, 5.0f }, { 90, 0, 0 }));
	deleteable.push_back(tree8);

	Entity * tree9 = new Entity("Tree_09", s);
	tree9->addComponent(new CMeshRender({ -55, -12, -10 }, "Arbol2.mesh", tree9, s->getSceneManager(), { 5.0f, 5.0f, 5.0f }, { 0, 300, 0 }));
	deleteable.push_back(tree9);

	Entity * tree10 = new Entity("Tree_10", s);
	tree10->addComponent(new CMeshRender({ -68, -5, -10 }, "ArbolRosa1.mesh", tree10, s->getSceneManager(), { 3.0f, 3.0f, 3.0f }, { 90, 0, 0 }));
	deleteable.push_back(tree10);

	Entity * tree11 = new Entity("Tree_11", s);
	tree11->addComponent(new CMeshRender({ -15, -12, -10 }, "Arbol1.mesh", tree11, s->getSceneManager(), { 5.0f, 5.0f, 5.0f }, { 0, 300, 0 }));
	deleteable.push_back(tree11);

	Entity * tree12 = new Entity("Tree_12", s);
	tree12->addComponent(new CMeshRender({ 28, -12, -15 }, "Arbol1.mesh", tree12, s->getSceneManager(), { 3.0f, 3.0f, 3.0f }, { 0, 180, 0 }));
	deleteable.push_back(tree12);

	Entity * tree13 = new Entity("Tree_13", s);
	tree13->addComponent(new CMeshRender({ -40, -7, -20 }, "ArbolRosa1.mesh", tree13, s->getSceneManager(), { 3.0f, 3.0f, 3.0f }, { 90, 0, 0 }));
	deleteable.push_back(tree13);

	Entity * tree14 = new Entity("Tree_14", s);
	tree14->addComponent(new CMeshRender({ 40, -1, -10 }, "ArbolRosa1.mesh", tree14, s->getSceneManager(), { 6.0f, 6.0f, 6.0f }, { 90, 0, 0 }));
	deleteable.push_back(tree14);




#pragma endregion

	Entity * rock1 = new Entity("Rock_01", s);
	rock1->addComponent(new CMeshRender({ 0, -10, -25 }, "Rock.mesh", rock1, s->getSceneManager(), { 5.0f, 5.0f, 5.0f }, { 0, 300, 0 }));
	deleteable.push_back(rock1);

	Entity * rock2 = new Entity("Rock_02", s);
	rock2->addComponent(new CMeshRender({ 50, -10, -15 }, "Rock.mesh", rock2, s->getSceneManager(), { 10.0f, 10.0f, 10.0f }, { 0, 300, 0 }));
	deleteable.push_back(rock2);

	Ogre::Viewport* vp = nullptr;
	Entity * cam = new Entity("Camera1", s);
	cam->addComponent(new CActionCamera(cam, s->getSceneManager(), vp, 100, 100, 40, 100));
	entities->push_back(cam);

	return entities;

}


std::vector<Entity*>* createStageIslands(GameScene* s, std::vector<Entity*> &deleteable){

	std::vector<Entity*>* entities = new std::vector<Entity*>();

	s->getSceneManager()->setAmbientLight(Ogre::ColourValue(.5, .5, .5));

	Ogre::Light* light = s->getSceneManager()->createLight("MainLight");
	light->setPosition(20, 30, 50);
	light->setCastShadows(true);



	Entity * e1 = new Entity("Suelo_01", s);
	CMeshRender* cM = new CMeshRender({ 0, -3, -30 }, "IslaPrincipal.mesh", e1, s->getSceneManager(), { 50.0f, 30.0f, 50.0f }, { 0, 0, 0 });
	e1->addComponent(cM);
	deleteable.push_back(e1);


	Entity * Suelo = new Entity("ColliderSuelo_01", s);
	Suelo->addComponent(new CRigidBody(Suelo, s->getGame()->getPhysicsWorld(), Ogre::Vector3(7 - cM->getSize().x / 2, -5, 0), 3, cM->getSize().x - 15, 0, RB_STATIC, SH_POLYGON, MASK_STATIC_TERRAIN, -1));
	entities->push_back(Suelo);


	Entity * isla2 = new Entity("Suelo_02", s);
	CMeshRender* cM2 = new CMeshRender({ -70, 10, 0 }, "IslaSecundaria1.mesh", isla2, s->getSceneManager(), { 10.0f, 10.0f, 10.0f }, { 0, 0, 0 });
	isla2->addComponent(cM2);
	deleteable.push_back(isla2);

	Entity * isla2Collider = new Entity("SueloCollider_02", s);
	isla2Collider->addComponent(new CRigidBody(isla2Collider, s->getGame()->getPhysicsWorld(), Ogre::Vector3(-70 - cM2->getSize().x / 2, 10 - cM2->getSize().y, 0), cM2->getSize().y, cM2->getSize().x, 0, RB_STATIC, SH_POLYGON, MASK_STATIC_TERRAIN, -1));
	entities->push_back(isla2Collider);

	Entity * isla3 = new Entity("Suelo_03", s);
	CMeshRender* cM3 = new CMeshRender({ -100, 0, 0 }, "IslaSecundaria1.mesh", isla3, s->getSceneManager(), { 10.0f, 10.0f, 10.0f }, { 0, 0, 0 });
	isla3->addComponent(cM3);
	deleteable.push_back(isla3);

	Entity * isla3Collider = new Entity("SueloCollider_03", s);
	isla3Collider->addComponent(new CRigidBody(isla3Collider, s->getGame()->getPhysicsWorld(), Ogre::Vector3(-100 - cM3->getSize().x / 2, 0 - cM3->getSize().y, 0), cM3->getSize().y, cM3->getSize().x, 0, RB_STATIC, SH_POLYGON, MASK_STATIC_TERRAIN, -1));
	entities->push_back(isla3Collider);


	Entity * isla4 = new Entity("Suelo_04", s);
	CMeshRender* cM4 = new CMeshRender({ 50, 40, 0 }, "IslaSecundaria2.mesh", isla4, s->getSceneManager(), { 10.0f, 10.0f, 10.0f }, { 0, 0, 0 });
	isla4->addComponent(cM4);
	deleteable.push_back(isla4);

	Entity * isla4Collider = new Entity("SueloCollider_04", s);
	isla4Collider->addComponent(new CRigidBody(isla4Collider, s->getGame()->getPhysicsWorld(), Ogre::Vector3(50 - cM4->getSize().x / 2, 40 - cM4->getSize().y, 0), cM4->getSize().y, cM4->getSize().x, 0, RB_STATIC, SH_POLYGON, MASK_STATIC_TERRAIN, -1));
	entities->push_back(isla4Collider);


	Entity * mountain1 = new Entity("mountain_01", s);
	mountain1->addComponent(new CMeshRender({ -15, -20, -42 }, "Montana.mesh", mountain1, s->getSceneManager(), { 40.0f, 55.0f, 45.0f }, { 0, 30, 0 }));
	deleteable.push_back(mountain1);
	// Nubes

	Entity * nube1 = new Entity("nube_01", s);
	nube1->addComponent(new CMeshRender({ 0, 40, -40 }, "Nube.mesh", nube1, s->getSceneManager(), { 1.0f, 1.0f, 1.0f }, { 0, 0, 0 }));
	deleteable.push_back(nube1);

	Entity * nube2 = new Entity("nube_02", s);
	nube2->addComponent(new CMeshRender({ 25, 40, -40 }, "Nube.mesh", nube2, s->getSceneManager(), { 1.0f, 1.0f, 1.0f }, { 0, 30, 0 }));
	deleteable.push_back(nube2);


	Entity * nube5 = new Entity("nube_05", s);
	nube5->addComponent(new CMeshRender({ 25, 40, 0 }, "Nube.mesh", nube5, s->getSceneManager(), { 1.0f, 1.0f, 1.0f }, { 0, 90, 0 }));
	deleteable.push_back(nube5);

	Entity * nube3 = new Entity("nube_03", s);
	nube3->addComponent(new CMeshRender({ -25, 30, -40 }, "Nube.mesh", nube3, s->getSceneManager(), { 1.0f, 1.0f, 1.0f }, { 0, 30, 0 }));
	deleteable.push_back(nube3);

	Entity * nube6 = new Entity("nube_06", s);
	nube6->addComponent(new CMeshRender({ -25, 30, 0 }, "Nube.mesh", nube6, s->getSceneManager(), { 1.0f, 1.0f, 1.0f }, { 0,45, 0 }));
	deleteable.push_back(nube6);

	Entity * nube4 = new Entity("nube_04", s);
	nube4->addComponent(new CMeshRender({ -100, 50, -40 }, "Nube.mesh", nube4, s->getSceneManager(), { 1.0f, 1.0f, 1.0f }, { 0, 0, 0 }));
	deleteable.push_back(nube4);

	Entity * nube7 = new Entity("nube_07", s);
	nube7->addComponent(new CMeshRender({ -80, 25, -40 }, "Nube.mesh", nube7, s->getSceneManager(), { 1.0f, 1.0f, 1.0f }, { 0, 0, 0 }));
	deleteable.push_back(nube7);

	Entity * nube11 = new Entity("nube_11", s);
	nube11->addComponent(new CMeshRender({ -70, 45, -40 }, "Nube.mesh", nube11, s->getSceneManager(), { 2.0f, 2.0f, 2.0f }, { 0, 0, 0 }));
	deleteable.push_back(nube11);


	Entity * nube12 = new Entity("nube_12", s);
	nube12->addComponent(new CMeshRender({ -90, 39, -20 }, "Nube.mesh", nube12, s->getSceneManager(), { 3.0f, 2.0f, 3.0f }, { 0, 0, 0 }));
	deleteable.push_back(nube12);


	Entity * nube8 = new Entity("nube_08", s);
	CMeshRender* cN = new CMeshRender({ -80, 35, -40 }, "Nube2.mesh", nube8, s->getSceneManager(), { 3.0f, 2.0f, 3.0f }, { 0, 0, 0 });
	nube8->addComponent(cN);
	nube8->addComponent(new CRigidBody(nube8, s->getGame()->getPhysicsWorld(), Ogre::Vector3(-50, 25, -40), cN->getSize().y - 0.5f, cN->getSize().x - 1, 0, RB_STATIC, SH_POLYGON, MASK_STATIC_TERRAIN, -1));
	entities->push_back(nube8);


	Entity * nube9 = new Entity("nube_09", s);
	CMeshRender* cN2 = new CMeshRender({ 0, 45, -40 }, "Nube2.mesh", nube9, s->getSceneManager(), { 3.0f, 2.0f, 3.0f }, { 0, 0, 0 });
	nube9->addComponent(cN2);
	nube9->addComponent(new CRigidBody(nube9, s->getGame()->getPhysicsWorld(), Ogre::Vector3(-20, 35, -40), cN2->getSize().y - 0.5f, cN2->getSize().x - 1, 0, RB_STATIC, SH_POLYGON, MASK_STATIC_TERRAIN, -1));
	entities->push_back(nube9);

	Entity * nube10 = new Entity("nube_10", s);
	CMeshRender* cN3 = new CMeshRender({ 0, 45, -40 }, "Nube2.mesh", nube10, s->getSceneManager(), { 3.0f, 2.0f, 3.0f }, { 0, 0, 0 });
	nube10->addComponent(cN3);
	nube10->addComponent(new CRigidBody(nube10, s->getGame()->getPhysicsWorld(), Ogre::Vector3(10, 45, -40), cN3->getSize().y - 0.5f, cN3->getSize().x - 1, 0, RB_STATIC, SH_POLYGON, MASK_STATIC_TERRAIN, -1));
	entities->push_back(nube10);

	Entity * nube13 = new Entity("nube_13", s);
	CMeshRender* cN4 = new CMeshRender({ 0, 45, -40 }, "Nube2.mesh", nube13, s->getSceneManager(), { 3.0f, 2.0f, 3.0f }, { 0, 0, 0 });
	nube13->addComponent(cN4);
	nube13->addComponent(new CRigidBody(nube13, s->getGame()->getPhysicsWorld(), Ogre::Vector3(60, -25, -40), cN4->getSize().y - 0.5f, cN4->getSize().x - 1, 0, RB_STATIC, SH_POLYGON, MASK_STATIC_TERRAIN, -1));
	entities->push_back(nube13);


	// ----------------------

	Entity * top = new Entity("ColliderTecho_01", s);
	top->addComponent(new CRigidBody(top, s->getGame()->getPhysicsWorld(), Ogre::Vector3(-300, 100, 0), 3, 2000, 0, RB_STATIC, SH_POLYGON, MASK_STATIC_TERRAIN, -1));
	entities->push_back(top);

	Entity * rightEdge = new Entity("ColliderLimite_Dcho", s);
	rightEdge->addComponent(new CRigidBody(rightEdge, s->getGame()->getPhysicsWorld(), Ogre::Vector3(100, -500, 0), 1000, 1, 0, RB_STATIC, SH_POLYGON, MASK_STATIC_TERRAIN, -1));
	entities->push_back(rightEdge);

	Entity * leftEdge = new Entity("ColliderLimite_Izqdo", s);
	leftEdge->addComponent(new CRigidBody(leftEdge, s->getGame()->getPhysicsWorld(), Ogre::Vector3(-150, -500, 0), 1000, 1, 0, RB_STATIC, SH_POLYGON, MASK_STATIC_TERRAIN, -1));
	entities->push_back(leftEdge);

	Entity *deathZone = new Entity("DeathZone_01", s);
	deathZone->addComponent(new CRigidBody(deathZone, s->getGame()->getPhysicsWorld(), Ogre::Vector3(-300, -100, 0), 3, 2000, 0, RB_STATIC, SH_POLYGON, MASK_DEATHZONE, -1));
	entities->push_back(deathZone);

	//trees
	Entity * tree1 = new Entity("Tree_01", s);
	tree1->addComponent(new CMeshRender({ -35, 5, -30 }, "ArbolVerde1.mesh", tree1, s->getSceneManager(), { 5.0f, 5.0f, 5.0f }, { 90, 0, 0 }));
	deleteable.push_back(tree1);

	Entity * tree2 = new Entity("Tree_02", s);
	tree2->addComponent(new CMeshRender({ -15, -3, -45 }, "Arbol1.mesh", tree2, s->getSceneManager(), { 4.0f, 4.0f, 4.0f }, { 0, 30, 0 }));
	deleteable.push_back(tree2);

	Entity * tree3 = new Entity("Tree_03", s);
	tree3->addComponent(new CMeshRender({ 42, -3, -30 }, "Arbol2.mesh", tree3, s->getSceneManager(), { 3.0f, 4.0f, 3.0f }, { 0, 60, 0 }));
	deleteable.push_back(tree3);

	Entity * tree4 = new Entity("Tree_04", s);
	tree4->addComponent(new CMeshRender({ 10, 4, -10 }, "ArbolVerde2.mesh", tree4, s->getSceneManager(), { 3.0f, 4.0f, 3.0f }, { 90, 0, 0 }));
	deleteable.push_back(tree4);

	Entity * tree5 = new Entity("Tree_05", s);
	tree5->addComponent(new CMeshRender({ -25, -3, -40 }, "Arbol2.mesh", tree5, s->getSceneManager(), { 5.0f, 5.0f, 5.0f }, { 0, 90, 0 }));
	deleteable.push_back(tree5);

	Entity * tree6 = new Entity("Tree_06", s);
	tree5->addComponent(new CMeshRender({ -98, 9, -7 }, "ArbolVerde2.mesh", tree6, s->getSceneManager(), { 4.0f, 4.0f, 4.0f }, { 90, 0, 0 }));
	deleteable.push_back(tree6);

	Entity * tree7 = new Entity("Tree_07", s);
	tree7->addComponent(new CMeshRender({ 50, 45, -7 }, "ArbolVerde2.mesh", tree7, s->getSceneManager(), { 3.0f, 3.0f, 3.0f }, { 90, 0, 0 }));
	deleteable.push_back(tree7);

	Ogre::Viewport* vp = nullptr;
	Entity * cam = new Entity("Camera1", s);
	cam->addComponent(new CActionCamera(cam, s->getSceneManager(), vp, 150, 50, 60, 150));
	entities->push_back(cam);

	return entities;

}
