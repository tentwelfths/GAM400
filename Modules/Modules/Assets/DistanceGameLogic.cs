using UnityEngine;
using System.Collections;

public class DistanceGameLogic : MonoBehaviour {

    private float distance = 0.0f;
    private int set = 0;

    public void SetDistance(float d)
    {
        distance = d;
    }

	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
        if(distance <= 0.1)
        {
            set = 5;
        }
        else if(distance > 0.1 && distance <= 0.2)
        {
            set = 3;
        }
        else if(distance > 0.2 && distance <= 0.3)
        {
            set = 9;
        }
        else if (distance > 0.3 && distance <= 0.4)
        {
            set = 2;
        }
        else if (distance > 0.4 && distance <= 0.5)
        {
            set = 6;
        }
        else if (distance > 0.6 && distance <= 0.7)
        {
            set = 8;
        }
        else if (distance > 0.7 && distance <= 0.8)
        {
            set = 1;
        }
        else if (distance > 0.8 && distance <= 0.9)
        {
            set = 4;
        }
        else if (distance > 0.9 && distance <= 1.0)
        {
            set = 7;
        }
        bool[] leds = new bool[10];
        for (int i = 0; i < 10; ++i)
        {
            //Do LED shit
            if(i < set)
            {
                leds[i] = true;
            }
            else
            {
                leds[i] = false;
            }
        }
        gameObject.transform.GetComponent<LEDManager>().SetLEDs(leds);
    }
}
