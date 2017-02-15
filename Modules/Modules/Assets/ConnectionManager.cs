using UnityEngine;
using System.Collections;
using System.Net;
using System;
using System.Net.Sockets;
using System.Text;

public class ConnectionManager : MonoBehaviour {

    private UdpClient socket;
    public IPEndPoint ep;

	// Use this for initialization
	void Start () {
        socket = new UdpClient("169.254.147.98", 27015);
        string name = "WHAT IS POPPING?";
        byte[] sdata = Encoding.ASCII.GetBytes(name);
        socket.Send(sdata, sdata.Length);
        //byte[] rdata = socket.Receive(ref ep);
        //string job = Encoding.ASCII.GetString(rdata);
    }
	
	// Update is called once per frame
	void Update () {
	
	}
}
