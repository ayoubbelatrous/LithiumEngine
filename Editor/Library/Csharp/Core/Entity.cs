using System;
using System.Runtime.CompilerServices;


namespace Lithium.Core
{
    public class Entity
    {
        public UInt64 ID = 0;
        public Entity()
        {

        }
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

        public bool HasScript<T>() where T : Script, new()
        {
            return HasScript_Internal(ID, typeof(T));

        }
        public T GetScript<T>() where T : Script
        {
            return (T)GetScript_Internal(ID, typeof(T));
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static bool AddComponent_Internal(UInt64 entityID, Type type);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static bool HasComponent_Internal(UInt64 entityID, Type type);


        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static bool AddScript_Internal(UInt64 entityID, Type type);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static bool HasScript_Internal(UInt64 entityID, Type type);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static Script GetScript_Internal(UInt64 entityID, Type type);
    }
}
