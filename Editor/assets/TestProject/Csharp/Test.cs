﻿using System;

using System.Runtime.InteropServices;




[StructLayout(LayoutKind.Sequential)]
public struct Vector3
{
    public float X;
    public float Y;
    public float Z;
    public Vector3(float x,float y,float z)
    {
        X = x;
        Y = y;
        Z = z;
    }
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
    public static Vector3 operator +(Vector3 v1, Vector3 v2)
    {
        Vector3 temp = new Vector3();
        temp.X = v1.X + v2.X;
        temp.Y = v1.Y + v2.Y;
        return temp;
    }
    public override string ToString() {
        string str = "( X: " + X + " Y:" + Y + " Z: " + Z + " ) ";
        return str;
    }


}

class Test : Component
{
    public Vector3 test;
    void Start()
    {

    }

   void Update()
   { 
      Debug.Log("TEST : " + test);
   }
}
