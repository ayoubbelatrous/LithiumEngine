using System;
using Lithium.Core;
using Lithium.Math;
class Test : Script
{

    public int speed = 100;
    public int speed2 = 99;

   void Start()
   {


   }

   void Update()
   {
        if(Input.IsKeyPressed(KeyCode.Space))
        {
            Debug.Log("hi");
        }
   }

}
