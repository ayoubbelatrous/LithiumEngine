using System;
using Lithium.Core;
using Lithium.Math;



class Test : Script
{

    void Start()
    {
        
    }

    void Update()
    {
        if(Input.IsKeyPressed(KeyCode.Space))
        Instantiate(entity);
    }

}
