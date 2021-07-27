# HackPack Exhell Write Up

## Details:

Jeopardy style CTF

Category: Reverse Engineering

Comments:

I'm tired of the IT department laughing at me when they see how much I spend on cans of effervescent water. That's why I've decided to secure my expences with password, so that nobody will laugh at me ever again! 😤

## Write up:

I used binwalk to extract all the information from the .xlsx file. I was then able to get the xml for the two sheets. The important information was in sheet2.xml.

At first I tried to use Z3 and making a python script to write the Z3 script, my teammate pointed out that was unecessary so he made a quick script to parse the xml, then we used sympy to solve for all of the B cell values. I then added a portion for converting the B values to ascii values:

```python
# imports
import xml.etree.ElementTree as ET
import sympy
import json
import re

# import sheet
tree = ET.parse('sheet2.xml')
root = tree.getroot()

data = root[4]

values = {"K1": True}

op = {"OR": " | ", "AND": " & ", "NOT": "~", "FALSE": False, "TRUE": True}

# process all the data
def process_formula(formula):
    if formula == "FALSE()" or formula == "TRUE()":
        return op[formula[:-2]]
    pos_open_bracket = formula.index('(')
    new_formula = formula[pos_open_bracket:].replace('NOT', op["NOT"])
    new_formula = new_formula.replace(",", op[formula[:pos_open_bracket]])
    return new_formula

def get_answer(formula, expected=False):
    if not expected:
        formula = '~{}'.format(formula)
    if re.match(r".*E1[^0-9].*", formula):
        formula = formula.replace("E1", "x")
    p = sympy.satisfiable(sympy.parse_expr(formula, evaluate=False))
    u = list(p.keys())
    new_p = {i.name: p[i] for i in u}
    if "x" in new_p:
        new_p["E1"] = new_p["x"]
        del new_p["x"]
    return new_p


def process_col(col):
    global values
    if 'A' not in col.attrib['r'] and 'B' not in col.attrib['r']:
        _op = process_formula(col[0].text)
        if _op in (True, False):
            values[col.attrib['r']] = _op
        else:
            values.update(get_answer(_op, values[col.attrib['r']]))

def process_row(row):
    for col in row[::-1]:
        process_col(col)


for row in data:
    process_row(row)

values_b = list(filter(lambda x: 'B' in x, values.keys()))
values_b = {i:values[i] for i in values_b}

# create array for B values
Bvals = [0 for i in range(0, len(values_b))]

# turn to binary and then ascii
for i in values_b:
	Bvals[int(i.replace('B', ''))-1] = 1 if values_b[i] else 0

s = ""

for i in range(0, len(Bvals), 8):
	curVal = ""
	for j in range(0, 8):
		curVal += str(Bvals[i+j])
	s += chr(int(curVal, 2))

print(s)
```

Once run this output:

```
flag{0h_g33z_th4t5_a_l0t_sp3nt_0n_L3Cr0ix}
```
