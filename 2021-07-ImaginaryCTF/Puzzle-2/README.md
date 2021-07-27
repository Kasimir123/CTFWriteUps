# ImaginaryCTF Puzzle 2 Write Up

## Details:

Jeopardy style CTF

Category: Miscellaneous

Points: 250

Comments: 

The evil presidential candidate Et3rnos is now forcing this poor game's players to vote on him in order to access the flag room. Can you teach him a lesson and get access to it without voting on him? The flag is in the format ICTF{[A-Z_]+}

## Write up:

I opened up the Assembly dll in dnyspy and edited the C# to turn off coliisions and set y velocity to 0. I was then able to just no clip through the walls and saw the flag:

```c#
using System;
using UnityEngine;

// Token: 0x02000007 RID: 7
public class PlayerController : MonoBehaviour
{
	// Token: 0x06000016 RID: 22 RVA: 0x00002116 File Offset: 0x00000316
	private void Start()
	{
		this.rb = base.GetComponent<Rigidbody>();
		this.rb.detectCollisions = false;
	}

	// Token: 0x06000017 RID: 23 RVA: 0x00002130 File Offset: 0x00000330
	private void Update()
	{
		if (Input.GetKeyDown(KeyCode.Space))
		{
			this.rb.AddForce(Vector3.up * this.jumpPower * 10f);
		}
	}

	// Token: 0x06000018 RID: 24 RVA: 0x00002160 File Offset: 0x00000360
	private void FixedUpdate()
	{
		this.Sprint();
		this.Move();
	}

	// Token: 0x06000019 RID: 25 RVA: 0x0000216E File Offset: 0x0000036E
	private void Sprint()
	{
		if (Input.GetKey(KeyCode.LeftShift))
		{
			this.movementSpeed = this.baseMovementSpeed * 2f;
			return;
		}
		this.movementSpeed = this.baseMovementSpeed;
	}

	// Token: 0x0600001A RID: 26 RVA: 0x000022F0 File Offset: 0x000004F0
	private void Move()
	{
		Vector3 vector = default(Vector3);
		if (Input.GetKey(KeyCode.W))
		{
			vector += Vector3.forward;
		}
		if (Input.GetKey(KeyCode.S))
		{
			vector += Vector3.back;
		}
		if (Input.GetKey(KeyCode.D))
		{
			vector += Vector3.right;
		}
		if (Input.GetKey(KeyCode.A))
		{
			vector += Vector3.left;
		}
		vector = this.rb.rotation * vector * this.movementSpeed;
		this.rb.velocity = new Vector3(vector.x, 0f, vector.z);
	}

	// Token: 0x0400000E RID: 14
	public float baseMovementSpeed;

	// Token: 0x0400000F RID: 15
	private float movementSpeed;

	// Token: 0x04000010 RID: 16
	public float jumpPower;

	// Token: 0x04000011 RID: 17
	private Rigidbody rb;
}
```

```
spy_kids_assemble
```