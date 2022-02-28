using System;
using Lithium.Core;
using Lithium.Math;
class Test2 : Script
{
    public double speed;
    void Start()
    {

    }

    void Update()
    {
        Random random = new Random();
        double rand = random.NextDouble();
        speed += 0.0002f;
        double sin = Math.Sin(speed);
        entity.GetComponent<Transform>().Position = new Vector3((float)sin / 2, -(float)(sin * rand * 0.2f));

    }
}
