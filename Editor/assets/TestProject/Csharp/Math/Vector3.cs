using System;
using System.Runtime.InteropServices;

namespace Lithium.Math
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Vector3
    {
        public float X ;
        public float Y ;
        public float Z ;

        public Vector3(float Scalar)
        {
            X = Scalar;
            Y = Scalar;
            Z = Scalar;
        }

        public Vector3(float x,float y)
        {
            X = x;
            Y = y;
            Z = 0;
        }

        public Vector3(float x, float y,float z)
        {
            X = x;
            Y = y;
            Z = z;
        }

        public Vector3(Vector2 vector2, float z)
        {
            X = vector2.X;
            Y = vector2.Y;
            Z = z;
        }

        public Vector3(Vector2 vector2)
        {
            X = vector2.X;
            Y = vector2.Y;
            Z = 0;
        }
    }
}
