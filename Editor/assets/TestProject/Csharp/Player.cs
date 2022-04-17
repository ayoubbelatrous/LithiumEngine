using System;
using Lithium.Core;
using Lithium.Math;
using Lithium.Physics;
class Player : Script
{
    public float Speed = 5.0f;
    private Rigidbody2D RigidBody;
    void Start()
	{
        RigidBody = entity.GetComponent<Rigidbody2D>();
    }

    void Update()
	{

    }


    void OnCollisionEnter(Collision2D collision)
    {
        Debug.Log(collision.entity.GetComponent<NameComponent>().Tag);
    }

    void OnCollisionExit()
    {
        Debug.Log("haha");
    }
}

