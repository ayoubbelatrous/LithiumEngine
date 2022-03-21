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
    public class Rigidbody2D : Component
    {

     
        public bool FixedRotation
        {
            get
            {
                return GetRigidbodyFixedRotation_Internal(Entity.ID);
            }
            set => SetRigidbodyFixedRotation_Internal(Entity.ID, ref value);
        }

        public Vector2 Velocity
        {
            get
            {
                Vector2 res;
                GetRigidbodyVelocity_Internal(Entity.ID,out res);
                return res;
            }
            set => SetRigidbodyVelocity_Internal(Entity.ID, ref value);
        }



     



        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void SetRigidbodyFixedRotation_Internal(UInt64 entityID, ref bool fixedRotation);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static bool GetRigidbodyFixedRotation_Internal(UInt64 entityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void SetRigidbodyVelocity_Internal(UInt64 entityID, ref Vector2 velocity);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void GetRigidbodyVelocity_Internal(UInt64 entityID,out Vector2 velocity);
    }

    public class Camera : Component
    {
        public enum CameraProjection : UInt16
        {
            Orthographic = 0,
            Perspective = 1,
        }

        public CameraProjection Projection
        {
            get
            {
                return GetCameraProjection_Internal(Entity.ID);
            }
            set => SetCameraProjection_Internal(Entity.ID, ref value);
        }
        public float OrthographicSize
        {
            get
            {
                
                return GetCameraOrthographicSize_Internal(Entity.ID);
            }
            set => SetCameraOrthographicSize_Internal(Entity.ID, ref value);
        }

        public bool Primary
        {
            get
            {
                return GetCameraPrimary_Internal(Entity.ID);
            }
            set => SetCameraPrimary_Internal(Entity.ID, ref value);
        }

        public bool FixedAspectRatio
        {
            get
            {
                return GetCameraFixedAspectRatio_Internal(Entity.ID);
            }
            set => SetCameraFixedAspectRatio_Internal(Entity.ID, ref value);
        }


        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void SetCameraProjection_Internal(UInt64 entityID, ref CameraProjection projection);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static CameraProjection GetCameraProjection_Internal(UInt64 entityID);



        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void SetCameraOrthographicSize_Internal(UInt64 entityID, ref float size);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static float GetCameraOrthographicSize_Internal(UInt64 entityID);



        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void SetCameraPrimary_Internal(UInt64 entityID, ref bool primary);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static bool GetCameraPrimary_Internal(UInt64 entityID);



        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void SetCameraFixedAspectRatio_Internal(UInt64 entityID, ref bool fixedaspectration);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static bool GetCameraFixedAspectRatio_Internal(UInt64 entityID);
    }
}
