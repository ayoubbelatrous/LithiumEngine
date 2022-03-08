using System;
using Lithium.Core;
using Lithium.Math;
class Test : Script
{
    public int speed;
    public float acceleration;
    private SpriteRenderer renderer;
    public Vector4 Color1;
    public Vector4 Color2;
   void Start()
   {
        renderer = entity.GetComponent<SpriteRenderer>();
        Color1 = renderer.Color;
   }

    void Update()

    {
        if(Input.IsKeyPressed(KeyCode.Space))
        {
            renderer.Color = Color2;
        }
        else
        {
            renderer.Color = Color1;
        }
        
    }

}
