using System;
using Lithium.Core;
using Lithium.Math;
class Test : Script
{

    public int speed = 100;
    public int speed2 = 99;

   void Start()
   {
        entity.GetComponent<SpriteRenderer>().Color = new Vector4(1, 0, 1, 1);
   }

   void Update()
   {
        if(Input.IsKeyPressed(KeyCode.Space))
        {
            Debug.Log("hi");
        }
   }

}
