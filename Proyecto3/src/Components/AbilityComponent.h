#ifndef ABILITYCOMPONENT_H
#define ABILITYCOMPONENT_H

#include "Components.h"

class Entity;

class CAbility : public GameComponent
{
public:
	CAbility(ComponentType c, Entity * father, float componentLife, float componentArmor, uint16 mask, int type);
	~CAbility();

	virtual void getMessage(Message* m);

	inline int getType(){ return _type; }

protected:
	float _componentLife;
	float _componentArmor;
	float _coolDown;
	uint16 _myMask;
	float _limitLife;
	int _type;		//0 = Active ; 1 = Passive
	bool dead;



};

#endif