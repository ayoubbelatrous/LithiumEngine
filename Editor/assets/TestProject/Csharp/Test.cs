using System;
using Lithium.Core;
using Lithium.Math;
class Test : Script
{
   public int inttest = 1;
   Transform transform;
   void Start()
   {
        Debug.Log("entity id is " + entity.ID);
        transform = entity.GetComponent<Transform>();
        Debug.Log("OnStart()");
        if(transform == null)
        {
            Debug.Log("transform is null");
        }
   }

   void Update()
   {
        transform.Position = new Vector3(inttest, 0, 0);
   }
}
