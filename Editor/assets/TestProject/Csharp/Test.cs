using Lithium.Core;
using Lithium.Math;

class Test : Script
{
    public Entity Player;
    void Start()
    {
        Debug.Log("Player Entity Name is " + Player.GetComponent<NameComponent>().Tag);
    }

    void Update()
    {
       
    }

}
