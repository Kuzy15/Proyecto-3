#include "GUIComponents.h"

#include <OgreOverlay.h>
#include <OgreOverlayManager.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayContainer.h>
#include <OgreFontManager.h>
#include <OgreOverlaySystem.h>

#include "Entity.h"
#include "Game.h"
#include "Scenes.h"


#pragma region GUI COMPONENTS
#pragma region ButtonGUI
CButtonGUI::CButtonGUI(ComponentType t, Ogre::Overlay * overlay, Entity * father, size_t sceneId, Ogre::Vector2 screenpos, Ogre::Vector2 pixelSize) :GameComponent(t, father), _clicked(false) {
	pOver = overlay;

	_sceneId = sceneId;


}
CButtonGUI::~CButtonGUI()
{

}
bool CButtonGUI::canClick() {

	float act = SDL_GetTicks();

	if (act - _lastClick > _minClickTime) {
		_lastClick = act;
		return true;
	}
	else return false;
}
size_t CButtonGUI::getScnId() {
	return _sceneId;
}

void CButtonGUI::tick(float delta)
{

}
void CButtonGUI::getMessage(Message * me)
{


}

#pragma endregion

#pragma region Normal Button
CNormalButton::CNormalButton(Ogre::Overlay * overlay, Entity * father, size_t sceneId, Ogre::Vector2 screenpos, Ogre::Vector2 pixelSize, ButtonCallback callback, std::string buttonTxt) :CButtonGUI(CMP_NORMAL_BUTTON, overlay, father, sceneId, screenpos, pixelSize) {
	materials[0] = "GUI/Button/Idle";
	materials[1] = "GUI/Button/Active";
	materials[2] = "GUI/Button/Click";


	_callback = callback;
	_txt = buttonTxt;


	pContainer = static_cast<Ogre::OverlayContainer *>(Ogre::OverlayManager::getSingleton().createOverlayElementFromTemplate("GUI/BaseButton", "Panel", father->getID()));



	pContainer->setPosition(screenpos.x, screenpos.y);
	overlay->add2D(pContainer);

	try {
		a = static_cast<Ogre::TextAreaOverlayElement *>(pContainer->getChild(pContainer->getName() + "/GUI/BaseButton/Text"));
		a->setCaption(_txt);

	}
	catch (Ogre::Exception e) {};

}
CNormalButton::~CNormalButton()
{
	if (Ogre::OverlayManager::getSingleton().hasOverlayElement(pContainer->getName())){
		Ogre::OverlayManager::getSingleton().destroyOverlayElement(pContainer);
		Ogre::OverlayManager::getSingleton().destroyOverlayElement(a);
	}
}


void CNormalButton::getMessage(Message * me)
{


	if (me->getType() == MSG_GUI_BUTTON_ACTIVE)
	{
		if (static_cast<MButtonAct*>(me)->getActiveButtonIndex() == _sceneId){
			_active = true;
			pContainer->setMaterialName(materials[1]);
			Game::getInstance()->getSoundEngine()->play2D("../Media/sounds/Buttons/Button.wav", false);
		}
		else if (_active)
		{
			_active = false;
			pContainer->setMaterialName(materials[0]);

		}


	}
	if (_active && me->getType() == MSG_GUI_BUTTON_CLICK) {
		pContainer->setMaterialName(materials[2]);
		_clicked = true;
		_callback();
		Game::getInstance()->getSoundEngine()->play2D("../Media/sounds/Buttons/ButtonOk.wav", false);

	}
}

#pragma endregion

#pragma region Ability Button
CAbilityButton::CAbilityButton(Ogre::Overlay * overlay, Entity * father, size_t sceneId, Ogre::Vector2 screenpos, Ogre::Vector2 pixelSize, int playerId, ComponentType compType) :CButtonGUI(CMP_NORMAL_BUTTON, overlay, father, sceneId, screenpos, pixelSize),
_playerId(playerId), _compType(compType){

	std::string cmpName = compToString(compType, _type);
	_clicked = false;

	materials[0] = cmpName + "-IDLE";
	materials[1] = cmpName + "-ACTIVE";
	materials[2] = cmpName + "-CLICK";


	pContainer = static_cast<Ogre::OverlayContainer *>(Ogre::OverlayManager::getSingleton().createOverlayElementFromTemplate("GUI/BaseButton", "Panel", father->getID() + std::to_string(SDL_GetTicks())));
	std::string _txt = "";
	try {
		a = static_cast<Ogre::TextAreaOverlayElement *>(pContainer->getChild(pContainer->getName() + "/GUI/BaseButton/Text"));
		a->setCaption(_txt);

	}
	catch (Ogre::Exception e) {};
	pContainer->setMetricsMode(Ogre::GuiMetricsMode::GMM_RELATIVE);
	pContainer->setHorizontalAlignment(Ogre::GuiHorizontalAlignment::GHA_LEFT);
	pContainer->setVerticalAlignment(Ogre::GuiVerticalAlignment::GVA_TOP);
	pContainer->setPosition(screenpos.x, screenpos.y);
	pContainer->setDimensions(0.156, 0.42);
	pContainer->setMaterialName(materials[0]);
	overlay->add2D(pContainer);

}
CAbilityButton::~CAbilityButton()
{
	if (Ogre::OverlayManager::getSingleton().hasOverlayElement(pContainer->getName())){
		Ogre::OverlayManager::getSingleton().destroyOverlayElement(pContainer);
		Ogre::OverlayManager::getSingleton().destroyOverlayElement(a);
	}

}


void CAbilityButton::getMessage(Message * me)
{

	if (!_clicked){
		switch (me->getType()){
		case MSG_GUI_BUTTON_ACTIVE:
			if (static_cast<MButtonAct*>(me)->getActiveButtonIndex() == _sceneId){
				_active = true;
				pContainer->setMaterialName(materials[1]);
				Game::getInstance()->getSoundEngine()->play2D("../Media/sounds/Buttons/Button.wav", false);

			}
			else if (_active)
			{
				_active = false;
				pContainer->setMaterialName(materials[0]);
			}
			break;
		case MSG_GUI_BUTTON_CLICK:
			if (static_cast<MButtonClick*>(me)->getId() == _sceneId){
				pContainer->setMaterialName(materials[2]);
				_clicked = true;
				Game::getInstance()->getSoundEngine()->play2D("../Media/sounds/Buttons/ButtonOk.wav", false);
				pEnt->getScene()->getMessage(new MAbilitySet(pEnt->getID(), _playerId, _compType, _type));
			}
			break;
		case MSG_ABILITY_SETTER:
			if (_type == static_cast<MAbilitySet*>(me)->getType()){
				pContainer->setMaterialName(materials[2]);
				_clicked = true;
			}
			break;
		default:
			break;
		}
	}
}

#pragma endregion




#pragma region God Button
CGodButton::CGodButton(Ogre::Overlay * overlay, Entity * father, size_t sceneId, Ogre::Vector2 screenpos, Ogre::Vector2 pixelSize, int playerId, E_GOD god) :CButtonGUI(CMP_NORMAL_BUTTON, overlay, father, sceneId, screenpos, pixelSize),
_playerId(playerId), _god(god){

	std::string godName = pEnt->getScene()->godToString(god);

	materials[0] = godName + "-IDLE";
	materials[1] = godName + "-ACTIVE";
	materials[2] = godName + "-CLICK";




	pContainer = static_cast<Ogre::OverlayContainer *>(Ogre::OverlayManager::getSingleton().createOverlayElementFromTemplate("GUI/BaseButton", "Panel", pEnt->getID()));


	std::string _txt = "";
	try {
		a = static_cast<Ogre::TextAreaOverlayElement *>(pContainer->getChild(pContainer->getName() + "/GUI/BaseButton/Text"));
		a->setCaption(_txt);

	}
	catch (Ogre::Exception e) {};

	pContainer->setMetricsMode(Ogre::GuiMetricsMode::GMM_RELATIVE);
	pContainer->setHorizontalAlignment(Ogre::GuiHorizontalAlignment::GHA_LEFT);
	pContainer->setVerticalAlignment(Ogre::GuiVerticalAlignment::GVA_TOP);
	pContainer->setPosition(screenpos.x, screenpos.y);
	pContainer->setDimensions(0.1041, 0.1851);
	overlay->add2D(pContainer);
	pContainer->setMaterialName(materials[0]);


}
CGodButton::~CGodButton()
{

	if (Ogre::OverlayManager::getSingleton().hasOverlayElement(pContainer->getName())){
		Ogre::OverlayManager::getSingleton().destroyOverlayElement(pContainer);
		Ogre::OverlayManager::getSingleton().destroyOverlayElement(a);
	}
}


void CGodButton::getMessage(Message * me)
{

	if (!_clicked){
		switch (me->getType()){
		case MSG_GUI_BUTTON_ACTIVE:
			if (static_cast<MButtonAct*>(me)->getActiveButtonIndex() == _sceneId){
				_active = true;
				pContainer->setMaterialName(materials[1]);
				Game::getInstance()->getSoundEngine()->play2D("../Media/sounds/Buttons/Button.wav", false);

			}
			else if (_active)
			{
				_active = false;
				pContainer->setMaterialName(materials[0]);
			}
			break;
		case MSG_GUI_BUTTON_CLICK:
			if (static_cast<MButtonClick*>(me)->getId() == _sceneId){
				pContainer->setMaterialName(materials[2]);
				_clicked = true;
				Game::getInstance()->getSoundEngine()->play2D("../Media/sounds/Buttons/ButtonOk.wav", false);
				pEnt->getScene()->getMessage(new MGodSet(pEnt->getID(), _playerId, _god));
			}
			break;
		default:
			break;
		}
	}


}

#pragma endregion




#pragma region PlayerGUI
CPlayerGUI::CPlayerGUI(Entity * father, Ogre::Overlay * ov, guiPlayer plyer, E_GOD character) : GameComponent(CMP_GUI_PLAYERGUI, father), pOverlay(ov), p(plyer)
{


	if (plyer == P1)player = "Player1";
	else player = "Player2";
	roundsWon = 0;

	//General container of the whole Player HUD
	pHud = pOverlay->getChild(player);
	pLowerHud = static_cast<Ogre::OverlayContainer *>(pHud->getChild(player + "/ActiveContainer"));

	//Specific reference to the lifebar and active bar, which we'll be using quite often
	plifeBar = static_cast<Ogre::OverlayContainer*>(pHud->getChild(player + "/LifeBar"));
	pActiveBar = static_cast<Ogre::OverlayContainer*>(pLowerHud->getChild(player + "/ActiveContainer/ActiveBar"));
	std::string charName;
	std::string iconMatName = "GUI/Icons/";


	switch (character) {
	case EG_RA:
		charName = "Ra";
		if (p == P1)iconMatName += "Ra_P1";
		else iconMatName += "Ra_P2";
		break;
	case EG_AHPUCH:
		charName = "Ahpuch";
		if (p == P1)iconMatName += "AhPuch_P1";
		else iconMatName += "AhPuch_P2";
		break;
	case EG_HACHIMAN:
		charName = "Hachiman";
		if (p == P1)iconMatName += "Hachiman_P1";
		else iconMatName += "Hachiman_P2";
		break;
	case EG_ZEUS:
		charName = "Zeus";
		if (p == P1)iconMatName += "Zeus_P1";
		else iconMatName += "Zeus_P2";
		break;
	}


	//	std::cout << iconMatName << std::endl;
	pHud->getChild(player + "/Name")->setCaption(charName);
	pHud->getChild(player + "/Icon")->setMaterialName(iconMatName);

	LIFE_MAX_WIDTH = plifeBar->getWidth();
	ACTIVE_MAX_WIDTH = pActiveBar->getWidth();
	LIFE_MIN_WIDTH = ACTIVE_MIN_WIDTH = 0.0f;




}
CPlayerGUI::~CPlayerGUI() {}

void CPlayerGUI::tick(float delta) {
}
void CPlayerGUI::getMessage(Message * m) {

	MActiveDead* mAD;
	MPassiveDead* mPD;

	switch (m->getType())
	{
	case MSG_LIFE_STATE:
		if ((m->getEmmiter() == "Player_0" && p == P1) || (m->getEmmiter() == "Player_1" && p == P2))
			updateLifebar(static_cast<MLifeState *>(m)->getLifeState());
		break;
	case MSG_UPDATE_ACTIVETIMER:
		if (m->getEmmiter() == "Player_0" && p == P1 || m->getEmmiter() == "Player_1" && p == P2)
			updateActive(static_cast<MUpdateActiveTimer *>(m)->getActiveTimer());
		break;
	case MSG_ROUND_FINISHED:
		if (static_cast<MRoundFinished *>(m)->getWinnerId() == "Player_0" && p == P1 || static_cast<MRoundFinished *>(m)->getWinnerId() == "Player_1" && p == P2) {
			if (roundsWon == 0) {
				pHud->getChild(player + "/Round1")->setMaterialName("GUI/RoundFull");
				roundsWon++;
			}
			else if (roundsWon == 1)
			{
				pHud->getChild(player + "/Round2")->setMaterialName("GUI/RoundFull");
				roundsWon++;
			}
		}
		break;
	case MSG_ACTIVE_DEAD:
		if (m->getEmmiter() == "Player_0" && p == P1 || m->getEmmiter() == "Player_1" && p == P2) {
			pLowerHud->getChild(player + "/ActiveContainer/ActiveFrame")->setMaterialName("GUI/" + player + "/ActiveDead");
			pActiveBar->setWidth(0);
		}
		break;
	case MSG_PASSIVE_DEAD:

		if (m->getEmmiter() == "Player_0" && p == P1 || m->getEmmiter() == "Player_1" && p == P2)
			pLowerHud->getChild(player + "/ActiveConainer/PassiveIcon")->setMaterialName("GUI/PassiveDown");
		break;
	case MSG_RESET_GUI:

		pActiveBar->setMaterialName("GUI/" + player + "/ActiveBar");

		pLowerHud->getChild(player + "/ActiveConainer/PassiveIcon")->setMaterialName("GUI/PassiveUp");
		updateLifebar(LIFE_MAX_WIDTH);
		updateActive(ACTIVE_MAX_WIDTH);


		break;



	}
}

void CPlayerGUI::updateActive(size_t val)
{
	float newVal = (ACTIVE_MAX_WIDTH * val) / 100;
	if (newVal < ACTIVE_MIN_WIDTH)newVal = ACTIVE_MIN_WIDTH;
	if (p == P1)
		pActiveBar->setWidth(newVal);
	else {
		Ogre::Real newX = pActiveBar->getLeft();
		newX = (newX + pActiveBar->getWidth()) - newVal;
		pActiveBar->setLeft(newX);
		pActiveBar->setWidth(newVal);

	}

}
void CPlayerGUI::updateLifebar(size_t val) {

	float newVal = (LIFE_MAX_WIDTH * val) / 100;
	if (newVal < LIFE_MIN_WIDTH)newVal = LIFE_MIN_WIDTH;
	if (p == P1)
		plifeBar->setWidth(newVal);
	else {
		Ogre::Real newX = plifeBar->getLeft();
		newX = (newX + plifeBar->getWidth()) - newVal;
		plifeBar->setLeft(newX);
		plifeBar->setWidth(newVal);
	}
}

#pragma endregion

#pragma region CGUITimer

CGUITimer::CGUITimer(Entity* father, Ogre::Overlay * overlay) :GameComponent(CMP_GUI_TIMER, father)
{
	pTimer = overlay->getChild("GUI/TimerPanel");
}

CGUITimer::~CGUITimer()
{
}
void CGUITimer::tick(float delta)
{

}
void CGUITimer::getMessage(Message * m)
{
	if (m->getType() == MSG_UPDATE_SCENETIMER) {
		pTimer->getChild("TimerPanel/TextArea")->setCaption(std::to_string(static_cast<MUpdateSceneTimer *>(m)->getSceneTimer()));
	}
}
#pragma endregion


