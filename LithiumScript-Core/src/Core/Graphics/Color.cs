using System;
using Lithium.Mathf;

namespace Lithium.Core
{
    public class Color
    {
        public Color(Vector4 col)
        {
            R = col.X;
            G = col.Y;
            B = col.Z;
            A = col.W;
        }
        public Color(Vector3 col)
        {
            R = col.X;
            G = col.Y;
            B = col.Z;
            A = 1.0f;
        }

        public Color(Vector2 col)
        {
            R = col.X;
            G = col.Y;
            B = 0.0f;
            A = 1.0f;
        }
        public float R;
        public float G;
        public float B;
        public float A;
    }
}
