using System;
using Lithium.Mathf;
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
        public Quaternion Rotation
        {
            get
            {
                Quaternion res;
                GetRotation_Internal(Entity.ID, out res);
                return res;
            }
            set => SetRotation_Internal(Entity.ID, ref value);
        }
        public Vector3 Scale
        {
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
        extern public static void SetPosition_Internal(UInt64 entityID, ref Vector3 pos);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void GetPosition_Internal(UInt64 entityID, out Vector3 pos);


        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void SetRotation_Internal(UInt64 entityID, ref Quaternion rot);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void GetRotation_Internal(UInt64 entityID, out Quaternion rot);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void SetScale_Internal(UInt64 entityID, ref Vector3 sca);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static Vector3 GetScale_Internal(UInt64 entityID, out Vector3 sca);


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
                GetRigidbodyVelocity_Internal(Entity.ID, out res);
                return res;
            }
            set => SetRigidbodyVelocity_Internal(Entity.ID, ref value);
        }
        public void ApplyForce(Vector2 Force)
        {
            RigidBodyApplyForce_Internal(Entity.ID, ref Force);
        }

        public void ApplyAngularForce(float force)
        {
            RigidBodyApplyAngularForce_Internal(Entity.ID, force);
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void SetRigidbodyFixedRotation_Internal(UInt64 entityID, ref bool fixedRotation);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static bool GetRigidbodyFixedRotation_Internal(UInt64 entityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void SetRigidbodyVelocity_Internal(UInt64 entityID, ref Vector2 velocity);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void GetRigidbodyVelocity_Internal(UInt64 entityID, out Vector2 velocity);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void RigidBodyApplyForce_Internal(UInt64 entityID, ref Vector2 force);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void RigidBodyApplyAngularForce_Internal(UInt64 entityID, float force);
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

    public class AudioSource : Component
    {
        public void Play()
        {
            SetAudioSourcePlay_Internal(Entity.ID);
        }

        public void Play(AudioClip clip)
        {
            AudioSourcePlayClip_Internal(Entity.ID, clip.AssetId);
        }

        public bool Loop
        {
            get
            {
                return GetAudioSourceLoop_Internal(Entity.ID);
            }
            set => SetAudioSourceLoop_Internal(Entity.ID, value);
        }
        public float Gain
        {
            get
            {
                return GetAudioSourceGain_Internal(Entity.ID);
            }
            set => SetAudioSourceGain_Internal(Entity.ID, value);
        }
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void SetAudioSourcePlay_Internal(UInt64 entityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void SetAudioSourceLoop_Internal(UInt64 entityID, bool gain);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static bool GetAudioSourceLoop_Internal(UInt64 entityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void SetAudioSourceGain_Internal(UInt64 entityID, float gain);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static float GetAudioSourceGain_Internal(UInt64 entityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void AudioSourcePlayClip_Internal(UInt64 entityID, UInt64 assetid);

    }

    public class TextRenderer : Component
    {
        public String Text
        {
            get
            {
                String Stringval;
                GetTextRendererText_Internal(Entity.ID, out Stringval);
                return Stringval;
            }
            set => SetTextRendererText_Internal(Entity.ID, value);
        }
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void SetTextRendererText_Internal(UInt64 entityID, String text);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void GetTextRendererText_Internal(UInt64 entityID, out String text);
    }
    public class ParticleSystemRenderer : Component
    {
        public bool Play
        {
            get
            {
                return ParticleSystemRendererGetPlay_Internal(Entity.ID);
            }
            set => ParticleSystemRendererSetPlay_Internal(Entity.ID, value);
        }
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void ParticleSystemRendererSetPlay_Internal(UInt64 entityID, bool play);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static bool ParticleSystemRendererGetPlay_Internal(UInt64 entityID);
    }
}
