using System;
using Lithium.Core;
using Lithium.Math;
class Pad : Script
{
    public Entity PhysicsEntity;
    public int timer = 0;
    void Start()
    {
        Instantiate(PhysicsEntity);
        Instantiate(PhysicsEntity);
        Instantiate(PhysicsEntity);
        Instantiate(PhysicsEntity);
        Instantiate(PhysicsEntity);
        Instantiate(PhysicsEntity);
        Instantiate(PhysicsEntity);
        Instantiate(PhysicsEntity);
        Instantiate(PhysicsEntity);
        Instantiate(PhysicsEntity);
        Instantiate(PhysicsEntity);
        Instantiate(PhysicsEntity);


        Instantiate(PhysicsEntity);
        Instantiate(PhysicsEntity);
        Instantiate(PhysicsEntity);
        Instantiate(PhysicsEntity);
        Instantiate(PhysicsEntity);
        Instantiate(PhysicsEntity);
        Instantiate(PhysicsEntity);
        Instantiate(PhysicsEntity);
        Instantiate(PhysicsEntity);
        Instantiate(PhysicsEntity);
        Instantiate(PhysicsEntity);
        Instantiate(PhysicsEntity);
    }

    void Update()
    {
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
