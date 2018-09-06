#ifndef PASSIVESCOMPONENTS_H
#define PASSIVESCOMPONENTS_H

#include "AbilityComponent.h"

class Entity;


GameComponent* createPassiveAbilityEmpty(Entity* father, int id);
class CPSkillEmpty : public CAbility
{
public:
	CPSkillEmpty(Entity * father);
	~CPSkillEmpty();

	virtual void tick(float delta);
	virtual void getMessage(Message * m);


};


//invisibility
GameComponent* createAbilityVidar(Entity* father, int id);
class CPSkillVidar : public CAbility
{
public:
	CPSkillVidar(Entity * father);
	~CPSkillVidar();

	virtual void tick(float delta);
	virtual void getMessage(Message * m);

};


//Increase damage of a god
GameComponent* createAbilityHades(Entity* father, int id);
class CPSkillHades : public CAbility
{
public:
	CPSkillHades(Entity * father);
	~CPSkillHades();

	virtual void tick(float delta);
	virtual void getMessage(Message * m);


};



//Modify velocity of a god
GameComponent* createAbilityUll(Entity* father, int id);
class CPSkillUll : public CAbility
{
public:
	CPSkillUll(Entity * father);
	~CPSkillUll();

	virtual void tick(float delta);
	virtual void getMessage(Message * m);


};

//modify vel of bullets
GameComponent* createAbilityVali(Entity* father, int id);
class CPSkillVali : public CAbility
{
public:
	CPSkillVali(Entity * father);
	~CPSkillVali();

	virtual void tick(float delta);
	virtual void getMessage(Message * m);


};


//Modify velocity and jump of a god
GameComponent* createAbilityHermes(Entity* father, int id);
class CPSkillHermes : public CAbility
{
public:
	CPSkillHermes(Entity * father);
	~CPSkillHermes();

	virtual void tick(float delta);
	virtual void getMessage(Message * m);


};


//Modify vel of fire rate
GameComponent* createAbilitySyn(Entity* father, int id);
class CPSkillSyn : public CAbility
{
public:
	CPSkillSyn(Entity * father);
	~CPSkillSyn();

	virtual void tick(float delta);
	virtual void getMessage(Message * m);


};

#endif