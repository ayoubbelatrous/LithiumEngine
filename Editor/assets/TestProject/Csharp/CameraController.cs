using System;
using Lithium.Core;
using Lithium.Math;
class CameraController : Script
{
    public Entity Target;
    public float Speed;

    void Start()
    {
    }
    float Lerp(float firstFloat, float secondFloat, float by)
    {
        return firstFloat * (1 - by) + secondFloat * by;
    }
    void Update()
    {
        Vector3 newPos;
        Vector3 Pos = entity.GetComponent<Transform>().Position;
        Vector3 targetPos = Target.GetComponent<Transform>().Position;
        newPos = new Vector3(Lerp(Pos.X, targetPos.X, Speed), Pos.Y, Pos.Z);
        entity.GetComponent<Transform>().Position = new Vector3(targetPos.X * -1.0f, newPos.Y, newPos.Z);
    }
}
