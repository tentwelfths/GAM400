using UnityEngine;
using System.Collections;

public class KnobController : MonoBehaviour {

    public Renderer rend;

    public int multiplier = 6;

    private bool over = false;

    private float percent = 0.0f;
    private float total = 0.0f;

    private bool canTurn = true;

    void Start()
    {
        rend = GetComponent<Renderer>();
    }
    void OnMouseEnter()
    {
        over = true;
    }

    public void DisableTurning()
    {
        canTurn = false;
    }

    void OnMouseOver()
    {
        if (!canTurn) return;
        total += Input.mouseScrollDelta.y;
        gameObject.transform.Rotate(new Vector3(0, 0, Input.mouseScrollDelta.y * multiplier));
        if (total >= 360 / multiplier)
        {
            gameObject.transform.parent.GetComponent<KnobGameLogic>().RegisterCounterClockwiseRotation();
           // Debug.Log("COUNTER CLOCKWISE");
            total -= 360 / multiplier;
        }
        if (total <= -360 / multiplier)
        {
            gameObject.transform.parent.GetComponent<KnobGameLogic>().RegisterClockwiseRotation();
           // Debug.Log("CLOCKWISE");
            total += 360 / multiplier;
        }
       // Debug.Log(total);
        bool[] leds = new bool[10];
        if (total > 0)
        {
            for (int i = 0; i < 10; ++i)
            {
                if (total > i * (360 / multiplier) / 10)
                {
                    leds[i] = true;
                }
                else
                {
                    leds[i] = false;
                }
            }
        }
        else
        {
            for (int i = 9; i >= 0; --i)
            {
                if(-total > (9 - i) * (360 / multiplier) / 10)
                {
                    leds[i] = true;
                }
                else
                {
                    leds[i] = false;
                }
            }
        }
        gameObject.transform.parent.GetComponent<LEDManager>().SetLEDs(leds);
    }
    void OnMouseExit()
    {
        over = false;
    }
    // Update is called once per frame
    void Update () {
	
	}
}
