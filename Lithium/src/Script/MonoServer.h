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


		std::string AssemblyPath = "assets/TestProject/Library/Assembly.dll";
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

			//camera component
			static void SetCameraProjection_Internal(uint64_t entityID,int* projection);
			static int GetCameraProjection_Internal(uint64_t entityID);
			static void SetCameraOrthographicSize_Internal(uint64_t entityID, float* size);
			static float GetCameraOrthographicSize_Internal(uint64_t entityID);
			static void SetCameraPrimary_Internal(uint64_t entityID, bool *primary);
			static bool GetCameraPrimary_Internal(uint64_t entityID);
			static void SetCameraFixedAspectRatio_Internal(uint64_t entityID, bool* fixedaspectration);
			static bool GetCameraFixedAspectRatio_Internal(uint64_t entityID);
			//audio
			static void SetAudioSourcePlay_Internal(uint64_t entityID);

			static void SetAudioSourceLoop_Internal(uint64_t entityID,bool loop);
			static bool GetAudioSourceLoop_Internal(uint64_t entityID);

			static void SetAudioSourceGain_Internal(uint64_t entityID, float gain);
			static float GetAudioSourceGain_Internal(uint64_t entityID);

			static void AudioSourcePlayClip_Internal(uint64_t entityID, uint64_t assetID);
			//ui
			//--//text renderer
			static void SetTextRendererText_Internal(uint64_t entityID, MonoString* text);
			static void GetTextRendererText_Internal(uint64_t entityID, MonoString** text);
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
			void* CreateMonoCollsion2D(UUID EntityID);
			void* CreateMonoAudioClip(UUID assetID);
			//Utils
			void* CreateMonoString(const char* str);
			char* LoadAssemblyFile(const std::string& path,uint32_t* size);
	};
	

}