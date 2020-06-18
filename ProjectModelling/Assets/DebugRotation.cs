using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[ExecuteInEditMode]
public class DebugRotation : MonoBehaviour {

	// Use this for initialization
	void Start () {
		Debug.Log(transform.eulerAngles);
	}
	
	// Update is called once per frame
	void Update () {
			Debug.Log(transform.eulerAngles);
	}
}
