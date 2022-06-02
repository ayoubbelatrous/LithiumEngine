using System;
using Lithium.Core;
using Lithium.Math;
using Lithium.Physics;
class Player : Script
{
    void Start()
	{
        Texture texture = new Texture(128, 128, 1);
        Vector4[] data = new Vector4[100 * 128];
        for(int i = 0;  i < data.Length;i++)
        {
            data[i].X = 10;
            data[i].Y = 10;
            data[i].Z = 10;
            data[i].W = 10;
            Debug.Log(data[i].X, data[i].Y, data[i].Z);
        }
        
        texture.SetData(data);
    }

    void Update()
	{

    }


    void OnCollisionEnter(Collision2D collision)
    {
        
    }

    void OnCollisionExit()
    {
    }
}

