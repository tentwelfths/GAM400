using UnityEngine;
using System.Collections;

public class DistanceController : MonoBehaviour
{

    bool held = false;

    //public int index = 0;
    // Use this for initialization
    void Start()
    {

    }

    void OnMouseDown()
    {
        held = true;
        //Debug.Log("CLICKED");
    }

    void OnMouseUp()
    {
        held = false;
    }

    // Update is called once per frame
    void Update()
    {
        if (held)
        {
            Vector3 pos = Input.mousePosition;
            pos.z = 20;
            pos = Camera.main.ScreenToWorldPoint(pos);
            float newX = pos.x;


            if (newX < gameObject.transform.parent.transform.position.x - gameObject.transform.parent.transform.localScale.x / 2.0f + 0.25f)
            {
                //Debug.Log("TOO FAR AWAY");
                newX = gameObject.transform.parent.transform.position.x - gameObject.transform.parent.transform.localScale.x / 2.0f + 0.25f;
            }

            if (newX > gameObject.transform.parent.transform.position.x + gameObject.transform.parent.transform.localScale.x / 2.0f - 0.25f)
            {
                //Debug.Log("TOO FAR AWAY");
                newX = gameObject.transform.parent.transform.position.x + gameObject.transform.parent.transform.localScale.x / 2.0f - 0.25f;
            }
            float percentage = -(newX - gameObject.transform.parent.transform.position.x - gameObject.transform.parent.transform.localScale.x / 2.0f + 0.25f) / (gameObject.transform.parent.transform.position.x + gameObject.transform.parent.transform.localScale.x / 2.0f - 0.25f - (gameObject.transform.parent.transform.position.x - gameObject.transform.parent.transform.localScale.x / 2.0f + 0.25f));
            //print(percentage);
            gameObject.transform.parent.parent.GetComponent<DistanceGameLogic>().SetDistance(percentage);
            gameObject.transform.position = new Vector3(newX, gameObject.transform.position.y, -1);
        }
    }
}
