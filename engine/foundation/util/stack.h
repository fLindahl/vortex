#pragma once

namespace Util
{

template<class TYPE>
class Stack
{
public:
	struct Node
	{
		TYPE data;
		Node* next;
	};

	/// Creates the list with a dummy node
	class Stack();

	// Destucts the list and makes sure the entire content gets deleted
	~Stack();
	
	/// Appends a node at the end of the list
	void Push(TYPE i);

	/// removes the last node from the list and returns its data
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
inline Stack<TYPE>::Stack()
{
	preNode = nullptr;
	head = new Node;
	head->data = -1;
	head->next = nullptr;
}

template<class TYPE>
inline Stack<TYPE>::~Stack()
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
inline void Stack<TYPE>::Push(TYPE i)
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
inline typename TYPE Stack<TYPE>::Pop()
{
	this->nodeIter = this->head;

	if (this->head->next == nullptr)
	{
		//We did not find a matching element
		printf("Stack is empty.");
		return -1;
	}

	//Loop until we find the first node that isn't linked to another node
	while (this->nodeIter->next != nullptr)
	{
		this->preNode = this->nodeIter;
		this->nodeIter = this->nodeIter->next;
	}

	this->preNode->next = nullptr;

	TYPE val = this->nodeIter->data;
	delete this->nodeIter;
	return val;
}

}