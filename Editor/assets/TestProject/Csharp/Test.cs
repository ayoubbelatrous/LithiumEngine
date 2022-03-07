using System;
using Lithium.Core;
using Lithium.Math;
class Test : Script
{

   public string playerName = "player1";
    private Transform transform;
   void Start()
   {
        transform = entity.GetComponent<Transform>();
        transform.Position = new Vector3(10, 0, 0);
   }

   void Update()
   {
       
   }

}
