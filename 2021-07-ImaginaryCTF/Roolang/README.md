# ImaginaryCTF Roolang Write Up

## Details:

Jeopardy style CTF

Category: Reverse Engineering

Points: 400

Comments: 

ELF files are sooooo last year :rooVoid:. Instead of making yet another ELF to reverse, I made a new type of executable binary for you to reverse :rooYay:! It uses my new language Roolang. Just execute flag.roo to get the flag :rooPuzzlerDevil:! It's dynamically linked, so make sure you have the provided roo library files in the same directory :rooCash:. It's not very optimized, so it might take a moment (sorry about that) :rooNobooli:...

Special shoutout to :rooRobin: and :rooOreos:, who have nothing to do with this challenge, but are both very cool people :rooHeart:. Good luck, and I hope you have fun :rooNervous:!

## Write up:

The first thing I did for this challenge was print out the program "opcodes", I simply did this by adding a print before the program starts running.

```py
['rnbon', 'rooon', 'robin', 'rooon', 'robin', 'rbbon', 'rooin', 'rbibn', 'roibn', 'ribin', 'roibn', 'rbibn', 'rioin', 'rbion', 'roiin', 'rbbon', 'rbiin', 'ribin', 'robin', 'rbbon', 'roobn', 'rbibn', 'rboin', 'riibn', 'rbbon', 'rioin', 'robbn', 'robin', 'rbbbn', 'riion', 'roiin', 'ribon', 'robin', 'rboin', 'riiin', 'ribbn', 'riion', 'ribon', 'ribin', 'rbibn', 'rboon', 'ribon', 'rbion', 'rbbbn', 'rbiin', 'robin', 'rboin', 'rbibn', 'rbibn', 'rooon', 'rbiin', 'ribin', 'rbbin', 'ribbn', 'riobn', 'roiin', 'ribin', 'ribon', 'robin', 'rboin', 'rbobn', 'roion', 'riion', 'robbn', 'rooon', 'roobn', 'rbbin', 'rooin', 'roion', 'rioon', 'ribon', 'robin', 'rboin', 'roion', 'rboon', 'robon', 'rbbbn', 'ribin', 'rbbbn', 'roiin', 'rbiin', 'rooin', 'rboon', 'riobn', 'robin', 'rboin', 'robon', 'riion', 'rioin', 'rbiin', 'robon', 'rbion', 'robin', 'rbbon', 'robbn', 'rboon', 'rboon', 'robin', 'rboin', 'rooin', 'rboin', 'rboon', 'ribin', 'riobn', 'riibn', 'robon', 'robbn', 'riion', 'rioon', 'rbibn', 'robin', 'rboin', 'roobn', 'rbbbn', 'rbobn', 'rioin', 'rbobn', 'rbiin', 'rooin', 'roibn', 'roion', 'rbibn', 'ribbn', 'robin', 'rbobn', 'riion', 'riiin', 'robon', 'rboon', 'roiin', 'rooon', 'riion', 'rioon', 'roiin', 'rbobn', 'robin', 'rbobn', 'rbion', 'rboin', 'rbiin', 'roobn', 'rboon', 'roobn', 'rboon', 'rbion', 'rboon', 'rbiin', 'robin', 'rbobn', 'rboon', 'rbbin', 'rbbbn', 'roibn', 'riibn', 'riiin', 'rbion', 'rooin', 'ribbn', 'rbobn', 'robin', 'rbobn', 'robin', 'riion', 'robon', 'rioin', 'rbobn', 'roobn', 'ribon', 'rbbon', 'rbobn', 'rbibn', 'robin', 'rbobn', 'robon', 'rbiin', 'rboon', 'rbbin', 'rbion', 'riion', 'rioin', 'rbiin', 'rboon', 'rboin', 'robin', 'rbobn', 'rooin', 'roion', 'ribon', 'robin', 'ribon', 'robbn', 'rooon', 'ribbn', 'roiin', 'rioin', 'robin', 'rbobn', 'roobn', 'rbobn', 'rbion', 'roiin', 'ribon', 'rioin', 'riobn', 'ribbn', 'roobn', 'rbbon', 'robin', 'rboon', 'ribin', 'robin', 'ribin', 'riiin', 'rbobn', 'roiin', 'rooon', 'roobn', 'rioon', 'robin', 'rboon', 'rbbin', 'rboon', 'rboin', 'ribbn', 'rbobn', 'rboin', 'ribon', 'riiin', 'roibn', 'robin', 'rboon', 'roiin', 'ribin', 'rbbon', 'robon', 'riiin', 'ribin', 'robin', 'robbn', 'robbn', 'robin', 'rboon', 'robin', 'rbbon', 'riiin', 'rioon', 'rbobn', 'rbion', 'rbiin', 'rooon', 'roiin', 'robin', 'rboon', 'robon', 'rbobn', 'rbbon', 'rbbon', 'rbibn', 'roibn', 'rbbin', 'riiin', 'ribbn', 'robin', 'rboon', 'rooin', 'rooin', 'rbbin', 'robin', 'ribon', 'roiin', 'rooin', 'roobn', 'rbion', 'robin', 'rboon', 'roobn', 'roibn', 'riibn', 'roion', 'ribon', 'riiin', 'rbbbn', 'roion', 'riiin', 'robin', 'roiin', 'ribon', 'rbion', 'riibn', 'riiin', 'roiin', 'rbion', 'riobn', 'rbbbn', 'robin', 'roiin', 'rbbbn', 'rboon', 'roibn', 'ribbn', 'rbiin', 'riion', 'rbobn', 'rioin', 'robin', 'roiin', 'roiin', 'roion', 'rioon', 'robon', 'rbiin', 'rioon', 'rbobn', 'rbbbn', 'robin', 'roiin', 'robin', 'rooin', 'rbibn', 'riobn', 'rooon', 'robin', 'rioin', 'roion', 'robin', 'roiin', 'robon', 'robbn', 'roobn', 'rboin', 'rbiin', 'rbbon', 'rbbin', 'ribin', 'robin', 'roiin', 'roobn', 'riibn', 'rbion', 'roibn', 'rboon', 'rioin', 'rboin', 'ribon', 'robin', 'roiin', 'roobn', 'robin', 'rbbbn', 'robbn', 'roibn', 'riobn', 'robbn', 'roion', 'robin', 'roibn', 'rioin', 'rooin', 'robon', 'rooin', 'roobn', 'robon', 'robin', 'robin', 'roibn', 'rbbon', 'rboin', 'roobn', 'robin', 'rioon', 'roibn', 'rbion', 'robin', 'roibn', 'roibn', 'rioon', 'roobn', 'ribin', 'rboon', 'riobn', 'roion', 'robin', 'roibn', 'robbn', 'riobn', 'riiin', 'rboon', 'roiin', 'roion', 'rbbon', 'robin', 'roibn', 'rooin', 'riibn', 'rooin', 'rbbin', 'roobn', 'rbbin', 'rbbbn', 'robin', 'roibn', 'roobn', 'ribon', 'ribin', 'ribbn', 'roion', 'ribbn', 'rioin', 'robin', 'roibn', 'roobn', 'robon', 'robin', 'rioon', 'ribon', 'rbiin', 'robbn', 'robin', 'roion', 'rioon', 'rioon', 'robon', 'rbbon', 'rbobn', 'rbbon', 'robin', 'roion', 'rboin', 'rbbin', 'rbion', 'rboon', 'rooin', 'riion', 'robin', 'roion', 'roibn', 'robon', 'rbion', 'robon', 'rooon', 'rbobn', 'robin', 'roion', 'robbn', 'rboin', 'rooon', 'roion', 'roion', 'roobn', 'robin', 'roion', 'rooin', 'riobn', 'rbbin', 'ribin', 'ribin', 'rbobn', 'robin', 'roion', 'roobn', 'rioon', 'rbbon', 'rboon', 'robon', 'riion', 'robin', 'roion', 'roobn', 'roobn', 'rooin', 'rbbin', 'rbbin', 'roion', 'robin', 'robin', 'rbiin', 'rboon', 'ribon', 'rioin', 'rioin', 'robin', 'robin', 'rbobn', 'riobn', 'riobn', 'ribon', 'rooon', 'robin', 'robin', 'roion', 'rbiin', 'roobn', 'rioin', 'rbion', 'robin', 'robin', 'robbn', 'rooin', 'rioon', 'robon', 'riion', 'robin', 'robin', 'rooin', 'rbbin', 'rbobn', 'rbiin', 'riion', 'robin', 'robin', 'roobn', 'rbion', 'roibn', 'rbbon', 'roion', 'robin', 'robbn', 'riiin', 'robon', 'roiin', 'rbion', 'robin', 'robbn', 'rbibn', 'robon', 'ribon', 'rbion', 'robin', 'robbn', 'rboon', 'riiin', 'rbbbn', 'rbbbn', 'robin', 'robbn', 'roion', 'robbn', 'rbbon', 'roiin', 'robin', 'robbn', 'robon', 'ribbn', 'roobn', 'rbion', 'robin', 'robbn', 'rooin', 'rboon', 'rbiin', 'rbiin', 'robin', 'robbn', 'roobn', 'rbbon', 'robin', 'riion', 'robin', 'robon', 'riion', 'roibn', 'roibn', 'robin', 'robon', 'rbion', 'rooin', 'robbn', 'robin', 'robon', 'rboon', 'robbn', 'rioon', 'robin', 'robon', 'robin', 'rioon', 'riiin', 'robin', 'robon', 'robon', 'rbiin', 'ribbn', 'robin', 'robon', 'rooin', 'robin', 'rbbbn', 'robin', 'robon', 'roobn', 'roiin', 'rbbon', 'robin', 'rooin', 'riobn', 'ribon', 'robin', 'rooin', 'rbobn', 'rbbon', 'robin', 'rooin', 'robbn', 'rioin', 'robin', 'rooin', 'roion', 'rbbin', 'robin', 'rooin', 'roobn', 'rbion', 'robin', 'rooin', 'robon', 'rbbbn', 'robin', 'roobn', 'rooin', 'robin', 'rooin', 'robbn', 'roion', 'robin', 'rooin', 'robbn', 'rooon', 'robin', 'rooin', 'robon', 'riobn', 'robin', 'rooin', 'robon', 'rioon', 'robin', 'rooin', 'roobn', 'roiin', 'robin', 'rooin', 'robon', 'ribbn', 'robin', 'rooin', 'robon', 'riion', 'robin', 'rooin', 'robon', 'robbn', 'rnbon', 'roobn', 'robin', 'rooon', 'rinin', 'rnbon', 'rooin', 'rbion', 'robon', 'rioon', 'robbn', 'rnbon', 'robon', 'robin', 'rinin', 'roiin', 'robin', 'roibn', 'rbiin', 'robin', 'rinin', 'robin', 'roobn', 'roobn', 'riobn', 'rinin', 'rioon', 'rooin', 'rioon', 'robbn', 'rnbon', 'robin', 'rbion', 'roion', 'rboin', 'robin', 'roobn', 'roobn', 'ribbn', 'rnbon', 'roion', 'riiin', 'robin', 'roobn', 'roobn', 'rooon', 'rbion', 'roibn', 'rboin', 'ribbn', 'rnbon', 'roibn', 'rboin', 'riiin', 'robin', 'roobn', 'roobn', 'rooon', 'robin', 'rooon', 'rooon', 'riiin', 'robin', 'roobn', 'roobn', 'rooon', 'robin', 'rooon', 'rooon', 'roiin', 'robin', 'rioin', 'roiin', 'robin', 'riobn', 'rioin', 'rboin', 'ribbn', 'rnbon', 'robbn', 'robin', 'rooon', 'rboin', 'robin', 'rooon', 'rboin']
```

From here I decided that I should write a decompiler for roolang. I built this decompiler in three main steps however I will post the entire program and then explain all of the portions individually:

```py
prg = ['rnbon', 'rooon', 'robin', 'rooon', 'robin', 'rbbon', 'rooin', 'rbibn', 'roibn', 'ribin', 'roibn', 'rbibn', 'rioin', 'rbion', 'roiin', 'rbbon', 'rbiin', 'ribin', 'robin', 'rbbon', 'roobn', 'rbibn', 'rboin', 'riibn', 'rbbon', 'rioin', 'robbn', 'robin', 'rbbbn', 'riion', 'roiin', 'ribon', 'robin', 'rboin', 'riiin', 'ribbn', 'riion', 'ribon', 'ribin', 'rbibn', 'rboon', 'ribon', 'rbion', 'rbbbn', 'rbiin', 'robin', 'rboin', 'rbibn', 'rbibn', 'rooon', 'rbiin', 'ribin', 'rbbin', 'ribbn', 'riobn', 'roiin', 'ribin', 'ribon', 'robin', 'rboin', 'rbobn', 'roion', 'riion', 'robbn', 'rooon', 'roobn', 'rbbin', 'rooin', 'roion', 'rioon', 'ribon', 'robin', 'rboin', 'roion', 'rboon', 'robon', 'rbbbn', 'ribin', 'rbbbn', 'roiin', 'rbiin', 'rooin', 'rboon', 'riobn', 'robin', 'rboin', 'robon', 'riion', 'rioin', 'rbiin', 'robon', 'rbion', 'robin', 'rbbon', 'robbn', 'rboon', 'rboon', 'robin', 'rboin', 'rooin', 'rboin', 'rboon', 'ribin', 'riobn', 'riibn', 'robon', 'robbn', 'riion', 'rioon', 'rbibn', 'robin', 'rboin', 'roobn', 'rbbbn', 'rbobn', 'rioin', 'rbobn', 'rbiin', 'rooin', 'roibn', 'roion', 'rbibn', 'ribbn', 'robin', 'rbobn', 'riion', 'riiin', 'robon', 'rboon', 'roiin', 'rooon', 'riion', 'rioon', 'roiin', 'rbobn', 'robin', 'rbobn', 'rbion', 'rboin', 'rbiin', 'roobn', 'rboon', 'roobn', 'rboon', 'rbion', 'rboon', 'rbiin', 'robin', 'rbobn', 'rboon', 'rbbin', 'rbbbn', 'roibn', 'riibn', 'riiin', 'rbion', 'rooin', 'ribbn', 'rbobn', 'robin', 'rbobn', 'robin', 'riion', 'robon', 'rioin', 'rbobn', 'roobn', 'ribon', 'rbbon', 'rbobn', 'rbibn', 'robin', 'rbobn', 'robon', 'rbiin', 'rboon', 'rbbin', 'rbion', 'riion', 'rioin', 'rbiin', 'rboon', 'rboin', 'robin', 'rbobn', 'rooin', 'roion', 'ribon', 'robin', 'ribon', 'robbn', 'rooon', 'ribbn', 'roiin', 'rioin', 'robin', 'rbobn', 'roobn', 'rbobn', 'rbion', 'roiin', 'ribon', 'rioin', 'riobn', 'ribbn', 'roobn', 'rbbon', 'robin', 'rboon', 'ribin', 'robin', 'ribin', 'riiin', 'rbobn', 'roiin', 'rooon', 'roobn', 'rioon', 'robin', 'rboon', 'rbbin', 'rboon', 'rboin', 'ribbn', 'rbobn', 'rboin', 'ribon', 'riiin', 'roibn', 'robin', 'rboon', 'roiin', 'ribin', 'rbbon', 'robon', 'riiin', 'ribin', 'robin', 'robbn', 'robbn', 'robin', 'rboon', 'robin', 'rbbon', 'riiin', 'rioon', 'rbobn', 'rbion', 'rbiin', 'rooon', 'roiin', 'robin', 'rboon', 'robon', 'rbobn', 'rbbon', 'rbbon', 'rbibn', 'roibn', 'rbbin', 'riiin', 'ribbn', 'robin', 'rboon', 'rooin', 'rooin', 'rbbin', 'robin', 'ribon', 'roiin', 'rooin', 'roobn', 'rbion', 'robin', 'rboon', 'roobn', 'roibn', 'riibn', 'roion', 'ribon', 'riiin', 'rbbbn', 'roion', 'riiin', 'robin', 'roiin', 'ribon', 'rbion', 'riibn', 'riiin', 'roiin', 'rbion', 'riobn', 'rbbbn', 'robin', 'roiin', 'rbbbn', 'rboon', 'roibn', 'ribbn', 'rbiin', 'riion', 'rbobn', 'rioin', 'robin', 'roiin', 'roiin', 'roion', 'rioon', 'robon', 'rbiin', 'rioon', 'rbobn', 'rbbbn', 'robin', 'roiin', 'robin', 'rooin', 'rbibn', 'riobn', 'rooon', 'robin', 'rioin', 'roion', 'robin', 'roiin', 'robon', 'robbn', 'roobn', 'rboin', 'rbiin', 'rbbon', 'rbbin', 'ribin', 'robin', 'roiin', 'roobn', 'riibn', 'rbion', 'roibn', 'rboon', 'rioin', 'rboin', 'ribon', 'robin', 'roiin', 'roobn', 'robin', 'rbbbn', 'robbn', 'roibn', 'riobn', 'robbn', 'roion', 'robin', 'roibn', 'rioin', 'rooin', 'robon', 'rooin', 'roobn', 'robon', 'robin', 'robin', 'roibn', 'rbbon', 'rboin', 'roobn', 'robin', 'rioon', 'roibn', 'rbion', 'robin', 'roibn', 'roibn', 'rioon', 'roobn', 'ribin', 'rboon', 'riobn', 'roion', 'robin', 'roibn', 'robbn', 'riobn', 'riiin', 'rboon', 'roiin', 'roion', 'rbbon', 'robin', 'roibn', 'rooin', 'riibn', 'rooin', 'rbbin', 'roobn', 'rbbin', 'rbbbn', 'robin', 'roibn', 'roobn', 'ribon', 'ribin', 'ribbn', 'roion', 'ribbn', 'rioin', 'robin', 'roibn', 'roobn', 'robon', 'robin', 'rioon', 'ribon', 'rbiin', 'robbn', 'robin', 'roion', 'rioon', 'rioon', 'robon', 'rbbon', 'rbobn', 'rbbon', 'robin', 'roion', 'rboin', 'rbbin', 'rbion', 'rboon', 'rooin', 'riion', 'robin', 'roion', 'roibn', 'robon', 'rbion', 'robon', 'rooon', 'rbobn', 'robin', 'roion', 'robbn', 'rboin', 'rooon', 'roion', 'roion', 'roobn', 'robin', 'roion', 'rooin', 'riobn', 'rbbin', 'ribin', 'ribin', 'rbobn', 'robin', 'roion', 'roobn', 'rioon', 'rbbon', 'rboon', 'robon', 'riion', 'robin', 'roion', 'roobn', 'roobn', 'rooin', 'rbbin', 'rbbin', 'roion', 'robin', 'robin', 'rbiin', 'rboon', 'ribon', 'rioin', 'rioin', 'robin', 'robin', 'rbobn', 'riobn', 'riobn', 'ribon', 'rooon', 'robin', 'robin', 'roion', 'rbiin', 'roobn', 'rioin', 'rbion', 'robin', 'robin', 'robbn', 'rooin', 'rioon', 'robon', 'riion', 'robin', 'robin', 'rooin', 'rbbin', 'rbobn', 'rbiin', 'riion', 'robin', 'robin', 'roobn', 'rbion', 'roibn', 'rbbon', 'roion', 'robin', 'robbn', 'riiin', 'robon', 'roiin', 'rbion', 'robin', 'robbn', 'rbibn', 'robon', 'ribon', 'rbion', 'robin', 'robbn', 'rboon', 'riiin', 'rbbbn', 'rbbbn', 'robin', 'robbn', 'roion', 'robbn', 'rbbon', 'roiin', 'robin', 'robbn', 'robon', 'ribbn', 'roobn', 'rbion', 'robin', 'robbn', 'rooin', 'rboon', 'rbiin', 'rbiin', 'robin', 'robbn', 'roobn', 'rbbon', 'robin', 'riion', 'robin', 'robon', 'riion', 'roibn', 'roibn', 'robin', 'robon', 'rbion', 'rooin', 'robbn', 'robin', 'robon', 'rboon', 'robbn', 'rioon', 'robin', 'robon', 'robin', 'rioon', 'riiin', 'robin', 'robon', 'robon', 'rbiin', 'ribbn', 'robin', 'robon', 'rooin', 'robin', 'rbbbn', 'robin', 'robon', 'roobn', 'roiin', 'rbbon', 'robin', 'rooin', 'riobn', 'ribon', 'robin', 'rooin', 'rbobn', 'rbbon', 'robin', 'rooin', 'robbn', 'rioin', 'robin', 'rooin', 'roion', 'rbbin', 'robin', 'rooin', 'roobn', 'rbion', 'robin', 'rooin', 'robon', 'rbbbn', 'robin', 'roobn', 'rooin', 'robin', 'rooin', 'robbn', 'roion', 'robin', 'rooin', 'robbn', 'rooon', 'robin', 'rooin', 'robon', 'riobn', 'robin', 'rooin', 'robon', 'rioon', 'robin', 'rooin', 'roobn', 'roiin', 'robin', 'rooin', 'robon', 'ribbn', 'robin', 'rooin', 'robon', 'riion', 'robin', 'rooin', 'robon', 'robbn', 'rnbon', 'roobn', 'robin', 'rooon', 'rinin', 'rnbon', 'rooin', 'rbion', 'robon', 'rioon', 'robbn', 'rnbon', 'robon', 'robin', 'rinin', 'roiin', 'robin', 'roibn', 'rbiin', 'robin', 'rinin', 'robin', 'roobn', 'roobn', 'riobn', 'rinin', 'rioon', 'rooin', 'rioon', 'robbn', 'rnbon', 'robin', 'rbion', 'roion', 'rboin', 'robin', 'roobn', 'roobn', 'ribbn', 'rnbon', 'roion', 'riiin', 'robin', 'roobn', 'roobn', 'rooon', 'rbion', 'roibn', 'rboin', 'ribbn', 'rnbon', 'roibn', 'rboin', 'riiin', 'robin', 'roobn', 'roobn', 'rooon', 'robin', 'rooon', 'rooon', 'riiin', 'robin', 'roobn', 'roobn', 'rooon', 'robin', 'rooon', 'rooon', 'roiin', 'robin', 'rioin', 'roiin', 'robin', 'riobn', 'rioin', 'rboin', 'ribbn', 'rnbon', 'robbn', 'robin', 'rooon', 'rboin', 'robin', 'rooon', 'rboin']

nums = []

def dsbmle(i):
	cur = prg[i]
	if cur == 'rboin':
		print(i, cur, 'IPOP     // Pop Stack')
	elif cur == 'riobn':
		print(i, cur, 'IADD     // Pop 2 and Add')
	elif cur == 'rooon':
		print(i, cur, 'ISUB     // Pop 2 and Subtract')
	elif cur == 'riibn':
		print(i, cur, 'IMUL     // Pop 2 and Multiply')
	elif cur == 'riion':
		print(i, cur, 'IDIV     // Pop 2 and Divide')
	elif cur == 'ribon':
		print(i, cur, 'IMOD     // Pop 2 and Mod')
	elif cur == 'ronon':
		print(i, cur, 'IBITADD  // Pop 2 and And')
	elif cur == 'roion':
		print(i, cur, 'IBITOR   // Pop 2 and Or')
	elif cur == 'roibn':
		print(i, cur, 'IXOR     // Pop 2 and Xor')
	elif cur == 'riiin':
		print(i, cur, 'DUPTOP   // Pop 1 and Add 2 of self to Stack')
	elif cur == 'rioin':
		print(i, cur, 'REVTOP   // Pop 2 and Add in opposite order')
	elif cur == 'rinin':
		print(i, cur, 'TOREG    // Pop 1 and save to register')
	elif cur == 'rbiin':
		print(i, cur, 'PRNTCHR  // Pop 1 and print as char')
	elif cur == 'rboon':
		print(i, cur, 'PRINT    // Pop 1 and print')
	elif cur == 'rnbon':
		print(i, cur, 'INCPTR   // Increase ip pointer')
	elif cur == 'rioon':
		print(i, cur, 'SETPTR   // Set pointer to 1, 652, 657, 663, 681, 691, 702, or 730')
	elif cur == 'rbion':
		print(i, cur, 'SETORINC // If we can peek the stack call rioon, else increase pointer')
	elif cur == 'ribbn':
		print(i, cur, 'RET      // Pop 2, first pop is pushed back to stack after pops, second pop sets pointer')
	elif cur == 'roiin':
		print(i, cur, 'FUNCCALL // Pop and save, Push pointer plus 1, push saved pop, call rioon')
	elif cur == 'robin':
		print(i, cur, 'If next value is rinin then push register, else push a newly calculated value')
	else:
		print(i, cur)

def findCall(i):
	cur = prg[i]
	if cur == 'rooon':
		return 1
	elif cur == 'roobn':
		return 652
	elif cur == 'rooin':
		return 657
	elif cur == 'robon':
		return 663
	elif cur == 'robin':
		return 682
	elif cur == 'roion':
		return 691
	elif cur == 'roibin':
		return 702
	elif cur == 'robbn':
		return 730

def parseDigit(s):
	return int(s.replace('o', '0').replace('b', '1').replace('i', '2')[1:-1], 3)

def findFuncs():
	inFunc = False
	i = 0
	while i < len(prg)-1:
		cur = prg[i]
		if prg[i] == 'rnbon':
			print('function' + str(i+1) + '():')
			i += 1
			inFunc = True
		if inFunc:
			if cur == 'riobn':
				print('\tIADD')
			elif cur == 'rooon':
				print('\tISUB')
			elif cur == 'riibn':
				print('\tIMUL')
			elif cur == 'riion':
				print('\tIDIV')
			elif cur == 'ribon':
				print('\tIMOD')
			elif cur == 'ronon':
				print('\tADD')
			elif cur == 'roion':
				print('\tOR')
			elif cur == 'roibn':
				print('\tXOR')
			elif cur == 'rbiin':
				print('\tPRNTCHR')
			elif cur == 'rboon':
				print('\tPRINT')
			elif cur == 'riiin':
				print('\tx = Pop(); Push(x); Push(x)')
			elif cur == 'rioin':
				print('\tx = Pop(); y = Pop(); Push(x); Push(y)')
			elif cur == 'rinin':
				print('\tregister = Pop()')
			elif cur == 'roiin':
				i += 1
				print('\tCall: ', findCall(i))
			elif cur == 'rbion':
				x = i
				print('\tIF stack.Peek() != 0:')
				i += 1
				print('\t\tCall: ', findCall(i))
				print('\tElse:')
				i = x + 1
			elif cur == 'robin':
				i += 1
				if prg[i] == 'rinin':
					print('\tPush(register)')
				else:
					x = i
					words = parseDigit(prg[i])
					toPush = 0
					for j in range(words):
						i += 1
						toPush += parseDigit(prg[i])
							toPush *= 27
						nums.append(toPush // 27)
						print('\tPush(' + str(toPush // 27) + ')')
			elif cur == 'rboin':
				print('\tPop()')


			elif cur == 'ribbn':
				print('\tRET')
				inFunc = False
		i+=1
	

for i in range(0, len(prg)-1):
	dsbmle(i)

print()

findFuncs()

print(nums)
```

The first section is the disassembly:

```py
for i in range(0, len(prg)-1):
	dsbmle(i)
```

This part of the program print out all the opcodes as well as the instructions and some comments.

The next section prints out all the functions and the actual decompilation:

```py
findFuncs()
```

This part of the script runs through the program as the processor would and write out how the code would be run.

```py
def findCall(i):
	cur = prg[i]
	if cur == 'rooon':
		return 1
	elif cur == 'roobn':
		return 652
	elif cur == 'rooin':
		return 657
	elif cur == 'robon':
		return 663
	elif cur == 'robin':
		return 682
	elif cur == 'roion':
		return 691
	elif cur == 'roibin':
		return 702
	elif cur == 'robbn':
		return 730
```

This function is extracted and changed from the original a little bit since rather than looping through the entire program I got the function starts and what code brings us to that function.

The last component of the script prints all the numbers that we push to the stack, I added this because I needed to extract those for the solve script.

When run the program outputs:

```
0 rnbon INCPTR   // Increase ip pointer
1 rooon ISUB     // Pop 2 and Subtract
2 robin If next value is rinin then push register, else push a newly calculated value
3 rooon ISUB     // Pop 2 and Subtract
4 robin If next value is rinin then push register, else push a newly calculated value
5 rbbon
6 rooin
7 rbibn
8 roibn IXOR     // Pop 2 and Xor
9 ribin
10 roibn IXOR     // Pop 2 and Xor
11 rbibn
12 rioin REVTOP   // Pop 2 and Add in opposite order
13 rbion SETORINC // If we can peek the stack call rioon, else increase pointer
14 roiin FUNCCALL // Pop and save, Push pointer plus 1, push saved pop, call rioon
15 rbbon
16 rbiin PRNTCHR  // Pop 1 and print as char
17 ribin
18 robin If next value is rinin then push register, else push a newly calculated value
19 rbbon
20 roobn
21 rbibn
22 rboin IPOP     // Pop Stack
23 riibn IMUL     // Pop 2 and Multiply
24 rbbon
25 rioin REVTOP   // Pop 2 and Add in opposite order
26 robbn
27 robin If next value is rinin then push register, else push a newly calculated value
28 rbbbn
29 riion IDIV     // Pop 2 and Divide
30 roiin FUNCCALL // Pop and save, Push pointer plus 1, push saved pop, call rioon
31 ribon IMOD     // Pop 2 and Mod
32 robin If next value is rinin then push register, else push a newly calculated value
33 rboin IPOP     // Pop Stack
34 riiin DUPTOP   // Pop 1 and Add 2 of self to Stack
35 ribbn RET      // Pop 2, first pop is pushed back to stack after pops, second pop sets pointer
36 riion IDIV     // Pop 2 and Divide
37 ribon IMOD     // Pop 2 and Mod
38 ribin
39 rbibn
40 rboon PRINT    // Pop 1 and print
41 ribon IMOD     // Pop 2 and Mod
42 rbion SETORINC // If we can peek the stack call rioon, else increase pointer
43 rbbbn
44 rbiin PRNTCHR  // Pop 1 and print as char
45 robin If next value is rinin then push register, else push a newly calculated value
46 rboin IPOP     // Pop Stack
47 rbibn
48 rbibn
49 rooon ISUB     // Pop 2 and Subtract
50 rbiin PRNTCHR  // Pop 1 and print as char
51 ribin
52 rbbin
53 ribbn RET      // Pop 2, first pop is pushed back to stack after pops, second pop sets pointer
54 riobn IADD     // Pop 2 and Add
55 roiin FUNCCALL // Pop and save, Push pointer plus 1, push saved pop, call rioon
56 ribin
57 ribon IMOD     // Pop 2 and Mod
58 robin If next value is rinin then push register, else push a newly calculated value
59 rboin IPOP     // Pop Stack
60 rbobn
61 roion IBITOR   // Pop 2 and Or
62 riion IDIV     // Pop 2 and Divide
63 robbn
64 rooon ISUB     // Pop 2 and Subtract
65 roobn
66 rbbin
67 rooin
68 roion IBITOR   // Pop 2 and Or
69 rioon SETPTR   // Set pointer to 1, 652, 657, 663, 681, 691, 702, or 730
70 ribon IMOD     // Pop 2 and Mod
71 robin If next value is rinin then push register, else push a newly calculated value
72 rboin IPOP     // Pop Stack
73 roion IBITOR   // Pop 2 and Or
74 rboon PRINT    // Pop 1 and print
75 robon
76 rbbbn
77 ribin
78 rbbbn
79 roiin FUNCCALL // Pop and save, Push pointer plus 1, push saved pop, call rioon
80 rbiin PRNTCHR  // Pop 1 and print as char
81 rooin
82 rboon PRINT    // Pop 1 and print
83 riobn IADD     // Pop 2 and Add
84 robin If next value is rinin then push register, else push a newly calculated value
85 rboin IPOP     // Pop Stack
86 robon
87 riion IDIV     // Pop 2 and Divide
88 rioin REVTOP   // Pop 2 and Add in opposite order
89 rbiin PRNTCHR  // Pop 1 and print as char
90 robon
91 rbion SETORINC // If we can peek the stack call rioon, else increase pointer
92 robin If next value is rinin then push register, else push a newly calculated value
93 rbbon
94 robbn
95 rboon PRINT    // Pop 1 and print
96 rboon PRINT    // Pop 1 and print
97 robin If next value is rinin then push register, else push a newly calculated value
98 rboin IPOP     // Pop Stack
99 rooin
100 rboin IPOP     // Pop Stack
101 rboon PRINT    // Pop 1 and print
102 ribin
103 riobn IADD     // Pop 2 and Add
104 riibn IMUL     // Pop 2 and Multiply
105 robon
106 robbn
107 riion IDIV     // Pop 2 and Divide
108 rioon SETPTR   // Set pointer to 1, 652, 657, 663, 681, 691, 702, or 730
109 rbibn
110 robin If next value is rinin then push register, else push a newly calculated value
111 rboin IPOP     // Pop Stack
112 roobn
113 rbbbn
114 rbobn
115 rioin REVTOP   // Pop 2 and Add in opposite order
116 rbobn
117 rbiin PRNTCHR  // Pop 1 and print as char
118 rooin
119 roibn IXOR     // Pop 2 and Xor
120 roion IBITOR   // Pop 2 and Or
121 rbibn
122 ribbn RET      // Pop 2, first pop is pushed back to stack after pops, second pop sets pointer
123 robin If next value is rinin then push register, else push a newly calculated value
124 rbobn
125 riion IDIV     // Pop 2 and Divide
126 riiin DUPTOP   // Pop 1 and Add 2 of self to Stack
127 robon
128 rboon PRINT    // Pop 1 and print
129 roiin FUNCCALL // Pop and save, Push pointer plus 1, push saved pop, call rioon
130 rooon ISUB     // Pop 2 and Subtract
131 riion IDIV     // Pop 2 and Divide
132 rioon SETPTR   // Set pointer to 1, 652, 657, 663, 681, 691, 702, or 730
133 roiin FUNCCALL // Pop and save, Push pointer plus 1, push saved pop, call rioon
134 rbobn
135 robin If next value is rinin then push register, else push a newly calculated value
136 rbobn
137 rbion SETORINC // If we can peek the stack call rioon, else increase pointer
138 rboin IPOP     // Pop Stack
139 rbiin PRNTCHR  // Pop 1 and print as char
140 roobn
141 rboon PRINT    // Pop 1 and print
142 roobn
143 rboon PRINT    // Pop 1 and print
144 rbion SETORINC // If we can peek the stack call rioon, else increase pointer
145 rboon PRINT    // Pop 1 and print
146 rbiin PRNTCHR  // Pop 1 and print as char
147 robin If next value is rinin then push register, else push a newly calculated value
148 rbobn
149 rboon PRINT    // Pop 1 and print
150 rbbin
151 rbbbn
152 roibn IXOR     // Pop 2 and Xor
153 riibn IMUL     // Pop 2 and Multiply
154 riiin DUPTOP   // Pop 1 and Add 2 of self to Stack
155 rbion SETORINC // If we can peek the stack call rioon, else increase pointer
156 rooin
157 ribbn RET      // Pop 2, first pop is pushed back to stack after pops, second pop sets pointer
158 rbobn
159 robin If next value is rinin then push register, else push a newly calculated value
160 rbobn
161 robin If next value is rinin then push register, else push a newly calculated value
162 riion IDIV     // Pop 2 and Divide
163 robon
164 rioin REVTOP   // Pop 2 and Add in opposite order
165 rbobn
166 roobn
167 ribon IMOD     // Pop 2 and Mod
168 rbbon
169 rbobn
170 rbibn
171 robin If next value is rinin then push register, else push a newly calculated value
172 rbobn
173 robon
174 rbiin PRNTCHR  // Pop 1 and print as char
175 rboon PRINT    // Pop 1 and print
176 rbbin
177 rbion SETORINC // If we can peek the stack call rioon, else increase pointer
178 riion IDIV     // Pop 2 and Divide
179 rioin REVTOP   // Pop 2 and Add in opposite order
180 rbiin PRNTCHR  // Pop 1 and print as char
181 rboon PRINT    // Pop 1 and print
182 rboin IPOP     // Pop Stack
183 robin If next value is rinin then push register, else push a newly calculated value
184 rbobn
185 rooin
186 roion IBITOR   // Pop 2 and Or
187 ribon IMOD     // Pop 2 and Mod
188 robin If next value is rinin then push register, else push a newly calculated value
189 ribon IMOD     // Pop 2 and Mod
190 robbn
191 rooon ISUB     // Pop 2 and Subtract
192 ribbn RET      // Pop 2, first pop is pushed back to stack after pops, second pop sets pointer
193 roiin FUNCCALL // Pop and save, Push pointer plus 1, push saved pop, call rioon
194 rioin REVTOP   // Pop 2 and Add in opposite order
195 robin If next value is rinin then push register, else push a newly calculated value
196 rbobn
197 roobn
198 rbobn
199 rbion SETORINC // If we can peek the stack call rioon, else increase pointer
200 roiin FUNCCALL // Pop and save, Push pointer plus 1, push saved pop, call rioon
201 ribon IMOD     // Pop 2 and Mod
202 rioin REVTOP   // Pop 2 and Add in opposite order
203 riobn IADD     // Pop 2 and Add
204 ribbn RET      // Pop 2, first pop is pushed back to stack after pops, second pop sets pointer
205 roobn
206 rbbon
207 robin If next value is rinin then push register, else push a newly calculated value
208 rboon PRINT    // Pop 1 and print
209 ribin
210 robin If next value is rinin then push register, else push a newly calculated value
211 ribin
212 riiin DUPTOP   // Pop 1 and Add 2 of self to Stack
213 rbobn
214 roiin FUNCCALL // Pop and save, Push pointer plus 1, push saved pop, call rioon
215 rooon ISUB     // Pop 2 and Subtract
216 roobn
217 rioon SETPTR   // Set pointer to 1, 652, 657, 663, 681, 691, 702, or 730
218 robin If next value is rinin then push register, else push a newly calculated value
219 rboon PRINT    // Pop 1 and print
220 rbbin
221 rboon PRINT    // Pop 1 and print
222 rboin IPOP     // Pop Stack
223 ribbn RET      // Pop 2, first pop is pushed back to stack after pops, second pop sets pointer
224 rbobn
225 rboin IPOP     // Pop Stack
226 ribon IMOD     // Pop 2 and Mod
227 riiin DUPTOP   // Pop 1 and Add 2 of self to Stack
228 roibn IXOR     // Pop 2 and Xor
229 robin If next value is rinin then push register, else push a newly calculated value
230 rboon PRINT    // Pop 1 and print
231 roiin FUNCCALL // Pop and save, Push pointer plus 1, push saved pop, call rioon
232 ribin
233 rbbon
234 robon
235 riiin DUPTOP   // Pop 1 and Add 2 of self to Stack
236 ribin
237 robin If next value is rinin then push register, else push a newly calculated value
238 robbn
239 robbn
240 robin If next value is rinin then push register, else push a newly calculated value
241 rboon PRINT    // Pop 1 and print
242 robin If next value is rinin then push register, else push a newly calculated value
243 rbbon
244 riiin DUPTOP   // Pop 1 and Add 2 of self to Stack
245 rioon SETPTR   // Set pointer to 1, 652, 657, 663, 681, 691, 702, or 730
246 rbobn
247 rbion SETORINC // If we can peek the stack call rioon, else increase pointer
248 rbiin PRNTCHR  // Pop 1 and print as char
249 rooon ISUB     // Pop 2 and Subtract
250 roiin FUNCCALL // Pop and save, Push pointer plus 1, push saved pop, call rioon
251 robin If next value is rinin then push register, else push a newly calculated value
252 rboon PRINT    // Pop 1 and print
253 robon
254 rbobn
255 rbbon
256 rbbon
257 rbibn
258 roibn IXOR     // Pop 2 and Xor
259 rbbin
260 riiin DUPTOP   // Pop 1 and Add 2 of self to Stack
261 ribbn RET      // Pop 2, first pop is pushed back to stack after pops, second pop sets pointer
262 robin If next value is rinin then push register, else push a newly calculated value
263 rboon PRINT    // Pop 1 and print
264 rooin
265 rooin
266 rbbin
267 robin If next value is rinin then push register, else push a newly calculated value
268 ribon IMOD     // Pop 2 and Mod
269 roiin FUNCCALL // Pop and save, Push pointer plus 1, push saved pop, call rioon
270 rooin
271 roobn
272 rbion SETORINC // If we can peek the stack call rioon, else increase pointer
273 robin If next value is rinin then push register, else push a newly calculated value
274 rboon PRINT    // Pop 1 and print
275 roobn
276 roibn IXOR     // Pop 2 and Xor
277 riibn IMUL     // Pop 2 and Multiply
278 roion IBITOR   // Pop 2 and Or
279 ribon IMOD     // Pop 2 and Mod
280 riiin DUPTOP   // Pop 1 and Add 2 of self to Stack
281 rbbbn
282 roion IBITOR   // Pop 2 and Or
283 riiin DUPTOP   // Pop 1 and Add 2 of self to Stack
284 robin If next value is rinin then push register, else push a newly calculated value
285 roiin FUNCCALL // Pop and save, Push pointer plus 1, push saved pop, call rioon
286 ribon IMOD     // Pop 2 and Mod
287 rbion SETORINC // If we can peek the stack call rioon, else increase pointer
288 riibn IMUL     // Pop 2 and Multiply
289 riiin DUPTOP   // Pop 1 and Add 2 of self to Stack
290 roiin FUNCCALL // Pop and save, Push pointer plus 1, push saved pop, call rioon
291 rbion SETORINC // If we can peek the stack call rioon, else increase pointer
292 riobn IADD     // Pop 2 and Add
293 rbbbn
294 robin If next value is rinin then push register, else push a newly calculated value
295 roiin FUNCCALL // Pop and save, Push pointer plus 1, push saved pop, call rioon
296 rbbbn
297 rboon PRINT    // Pop 1 and print
298 roibn IXOR     // Pop 2 and Xor
299 ribbn RET      // Pop 2, first pop is pushed back to stack after pops, second pop sets pointer
300 rbiin PRNTCHR  // Pop 1 and print as char
301 riion IDIV     // Pop 2 and Divide
302 rbobn
303 rioin REVTOP   // Pop 2 and Add in opposite order
304 robin If next value is rinin then push register, else push a newly calculated value
305 roiin FUNCCALL // Pop and save, Push pointer plus 1, push saved pop, call rioon
306 roiin FUNCCALL // Pop and save, Push pointer plus 1, push saved pop, call rioon
307 roion IBITOR   // Pop 2 and Or
308 rioon SETPTR   // Set pointer to 1, 652, 657, 663, 681, 691, 702, or 730
309 robon
310 rbiin PRNTCHR  // Pop 1 and print as char
311 rioon SETPTR   // Set pointer to 1, 652, 657, 663, 681, 691, 702, or 730
312 rbobn
313 rbbbn
314 robin If next value is rinin then push register, else push a newly calculated value
315 roiin FUNCCALL // Pop and save, Push pointer plus 1, push saved pop, call rioon
316 robin If next value is rinin then push register, else push a newly calculated value
317 rooin
318 rbibn
319 riobn IADD     // Pop 2 and Add
320 rooon ISUB     // Pop 2 and Subtract
321 robin If next value is rinin then push register, else push a newly calculated value
322 rioin REVTOP   // Pop 2 and Add in opposite order
323 roion IBITOR   // Pop 2 and Or
324 robin If next value is rinin then push register, else push a newly calculated value
325 roiin FUNCCALL // Pop and save, Push pointer plus 1, push saved pop, call rioon
326 robon
327 robbn
328 roobn
329 rboin IPOP     // Pop Stack
330 rbiin PRNTCHR  // Pop 1 and print as char
331 rbbon
332 rbbin
333 ribin
334 robin If next value is rinin then push register, else push a newly calculated value
335 roiin FUNCCALL // Pop and save, Push pointer plus 1, push saved pop, call rioon
336 roobn
337 riibn IMUL     // Pop 2 and Multiply
338 rbion SETORINC // If we can peek the stack call rioon, else increase pointer
339 roibn IXOR     // Pop 2 and Xor
340 rboon PRINT    // Pop 1 and print
341 rioin REVTOP   // Pop 2 and Add in opposite order
342 rboin IPOP     // Pop Stack
343 ribon IMOD     // Pop 2 and Mod
344 robin If next value is rinin then push register, else push a newly calculated value
345 roiin FUNCCALL // Pop and save, Push pointer plus 1, push saved pop, call rioon
346 roobn
347 robin If next value is rinin then push register, else push a newly calculated value
348 rbbbn
349 robbn
350 roibn IXOR     // Pop 2 and Xor
351 riobn IADD     // Pop 2 and Add
352 robbn
353 roion IBITOR   // Pop 2 and Or
354 robin If next value is rinin then push register, else push a newly calculated value
355 roibn IXOR     // Pop 2 and Xor
356 rioin REVTOP   // Pop 2 and Add in opposite order
357 rooin
358 robon
359 rooin
360 roobn
361 robon
362 robin If next value is rinin then push register, else push a newly calculated value
363 robin If next value is rinin then push register, else push a newly calculated value
364 roibn IXOR     // Pop 2 and Xor
365 rbbon
366 rboin IPOP     // Pop Stack
367 roobn
368 robin If next value is rinin then push register, else push a newly calculated value
369 rioon SETPTR   // Set pointer to 1, 652, 657, 663, 681, 691, 702, or 730
370 roibn IXOR     // Pop 2 and Xor
371 rbion SETORINC // If we can peek the stack call rioon, else increase pointer
372 robin If next value is rinin then push register, else push a newly calculated value
373 roibn IXOR     // Pop 2 and Xor
374 roibn IXOR     // Pop 2 and Xor
375 rioon SETPTR   // Set pointer to 1, 652, 657, 663, 681, 691, 702, or 730
376 roobn
377 ribin
378 rboon PRINT    // Pop 1 and print
379 riobn IADD     // Pop 2 and Add
380 roion IBITOR   // Pop 2 and Or
381 robin If next value is rinin then push register, else push a newly calculated value
382 roibn IXOR     // Pop 2 and Xor
383 robbn
384 riobn IADD     // Pop 2 and Add
385 riiin DUPTOP   // Pop 1 and Add 2 of self to Stack
386 rboon PRINT    // Pop 1 and print
387 roiin FUNCCALL // Pop and save, Push pointer plus 1, push saved pop, call rioon
388 roion IBITOR   // Pop 2 and Or
389 rbbon
390 robin If next value is rinin then push register, else push a newly calculated value
391 roibn IXOR     // Pop 2 and Xor
392 rooin
393 riibn IMUL     // Pop 2 and Multiply
394 rooin
395 rbbin
396 roobn
397 rbbin
398 rbbbn
399 robin If next value is rinin then push register, else push a newly calculated value
400 roibn IXOR     // Pop 2 and Xor
401 roobn
402 ribon IMOD     // Pop 2 and Mod
403 ribin
404 ribbn RET      // Pop 2, first pop is pushed back to stack after pops, second pop sets pointer
405 roion IBITOR   // Pop 2 and Or
406 ribbn RET      // Pop 2, first pop is pushed back to stack after pops, second pop sets pointer
407 rioin REVTOP   // Pop 2 and Add in opposite order
408 robin If next value is rinin then push register, else push a newly calculated value
409 roibn IXOR     // Pop 2 and Xor
410 roobn
411 robon
412 robin If next value is rinin then push register, else push a newly calculated value
413 rioon SETPTR   // Set pointer to 1, 652, 657, 663, 681, 691, 702, or 730
414 ribon IMOD     // Pop 2 and Mod
415 rbiin PRNTCHR  // Pop 1 and print as char
416 robbn
417 robin If next value is rinin then push register, else push a newly calculated value
418 roion IBITOR   // Pop 2 and Or
419 rioon SETPTR   // Set pointer to 1, 652, 657, 663, 681, 691, 702, or 730
420 rioon SETPTR   // Set pointer to 1, 652, 657, 663, 681, 691, 702, or 730
421 robon
422 rbbon
423 rbobn
424 rbbon
425 robin If next value is rinin then push register, else push a newly calculated value
426 roion IBITOR   // Pop 2 and Or
427 rboin IPOP     // Pop Stack
428 rbbin
429 rbion SETORINC // If we can peek the stack call rioon, else increase pointer
430 rboon PRINT    // Pop 1 and print
431 rooin
432 riion IDIV     // Pop 2 and Divide
433 robin If next value is rinin then push register, else push a newly calculated value
434 roion IBITOR   // Pop 2 and Or
435 roibn IXOR     // Pop 2 and Xor
436 robon
437 rbion SETORINC // If we can peek the stack call rioon, else increase pointer
438 robon
439 rooon ISUB     // Pop 2 and Subtract
440 rbobn
441 robin If next value is rinin then push register, else push a newly calculated value
442 roion IBITOR   // Pop 2 and Or
443 robbn
444 rboin IPOP     // Pop Stack
445 rooon ISUB     // Pop 2 and Subtract
446 roion IBITOR   // Pop 2 and Or
447 roion IBITOR   // Pop 2 and Or
448 roobn
449 robin If next value is rinin then push register, else push a newly calculated value
450 roion IBITOR   // Pop 2 and Or
451 rooin
452 riobn IADD     // Pop 2 and Add
453 rbbin
454 ribin
455 ribin
456 rbobn
457 robin If next value is rinin then push register, else push a newly calculated value
458 roion IBITOR   // Pop 2 and Or
459 roobn
460 rioon SETPTR   // Set pointer to 1, 652, 657, 663, 681, 691, 702, or 730
461 rbbon
462 rboon PRINT    // Pop 1 and print
463 robon
464 riion IDIV     // Pop 2 and Divide
465 robin If next value is rinin then push register, else push a newly calculated value
466 roion IBITOR   // Pop 2 and Or
467 roobn
468 roobn
469 rooin
470 rbbin
471 rbbin
472 roion IBITOR   // Pop 2 and Or
473 robin If next value is rinin then push register, else push a newly calculated value
474 robin If next value is rinin then push register, else push a newly calculated value
475 rbiin PRNTCHR  // Pop 1 and print as char
476 rboon PRINT    // Pop 1 and print
477 ribon IMOD     // Pop 2 and Mod
478 rioin REVTOP   // Pop 2 and Add in opposite order
479 rioin REVTOP   // Pop 2 and Add in opposite order
480 robin If next value is rinin then push register, else push a newly calculated value
481 robin If next value is rinin then push register, else push a newly calculated value
482 rbobn
483 riobn IADD     // Pop 2 and Add
484 riobn IADD     // Pop 2 and Add
485 ribon IMOD     // Pop 2 and Mod
486 rooon ISUB     // Pop 2 and Subtract
487 robin If next value is rinin then push register, else push a newly calculated value
488 robin If next value is rinin then push register, else push a newly calculated value
489 roion IBITOR   // Pop 2 and Or
490 rbiin PRNTCHR  // Pop 1 and print as char
491 roobn
492 rioin REVTOP   // Pop 2 and Add in opposite order
493 rbion SETORINC // If we can peek the stack call rioon, else increase pointer
494 robin If next value is rinin then push register, else push a newly calculated value
495 robin If next value is rinin then push register, else push a newly calculated value
496 robbn
497 rooin
498 rioon SETPTR   // Set pointer to 1, 652, 657, 663, 681, 691, 702, or 730
499 robon
500 riion IDIV     // Pop 2 and Divide
501 robin If next value is rinin then push register, else push a newly calculated value
502 robin If next value is rinin then push register, else push a newly calculated value
503 rooin
504 rbbin
505 rbobn
506 rbiin PRNTCHR  // Pop 1 and print as char
507 riion IDIV     // Pop 2 and Divide
508 robin If next value is rinin then push register, else push a newly calculated value
509 robin If next value is rinin then push register, else push a newly calculated value
510 roobn
511 rbion SETORINC // If we can peek the stack call rioon, else increase pointer
512 roibn IXOR     // Pop 2 and Xor
513 rbbon
514 roion IBITOR   // Pop 2 and Or
515 robin If next value is rinin then push register, else push a newly calculated value
516 robbn
517 riiin DUPTOP   // Pop 1 and Add 2 of self to Stack
518 robon
519 roiin FUNCCALL // Pop and save, Push pointer plus 1, push saved pop, call rioon
520 rbion SETORINC // If we can peek the stack call rioon, else increase pointer
521 robin If next value is rinin then push register, else push a newly calculated value
522 robbn
523 rbibn
524 robon
525 ribon IMOD     // Pop 2 and Mod
526 rbion SETORINC // If we can peek the stack call rioon, else increase pointer
527 robin If next value is rinin then push register, else push a newly calculated value
528 robbn
529 rboon PRINT    // Pop 1 and print
530 riiin DUPTOP   // Pop 1 and Add 2 of self to Stack
531 rbbbn
532 rbbbn
533 robin If next value is rinin then push register, else push a newly calculated value
534 robbn
535 roion IBITOR   // Pop 2 and Or
536 robbn
537 rbbon
538 roiin FUNCCALL // Pop and save, Push pointer plus 1, push saved pop, call rioon
539 robin If next value is rinin then push register, else push a newly calculated value
540 robbn
541 robon
542 ribbn RET      // Pop 2, first pop is pushed back to stack after pops, second pop sets pointer
543 roobn
544 rbion SETORINC // If we can peek the stack call rioon, else increase pointer
545 robin If next value is rinin then push register, else push a newly calculated value
546 robbn
547 rooin
548 rboon PRINT    // Pop 1 and print
549 rbiin PRNTCHR  // Pop 1 and print as char
550 rbiin PRNTCHR  // Pop 1 and print as char
551 robin If next value is rinin then push register, else push a newly calculated value
552 robbn
553 roobn
554 rbbon
555 robin If next value is rinin then push register, else push a newly calculated value
556 riion IDIV     // Pop 2 and Divide
557 robin If next value is rinin then push register, else push a newly calculated value
558 robon
559 riion IDIV     // Pop 2 and Divide
560 roibn IXOR     // Pop 2 and Xor
561 roibn IXOR     // Pop 2 and Xor
562 robin If next value is rinin then push register, else push a newly calculated value
563 robon
564 rbion SETORINC // If we can peek the stack call rioon, else increase pointer
565 rooin
566 robbn
567 robin If next value is rinin then push register, else push a newly calculated value
568 robon
569 rboon PRINT    // Pop 1 and print
570 robbn
571 rioon SETPTR   // Set pointer to 1, 652, 657, 663, 681, 691, 702, or 730
572 robin If next value is rinin then push register, else push a newly calculated value
573 robon
574 robin If next value is rinin then push register, else push a newly calculated value
575 rioon SETPTR   // Set pointer to 1, 652, 657, 663, 681, 691, 702, or 730
576 riiin DUPTOP   // Pop 1 and Add 2 of self to Stack
577 robin If next value is rinin then push register, else push a newly calculated value
578 robon
579 robon
580 rbiin PRNTCHR  // Pop 1 and print as char
581 ribbn RET      // Pop 2, first pop is pushed back to stack after pops, second pop sets pointer
582 robin If next value is rinin then push register, else push a newly calculated value
583 robon
584 rooin
585 robin If next value is rinin then push register, else push a newly calculated value
586 rbbbn
587 robin If next value is rinin then push register, else push a newly calculated value
588 robon
589 roobn
590 roiin FUNCCALL // Pop and save, Push pointer plus 1, push saved pop, call rioon
591 rbbon
592 robin If next value is rinin then push register, else push a newly calculated value
593 rooin
594 riobn IADD     // Pop 2 and Add
595 ribon IMOD     // Pop 2 and Mod
596 robin If next value is rinin then push register, else push a newly calculated value
597 rooin
598 rbobn
599 rbbon
600 robin If next value is rinin then push register, else push a newly calculated value
601 rooin
602 robbn
603 rioin REVTOP   // Pop 2 and Add in opposite order
604 robin If next value is rinin then push register, else push a newly calculated value
605 rooin
606 roion IBITOR   // Pop 2 and Or
607 rbbin
608 robin If next value is rinin then push register, else push a newly calculated value
609 rooin
610 roobn
611 rbion SETORINC // If we can peek the stack call rioon, else increase pointer
612 robin If next value is rinin then push register, else push a newly calculated value
613 rooin
614 robon
615 rbbbn
616 robin If next value is rinin then push register, else push a newly calculated value
617 roobn
618 rooin
619 robin If next value is rinin then push register, else push a newly calculated value
620 rooin
621 robbn
622 roion IBITOR   // Pop 2 and Or
623 robin If next value is rinin then push register, else push a newly calculated value
624 rooin
625 robbn
626 rooon ISUB     // Pop 2 and Subtract
627 robin If next value is rinin then push register, else push a newly calculated value
628 rooin
629 robon
630 riobn IADD     // Pop 2 and Add
631 robin If next value is rinin then push register, else push a newly calculated value
632 rooin
633 robon
634 rioon SETPTR   // Set pointer to 1, 652, 657, 663, 681, 691, 702, or 730
635 robin If next value is rinin then push register, else push a newly calculated value
636 rooin
637 roobn
638 roiin FUNCCALL // Pop and save, Push pointer plus 1, push saved pop, call rioon
639 robin If next value is rinin then push register, else push a newly calculated value
640 rooin
641 robon
642 ribbn RET      // Pop 2, first pop is pushed back to stack after pops, second pop sets pointer
643 robin If next value is rinin then push register, else push a newly calculated value
644 rooin
645 robon
646 riion IDIV     // Pop 2 and Divide
647 robin If next value is rinin then push register, else push a newly calculated value
648 rooin
649 robon
650 robbn
651 rnbon INCPTR   // Increase ip pointer
652 roobn
653 robin If next value is rinin then push register, else push a newly calculated value
654 rooon ISUB     // Pop 2 and Subtract
655 rinin TOREG    // Pop 1 and save to register
656 rnbon INCPTR   // Increase ip pointer
657 rooin
658 rbion SETORINC // If we can peek the stack call rioon, else increase pointer
659 robon
660 rioon SETPTR   // Set pointer to 1, 652, 657, 663, 681, 691, 702, or 730
661 robbn
662 rnbon INCPTR   // Increase ip pointer
663 robon
664 robin If next value is rinin then push register, else push a newly calculated value
665 rinin TOREG    // Pop 1 and save to register
666 roiin FUNCCALL // Pop and save, Push pointer plus 1, push saved pop, call rioon
667 robin If next value is rinin then push register, else push a newly calculated value
668 roibn IXOR     // Pop 2 and Xor
669 rbiin PRNTCHR  // Pop 1 and print as char
670 robin If next value is rinin then push register, else push a newly calculated value
671 rinin TOREG    // Pop 1 and save to register
672 robin If next value is rinin then push register, else push a newly calculated value
673 roobn
674 roobn
675 riobn IADD     // Pop 2 and Add
676 rinin TOREG    // Pop 1 and save to register
677 rioon SETPTR   // Set pointer to 1, 652, 657, 663, 681, 691, 702, or 730
678 rooin
679 rioon SETPTR   // Set pointer to 1, 652, 657, 663, 681, 691, 702, or 730
680 robbn
681 rnbon INCPTR   // Increase ip pointer
682 robin If next value is rinin then push register, else push a newly calculated value
683 rbion SETORINC // If we can peek the stack call rioon, else increase pointer
684 roion IBITOR   // Pop 2 and Or
685 rboin IPOP     // Pop Stack
686 robin If next value is rinin then push register, else push a newly calculated value
687 roobn
688 roobn
689 ribbn RET      // Pop 2, first pop is pushed back to stack after pops, second pop sets pointer
690 rnbon INCPTR   // Increase ip pointer
691 roion IBITOR   // Pop 2 and Or
692 riiin DUPTOP   // Pop 1 and Add 2 of self to Stack
693 robin If next value is rinin then push register, else push a newly calculated value
694 roobn
695 roobn
696 rooon ISUB     // Pop 2 and Subtract
697 rbion SETORINC // If we can peek the stack call rioon, else increase pointer
698 roibn IXOR     // Pop 2 and Xor
699 rboin IPOP     // Pop Stack
700 ribbn RET      // Pop 2, first pop is pushed back to stack after pops, second pop sets pointer
701 rnbon INCPTR   // Increase ip pointer
702 roibn IXOR     // Pop 2 and Xor
703 rboin IPOP     // Pop Stack
704 riiin DUPTOP   // Pop 1 and Add 2 of self to Stack
705 robin If next value is rinin then push register, else push a newly calculated value
706 roobn
707 roobn
708 rooon ISUB     // Pop 2 and Subtract
709 robin If next value is rinin then push register, else push a newly calculated value
710 rooon ISUB     // Pop 2 and Subtract
711 rooon ISUB     // Pop 2 and Subtract
712 riiin DUPTOP   // Pop 1 and Add 2 of self to Stack
713 robin If next value is rinin then push register, else push a newly calculated value
714 roobn
715 roobn
716 rooon ISUB     // Pop 2 and Subtract
717 robin If next value is rinin then push register, else push a newly calculated value
718 rooon ISUB     // Pop 2 and Subtract
719 rooon ISUB     // Pop 2 and Subtract
720 roiin FUNCCALL // Pop and save, Push pointer plus 1, push saved pop, call rioon
721 robin If next value is rinin then push register, else push a newly calculated value
722 rioin REVTOP   // Pop 2 and Add in opposite order
723 roiin FUNCCALL // Pop and save, Push pointer plus 1, push saved pop, call rioon
724 robin If next value is rinin then push register, else push a newly calculated value
725 riobn IADD     // Pop 2 and Add
726 rioin REVTOP   // Pop 2 and Add in opposite order
727 rboin IPOP     // Pop Stack
728 ribbn RET      // Pop 2, first pop is pushed back to stack after pops, second pop sets pointer
729 rnbon INCPTR   // Increase ip pointer
730 robbn
731 robin If next value is rinin then push register, else push a newly calculated value
732 rooon ISUB     // Pop 2 and Subtract
733 rboin IPOP     // Pop Stack
734 robin If next value is rinin then push register, else push a newly calculated value
735 rooon ISUB     // Pop 2 and Subtract

function1():
        Push(0)
        Push(14472334024676096)
        Push(8944394323791450)
        Push(5527939700884769)
        Push(3416454622906725)
        Push(2111485077978096)
        Push(1304969544928756)
        Push(806515533049347)
        Push(498454011879172)
        Push(308061521170150)
        Push(190392490709200)
        Push(117669030460982)
        Push(72723460248127)
        Push(44945570212756)
        Push(27777890035307)
        Push(17167680177653)
        Push(10610209857675)
        Push(6557470319826)
        Push(4052739537835)
        Push(2504730782038)
        Push(1548008755937)
        Push(956722025992)
        Push(591286729974)
        Push(365435296253)
        Push(225851433664)
        Push(139583862488)
        Push(86267571223)
        Push(53316291075)
        Push(32951280083)
        Push(20365011165)
        Push(12586268949)
        Push(7778742098)
        Push(4807527027)
        Push(2971214979)
        Push(1836311808)
        Push(1134903217)
        Push(701408693)
        Push(433494481)
        Push(267914343)
        Push(165580035)
        Push(102334114)
        Push(63246016)
        Push(39088153)
        Push(24157707)
        Push(14930304)
        Push(9227513)
        Push(5702805)
        Push(3524541)
        Push(2178357)
        Push(1346217)
        Push(832119)
        Push(514176)
        Push(317697)
        Push(196465)
        Push(121346)
        Push(75129)
        Push(46403)
        Push(28590)
        Push(17692)
        Push(10993)
        Push(6687)
        Push(4157)
        Push(2668)
        Push(1606)
        Push(957)
        Push(534)
        Push(282)
        Push(128)
        Push(176)
        Push(42)
        Push(94)
        Push(2)
        Push(114)
        Push(108)
        Push(100)
        Push(99)
        Push(35)
        Push(103)
        Push(105)
        Push(85)
function652():
        Push(0)
        register = Pop()
function657():
        IF stack.Peek() != 0:
                Call:  663
        Else:
function663():
        Push(register)
        Call:  682
        XOR
        PRNTCHR
        Push(register)
        Push(1)
        IADD
        register = Pop()
function682():
        IF stack.Peek() != 0:
                Call:  691
        Else:
        Pop()
        Push(1)
        RET
function691():
        x = Pop(); Push(x); Push(x)
        Push(1)
        ISUB
        IF stack.Peek() != 0:
                Call:  None
        Else:
        Pop()
        RET
function702():
        Pop()
        x = Pop(); Push(x); Push(x)
        Push(1)
        ISUB
        Push(0)
        ISUB
        x = Pop(); Push(x); Push(x)
        Push(1)
        ISUB
        Push(0)
        ISUB
        Call:  682
        x = Pop(); y = Pop(); Push(x); Push(y)
        Call:  682
        IADD
        x = Pop(); y = Pop(); Push(x); Push(y)
        Pop()
        RET
function730():
        Push(0)
        Pop()
        Push(0)
[0, 14472334024676096, 8944394323791450, 5527939700884769, 3416454622906725, 2111485077978096, 1304969544928756, 806515533049347, 498454011879172, 308061521170150, 190392490709200, 117669030460982, 72723460248127, 44945570212756, 27777890035307, 17167680177653, 10610209857675, 6557470319826, 4052739537835, 2504730782038, 1548008755937, 956722025992, 591286729974, 365435296253, 225851433664, 139583862488, 86267571223, 53316291075, 32951280083, 20365011165, 12586268949, 7778742098, 4807527027, 2971214979, 1836311808, 1134903217, 701408693, 433494481, 267914343, 165580035, 102334114, 63246016, 39088153, 24157707, 14930304, 9227513, 5702805, 3524541, 2178357, 1346217, 832119, 514176, 317697, 196465, 121346, 75129, 46403, 28590, 17692, 10993, 6687, 4157, 2668, 1606, 957, 534, 282, 128, 176, 42, 94, 2, 114, 108, 100, 99, 35, 103, 105, 85, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0]
```

Looking at my decompiled roolang code I saw that we print out the chars in this function. 

```
function663():
        Push(register)
        Call:  682
        XOR
        PRNTCHR
        Push(register)
        Push(1)
        IADD
        register = Pop()
```

And before we printed the value we do an xor. This xor does not show up anywhere else in the decompiled code so I went back to the original roolang.py and added a print statement to the xor so that it would print out the two values that get xor'ed.

When run the output was:

```
1 85 T
T1 105 h
h2 103 e
e3 35  
 5 99 f
f8 100 l
l13 108 a
a21 114 g
g34 2  
 55 94 i
i89 42 s
s144 176  
 233 128 i
i377 282 c
c610 534 t
t987 957 f
```

I could see that the values that were xor'ed were the value we push in function1 as well as the fibonacci sequence. I then wrote a script to output the flag based on this pattern:

```py
x = [0, 14472334024676096, 8944394323791450, 5527939700884769, 3416454622906725, 2111485077978096, 1304969544928756, 806515533049347, 498454011879172, 308061521170150, 190392490709200, 117669030460982, 72723460248127, 44945570212756, 27777890035307, 17167680177653, 10610209857675, 6557470319826, 4052739537835, 2504730782038, 1548008755937, 956722025992, 591286729974, 365435296253, 225851433664, 139583862488, 86267571223, 53316291075, 32951280083, 20365011165, 12586268949, 7778742098, 4807527027, 2971214979, 1836311808, 1134903217, 701408693, 433494481, 267914343, 165580035, 102334114, 63246016, 39088153, 24157707, 14930304, 9227513, 5702805, 3524541, 2178357, 1346217, 832119, 514176, 317697, 196465, 121346, 75129, 46403, 28590, 17692, 10993, 6687, 4157, 2668, 1606, 957, 534, 282, 128, 176, 42, 94, 2, 114, 108, 100, 99, 35, 103, 105, 85]

secondLast = 0
last = 1

s = ""

for i in range(2, len(x)-1):
	a = secondLast + last
	secondLast = last
	last = a
	s += chr((x[-i])^a)
	print(chr((x[-i])^a), x[-i], a)

print(s)
```

This then output:

```
h 105 1
e 103 2
  35 3
f 99 5
l 100 8
a 108 13
g 114 21
  2 34
i 94 55
s 42 89
  176 144
i 128 233
c 282 377
t 534 610
f 957 987
{ 1606 1597
t 2668 2584
h 4157 4181
r 6687 6765
3 10993 10946
3 17692 17711
_ 28590 28657
c 46403 46368
h 75129 75025
3 121346 121393
3 196465 196418
r 317697 317811
5 514176 514229
_ 832119 832040
t 1346217 1346269
0 2178357 2178309
_ 3524541 3524578
r 5702805 5702887
0 9227513 9227465
0 14930304 14930352
r 24157707 24157817
0 39088153 39088169
b 63246016 63245986
i 102334114 102334155
n 165580035 165580141
_ 267914343 267914296
t 433494481 433494437
h 701408693 701408733
3 1134903217 1134903170
_ 1836311808 1836311903
b 2971214979 2971215073
3 4807527027 4807526976
s 7778742098 7778742049
t 12586268949 12586269025
_ 20365011165 20365011074
0 32951280083 32951280099
f 53316291075 53316291173
_ 86267571223 86267571272
u 139583862488 139583862445
5 225851433664 225851433717
_ 365435296253 365435296162
a 591286729974 591286729879
1 956722025992 956722026041
1 1548008755937 1548008755920
_ 2504730782038 2504730781961
r 4052739537835 4052739537881
0 6557470319826 6557470319842
0 10610209857675 10610209857723
h 17167680177653 17167680177565
3 27777890035307 27777890035288
a 44945570212756 44945570212853
r 72723460248127 72723460248141
t 117669030460982 117669030460994
_ 190392490709200 190392490709135
7 308061521170150 308061521170129
d 498454011879172 498454011879264
2 806515533049347 806515533049393
e 1304969544928756 1304969544928657
2 2111485077978096 2111485077978050
6 3416454622906725 3416454622906707
4 5527939700884769 5527939700884757
2 8944394323791450 8944394323791464
he flag is ictf{thr33_ch33r5_t0_r00r0bin_th3_b3st_0f_u5_a11_r00h3art_7d2e2642
```