using System;
using System.Runtime.CompilerServices;


namespace Lithium.Core
{
    public class Entity
    {
        public UInt64 ID = 0;
        public Entity(UInt64 id) { ID = id; }
        public bool HasComponent<T>() where T : Component, new()
        {
            return HasComponent_Internal(ID, typeof(T));
        }
        public T AddComponent<T>() where T : Component, new()
        {
            AddComponent_Internal(ID, typeof(T));
            T _Component = new T();
            return _Component;
        }
        public T GetComponent<T>() where T : Component, new()
        {
            if (HasComponent<T>())
            {
                T _Component = new T();
                _Component.Entity = this;
                return _Component;
            }

            return null;
        }



        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void AddComponent_Internal(UInt64 entityID, Type type);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static bool HasComponent_Internal(UInt64 entityID, Type type);
    }
}
