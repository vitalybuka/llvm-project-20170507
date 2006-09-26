//===-- llvm/CodeGen/AsmPrinter.h - AsmPrinter Framework --------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file was developed by the LLVM research group and is distributed under
// the University of Illinois Open Source License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains a class to be used as the base class for target specific
// asm writers.  This class primarily handles common functionality used by
// all asm writers.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CODEGEN_ASMPRINTER_H
#define LLVM_CODEGEN_ASMPRINTER_H

#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/Support/DataTypes.h"

namespace llvm {
  class Constant;
  class ConstantArray;
  class GlobalVariable;
  class MachineConstantPoolEntry;
  class MachineConstantPoolValue;
  class Mangler;
  class TargetAsmInfo;
  

  /// AsmPrinter - This class is intended to be used as a driving class for all
  /// asm writers.
  class AsmPrinter : public MachineFunctionPass {
    /// FunctionNumber - This provides a unique ID for each function emitted in
    /// this translation unit.  It is autoincremented by SetupMachineFunction,
    /// and can be accessed with getFunctionNumber() and 
    /// IncrementFunctionNumber().
    ///
    unsigned FunctionNumber;

  public:
    /// Output stream on which we're printing assembly code.
    ///
    std::ostream &O;

    /// Target machine description.
    ///
    TargetMachine &TM;
    
    /// Target Asm Printer information.
    ///
    const TargetAsmInfo *TAI;

    /// Name-mangler for global names.
    ///
    Mangler *Mang;

    /// Cache of mangled name for current function. This is recalculated at the
    /// beginning of each call to runOnMachineFunction().
    ///
    std::string CurrentFnName;
    
    /// CurrentSection - The current section we are emitting to.  This is
    /// controlled and used by the SwitchSection method.
    std::string CurrentSection;
  
  protected:
    AsmPrinter(std::ostream &o, TargetMachine &TM, const TargetAsmInfo *T);
    
  public:
    /// SwitchToTextSection - Switch to the specified section of the executable
    /// if we are not already in it!  If GV is non-null and if the global has an
    /// explicitly requested section, we switch to the section indicated for the
    /// global instead of NewSection.
    ///
    /// If the new section is an empty string, this method forgets what the
    /// current section is, but does not emit a .section directive.
    ///
    /// This method is used when about to emit executable code.
    ///
    void SwitchToTextSection(const char *NewSection, const GlobalValue *GV);

    /// SwitchToDataSection - Switch to the specified section of the executable
    /// if we are not already in it!  If GV is non-null and if the global has an
    /// explicitly requested section, we switch to the section indicated for the
    /// global instead of NewSection.
    ///
    /// If the new section is an empty string, this method forgets what the
    /// current section is, but does not emit a .section directive.
    ///
    /// This method is used when about to emit data.  For most assemblers, this
    /// is the same as the SwitchToTextSection method, but not all assemblers
    /// are the same.
    ///
    void SwitchToDataSection(const char *NewSection, const GlobalValue *GV);
    
    /// getPreferredAlignmentLog - Return the preferred alignment of the
    /// specified global, returned in log form.  This includes an explicitly
    /// requested alignment (if the global has one).
    unsigned getPreferredAlignmentLog(const GlobalVariable *GV) const;
  protected:
    /// doInitialization - Set up the AsmPrinter when we are working on a new
    /// module.  If your pass overrides this, it must make sure to explicitly
    /// call this implementation.
    bool doInitialization(Module &M);

    /// doFinalization - Shut down the asmprinter.  If you override this in your
    /// pass, you must make sure to call it explicitly.
    bool doFinalization(Module &M);

    /// PrintAsmOperand - Print the specified operand of MI, an INLINEASM
    /// instruction, using the specified assembler variant.  Targets should
    /// override this to format as appropriate.  This method can return true if
    /// the operand is erroneous.
    virtual bool PrintAsmOperand(const MachineInstr *MI, unsigned OpNo,
                                 unsigned AsmVariant, const char *ExtraCode);
    
    /// PrintAsmMemoryOperand - Print the specified operand of MI, an INLINEASM
    /// instruction, using the specified assembler variant as an address.
    /// Targets should override this to format as appropriate.  This method can
    /// return true if the operand is erroneous.
    virtual bool PrintAsmMemoryOperand(const MachineInstr *MI, unsigned OpNo,
                                       unsigned AsmVariant, 
                                       const char *ExtraCode);
    
    /// SetupMachineFunction - This should be called when a new MachineFunction
    /// is being processed from runOnMachineFunction.
    void SetupMachineFunction(MachineFunction &MF);
    
    /// getFunctionNumber - Return a unique ID for the current function.
    ///
    unsigned getFunctionNumber() const { return FunctionNumber; }
    
    /// IncrementFunctionNumber - Increase Function Number.  AsmPrinters should
    /// not normally call this, as the counter is automatically bumped by
    /// SetupMachineFunction.
    void IncrementFunctionNumber() { FunctionNumber++; }
    
    /// EmitConstantPool - Print to the current output stream assembly
    /// representations of the constants in the constant pool MCP. This is
    /// used to print out constants which have been "spilled to memory" by
    /// the code generator.
    ///
    void EmitConstantPool(MachineConstantPool *MCP);

    /// EmitJumpTableInfo - Print assembly representations of the jump tables 
    /// used by the current function to the current output stream.  
    ///
    void EmitJumpTableInfo(MachineJumpTableInfo *MJTI);
    
    /// EmitSpecialLLVMGlobal - Check to see if the specified global is a
    /// special global used by LLVM.  If so, emit it and return true, otherwise
    /// do nothing and return false.
    bool EmitSpecialLLVMGlobal(const GlobalVariable *GV);

    /// EmitAlignment - Emit an alignment directive to the specified power of
    /// two boundary.  For example, if you pass in 3 here, you will get an 8
    /// byte alignment.  If a global value is specified, and if that global has
    /// an explicit alignment requested, it will override the alignment request.
    void EmitAlignment(unsigned NumBits, const GlobalValue *GV = 0) const;

    /// EmitZeros - Emit a block of zeros.
    ///
    void EmitZeros(uint64_t NumZeros) const;

    /// EmitString - Emit a zero-byte-terminated string constant.
    ///
    virtual void EmitString(const ConstantArray *CVA) const;

    /// EmitConstantValueOnly - Print out the specified constant, without a
    /// storage class.  Only constants of first-class type are allowed here.
    void EmitConstantValueOnly(const Constant *CV);

    /// EmitGlobalConstant - Print a general LLVM constant to the .s file.
    ///
    void EmitGlobalConstant(const Constant* CV);

    virtual void EmitMachineConstantPoolValue(MachineConstantPoolValue *MCPV);
    
    /// printInlineAsm - This method formats and prints the specified machine
    /// instruction that is an inline asm.
    void printInlineAsm(const MachineInstr *MI) const;
    
    /// printBasicBlockLabel - This method prints the label for the specified
    /// MachineBasicBlock
    virtual void printBasicBlockLabel(const MachineBasicBlock *MBB,
                                      bool printColon = false,
                                      bool printComment = true) const;
                                      
    /// printSetLabel - This method prints a set label for the specified
    /// MachineBasicBlock
    void printSetLabel(unsigned uid, const MachineBasicBlock *MBB) const;

    /// printDataDirective - This method prints the asm directive for the
    /// specified type.
    void printDataDirective(const Type *type);

  private:
    void EmitLLVMUsedList(Constant *List);
    void EmitXXStructorList(Constant *List);
    void EmitConstantPool(unsigned Alignment, const char *Section,
                std::vector<std::pair<MachineConstantPoolEntry,unsigned> > &CP);

  };
}

#endif
