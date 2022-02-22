class Hello : Component
{
    public float test = 33;
   
}


class Test : Component
{
    Test()
    {
        Debug.Log("NONONO test");
    }
   public Hello test ;
    
   public void Start()
   {
   }

   public void Update(float dt)
   {
        System.Console.WriteLine(test.test * entityID);
        //Debug.Log("bla goo");
   }
}