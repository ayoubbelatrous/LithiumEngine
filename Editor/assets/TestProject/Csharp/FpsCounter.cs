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
        float frametime = (float)Time.deltaTime * 1000.0f;
        rend.Text = String.Format("{0}", frametime);

    }
  
    void OnCollisionEnter()
    {
        
    }

    void OnCollisionExit()
    {

    }
}

