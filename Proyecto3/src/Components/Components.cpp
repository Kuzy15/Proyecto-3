#include "Components.h"
#include "Entity.h"
#include <OgreEntity.h>
#include <OgreSceneNode.h>
#include "Messages.h"
#include "Game.h"
#include "EntityFactory.h"
#include "DebugNew.h"

//Debug 
#ifdef _DEBUG
#include <iostream>
#define new DEBUG_NEW
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
	//pSceneMgr->destroySceneNode(pChild);
	//pSceneMgr->destroySceneNode(pOgreSceneNode);
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
CMeshRender::CMeshRender(Ogre::Vector3 pos, float grades ,std::string meshName, Entity * father, Ogre::SceneManager * scnM, Ogre::Vector3 scale) :CRender(CMP_MESH_RENDER, father, scnM) {
	pOgreEnt = pSceneMgr->createEntity(meshName);
	pOgreSceneNode->setPosition(pos);
	pChild->setOrientation(Ogre::Quaternion(Ogre::Degree(grades), Ogre::Vector3(0, 0, 1)));
	pChild->attachObject(pOgreEnt);
	pChild->scale(scale);
}
CMeshRender::~CMeshRender() {
	//pChild->detachObject(pOgreEnt);
	//pSceneMgr->destroyEntity(pOgreEnt);
}
void CMeshRender::tick(float delta) {


	//std::cout << pOgreSceneNode->getPosition().x << std::endl;
	
	
	
	
}
void CMeshRender::getMessage(Message * m) {
	CRender::getMessage(m);
	//TO DO: IMPLEMENT MESSAGE RECEIVEING TO MOVE.

}


/*------------------------- CAMERA COMPONENTS------------------------------------*/
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

	//borrar
	_vp = vp;
}
CCamera::~CCamera() {
	delete _vp;
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

#pragma region Action Camera Component 
CActionCamera::CActionCamera(Entity * father, Ogre::SceneManager * scnMgr, Ogre::Viewport * vp):
	CCamera(father, scnMgr, vp, "MainCamera", Ogre::Vector3(0,0,100), Ogre::Vector3(0,0,0), 5), smooth(60) {
	_pj1 = Ogre::Vector3(20, 20, 0);
	_pj2 = Ogre::Vector3(-20, 20, 0);

	_lookAt = (_pj1 - _pj2) / 2.0;
	_newPos = _pos;
}

CActionCamera::~CActionCamera() {

}
void CActionCamera::getMessage(Message * m) {
	
	if (m->getType() == MSG_UPDATE_TRANSFORM && static_cast<MUpdateTransform *>(m)->getEmmiter() == "AhPuch") {
		MUpdateTransform * aux = static_cast<MUpdateTransform * >(m);
			if (aux->GetPos() != _pj1)
				_pj1 = aux->GetPos();

	}
	else if (m->getType() == MSG_UPDATE_TRANSFORM && static_cast<MUpdateTransform *>(m)->getEmmiter() == "Ra") {
		if (static_cast<MUpdateTransform *>(m)->GetPos() != _pj2)
			_pj2 = static_cast<MUpdateTransform *>(m)->GetPos();
	}
	else return;

	_toMove = Ogre::Vector3::ZERO;
	_newPos = (_pj1 + _pj2) / 2;
	_newPos.z = _pos.z; //Cambiar esto luego. Puenteado para la distancia
	_toMove = (_pos + _newPos) / smooth;
	_toMove.z = 0.0f;
	_lookAt = Ogre::Vector3(_newPos.x, _newPos.y, 0.0);

	//if("ra = -20")


}

void CActionCamera::tick(float delta) {
	float deltax = _newPos.x - _pos.x;
	float deltay = _newPos.y - _pos.y;
	if (sqrt(pow(deltax, 2)+pow(deltay,2)) > 5)
		_pos += _toMove;

	CCamera::tick(delta);

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
	if (myCategory == MASK_PLAYER)
		_bodyDef.fixedRotation = true;
	if (myCategory == MASK_BULLET){
		_bodyDef.bullet = true;
		_bodyDef.gravityScale = 0.0f;
	}

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
	_body->SetUserData(pEnt);	

	//Fixture Definition.	
	_fixtureDef.density = 950.0f;
	//_fixtureDef.restitution = 0.0f;
	_fixtureDef.friction = 0.0f;

	switch (myCategory)
	{
		case MASK_PLAYER:
			//_fixtureDef.filter.categoryBits = MASK_PLAYER;				
			_fixtureDef.filter.maskBits = MASK_STATIC_TERRAIN | MASK_DINAMIC_TERRAIN | MASK_CHEST;
			
			break;
		case MASK_STATIC_TERRAIN:
			_fixtureDef.filter.categoryBits = MASK_STATIC_TERRAIN;
			_fixtureDef.filter.maskBits = MASK_BULLET | MASK_DINAMIC_TERRAIN | MASK_LEGS | MASK_HEAD;
			break;
		case MASK_DINAMIC_TERRAIN:
			_fixtureDef.filter.categoryBits = MASK_DINAMIC_TERRAIN;
			_fixtureDef.filter.maskBits = MASK_BULLET | MASK_LEGS | MASK_HEAD;
			break;
		case MASK_BULLET:
			_fixtureDef.filter.categoryBits = MASK_BULLET;
			_fixtureDef.filter.maskBits = /*MASK_LEGS | MASK_CHEST | MASK_HEAD |*/ MASK_DINAMIC_TERRAIN | MASK_STATIC_TERRAIN;
			break;
		default:
			break;
	}

	float playerSize = _rbHeight / 3;
	float legsSize = _rbWeight / 1.5f;

	//Shape creation.
	switch (shType)
	{
	case SH_CIRCLE:
		_shape = new b2CircleShape;
		static_cast<b2CircleShape*>(_shape)->m_p.Set(0, 0);
		static_cast<b2CircleShape*>(_shape)->m_radius = _rbWeight;
		_fixtureDef.shape = _shape;
		//Fixture creation.
		_fixture = _body->CreateFixture(&_fixtureDef);
		break;
	case SH_POLYGON:
		_shape = new b2PolygonShape;
		static_cast<b2PolygonShape*>(_shape)->SetAsBox(_rbWeight / 2, _rbHeight / 2, { _rbWeight / 2, _rbHeight / 2 }, 0);
		_fixtureDef.shape = _shape;
		//Fixture creation.
		_fixture = _body->CreateFixture(&_fixtureDef);
		break;
	case SH_PLAYER:
		_shape = new b2PolygonShape;
		static_cast<b2PolygonShape*>(_shape)->SetAsBox(_rbWeight / 2, playerSize / 2, { _rbWeight / 2, _rbHeight / 2 }, 0);
		_fixtureDef.shape = _shape;
		_fixtureDef.filter.categoryBits = MASK_CHEST;
		_body->CreateFixture(&_fixtureDef);		
		
		static_cast<b2PolygonShape*>(_shape)->SetAsBox(_rbWeight / 2, playerSize / 2, { _rbWeight / 2, _rbHeight / 2 + playerSize}, 0);
		_fixtureDef.shape = _shape;
		_fixtureDef.filter.categoryBits = MASK_HEAD;
		_body->CreateFixture(&_fixtureDef);

		static_cast<b2PolygonShape*>(_shape)->SetAsBox( legsSize / 2, playerSize / 2, { _rbWeight / 2, _rbHeight / 2 - playerSize}, 0);
		_fixtureDef.shape = _shape;
		_fixtureDef.filter.categoryBits = MASK_LEGS;
		_fixture = _body->CreateFixture(&_fixtureDef);
		break;
	default:
		break;
	}

	
	


}
CRigidBody::~CRigidBody() {
	//_myWorld->DestroyBody(_body);
	_body = nullptr;
	
}
void CRigidBody::tick(float delta) {

	

	//Send the message to the entity.
	//Transformation from physics world to ogre world.
	
	
	MUpdateTransform * m = new MUpdateTransform(Ogre::Vector3((_body->GetPosition().x )* PPM , _body->GetPosition().y * PPM, 0), _body->GetAngle(),_rbHeight * PPM, _rbWeight * PPM, pEnt->getID());
	pEnt->getMessage(m);

	//std::cout << _body->GetAngle() << std::endl;


}

void CRigidBody::getMessage(Message * m) {

	
	MRigidbodyMoveX* mMoveX;
	float velX;
	MRigidbodyMoveY* mMoveY;
	float velY;
	MRigidbodyJump* mJump;
	float jForce;
	
	switch (m->getType()){
		case MSG_RIGIDBODY_MOVE_X:
			mMoveX = static_cast<MRigidbodyMoveX*>(m);
			velX = mMoveX->getXValue();
			_body->SetLinearVelocity(b2Vec2(velX, _body->GetLinearVelocity().y));
			break;
		case MSG_RIGIDBODY_MOVE_Y:
			mMoveY = static_cast<MRigidbodyMoveY*>(m);
			velY = mMoveY->getYValue();
			_body->SetLinearVelocity(b2Vec2(_body->GetLinearVelocity().x, velY));
			break;
		case MSG_RIGIDBODY_JUMP:
			mJump = static_cast<MRigidbodyJump*>(m);
			jForce = mJump->getForce();
			
			_body->ApplyForceToCenter(b2Vec2(0, jForce),true);
			break;
		case MSG_COLLISION_TERRAIN:
			_body->SetLinearVelocity(b2Vec2(_body->GetLinearVelocity().x,0));
			break;
		default:
			break;
	}
}
#pragma endregion

#pragma region CollisionHandler
CPlayerCollisionHandler::CPlayerCollisionHandler(Entity* father) :GameComponent(CMP_PLAYER_CH, father) {
}
CPlayerCollisionHandler::~CPlayerCollisionHandler(){
}

void CPlayerCollisionHandler::tick(float delta){}
void CPlayerCollisionHandler::getMessage(Message * m){
	

	if (m->getType() == MSG_COLLISION){
			
		MCollisionBegin* mColBegin = static_cast<MCollisionBegin*>(m);
		_myMask = mColBegin->GetMyCategory();
		std::cout << mColBegin->GetContactMask() << std::endl;
		switch (mColBegin->GetContactMask()){
			case MASK_STATIC_TERRAIN:
				if (_myMask == MASK_LEGS)
					pEnt->getMessage(new MCollisionTerrain(pEnt->getID()));
				break;
			default:
				break;
		}
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
				MJump* m = new MJump( pEnt->getID());
				pEnt->getMessage(m);
			}

			if (cState.Trigger_Right > 100){
				float xValue = cState.Axis_RightX;
				float yValue = cState.Axis_RightY;
				//Tal vez estas comprobaciones no sean necesarias, testear la sensibilidad de los joysticks al disparar
				float finalXValue, finalYValue;
				finalXValue = finalYValue = 0.0f;
				//Check joystick rotation, to control the bullet spawn
				if ((xValue > 10.0f || xValue < -10.0f) && (yValue > 10.0f || yValue < -10.0f)){
					finalXValue = cState.Axis_RightX;
					finalYValue = cState.Axis_RightY;
				}
				else{
					//if
					//if
				}
				MPlayerShot* m = new MPlayerShot(finalXValue, finalYValue, pEnt->getID());
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
CLife::CLife(Entity* father, float iniLife):GameComponent(CMP_LIFE,father), _maxLife(iniLife), _currentLife(iniLife){}
CLife::~CLife(){}

void CLife::tick(float delta){}
void CLife::getMessage(Message* m){}
#pragma endregion

//Move Component
#pragma region Player Move Component
CPlayerMove::CPlayerMove(Entity* father, float vel) :GameComponent(CMP_MOVEMENT_SPEED, father), _maxSpeed(MAX_SPEED), _moveVel(vel){}
CPlayerMove::~CPlayerMove(){}

void CPlayerMove::tick(float delta){}
void CPlayerMove::getMessage(Message* m)
{
	if (m->getType() == MSG_PLAYER_MOVE_X){
		//transformarlo
		float value = static_cast<MPlayerMoveX*>(m)->GetValue();
		value = value / 200.0f;
		if (value > _moveVel)
			value = _moveVel;
		else if (value < - _moveVel)
			value = - _moveVel;
		
		pEnt->getMessage(new MRigidbodyMoveX(value, pEnt->getID()));
	}


}
#pragma endregion

//Jump Component
#pragma region Player Jump Component
CPlayerJump::CPlayerJump(Entity* father, float startForce) :GameComponent(CMP_JUMP, father), _maxForce(MAX_JUMP_DISTANCE), _jumpForce(startForce), _maxJumps(2), _nJumps(2){}
CPlayerJump::~CPlayerJump(){}

void CPlayerJump::tick(float delta){}
void CPlayerJump::getMessage(Message* m)
{

	switch (m->getType()){
	case MSG_PLAYER_JUMP:
		if (_nJumps > 0){
			std::cout << _nJumps << std::endl;
			pEnt->getMessage(new MRigidbodyJump(_jumpForce * _nJumps, pEnt->getID()));
			_nJumps--;
		}
		break;
	case MSG_COLLISION_TERRAIN:
		_nJumps = _maxJumps;
		break;

	}
}
#pragma endregion

//Basic Attack Component
#pragma region Player Basic Attack Component

CPlayerBasicAttack::CPlayerBasicAttack(Entity* father, float fireRate, E_BULLET bT, Ogre::Vector3 entPos) :GameComponent(CMP_BASIC_ATTACK, father),
_maxFireRate(MAX_FIRE_RATE), _fireRate(fireRate), _bulletType(bT), _ogrepos(entPos)
{
	_lastTimeShot = 0;
	_timeCounter = 0;
	_radius = 10.0f;

}
CPlayerBasicAttack::~CPlayerBasicAttack(){

}
void CPlayerBasicAttack::tick(float delta){


}
void CPlayerBasicAttack::getMessage(Message* m){

	if (m->getType() == MSG_PLAYER_SHOT){
		MPlayerShot* mPS = static_cast<MPlayerShot*>(m);
		
		//Check if the player can spawn the next bullet
		_timeCounter = (SDL_GetTicks());
		if ((_timeCounter - _lastTimeShot) > _fireRate){
			float angle;
			Ogre::Vector3 iniPos;
			calculateSpawnPoint(mPS->getXValue(), mPS->getYValue(), angle, iniPos);
			Ogre::Vector3 dir = iniPos;
			iniPos.x += _ogrepos.x;
			iniPos.y += _ogrepos.y;
			iniPos.z = _ogrepos.z;

			Entity* b = EntityFactory::getInstance().createBullet(_bulletType, pEnt->getScene(), iniPos, angle);
			pEnt->getMessage(new MAddEntity(pEnt->getID(), b));
			b->getMessage(new MShot(dir.x, dir.y,pEnt->getID()));

			_lastTimeShot = (SDL_GetTicks());
		}
	}

	else if (m->getType() == MSG_UPDATE_TRANSFORM){
		if (m->getEmmiter() == pEnt->getID()){
			//Message cast
			MUpdateTransform* msg = static_cast<MUpdateTransform *>(m);

			//We get the size of the colliderbox.
			float w = msg->getW();
			float h = msg->getH();



			//Where our node will rotate.
			Ogre::Vector3 parentPos = msg->GetPos();

			//Where our mesh is relative to the parent. The real pos of the object is the parent pos + this variable, _ogrepos.
			_ogrepos.x = parentPos.x +  w / 2;
			_ogrepos.y = parentPos.y + h / 2;
			_ogrepos.z = parentPos.z;

			//Rotate the parent node the same degree as the collider.
			float angleRad = msg->getRotation();
			float grades = (angleRad * 180) / 3.14159265359;
		}
	}
}
void CPlayerBasicAttack::calculateSpawnPoint(float vX, float vY, float &angle, Ogre::Vector3 &iniPos){

	//Calculate point
	/*5 - 327*/
	//Normalize
	if (vX == 0){
		iniPos.x = 0;
		iniPos.y = _radius;
		iniPos.z = 0;

		if (vY > 0)
			angle = 180;
		else
			angle = 0;
	}
	else{

		float normalX = vX * SPAWN_PARSE;
		float normalY = vY * SPAWN_PARSE;

		//std::cout << vX << " " << vY << std::endl;

		float A = vY;
		float B = -vX;
		float yFinal = std::sqrt(std::pow(_radius, 2) / (std::pow((-(B) / A), 2) + 1));
		float xFinal = ((-B * yFinal) / A);


		//std::cout << xFinal << " " << yFinal << std::endl;

		if (vY > 0.0f){
			if (yFinal < 0)
				yFinal = std::abs(yFinal);
		}
		else{
			if (yFinal > 0)
				yFinal *= -1.0f;
		}

		if (vX > 0.0f){
			if (xFinal < 0)
				xFinal = std::abs(xFinal);
		}
		else{
			if (xFinal > 0)
				xFinal *= -1.0f;
		}

		//std::cout << xFinal << " " << yFinal << std::endl;


		iniPos = Ogre::Vector3(xFinal, yFinal, 0.0f);
		Ogre::Vector3 idleVector(0.0f, _radius, 0.0f);

		float escalarProduct = (iniPos.x * idleVector.x) + (iniPos.y * idleVector.y);
		float lengthIni = std::sqrt(std::pow(iniPos.x, 2) * std::pow(iniPos.y, 2));
		float lengthIdle = _radius;

		float cos = escalarProduct / (lengthIdle * lengthIni);
		std::cout << cos << std::endl;

		if (cos > 1)
			cos = 1;
		else if (cos < -1)
			cos = -1;

		angle = std::acosf(cos);
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

	
 	MShot* mShot;
	float xDir;
	float yDir;
	

	switch (m->getType()){
	case MSG_SHOT:
		mShot = static_cast<MShot*>(m);

		xDir = (mShot->getXValue() / 200.0f) * _velocity;
		yDir = (mShot->getYValue() / 200.0f) * _velocity;

		pEnt->getMessage(new MRigidbodyMoveX(xDir, pEnt->getID()));
		pEnt->getMessage(new MRigidbodyMoveY(yDir, pEnt->getID()));
		
		break;
	
	default:
		break;
	}

}
#pragma endregion
