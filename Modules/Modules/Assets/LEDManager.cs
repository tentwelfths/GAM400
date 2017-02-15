using UnityEngine;
using System.Collections;

public class LEDManager : MonoBehaviour {
    public SpriteRenderer[] LEDs = new SpriteRenderer[10];
    // Use this for initialization
    void Start() {
        bool[] temp = { false,false,false,false,false,false,false,false,false,false};
        SetLEDs(temp);
	}

    public void SetLEDs(bool[] leds)
    {
        for(int i = 0; i < leds.Length && i < LEDs.Length; ++i)
        {
            LEDs[i].GetComponent<Renderer>().material.color = (leds[i]) ? new Vector4(1,1,1,1) : new Vector4(0.25f, 0.25f, 0.25f, 1);
        }
    }
	
	// Update is called once per frame
	void Update () {
	
	}
}
