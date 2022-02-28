#pragma once

#include <filesystem>
#include "Script/ScriptClass.h"
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/object.h>
#include <mono/metadata/environment.h>
#include <mono/metadata/attrdefs.h>
#include "glm.hpp"
namespace Lithium
{
	class Scene;
	class MonoServer
	{
		MonoDomain* _MonoRootDomain = nullptr;
		MonoDomain* _MonoAppDomain = nullptr;
		MonoAssembly* _MonoAssembly = nullptr;
		MonoImage* _MonoImage = nullptr;
		MonoClass* _ScriptBaseClass;
		char* _assemblyData;
		std::string _Path = "assets/TestProject/Assem/Csharp.dll";
		std::string _BinPath = "assets/TestProject/bin/Csharp.dll";
		std::filesystem::file_time_type lastassemblytime;
		void Bindinternals();
		std::unordered_map<std::string, std::string> _AllClassesInImage;
		void LoadAllClassesInImage();
		public:
			MonoServer();
			~MonoServer();

			void InitMono();

			//internal calls
			static void Log(MonoString* log);
			static bool HasComponent_Interal(int entityID,MonoObject* type);
			static void SetPosition_Internal(int entityID, glm::vec3* vector);
			static void GetPosition_Internal(int entityID, glm::vec3* vector);
			static std::vector<const char*> _BufferLog;

			void Reload();
			void DeleteAssemblies();
			bool CheckForChange();
			bool CheckIfClassExists(const std::string& name);

			Ref<Scene> m_ActiveScene;
			Ref<ScriptClass> GetClass(const std::string& name);
			void SetActiveScene(const Ref<Scene>& scene);
			std::unordered_map<std::string, Ref<ScriptClass>> _ClassCache;
	};
	

}