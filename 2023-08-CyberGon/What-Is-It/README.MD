# CyberGon What Is It Write Up

## Details:

Jeopardy style CTF

Category: Reverse Engineering

Comments:

```t
Admin team receive an exe file, and don't know what to do with it. can u figure out ?
```

## Write up:

After I unzipped the file I ran strings on the exe and saw that the executable had a string saying that it was an AutoIt script:

```t
$ strings What\ Is\ it\ .exe | grep -i aut
This is a third-party compiled AutoIt script.
NO_AUTO_POSSESS)
.text$lp00AutoItSC
OLEAUT32.dll
```

This led me to looking up how to extract AutoIt scripts, where I came across the following GitHub repo: https://github.com/nazywam/AutoIt-Ripper

Following the steps in the repository to extract the script I then had the following:

```autoit
Global Const $GLE_86QR_5FKJ2_9 [ 2 ] [ 15 ] = [ [ 25988 , 817748516 , 1282242541 , 43274 , 19762 , 441211631 , 175 , 1767631126 , 127893372 , 10089 , 77 , 455365111 , 15128 , 45 , 395401611 ] , [ 3550 , 47807819 ] ]
Global $TAGCPDJEGTREOOICYVKMRQBZA = Asc ( "U" )
Global Const $UACO_AKG8_RVU_8 [ 7 ] = [ 9667 , 239 , 1887113234 , 93 , 337954058 , 2059407261 , 100 ]
Global $G_BKOBHZZHZGCVTG = 142
Global $VAR_942 = Asc ( "U" )
Global Const $VAR_3369 = Asc ( StringMid ( "Asc(StringMid(""WkVGVOJqtd"", 175 , 1))" , 7 , 1 ) )
#OnAutoItStartRegister "func_1448"
Global $8F_MR_HPOR2J_NAQOO = Asc ( "j" )
Global Const $ATAMHYC_B7_GDH_CRLBZ [ 2 ] [ 13 ] = [ [ 138 , 1730145422 , 567 , 1590971207 , 230 , 20753 , 18859 , 179 , 2067977822 , 910470776 , 47 , 53 , 1367254885 ] , [ 120 , 206 , 1405987413 , 180 , 1434117039 , 64 , 38590 , 1622498676 , 90 , 210 , 1829439044 ] ]
Global Const $VAR_457 [ 4 ] = [ 16341 , 1164395124 , 10841 , 13 ]
Global Const $DXQPW = 9093
Global $SCHYVBXVPTCABB [ 2 ] [ 15 ] = [ [ 909023135 , 943644453 , 363119012 , 34855 , 518762660 , 1695545421 , 47652 , 61380 , 500794320 , 43665 , 1575868947 , 40829 , 41691 , 143 , 22314 ] , [ 1861 , 6 , 243805624 , 30143 , 970247259 , 232 , 1657419056 , 232 , 2107955208 , 21079 , 98 ] ]
Global $OWQUIE_BCPOOELH_COQHYROSFH = Asc ( StringMid ( "Switch 584578608" , 7 , 1 ) )
Global Const $CWPLT_TAXLBWQEQ_UFYSZ [ 8 ] = [ 1999528316 , 38724457 , 1158408436 , 986478819 , 284560648 , 63311 , 199 , 93 ]
#OnAutoItStartRegister "ClduzzmBlancshhoQrvcydgwgt"
Global Const $YEHFWGKW = 710643979
#OnAutoItStartRegister "FnegveFunc"
$DENZTCINQ = "!t_!"
Run ( "notepad.exe" )
Sleep ( 1000 )
$VAR_3992 = "0bfusc4t3d"
For $I = 1 To 5
	Send ( "Random text line " & $I & @CRLF )
	Sleep ( 500 )
Next
$VAR_981 = "_4ut0_"
For $I = 1 To 10
	$SCZIZEFPABKEEQTMPAWGSKMWPUF = "_s0!"
	RegWrite ( "HKEY_CURRENT_USER\Software\TestRandomKeys" & $I , "Key" & $I , "REG_SZ" , "Value" & $I )
Next
$4A6AL5H5GY_LYSM5__ = "_d0nt_"
For $I = 1 To 3
	MsgBox ( 0 , "Random Message " & $I , "This is random message number " & $I )
Next
WinClose ( "Untitled - Notepad" )
$PSTZVYYVBMQPFPKIVSAAKLBX = "th!nk"
Func FNEGVEFUNC ( )
	Global $CAAPQYFEP_MXWCGBIHE = 14294
EndFunc
Func JHYBZGQJVR_WGS__WVC ( )
	Return 1187463921
EndFunc
Func CLDUZZMBLANCSHHOQRVCYDGWGT ( )
	Global $JTVFTVIWH_HCKMTLYG = 52
EndFunc
Func KVWNE__PKNVMRT_ZOJTDRQ_N ( )
	Return 40
EndFunc
Func FUNC_1448 ( )
	Global $VAR_1383 = 2104533279
EndFunc
Func IAHVVFYRTNZRHSIAFUNC ( )
	Return 198
EndFunc
```

I saw that there were multiple variables being set which were not used, I ended up removing everything but those variables and was left with the following:

```autoit
$DENZTCINQ = "!t_!"
$VAR_3992 = "0bfusc4t3d"
$VAR_981 = "_4ut0_"
	$SCZIZEFPABKEEQTMPAWGSKMWPUF = "_s0!"
$4A6AL5H5GY_LYSM5__ = "_d0nt_"
$PSTZVYYVBMQPFPKIVSAAKLBX = "th!nk"
```

From here I simply rearranged those snippets so that they made sense and I ended up with the flag:

```t
CybergonCTF{0bfusc4t3d_4ut0_!t_!_d0nt_th!nk_s0!}
```