using System;
using System.Runtime.CompilerServices;
using System.CodeDom.Compiler;


public static class Debug
{

    
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    public extern static string Log(String log);

}

