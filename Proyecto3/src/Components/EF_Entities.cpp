#include "EF_Entities.h"
#include "Entity.h"
#include "Components.h"
#include "Scenes.h"
#include "Game.h"

//-------------------------------------------    GODS    -------------------------------------------//
#pragma region Gods
//Ra
Entity* createGodRa(std::string id, GameScene* s){
	Entity * test1 = new Entity(id, s);
	test1->addComponent(new CMessageSend(test1));
	
	return test1;;
}

#pragma endregion

#pragma region Bullets

Entity* createBulletRa(std::string id, GameScene* s){

	Entity * bRa = new Entity(id, s);
	//Mesh Render
	bRa->addComponent(new CMeshRender(Ogre::Vector3(0, 0, 0), "Bullet_Ra", bRa, s->getSceneManager()));

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