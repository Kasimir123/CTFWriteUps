# UMDCTF Starbucks Write Up

## Details:

Jeopardy style CTF

Category: Reverse Engineering

Comments:

Unfortunately, you have been forced to use Java, but you are only given a single class file which doesn't seem to work.

## Write up:

I decompiled the .class file and got:

```java
public class Challenge {
   public static String f1(String s) {
      StringBuilder b = new StringBuilder();
      char[] arr = s.toCharArray();

      for(int i = 0; i < arr.length; ++i) {
         b.append((char)(arr[i] + i));
      }

      return b.toString();
   }

   public static String f1_rev(String s) {
      StringBuilder b = new StringBuilder();
      char[] arr = s.toCharArray();

      for(int i = 0; i < arr.length; ++i) {
         b.append((char)(arr[i] - i));
      }

      return b.toString();
   }

   public static String f2(String s) {
      int half = s.length() / 2;
      return s.substring(half + 1) + s.substring(0, half + 1);
   }

   public static String f3() {
      return f1(f2("$aQ\"cNP `_\u001d[eULB@PA'thpj]"));
   }

   public static void main(String[] args) {
      System.out.println("You really thought finding the flag would be so easy?");
   }
}
```

I then created a java file and simply changed the call to f3() in main. Compiled and ran and got:

```
UMDCTF-{pyth0n_1s_b3tt3r}
```