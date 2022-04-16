#pragma once
#include "Command.h"

namespace Lithium
{
	class CommandHistory
	{
	public:
		static void AddCommand(Command *command);
		static void Undo();
		static void Redo();
		static void SetNoMergeMostRecent();
	private:
		static Command* m_Commands[1000];
		static int m_CommandSize;
		static int m_CommandPtr;
	};

}
	