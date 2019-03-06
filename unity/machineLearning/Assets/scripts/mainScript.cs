using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Runtime.InteropServices;
using System;

public class mainScript : MonoBehaviour

{

    [DllImport("machineLearning")]
    public static extern IntPtr CreateModel(int inputdimension);
    [DllImport("machineLearning")]
    public static extern void RemoveLinearModel(IntPtr poiteur);
    // Start is called before the first frame update
    void Start()
    {
        var result = CreateModel(6);
        Debug.Log(result);
        RemoveLinearModel(result);
        Debug.Log("Deletion OK");
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
