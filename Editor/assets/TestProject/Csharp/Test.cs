using System;
using Lithium.Core;
using Lithium.Math;
class Test : Script
{
   public Transform transform;
   public float speed = 1.5f;
   public Vector3 test;

   void Start()
   {
        transform = entity.GetComponent<Transform>();
        Debug.Log("test is " + test.X);
   }

   void Update()
   {
        Debug.Log("test is " + test.X);

        if (Input.IsKeyPressed(KeyCode.W))
        {
            transform.Position = new Vector3(transform.Position.X, transform.Position.Y + 1 * speed * (float)Time.deltaTime, 0);
        }

        if (Input.IsKeyPressed(KeyCode.S))
        {
            transform.Position = new Vector3(transform.Position.X, transform.Position.Y - 1 * speed*(float)Time.deltaTime, 0);
        }

        if (Input.IsKeyPressed(KeyCode.A))
        {
            transform.Position = new Vector3(transform.Position.X - 1 * speed*(float)Time.deltaTime, transform.Position.Y, 0);
        }

        if (Input.IsKeyPressed(KeyCode.D))
        {
            transform.Position = new Vector3(transform.Position.X + 1 * speed*(float)Time.deltaTime, transform.Position.Y, 0);
        }
    }
}
