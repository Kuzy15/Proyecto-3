#include "Components.h"
#include "Entity.h"
#include <OgreEntity.h>
#include <OgreSceneNode.h>
#include "Messages.h"
#include "Game.h"



//Debug 
#ifdef _DEBUG
#include <iostream>
#endif


/*-------------------------BASIC GAME COMPONENT------------------------------------*/
//Component base class, made for inheriting.
//It implements basic behaviours like gets, sets 
//and message sending
#pragma region gameComponent
gameComponent::gameComponent(componentType id, Entity * father): _id(id), pEnt(father)
{
	
}
gameComponent::~gameComponent(){

}
componentType gameComponent::getID(){
	return _id;
}
bool gameComponent::getActive(){
	return _active;
}
void gameComponent::setActive(bool nw){
	_active = nw;
}
void gameComponent::sendMessage(Message * m){
	pEnt->getMessage(m);
}

#pragma endregion

/*-------------------------DEBUG COMPONENTS------------------------------------*/
//PRINTS A STRING WHEN RECEIVEING A STRING_MESSAGE
#pragma region stringComponent
stringComponent::stringComponent(Entity * fath) : gameComponent(STRING_COMPONENT, fath), whatSay("HOLA, soy el componente basico"){
}
stringComponent::~stringComponent(){

}
void stringComponent::getMessage(Message * m){
#ifdef _DEBUG
	//if (m->getType() == STRING_MSG)std::cout << "MESSAGE SAID: " << static_cast<stringMessage*>(m)->getText() << std::endl;
	if (m->getType() == INPUT_MSG){ 
		
		std::cout << "Basic Component from entity " << pEnt->getID() << " received message!" << std::endl;
		std::list<Message*> inputMessages = static_cast<InputMessage*>(m)->getMessages();
		std::list<Message*>::const_iterator it = inputMessages.cbegin();
		while (it != inputMessages.cend()){
			if ((*it)->getType() == I_BUTTONDOWN_MSG) std::cout << "MESSAGE SAID: " << static_cast<IButtonDownMessage*>(m)->getButton() << std::endl; 
			it++;
		}
	
	}
#endif
}
void stringComponent::tick(float delta){
#ifdef _DEBUG
#endif
}

#pragma endregion

#pragma region messageSendComponent
messageSendComponent::messageSendComponent(Entity * father):gameComponent(MESSAGESEND_COMPONENT, father) {
	i = 0;
}
messageSendComponent::~messageSendComponent() {
}
void messageSendComponent::tick(float delta) {
	i++;
	UpdateTransformMessage * m = new UpdateTransformMessage(Ogre::Vector3(0, 0, i/5.0), Ogre::Quaternion::IDENTITY, pEnt->getID());
	pEnt->getMessage(m);
	
}
void messageSendComponent::getMessage(Message * m) {

}

#pragma endregion


/*-------------------------OGRE COMPONENTS------------------------------------*/
//Render Component class. Father to every
//other render component.
#pragma region renderComponent

renderComponent::renderComponent(componentType t, Entity * father, Ogre::SceneManager * scnM)
	: gameComponent(t, father), pSceneMgr(scnM)
{
	pOgreSceneNode = pSceneMgr->getRootSceneNode()->createChildSceneNode();
}
renderComponent::~renderComponent(){
	pSceneMgr->destroySceneNode(pOgreSceneNode);
}

//Get Message general to every other render component child to this
void renderComponent::getMessage(Message *m) {
	switch (m->getType()) {
	case ENTITY_UPDATETRANSFORM:
		_ogrepos = static_cast<UpdateTransformMessage *>(m)->GetPos();
		_ogrequat = static_cast<UpdateTransformMessage *>(m)->GetQuat();
		std::cout << "new position: " << _ogrepos.x << " "<< _ogrepos.y << " " << _ogrepos.z << std::endl;
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
meshRenderComponent::meshRenderComponent(Ogre::Vector3 p, std::string meshName, Entity * father, Ogre::SceneManager * scnM) :renderComponent(MESH_RENDER_COMPONENT, father, scnM) {
	pOgreEnt = pSceneMgr->createEntity(meshName);
	pOgreSceneNode->attachObject(pOgreEnt);
	pOgreSceneNode->setPosition(p);
	_ogrepos = p;
	_ogrequat = Ogre::Quaternion();
}
meshRenderComponent::~meshRenderComponent() {
	pOgreSceneNode->detachObject(pOgreEnt);
	pSceneMgr->destroyEntity(pOgreEnt);
}
void meshRenderComponent::tick(float delta) {

	//Firstly we update the Ogre position and rotation with the values
	//stored in the variables.
	pOgreSceneNode->setPosition(_ogrepos);
	pOgreSceneNode->setOrientation(_ogrequat);
}
void meshRenderComponent::getMessage(Message * m) {
	renderComponent::getMessage(m);
	//TO DO: IMPLEMENT MESSAGE RECEIVEING TO MOVE.

}

#pragma endregion
#pragma region Camera Component
CameraComponent::CameraComponent(Entity * father, Ogre::SceneManager * scnMgr, Ogre::Viewport * vp, std::string camName, Ogre::Vector3 pos, Ogre::Vector3 lookAt, Ogre::Real ratio, int clipDistance)
	: gameComponent(CAMERA_COMPONENT, father), _scnMgr(scnMgr), _camName(camName), _vp(vp), _pos(pos), _lookAt(lookAt)
{
	_cam = _scnMgr->createCamera(_camName);
	_cam->setPosition(pos);
	_cam->lookAt(lookAt);
	_cam->setAspectRatio(ratio);

	vp = Game::getInstance()->getRenderWindow()->addViewport(_cam); 
	vp->setBackgroundColour(Ogre::ColourValue(150, 150, 150));
}
CameraComponent::~CameraComponent() {
	delete _cam;
}



#pragma endregion



/*-------------------------BOX2D COMPONENTS------------------------------------*/

//Rigid Body component.
//Gives an entity a rigid body to simulate physics
#pragma region rigidBodyComponent
rigidBodyComponent::rigidBodyComponent(Entity * father, b2World * world, Ogre::Vector2 posInPixels, float heightInPixels, float weightInPixels, rigidBodyType rbType, shapeType shType) 
: _rbHeight(heightInPixels / PPM), _rbWeight(weightInPixels / PPM), _myWorld(world), gameComponent(PHYSICS_COMPONENT,father) {
	
	//Sets the pos attached to the render.
	_pos.x = posInPixels.x / PPM;
	_pos.y = posInPixels.y / PPM;

	//Initial velocity 0.
	_vel.x = _vel.y = 0;

	//Body definition.
	_bodyDef.position.Set(_pos.x, _pos.y);
	switch (rbType)
	{
		case DYNAMIC:
			_bodyDef.type = b2_dynamicBody;
			break;
		case STATIC:
			_bodyDef.type = b2_staticBody;
			break;
		case KINEMATIC:
			_bodyDef.type = b2_kinematicBody;
			break;
		default:
			_bodyDef.type = b2_staticBody;
			break;
	}
	//Body creation.
	_body = _myWorld->CreateBody(&_bodyDef);
	
	//Shape creation.
	switch (shType)
	{
		case CIRCLE:
			_shape = new b2CircleShape;
			static_cast<b2CircleShape*>(_shape)->m_p.Set(0, 0);
			static_cast<b2CircleShape*>(_shape)->m_radius = _rbWeight;
			break;
		case POLYGON:
			_shape = new b2PolygonShape;
			static_cast<b2PolygonShape*>(_shape)->SetAsBox(_rbWeight / 2, _rbHeight / 2, { _rbWeight / 2, _rbHeight / 2 }, 0);
			break;
		default:
			break;
	}

	//Fixture Definition.
	_fixtureDef.shape = _shape;
	_fixtureDef.density = 1.0;
	_fixtureDef.friction = 0;
	/* FALTA ESTO POR CONFIGURAR
	fDef.filter.categoryBits = Juego::JUGADOR;
	fDef.filter.maskBits = Juego::ENEMIGO | Juego::ITEM | Juego::ESCENARIO | Juego::ESCENARIO_NOCOL | Juego::AT_ENEMIGO;
	*/
	
	//Fixture creation.
	_fixture = _body->CreateFixture(&_fixtureDef);
	



}
rigidBodyComponent::~rigidBodyComponent() {
	_myWorld->DestroyBody(_body);
	_body = nullptr;
}
void rigidBodyComponent::tick(float delta) {

	//Procesar los mensajes que han llegado

	




}
void rigidBodyComponent::getMessage(Message * m) {

}

#pragma endregion