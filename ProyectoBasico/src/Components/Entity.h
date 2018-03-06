#include <list>
#include <queue>


#ifndef ENTITY_H
#define ENTITY_H

class Message;
class gameComponent;
class Scene;

class Entity
{
public:
	Entity(std::string id, Scene * sc);
	~Entity();
	void tick(float delta);

//Getters and setters
	bool getActive();
	void setActive(bool nw);
	std::string getID();

// Message Methods
	void getMessage(Message *);
	void sendMessage(Message *);



private:
	bool _active;
	std::string _id;
	std::list<gameComponent *> components;
	std::queue <Message*>msgs;

	Scene * scene;
	//queue managing during tick function
	void dispatch();
};




#endif