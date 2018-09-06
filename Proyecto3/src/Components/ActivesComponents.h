#ifndef ACTIVESCOMPONENTS_H
#define ACTIVESCOMPONENTS_H



#include "AbilityComponent.h"

class Entity;

GameComponent* createActiveAbilityEmpty(Entity* father, int id);
class CASkillEmpty : public CAbility
{
public:
	CASkillEmpty(Entity * father);
	~CASkillEmpty();

	virtual void tick(float delta);
	virtual void getMessage(Message * m);


};
//Shu Headdress, dash
GameComponent* createAbilityShuHeaddress(Entity* father, int id);
class CShuHeaddress : public CAbility
{
public:
	CShuHeaddress(Entity * father, int id);
	~CShuHeaddress();

	virtual void tick(float delta);
	virtual void getMessage(Message * m);

private:

	b2Vec2* calculateDash(float xValue, float yValue);

	int _playerId;
	float _timeCounter;
	float _lastTimeDash;
	float _dashImpulse;





};

//Jonsu Moon, give mov speed during 5s
GameComponent* createAbilityJonsuMoon(Entity* father, int id);
class CJonsuMoon : public CAbility
{
public:
	CJonsuMoon(Entity * father, int id);
	~CJonsuMoon();

	virtual void tick(float delta);
	virtual void getMessage(Message * m);

private:

	int _playerId;
	float _timeCounter;
	float _initTime;
	float _timeActiveLimit;
	float _velocityPercentage;
	bool _isActive;
	bool isAvailable;

};

//Khepri Beetle, increase fireRate during 3s
GameComponent* createAbilityKhepriBeetle(Entity* father, int id);
class CKhepriBeetle : public CAbility
{
public:
	CKhepriBeetle(Entity * father, int id);
	~CKhepriBeetle();

	virtual void tick(float delta);
	virtual void getMessage(Message * m);

private:

	int _playerId;
	float _timeCounter;
	float _initTime;
	float _timeActiveLimit;
	float _fireRatePercentage;
	bool _isActive;
	bool isAvailable;
	MUpdateActiveTimer * _updateTimer;

};

//Hera´s Rune, Restore life of Cards
GameComponent* createAbilityHeraRune(Entity* father, int id);
class CHeraRune : public CAbility
{
public:
	CHeraRune(Entity * father, int id);
	~CHeraRune();

	virtual void tick(float delta);
	virtual void getMessage(Message * m);

private:

	int _playerId;
	float _timeCounter;
	float _initTime;
	bool isAvailable;


};

// Heris' Mark, plus 20% damage on next 10 attacks
GameComponent* createAbilityHerisMark(Entity* father, int id);
class CHerisMark : public CAbility
{
public:
	CHerisMark(Entity * father, int id);
	~CHerisMark();

	virtual void tick(float delta);
	virtual void getMessage(Message * m);

private:

	int _playerId;
	float _timeCounter;
	float _initTime;
	float _timeActiveLimit;
	bool _isActive;
	bool isAvailable;
	int _availableShots;
	bool _maxShots;

};



#endif