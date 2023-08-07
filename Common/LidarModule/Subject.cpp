#include "Subject.h"

Observer::Observer()
{

}

Observer::~Observer()
{

}

Subject::Subject()
{
	m_pObservers = new std::list<Observer*>;
}

Subject::~Subject()
{
}

void Subject::attach(Observer* pObs)
{
	if (nullptr == pObs)
	{
		return;
	}

	m_pObservers->push_front(pObs);
}

void Subject::detach(Observer* pObs)
{
	if (nullptr == pObs)
	{
		return;
	}

	m_pObservers->remove(pObs);
}

void Subject::notify()
{
	std::list<Observer*>::iterator  iter;
	for (iter = m_pObservers->begin(); iter != m_pObservers->end(); iter++)
	{
		(*iter)->update(this);
	}
}
