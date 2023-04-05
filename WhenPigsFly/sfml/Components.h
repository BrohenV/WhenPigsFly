#pragma once

#include "Common.h"
#include "Animation.h"
#include "Assets.h"
 
struct Component
{
	bool has{ false };
	Component() = default;
};


struct CAnimation : public Component
{
	Animation animation;
	bool repeat{ false };
	CAnimation() = default;

	CAnimation(const Animation& animation, bool r = true)
		:animation(animation), repeat(r) {}
};


struct CCollision : public Component
{
	float radius{ 0.f };
	
	CCollision() = default;
	CCollision(float r)
		: radius(r) {}
};


struct CInput : public Component
{
	bool up			{false};
	bool left		{false};
	bool right		{false};
	bool down		{false};
	bool shoot		{false};
	bool canShoot	{true};
	bool canJump	{true};

	CInput() = default;
};


struct CLifespan : public Component
{
	int total{ 0 };
	int remaining{ 0 };

	CLifespan() = default;
	CLifespan(int t) : total(t), remaining{ t } {}
	 
};


struct CBoundingBox : public Component
{
	sf::Vector2f size{0.f, 0.f};
	sf::Vector2f halfSize{ 0.f, 0.f };

	CBoundingBox() = default;
	CBoundingBox(const sf::Vector2f& s) : size(s), halfSize(0.5f * s)
	{}
};


struct CTransform : public Component
{
	
	sf::Vector2f	pos			{ 0.f, 0.f };
	sf::Vector2f	prevPos		{ 0.f, 0.f };
	sf::Vector2f	scale		{ 1.f, 1.f };
	sf::Vector2f	vel			{ 0.f, 0.f };

	float   angVel{ 0 };
	float	angle{ 0.f };

	CTransform() = default;
	CTransform(const sf::Vector2f& p) : pos(p)  {}
	CTransform(const sf::Vector2f& p, const sf::Vector2f& v, const sf::Vector2f& sc, float a)
		: pos(p), prevPos(p), vel(v), scale(sc), angle(a) {}
	CTransform(const sf::Vector2f& p, const sf::Vector2f& v, float a)
		: pos(p), prevPos(p), vel(v), scale{ 1.f, 1.f }, angle{ a } {}
	CTransform(const sf::Vector2f& p, const sf::Vector2f& v) : pos(p), vel(v) {}

};


struct CScore : public Component
{
	int score{ 0 };

	CScore() = default;
	CScore(int s = 0) : score(s) {}
};


struct CGravity : public Component
{
	float g{ 0 };

	CGravity() = default;
	CGravity(float g ) : g(g) {}
};


struct CState : public Component
{
	enum State {
		isGrounded		= 1,
		isFacingLeft	= 1 << 1, 
		isRunning		= 1 << 2,
		
	};
	unsigned int  state{ 0 };

	CState() = default;
	CState(unsigned int s ) : state(s) {}
	bool isDead = false;
	bool test(unsigned int x) { return (state & x); }
	void set(unsigned int x) { state |= x; }
	void unSet(unsigned int x) { state &= ~x; }

};

struct CKnife : public Component {
	bool isFiring{ false };
	sf::Time countdown{ sf::Time::Zero };
	int fireRate{ 1 };
	int spreadLevel{ 1 };

	CKnife() = default;
};