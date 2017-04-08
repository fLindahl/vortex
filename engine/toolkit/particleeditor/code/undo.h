#ifndef _UNDO_H_
#define _UNDO_H_

#pragma once
#include <list>
#include <memory>

namespace Edit
{

class Command;
template <class T>
class CommandManagerT;


///Abstract class for a command
class Command
{
public:
	virtual bool Execute() = 0;		// do the command
	virtual bool Unexecute() = 0;	// undo the command
	virtual ~Command() { }			// free your resources in subclasses
};

template <class T = Command>
class CommandManagerT
{
// Data members
private:
	typedef std::list<std::shared_ptr<T>>	CommandList;
	CommandList m_undoList;
	CommandList m_redoList;
	size_t m_nUndoLevel;
	size_t m_nCleanCount;
	
	CommandManagerT() :
		m_nUndoLevel(100),
		m_nCleanCount(0)
	{ }

	~CommandManagerT()
	{
		Clear();
	}
public:
	static CommandManagerT* Instance()
	{
		static CommandManagerT instance;
		return &instance;
	}

	// C++ 11
	// Delete the methods we don't want.
	CommandManagerT(CommandManagerT const&) = delete;
	void operator=(CommandManagerT const&) = delete;

// Attributes
public:
	bool CanUndo() const
	{
		return (m_undoList.size() > 0);
	}

	bool CanRedo() const
	{
		return (m_redoList.size() > 0);
	}

	int getUndoLevel() const
	{
		return m_nUndoLevel;
	}

	void setUndoLevel(int newValue)
	{
		m_nUndoLevel = newValue;
	}

	bool IsDirty() const
	{
		return (m_nCleanCount != 0);
	}

	std::shared_ptr<T> getLastUndoCommand() const
	{
		return m_undoList.back();
	}

	std::shared_ptr<T> getLastRedoCommand() const
	{
		return m_redoList.back();
	}

// Methods
public:
	void DoCommand(std::shared_ptr<T> pCommand)
	{
		// Clear redo list
		ClearRedoList();
		// Execute the command and add it to undo list if succeeded
		if (pCommand->Execute())
			AddUndo(pCommand);
	}

	void Undo()
	{
		if (CanUndo())
		{
			m_nCleanCount--;
			std::shared_ptr<T> pCommand = m_undoList.back();
			m_undoList.pop_back();
			if (pCommand->Unexecute())
				AddRedo(pCommand);
		}
	}

	void Redo()
	{
		if (CanRedo())
		{
			m_nCleanCount++;
			std::shared_ptr<T> pCommand = m_redoList.back();
			m_redoList.pop_back();
			if (pCommand->Execute())
				AddUndo(pCommand);
		}
	}

	void Clear()
	{
		ClearUndoList();
		ClearRedoList();
	}

	void SetClean()
	{
		m_nCleanCount = 0;
	}

// Implementations
	void AddUndo(std::shared_ptr<T> pCommand)
	{
		if (m_undoList.size() >= m_nUndoLevel)
		{
			m_undoList.pop_front();
		}
		m_undoList.push_back(pCommand);
		if (m_nCleanCount < 0 && m_redoList.size() > 0)
			m_nCleanCount = m_undoList.size() + m_redoList.size() + 1;
		else
			m_nCleanCount++;
	}

	void AddRedo(std::shared_ptr<T> pCommand)
	{
		m_redoList.push_back(pCommand);
	}

	void ClearUndoList()
	{
		ClearCommandList(&m_undoList);
	}

	void ClearRedoList()
	{
		ClearCommandList(&m_redoList);
	}

	void ClearCommandList(CommandList* pList)
	{
		//CommandList::iterator it;
		//for (it = pList->begin(); it != pList->end(); it++)
		//	delete *it;
		pList->erase(pList->begin(), pList->end());
	}
};

typedef CommandManagerT<>	CommandManager;

} // namespace Edit

#endif // ifndef _UNDO_H_
