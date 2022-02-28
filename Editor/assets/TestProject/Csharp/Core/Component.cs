using System;
using Lithium.Math;
using System.Runtime.CompilerServices;

namespace Lithium.Core
{
    public abstract class Component
    {
        public Entity Entity { get; set; }
    }

    public class Transform : Component
    {
        
        public Vector3 Position
        {
            get
            {
                Vector3 res;
                GetPosition_Internal(Entity.ID, out res);
                return res;
            }
            set => SetPosition_Internal(Entity.ID, ref value);

        }
        public Vector3 Rotation
        {
            get
            {
                Vector3 res;
                GetRotation_Internal(Entity.ID, out res);
                return res;
            }
            set => SetRotation_Internal(Entity.ID, ref value);
        }
        public Vector3 Scale {
            get
            {
                Vector3 res;
                GetScale_Internal(Entity.ID, out res);
                return res;
            }
            set => SetScale_Internal(Entity.ID, ref value);
        }
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void SetPosition_Internal(int entityID,ref Vector3 pos);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void GetPosition_Internal(int entityID,out Vector3 pos);


        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void SetRotation_Internal(int entityID, ref Vector3 pos);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static Vector3 GetRotation_Internal(int entityID, out Vector3 pos);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void SetScale_Internal(int entityID, ref Vector3 pos);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static Vector3 GetScale_Internal(int entityID, out Vector3 pos);
    }

    public class SpriteRenderer : Component
    {
    }

    public class NameComponent : Component
    {
    }



    
}
