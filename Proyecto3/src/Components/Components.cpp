#include "Components.h"
#include "Entity.h"
#include <OgreEntity.h>
#include <OgreSceneNode.h>
#include "Messages.h"



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

	
}
void messageSendComponent::getMessage(Message * m) {

}

#pragma endregion


/*-------------------------RENDER COMPONENTS------------------------------------*/
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
		//std::cout << "new position: " << _ogrepos.x << " "<< _ogrepos.y << " " << _ogrepos.z << std::endl;
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

//Rigid Body component.
//Gives an entity a rigid body to simulate physics
#pragma region RigidBodyComponent
RigidBodyComponent::RigidBodyComponent(Entity * father, b2World * world, Ogre::Vector3 posInPixels, float heightInPixels, float weightInPixels, rigidBodyType rbType, shapeType shType) 
: _rbHeight(heightInPixels / PPM), _rbWeight(weightInPixels / PPM), _myWorld(world), gameComponent(PHYSICS_COMPONENT,father) {
	
	//Sets the pos attached to the render.
	_pos.x = posInPixels.x / PPM;
	_pos.y = posInPixels.y / PPM;

	//Initial velocity 0.
	_vel.x = _vel.y = 0;

	//Body definition.
	_bodyDef.position.Set(_pos.x, _pos.y);
	/*_bodyDef.linearDamping = 5.0f;
	_bodyDef.angularDamping = 0.0f;*/
	
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
RigidBodyComponent::~RigidBodyComponent() {
	_myWorld->DestroyBody(_body);
	_body = nullptr;
}
void RigidBodyComponent::tick(float delta) {

	

	//Send the message to the entity.
	//Transformation from physics world to ogre world.
	
	
	UpdateTransformMessage * m = new UpdateTransformMessage(Ogre::Vector3(_body->GetPosition().x * PPM, _body->GetPosition().y * PPM, 0), Ogre::Quaternion::IDENTITY, pEnt->getID());
	pEnt->getMessage(m);

	std::cout << pEnt->getID()  << _body->GetPosition().y << std::endl;




}
void RigidBodyComponent::getMessage(Message * m) {
	
	if (m->getType() == MSG_PLAYER_MOVE_X){
		
			//transformarlo
		float value = static_cast<MessagePlayerMoveX*>(m)->GetValue();
		value = value / 1000;
		b2Vec2 newForce(0, value);
		_body->ApplyForceToCenter(newForce,true);
		std::cout << "Aplicada fuerza de: " << value << std::endl;	
	
	}

}

#pragma endregion

//Player Controller Component
#pragma region PlayerControllerComponent
PlayerControllerComponent::PlayerControllerComponent(Entity* f, int i): gameComponent(PLAYER_CONTROLLER_COMPONENT, f), _id(i){
	
}

PlayerControllerComponent::~PlayerControllerComponent(){}

void PlayerControllerComponent::tick(float delta){
}

void PlayerControllerComponent::getMessage(Message* m){
	//If the msg type is CInputState, read the input and process it
	if (m->getType() == INPUT_STATE_MSG){
		InputStateMessage* inputM = static_cast<InputStateMessage*>(m);
		if (inputM->getId() == _id){

			CInputState cState = inputM->getCInputState();

			if (cState.Button_A == BTT_PRESSED){
				std::cout << "Se ha pulsado el boton A" << std::endl;
			}
			if (cState.Axis_LeftX > 0){
				MessagePlayerMoveX* m = new MessagePlayerMoveX(cState.Axis_LeftX, _id, pEnt->getID());
				pEnt->getMessage(m);
			}
		}
	}
}
#pragma endregion