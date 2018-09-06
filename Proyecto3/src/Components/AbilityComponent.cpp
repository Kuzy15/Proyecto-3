#include "AbilityComponent.h"

#include "Entity.h"

CAbility::CAbility(ComponentType c, Entity* father, float componentLife, float componentArmor, uint16 mask, int t) :GameComponent(c, father), _componentLife(componentLife),
_componentArmor(componentArmor), _limitLife(componentLife), _type(t)

{
	dead = false;
	if (pEnt->getID() == "Player_0"){
		switch (mask)
		{
		case MASK_LEGS_0:
			_myMask = MASK_LEGS_0;
			break;
		case MASK_HEAD_0:
			_myMask = MASK_HEAD_0;
			break;
		case MASK_CHEST_0:
			_myMask = MASK_CHEST_0;
			break;
		default:
			break;
		}

	}
	else
	{

		switch (mask)
		{
		case MASK_LEGS_0:
			_myMask = MASK_LEGS_1;
			break;
		case MASK_HEAD_0:
			_myMask = MASK_HEAD_1;
			break;
		case MASK_CHEST_0:
			_myMask = MASK_CHEST_1;
			break;
		default:
			break;
		}
				
	}

}
CAbility::~CAbility(){}

void CAbility::getMessage(Message *m){

	/*Comprobar si hay que llamar a esta funcion en cada una de las virtuales de los hijos*/

	switch (m->getType()){
	case MSG_DAMAGE_ARMATURE:
		MDamageArmature* mDA = static_cast<MDamageArmature*>(m);
		if (mDA->getWhere() == _myMask){
			_componentLife -= mDA->getDamage();
			//Si se acaba la vida pues quitarlo o lo que sea. Aclarar el significado de armadura
			float dmg = mDA->getDamage();
			if (_componentLife > 0){
				dmg = mDA->getDamage() * (1 - _componentArmor / 100.0f);
			}
			else if (!dead){
				if (_myMask == MASK_HEAD_0 || _myMask == MASK_HEAD_1)
					pEnt->getMessage(new MActiveDead(pEnt->getID()));
				else
					pEnt->getMessage(new MPassiveDead(pEnt->getID()));
				dead = true;
			}
			pEnt->getMessage(new MDamage(dmg, pEnt->getID()));
		}
		break;
	}
}