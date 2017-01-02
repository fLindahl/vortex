#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#pragma once
#include <cassert>

namespace Util
{

template<class TYPE>
class LinkedList
{
public:
	struct Node
	{
		TYPE data;
		Node* next;
	};

	/// Creates the list with a dummy node
	LinkedList();

	/// Destucts the list and makes sure the entire content gets deleted
	~LinkedList();
	
	/// Checks if the node iterator is out of range
	bool IsOff();

	/// Retrieves the data at the current iterator position
	TYPE Item();

	/// Inserts an element before the iterator position
	void InsertBefore(TYPE i);

	/// Removes node at iterator position
	void RemoveAt();

	/// Moves the iterator forth one position
	void Forth();

	/// Moves the iterator to head
	void Start();


private:
	///Specifies the first node in the list
	Node* head;

	///The node we're currently on
	Node* nodeIter;

	///The node we were previously on
	Node* preNode;

};


template<class TYPE>
inline LinkedList<TYPE>::LinkedList()
{
	preNode = nullptr;
	head = new Node;
	head->data = -1;
	head->next = nullptr;
	this->nodeIter = head;
}

template<class TYPE>
inline LinkedList<TYPE>::~LinkedList()
{
	//Go through the entire list and make sure no nodes are missed!
	while (true)
	{
		this->preNode = this->nodeIter;
		this->nodeIter = this->preNode->next;

		delete this->preNode;

		//We've reached the end of the list.
		if (this->nodeIter == nullptr)
		{
			break;
		}
	}

}

template<class TYPE>
inline bool LinkedList<TYPE>::IsOff()
{
	if (this->nodeIter == nullptr)
		return true;
	else
		return false;
}

template<class TYPE>
inline TYPE LinkedList<TYPE>::Item()
{
	assert(!IsOff());
	
	return this->nodeIter->data;
		
}

template<class TYPE>
inline void LinkedList<TYPE>::InsertBefore(TYPE i)
{
	Node* newNode = new Node;
	newNode->data = i;
	
	if (this->preNode != nullptr)
		this->preNode->next = newNode;

	this->preNode = newNode;
	newNode->next = this->nodeIter;

	if (this->nodeIter == this->head)
	{
		this->head = newNode;
	}
}

template<class TYPE>
inline void LinkedList<TYPE>::RemoveAt()
{
	Node* delNode = this->nodeIter;
	this->nodeIter = this->nodeIter->next;
	
	if (this->preNode != nullptr)
	{
		this->preNode->next = this->nodeIter;
	}
	else
	{
		this->head = this->nodeIter;
	}

	delete delNode;
}

template<class TYPE>
inline void LinkedList<TYPE>::Forth()
{
	if (IsOff())
		return;

	this->preNode = this->nodeIter;
	this->nodeIter = this->nodeIter->next;
}

template<class TYPE>
inline void LinkedList<TYPE>::Start()
{
	this->preNode = nullptr;
	this->nodeIter = this->head;
}

}

#endif // LINKEDLIST_H