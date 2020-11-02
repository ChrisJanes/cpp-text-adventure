#include "../headers/Enemy.h"
#include <algorithm>

void Enemy::take_damage(int d)
{
	// use max to ensure we don't end up with negative damage from defense
	int damage = std::max(0, d - defense);

	// and then again to make sure we don't go into negative health
	health = std::max(0, health - damage);
}
