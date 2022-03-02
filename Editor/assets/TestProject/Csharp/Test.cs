using System;
using Lithium.Core;
using Lithium.Math;
class Test : Script
{
   public int inttest = 24;


   void Start()
   {
   }

   void Update()
   {
        Debug.Log("test is " + inttest);


//         if (Input.IsKeyPressed(KeyCode.W))
//         {
//             transform.Position = new Vector3(transform.Position.X, transform.Position.Y + 1 * speed * (float)Time.deltaTime, 0);
//         }
// 
//         if (Input.IsKeyPressed(KeyCode.S))
//         {
//             transform.Position = new Vector3(transform.Position.X, transform.Position.Y - 1 * speed*(float)Time.deltaTime, 0);
//         }
// 
//         if (Input.IsKeyPressed(KeyCode.A))
//         {
//             transform.Position = new Vector3(transform.Position.X - 1 * speed*(float)Time.deltaTime, transform.Position.Y, 0);
//         }
// 
//         if (Input.IsKeyPressed(KeyCode.D))
//         {
//             transform.Position = new Vector3(transform.Position.X + 1 * speed*(float)Time.deltaTime, transform.Position.Y, 0);
//         }
    }
}
