# dvCTF substitution Write Up

## Details:
Points: 25

Jeopardy style CTF

Category: crypto

Comment: 

```
Rm xibkgltizksb, z hfyhgrgfgrlm xrksvi rh z nvgslw lu vmxibkgrmt rm dsrxs fmrgh lu kozrmgvcg ziv ivkozxvw drgs xrksvigvcg, zxxliwrmt gl z urcvw hbhgvn; gsv "fmrgh" nzb yv hrmtov ovggvih (gsv nlhg xlnnlm), kzrih lu ovggvih, girkovgh lu ovggvih, nrcgfivh lu gsv zylev, zmw hl uligs. Gsv ivxvrevi wvxrksvih gsv gvcg yb kviulinrmt gsv rmevihv hfyhgrgfgrlm.

Hfyhgrgfgrlm xrksvih xzm yv xlnkzivw drgs gizmhklhrgrlm xrksvih. Rm z gizmhklhrgrlm xrksvi, gsv fmrgh lu gsv kozrmgvcg ziv ivziizmtvw rm z wruuvivmg zmw fhfzoob jfrgv xlnkovc liwvi, yfg gsv fmrgh gsvnhvoevh ziv ovug fmxszmtvw. Yb xlmgizhg, rm z hfyhgrgfgrlm xrksvi, gsv fmrgh lu gsv kozrmgvcg ziv ivgzrmvw rm gsv hznv hvjfvmxv rm gsv xrksvigvcg, yfg gsv fmrgh gsvnhvoevh ziv zogvivw.

Gsviv ziv z mfnyvi lu wruuvivmg gbkvh lu hfyhgrgfgrlm xrksvi. Ru gsv xrksvi lkvizgvh lm hrmtov ovggvih, rg rh gvinvw z hrnkov hfyhgrgfgrlm xrksvi; z xrksvi gszg lkvizgvh lm ozitvi tilfkh lu ovggvih rh gvinvw klobtizksrx. Z nlmlzokszyvgrx xrksvi fhvh urcvw hfyhgrgfgrlm levi gsv vmgriv nvhhztv, dsvivzh z klobzokszyvgrx xrksvi fhvh z mfnyvi lu hfyhgrgfgrlmh zg wruuvivmg klhrgrlmh rm gsv nvhhztv, dsviv z fmrg uiln gsv kozrmgvcg rh nzkkvw gl lmv lu hvevizo klhhryrorgrvh rm gsv xrksvigvcg zmw erxv evihz.
weXGU{xi1kg3w_x1ks3i}
```

## Write up:

Since the name of this cipher was substitution I decided to see if the cipher could be decoded using a substition cipher. I went online and entered the text into guballa. The output of this was:

```
In cryptography, a substitution cipher is a method of encrypting in which units of plaintext are replaced with ciphertext, according to a fixed system; the "units" may be single letters (the most common), pairs of letters, triplets of letters, mixtures of the above, and so forth. The receiver deciphers the text by performing the inverse substitution.

Substitution ciphers can be compared with transposition ciphers. In a transposition cipher, the units of the plaintext are rearranged in a different and usually quite complex order, but the units themselves are left unchanged. By contrast, in a substitution cipher, the units of the plaintext are retained in the same sequence in the ciphertext, but the units themselves are altered.

There are a number of different types of substitution cipher. If the cipher operates on single letters, it is termed a simple substitution cipher; a cipher that operates on larger groups of letters is termed polygraphic. A monoalphabetic cipher uses fixed substitution over the entire message, whereas a polyalphabetic cipher uses a number of substitutions at different positions in the message, where a unit from the plaintext is mapped to one of several possibilities in the ciphertext and vice versa.
dvCTF{cr1pt3d_c1ph3r}
```