#include <string>
#include <list>


#ifndef COMPONENTS_H
#define COMPONENTS_H

class Entity;
class Message;


class gameComponent
{
public:
	
	virtual ~gameComponent() = 0;

	//Update method
	virtual void tick(float delta) = 0;


	//Getters and setters
	bool getActive();
	void setActive(bool nw);
	std::string getID();

	//Gets a message from the entity and acts according to it
	virtual void getMessage(Message *) = 0;
    


protected:
	//Used to send a message from the component
	void sendMessage(Message *);

	//Constructor protected so that only
	//other game components inheriting from this class can use it
	gameComponent(std::string id, Entity * ent);


private:
	//pointer to the components' entity
	Entity * pEnt;
	//basic atributes of the component
	std::string _id;
	bool _active;

};
class stringComponent : public gameComponent
{
public:
	stringComponent();
	~stringComponent();

private:

};


#endif