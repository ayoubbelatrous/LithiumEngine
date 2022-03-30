using System;
using Lithium.Core;
using Lithium.Math;
class CameraController : Script
{
    private float Speed = 10.0f;
    void Start()
    {

    }

    void Update()
    {
        Vector3 vel = entity.GetComponent<Transform>().Position;


        entity.GetComponent<Transform>().Position = new Vector3(Speed * GetAxialInput().X, Speed * GetAxialInput().X);
        if(Input.MouseKeyPressed(0))
        {
            entity.GetComponent<Camera>().OrthographicSize = entity.GetComponent<Camera>().OrthographicSize + 1;
        }
        else if(Input.MouseKeyPressed(1))
        {
            entity.GetComponent<Camera>().OrthographicSize = entity.GetComponent<Camera>().OrthographicSize - 1;
        }

    }
    private Vector2 GetAxialInput()
    {
        Vector2 AxialInput = new Vector2(0);

        if (Input.IsKeyPressed(KeyCode.W))
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

    void OnCollisionEnter()
    {

    }

    void OnCollisionExit()
    {

    }
}

