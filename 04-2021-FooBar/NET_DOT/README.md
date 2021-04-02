# FooBar NET_DOT Write Up

## Details:

Jeopardy style CTF

Category: Reverse Engineering

## Write up:

This was a .net Dll so I used dnSpy to decompile:

```c#
using System;

namespace win
{
	// Token: 0x02000002 RID: 2
	internal class Program
	{
		// Token: 0x06000001 RID: 1 RVA: 0x00002050 File Offset: 0x00000250
		private static int sum_all(string password)
		{
			int num = 0;
			foreach (char c in password)
			{
				num += (int)c;
			}
			return num;
		}

		// Token: 0x06000002 RID: 2 RVA: 0x0000208C File Offset: 0x0000028C
		private static int check(int[] values)
		{
			int[] array = new int[]
			{
				2410,
				2404,
				2430,
				2408,
				2391,
				2381,
				2333,
				2396,
				2369,
				2332,
				2398,
				2422,
				2332,
				2397,
				2416,
				2370,
				2393,
				2304,
				2393,
				2333,
				2416,
				2376,
				2371,
				2305,
				2377,
				2391
			};
			int result = 0;
			for (int i = 0; i < array.Length; i++)
			{
				bool flag = array[i] == values[i];
				if (!flag)
				{
					result = 0;
					break;
				}
				result = 1;
			}
			return result;
		}

		// Token: 0x06000003 RID: 3 RVA: 0x000020E4 File Offset: 0x000002E4
		private static void Main(string[] args)
		{
			Console.WriteLine("Hello there mate \nJust enter the flag to check : ");
			string text = Console.ReadLine();
			int[] array = new int[26];
			bool flag = text.Length != 26;
			if (flag)
			{
				Console.WriteLine("Input length error");
				Console.ReadLine();
			}
			else
			{
				for (int i = 0; i < text.Length; i++)
				{
					array[i] = (int)text[i];
				}
				int[] array2 = new int[26];
				for (int j = 0; j < 26; j++)
				{
					array2[j] = (array[j] - (j % 2 * 2 + j % 3) ^ Program.sum_all(text));
				}
				int num = Program.check(array2);
				bool flag2 = num == 1;
				if (flag2)
				{
					Console.WriteLine("Your flag : " + text);
					Console.ReadLine();
				}
				else
				{
					Console.WriteLine("try harder");
					Console.ReadLine();
				}
			}
		}
	}
}
```

For this challenge I assumed that the flag started with the correct flag text and used that to find the sum of the total text which was 2349. I then wrote a python script for reverse the key:

```python
# key in code
key = [2410, 2404, 2430, 2408, 2391, 2381, 2333, 2396, 2369, 2332, 2398, 2422, 2332, 2397, 2416, 2370, 2393, 2304, 2393, 2333, 2416, 2376, 2371, 2305, 2377, 2391]

# string to store result
s = ""

# loop through the 26 characters specified
for i in range(0, 26):

    # do the needed math
	s += chr((key[i]^2349)+((i%2)*2 + (i%3)))

# print
print(s)
```

This gave me the following:

```
GLUG{d0tn3t_1s_qu1t3_go0d}
```