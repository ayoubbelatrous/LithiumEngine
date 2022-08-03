using System;
using System.Runtime.CompilerServices;
using Lithium.Mathf;

namespace Lithium.Core
{

    struct PhysicsSpecification
    {
        Vector2 Gravity;
    }
    class Scene
    {
        public Scene(UInt64 ID)
        {
            SceneID = ID;
        }
        UInt64 SceneID;
        void SetPhysicsSpecificationt(PhysicsSpecification spec)
        {
            SetScenePhysicsSpecificationt_Internal(SceneID, spec);
        }
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void SetScenePhysicsSpecificationt_Internal(UInt64 ID, PhysicsSpecification spec);
    }

    class SceneManager
    {
        public Scene GetActiveScene()
        {
            UInt64 ID = GetActiveScene_Internal();
            return new Scene(ID);
        }
        public void LoadScene(int Index)
        {
            LoadScene_Internal(Index);
        }
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern UInt64 GetActiveScene_Internal();
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void LoadScene_Internal(int Index);
    }
}
