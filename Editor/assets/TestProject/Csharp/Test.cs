using System;
using Lithium.Core;
using Lithium.Math;
class Test : Script
{
   public int inttest = 1;
   public Transform transform;
   public Vector2 vec2;
   void Start()
   {
        transform = entity.GetComponent<Transform>();
   }

   void Update()
   {
        transform.Position = new Vector3(vec2);
   }
}
