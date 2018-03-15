#include <list>
#include <queue>


#ifndef ENTITY_H
#define ENTITY_H

class Message;
class gameComponent;
class gameScene;
enum componentType;

class Entity
{
public:
	Entity(std::string id, gameScene * sc);
	~Entity();
	void tick(float delta);

//Getters and setters
	bool getActive();
	void setActive(bool nw);
	std::string getID();

// Message Methods
	void getMessage(Message *);
	void sendMessage(Message *);

	//Component methods
	void addComponent(gameComponent * c);
	void deleteComponent(componentType id);



private:
	bool _active;
	std::string _id;
	std::list<gameComponent *> components;
	std::queue <Message*>msgs;

	gameScene * scene;
	//queue managing during tick function
	void dispatch();
};




#endif