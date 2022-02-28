using System;
using Lithium.Core;
using Lithium.Math;
class Test : Script
{
    public float speed;
   void Start()
   {
           
   }

   void Update()
   {
        speed+= 0.005f;
        entity.GetComponent<Transform>().Position = new Vector3(speed,0,0);
   }
}
