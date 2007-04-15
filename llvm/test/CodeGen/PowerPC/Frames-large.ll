; RUN: llvm-upgrade < %s | llvm-as | \
; RUN:   llc -march=ppc32 -mtriple=powerpc-apple-darwin8 | \
; RUN:   not grep {stw r31, 20(r1)}
; RUN: llvm-upgrade < %s | llvm-as | \
; RUN:   llc -march=ppc32 -mtriple=powerpc-apple-darwin8 | grep {lis r0, -1}
; RUN: llvm-upgrade < %s | llvm-as | \
; RUN:   llc -march=ppc32 -mtriple=powerpc-apple-darwin8 | \
; RUN:   grep {ori r0, r0, 32704}
; RUN: llvm-upgrade < %s | llvm-as | \
; RUN:   llc -march=ppc32 -mtriple=powerpc-apple-darwin8 | \
; RUN:   grep {stwux r1, r1, r0}
; RUN: llvm-upgrade < %s | llvm-as | \
; RUN:   llc -march=ppc32 -mtriple=powerpc-apple-darwin8 | \
; RUN:   grep {lwz r1, 0(r1)}
; RUN: llvm-upgrade < %s | llvm-as | \
; RUN:   llc -march=ppc32 -mtriple=powerpc-apple-darwin8 | \
; RUN:   not grep {lwz r31, 20(r1)}
; RUN: llvm-upgrade < %s | llvm-as | \
; RUN:   llc -march=ppc32 -mtriple=powerpc-apple-darwin8 -disable-fp-elim | \
; RUN:   grep {stw r31, 20(r1)}
; RUN: llvm-upgrade < %s | llvm-as | \
; RUN:   llc -march=ppc32 -mtriple=powerpc-apple-darwin8 -disable-fp-elim | \
; RUN:   grep {lis r0, -1}
; RUN: llvm-upgrade < %s | llvm-as | \
; RUN:   llc -march=ppc32 -mtriple=powerpc-apple-darwin8 -disable-fp-elim | \
; RUN:   grep {ori r0, r0, 32704}
; RUN: llvm-upgrade < %s | llvm-as | \
; RUN:   llc -march=ppc32 -mtriple=powerpc-apple-darwin8 -disable-fp-elim | \
; RUN:   grep {stwux r1, r1, r0}
; RUN: llvm-upgrade < %s | llvm-as | \
; RUN:   llc -march=ppc32 -mtriple=powerpc-apple-darwin8 -disable-fp-elim | \
; RUN:   grep {lwz r1, 0(r1)}
; RUN: llvm-upgrade < %s | llvm-as | \
; RUN:   llc -march=ppc32 -mtriple=powerpc-apple-darwin8 -disable-fp-elim | \
; RUN:   grep {lwz r31, 20(r1)}
; RUN: llvm-upgrade < %s | llvm-as | \
; RUN:   llc -march=ppc64 -mtriple=powerpc-apple-darwin8 | \
; RUN:   not grep {std r31, 40(r1)}
; RUN: llvm-upgrade < %s | llvm-as | \
; RUN:   llc -march=ppc64 -mtriple=powerpc-apple-darwin8 | \
; RUN:   grep {lis r0, -1}
; RUN: llvm-upgrade < %s | llvm-as | \
; RUN:   llc -march=ppc64 -mtriple=powerpc-apple-darwin8 | \
; RUN:   grep {ori r0, r0, 32656}
; RUN: llvm-upgrade < %s | llvm-as | \
; RUN:   llc -march=ppc64 -mtriple=powerpc-apple-darwin8 | \
; RUN:   grep {stdux r1, r1, r0}
; RUN: llvm-upgrade < %s | llvm-as | \
; RUN:   llc -march=ppc64 -mtriple=powerpc-apple-darwin8 | \
; RUN:   grep {ld r1, 0(r1)}
; RUN: llvm-upgrade < %s | llvm-as | \
; RUN:   llc -march=ppc64 -mtriple=powerpc-apple-darwin8 | \
; RUN:   not grep {ld r31, 40(r1)}
; RUN: llvm-upgrade < %s | llvm-as | \
; RUN:   llc -march=ppc64 -mtriple=powerpc-apple-darwin8 -disable-fp-elim | \
; RUN:   grep {std r31, 40(r1)}
; RUN: llvm-upgrade < %s | llvm-as | \
; RUN:   llc -march=ppc64 -mtriple=powerpc-apple-darwin8 -disable-fp-elim | \
; RUN:   grep {lis r0, -1}
; RUN: llvm-upgrade < %s | llvm-as | \
; RUN:   llc -march=ppc64 -mtriple=powerpc-apple-darwin8 -disable-fp-elim | \
; RUN:   grep {ori r0, r0, 32656}
; RUN: llvm-upgrade < %s | llvm-as | \
; RUN:   llc -march=ppc64 -mtriple=powerpc-apple-darwin8 -disable-fp-elim | \
; RUN:   grep {stdux r1, r1, r0}
; RUN: llvm-upgrade < %s | llvm-as | \
; RUN:   llc -march=ppc64 -mtriple=powerpc-apple-darwin8 -disable-fp-elim | \
; RUN:   grep {ld r1, 0(r1)}
; RUN: llvm-upgrade < %s | llvm-as | \
; RUN:   llc -march=ppc64 -mtriple=powerpc-apple-darwin8 -disable-fp-elim | \
; RUN:   grep {ld r31, 40(r1)}


implementation

int* %f1() {
	%tmp = alloca int, uint 8191
	ret int* %tmp
}
