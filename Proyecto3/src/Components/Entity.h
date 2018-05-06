#include <list>
#include <queue>
#include <vector>


#ifndef ENTITY_H
#define ENTITY_H

class Message;
class GameComponent;
class GameScene;
enum ComponentType;

class Entity
{
public:
	Entity(std::string id, GameScene * sc);
	~Entity();
	void tick(float delta);

//Getters and setters
	bool getActive();
	void setActive(bool nw);
	GameScene* getScene();
	std::string getID();

// Message Methods
	void getMessage(Message *);
	void sendMessage(Message *);

	//Component methods
	void addComponent(GameComponent * c);
	void deleteComponent(ComponentType id);



private:
	bool _active;
	std::string _id;
	std::vector<GameComponent *> components;
	std::queue <Message*>msgs;

	GameScene * scene;
	//queue managing during tick function
	void dispatch();

};




#endif