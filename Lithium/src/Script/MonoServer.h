#pragma once

#include <filesystem>
#include "Script/ScriptClass.h"
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/object.h>
#include <mono/metadata/environment.h>
#include <mono/metadata/attrdefs.h>

namespace Lithium
{
	class MonoServer
	{
		MonoDomain* _MonoRootDomain = nullptr;
		MonoDomain* _MonoAppDomain = nullptr;
		MonoAssembly* _MonoAssembly = nullptr;
		MonoImage* _MonoImage = nullptr;
	
		char* _assemblyData;
		std::string _Path = "assets/TestProject/Assem/Csharp.dll";
		std::string _BinPath = "assets/TestProject/bin/Csharp.dll";



		std::filesystem::file_time_type lastassemblytime;

		void Bindinternals();
		public:
			MonoServer();
			~MonoServer();
	
			void InitMono();
			static void Log(MonoString* log);
			static std::vector<const char*> _BufferLog;
			void Reload();
			void DeleteAssemblies();
			Ref<ScriptClass> GetClass(const std::string& name);
			bool CheckForChange();
			std::unordered_map<std::string, Ref<ScriptClass>> _ClassCache;
	};
	

}