using System;
using System.Runtime.CompilerServices;


namespace Lithium
{
    public class Entity
    {
        public uint ID { get; private set; }


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
                return _Component;
            }

            return null;
        }


        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void AddComponent_Internal(uint entityID,Type type); 
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static bool HasComponent_Internal(uint entityID,Type type);
    }
}
