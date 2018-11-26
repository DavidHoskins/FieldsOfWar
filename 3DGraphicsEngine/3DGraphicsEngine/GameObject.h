#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class InputHandler;
class HexHandler;

class GameObject: public sf::CircleShape
{
private:
	//Called for the rendering, if this unit is active then it's in use otherwise it's not in use.
	bool m_isActive;
public:

	GameObject(float radius, int numbPoints);

	//Getters and setters.
	bool getActive()const {return m_isActive;}
	void setActive(bool value){m_isActive = value;}

	virtual void render(sf::RenderWindow& renderWindow) const = 0;
	virtual void update(InputHandler& inputHandler, HexHandler* hexHandler) = 0;
};

#endif // !GAMEOBJECT_H

