using Lithium.Core;
using Lithium.Math;

class Test : Script
{
    
    void Start()
    {
        
    }

    void Update()
    {
        if(Input.IsKeyDown(KeyCode.Space))
        {
            Debug.Log("entity");
        }
    }

}
