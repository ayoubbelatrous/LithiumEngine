using System;
using Lithium.Core;
using Lithium.Math;
class Pad : Script
{
    public float Speed = 1.0f;
    public Vector3 Velocity = new Vector3(0);

    void Start()
    {
    }

    void Update()
    {
        Transform transform = entity.GetComponent<Transform>();
        Velocity.X = GetAxialInput().X * (float)Time.deltaTime * Speed;
        Velocity.Y = GetAxialInput().Y * (float)Time.deltaTime * Speed;

        transform.Position = new Vector3(transform.Position.X + Velocity.X, transform.Position.Y + Velocity.Y);
    }

    private Vector2 GetAxialInput()
    {
        Vector2 AxialInput = new Vector2(0);

        if(Input.IsKeyPressed(KeyCode.W))
        {
            AxialInput.Y = 1.0f;
        }

        if (Input.IsKeyPressed(KeyCode.S))
        {
            AxialInput.Y = -1.0f;
        }

        if (Input.IsKeyPressed(KeyCode.D))
        {
            AxialInput.X = 1.0f;
        }

        if (Input.IsKeyPressed(KeyCode.A))
        {
            AxialInput.X = -1.0f;
        }

        return AxialInput;
    }
}
