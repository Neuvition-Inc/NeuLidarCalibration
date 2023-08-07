#pragma once

#include <list>
class Subject;

class Observer
{
public:
	Observer();
	virtual ~Observer();
	virtual void update(Subject* subject) = 0;
};

class Subject
{

public:
	Subject();
	virtual ~Subject();

	//×¢²á
	virtual void attach(Observer* pObs);
	//×¢Ïú
	virtual void detach(Observer* pObs);
	//Í¨Öª
	virtual void notify();

private:
	std::list<Observer*>*  m_pObservers = nullptr;
};
