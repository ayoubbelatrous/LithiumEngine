using System;
using Lithium.Core;
using Lithium.Math;
class Player : Script
{
	public float speed = 0.0f;
	public float AirSpeed = 0.0f;
	public float JumpSpeed = 0.0f;
    public bool Grounded = false;

    void Start()
	{
	}

	void Update()
	{
		Vector2 vel = entity.GetComponent<Rigidbody2D>().Velocity;

   
        if(Input.IsKeyPressed(KeyCode.Space) && Grounded)
        {
            vel.Y = speed;
        }
        if(Grounded)
        {
            entity.GetComponent<Rigidbody2D>().Velocity = new Vector2(speed * GetAxialInput().X, vel.Y);

        }
        else
        {
            entity.GetComponent<Rigidbody2D>().Velocity = new Vector2(AirSpeed * GetAxialInput().X, vel.Y);
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
        entity.GetComponent<SpriteRenderer>().Color = new Vector4(1, 0, 0, 1);
        Grounded = true;
    }

    void OnCollisionExit()
    {
        entity.GetComponent<SpriteRenderer>().Color = new Vector4(1);
        Grounded = false;

    }
}

