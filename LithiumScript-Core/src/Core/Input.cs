using System;
using System.Runtime.CompilerServices;

using Lithium.Mathf;
namespace Lithium.Core
{
    public class Input
    {



        public static Vector2 Mouse
        {
            get
            {
                Vector2 res;
                MousePosition_Internal(out res);
                return res;
            }
            private set { }
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static bool MouseKeyPressed(int keycode);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static bool MouseKeyDown(int keycode);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static bool IsKeyPressed(KeyCode keycode);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static bool IsKeyDown(KeyCode keycode);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void MousePosition_Internal(out Vector2 pos);
    }
}
