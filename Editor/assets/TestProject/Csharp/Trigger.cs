using System;
using Lithium.Core;
using Lithium.Math;
class Trigger : Script
{
    
    void Start()
    {
    }

    void Update()
    {
    }

    void OnCollisionEnter()
    {
        Debug.Log("collision from sensor");
    }

    void OnCollisionExit()
    {
        Debug.Log("collision exit from sensor");
    }
}


