using System;
using Lithium.Core;
using Lithium.Math;
class Test : Script
{
   public int inttest = 27;
   Transform transform;
   void Start()
   {
        transform = entity.GetComponent<Transform>();
   }

   void Update()
   {
      
       Debug.Log("test is " + inttest + " transform is " + transform.Position.X);
   }
}
