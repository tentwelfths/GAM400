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
        for (int i = 0; i < 10; ++i)
        {
            //Do LED shit
        }
    }
}
