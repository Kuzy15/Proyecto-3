#ifndef CAMERA_H
#define CAMERA_H
#include "Components.h"

class Entity;
class Message;

//---------   CAMERA COMPONENT   ---------
class CCamera : public GameComponent
{
public:
	CCamera(Entity * father, Ogre::SceneManager * scnMgr, Ogre::Viewport * vp, std::string camName, Ogre::Vector3 pos, Ogre::Vector3 lookAt, int clipDistance);
	virtual ~CCamera();
	virtual void tick(float delta);
	virtual void getMessage(Message * m);
	inline Ogre::Viewport* getVP(){ return _vp; };


protected:
	std::string _camName;
	Ogre::SceneManager * _scnMgr;
	Ogre::Viewport * _vp;
	Ogre::Vector3 _pos, _lastPos, _newPos;
	Ogre::Vector3 _lookAt, _lastLookAt;



	//Parameters for the camera movement
	Ogre::Camera * pCam;
};

class CActionCamera : public CCamera
{
public:
	CActionCamera(Entity * father, Ogre::SceneManager * scnMgr, Ogre::Viewport * vp, float xBoundary, float yBoundary, float minZ, float maxZ);
	~CActionCamera();

	virtual void getMessage(Message * m);
	virtual void tick(float delta);
	bool outOfBoundaries(const Ogre::Vector3 &pos);


	const float BOUNDARY_X;
	const float BOUNDARY_Y;

	//Cache positions of the players
	Ogre::Vector3 _pj1, _pj2;
	//Vector that we will use to add to our position
	Ogre::Vector3 _toMove;
	//Smooth factor, so the camera does not move sharply
	const float smooth;
	//Max and min Z coordinates for the camera.
	const float MAXZ, MINZ;

	//Z ratio, to calculate the Z after initialization
	float zRatio;

private:

};

/*---------------------------------- CAMERA FOLLOW -----------------------------------*/

class CCameraFollow : public GameComponent
{
public:
CCameraFollow(Entity * father);
virtual ~CCameraFollow();

virtual void getMessage(Message * m);
virtual void tick(float delta);
private:
Ogre::Vector3 _nPos;
MCameraFollow * _camFollow;
};

#endif