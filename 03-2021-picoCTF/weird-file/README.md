# picoCTF Weird File Write Up

## Details:
Points: 20

Jeopardy style CTF

Category: Forensics

Comments: What could go wrong if we let Word documents run programs? (aka "in-the-clear")

## Write up:

Based on the hint we want to see any macros in the file they provided us, we are going to use oletools:

```
olevba -c ./weird.docm                                                                                                                             100 ⨯
olevba 0.56 on Python 3.9.1 - http://decalage.info/python/oletools
===============================================================================
FILE: ./weird.docm
Type: OpenXML
WARNING  For now, VBA stomping cannot be detected for files in memory
-------------------------------------------------------------------------------
VBA MACRO ThisDocument.cls 
in file: word/vbaProject.bin - OLE stream: 'VBA/ThisDocument'
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
Sub AutoOpen()
    MsgBox "Macros can run any program", 0, "Title"
    Signature

End Sub
 
 Sub Signature()
    Selection.TypeText Text:="some text"
    Selection.TypeParagraph
    
 End Sub
 
 Sub runpython()

Dim Ret_Val
Args = """" '"""
Ret_Val = Shell("python -c 'print(\"cGljb0NURnttNGNyMHNfcl9kNG5nM3IwdXN9\")'" & " " & Args, vbNormalFocus)
If Ret_Val = 0 Then
   MsgBox "Couldn't run python script!", vbOKOnly
End If
End Sub
```

I then used a base64 decoder to turn the string into the flag:

```
picoCTF{m4cr0s_r_d4ng3r0us}
```