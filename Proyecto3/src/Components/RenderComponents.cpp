#include "RenderComponents.h"
#include "Entity.h"
#include "Scenes.h"

#pragma region RenderComponent
//Render Component class. Father to every
//other render component.

CRender::CRender(ComponentType t, Entity * father, Ogre::SceneManager * scnM)
	: GameComponent(t, father), pSceneMgr(scnM)
{
	pOgreSceneNode = pSceneMgr->getRootSceneNode()->createChildSceneNode();
	pChild = pOgreSceneNode->createChildSceneNode();
	lastDir = 3.0f;

}
CRender::~CRender(){
	pSceneMgr->destroySceneNode(pChild);
	pSceneMgr->destroySceneNode(pOgreSceneNode);
}

//Get Message general to every other render component child to this
void CRender::getMessage(Message *m) {
	switch (m->getType()) {
	case MSG_UPDATE_TRANSFORM:
		if (m->getEmmiter() == pEnt->getID()){
			//Message cast
			MUpdateTransform* msg = static_cast<MUpdateTransform *>(m);

			//We get the size of the colliderbox.
			float w = msg->getW();
			float h = msg->getH();

			//Where our node will rotate.
			Ogre::Vector3 parentPos = msg->GetPos();

			//Where our mesh is relative to the parent.
			//The real pos of the object is the parent pos + this variable, _ogrepos.
			float angleRad = msg->getRotation();

			if (angleRad != -1){
				_ogrepos.x = w / 2;
				_ogrepos.y = 0;
				_ogrepos.z = 0;

				//Move the parent to the collider location of rotation.
				pOgreSceneNode->setPosition(parentPos);
				//Move the child to the real pos of the collider.
				pChild->setPosition(_ogrepos);

				//Rotate the parent node the same degree as the collider.
				float grades = (angleRad * 180) / 3.14159265359;
				pOgreSceneNode->setOrientation(Ogre::Quaternion(Ogre::Degree(grades), Ogre::Vector3(0, 0, 1)));
			}
			else{

				_ogrepos.x = w / 2;
				_ogrepos.y = 0;
				_ogrepos.z = 0;

				/*float actualX = pOgreSceneNode->getPosition().x;
				float dir = parentPos.x - actualX;
				float angle = pChild->getOrientation().getYaw().valueDegrees();

				if (dir != 0){
				if (dir < 0){
				pChild->setOrientation(Ogre::Quaternion(Ogre::Degree(-angle), Ogre::Vector3(0, 1, 0)));
				pChild->setOrientation(Ogre::Quaternion(Ogre::Degree(-90), Ogre::Vector3(0, 1, 0)));
				}
				else{
				pChild->setOrientation(Ogre::Quaternion(Ogre::Degree(-angle), Ogre::Vector3(0, 1, 0)));
				pChild->setOrientation(Ogre::Quaternion(Ogre::Degree(90), Ogre::Vector3(0, 1, 0)));

				}
				}*/

				
				//Move the parent to the collider location of rotation.
				pOgreSceneNode->setPosition(parentPos);
				//Move the child to the real pos of the collider.
				pChild->setPosition(_ogrepos);


				}
		}
		break;
	default:
		break;
	}
}
				
#pragma endregion


#pragma region meshRenderComponent
//Mesh Render component.
//Takes a string with the name of the mesh to render
//and renders it.

CMeshRender::CMeshRender(Ogre::Vector3 pos, std::string meshName, Entity * father, Ogre::SceneManager * scnM, Ogre::Vector3 scale, Ogre::Vector3 rotation) :CRender(CMP_MESH_RENDER, father, scnM) {
	pOgreEnt = pSceneMgr->createEntity(father->getID(), meshName);
	pOgreSceneNode->setPosition(pos);

	pChild->attachObject(pOgreEnt);
	pChild->scale(scale);
	//pChild->showBoundingBox(true);
	//pOgreEnt->setCastShadows(true);

	pOgreSceneNode->rotate(Ogre::Quaternion(Ogre::Degree(rotation.x), Ogre::Vector3(1, 0, 0)));
	pOgreSceneNode->rotate(Ogre::Quaternion(Ogre::Degree(rotation.y), Ogre::Vector3(0, 1, 0)));
	pOgreSceneNode->rotate(Ogre::Quaternion(Ogre::Degree(rotation.z), Ogre::Vector3(0, 0, 1)));

	invisActive = false;


}
CMeshRender::~CMeshRender() {
	pChild->detachObject(pOgreEnt);
	pSceneMgr->destroyEntity(pOgreEnt);
}
void CMeshRender::tick(float delta) {


}
void CMeshRender::getMessage(Message * m) {
	CRender::getMessage(m);
	float angle;
	float dir;
	MPlayerShot* mPShot;
	//TO DO: IMPLEMENT MESSAGE RECEIVEING TO MOVE.
	switch (m->getType())
	{
	case MSG_MOD_INVS:
		invisActive = true;
		break;

	case MSG_PLAYER_JUMP:
		if (invisActive){
			pOgreEnt->setVisible(false);
		}
		break;

	case MSG_COLLISION_TERRAIN:
		if (invisActive){
			pOgreEnt->setVisible(true);
		}
		break;
	case MSG_PLAYER_SHOT:
		break;
	case MSG_PASSMOD_DES:
		invisActive = false;
		break;
	case MSG_ACT_RENDERGODMENU:
		pOgreEnt->setVisible(true);
		break;
	case MSG_DES_RENDERGODMENU:
		pOgreEnt->setVisible(false);
		break;
	default:
		break;
	}
}

Ogre::Vector3 CMeshRender::getSize(){

	Ogre::Vector3 v;

	//Ogre::AxisAlignedBox box = pSceneMgr->getSceneNode(pChild->getName())->_getWorldAABB();

	Ogre::AxisAlignedBox box = pOgreEnt->getWorldBoundingBox(true);

	v = box.getSize();

	return v;
}


#pragma endregion



#pragma region Skyplane Render Component
CSkyPlaneRender::CSkyPlaneRender(Entity * father, Ogre::SceneManager * scnM, float scale, std::string materialName, Ogre::Vector3 pos, Ogre::Viewport* vp) :CRender(CMP_SKYPLANE_RENDER, father, scnM){

	plane = Ogre::MeshManager::getSingleton().createPlane(materialName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::Plane(Ogre::Vector3::UNIT_Z, 0),
		96, 54, 100, 100, false, 1, 1.0, 1.0);
	entity = scnM->createEntity("plano" + materialName + pEnt->getScene()->getId(), materialName);

	entity->setMaterialName(materialName);
	pOgreSceneNode->attachObject(entity);

	pOgreSceneNode->setPosition(pos);
	//pOgreSceneNode->setScale({ scale, scale, scale });

}
CSkyPlaneRender::~CSkyPlaneRender(){

	Ogre::MeshManager::getSingleton().remove(plane);

	pOgreSceneNode->detachObject(entity);
	pSceneMgr->destroyEntity(entity);
}


void CSkyPlaneRender::tick(float delta){}
void CSkyPlaneRender::getMessage(Message * m){}


#pragma endregion

#pragma region particleRenderComponent

//PARTICLES
CParticleRender::CParticleRender(Ogre::Vector3 pos, std::string id, std::string particleSystem, Entity * father, Ogre::SceneManager * scnM, Ogre::Vector3 scale, Ogre::Vector3 rotation) :CRender(CMP_PARTICLE_RENDER, father, scnM) {


	_particleSystem = scnM->createParticleSystem(id, particleSystem);
	pChild->attachObject(_particleSystem);

	pOgreSceneNode->setPosition(pos);
	pChild->scale(scale);

	pOgreSceneNode->rotate(Ogre::Quaternion(Ogre::Degree(rotation.x), Ogre::Vector3(1, 0, 0)));
	pOgreSceneNode->rotate(Ogre::Quaternion(Ogre::Degree(rotation.y), Ogre::Vector3(0, 1, 0)));
	pOgreSceneNode->rotate(Ogre::Quaternion(Ogre::Degree(rotation.z), Ogre::Vector3(0, 0, 1)));

	_particleSystem->setEmitting(true);


}
CParticleRender::~CParticleRender() {
	pChild->detachObject(_particleSystem);
	pSceneMgr->destroyParticleSystem(_particleSystem);
}
void CParticleRender::tick(float delta) {



}
void CParticleRender::getMessage(Message * m) {
	CRender::getMessage(m);

}
#pragma endregion