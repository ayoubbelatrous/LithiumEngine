#pragma once

namespace Lithium
{
	class Command
	{
	public:
		virtual void Excute() = 0;
		virtual void Undo() = 0;
		virtual bool MergeWith(Command *other) = 0;
		void SetNoMerge() { m_CanMerge = false; }
		bool CanMerge() const { return m_CanMerge; }
	private:
		static uint64_t m_Id;
	protected:
		bool m_CanMerge = true;
	};
}