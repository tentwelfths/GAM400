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
        for (int i = 0; i < 10; ++i)
        {
            //Do LED shit
        }
    }
}
