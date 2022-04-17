using System;
using Lithium.Core;
using Lithium.Math;
class FpsCounter : Script
{
	private float seconds;
	private float frames;
    private TextRenderer rend;
    void Start()
	{
        rend = entity.GetComponent<TextRenderer>();
    }

    void Update()
	{
        seconds += (float)Time.deltaTime;
        rend.Text = String.Format("{0}s", (int)seconds);

    }
  
    void OnCollisionEnter()
    {
        
    }

    void OnCollisionExit()
    {

    }
}

