#include "PlayerComponents.h"
#include "Entity.h"
#include "EntityFactory.h"
#include "Scenes.h"
#include "Game.h"

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
		//std::cout << mColBegin->GetContactMask() << std::endl;
		switch (mColBegin->GetContactMask()){
		case MASK_STATIC_TERRAIN:
			if (_myMask == MASK_FOOT_SENSOR)
				pEnt->getMessage(new MCollisionTerrain(pEnt->getID()));
			break;
		case MASK_DEATHZONE:
			pEnt->getMessage(new MDie(pEnt->getID()));
			break;
		default:
			break;
		}
	}
	else if (m->getType() == MSG_BULLET_HIT){
		MBulletHit* mBH = static_cast<MBulletHit*>(m);
		if (mBH->getTargetMask() == MASK_CHEST_0 || mBH->getTargetMask() == MASK_CHEST_1)
			pEnt->getMessage(new MDamage(mBH->getDamage(), pEnt->getID()));
		else
			pEnt->getMessage(new MDamageArmature(mBH->getDamage(), mBH->getTargetMask(), pEnt->getID()));
	}
}
#pragma endregion

//Player Controller Component
#pragma region PlayerControllerComponent
CPlayerController::CPlayerController(Entity* f, int i) : GameComponent(CMP_PLAYER_CONTROLLER, f), _id(i){

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

			if (cState.Right_Shoulder == BTT_PRESSED){
				MJump* m = new MJump(pEnt->getID());
				pEnt->getMessage(m);
				//Game::getInstance()->getSoundEngine()->play2D("../Media/sounds/Pruebo.ogg");
			}


			float xValue = cState.Axis_RightX;
			float yValue = cState.Axis_RightY;
			//Check joystick rotation, to control the bullet spawn
			if (!(xValue == 0.0f && yValue == 0.0f)){
				MPlayerShot* m = new MPlayerShot(xValue, yValue, pEnt->getID());
				pEnt->getMessage(m);
			}
			if (cState.Axis_LeftX > AXIS_DEADZONE){
				MPlayerMoveX* m = new MPlayerMoveX(cState.Axis_LeftX, _id, pEnt->getID());
				pEnt->getMessage(m);
			}
			else if (cState.Axis_LeftX < -AXIS_DEADZONE){
				MPlayerMoveX* m = new MPlayerMoveX(cState.Axis_LeftX, _id, pEnt->getID());
				pEnt->getMessage(m);
			}
			else if (cState.Axis_LeftX <= AXIS_DEADZONE && cState.Axis_LeftX >= -AXIS_DEADZONE){
				MPlayerMoveX* m = new MPlayerMoveX(0, _id, pEnt->getID());
				pEnt->getMessage(m);
			}
		}
	}

}

#pragma endregion



//Life Component
#pragma region Life Component
CLife::CLife(Entity* father, float iniLife) :GameComponent(CMP_LIFE, father), _maxLife(iniLife), _currentLife(iniLife) {


}
CLife::~CLife(){}

void CLife::tick(float delta){



#ifdef DEBUG

	//std::cout << _currentLife << std::endl;
#endif //  DEBUG

}
void CLife::getMessage(Message* m){

	switch (m->getType()){
	case MSG_DAMAGE:
		if (_currentLife > 0.0f){
			_currentLife -= static_cast<MDamage*>(m)->getDamage();
			pEnt->getMessage(new MLifeState(pEnt->getID(), _currentLife));
			if (_currentLife <= 0.0f){
				pEnt->getMessage(new MDie(pEnt->getID()));
			}
		}
		break;
	default:
		break;

	}
}
#pragma endregion


//Move Component
#pragma region Player Move Component
CPlayerMove::CPlayerMove(Entity* father, float vel) :GameComponent(CMP_MOVEMENT_SPEED, father), _maxSpeed(MAX_SPEED), _moveVel(vel), _auxVelReset(vel){}
CPlayerMove::~CPlayerMove(){}

void CPlayerMove::tick(float delta){}
void CPlayerMove::getMessage(Message* m)
{
	if (m->getType() == MSG_PLAYER_MOVE_X){
		//transformarlo
		float value = static_cast<MPlayerMoveX*>(m)->GetValue();
		//value = value / 40;

		if (value > _moveVel)
			value = _moveVel;
		else if (value < -_moveVel)
			value = -_moveVel;

		pEnt->getMessage(new MRigidbodyMoveX(value, pEnt->getID()));
	}

	if (m->getType() == MSG_MOD_VEL){
		float mModVelValue = static_cast<MModVel*>(m)->getValue();
		_moveVel = _moveVel + (_moveVel * mModVelValue / 100.0f);
		if (_moveVel > _maxSpeed){
			_moveVel = _maxSpeed;
		}
	}

	if (m->getType() == MSG_MOD_VEL_JUMP){
		float mModVelValue = static_cast<MModVelAndJump*>(m)->getVelValue();
		_moveVel = _moveVel + (_moveVel * mModVelValue / 100.0f);
		if (_moveVel > _maxSpeed){
			_moveVel = _maxSpeed;
		}
	}

	if (m->getType() == MSG_PASSMOD_DES){
		resetVel();
	}

	if (m->getType() == MSG_ACTIVEMOD_RES){
		resetVel();
	}

}
#pragma endregion

//Jump Component
#pragma region Player Jump Component

CPlayerJump::CPlayerJump(Entity* father, float startForce) :GameComponent(CMP_JUMP, father), _maxForce(MAX_JUMP_DISTANCE), _jumpForce(startForce), _maxJumps(2), _nJumps(0), _auxJumpReset(startForce)
{
	_timeCounter = 0.0f;
	_lastTimeJump = 0.0f;
	_jumpRate = 170.0f;

}

CPlayerJump::~CPlayerJump(){}

void CPlayerJump::tick(float delta){}
void CPlayerJump::getMessage(Message* m)
{

	MModVelAndJump* mModVelJump;

	switch (m->getType()){
	case MSG_PLAYER_JUMP:
		_timeCounter = SDL_GetTicks();
		if (_nJumps < _maxJumps && ((_timeCounter - _lastTimeJump) > _jumpRate)){
			//std::cout << _nJumps << std::endl;
			_nJumps++;
			pEnt->getMessage(new MRigidbodyJump(_jumpForce, pEnt->getID()));
			_lastTimeJump = SDL_GetTicks();
		}
		break;
	case MSG_COLLISION_TERRAIN:
		_nJumps = 0;
		break;

	case MSG_MOD_VEL_JUMP:
		mModVelJump = static_cast<MModVelAndJump*>(m);
		_jumpForce = _jumpForce + (_jumpForce * mModVelJump->getJumpValue() / 100.0f);
		if (_jumpForce > _maxForce){
			_jumpForce = _maxForce;
		}
		break;

	case MSG_PASSMOD_DES:
		resetForceJump();
		break;

	default:
		break;

	}
}
#pragma endregion

//Basic Attack Component
#pragma region Player Basic Attack Component

CPlayerBasicAttack::CPlayerBasicAttack(Entity* father, float fireRate, E_BULLET bT, Ogre::Vector3 entPos, float damage) :GameComponent(CMP_BASIC_ATTACK, father),
_maxFireRate(MAX_FIRE_RATE), _fireRate(fireRate), _bulletType(bT), _ogrepos(entPos), _damage(damage), _auxDamageReset(damage), _auxFireRateReset(fireRate)
{
	_lastTimeShot = 0;
	_timeCounter = 0;
	_radius = 3.0f;

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
			if (mPS->getXValue() != 0 || mPS->getYValue() != 0){
				calculateSpawnPoint(mPS->getXValue(), mPS->getYValue(), angle, iniPos);
				Ogre::Vector3 dir = iniPos;
				iniPos.x += _ogrepos.x;
				iniPos.y += _ogrepos.y;
				iniPos.z = _ogrepos.z;

				Entity* b = EntityFactory::getInstance().createBullet(_bulletType, pEnt->getScene(), iniPos, angle, _damage, pEnt->getID());
				pEnt->getMessage(new MAddEntity(pEnt->getID(), b));
				pEnt->getMessage(new MShot(dir.x, dir.y, pEnt->getID()));
				b->getMessage(new MShot(dir.x, dir.y, pEnt->getID()));

			}

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
			_ogrepos.x = parentPos.x + w / 2;
			_ogrepos.y = parentPos.y + h / 2;
			_ogrepos.z = parentPos.z;

			//Rotate the parent node the same degree as the collider.
			float angleRad = msg->getRotation();
			float grades = (angleRad * 180) / 3.14159265359f;
		}
	}


	else if (m->getType() == MSG_MOD_DMG){
		float dmgValue = static_cast<MModDmg*>(m)->getValue();
		_damage = _damage + (_damage* dmgValue / 100.0f);

#ifdef _DEBUG
		//std::cout << _damage << "\n";

#endif // DEBUG


	}

	else if (m->getType() == MSG_MOD_FIRERATE){
		float fireRateValue = static_cast<MModFireRate*>(m)->getFireRateValue();
		_fireRate = _fireRate - (_fireRate*fireRateValue / 100.0f);
		if (_fireRate < _maxFireRate){
			_fireRate = _maxFireRate;
		}

	}
	else if (m->getType() == MSG_PASSMOD_DES){
		resetDamage();
		resetFireRate();
	}

	else if (m->getType() == MSG_ACTIVEMOD_RES){
		resetDamage();
		resetFireRate();
	}
}


void CPlayerBasicAttack::calculateSpawnPoint(float vX, float vY, float &angle, Ogre::Vector3 &iniPos){

	//Calculate point
	/*5 - 327*/
	//Normalize
	if (vX == 0){
		iniPos.x = 0;
		iniPos.z = 0;
		iniPos.y = _radius;
		angle = 0.0f;

		if (vY < 0){
			iniPos.y = -_radius;
			angle = 180.0f;
		}


	}
	else if (vY == 0){

		iniPos.x = _radius;
		iniPos.y = 0;
		iniPos.z = 0;
		angle = -90;

		if (vX < 0){
			iniPos.x = -_radius;
			angle = 90;
		}
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
		Ogre::Vector3 idleVector(_radius, 0.0f, 0.0f);

		float escalarProduct = (iniPos.x * idleVector.x) + (iniPos.y * idleVector.y);
		float lengthIni = std::sqrt(std::pow(iniPos.x, 2) + std::pow(iniPos.y, 2));
		float lengthIdle = _radius;

		float cos = escalarProduct / (lengthIdle * lengthIni);

		if (cos > 1)
			cos = 1;
		else if (cos < -1)
			cos = -1;

		angle = std::acosf(cos);
		angle = ((angle * 180.0f) / 3.14159265359f) - 90.0f;


		if (yFinal < 0.0f){
			if (xFinal < 0.0f){
				float comp = 90.0f - angle;
				angle += comp * 2;
			}
			else{
				float comp = 90.0f + angle;
				comp = -comp;
				angle += comp * 2;
			}
		}
	}
}

#pragma endregion

//Bullet Component
#pragma region Bullet Component

CBullet::CBullet(Entity* father, E_BULLET bT, float damage, float vel) :GameComponent(CMP_BASIC_ATTACK, father), _damage(damage)
, _velocity(vel), _toDelete(false), _auxVelocityReset(vel)
{
	s = nullptr;

}
CBullet::~CBullet(){}

void CBullet::tick(float delta){}
void CBullet::getMessage(Message* m){

	//posible error de memoria dinamica


	MShot* mShot;
	MCollisionBegin* mCollision;

	float xDir;
	float yDir;

	float velBullets;

	switch (m->getType()){

	case MSG_SHOT:
		mShot = static_cast<MShot*>(m);

		xDir = (mShot->getXValue() / 200.0f) * _velocity;
		yDir = (mShot->getYValue() / 200.0f) * _velocity;

		pEnt->getMessage(new MRigidbodyMoveX(xDir, pEnt->getID()));
		pEnt->getMessage(new MRigidbodyMoveY(yDir, pEnt->getID()));



		break;

	case MSG_COLLISION:
		if (!_toDelete){
			mCollision = static_cast<MCollisionBegin*>(m);
			mCollision->GetWho()->getMessage(new MBulletHit(_damage, mCollision->GetContactMask(), pEnt->getID()));

			pEnt->getScene()->addEntityToDelete(pEnt);

			_toDelete = true;
		}
		break;

	case MSG_MOD_VELBULLETS:
		velBullets = static_cast<MModVelBullets*>(m)->getVelBulletsValue();
		_velocity = _velocity + (_velocity * velBullets / 100.0f);
		//a�adir un maximo de vel de las balas si queremos
		break;

	case MSG_PASSMOD_DES:
		resetVelocity();
		break;

	default:
		break;
	}

}
#pragma endregion

#pragma region RigidBodyComponent
//Rigid Body component.
//Gives an entity a rigid body to simulate physics
CRigidBody::CRigidBody(Entity * father, b2World * world, Ogre::Vector3 posInPixels, float heightInPixels, float weightInPixels, float angle, RigidBodyType rbType, ShapeType shType, FilterMask myCategory, int controllerId)
	: _rbHeight(heightInPixels / PPM), _rbWeight(weightInPixels / PPM), _myWorld(world), GameComponent(CMP_PHYSICS, father) {

	//soundW = Game::getInstance()->getSoundEngine()->addSoundSourceFromFile("../Media/sounds/Movement/paso1.wav");

	//soundJ = Game::getInstance()->getSoundEngine()->addSoundSourceFromFile("../Media/sounds/Movement/SaltoAire.wav");
	//Sets the pos attached to the render.
	_pos.x = posInPixels.x / PPM;
	_pos.y = posInPixels.y / PPM;

	//Initial velocity 0.
	_vel.x = _vel.y = 0;

	//Body definition.
	_bodyDef.position.Set(_pos.x, _pos.y);



	if (myCategory == MASK_PLAYER)
		_bodyDef.fixedRotation = true;
	if (myCategory == MASK_BULLET_0 || myCategory == MASK_BULLET_1){
		_bodyDef.bullet = true;
		//_bodyDef.fixedRotation = true;
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

	float radians = (3.14159265359f * angle) / 180.0f;

	_body->SetTransform(_pos, radians);

	//Set the body data pointer to entity
	_body->SetUserData(pEnt);

	//Fixture Definition.
	_fixtureDef.density = 9.0f / 3.0f;
	//_fixtureDef.restitution = 0.0f;
	_fixtureDef.friction = 0.0f;

	switch (myCategory)
	{
	case MASK_STATIC_TERRAIN:
		_fixtureDef.filter.categoryBits = MASK_STATIC_TERRAIN;
		_fixtureDef.filter.maskBits = MASK_BULLET_0 | MASK_BULLET_1 | MASK_DINAMIC_TERRAIN | MASK_LEGS_0 | MASK_LEGS_1 | MASK_HEAD_1 | MASK_HEAD_0 | MASK_CHEST_0 | MASK_CHEST_1 | MASK_FOOT_SENSOR;
		break;
	case MASK_DINAMIC_TERRAIN:
		_fixtureDef.filter.categoryBits = MASK_DINAMIC_TERRAIN;
		_fixtureDef.filter.maskBits = MASK_BULLET_0 | MASK_BULLET_1 | MASK_STATIC_TERRAIN | MASK_LEGS_0 | MASK_LEGS_1 | MASK_HEAD_1 | MASK_HEAD_0 | MASK_CHEST_0 | MASK_CHEST_1 | MASK_FOOT_SENSOR;
		break;
	case MASK_BULLET_0:
		_fixtureDef.filter.categoryBits = MASK_BULLET_0;
		_fixtureDef.filter.maskBits = MASK_LEGS_1 | MASK_CHEST_1 | MASK_HEAD_1 | MASK_DINAMIC_TERRAIN | MASK_STATIC_TERRAIN | MASK_DEATHZONE;
		break;
	case MASK_BULLET_1:
		_fixtureDef.filter.categoryBits = MASK_BULLET_1;
		_fixtureDef.filter.maskBits = MASK_LEGS_0 | MASK_CHEST_0 | MASK_HEAD_0 | MASK_DINAMIC_TERRAIN | MASK_STATIC_TERRAIN | MASK_DEATHZONE;
		break;
	case MASK_DEATHZONE:
		_fixtureDef.filter.categoryBits = MASK_DEATHZONE;
		_fixtureDef.filter.maskBits = MASK_LEGS_0 | MASK_CHEST_0 | MASK_HEAD_0 | MASK_BULLET_0 | MASK_LEGS_1 | MASK_CHEST_1 | MASK_HEAD_1 | MASK_BULLET_1;
		break;
	default:
		break;
	}

	float playerSize = _rbHeight / 3;
	float legsSize = _rbWeight / 1.5f;

	b2PolygonShape _pShape;

	b2CircleShape _cShape;

	//Shape creation.
	switch (shType)
	{
	case SH_CIRCLE:
		_cShape.m_p.Set(0, 0);
		_cShape.m_radius = _rbWeight;
		_fixtureDef.shape = &_cShape;
		//Fixture creation.
		_fixture = _body->CreateFixture(&_fixtureDef);
		break;
	case SH_POLYGON:

		_pShape.SetAsBox(_rbWeight / 2, _rbHeight / 2, { _rbWeight / 2, _rbHeight / 2 }, 0);
		_fixtureDef.shape = &_pShape;

		//Fixture creation.
		_fixture = _body->CreateFixture(&_fixtureDef);
		break;
	case SH_PLAYER:

		_pShape.SetAsBox(_rbWeight / 2, playerSize / 2, { _rbWeight / 2, _rbHeight / 2 }, 0);
		_fixtureDef.shape = &_pShape;

		if (controllerId == 1){
			_fixtureDef.filter.categoryBits = MASK_CHEST_1;
			_fixtureDef.filter.maskBits = MASK_STATIC_TERRAIN | MASK_DEATHZONE | MASK_DINAMIC_TERRAIN | MASK_CHEST_0 | MASK_HEAD_0 | MASK_LEGS_0 | MASK_BULLET_0;

		}
		else{

			_fixtureDef.filter.categoryBits = MASK_CHEST_0;
			_fixtureDef.filter.maskBits = MASK_STATIC_TERRAIN | MASK_DEATHZONE | MASK_DINAMIC_TERRAIN | MASK_HEAD_1 | MASK_CHEST_1 | MASK_LEGS_1 | MASK_BULLET_1;

		}
		_body->CreateFixture(&_fixtureDef);

		_pShape.SetAsBox(_rbWeight / 2, playerSize / 2, { _rbWeight / 2, _rbHeight / 2 + playerSize }, 0);
		_fixtureDef.shape = &_pShape;
		if (controllerId == 1){
			_fixtureDef.filter.categoryBits = MASK_HEAD_1;
			_fixtureDef.filter.maskBits = MASK_STATIC_TERRAIN | MASK_DEATHZONE | MASK_DINAMIC_TERRAIN | MASK_CHEST_0 | MASK_HEAD_0 | MASK_LEGS_0 | MASK_BULLET_0;

		}
		else{

			_fixtureDef.filter.categoryBits = MASK_HEAD_0;
			_fixtureDef.filter.maskBits = MASK_STATIC_TERRAIN | MASK_DEATHZONE | MASK_DINAMIC_TERRAIN | MASK_HEAD_1 | MASK_CHEST_1 | MASK_LEGS_1 | MASK_BULLET_1;

		}
		_body->CreateFixture(&_fixtureDef);

		_pShape.SetAsBox(_rbWeight / 2, playerSize / 2, { _rbWeight / 2, _rbHeight / 2 - playerSize }, 0);
		_fixtureDef.shape = &_pShape;
		if (controllerId == 1){
			_fixtureDef.filter.categoryBits = MASK_LEGS_1;
			_fixtureDef.filter.maskBits = MASK_STATIC_TERRAIN | MASK_DEATHZONE | MASK_DINAMIC_TERRAIN | MASK_CHEST_0 | MASK_HEAD_0 | MASK_LEGS_0 | MASK_BULLET_0;

		}
		else{

			_fixtureDef.filter.categoryBits = MASK_LEGS_0;
			_fixtureDef.filter.maskBits = MASK_STATIC_TERRAIN | MASK_DEATHZONE | MASK_DINAMIC_TERRAIN | MASK_HEAD_1 | MASK_CHEST_1 | MASK_LEGS_1 | MASK_BULLET_1;

		}
		_fixture = _body->CreateFixture(&_fixtureDef);

		//Foot sensor
		_pShape.SetAsBox(legsSize / 2, playerSize / 2, { _rbWeight / 2, 0 }, 0);
		//_fixtureDef.isSensor = true;
		_fixtureDef.shape = &_pShape;
		_fixtureDef.density = 1.0f;
		_fixtureDef.filter.categoryBits = MASK_FOOT_SENSOR;
		_fixtureDef.filter.maskBits = MASK_DINAMIC_TERRAIN | MASK_STATIC_TERRAIN;
		_fixture = _body->CreateFixture(&_fixtureDef);
		_fixture->SetSensor(true);
		break;
	default:
		break;
	}





}
CRigidBody::~CRigidBody() {
	pEnt->getScene()->addBodyToDelete(_body);

	if (_updateTransform != nullptr){
		delete _updateTransform;
		_updateTransform = nullptr;
	}
}
void CRigidBody::tick(float delta) {



	//Send the message to the entity.
	//Transformation from physics world to ogre world.

	if (_body->IsFixedRotation()){
		_updateTransform = new MUpdateTransform(Ogre::Vector3((_body->GetPosition().x)* PPM, _body->GetPosition().y * PPM, 0), -1, _rbHeight * PPM, _rbWeight * PPM, pEnt->getID());
		pEnt->getMessage(_updateTransform);
	}
	else{

		_updateTransform = new MUpdateTransform(Ogre::Vector3((_body->GetPosition().x)* PPM, _body->GetPosition().y * PPM, 0), _body->GetAngle(), _rbHeight * PPM, _rbWeight * PPM, pEnt->getID());
		pEnt->getMessage(_updateTransform);

	}

	//if (pEnt->getID() == "Player_0";
	//std::cout << _body->GetPosition().x * PPM << std::endl;


}

void CRigidBody::getMessage(Message * m) {


	MRigidbodyMoveX* mMoveX;
	float velX;
	MRigidbodyMoveY* mMoveY;
	float velY;
	MRigidbodyJump* mJump;
	float jForce;
	MDash* mDash;
	bool playingW = false;
	irrklang::ISound* s = nullptr;

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



		Game::getInstance()->getSoundEngine()->play2D("../Media/sounds/Movement/SaltoAire.wav");



		//std::cout << jForce << std::endl;
		_body->ApplyLinearImpulseToCenter(b2Vec2(0, jForce), true);
		break;
	case MSG_DASH:
		mDash = static_cast<MDash*>(m);
		_body->ApplyLinearImpulseToCenter(b2Vec2(mDash->getDashValue()->x, mDash->getDashValue()->y), true);
		break;
	default:
		break;
	}
}
#pragma endregion
