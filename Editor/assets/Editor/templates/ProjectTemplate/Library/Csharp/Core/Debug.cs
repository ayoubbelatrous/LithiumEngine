﻿using System;
using System.Runtime.CompilerServices;


namespace Lithium.Core
{

    public class Debug
    {

        public static void OnException(Exception exc)
        {
            Debug.Log(exc.ToString());
        }
        public static void Log(string message, params object[] objects)
        {
            string _FinalMessage = message;
            foreach (var obj in objects)
                _FinalMessage += obj.ToString();
            Log(_FinalMessage);
        }

        public static void Log(params object[] objects)
        {
            string _FinalMessage = "";
            foreach (var obj in objects)
                _FinalMessage += obj.ToString();
            Log(_FinalMessage);
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void Log(String log);

    }


}