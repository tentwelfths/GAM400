using UnityEngine;
using System.Collections;

public class KnobGameLogic : MonoBehaviour {

    private int clockwiseRots = 0;
    private int counterClockwiseRots = 0;
    private bool AnswerDisplayed = false;
    private bool[] order = new bool[5];
    public void RegisterClockwiseRotation()
    {
        order[clockwiseRots + counterClockwiseRots] = true;
        clockwiseRots += 1;
    }

    public void RegisterCounterClockwiseRotation()
    {
        order[clockwiseRots + counterClockwiseRots] = false;
        counterClockwiseRots += 1;
    }

    public void Reset()
    {
        counterClockwiseRots = 0;
        clockwiseRots = 0;
        AnswerDisplayed = false;
    }

    // Use this for initialization
    void Start () {
    
    }

    // Update is called once per frame
    void Update () {
        if(clockwiseRots + counterClockwiseRots == 5)
        {
            gameObject.transform.GetComponentInChildren<KnobController>().DisableTurning();
            bool[] leds = new bool[10];
            if (!AnswerDisplayed)
            {
                for(int i = 0; i < 10; ++i)
                {
                    if(i == 0 || i == 9)
                    {
                        if(order[0])
                        {
                            leds[i] = !leds[i];
                        }
                    }
                    else if(i == 1 || i == 7)
                    {
                        if (order[1])
                        {
                            leds[i] = !leds[i];
                        }
                    }
                    else if (i == 2 || i == 3)
                    {
                        if (order[2])
                        {
                            leds[i] = !leds[i];
                        }
                    }
                    else if (i == 6 || i == 8)
                    {
                        if (order[3])
                        {
                            leds[i] = !leds[i];
                        }
                    }
                    else if (i == 4)
                    {
                        if (order[4])
                        {
                            leds[i] = !leds[i];
                        }
                    }
                    else if(i == 5)
                    {
                        if (!order[4])
                        {
                            leds[i] = !leds[i];
                        }
                    }
                    if(i == 0 || i == 4)
                    {
                        if(!order[2])
                        {
                            leds[i] = !leds[i];
                        }
                    }
                    else if (i == 1 || i == 7)
                    {
                        if (!order[0])
                        {
                            leds[i] = !leds[i];
                        }
                    }
                    else if (i == 2 || i == 9)
                    {
                        if (!order[1])
                        {
                            leds[i] = !leds[i];
                        }
                    }
                    else if (i == 3 || i == 6)
                    {
                        if (!order[4])
                        {
                            leds[i] = !leds[i];
                        }
                    }
                    else if (i == 4 || i == 5)
                    {
                        if (!order[3])
                        {
                            leds[i] = !leds[i];
                        }
                    }
                    //Do shti with LEDs;
                    //int check = i;
                    //print(check);
                    //if (check == 0)
                    //{
                    //    if (order[0])
                    //    {
                    //        leds[i] = !leds[i];
                    //    }
                    //}
                    //check = i % 2;
                    //if (check == 1)
                    //{
                    //  if (order[1])
                    //  {
                    //    leds[i] = !leds[i];
                    //  }
                    //}
                    //check = i % 3;
                    //print(check);
                    //if (check == 2)
                    //{
                    //  if(order[2])
                    //  {
                    //    leds[i] = !leds[i];
                    //  }
                    //}
                    //check = i % 4;
                    //print(check);
                    //if (check == 3)
                    //{
                    //   if(!order[3])
                    //   {
                    //     leds[i] = !leds[i];
                    //   }
                    //}
                    //check = i % 5;
                    //print(check);
                    //if (check == 4)
                    //{
                    //    if (!order[4])
                    //    {
                    //        leds[i] = !leds[i];
                    //    }
                    //}
                }
                //Debug.Log(leds.ToString());
                gameObject.transform.GetComponent<LEDManager>().SetLEDs(leds);
                AnswerDisplayed = true;
            }
        }
	}
}
