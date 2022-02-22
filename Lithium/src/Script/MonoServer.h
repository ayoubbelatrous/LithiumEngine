#pragma once


#include "Core/Base.h"

namespace Lithium
{
	class MonoServer
	{
		MonoDomain* _MonoRootDomain;
		MonoDomain* _MonoAppDomain;
		MonoAssembly* _MonoAssembly;
		MonoImage* _MonoImage;
		std::string _Path = "assets/TestProject/Assem/Csharp.dll";
		
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

			bool Invoke()
			{

			}


	};
	

}