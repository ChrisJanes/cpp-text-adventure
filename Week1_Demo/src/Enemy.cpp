#include "../headers/Enemy.h"
#include <algorithm>

void Enemy::take_damage(int d)
{
	int damage = std::max(0, d - defense);
	health = std::max(0, health - damage);
}
