#ifndef RENDERCOMPONENTS_H
#define RENDERCOMPONENTS_H

#include "Components.h"
//#include <Ogre.h>
class Entity;
class Message;





/*-------------------------RENDER COMPONENTS------------------------------------*/
//--------- RENDER COMPONENT ---------
class CRender : public GameComponent
{
public:
	virtual ~CRender();
	//Ogre::SceneNode * getSceneNode();
	virtual void getMessage(Message *m);


protected:
	CRender(ComponentType t, Entity * father, Ogre::SceneManager * scnM);
	// Inside the Ogre Node we can find al the render values needed by ogre
	Ogre::SceneNode * pOgreSceneNode;
	// Pointer to the Ogre Scene Manager that created the component
	Ogre::SceneManager * pSceneMgr;
	// Cache value for the rendering positions. They will be updated when needed in the tick function
	Ogre::Vector3 _ogrepos;
	Ogre::Quaternion _ogrequat;
	Ogre::SceneNode* pChild;
	float lastDir;

};

//--------- PARTICLE RENDER COMPONENT ---------
class CParticleRender : public CRender
{
public:

	CParticleRender(Ogre::Vector3 pos, std::string id, std::string particleSystem, Entity * father, Ogre::SceneManager * scnM, Ogre::Vector3 scale, Ogre::Vector3 rotation);
	~CParticleRender();


	virtual void tick(float delta);
	virtual void getMessage(Message * m);
private:
	Ogre::ParticleSystem* _particleSystem;



};

//--------- RIBBON TRAIL RENDER COMPONENT ---------
class CRibbonTrailRender : public CRender
{
public:

	CRibbonTrailRender(Ogre::Vector3 pos, std::string id, std::string trail, Entity * father, Ogre::SceneManager * scnM, Ogre::Vector3 scale, Ogre::Vector3 rotation);
	~CRibbonTrailRender();


	virtual void tick(float delta);
	virtual void getMessage(Message * m);
private:
	Ogre::RibbonTrail * trail;

};


//--------- SKYPLANE RENDER COMPONENT ---------
class CSkyPlaneRender : public CRender
{
public:

	CSkyPlaneRender(Entity * father, Ogre::SceneManager * scnM, float scale, std::string materialName, Ogre::Vector3 pos, Ogre::Viewport* vp);
	~CSkyPlaneRender();


	virtual void tick(float delta);
	virtual void getMessage(Message * m);

private:
	Ogre::MeshPtr plane;
	Ogre::Entity* entity;
};


//--------- MESH RENDER COMPONENT ---------
class CMeshRender : public CRender
{
public:

	CMeshRender(Ogre::Vector3 pos, std::string meshName, Entity * father, Ogre::SceneManager * scnM, Ogre::Vector3 scale, Ogre::Vector3 rotation);
	~CMeshRender();


	virtual void tick(float delta);
	virtual void getMessage(Message * m);
	Ogre::Vector3 getSize();
	Ogre::SceneNode* getChildNode(){ return pChild; }
	inline void setVisible(bool visible){ pOgreEnt->setVisible(visible); };

private:
	Ogre::Entity * pOgreEnt;
	bool invisActive;           //is invis´ component added



};

#endif