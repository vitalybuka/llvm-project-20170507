# REQUIRES: x86

# RUN: llvm-mc -filetype=obj -triple=x86_64-unknown-linux %s -o %t

# RUN: ld.lld --build-id %t -o %t2 -threads
# RUN: llvm-objdump -s %t2 | FileCheck -check-prefix=DEFAULT %s
# RUN: ld.lld --build-id %t -o %t2 -no-threads
# RUN: llvm-objdump -s %t2 | FileCheck -check-prefix=DEFAULT %s

# RUN: ld.lld --build-id=md5 %t -o %t2 -threads
# RUN: llvm-objdump -s %t2 | FileCheck -check-prefix=MD5 %s
# RUN: ld.lld --build-id=md5 %t -o %t2 -no-threads
# RUN: llvm-objdump -s %t2 | FileCheck -check-prefix=MD5 %s

# RUN: ld.lld --build-id=sha1 %t -o %t2 -threads
# RUN: llvm-objdump -s %t2 | FileCheck -check-prefix=SHA1 %s
# RUN: ld.lld --build-id=sha1 %t -o %t2 -no-threads
# RUN: llvm-objdump -s %t2 | FileCheck -check-prefix=SHA1 %s

# RUN: ld.lld --build-id=tree %t -o %t2 -threads
# RUN: llvm-objdump -s %t2 | FileCheck -check-prefix=SHA1 %s
# RUN: ld.lld --build-id=tree %t -o %t2 -no-threads
# RUN: llvm-objdump -s %t2 | FileCheck -check-prefix=SHA1 %s

# RUN: ld.lld --build-id=uuid %t -o %t2
# RUN: llvm-objdump -s %t2 | FileCheck -check-prefix=UUID %s

# RUN: ld.lld --build-id=0x12345678 %t -o %t2
# RUN: llvm-objdump -s %t2 | FileCheck -check-prefix=HEX %s

# RUN: ld.lld %t -o %t2
# RUN: llvm-objdump -s %t2 | FileCheck -check-prefix=NONE %s

# RUN: ld.lld --build-id=md5 --build-id=none %t -o %t2
# RUN: llvm-objdump -s %t2 | FileCheck -check-prefix=NONE %s
# RUN: ld.lld --build-id --build-id=none %t -o %t2
# RUN: llvm-objdump -s %t2 | FileCheck -check-prefix=NONE %s
# RUN: ld.lld --build-id=none --build-id %t -o %t2
# RUN: llvm-objdump -s %t2 | FileCheck -check-prefix=DEFAULT %s

.globl _start
_start:
  nop

.section .note.test, "a", @note
   .quad 42

# DEFAULT:      Contents of section .note.test:
# DEFAULT:      Contents of section .note.gnu.build-id:
# DEFAULT-NEXT: 04000000 08000000 03000000 474e5500  ............GNU.
# DEFAULT-NEXT: bc208f98 7f8e4770

# MD5:      Contents of section .note.gnu.build-id:
# MD5-NEXT: 04000000 10000000 03000000 474e5500  ............GNU.
# MD5-NEXT: 8e13f077 f9c94068 9d63c715 6c952cd3

# SHA1:      Contents of section .note.gnu.build-id:
# SHA1-NEXT: 04000000 14000000 03000000 474e5500  ............GNU.
# SHA1-NEXT: 754ac1ea 0a2e5144 0f4642b9 3f096bbb

# UUID:      Contents of section .note.gnu.build-id:
# UUID-NEXT: 04000000 10000000 03000000 474e5500  ............GNU.

# HEX:      Contents of section .note.gnu.build-id:
# HEX-NEXT: 04000000 04000000 03000000 474e5500  ............GNU.
# HEX-NEXT: 12345678

# NONE-NOT: Contents of section .note.gnu.build-id:
