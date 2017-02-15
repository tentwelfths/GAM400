using UnityEngine;
using System.Collections;

public class DistanceGameLogic : MonoBehaviour {

    private float distance;

    public void SetDistance(float d)
    {
        distance = d;
    }

	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
        for (int i = 0; i < 10; ++i)
        {
            //Do LED shit
        }
    }
}
