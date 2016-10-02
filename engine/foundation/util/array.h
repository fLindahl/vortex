#pragma once

template<class TYPE>
class Array
{
public:
	/// define element iterator
	typedef TYPE* Iterator;

    /// default constructor
    Array();
    /// constructor with capacity
    Array(size_t s);
    /// copy constructor
    Array(const Array<TYPE>& rhs);
    /// destructor
    ~Array();
    /// assignment operator
    void operator=(const Array<TYPE>& rhs);
    /// write [] operator
	TYPE& operator[](size_t index) const;
    /// equality operator
    bool operator==(const Array<TYPE>& rhs) const;
    /// inequality operator
    bool operator!=(const Array<TYPE>& rhs) const;

    /// get number of filled elements
    size_t size() const;
    /// get total capacity of array
    size_t Capacity() const;
    
    //Returns a new element and edits index variable to be the index of said element
    TYPE& NewElement(size_t& outIndex);
    // Without index
    TYPE& NewElement();


    /// resize array without deleting existing content
    void Resize(size_t newSize);
    /// return true if array if empty (has no elements)
    bool IsEmpty() const;
    /// clear the array, free elements
    void Clear();
	
    /// Appends the value to the end of the list. Resizes list to the double if necessary
    void Append(const TYPE& value);

    void Fill(const TYPE& val);

    /// get iterator to first element
    Iterator begin() const;
    /// get iterator past last element
    Iterator end() const;    

private:
    /// delete content
    void Delete();
    /// allocate array for given capacity (deletes content)
	void Alloc(size_t s);
    /// copy content
    void Copy(const Array<TYPE>& src);

	size_t _capacity;
	size_t _size;

    TYPE* elements;
};

template<class TYPE>
Array<TYPE>::Array() :
    _capacity(0),
    elements(0)
{
    // empty
}

template<class TYPE> TYPE&
Array<TYPE>::NewElement()
{
	// grow allocated space if exhausted
	if (this->_size == this->_capacity)
	{
		//HACK: This should have an upper limit cause we can run out of memory quickly
		this->Resize(_capacity * 2);
	}

	return this->elements[this->_size++];
}

template<class TYPE> TYPE&
Array<TYPE>::NewElement(size_t& outIndex)
{
	// grow allocated space if exhausted
	if (this->_size == this->_capacity)
	{
		//HACK: This should have an upper limit cause we can run out of memory quickly
		this->Resize(_capacity * 2);
	}

	outIndex = this->_size++;

	return this->elements[outIndex];
}

template<class TYPE> void
Array<TYPE>::Append(const TYPE& value)
{
	// grow allocated space if exhausted
	if (this->_size == this->_capacity)
	{
		//HACK: This should have an upper limit cause we can run out of memory quickly
		this->Resize(_capacity * 2);
	}

	this->elements[this->_size++] = value;
}

template<class TYPE> void
Array<TYPE>::Delete()
{
    if (this->elements)
    {
        delete[] this->elements;
        this->elements = 0;
    }
    this->_capacity = 0;
}

template<class TYPE> void
Array<TYPE>::Alloc(size_t s)
{
    if (s > 0)
    {
		delete[] this->elements;
        this->elements = new TYPE[s];
    }
    this->_capacity = s;
	this->_size = 0;
}

template<class TYPE> void
Array<TYPE>::Copy(const Array<TYPE>& rhs)
{
    if (this != &rhs)
    {
        this->Alloc(rhs._capacity);
		size_t i;
        for (i = 0; i < this->_capacity; i++)
        {
            this->elements[i] = rhs.elements[i];
        }
		this->_size = rhs._size;
    }

}

template<class TYPE>
Array<TYPE>::Array(size_t s) :
    _capacity(0),
    elements(0)
{
    this->Alloc(s);
}

template<class TYPE>
Array<TYPE>::Array(const Array<TYPE>& rhs) :
    _capacity(0),
    elements(0)
{
    this->Copy(rhs);
}

template<class TYPE>
Array<TYPE>::~Array()
{
    this->Delete();
}

template<class TYPE> void
Array<TYPE>::operator=(const Array<TYPE>& rhs)
{
    this->Delete();
    this->Copy(rhs);
}

template<class TYPE> TYPE&
Array<TYPE>::operator[](size_t index) const
{
    return this->elements[index];
}

template<class TYPE> bool
Array<TYPE>::operator==(const Array<TYPE>& rhs) const
{
    if (this->_capacity != rhs.capacity)
    {
        return false;
    }
    else
    {
		size_t i;
		size_t num = this->_capacity;
        for (i = 0; i < num; i++)
        {
            if (this->elements[i] != rhs.elements[i])
            {
                return false;
            }
        }
        return true;
    }
}

template<class TYPE> bool
Array<TYPE>::operator!=(const Array<TYPE>& rhs) const
{
    return !(*this == rhs);
}

template<class TYPE> void
Array<TYPE>::Resize(size_t newSize)
{
    // allocate new array and copy over old elements
    TYPE* newElements = 0;
    if (newSize > 0)
    {
        newElements = new TYPE[newSize];
		size_t numCopy = this->_capacity;
        if (numCopy > newSize) numCopy = newSize;
		size_t i;
        for (i = 0; i < numCopy; i++)
        {
            newElements[i] = this->elements[i];
        }
    }

    // delete old elements
    this->Delete();

    // set content to new elements
    this->elements = newElements;
    this->_capacity = newSize;
}

template<class TYPE> size_t
Array<TYPE>::Capacity() const
{
    return this->_capacity;
}

template<class TYPE> size_t
Array<TYPE>::size() const
{
	return this->_size;
}

template<class TYPE> bool
Array<TYPE>::IsEmpty() const
{
    return 0 == this->_size;
}

template<class TYPE> void
Array<TYPE>::Clear()
{
    this->Delete();
}

template<class TYPE> void
Array<TYPE>::Fill(const TYPE& val)
{
    size_t i;
    for (i = 0; i < this->_capacity; i++)
    {
        this->elements[i] = val;
    }
	this->_size = this->_capacity;
}       

template<class TYPE> typename Array<TYPE>::Iterator
Array<TYPE>::begin() const
{
	return this->elements;
}

template<class TYPE> typename Array<TYPE>::Iterator
Array<TYPE>::end() const
{
	return this->elements + this->_size;
}