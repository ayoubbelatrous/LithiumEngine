#pragma once

#include "Script/ScriptClass.h"
#include "Script/ScriptObject.h"
#include "Core/UUID.h"
#include <filesystem>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/object.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/environment.h>
#include <mono/metadata/attrdefs.h>

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
		MonoMethod* m_ExceptionMethod;
		char* _assemblyData;


		std::string _Path = "assets/TestProject/Assem/Csharp.dll";
		std::string _BinPath = "assets/TestProject/bin/Csharp.dll";
		std::filesystem::file_time_type m_LastAssemblyTime;
		std::unordered_map<std::string, std::string> m_AllClassesInImage;
		std::unordered_map<std::string, Ref<ScriptClass>> m_ScriptClassMap;

		void Bindinternals();
		void LoadAllClassesInImage();
		void Reload();
		void DeleteAssemblies();
		public:
			MonoServer();
			~MonoServer();

			void InitMono();

			//-----internal-calls------------------------------------------------------
			static void Log(MonoString* log);
			static bool HasComponent_Interal(uint64_t entityID, MonoObject* type);
			static bool AddComponent_Interal(uint64_t entityID, MonoObject* type);
			
			static bool HasScript_Interal(uint64_t entityID, MonoObject* type);
			static bool AddScript_Interal(uint64_t entityID, MonoObject* type);
			static MonoObject* GetScript_Internal(uint64_t entityID, MonoObject* type);
			static uint64_t CopyEntity_Internal(uint64_t entityID);
			//name component
			static MonoString* GetName_Internal(uint64_t entityID);
			static void SetName_Internal(uint64_t entityID, MonoString* name);
			//transform component
			static void SetPosition_Internal(uint64_t entityID, glm::vec3* vector);
			static void GetPosition_Internal(uint64_t entityID, glm::vec3* vector);
			static void SetRotation_Internal(uint64_t entityID, glm::vec3* vector);
			static void GetRotation_Internal(uint64_t entityID, glm::vec3* vector);
			static void SetScale_Internal(uint64_t entityID, glm::vec3* vector);
			static void GetScale_Internal(uint64_t entityID, glm::vec3* vector);


			//sprite renderer component
			static void SetColor_Internal(uint64_t entityID, glm::vec4* color);
			static void GetColor_Internal(uint64_t entityID, glm::vec4* color);
			
			//rigid body 2d component
			static void SetRigidbodyFixedRotation_Internal(uint64_t entityID, bool* fixedRotation);
			static bool GetRigidbodyFixedRotation_Internal(uint64_t entityID);

			static void SetRigidbodyVelocity_Internal(uint64_t entityID, glm::vec2* velocity);
			static void GetRigidbodyVelocity_Internal(uint64_t entityID, glm::vec2* velocity);

			//input
			static bool MouseKey_Internal(int button);
			static bool MouseKeyDown_Internal(int button);
			static void MousePosition_Internal(glm::vec2* pos);
			static bool KeyPressed_Internal(uint16_t button);
			static bool KeyPressedDown_Internal(uint16_t button);
			static double DeltaTime_Internal();
			//--------------------------------------------------------------------------
			void ForwardMonoException(MonoObject* object);
			static std::vector<const char*> _BufferLog;
			bool CheckForChange();
			bool CheckIfClassExists(const std::string& name);
			void ForceReload();
			Ref<Scene> m_ActiveScene;
			void SetActiveScene(const Ref<Scene>& scene);

			Ref<ScriptObject> GetObject(const std::string& name);
			Ref<ScriptObject> CopyObject(const Ref<ScriptObject>& object);
			void* CreateMonoEntity(UUID id);
			//Utils
			void* CreateMonoString(const char* str);
	};
	

}