using UnityEngine;
using System.Collections;

public class LightConfig
{
    public bool[] leds = new bool[10];
}


public class KnobGameLogic : MonoBehaviour {

    private int clockwiseRots = 0;
    private int counterClockwiseRots = 0;
    private bool AnswerDisplayed = false;
    private bool[] order = new bool[5];
    private LightConfig[] leds = new LightConfig[10];
    private int section = 0;
    
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
        print(leds);
        for (int i = 0; i < 10; ++i) leds[i] = new LightConfig();
        leds[0].leds = new bool[10] { false, false, false, false, false, false, false, false, false, false };
        leds[1].leds = new bool[10] { false, false, false, true, false, false, true, false, false, true };
        leds[2].leds = new bool[10] { false, false, true, false, false, false, false, true, false, false };
        leds[3].leds = new bool[10] { true, false, true, true, true, true, true, true, false, true };
        leds[4].leds = new bool[10] { true, false, true, true, false, false, true, true, false, false };
        leds[5].leds = new bool[10] { false, false, false, false, false, true, false, false, false, false };
        leds[6].leds = new bool[10] { false, true, false, true, true, true, true, false, true, false };
        leds[7].leds = new bool[10] { true, true, false, false, true, false, false, true, false, false };
        leds[8].leds = new bool[10] { true, true, true, true, true, true, true, true, true, true };
        leds[9].leds = new bool[10] { false, true, true, true, true, true, true, true, true, true };
    }

    // Update is called once per frame
    void Update () {
        if(counterClockwiseRots > 1)
        {
            counterClockwiseRots = 0;
            clockwiseRots = 0;
            --section;
            if(section < 0)
            {
                section = 9;
            }
        }
        if (clockwiseRots > 1)
        {
            counterClockwiseRots = 0;
            clockwiseRots = 0;
            ++section;
            if (section > 9)
            {
                section = 0;
            }
        }
        gameObject.transform.GetComponent<LEDManager>().SetLEDs(leds[section].leds);
        AnswerDisplayed = true;
    }
}

