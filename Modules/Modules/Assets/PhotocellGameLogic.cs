using UnityEngine;
using System.Collections;

public class PhotocellGameLogic : MonoBehaviour {

    private bool[] cells = new bool[8];
    public void SetPhotocell(int index, bool lit)
    {
        cells[index] = lit;
    }

	// Use this for initialization
	void Start () {
        for (int i = 0; i < cells.Length; ++i)
        {
            cells[i] = false;
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
                    if (cells[0])
                    {
                        leds[i] = !leds[i];
                    }
                    if (cells[1])
                    {
                        leds[i] = !leds[i];
                    }
                    if (cells[2])
                    {
                        leds[i] = !leds[i];
                    }
                    if (cells[3])
                    {
                        leds[i] = !leds[i];
                    }
                    if (cells[4])
                    {
                        leds[i] = !leds[i];
                    }
                    break;
                case 1:
                    if (cells[0])
                    {
                        leds[i] = !leds[i];
                    }
                    if (!cells[3])
                    {
                        leds[i] = !leds[i];
                    }
                    if (cells[4])
                    {
                        leds[i] = !leds[i];
                    }
                    if (!cells[5])
                    {
                        leds[i] = !leds[i];
                    }
                    if (cells[6])
                    {
                        leds[i] = !leds[i];
                    }
                    if (!cells[7])
                    {
                        leds[i] = !leds[i];
                    }
                    break;
                case 2:
                    if (!cells[0])
                    {
                        leds[i] = !leds[i];
                    }
                    if (!cells[2])
                    {
                        leds[i] = !leds[i];
                    }
                    if (cells[3])
                    {
                        leds[i] = !leds[i];
                    }
                    if (!cells[4])
                    {
                        leds[i] = !leds[i];
                    }
                    if (cells[5])
                    {
                        leds[i] = !leds[i];
                    }
                    if (!cells[6])
                    {
                        leds[i] = !leds[i];
                    }
                    break;
                case 3:
                    if (!cells[0])
                    {
                        leds[i] = !leds[i];
                    }
                    if (cells[2])
                    {
                        leds[i] = !leds[i];
                    }
                    if (!cells[5])
                    {
                        leds[i] = !leds[i];
                    }
                    if (cells[6])
                    {
                        leds[i] = !leds[i];
                    }
                    if (cells[7])
                    {
                        leds[i] = !leds[i];
                    }
                    break;
                case 4:
                    if (cells[1])
                    {
                        leds[i] = !leds[i];
                    }
                    if (!cells[2])
                    {
                        leds[i] = !leds[i];
                    }
                    if (!cells[3])
                    {
                        leds[i] = !leds[i];
                    }
                    if (cells[4])
                    {
                        leds[i] = !leds[i];
                    }
                    if (cells[5])
                    {
                        leds[i] = !leds[i];
                    }
                    if (!cells[6])
                    {
                        leds[i] = !leds[i];
                    }
                    break;
                case 5:
                    if (cells[0])
                    {
                        leds[i] = !leds[i];
                    }
                    if (cells[1])
                    {
                        leds[i] = !leds[i];
                    }
                    if (!cells[2])
                    {
                        leds[i] = !leds[i];
                    }
                    if (!cells[3])
                    {
                        leds[i] = !leds[i];
                    }
                    if (!cells[4])
                    {
                        leds[i] = !leds[i];
                    }
                    if (cells[7])
                    {
                        leds[i] = !leds[i];
                    }
                    break;
                case 6:
                    if (!cells[0])
                    {
                        leds[i] = !leds[i];
                    }
                    if (cells[1])
                    {
                        leds[i] = !leds[i];
                    }
                    if (cells[2])
                    {
                        leds[i] = !leds[i];
                    }
                    if (cells[3])
                    {
                        leds[i] = !leds[i];
                    }
                    if (!cells[5])
                    {
                        leds[i] = !leds[i];
                    }
                    if (!cells[6])
                    {
                        leds[i] = !leds[i];
                    }
                    if (!cells[7])
                    {
                        leds[i] = !leds[i];
                    }
                    break;
                case 7:
                    if (!cells[0])
                    {
                        leds[i] = !leds[i];
                    }
                    if (!cells[2])
                    {
                        leds[i] = !leds[i];
                    }
                    if (cells[3])
                    {
                        leds[i] = !leds[i];
                    }
                    if (cells[4])
                    {
                        leds[i] = !leds[i];
                    }
                    if (cells[5])
                    {
                        leds[i] = !leds[i];
                    }
                    if (!cells[7])
                    {
                        leds[i] = !leds[i];
                    }
                    break;
                case 8:
                    if (cells[0])
                    {
                        leds[i] = !leds[i];
                    }
                    if (cells[1])
                    {
                        leds[i] = !leds[i];
                    }
                    if (cells[6])
                    {
                        leds[i] = !leds[i];
                    }
                    if (cells[7])
                    {
                        leds[i] = !leds[i];
                    }
                    break;
                case 9:
                    if (!cells[3])
                    {
                        leds[i] = !leds[i];
                    }
                    if (!cells[4])
                    {
                        leds[i] = !leds[i];
                    }
                    break;
            }
        }
        gameObject.transform.GetComponent<LEDManager>().SetLEDs(leds);
    }
}
