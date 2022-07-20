using System;
using System.Runtime.CompilerServices;

namespace Lithium.Core
{
    public class Time
    {

        public static double deltaTime
        {
            get { return DeltaTime_Internal(); }
            private set { }
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static double DeltaTime_Internal();
    }
}
