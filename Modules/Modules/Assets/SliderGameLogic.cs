using UnityEngine;
using System.Collections;

public class SliderGameLogic : MonoBehaviour {

    private float[] sliderPercentages = new float[3];


    public void UpdateSlider(int index, float percentage)
    {
        sliderPercentages[index] = percentage;
    }

	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
	    for(int i = 0; i < 10; ++i)
        {
            //Do LED shit
        }
	}
}
