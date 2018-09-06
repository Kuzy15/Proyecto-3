#include "ActivesComponents.h"


#include "Entity.h"
#include "Game.h"


GameComponent* createActiveAbilityEmpty(Entity* father, int id){ return new CASkillEmpty(father); }
CASkillEmpty::CASkillEmpty(Entity * father) :CAbility(CMP_ACTIVE_DEFAULT, father, 0, 0, MASK_HEAD_0, 1){

}
CASkillEmpty::~CASkillEmpty(){}

void CASkillEmpty::tick(float delta){}
void CASkillEmpty::getMessage(Message* m){

}


#pragma region Shu Headdress
//Dash
GameComponent* createAbilityShuHeaddress(Entity* father, int id){ return new CShuHeaddress(father, id); }
CShuHeaddress::CShuHeaddress(Entity * father, int id) :CAbility(CMP_SHU_HEADDRESS, father, 100, 100, MASK_HEAD_0, 0), _playerId(id){
	_timeCounter = _lastTimeDash = 0;
	_coolDown = 2000.0f; //5 seconds
	_dashImpulse = 1000.0f;
	_lastTimeDash = SDL_GetTicks();

}
CShuHeaddress::~CShuHeaddress(){


}

void CShuHeaddress::tick(float delta){
	float val = (SDL_GetTicks() - _lastTimeDash) * 100 / _coolDown;
	if (val > 100)val = 100;

	pEnt->getMessage(new MUpdateActiveTimer(pEnt->getID(), val));


}
void CShuHeaddress::getMessage(Message* m)
{

	if (m->getType() == MSG_INPUT_STATE){
		MInputState* inputM = static_cast<MInputState*>(m);
		if (inputM->getId() == _playerId){
			_timeCounter = SDL_GetTicks();
			ControllerInputState cState = inputM->getCInputState();

			if (cState.Left_Shoulder == BTT_RELEASED && (_timeCounter - _lastTimeDash) > _coolDown){

				Game::getInstance()->getSoundEngine()->play2D("../Media/sounds/FightScene/ActiveUse.wav");
				b2Vec2 *impulse = calculateDash(cState.Axis_LeftX, 0);
				pEnt->getMessage(new MDash(pEnt->getID(), impulse));
				_lastTimeDash = SDL_GetTicks();
			}
		}
	}
	CAbility::getMessage(m);
}

const float SPAWN_PARSE = 1.0f / 328.0f;
b2Vec2* CShuHeaddress::calculateDash(float xValue, float yValue){

	float normalX = xValue * SPAWN_PARSE;
	float normalY = yValue * SPAWN_PARSE;

	return new b2Vec2(_dashImpulse * normalX, _dashImpulse * normalY);

}
#pragma endregion

#pragma region Jonsu Moon
//Velocity improvement
GameComponent* createAbilityJonsuMoon(Entity* father, int id){ return new CJonsuMoon(father, id); }

CJonsuMoon::CJonsuMoon(Entity * father, int id) :CAbility(CMP_JONSU_MOON, father, 100, 100, MASK_HEAD_0, 0), _playerId(id){
	_timeCounter = _initTime = 0;
	_timeActiveLimit = 5000.0f; //5 seconds
	_coolDown = 10000.0f;
	_velocityPercentage = 50.0f;
	_isActive = false;
	isAvailable = true;
}
CJonsuMoon::~CJonsuMoon(){}

void CJonsuMoon::tick(float delta){

	//Whe is active and timeActiveLimit completes, deactivate it and start cooldown
	if (_isActive){
		_timeCounter = SDL_GetTicks();
		if ((_timeCounter - _initTime) >= _timeActiveLimit){
			pEnt->getMessage(new MReset(pEnt->getID()));
			_isActive = false;
			_initTime = SDL_GetTicks();
		}
	}
	//If is not active and is not available, we count the cooldown. Then turn it to available.
	else if (!isAvailable){
		_timeCounter = SDL_GetTicks();
		if ((_timeCounter - _initTime) >= _coolDown){
			isAvailable = true;
		}
		float  val = (SDL_GetTicks() - _initTime) * 100 / _coolDown;
		if (val > 100)val = 100;
		pEnt->getMessage(new MUpdateActiveTimer(pEnt->getID(), val));

	}

}
void CJonsuMoon::getMessage(Message* m)
{
	if (m->getType() == MSG_INPUT_STATE){
		MInputState* inputM = static_cast<MInputState*>(m);
		if (inputM->getId() == _playerId && isAvailable){
			ControllerInputState cState = inputM->getCInputState();
			if (cState.Left_Shoulder == BTT_RELEASED){
				pEnt->getMessage(new MModVel(pEnt->getID(), _velocityPercentage));
				_initTime = SDL_GetTicks();
				_isActive = true;
				isAvailable = false;

				Game::getInstance()->getSoundEngine()->play2D("../Media/sounds/FightScene/ActiveUse.wav");
				pEnt->getMessage(new MUpdateActiveTimer(pEnt->getID(), 0));
			}
		}
	}
	CAbility::getMessage(m);
}
#pragma endregion

#pragma region Khepri Beetle
//Velocity improvement
GameComponent* createAbilityKhepriBeetle(Entity* father, int id){ return new CKhepriBeetle(father, id); }
CKhepriBeetle::CKhepriBeetle(Entity * father, int id) :CAbility(CMP_KHEPRI_BEETLE, father, 100, 100, MASK_HEAD_0, 0), _playerId(id){
	_timeCounter = _initTime = 0;
	_timeActiveLimit = 3000.0f; //3 seconds
	_coolDown = 10000.0f;
	_fireRatePercentage = 40.0f;
	_isActive = false;
	isAvailable = true;
}
CKhepriBeetle::~CKhepriBeetle(){}

void CKhepriBeetle::tick(float delta){

	//Whe is active and timeActiveLimit completes, deactivate it and start cooldown
	if (_isActive){
		_timeCounter = SDL_GetTicks();
		if ((_timeCounter - _initTime) >= _timeActiveLimit){
			pEnt->getMessage(new MReset(pEnt->getID()));
			_isActive = false;
			_initTime = SDL_GetTicks();
		}
	}
	//If is not active and is not available, we count the cooldown. Then turn it to available.
	else if (!isAvailable){

		_timeCounter = SDL_GetTicks();
		if ((_timeCounter - _initTime) >= _coolDown){
			isAvailable = true;
		}
		float val = (SDL_GetTicks() - _initTime) * 100 / _coolDown;
		if (val > 100)val = 100;
		pEnt->getMessage(new MUpdateActiveTimer(pEnt->getID(), val));

	}


}
void CKhepriBeetle::getMessage(Message* m)
{
	if (m->getType() == MSG_INPUT_STATE){
		MInputState* inputM = static_cast<MInputState*>(m);
		if (inputM->getId() == _playerId && isAvailable){
			ControllerInputState cState = inputM->getCInputState();
			if (cState.Left_Shoulder == BTT_RELEASED){
				pEnt->getMessage(new MModFireRate(pEnt->getID(), _fireRatePercentage));
				_initTime = SDL_GetTicks();
				_isActive = true;
				isAvailable = false;

				Game::getInstance()->getSoundEngine()->play2D("../Media/sounds/FightScene/ActiveUse.wav");
				pEnt->getMessage(new MUpdateActiveTimer(pEnt->getID(), 0));
			}
		}
	}
	CAbility::getMessage(m);
}
#pragma endregion


#pragma region Hera�s Rune
//Velocity improvement

GameComponent* createAbilityHeraRune(Entity* father, int id){ return new CHeraRune(father, id); }
CHeraRune::CHeraRune(Entity * father, int id) :CAbility(CMP_HERA_RUNE, father, 50, 100, MASK_HEAD_0, 0), _playerId(id){
	_timeCounter = _initTime = 0;
	_coolDown = 10000.0f;
	isAvailable = true;
}
CHeraRune::~CHeraRune() {}

void CHeraRune::tick(float delta) {

	if (!isAvailable) {
		_timeCounter = SDL_GetTicks();
		if ((_timeCounter - _initTime) >= _coolDown) {
			isAvailable = true;
		}

		float val = (SDL_GetTicks() - _initTime) * 100 / _coolDown;
		if (val > 100)val = 100;
		pEnt->getMessage(new MUpdateActiveTimer(pEnt->getID(), val));

	}

}

void CHeraRune::getMessage(Message* m)
{
	if (m->getType() == MSG_INPUT_STATE) {
		MInputState* inputM = static_cast<MInputState*>(m);
		if (inputM->getId() == _playerId && isAvailable) {
			ControllerInputState cState = inputM->getCInputState();
			if (cState.Left_Shoulder == BTT_RELEASED) {
				pEnt->getMessage(new MRestoreLifeCards(pEnt->getID()));
				_initTime = SDL_GetTicks();
				isAvailable = false;

				Game::getInstance()->getSoundEngine()->play2D("../Media/sounds/FightScene/ActiveUse.wav");

				pEnt->getMessage(new MUpdateActiveTimer(pEnt->getID(), 0));
			}
		}
	}
	if (m->getType() == MSG_RESTORE_LIFE_CARDS) {
		_componentLife += (_componentLife / 2);
		if (_componentLife > _limitLife) {
			_componentLife = _limitLife;
		}
	}
	CAbility::getMessage(m);
}
#pragma endregion


#pragma region Heris Mark

GameComponent* createAbilityHerisMark(Entity* father, int id){ return new CHerisMark(father, id); }


CHerisMark::CHerisMark(Entity * father, int id) :CAbility(CMP_HERIS_MARK, father, 50, 100, MASK_HEAD_0, 0), _playerId(id){
	_timeCounter = _initTime = 0;
	_coolDown = 10000.0f;
	_timeActiveLimit = 30000.0f;
	isAvailable = true;
	_isActive = false;
	_availableShots = 10;
	_maxShots = false;
}
CHerisMark::~CHerisMark() {}

void CHerisMark::tick(float delta) {


	//When is active and timeActiveLimit completes, deactivate it and start cooldown
	if (_isActive) {
		_timeCounter = SDL_GetTicks();
		if ((_timeCounter - _initTime) >= _timeActiveLimit) {
			pEnt->getMessage(new MReset(pEnt->getID())); // Mensage modificar da�o -20%
			_availableShots = 10;
			_isActive = false;
			_initTime = SDL_GetTicks();
		}
	}
	//If is not active and is not available, we count the cooldown. Then turn it to available.
	else if (!isAvailable) {

		_timeCounter = SDL_GetTicks();
		if ((_timeCounter - _initTime) >= _coolDown) {
			isAvailable = true;
		}

		float val = (SDL_GetTicks() - _initTime) * 100 / _coolDown;
		if (val > 100)val = 100;
		pEnt->getMessage(new MUpdateActiveTimer(pEnt->getID(), val));

	}

	// Maximum number of shots has been reached so deactivate the effect and start cooldown.
	if (_maxShots) {
		_isActive = false;
		_availableShots = 10;
		_maxShots = false;
		pEnt->getMessage(new MReset(pEnt->getID()));
		_initTime = SDL_GetTicks();
	}





}
void CHerisMark::getMessage(Message* m)
{
	if (m->getType() == MSG_INPUT_STATE) {
		MInputState* inputM = static_cast<MInputState*>(m);
		if (inputM->getId() == _playerId && isAvailable) {
			ControllerInputState cState = inputM->getCInputState();

			if (cState.Left_Shoulder == BTT_PRESSED) {
				pEnt->getMessage(new MModDmg(pEnt->getID(), 20.0f)); // Mensage modificar da�o +20%
				_initTime = SDL_GetTicks();
				_isActive = true;
				isAvailable = false;

				Game::getInstance()->getSoundEngine()->play2D("../Media/sounds/FightScene/ActiveUse.wav");

				pEnt->getMessage(new MUpdateActiveTimer(pEnt->getID(), 0));
			}
		}
	}

	// Check if a shot has been made
	else if (_isActive && m->getType() == MSG_SHOT) {
		_availableShots--;
		//std::cout << _availableShots << "\n";
		if (_availableShots == 0) {
			_maxShots = true;
		}

	}
	CAbility::getMessage(m);

}
#pragma endregion



