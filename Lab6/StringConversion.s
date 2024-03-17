// StringConversion.s
// Student names: change this to your names or look very silly
// Last modification date: change this to the last modification date or look very silly
// Runs on any Cortex M0
// ECE319K lab 6 number to string conversion
//
// You write udivby10 and Dec2String
     .data
     .align 2
// no globals allowed for Lab 6
    .global OutChar    // virtual output device
    .global OutDec     // your Lab 6 function
    .global Test_udivby10

    .text
    .align 2
// **test of udivby10**
// since udivby10 is not AAPCS compliant, we must test it in assembly
Test_udivby10:
    PUSH {LR}

    MOVS R0,#123
    BL   udivby10
// put a breakpoint here
// R0 should equal 12 (0x0C)
// R1 should equal 3

    LDR R0,=12345
    BL   udivby10
// put a breakpoint here
// R0 should equal 1234 (0x4D2)
// R1 should equal 5

    MOVS R0,#0
    BL   udivby10
// put a breakpoint here
// R0 should equal 0
// R1 should equal 0
    POP {PC}

//****************************************************
// divisor=10
// Inputs: R0 is 16-bit dividend
// quotient*10 + remainder = dividend
// Output: R0 is 16-bit quotient=dividend/10
//         R1 is 16-bit remainder=dividend%10 (modulus)
// not AAPCS compliant because it returns two values
udivby10:
      PUSH {R4, LR}
      LDR  R4,=0x00010000 // bit mask
      MOVS R1, #10
      MOVS R3, #0  // quotient
      MOVS R2, #16 // loop counter
      LSLS R1, #15 // move divisor under dividend

loop:
	LSRS R4,R4,#1 // bit mask 15 to 0
      CMP  R0,R1 // need to subtract?
      BLO  next
      SUBS R0,R0,R1 // subtract divisor
      ORRS R3,R3,R4 // set bit

next:
	LSRS R1,R1,#1
      SUBS R2,R2,#1
      BNE  loop
      MOVS R1,R0 // remainder
      MOVS R0,R3 // quotient

      POP{R4, PC}

  
//-----------------------OutDec-----------------------
// Convert a 16-bit number into unsigned decimal format
// ECE319H students must use recursion
// Call the function OutChar to output each character
// OutChar does not do actual output, OutChar does virtual output used by the grader
// You will call OutChar 1 to 5 times
// Input: R0 (call by value) 16-bit unsigned number
// Output: none
// Invariables: This function must not permanently modify registers R4 to R11
OutDec:
    PUSH {LR}

    POP  {PC}
//* * * * * * * * End of OutDec * * * * * * * *



     .end
