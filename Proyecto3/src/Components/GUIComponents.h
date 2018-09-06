#ifndef GUICOMPONENTS_H
#define GUICOMPONENTS_H


#include "Components.h"
#include <OgreOverlay.h>

class Entity;
class Message;




class CButtonGUI : public GameComponent
{
public:
	CButtonGUI(ComponentType t, Ogre::Overlay * overlay, Entity * father, size_t _id, Ogre::Vector2 screenpos, Ogre::Vector2 pixelSize);
	~CButtonGUI();
	virtual void tick(float delta);
	virtual void getMessage(Message * m);
	size_t getScnId();

protected:
	void toggleClick(bool click);
	void toggleActive(bool active);
	bool canClick();
	std::string materials[3];
	Ogre::OverlayContainer * pContainer;
	Ogre::Overlay * pOver;

	bool _active;
	bool _clicked;

	//The id that the button will have in the scene
	size_t _sceneId;

	float _lastClick;
	const float _minClickTime = 500;
};



class CGUITimer : public GameComponent
{
public:
	CGUITimer(Entity * father, Ogre::Overlay * overlay);
	~CGUITimer();
	virtual void tick(float delta);
	virtual void getMessage(Message * m);

private:
	Ogre::OverlayContainer * pTimer;

};




class CNormalButton : public CButtonGUI
{
public:
	CNormalButton(Ogre::Overlay * overlay, Entity * father, size_t _id, Ogre::Vector2 screenpos, Ogre::Vector2 pixelSize, ButtonCallback callback, std::string buttonTxt);
	~CNormalButton();

	virtual void getMessage(Message * m);


private:
	ButtonCallback * _callback;
	Ogre::String _txt;
	Ogre::TextAreaOverlayElement * a;


};




class CAbilityButton : public CButtonGUI
{
public:
	CAbilityButton(Ogre::Overlay * overlay, Entity * father, size_t _id, Ogre::Vector2 screenpos, Ogre::Vector2 pixelSize, int playerId, ComponentType compType);
	~CAbilityButton();

	virtual void getMessage(Message * m);

private:
	int _playerId;
	ComponentType _compType;
	int _type;
	Ogre::TextAreaOverlayElement * a;


};




class CGodButton : public CButtonGUI
{
public:
	CGodButton(Ogre::Overlay * overlay, Entity * father, size_t _id, Ogre::Vector2 screenpos, Ogre::Vector2 pixelSize, int playerId, E_GOD god);
	~CGodButton();

	virtual void getMessage(Message * m);

private:
	int _playerId;
	Ogre::TextAreaOverlayElement * a;
	E_GOD _god;

};




enum guiPlayer{
	P1 = 0, P2 = 1
};



class CPlayerGUI : public GameComponent
{
public:
	CPlayerGUI(Entity* father, Ogre::Overlay * ov, guiPlayer p, E_GOD character);
	~CPlayerGUI();
	void updateLifebar(size_t val);
	void updateActive(size_t val);
	virtual void tick(float delta);
	virtual void getMessage(Message * m);
	void updatePassive();

private:
	Ogre::OverlayContainer * pHud;
	Ogre::OverlayContainer * pLowerHud;
	Ogre::OverlayContainer * plifeBar;
	Ogre::OverlayContainer * pActiveBar;
	Ogre::Overlay * pOverlay;
	std::string player;
	float LIFE_MAX_WIDTH, LIFE_MIN_WIDTH = 5;
	float ACTIVE_MAX_WIDTH, ACTIVE_MIN_WIDTH = 5;
	size_t roundsWon;
	guiPlayer p;
};

#endif
