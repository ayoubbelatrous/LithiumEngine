using System;
using System.Runtime.InteropServices;

namespace Lithium.Math
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Vector2
    {
        public float X;
        public float Y;

        public Vector2(float Scalar)
        {
            X = Scalar;
            Y = Scalar;

        }

        public Vector2(float x, float y)
        {
            X = x;
            Y = y;
        }
    }
}
