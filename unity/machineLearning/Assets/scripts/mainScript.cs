using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Runtime.InteropServices;
using System;
using System.Threading;

public class mainScript : MonoBehaviour

{

    [DllImport("machineLearning")]
    public static extern IntPtr CreateModel(int inputdimension);
    [DllImport("machineLearning")]
    public static extern double LinearClassification(IntPtr model, double[] input, int inputSize);
    [DllImport("machineLearning")]
    public static extern int FitRosenblatt(IntPtr model, double[] arrayInputs, int inputsSize,
        int inputSize, double[] outputs, int outputsSize, double step, int iterations);


    // Start is called before the first frame update
    public Transform[] spheresToTrain;
    public Transform[] spheresToMove;
    private IntPtr model;
    void Start()
    {
        model = CreateModel(3);



        int tailleSphereToTrain = spheresToTrain.Length;
        double[] arrayInputs = new double[tailleSphereToTrain * 2];
        var pos = 0;
        for (int i = 0; i < tailleSphereToTrain; ++i)
        {
            arrayInputs[pos] = spheresToTrain[i].position.x;
            arrayInputs[pos + 1] = spheresToTrain[i].position.z;
            pos += 2;
        }
        double[] outputs = new double[tailleSphereToTrain];
        for (int i = 0; i < tailleSphereToTrain; ++i)
        {
            if (spheresToTrain[i].position.y > 0)
            {
                outputs[i] = 1;
            }
            else
            {
                outputs[i] = -1;
            }
        }
        var rawArray = new double[3];
        int result = FitRosenblatt(model, arrayInputs, tailleSphereToTrain, 2, outputs, tailleSphereToTrain, 0.01, 10000);
        int tailleSphereToMove = spheresToMove.Length;
        for (int i = 0; i < tailleSphereToMove; ++i)
        {
            // spheresToMove[i]
            double[] cord = new double[2] { spheresToMove[i].position.x, spheresToMove[i].position.z };
            double newcord = LinearClassification(model, cord, 2);
            //
            spheresToMove[i].position = new Vector3(spheresToMove[i].position.x, (float)newcord, spheresToMove[i].position.z);
        }
    }

    //essayer de bouger les spheres une par une mais c'est moche
   /* IEnumerator Example()
    {
        int tailleSphereToMove = spheresToMove.Length;
        int[] result = new int[tailleSphereToMove];
        //fake tableauresultatinferenec
        for (int i = 0; i < tailleSphereToMove; ++i)
        {
            if (i > 0)
                result[i] = 1;
            else
                result[i] = -1;
        }
        for (int i = 0; i < tailleSphereToMove; ++i)
        {
            Debug.Log("avant");
            Debug.Log(spheresToMove[i].position);
            //Thread.Sleep(100);
            
            spheresToMove[i].position = new Vector3(spheresToMove[i].position.x, spheresToMove[i].position.y + result[i], spheresToMove[i].position.z);
            yield return new WaitForSeconds(0.00005f);
            Debug.Log("apres");
            Debug.Log(spheresToMove[i].position);
        }
       
    }*/

    // Update is called once per frame
    void Update()
    {
      
    }
}
