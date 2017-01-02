#pragma once

namespace Util
{


template<class TYPE>
class Queue
{
public:
	struct Node
	{
		TYPE data;
		Node* next;
	};

	/// Creates the list with a dummy node
	class Queue();

	// Destucts the list and makes sure the entire content gets deleted
	~Queue();
	
	/// Appends a node at the end of the list
	void Push(TYPE i);

	/// removes and returns the first element from the list
	TYPE Pop();
	
private:
	///Specifies the first node in the list
	Node* head;

	///The node we're currently on
	Node* nodeIter;

	///The node we were previously on
	Node* preNode;
};


template<class TYPE>
inline Queue<TYPE>::Queue()
{
	preNode = nullptr;
	head = new Node;
	head->data = -1;
	head->next = nullptr;
}

template<class TYPE>
inline Queue<TYPE>::~Queue()
{
	//Go through the entire list and make sure no nodes are missed!
	while (true)
	{
		this->preNode = this->nodeIter;

		if (preNode == nullptr)
		{
			return;
		}

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
inline void Queue<TYPE>::Push(TYPE i)
{
	Node* newNode = new Node;
	newNode->data = i;
	newNode->next = nullptr;

	this->nodeIter = this->head;
	//Loop until we find the first node that isn't linked to another node
	while (true)
	{
		if (this->nodeIter->next == nullptr)
		{
			this->nodeIter->next = newNode;
			return;
		}
		this->nodeIter = this->nodeIter->next;
	}
}

template<class TYPE>
inline typename TYPE Queue<TYPE>::Pop()
{
	this->nodeIter = this->head;

	if (this->head->next == nullptr)
	{
		//We did not find a matching element
		printf("Queue is empty.");
		return -1;
	}

	this->preNode = this->nodeIter;
	this->nodeIter = this->nodeIter->next;

	this->preNode->next = this->nodeIter->next;

	TYPE val = this->nodeIter->data;
	delete this->nodeIter;
	return val;	
}

}