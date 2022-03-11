using System;
using Lithium.Core;
using Lithium.Math;
class Test : Script
{
    public float speed;
    public float speed2;
    public Entity Camera;

    void Start()
    {
        if(entity.HasScript<Player>())
        {

            Player player = entity.GetScript<Player>();
            player.Origin = this.entity;
        }

    }

    void Update()
    {
    }
}
