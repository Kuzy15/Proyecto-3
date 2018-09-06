#include "PassivesComponents.h"

#include "Entity.h"

//Passive Skill Component
#pragma region CPSkill Component

GameComponent* createPassiveAbilityEmpty(Entity* father, int id){ return new CPSkillEmpty(father); }
CPSkillEmpty::CPSkillEmpty(Entity * father) :CAbility(CMP_PASSIVE_DEFAULT, father, 0, 0, MASK_LEGS_0, 1){

}
CPSkillEmpty::~CPSkillEmpty(){}

void CPSkillEmpty::tick(float delta){}
void CPSkillEmpty::getMessage(Message* m){

}




///invisibility
GameComponent* createAbilityVidar(Entity* father, int id){ return new CPSkillVidar(father); }
CPSkillVidar::CPSkillVidar(Entity * father) :CAbility(CMP_PASSIVE_VIDAR, father, 25, 25, MASK_LEGS_0, 1){
	pEnt->getMessage(new MModInvisibility(pEnt->getID()));


}
CPSkillVidar::~CPSkillVidar(){


}

void CPSkillVidar::tick(float delta){}
void CPSkillVidar::getMessage(Message* m){
	if (m->getType() == MSG_RESTORE_LIFE_CARDS){
		_componentLife += (_componentLife / 2);
	}
	CAbility::getMessage(m);
}


///modify dmg of a god
GameComponent* createAbilityHades(Entity* father, int id){ return new CPSkillHades(father); }
CPSkillHades::CPSkillHades(Entity * father) :CAbility(CMP_PASSIVE_HADES, father, 25, 100, MASK_LEGS_0, 1){
	pEnt->getMessage(new MModDmg(pEnt->getID(), 10.0f));
}
CPSkillHades::~CPSkillHades(){}

void CPSkillHades::tick(float delta){}
void CPSkillHades::getMessage(Message* m){
	if (m->getType() == MSG_RESTORE_LIFE_CARDS){
		_componentLife += (_componentLife / 2);
		if (_componentLife > _limitLife){
			_componentLife = _limitLife;
		}
	}
	CAbility::getMessage(m);
}




///modify velocity of a god
GameComponent* createAbilityUll(Entity* father, int id){ return new CPSkillUll(father); }
CPSkillUll::CPSkillUll(Entity * father) :CAbility(CMP_PASSIVE_ULL, father, 100, 100, MASK_LEGS_0, 1){
	pEnt->getMessage(new MModVel(pEnt->getID(), -20.0f));
}
CPSkillUll::~CPSkillUll(){}

void CPSkillUll::tick(float delta){}
void CPSkillUll::getMessage(Message* m){
	if (m->getType() == MSG_RESTORE_LIFE_CARDS){
		_componentLife += (_componentLife / 2);
		if (_componentLife > _limitLife){
			_componentLife = _limitLife;
		}
	}
	CAbility::getMessage(m);
}


///modify vel of bullets
GameComponent* createAbilityVali(Entity* father, int id){ return new CPSkillVali(father); }
CPSkillVali::CPSkillVali(Entity * father) :CAbility(CMP_PASSIVE_VALI, father, 50, 75, MASK_LEGS_0, 1){
	pEnt->getMessage(new MModVelBullets(pEnt->getID(), 10));
}
CPSkillVali::~CPSkillVali(){}

void CPSkillVali::tick(float delta){}
void CPSkillVali::getMessage(Message* m){
	if (m->getType() == MSG_RESTORE_LIFE_CARDS){
		_componentLife += (_componentLife / 2);
		if (_componentLife > _limitLife){
			_componentLife = _limitLife;
		}
	}
	CAbility::getMessage(m);
}


///modify velocity and jump of a god
GameComponent* createAbilityHermes(Entity* father, int id){ return new CPSkillHermes(father); }
CPSkillHermes::CPSkillHermes(Entity * father) :CAbility(CMP_PASSIVE_HERMES, father, 50, 25, MASK_LEGS_0, 1){
	pEnt->getMessage(new MModVelAndJump(pEnt->getID(), 20.0f, 20.0f));
}
CPSkillHermes::~CPSkillHermes(){}

void CPSkillHermes::tick(float delta){}
void CPSkillHermes::getMessage(Message* m){
	if (m->getType() == MSG_RESTORE_LIFE_CARDS){
		_componentLife += (_componentLife / 2);
		if (_componentLife > _limitLife){
			_componentLife = _limitLife;
		}
	}
	CAbility::getMessage(m);
}


///modify vel of fire rate
GameComponent* createAbilitySyn(Entity* father, int id){ return new CPSkillSyn(father); }
CPSkillSyn::CPSkillSyn(Entity * father) :CAbility(CMP_PASSIVE_SYN, father, 50, 50, MASK_LEGS_0, 1){
	pEnt->getMessage(new MModVelAndJump(pEnt->getID(), 20, 20));
}
CPSkillSyn::~CPSkillSyn(){}

void CPSkillSyn::tick(float delta){}
void CPSkillSyn::getMessage(Message* m){
	if (m->getType() == MSG_RESTORE_LIFE_CARDS){
		_componentLife += (_componentLife / 2);
		if (_componentLife > _limitLife){
			_componentLife = _limitLife;
		}
	}
	CAbility::getMessage(m);
}

#pragma endregion