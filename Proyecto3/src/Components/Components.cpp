#include "Components.h"
#include "Entity.h"
#include <OgreEntity.h>
#include <OgreSceneNode.h>
#include "Messages.h"
#include "Game.h"



//Debug 
#ifdef _DEBUG
#include <iostream>
#endif


/*-------------------------BASIC GAME COMPONENT------------------------------------*/
//Component base class, made for inheriting.
//It implements basic behaviours like gets, sets 
//and message sending
#pragma region gameComponent
GameComponent::GameComponent(ComponentType id, Entity * father): _id(id), pEnt(father)
{
	
}
GameComponent::~GameComponent(){

}
ComponentType GameComponent::getID(){
	return _id;
}
bool GameComponent::getActive(){
	return _active;
}
void GameComponent::setActive(bool nw){
	_active = nw;
}
void GameComponent::sendMessage(Message * m){
	pEnt->getMessage(m);
}

#pragma endregion

/*-------------------------DEBUG COMPONENTS------------------------------------*/
//PRINTS A STRING WHEN RECEIVEING A STRING_MESSAGE
#pragma region stringComponent
CString::CString(Entity * fath) : GameComponent(CMP_STRING, fath), whatSay("HOLA, soy el componente basico"){
}
CString::~CString(){

}
void CString::getMessage(Message * m){
#ifdef _DEBUG
	//if (m->getType() == STRING_MSG)std::cout << "MESSAGE SAID: " << static_cast<stringMessage*>(m)->getText() << std::endl;
	
#endif
}
void CString::tick(float delta){
#ifdef _DEBUG
#endif
}

#pragma endregion

#pragma region messageSendComponent
CMessageSend::CMessageSend(Entity * father):GameComponent(CMP_MESSAGE_SEND, father) {
	i = 0;
}
CMessageSend::~CMessageSend() {
}
void CMessageSend::tick(float delta) {
	/*i++;
	MUpdateTransform * m = new MUpdateTransform(Ogre::Vector3(0, 0, i/5.0), 3.14*i/180.0, pEnt->getID());
	pEnt->getMessage(m);*/

	
}
void CMessageSend::getMessage(Message * m) {

}

#pragma endregion


/*-------------------------OGRE COMPONENTS------------------------------------*/
//Render Component class. Father to every
//other render component.
#pragma region renderComponent

CRender::CRender(ComponentType t, Entity * father, Ogre::SceneManager * scnM)
	: GameComponent(t, father), pSceneMgr(scnM)
{
	pOgreSceneNode = pSceneMgr->getRootSceneNode()->createChildSceneNode();
	pChild = pOgreSceneNode->createChildSceneNode();
	
}
CRender::~CRender(){
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
			_ogrepos.x = w / 2;
			_ogrepos.y = 0;	
			_ogrepos.z = 0;

			//Move the parent to the collider location of rotation.
			pOgreSceneNode->setPosition(parentPos);			
			//Move the child to the real pos of the collider.
			pChild->setPosition(_ogrepos);

			//Rotate the parent node the same degree as the collider.
			float angleRad = msg->getRotation();
			float grades = (angleRad * 180) / 3.14159265359;
			pOgreSceneNode->setOrientation(Ogre::Quaternion(Ogre::Degree(grades), Ogre::Vector3(0, 0, 1)));

		
		}
		break;
	default: 
		break;
	}
}

#pragma endregion


//Mesh Render component.
//Takes a string with the name of the mesh to render
//and renders it.
#pragma region meshRenderComponent
CMeshRender::CMeshRender(Ogre::Vector3 p, std::string meshName, Entity * father, Ogre::SceneManager * scnM) :CRender(CMP_MESH_RENDER, father, scnM) {
	pOgreEnt = pSceneMgr->createEntity(meshName);
	
	pChild->attachObject(pOgreEnt);
	pOgreSceneNode->setPosition(p);
	
	_ogrepos = p;
	_ogrequat = Ogre::Quaternion();
	
}
CMeshRender::~CMeshRender() {
	pChild->detachObject(pOgreEnt);
	pSceneMgr->destroyEntity(pOgreEnt);
}
void CMeshRender::tick(float delta) {



	
	
	
	
}
void CMeshRender::getMessage(Message * m) {
	CRender::getMessage(m);
	//TO DO: IMPLEMENT MESSAGE RECEIVEING TO MOVE.

}

#pragma endregion
#pragma region Camera Component
CCamera::CCamera(Entity * father, Ogre::SceneManager * scnMgr, Ogre::Viewport * vp, std::string camName, Ogre::Vector3 pos, Ogre::Vector3 lookAt, int clipDistance)
	: GameComponent(CMP_CAMERA, father), _scnMgr(scnMgr), _camName(camName), _vp(vp), _pos(pos), _lookAt(lookAt), pCam(0)
{
	pCam = _scnMgr->createCamera(_camName);
	vp = Game::getInstance()->getRenderWindow()->addViewport(pCam); 

	pCam->setPosition(_pos);
	pCam->lookAt(_lookAt);
	pCam->setAspectRatio(
		Ogre::Real(vp->getActualWidth()) /
		Ogre::Real(vp->getActualHeight()));

	pCam->setNearClipDistance(clipDistance);

	vp->setBackgroundColour(Ogre::ColourValue(0.5, 0.5, 0.5));
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
	//DEBUG MESSAGE RECEIVING
	/*if (m->getType() == MSG_UPDATE_TRANSFORM) {
		std::cout << "MESSAGE RECEIVED" << std::endl;
		_lookAt = static_cast<MUpdateTransform *> (m)->GetPos();
	}
	*/

}

#pragma endregion



/*-------------------------BOX2D COMPONENTS------------------------------------*/

//Rigid Body component.
//Gives an entity a rigid body to simulate physics
#pragma region RigidBodyComponent
CRigidBody::CRigidBody(Entity * father, b2World * world, Ogre::Vector3 posInPixels, float heightInPixels, float weightInPixels, RigidBodyType rbType, ShapeType shType, FilterMask myCategory)
: _rbHeight(heightInPixels / PPM), _rbWeight(weightInPixels / PPM), _myWorld(world), GameComponent(CMP_PHYSICS,father) {
	
	//Sets the pos attached to the render.
	_pos.x = posInPixels.x / PPM;
	_pos.y = posInPixels.y / PPM;

	//Initial velocity 0.
	_vel.x = _vel.y = 0;

	//Body definition.
	_bodyDef.position.Set(_pos.x, _pos.y);
	/*_bodyDef.linearDamping = 5.0f;
	_bodyDef.angularDamping = 0.0f;*/
	
	switch (rbType)
	{
		case RB_DYNAMIC:
			_bodyDef.type = b2_dynamicBody;
			break;
		case RB_STATIC:
			_bodyDef.type = b2_staticBody;
			break;
		case RB_KINEMATIC:
			_bodyDef.type = b2_kinematicBody;
			break;
		default:
			_bodyDef.type = b2_staticBody;
			break;
	}
	//Body creation.
	_body = _myWorld->CreateBody(&_bodyDef);
	

	//Set the body data pointer to entity
	_body->SetUserData(this);

	
	//Shape creation.
	switch (shType)
	{
		case SH_CIRCLE:
			_shape = new b2CircleShape;
			static_cast<b2CircleShape*>(_shape)->m_p.Set(0, 0);
			static_cast<b2CircleShape*>(_shape)->m_radius = _rbWeight;
			break;
		case SH_POLYGON:
			_shape = new b2PolygonShape;
			static_cast<b2PolygonShape*>(_shape)->SetAsBox(_rbWeight / 2, _rbHeight / 2, { _rbWeight / 2, _rbHeight / 2 }, 0);
			break;
		default:
			break;
	}

	

	//Fixture Definition.
	_fixtureDef.shape = _shape;
	_fixtureDef.density = 950.0f;
	_fixtureDef.restitution = 0.0f;
	_fixtureDef.friction = 0.3f;

	switch (myCategory)
	{
		case MASK_PLAYER:
			_fixtureDef.filter.categoryBits = MASK_PLAYER;				
			_fixtureDef.filter.maskBits = MASK_BULLET | MASK_STATIC_TERRAIN | MASK_DINAMIC_TERRAIN;
			break;
		case MASK_STATIC_TERRAIN:
			_fixtureDef.filter.categoryBits = MASK_STATIC_TERRAIN;
			/*_fixtureDef.filter.maskBits = ;*/
			break;
		case MASK_DINAMIC_TERRAIN:
			_fixtureDef.filter.categoryBits = MASK_DINAMIC_TERRAIN;
			_fixtureDef.filter.maskBits = MASK_BULLET;
			break;
		case MASK_BULLET:
			_fixtureDef.filter.categoryBits = MASK_BULLET;
			_fixtureDef.filter.maskBits = MASK_PLAYER;
			break;
		default:
			break;
	}
	/* FALTA ESTO POR CONFIGURAR
	fDef.filter.categoryBits = Juego::JUGADOR;
	fDef.filter.maskBits = Juego::ENEMIGO | Juego::ITEM | Juego::ESCENARIO | Juego::ESCENARIO_NOCOL | Juego::AT_ENEMIGO;
	*/
	
	//Fixture creation.
	_fixture = _body->CreateFixture(&_fixtureDef);
	


}
CRigidBody::~CRigidBody() {
	_myWorld->DestroyBody(_body);
	_body = nullptr;
}
void CRigidBody::tick(float delta) {

	

	//Send the message to the entity.
	//Transformation from physics world to ogre world.
	
	
	MUpdateTransform * m = new MUpdateTransform(Ogre::Vector3((_body->GetPosition().x  /*+ (_rbWeight / 2)*/)* PPM , _body->GetPosition().y /*+ (_rbHeight / 2)9*/ * PPM, 0), _body->GetAngle(),_rbHeight * PPM, _rbWeight * PPM, pEnt->getID());
	pEnt->getMessage(m);

	std::cout << _body->GetAngle() << std::endl;


}

void CRigidBody::getMessage(Message * m) {

	if (m->getType() == MSG_PLAYER_MOVE_X){
		//transformarlo
		float value = static_cast<MPlayerMoveX*>(m)->GetValue();
		value = value / 1000;
		b2Vec2 v (value,0);
		_body->SetLinearVelocity(v);
		
		std::cout << "Aplicada vel de: " << value << std::endl;
	}
	else if (m->getType() == MSG_PLAYER_JUMP){
		if (static_cast<MJump*>(m)->GetJump()){
			b2Vec2 newForce(0, 700);
			//_body->ApplyForceToCenter(newForce, true);		
			
			_body->SetAngularVelocity(2);
			
		}
	
	}
}
#pragma endregion

CPlayerCollisionHandler::CPlayerCollisionHandler(Entity* father) :GameComponent(CMP_PLAYER_CH, father) {
}
CPlayerCollisionHandler::~CPlayerCollisionHandler(){
}

void CPlayerCollisionHandler::tick(float delta){}
void CPlayerCollisionHandler::getMessage(Message * m){
	
	//If the msg is the type Collision, read the mask and category bits and process it.
	if (m->getType() == MSG_COLLISION){
		uint16_t me = static_cast<MCollisionBegin*>(m)->GetMyCategory();
		uint16_t contact = static_cast<MCollisionBegin*>(m)->GetContactMask();
		if (contact == MASK_BULLET){
			//me haacen pupa
		}/*
		else if (){
		
		
		}*/	
	
	}

}




#pragma endregion

//Player Controller Component
#pragma region PlayerControllerComponent
CPlayerController::CPlayerController(Entity* f, int i): GameComponent(CMP_PLAYER_CONTROLLER, f), _id(i){
	
}

CPlayerController::~CPlayerController(){}

void CPlayerController::tick(float delta){
}

void CPlayerController::getMessage(Message* m){
	//If the msg type is CInputState, read the input and process it
	if (m->getType() == MSG_INPUT_STATE){
		MInputState* inputM = static_cast<MInputState*>(m);
		if (inputM->getId() == _id){

			ControllerInputState cState = inputM->getCInputState();

			if (cState.Button_A == BTT_PRESSED){
				MJump* m = new MJump(true, pEnt->getID());
				pEnt->getMessage(m);
			}
			

			if (cState.Axis_LeftX > 100){
				MPlayerMoveX* m = new MPlayerMoveX(cState.Axis_LeftX, _id, pEnt->getID());
				pEnt->getMessage(m);				
			}
			else if (cState.Axis_LeftX < -100){
				MPlayerMoveX* m = new MPlayerMoveX(cState.Axis_LeftX, _id, pEnt->getID());
				pEnt->getMessage(m);
			}
			else if (cState.Axis_LeftX <= 100 && cState.Axis_LeftX >= -100){
				MPlayerMoveX* m = new MPlayerMoveX(0, _id, pEnt->getID());
				pEnt->getMessage(m);
			}
		}
	}

}

#pragma endregion

//Life Component
#pragma region Life Component
CLife::CLife(Entity* father):GameComponent(CMP_LIFE,father), maxLife(MAX_LIFE), _currentLife(MAX_LIFE){}
CLife::~CLife(){}

void CLife::tick(float delta){}
void CLife::getMessage(Message* m){}
#pragma endregion

//Move Component
#pragma region Player Move Component
CPlayerMove::CPlayerMove(Entity* father, float vel) :GameComponent(CMP_MOVEMENT_SPEED, father), _maxSpeed(MAX_SPEED), _moveVel(vel){}
CPlayerMove::~CPlayerMove(){}

void CPlayerMove::tick(float delta){}
void CPlayerMove::getMessage(Message* m){}
#pragma endregion

//Jump Component
#pragma region Player Jump Component
CPlayerJump::CPlayerJump(Entity* father, float startDist) :GameComponent(CMP_JUMP, father), _maxDistance(MAX_JUMP_DISTANCE), _jumpDist(startDist){}
CPlayerJump::~CPlayerJump(){}

void CPlayerJump::tick(float delta){}
void CPlayerJump::getMessage(Message* m){}
#pragma endregion

//Basic Attack Component
#pragma region Player Basic Attack Component

CPlayerBasicAttack::CPlayerBasicAttack(Entity* father, float fireRate, E_BULLET bT) :GameComponent(CMP_BASIC_ATTACK, father),
_maxFireRate(MAX_FIRE_RATE), _fireRate(fireRate), _bulletType(bT) 
{
	_lastTimeShot = 0;
	_timeCounter = 0;

}
CPlayerBasicAttack::~CPlayerBasicAttack(){}

void CPlayerBasicAttack::tick(float delta){}
void CPlayerBasicAttack::getMessage(Message* m){

	if (m->getType() == MSG_PLAYER_SHOT){
		MPlayerShot* mPS = static_cast<MPlayerShot*>(m);

		//Check if the player can spawn the next bullet
		_timeCounter = (SDL_GetTicks() * 1000);
		if ((_timeCounter - _lastTimeShot) > _fireRate){
			//Spawn bullet
			_lastTimeShot = (SDL_GetTicks() * 1000);
		}
	}
}
#pragma endregion

//Bullet Component
#pragma region Bullet Component

CBullet::CBullet(Entity* father, E_BULLET bT, float damage, float vel) :GameComponent(CMP_BASIC_ATTACK, father), _damage(damage)
, _velocity(vel)
{
	

}
CBullet::~CBullet(){}

void CBullet::tick(float delta){}
void CBullet::getMessage(Message* m){

	
}
#pragma endregion
