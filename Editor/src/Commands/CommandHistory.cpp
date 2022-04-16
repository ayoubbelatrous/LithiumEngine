#include "lipch.h"
#include "CommandHistroy.h"

namespace Lithium {

	Command* CommandHistory::m_Commands[1000] = {};
	int CommandHistory::m_CommandSize = 0;
	int CommandHistory::m_CommandPtr = 0;

	void CommandHistory::AddCommand(Command* command)
	{
		command->Excute();

		if (m_CommandPtr < m_CommandSize - 1)
		{
			for (int i = m_CommandSize - 1; i > m_CommandPtr; i--)
			{
				delete m_Commands[i];
			}
			m_CommandSize = m_CommandPtr + 1;
		}

		m_Commands[m_CommandSize] = command;
		m_CommandSize++;

		if (m_CommandSize > 1 && m_Commands[m_CommandSize - 1]->CanMerge() && m_Commands[m_CommandSize - 2]->CanMerge())
		{
			if (m_Commands[m_CommandSize - 1]->MergeWith(m_Commands[m_CommandSize - 2]))
			{
				delete m_Commands[m_CommandSize - 1];
				m_CommandSize--;
			}
		}

		m_CommandPtr = m_CommandSize - 1;
	}
	void CommandHistory::Undo()
	{
		if (m_CommandPtr >= 0)
		{
			m_Commands[m_CommandPtr]->Undo();
			m_CommandPtr--;
		}
	}

	void CommandHistory::Redo()
	{
		int redoCommand = m_CommandPtr + 1;
		if (redoCommand < m_CommandSize && redoCommand >= 0)
		{
			m_Commands[redoCommand]->Excute();
			m_CommandPtr++;
		}
	}

	void CommandHistory::SetNoMergeMostRecent()
	{
		if (m_CommandSize - 1 >= 0)
		{
			m_Commands[m_CommandSize - 1]->SetNoMerge();
		}
	}

}