# CyberGon Super Secure Encryption Write Up

## Details:

Jeopardy style CTF

Category: Reverse Engineering / Crypto

Comments:

```t
Our Team created a super encryptor. can u break it ?
```

## Write up:

After unzipping the zip file I saw that we had 3 files: a compiled python file containing the algorithm, a python file that was used to encrypt files, and an encrypted flag file. 

The first thing I did was look into the encrypt python file to see if there was anything important:

```python
import sys
from algorithm import encrypt

def encrypt_file(input_file: str, output_file: str):
    with open(input_file, 'rb') as infile:
        data = infile.read()

    encrypted_data = encrypt(data)

    with open(output_file, 'wb') as outfile:
        outfile.write(encrypted_data)

    print(f"File {input_file} has been encrypted and saved to {output_file}")

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python encrypt_file.py <input_file> <output_file>")
    else:
        input_file = sys.argv[1]
        output_file = sys.argv[2]
        encrypt_file(input_file, output_file)
```

I saw that we imported a function called encrypt from the compiled python file, so that was my next step. I then tried using pycdc to decompile the compiled file however it failed to decompile the entire file and I was left with:

```python
# Source Generated with Decompyle++
# File: algorithm.pyc (Python 3.11)

from cryptography.hazmat.primitives.kdf.pbkdf2 import PBKDF2HMAC
from cryptography.hazmat.primitives import hashes
from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
import os

def encrypt(data = None):
Unsupported opcode: KW_NAMES
    passphrase = b'UniquePassphrase'
    salt = b'FixedUniqueSalt123'
# WARNING: Decompyle incomplete
```

This gave me a fair amount of hints as to what was being used but was not enough so I decided to diassemble the file with pydas:

```python
[Code]
    File Name: .\algorithm.py
    Object Name: <module>
    Qualified Name: <module>
    Arg Count: 0
    Pos Only Arg Count: 0
    KW Only Arg Count: 0
    Stack Size: 4
    Flags: 0x00000000
    [Names]
        'cryptography.hazmat.primitives.kdf.pbkdf2'
        'PBKDF2HMAC'
        'cryptography.hazmat.primitives'
        'hashes'
        'cryptography.hazmat.backends'
        'default_backend'
        'cryptography.hazmat.primitives.ciphers'
        'Cipher'
        'algorithms'
        'modes'
        'os'
        'bytes'
        'encrypt'
    [Locals+Names]
    [Constants]
        0
        (
            'PBKDF2HMAC'
        )
        (
            'hashes'
        )
        (
            'default_backend'
        )
        (
            'Cipher'
            'algorithms'
            'modes'
        )
        None
        'data'
        'return'
        [Code]
            File Name: .\algorithm.py
            Object Name: encrypt
            Qualified Name: encrypt
            Arg Count: 1
            Pos Only Arg Count: 0
            KW Only Arg Count: 0
            Stack Size: 8
            Flags: 0x00000003 (CO_OPTIMIZED | CO_NEWLOCALS)
            [Names]
                'PBKDF2HMAC'
                'hashes'
                'SHA256'
                'default_backend'
                'derive'
                'os'
                'urandom'
                'Cipher'
                'algorithms'
                'AES'
                'modes'
                'CFB'
                'encryptor'
                'update'
                'finalize'
            [Locals+Names]
                'data'
                'passphrase'
                'salt'
                'kdf'
                'key'
                'iv'
                'cipher'
                'encryptor'
                'encrypted_data'
            [Constants]
                None
                b'UniquePassphrase'
                b'FixedUniqueSalt123'
                32
                100000
                (
                    'algorithm'
                    'length'
                    'salt'
                    'iterations'
                    'backend'
                )
                16
                (
                    'backend'
                )
            [Disassembly]
                0       RESUME                        0
                2       LOAD_CONST                    1: b'UniquePassphrase'
                4       STORE_FAST                    1: passphrase
                6       LOAD_CONST                    2: b'FixedUniqueSalt123'
                8       STORE_FAST                    2: salt
                10      LOAD_GLOBAL                   1: NULL + PBKDF2HMAC
                22      LOAD_GLOBAL                   3: NULL + hashes
                34      LOAD_ATTR                     2: SHA256
                44      PRECALL                       0
                48      CALL                          0
                58      LOAD_CONST                    3: 32
                60      LOAD_FAST                     2: salt
                62      LOAD_CONST                    4: 100000
                64      LOAD_GLOBAL                   7: NULL + default_backend
                76      PRECALL                       0
                80      CALL                          0
                90      KW_NAMES                      5
                92      PRECALL                       5
                96      CALL                          5
                106     STORE_FAST                    3: kdf
                108     LOAD_FAST                     3: kdf
                110     LOAD_METHOD                   4: derive
                132     LOAD_FAST                     1: passphrase
                134     PRECALL                       1
                138     CALL                          1
                148     STORE_FAST                    4: key
                150     LOAD_GLOBAL                   11: NULL + os
                162     LOAD_ATTR                     6: urandom
                172     LOAD_CONST                    6: 16
                174     PRECALL                       1
                178     CALL                          1
                188     STORE_FAST                    5: iv
                190     LOAD_GLOBAL                   15: NULL + Cipher
                202     LOAD_GLOBAL                   17: NULL + algorithms
                214     LOAD_ATTR                     9: AES
                224     LOAD_FAST                     4: key
                226     PRECALL                       1
                230     CALL                          1
                240     LOAD_GLOBAL                   21: NULL + modes
                252     LOAD_ATTR                     11: CFB
                262     LOAD_FAST                     5: iv
                264     PRECALL                       1
                268     CALL                          1
                278     LOAD_GLOBAL                   7: NULL + default_backend
                290     PRECALL                       0
                294     CALL                          0
                304     KW_NAMES                      7
                306     PRECALL                       3
                310     CALL                          3
                320     STORE_FAST                    6: cipher
                322     LOAD_FAST                     6: cipher
                324     LOAD_METHOD                   12: encryptor
                346     PRECALL                       0
                350     CALL                          0
                360     STORE_FAST                    7: encryptor
                362     LOAD_FAST                     7: encryptor
                364     LOAD_METHOD                   13: update
                386     LOAD_FAST                     0: data
                388     PRECALL                       1
                392     CALL                          1
                402     LOAD_FAST                     7: encryptor
                404     LOAD_METHOD                   14: finalize
                426     PRECALL                       0
                430     CALL                          0
                440     BINARY_OP                     0 (+)
                444     STORE_FAST                    8: encrypted_data
                446     LOAD_FAST                     5: iv
                448     LOAD_FAST                     8: encrypted_data
                450     BINARY_OP                     0 (+)
                454     RETURN_VALUE                  
    [Disassembly]
        0       RESUME                        0
        2       LOAD_CONST                    0: 0
        4       LOAD_CONST                    1: ('PBKDF2HMAC',)
        6       IMPORT_NAME                   0: cryptography.hazmat.primitives.kdf.pbkdf2
        8       IMPORT_FROM                   1: PBKDF2HMAC
        10      STORE_NAME                    1: PBKDF2HMAC
        12      POP_TOP                       
        14      LOAD_CONST                    0: 0
        16      LOAD_CONST                    2: ('hashes',)
        18      IMPORT_NAME                   2: cryptography.hazmat.primitives
        20      IMPORT_FROM                   3: hashes
        22      STORE_NAME                    3: hashes
        24      POP_TOP                       
        26      LOAD_CONST                    0: 0
        28      LOAD_CONST                    3: ('default_backend',)
        30      IMPORT_NAME                   4: cryptography.hazmat.backends
        32      IMPORT_FROM                   5: default_backend
        34      STORE_NAME                    5: default_backend
        36      POP_TOP                       
        38      LOAD_CONST                    0: 0
        40      LOAD_CONST                    4: ('Cipher', 'algorithms', 'modes')
        42      IMPORT_NAME                   6: cryptography.hazmat.primitives.ciphers
        44      IMPORT_FROM                   7: Cipher
        46      STORE_NAME                    7: Cipher
        48      IMPORT_FROM                   8: algorithms
        50      STORE_NAME                    8: algorithms
        52      IMPORT_FROM                   9: modes
        54      STORE_NAME                    9: modes
        56      POP_TOP                       
        58      LOAD_CONST                    0: 0
        60      LOAD_CONST                    5: None
        62      IMPORT_NAME                   10: os
        64      STORE_NAME                    10: os
        66      LOAD_CONST                    6: 'data'
        68      LOAD_NAME                     11: bytes
        70      LOAD_CONST                    7: 'return'
        72      LOAD_NAME                     11: bytes
        74      BUILD_TUPLE                   4
        76      LOAD_CONST                    8: <CODE> encrypt
        78      MAKE_FUNCTION                 4
        80      STORE_NAME                    12: encrypt
        82      LOAD_CONST                    5: None
        84      RETURN_VALUE  
```

From here I could see the imports used as well as the algorithms used to derive the key and encrypt the flag. From here it was just writing a script that would derive the key using the hardcoded values and then decrypt the flag:

```python
from cryptography.hazmat.primitives.kdf.pbkdf2 import PBKDF2HMAC
from cryptography.hazmat.primitives import hashes
from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
import os

flag = open('./flag.enc', 'rb').read()

kdf = PBKDF2HMAC(
    algorithm=hashes.SHA256(),
    backend=default_backend(),
    length=32,
    salt=b'FixedUniqueSalt123',
    iterations=100_000
)

key = kdf.derive(b'UniquePassphrase')

cipher = Cipher(algorithms.AES(key), modes.CFB(flag[:16]), backend=default_backend())
decryptor = cipher.decryptor()

decryptor.update(flag[16:])
```

Which gave us the following flag:

```t
CybergonCTF{$up3r_$3cur3_3ncrypt0r!!!}
```