using System;
using Lithium.Core;
using Lithium.Math;
class CameraController : Script
{
    public Entity Target;
    private Transform transform;
    private Transform TargetTransform;
    void Start()
    {
        transform = entity.GetComponent<Transform>();
        TargetTransform = Target.GetComponent<Transform>();
    }

    void Update()
    {
        transform.Position = new Vector3(TargetTransform.Position.X, transform.Position.Y, 0.0f);
    }
 
    void OnCollisionEnter()
    {

    }

    void OnCollisionExit()
    {

    }
}

