using System;
using Lithium.Math;
using System.Runtime.CompilerServices;

namespace Lithium.Core
{

    
    public abstract class Component
    {
        public Entity Entity { get; set; }
    }
    public class NameComponent : Component
    {
        public String Tag
        {
            get
            {
                return GetName_Internal(Entity.ID);
            }
            set => SetName_Internal(Entity.ID, value);
        }


        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void SetName_Internal(UInt64 entityID, String name);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static String GetName_Internal(UInt64 entityID);

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
        //Transform Comp
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void SetPosition_Internal(UInt64 entityID,ref Vector3 pos);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void GetPosition_Internal(UInt64 entityID,out Vector3 pos);


        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void SetRotation_Internal(UInt64 entityID, ref Vector3 pos);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static Vector3 GetRotation_Internal(UInt64 entityID, out Vector3 pos);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void SetScale_Internal(UInt64 entityID, ref Vector3 pos);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static Vector3 GetScale_Internal(UInt64 entityID, out Vector3 pos);


    }

    public class SpriteRenderer : Component
    {
        public Texture MainTexture;
        public Vector4 Color
        {
            get
            {
                Vector4 res;
                GetColor_Internal(Entity.ID, out res);
                return res;
            }
            set => SetColor_Internal(Entity.ID, ref value);
        }
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void SetColor_Internal(UInt64 entityID, ref Vector4 color);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void GetColor_Internal(UInt64 entityID, out Vector4 color);
    }
    
}
