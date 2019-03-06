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
    public static extern void RemoveLinearModel(IntPtr poiteur);
    // Start is called before the first frame update
    public Transform[] spheresToTrain;
    public Transform[] spheresToMove;
    void Start()
    {

        //StartCoroutine(Example());
        int tailleSphereToMove = spheresToMove.Length;
        int[] result = new int[tailleSphereToMove];
        //fake tableauresultatinferenec
        for (int i = 0; i < tailleSphereToMove; ++i)
        {
            if (i < tailleSphereToMove / 2)
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
            Debug.Log("apres");
            Debug.Log(spheresToMove[i].position);
        }
    }

    //essayer de bouger les spheres une par une mais c'est moche
    IEnumerator Example()
    {
        int tailleSphereToMove = spheresToMove.Length;
        int[] result = new int[tailleSphereToMove];
        //fake tableauresultatinferenec
        for (int i = 0; i < tailleSphereToMove; ++i)
        {
            if (i < tailleSphereToMove / 2)
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
       
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
