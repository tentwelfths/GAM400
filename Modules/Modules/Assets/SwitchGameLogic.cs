using UnityEngine;
using System.Collections;

public class SwitchGameLogic : MonoBehaviour {

    private bool[] switches = new bool[3];


    public void SetSwitch(int index, bool on)
    {
        switches[index] = on;
    }


	// Use this for initialization
	void Start () {
	    for(int i = 0; i < switches.Length; ++i)
        {
            switches[i] = false;
        }
	}
	
	// Update is called once per frame
	void Update () {
        bool[] leds = new bool[10];
        for (int i = 0; i < 10; ++i)
        {
            //Do LED shit
            switch (i)
            {
                case 0:
                    if (switches[0])
                    {
                        leds[i] = !leds[i];
                    }
                    else if (!switches[1])
                    {
                        leds[i] = !leds[i];
                    }
                    if (!switches[2])
                    {
                        leds[i] = !leds[i];
                    }
                    break;
                case 1:
                    if (!switches[0])
                    {
                        leds[i] = !leds[i];
                    }
                    else if (switches[1])
                    {
                        leds[i] = !leds[i];
                    }
                    if (!switches[2])
                    {
                        leds[i] = !leds[i];
                    }
                    break;
                case 2:
                    if (!switches[0])
                    {
                        leds[i] = !leds[i];
                    }
                    if (!switches[1])
                    {
                        leds[i] = !leds[i];
                    }
                    else if (switches[2])
                    {
                        leds[i] = !leds[i];
                    }
                    break;
                case 3:
                    if (switches[0])
                    {
                        leds[i] = !leds[i];
                    }
                    else if (switches[1])
                    {
                        leds[i] = !leds[i];
                    }
                    if (!switches[2])
                    {
                        leds[i] = !leds[i];
                    }
                    break;
                case 4:
                    if (!switches[0])
                    {
                        leds[i] = !leds[i];
                    }
                    if (switches[1])
                    {
                        leds[i] = !leds[i];
                    }
                    else if (switches[2])
                    {
                        leds[i] = !leds[i];
                    }
                    break;
                case 5:
                    if (switches[0])
                    {
                        leds[i] = !leds[i];
                    }
                    else if (!switches[1])
                    {
                        leds[i] = !leds[i];
                    }
                    else if (switches[2])
                    {
                        leds[i] = !leds[i];
                    }
                    break;
                case 6:
                    if (switches[0])
                    {
                        leds[i] = !leds[i];
                    }
                    else if (!switches[1])
                    {
                        leds[i] = !leds[i];
                    }
                    if (!switches[2])
                    {
                        leds[i] = !leds[i];
                    }
                    break;
                case 7:
                    if (!switches[0])
                    {
                        leds[i] = !leds[i];
                    }
                    if (!switches[1])
                    {
                        leds[i] = !leds[i];
                    }
                    else if (!switches[2])
                    {
                        leds[i] = !leds[i];
                    }
                    break;
                case 8:
                    if (switches[0])
                    {
                        leds[i] = !leds[i];
                    }
                    if (switches[1])
                    {
                        leds[i] = !leds[i];
                    }
                    else if (switches[2])
                    {
                        leds[i] = !leds[i];
                    }
                    break;
                case 9:
                    if (!switches[0])
                    {
                        leds[i] = !leds[i];
                    }
                    if (!switches[1])
                    {
                        leds[i] = !leds[i];
                    }
                    else if (switches[2])
                    {
                        leds[i] = !leds[i];
                    }
                    break;
            }
        }
        gameObject.transform.GetComponent<LEDManager>().SetLEDs(leds);
    }
}
