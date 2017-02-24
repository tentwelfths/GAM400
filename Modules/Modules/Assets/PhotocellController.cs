using UnityEngine;
using System.Collections;

public class PhotocellController : MonoBehaviour
{

    public Color unlit;
    public Color lit;
    public int index;

    private bool isLit = false;

    // Use this for initialization
    void Start()
    {
        gameObject.GetComponent<Renderer>().material.color = unlit;
    }


    void OnMouseDown()
    {
        isLit = !isLit;
        if (isLit)
        {
            gameObject.GetComponent<Renderer>().material.color = lit;
        }
        else
        {
            gameObject.GetComponent<Renderer>().material.color = unlit;
        }
        gameObject.transform.parent.GetComponent<PhotocellGameLogic>().SetPhotocell(index, isLit);
    }

    // Update is called once per frame
    void Update()
    {

    }
}
