using System;
using Lithium.Core;
class Test : Script
{
    
   void Start()
   {
        if(entity.HasComponent<Transform>())
        {
            Debug.Log("yes it does");
        }
   }

   void Update()
   {
   }
}
