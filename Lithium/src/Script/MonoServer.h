#pragma once


#include "Core/Base.h"
#include "Script/ScriptClass.h"
#include <filesystem>

namespace Lithium
{
	class MonoServer
	{
		MonoDomain* _MonoRootDomain;
		MonoDomain* _MonoAppDomain;
		MonoAssembly* _MonoAssembly;
		MonoImage* _MonoImage;
	
		char* _assemblyData;
		std::string _Path = "assets/TestProject/Assem/Csharp.dll";
		std::string _BinPath = "assets/TestProject/bin/Csharp.dll";



		std::filesystem::file_time_type lastassemblytime;

		void Bindinternals();
		public:
			MonoServer();
			~MonoServer();
			void InitMono();
			inline MonoDomain* GetRootDomain()
				const
			{
				return _MonoRootDomain;
			}
			inline MonoDomain* GetAppDomain()
				const
			{
				return _MonoAppDomain;
			}
			//Set Mono Assembly Path
			inline std::string SetPath(const std::string& path)
				const
			{
				return path;
			}

			inline std::string GetPath()
			{
				return _Path;
			}


			void Reload();
			void DeleteAssemblies();
			Ref<ScriptClass> GetClass(const std::string& name);
			bool CheckForChange();
			std::unordered_map<std::string, Ref<ScriptClass>> _ClassCache;
			
	};
	

}