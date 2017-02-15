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
                for(int i = 0; i < 9; ++i)
                {
                    //Do shti with LEDs
                }
                Debug.Log(leds.ToString());
                gameObject.transform.GetComponent<LEDManager>().SetLEDs(leds);
                AnswerDisplayed = true;
            }
        }
	}
}
