# RaRCTF Dotty Write Up

## Details:

Jeopardy style CTF

Category: Reverse Engineering

Points: 150

Comments: My new program will keep your secrets safe using military grade encryption!

## Write up:

Running the file command on dotty.exe we can see that it is a .NET assembly so I opened the file using dnSpy:

```c#
using System;
using System.Collections.Generic;
using System.Linq;

namespace Dotty
{
	// Token: 0x02000002 RID: 2
	internal class Program
	{
		// Token: 0x06000002 RID: 2 RVA: 0x00002058 File Offset: 0x00000258
		private static string Dotter(string phrase)
		{
			return string.Join("|", from char c in phrase
			select Program.mapper[char.ToUpper(c)]);
		}

		// Token: 0x06000003 RID: 3 RVA: 0x0000208C File Offset: 0x0000028C
		private static void Main(string[] args)
		{
			Console.Write("Please enter your secret to encode: ");
			string phrase = Console.ReadLine();
			string text = Program.Dotter(phrase);
			if (text == Check.check)
			{
				Console.WriteLine("That's the right secret!");
			}
			else
			{
				Console.WriteLine(text);
			}
		}

		// Token: 0x04000001 RID: 1
		private static Dictionary<char, string> mapper = new Dictionary<char, string>
		{
			{
				' ',
				"/"
			},
...
			{
				'0',
				"-----"
			}
		};
	}
}
```

```c#
using System;

namespace Dotty
{
	// Token: 0x02000003 RID: 3
	internal class Check
	{
		// Token: 0x04000003 RID: 3
		public static string check = "-|....|.|/|..-.|.-..|.-|--.|/|..|...|/|---|.---|--.-|-..-|.|-.--|...--|..-|--|--..|.....|.--|..|--|.-..|.|.-..|.....|....-|-|.-|.....|-.-|--...|---|.-|--..|-|--.|..---|..---|--...|--.|-...|--..|..-.|-....|-.|.-..|--.-|.--.|.|--...|-|-....|.--.|--..|--...|.-..|.....|-|--.|-.-.|-.|-..|-...|--|--|...--|-..|.-|-.|.-..|.....|/|-...|.-|...|.|...--|..---";
	}
}
```

From this I could see that check was the flag in it's encrypted form. So what I did was extract the dictionary and write a script to convert the flag back to text:

```python
# flag string
flag = "-|....|.|/|..-.|.-..|.-|--.|/|..|...|/|---|.---|--.-|-..-|.|-.--|...--|..-|--|--..|.....|.--|..|--|.-..|.|.-..|.....|....-|-|.-|.....|-.-|--...|---|.-|--..|-|--.|..---|..---|--...|--.|-...|--..|..-.|-....|-.|.-..|--.-|.--.|.|--...|-|-....|.--.|--..|--...|.-..|.....|-|--.|-.-.|-.|-..|-...|--|--|...--|-..|.-|-.|.-..|.....|/|-...|.-|...|.|...--|..---"

# mapped dictionary
mapped = [[ ' ', "/" ], [ 'A', ".-" ], [ 'B', "-..." ], [ 'C', "-.-." ], [ 'D', "-.." ], [ 'E', "." ], [ 'F', "..-." ], [ 'G', "--." ], [ 'H', "...." ], [ 'I', ".." ], [ 'J', ".---" ], [ 'K', "-.-" ], [ 'L', ".-.." ], [ 'M', "--" ], [ 'N', "-." ], [ 'O', "---" ], [ 'P', ".--." ], [ 'Q', "--.-" ], [ 'R', ".-." ], [ 'S', "..." ], [ 'T', "-" ], [ 'U', "..-" ], [ 'V', "...-" ], [ 'W', ".--" ], [ 'X', "-..-" ], [ 'Y', "-.--" ], [ 'Z', "--.." ], [ '1', ".----" ], [ '2', "..---" ], [ '3', "...--" ], [ '4', "....-" ], [ '5', "....." ], [ '6', "-...." ], [ '7', "--..." ], [ '8', "---.." ], [ '9', "----." ], [ '0', "-----" ]  ]

# split the flag the way the program does it
flag = flag.split("|")

s = ""

# go through and convert the flag
for i in flag:
	for j in mapped:
		if i == j[1]:
			s += j[0]
			break	


print(s)
```

When run this pritned out:

```
THE FLAG IS OJQXEY3UMZ5WIMLEL54TA5K7OAZTG227GBZF6NLQPE7T6PZ7L5TGCNDBMM3DANL5 BASE32
```

When converted from BASE32 I got:

```
rarctf{d1d_y0u_p33k_0r_5py????_fa4ac605}
```