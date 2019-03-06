using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;

public class DLLScript : MonoBehaviour
{
  /*  [DllImport("Assets/MLLib.dll")]
    public static extern double[] CreateModel(int inputdimension);
    [DllImport("Assets/MLLib.dll")]
    public static extern void RemoveLinearModel(double[] model);
    [DllImport("Assets/MLLib.dll")]
    public static extern int LinearFirstRegression(double[] model, double[] inputs, int inputsSize, int inputSize, double[] output);
    [DllImport("Assets/MLLib.dll")]
    [DllImport("Assets/MLLib.dll")]
    [DllImport("Assets/MLLib.dll")]*/
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}

/*

    __declspec(dllexport) int LinearFirstRegression(double *model, double *inputs, int inputsSize, int inputSize, double *outputs, int outputsSize);
    __declspec(dllexport) int FitRosenblatt(double *modem, double *inputs, int inputsSize, double *outputs, int outputsSize);
    __declspec(dllexport) double LinearClassification(double *model, double *input, int inputSize);
    __declspec(dllexport) double LinearRegression(double *model, double *input, int inputSize);

    */
