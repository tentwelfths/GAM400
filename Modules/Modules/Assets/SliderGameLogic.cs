using UnityEngine;
using System.Collections;

public class SliderGameLogic : MonoBehaviour {

    private float[] sliderPercentages = new float[3];
    private int[] sliderSet = new int[3];

    public void UpdateSlider(int index, float percentage)
    {
        sliderPercentages[index] = percentage;
    }

	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
        for(int i = 0; i < 3; ++i)
        {
            if(sliderPercentages[i] <= 0.25)
            {
                sliderSet[i] = 1;
            }
            else if(sliderPercentages[i] > 0.25 && sliderPercentages[i] <= 0.5)
            {
                sliderSet[i] = 2;
            }
            else if (sliderPercentages[i] > 0.5 && sliderPercentages[i] <= 0.75)
            {
                sliderSet[i] = 3;
            }
            else if (sliderPercentages[i] > 0.75 && sliderPercentages[i] <= 1.0)
            {
                sliderSet[i] = 4;
            }
        }
        bool[] leds = new bool[10];
        for (int i = 0; i < 10; ++i)
        {
            //Do LED shit
            switch (i)
            {
                case 0:
                    if (sliderSet[0] == 1)
                    {
                        leds[i] = !leds[i];
                    }
                    if(sliderSet[1] == 4)
                    {
                        leds[i] = !leds[i];
                    }
                    if(sliderSet[2] == 3)
                    {
                        leds[i] = !leds[i];
                    }
                    break;
                case 1:
                    if (sliderSet[0] == 4)
                    {
                        leds[i] = !leds[i];
                    }
                    if (sliderSet[1] == 3)
                    {
                        leds[i] = !leds[i];
                    }
                    if (sliderSet[2] == 1)
                    {
                        leds[i] = !leds[i];
                    }
                    break;
                case 2:
                    if (sliderSet[0] == 2)
                    {
                        leds[i] = !leds[i];
                    }
                    if (sliderSet[1] == 1)
                    {
                        leds[i] = !leds[i];
                    }
                    if (sliderSet[2] == 1)
                    {
                        leds[i] = !leds[i];
                    }
                    break;
                case 3:
                    if (sliderSet[0] == 3)
                    {
                        leds[i] = !leds[i];
                    }
                    if (sliderSet[1] == 2)
                    {
                        leds[i] = !leds[i];
                    }
                    if (sliderSet[2] == 4)
                    {
                        leds[i] = !leds[i];
                    }
                    break;
                case 4:
                    if (sliderSet[1] == 1)
                    {
                        leds[i] = !leds[i];
                    }
                    else if (sliderSet[1] == 3)
                    {
                        leds[i] = !leds[i];
                    }
                    break;
                case 5:
                    if (sliderSet[1] == 2)
                    {
                        leds[i] = !leds[i];
                    }
                    else if (sliderSet[1] == 4)
                    {
                        leds[i] = !leds[i];
                    }
                    break;
                case 6:
                    if (sliderSet[0] == 2)
                    {
                        leds[i] = !leds[i];
                    }
                    if (sliderSet[1] == 4)
                    {
                        leds[i] = !leds[i];
                    }
                    if (sliderSet[2] == 1)
                    {
                        leds[i] = !leds[i];
                    }
                    break;
                case 7:
                    if (sliderSet[0] == 1)
                    {
                        leds[i] = !leds[i];
                    }
                    if (sliderSet[1] == 1)
                    {
                        leds[i] = !leds[i];
                    }
                    if (sliderSet[2] == 4)
                    {
                        leds[i] = !leds[i];
                    }
                    break;
                case 8:
                    if (sliderSet[0] == 3)
                    {
                        leds[i] = !leds[i];
                    }
                    if (sliderSet[1] == 2)
                    {
                        leds[i] = !leds[i];
                    }
                    if (sliderSet[2] == 2)
                    {
                        leds[i] = !leds[i];
                    }
                    break;
                case 9:
                    if (sliderSet[0] == 4)
                    {
                        leds[i] = !leds[i];
                    }
                    if (sliderSet[1] == 3)
                    {
                        leds[i] = !leds[i];
                    }
                    if (sliderSet[2] == 3)
                    {
                        leds[i] = !leds[i];
                    }
                    break;
            }

        }
        gameObject.transform.GetComponent<LEDManager>().SetLEDs(leds);
    }
}
