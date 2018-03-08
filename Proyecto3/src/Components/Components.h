#include <string>
#include <list>


#ifndef COMPONENTS_H
#define COMPONENTS_H

class Entity;
class Message;


class gameComponent
{
public:
	
	virtual ~gameComponent();
	//Update method
	virtual void tick(float delta) = 0;
	//Gets a message from the entity and acts according to it
	virtual void getMessage(Message *) = 0;

	//Getters and setters
	bool getActive();
	void setActive(bool nw);
	std::string getID();


    


protected:
	//Used to send a message from the component
	void sendMessage(Message *);

	//Constructor protected so that only
	//other game components inheriting from this class can use it
	gameComponent(std::string id, Entity * ent);

	//pointer to the components' entity
	Entity * pEnt;
	//basic atributes of the component
	std::string _id;
	bool _active;

};
class stringComponent : public gameComponent
{
public:
	stringComponent(std::string id, Entity *);
	~stringComponent();
	virtual void getMessage(Message * m);
	virtual void tick(float delta);

private:
	std::string whatSay;

};


#endif