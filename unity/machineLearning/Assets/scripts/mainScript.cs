using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Runtime.InteropServices;
using System;
using System.Threading;
using UnityEngine.SceneManagement;

public class mainScript : MonoBehaviour

{

    [DllImport("machineLearning")]
    public static extern IntPtr CreateModel(int inputdimension);
    [DllImport("machineLearning")]
    public static extern double LinearClassification(IntPtr model, double[] input, int inputSize);
    [DllImport("machineLearning")]
    public static extern int FitLinearClassification(IntPtr model, double[] arrayInputs, int inputsSize,
        int inputSize, double[] outputs, int outputsSize, double step, int iterations);

    [DllImport("machineLearning")]
    public static extern double LinearRegression(IntPtr model, double[] input, int inputSize);
    [DllImport("machineLearning")]
    public static extern int FitLinearRegression(IntPtr model, double[] arrayInputs, int inputsSize,
        int inputSize, double[] outputs, int outputsSize);
    [DllImport("machineLearning")]
    public static extern IntPtr trainPCM(int[] neuroneParCouche, int nbCouche, double[] data, int colSizeData, int lineCountData,
        double[] target, int colSizeTarget, int epoch, double apprentissage, bool classifOrRegress);
    [DllImport("machineLearning")]
    public static extern double[] usePCM(double[] data, int colSize, IntPtr model);

    // Start is called before the first frame update
    public Transform[] spheresToTrain;
    public Transform[] spheresToMove;
    private List<Color> listColors;
    private IntPtr model;


    void Start()
    {
        string name = SceneManager.GetActiveScene().name;
        string type = name.Split('_')[0].ToUpper();

        switch(type)
        {
            case "CLASSIFICATION":
                ActionForClassification();
                break;
            case "REGRESSION":
                ActionForRegression();
                break;
            case "SOLUTION":
                if (name.Split('_')[1].ToUpper() == "XOR") ActionForXOR();
                break;
            case "PMC":
                if(name.Split('_')[1].ToUpper() == "CLASSIFICATION")
                {
                    ActionForPMCClassification();
                }
                break;
        }
    }

    void ActionForClassification()
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
            outputs[i] = spheresToTrain[i].position.y > 0 ? 1 : -1;
        }

        int result = FitLinearClassification(model, arrayInputs, tailleSphereToTrain, 2, outputs, tailleSphereToTrain, 0.01, 10000);
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

    void ActionForXOR()
    {
        model = CreateModel(3);

        int tailleSphereToTrain = spheresToTrain.Length;
        double[] arrayInputs = new double[tailleSphereToTrain * 2];
        var pos = 0;
        for (int i = 0; i < tailleSphereToTrain; ++i)
        {
            arrayInputs[pos] = spheresToMove[i].position.x * spheresToMove[i].position.z;
            arrayInputs[pos + 1] = spheresToMove[i].position.z;
            pos += 2;
        }
        double[] outputs = new double[tailleSphereToTrain];
        for (int i = 0; i < tailleSphereToTrain; ++i)
        {
            outputs[i] = spheresToTrain[i].position.y > 0 ? 1 : -1;
        }

        int result = FitLinearClassification(model, arrayInputs, tailleSphereToTrain, 2, outputs, tailleSphereToTrain, 0.01, 10000);
        int tailleSphereToMove = spheresToMove.Length;
        for (int i = 0; i < tailleSphereToMove; ++i)
        {
            // spheresToMove[i]
            double[] cord = new double[2] { spheresToMove[i].position.x * spheresToMove[i].position.z, spheresToMove[i].position.z };
            double newcord = LinearClassification(model, cord, 2);
            //
            spheresToMove[i].position = new Vector3(spheresToMove[i].position.x, (float)newcord, spheresToMove[i].position.z);
        }
    }

    void ActionForRegression()
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
            outputs[i] = spheresToTrain[i].position.y;
        }

        int result = FitLinearRegression(model, arrayInputs, tailleSphereToTrain, 2, outputs, tailleSphereToTrain);
        int tailleSphereToMove = spheresToMove.Length;
        for (int i = 0; i < tailleSphereToMove; ++i)
        {
            // spheresToMove[i]
            double[] cord = new double[2] { spheresToMove[i].position.x, spheresToMove[i].position.z };
            double newcord = LinearRegression(model, cord, 2);
            //
            spheresToMove[i].position = new Vector3(spheresToMove[i].position.x, (float)newcord, spheresToMove[i].position.z);
        }
    }



    void ActionForPMCClassification()
    {
        listColors = new List<Color>();

        foreach(var sphere in spheresToTrain)
        {
            if (!listColors.Contains(sphere.GetComponent<Renderer>().material.color))
            {
                listColors.Add(sphere.GetComponent<Renderer>().material.color);
            }
        }

        if (listColors.Count == 2)
        {
            PMCForDuoClasses(listColors);
        }
        else if (listColors.Count == 3)
        {
            PMCForTrioClasses(listColors);
        }
    }

    void PMCForDuoClasses(List<Color> colors)
    {
        Dictionary<Color, int> dicoIntForColor = new Dictionary<Color, int>();
        Dictionary<int, Color> dicoColorForInt = new Dictionary<int, Color>();

        dicoIntForColor[colors[0]] = 1;
        dicoIntForColor[colors[1]] = -1;
        dicoColorForInt[1] = colors[0];
        dicoColorForInt[-1] = colors[1];


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
            outputs[i] = dicoIntForColor[spheresToTrain[i].GetComponent<Renderer>().material.color];
        }

        int result = FitLinearClassification(model, arrayInputs, tailleSphereToTrain, 2, outputs, tailleSphereToTrain, 0.01, 10000);
        int tailleSphereToMove = spheresToMove.Length;
        for (int i = 0; i < tailleSphereToMove; ++i)
        {
            double[] cord = new double[2] { spheresToMove[i].position.x, spheresToMove[i].position.z };
            double newColor = LinearClassification(model, cord, 2);
            spheresToMove[i].GetComponent<Renderer>().material.color = dicoColorForInt[(int)newColor];
        }
    }

    void PMCForTrioClasses(List<Color> colors)
    {
        int tailleSphereToTrain = spheresToTrain.Length;

        double[] arrayInputs = new double[tailleSphereToTrain * 2];
        var pos = 0;

        for (int i = 0; i < tailleSphereToTrain; ++i)
        {
            arrayInputs[pos] = spheresToTrain[i].position.x;
            arrayInputs[pos + 1] = spheresToTrain[i].position.z;
            pos += 2;
        }

        double[] outputs = new double[tailleSphereToTrain * 3];
        int j = 0;
        for (int i = 0; i < tailleSphereToTrain; ++i)
        {
            Color c = spheresToTrain[i].GetComponent<Renderer>().material.color;
            outputs[j] = c.r;
            j += 1;
            outputs[j] = c.g;
            j += 1;
            outputs[j] = c.b;
            j += 1;
        }

        int[] neuronesc = new int[] { 4, 3 };

        string s = "neurones couches : ";

        foreach(var n in neuronesc)
        {
            s += n;
            s += ", ";
        }

        s += " nombre couche : ";
        s += 2;
        s += ", inputs : ";

        foreach (var n in arrayInputs)
        {
            s += n;
            s += ", ";
        }

        s += "colsizedata : ";
        s += 2;
        s += ", linesizedata : ";
        s += tailleSphereToTrain;
        s += ", outputs : ";

        foreach (var n in outputs)
        {
            s += n;
            s += ", ";
        }

        Debug.Log(s);


        trainPCM(neuronesc, 2, arrayInputs, 2, tailleSphereToTrain, outputs, 3, 10, 0.01, true);

        var array = usePCM(new double[] { 9, 7 }, 2);
        Debug.Log(array[0] + ", " + array[1] + ", " + array[2]);

        /*foreach(var sphere in spheresToMove)
        {
            double[] array = usePCM(new double[] { sphere.position.x, sphere.position.z }, 2);
            Debug.Log(sphere.position.x + "x" + sphere.position.z + "y, rgb :" + array[0] + ", " + array[1] + ", " + array[2]);
        }*/
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
