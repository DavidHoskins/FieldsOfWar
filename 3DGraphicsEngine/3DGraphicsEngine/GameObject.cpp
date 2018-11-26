#include "GameObject.h"

GameObject::GameObject(float radius, int numbPoints)
	:sf::CircleShape(radius, numbPoints)
	,m_isActive(false)
{

}
