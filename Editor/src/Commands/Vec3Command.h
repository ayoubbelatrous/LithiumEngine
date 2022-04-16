#pragma once
#include "Command.h"
#include "glm.hpp"



namespace Lithium
{
	class Vec3Command : public Command
	{
	public:
		Vec3Command(glm::vec3& ValueRef, glm::vec3 newVec3)
			: m_Vec3(ValueRef), m_NewVec3(newVec3), m_OldVec3(0)
		{
		}

		void Excute() override {
			m_OldVec3 = m_Vec3;
			m_Vec3 = m_NewVec3;
		}
		virtual void Undo() override
		{
			m_Vec3 = m_OldVec3;
		}
		virtual bool MergeWith(Command* other) override
		{
			Vec3Command* changeFloatCommand = dynamic_cast<Vec3Command*>(other);
			if (changeFloatCommand != nullptr)
			{
				if (&changeFloatCommand->m_Vec3 == &this->m_Vec3)
				{
					changeFloatCommand->m_NewVec3 = this->m_NewVec3;
					return true;
				}
			}

			return false;
		}
	private:
		glm::vec3& m_Vec3;
		glm::vec3 m_NewVec3;
		glm::vec3 m_OldVec3;
	};
}