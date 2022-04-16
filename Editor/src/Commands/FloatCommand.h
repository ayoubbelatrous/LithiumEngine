#pragma once
#include "epch.h"
#include "Command.h"


namespace Lithium
{
	class FloatCommand : public Command
	{
	public:
		FloatCommand(float& originalFloat, float newFloat)
			: m_Float(originalFloat), m_NewFloat(newFloat), m_OldFloat(0)
		{
		}

		void Excute() override {
			m_OldFloat = m_Float;
			m_Float = m_NewFloat;
		}
		virtual void Undo() override
		{
			m_Float = m_OldFloat;
		}
		virtual bool MergeWith(Command *other) override
		{
			FloatCommand* changeFloatCommand = dynamic_cast<FloatCommand*>(other);
			if (changeFloatCommand != nullptr)
			{
				if (&changeFloatCommand->m_Float == &this->m_Float)
				{
					changeFloatCommand->m_NewFloat = this->m_NewFloat;
					return true;
				}
			}

			return false;
		}
	private:
		float& m_Float;
		float m_NewFloat;
		float m_OldFloat;
	};
}