#include "../headers/Enemy.h"
#include <algorithm>

void Enemy::TakeDamage(const int d)
{
    const int damage = std::max(0, d - m_Defense);
	m_Health = std::max(0, m_Health - damage);
}
