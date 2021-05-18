# ShadowCTF reverse engineering warmup Write Up

## Details:
Points: 50

Jeopardy style CTF

Category: Reversing

## Write up:

By the time I got to this challenge I was skeptical about the difficulty of this CTF so rather than trying to reverse it I just used strings.

```
$ strings Intro                                                                          

1 ⨯
/lib64/ld-linux-x86-64.so.2
puts
printf
sleep
__cxa_finalize
__libc_start_main
libc.so.6
GLIBC_2.2.5
_ITM_deregisterTMCloneTable
__gmon_start__
_ITM_registerTMCloneTable
u/UH
[]A\A]A^A_
you need patience to get the flag.
{steppingstone}
;*3$"
GCC: (Debian 9.3.0-18) 9.3.0
crtstuff.c
deregister_tm_clones
__do_global_dtors_aux
completed.7454
__do_global_dtors_aux_fini_array_entry
frame_dummy
__frame_dummy_init_array_entry
gk.c
__FRAME_END__
__init_array_end
_DYNAMIC
__init_array_start
__GNU_EH_FRAME_HDR
_GLOBAL_OFFSET_TABLE_
__libc_csu_fini
_ITM_deregisterTMCloneTable
puts@@GLIBC_2.2.5
_edata
printf@@GLIBC_2.2.5
__libc_start_main@@GLIBC_2.2.5
__data_start
__gmon_start__
__dso_handle
_IO_stdin_used
__libc_csu_init
__bss_start
main
__TMC_END__
_ITM_registerTMCloneTable
sleep@@GLIBC_2.2.5
__cxa_finalize@@GLIBC_2.2.5
.symtab
.strtab
.shstrtab
.interp
.note.gnu.build-id
.note.ABI-tag
.gnu.hash
.dynsym
.dynstr
.gnu.version
.gnu.version_r
.rela.dyn
.rela.plt
.init
.plt.got
.text
.fini
.rodata
.eh_frame_hdr
.eh_frame
.init_array
.fini_array
.dynamic
.got.plt
.data
.bss
.comment
```

There was one string wrapped in {}, and that ended up being the flag.

```
{steppingstone}
```