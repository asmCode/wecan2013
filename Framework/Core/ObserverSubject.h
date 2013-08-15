#pragma once

#include <list>

template <typename T>
class ObserversContainer
{
	//friend class ObserverSubject;
	
public:
	std::list<T> observers;

	void AddObserver(T observer)
	{
		observers.push_back(observer);
	}
	
	void RemoveObserver(T observer)
	{
		typename std::list<T>::iterator it;
		for (it = observers.begin(); it != observers.end(); it++)
			if (*it == observer)
				break;
		
		if (it != observers.end())
			observers.erase(it);
	}
	
	template <typename Method>
	void NotifyObservers(Method methodPointer)
	{
		typename std::list<T>::iterator it;
		for (it = observers.begin(); it != observers.end(); it++)
			((*it) ->*methodPointer)();
	}
	
	template <typename Method, typename Arg1>
	void NotifyObservers(Method methodPointer, Arg1 a1)
	{
		typename std::list<T>::iterator it;
		for (it = observers.begin(); it != observers.end(); it++)
			((*it) ->*methodPointer)(a1);
	}

	template <typename Method, typename Arg1, typename Arg2>
	void NotifyObservers(Method methodPointer, Arg1 a1, Arg2 a2)
	{
		typename std::list<T>::iterator it;
		for (it = observers.begin(); it != observers.end(); it++)
			((*it) ->*methodPointer)(a1, a2);
	}
	
	template <typename Method, typename Arg1, typename Arg2, typename Arg3>
	void NotifyObservers(Method methodPointer, Arg1 a1, Arg2 a2, Arg3 a3)
	{
		typename std::list<T>::iterator it;
		for (it = observers.begin(); it != observers.end(); it++)
			((*it) ->*methodPointer)(a1, a2, a3);
	}
};

//class ObserverSubject
//{
//public:
//	template <typename T>
//	void AddObserver(T observer)
//	{
//		((ObserversContainer<T>*)this) ->_AddObserver(observer);
//	}
//
//	template <typename T>
//	void _RemoveObserver(T observer)
//	{
//		((ObserversContainer<T>*)this) ->_RemoveObserver(observer);
//	}
//	
//	template <typename T, typename Method>
//	void NotifyObservers(Method action)
//	{
//		((ObserversContainer<T>*)this) ->_NotifyObservers(action);
//	}
//	
//	template <typename T, typename Method, typename Arg1>
//	void NotifyObservers(Method action, Arg1 a1)
//	{
//		((ObserversContainer<T>*)this) ->_NotifyObservers(action, a1);
//	}
//	
//	template <typename T, typename Method, typename Arg1, typename Arg2>
//	void NotifyObservers(Method action, Arg1 a1, Arg2 a2)
//	{
//		((ObserversContainer<T>*)this) ->_NotifyObservers(action, a1, a2);
//	}
//	
//	template <typename T, typename Method, typename Arg1, typename Arg2, typename Arg3>
//	void NotifyObservers(Method action, Arg1 a1, Arg2 a2, Arg3 a3)
//	{
//		((ObserversContainer<T>*)this) ->_NotifyObservers(action, a1, a2, a3);
//	}
//};

#define ObsCast(type, ptr) ((ObserversContainer<type*>*)ptr)