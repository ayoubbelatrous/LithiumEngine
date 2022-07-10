using System;
using System.Runtime.InteropServices;

namespace Lithium.Math
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Vector4
    {
        public float X;
        public float Y;
        public float Z;
        public float W;

        public Vector4(float Scalar)
        {
            X = Scalar;
            Y = Scalar;
            Z = Scalar;
            W = Scalar;
        }

        public Vector4(float x, float y)
        {
            X = x;
            Y = y;
            Z = 0;
            W = 0;
        }

        public Vector4(float x, float y, float z)
        {
            X = x;
            Y = y;
            Z = z;
            W = 0;
        }

        public Vector4(float x, float y, float z,float w)
        {
            X = x;
            Y = y;
            Z = z;
            W = w;
        }

        public Vector4(Vector2 vector2, float z)
        {
            X = vector2.X;
            Y = vector2.Y;
            Z = z;
            W = 0;
        }

        public Vector4(Vector2 vector2, float z,float w)
        {
            X = vector2.X;
            Y = vector2.Y;
            Z = z;
            W = w;
        }

        public Vector4(Vector2 vector2)
        {
            X = vector2.X;
            Y = vector2.Y;
            Z = 0;
            W = 0;
        }

        public Vector4(Vector3 vector3)
        {
            X = vector3.X;
            Y = vector3.Y;
            Z = vector3.Z;
            W = 0;
        }

        public Vector4(Vector3 vector3, float w)
        {
            X = vector3.X;
            Y = vector3.Y;
            Z = vector3.Z;
            W = w;
        }
    }
}
