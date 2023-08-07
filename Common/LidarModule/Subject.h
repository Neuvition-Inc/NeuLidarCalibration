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

	//ע��
	virtual void attach(Observer* pObs);
	//ע��
	virtual void detach(Observer* pObs);
	//֪ͨ
	virtual void notify();

private:
	std::list<Observer*>*  m_pObservers = nullptr;
};
