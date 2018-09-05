#include "Camera.h"
#include "Messages.h"
#include "Game.h"
#include "Entity.h"


#pragma region Camera Component
CCamera::CCamera(Entity * father, Ogre::SceneManager * scnMgr, Ogre::Viewport * vp, std::string camName, Ogre::Vector3 pos, Ogre::Vector3 lookAt, int clipDistance)
	: GameComponent(CMP_CAMERA, father), _scnMgr(scnMgr), _camName(camName), _vp(vp), _pos(pos), _lookAt(lookAt), pCam(0)
{
	pCam = _scnMgr->createCamera(_camName);
	Ogre::RenderWindow* w = Game::getInstance()->getRenderWindow();
	vp = w->addViewport(pCam);


	pCam->setPosition(_pos);
	pCam->lookAt(_lookAt);
	pCam->setAspectRatio(
		Ogre::Real(vp->getActualWidth()) /
		Ogre::Real(vp->getActualHeight()));

	pCam->setNearClipDistance(clipDistance);

	vp->setBackgroundColour(Ogre::ColourValue(0.2, 0.5, 0.8));

	//borrar
	_vp = vp;


}
CCamera::~CCamera() {
	delete pCam;
}
void CCamera::tick(float delta) {
	if (_lookAt != _lastLookAt) {
		pCam->lookAt(_lookAt);
		_lastLookAt = _lookAt;
	}
	if (_pos != _lastPos) {
		pCam->setPosition(_pos);
		_lastPos = _pos;
	}

}
void CCamera::getMessage(Message * m) {

}
#pragma endregion



#pragma region Action Camera Component
CActionCamera::CActionCamera(Entity * father, Ogre::SceneManager * scnMgr, Ogre::Viewport * vp, float xBoundary, float yBoundary, float minZ, float maxZ) :
CCamera(father, scnMgr, vp, "MainCamera", Ogre::Vector3(0, 0, 100), Ogre::Vector3(0, 0, 0), 5),
smooth(40.0), MAXZ(maxZ), MINZ(minZ), BOUNDARY_X(xBoundary), BOUNDARY_Y(yBoundary) {
	_pj1 = Ogre::Vector3(20, 20, 0);
	_pj2 = Ogre::Vector3(-20, 20, 0);

	//Inicializating values for the camera
	_lastPos = Ogre::Vector3(0, 0, 100);
	_pos = Ogre::Vector3(0, 0, 100);

	//We start looking at the center of the 2 players
	_lookAt = (_pj1 + _pj2) / 2.0;

	//And we calculate the zRatio.
	zRatio = (_pj1.distance(_pj2) / _pos.z);
}

CActionCamera::~CActionCamera() {

}

//Function that calculates if a certain point is out of the defined boundaries for the camera
bool CActionCamera::outOfBoundaries(const Ogre::Vector3 &pos) {
	//Boundaries in each axis


	if ((pos.x > BOUNDARY_X || pos.x < -BOUNDARY_X) || (pos.y > BOUNDARY_Y || pos.y < -BOUNDARY_Y))return true;
	else return false;
}
void CActionCamera::getMessage(Message * m) {

	if (m->getType() == MSG_CAMERA_FOLLOW && static_cast<MCameraFollow *>(m)->getEmmiter() == "Player_0") {
		//We check if the player is inside the boundaries of the camera. If so, we put its camera position to 0.0.0
		if (outOfBoundaries(static_cast<MCameraFollow *>(m)->GetPos()))
			_pj1 = Ogre::Vector3::ZERO;
		//if it has moved from its previous position, we update its position
		else
			_pj1 = static_cast<MCameraFollow * >(m)->GetPos();

	}
	else if (m->getType() == MSG_CAMERA_FOLLOW && static_cast<MCameraFollow *>(m)->getEmmiter() == "Player_1") {
		if (outOfBoundaries(static_cast<MCameraFollow *>(m)->GetPos()))
			_pj2 = Ogre::Vector3::ZERO;
		else
			_pj2 = static_cast<MCameraFollow * >(m)->GetPos();
	}
	else return;

	//We calculate the midpoint between the 2 players
	_newPos = (_pj1 + _pj2) / 2.0;

	//we want the camera to be always a bit higher than the players
	_newPos.y += 5;

	//Now we want to know the camera new Z position
	float camz = (_pj1.distance(_pj2) / zRatio);


	//We dont want the camera to go too far away or too close
	if (camz >= MAXZ)_newPos.z = MAXZ;
	else if (camz < MINZ)_newPos.z = MINZ;
	else _newPos.z = camz;

	//Now we want to make it smooth, for that we calculate the director vector of the line.
	//And we divide it by the smooth factor we declared at initialization
	Ogre::Vector3 dir = (_newPos - _pos) / smooth;


	_pos += dir;
	_lookAt = _pos;
	_lookAt.z = 0;
	_lookAt.y += 10;


}

void CActionCamera::tick(float delta) {
	CCamera::tick(delta);

}


#pragma region Camera Follow
CCameraFollow::CCameraFollow(Entity * father) :GameComponent(CMP_CAMERA_FOLLOW, father){

	_camFollow = nullptr;
}
CCameraFollow::~CCameraFollow(){

	if (_camFollow != nullptr){
		delete _camFollow;
		_camFollow = nullptr;
	}
}

void CCameraFollow::tick(float delta){

	_camFollow = new MCameraFollow(_nPos, pEnt->getID());
	pEnt->getMessage(_camFollow);

}
void CCameraFollow::getMessage(Message* m){

	if (m->getType() == MSG_UPDATE_TRANSFORM){
		_nPos = static_cast<MUpdateTransform*>(m)->GetPos();
	}

}
#pragma endregion