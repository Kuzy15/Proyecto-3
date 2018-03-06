#include <string>
#include <list>


#ifndef COMPONENTS_H
#define COMPONENTS_H

class Entity;
class Message;


class gameComponent
{
public:
	gameComponent(std::string id, Entity * ent);
	~gameComponent();

	//Update method
	virtual void tick(float delta) = 0;


	//Getters and setters
	bool getActive();
	void setActive(bool nw);
	std::string getID();

	// Message Methods
	void getMessage(Message *);
	void sendMessage(Message *);


private:
	Entity * pEnt;

};


#endif