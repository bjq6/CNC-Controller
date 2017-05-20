
;@-------------------------------------------------------------------------
;@-------------------------------------------------------------------------

// Some constants for later use
.equ    CPSR_MODE_USER,         0x10
.equ    CPSR_MODE_FIQ,          0x11
.equ    CPSR_MODE_IRQ,          0x12
.equ    CPSR_MODE_SVR,          0x13
.equ    CPSR_MODE_ABORT,        0x17
.equ    CPSR_MODE_UNDEFINED,    0x1B
.equ    CPSR_MODE_SYSTEM,       0x1F

// See ARM Tech Ref section A2.5 (Program status registers)
.equ    CPSR_IRQ_INHIBIT,       0x80
.equ    CPSR_FIQ_INHIBIT,       0x40
.equ    CPSR_THUMB,             0x20

.equ    SP_INIT,                0x7FFC

// This is the main entry point for the program
// All we do here is define the IVT
.globl _start
_start:
    ldr pc,reset_handler
    ldr pc,undefined_handler
    ldr pc,swi_handler
    ldr pc,prefetch_handler
    ldr pc,data_handler
    ldr pc,hyp_handler
    ldr pc,irq_handler
    ldr pc,fiq_handler
reset_handler:      .word reset
undefined_handler:  .word alarm
swi_handler:        .word hang
prefetch_handler:   .word hang
data_handler:       .word hang
hyp_handler:        .word hang
irq_handler:        .word irq
fiq_handler:        .word hang

// Reset exception handler, called whenever the processor resets
reset:

    // Enter IRQ mode and setup the stack pointer
    mov r0, #(CPSR_MODE_IRQ | CPSR_IRQ_INHIBIT | CPSR_FIQ_INHIBIT )
    msr cpsr_c, r0
    ldr sp, =SP_INIT

    // return to Supervisor mode and setup the stack pointer
    mov r0, #(CPSR_MODE_SVR | CPSR_IRQ_INHIBIT | CPSR_FIQ_INHIBIT )
    msr cpsr_c, r0
    mov sp, #0x8000000

    // call the function to run this core's initialization (must be called by all cores)
    bl _init_core

    // branch to the main function (defined in C)
    bl kernel_main
    
// alarm handler, called if something bad happens (like undefined instructions)
alarm: 
    bl c_hang

// general hang function. will just loop forever
hang:
    b hang

// Core init function
.global _init_core
_init_core:
    // set all registers to 0
    mov r0, #0
    mov r1, #0
    mov r2, #0
    mov r3, #0
    mov r4, #0
    mov r5, #0
    mov r6, #0
    mov r7, #0
    mov r8, #0
    mov r9, #0

    // configure the location of the IVT. Since the IVT was the first thing in the _start section, it will be located at 0x8000 after linking
    mov r0,#0x8000
    MCR p15, 4, r0, c12, c0, 0

    // turn on the NEON VFP
    MRC p15, #0, r1, c1, c0, #2
    ORR r1, r1, #(0xf << 20)
    MCR p15, #0, r1, c1, c0, #2
    MOV r1, #0
    MCR p15, #0, r1, c7, c5, #4
    MOV r0,#0x40000000
    FMXR FPEXC, r0

    // return
    bx lr


// function to initialize the stack pointer for secondary cores. Called from C.
.global _init_sp
_init_sp:
    mov sp, r0
    bx lr

// write a 32 bit number to a memory location
.globl PUT32
PUT32:
    str r1,[r0]
    bx lr

// read a 32 bit number from a memory location
.globl GET32
GET32:
    ldr r0,[r0]
    bx lr

// dummy function call
.globl dummy
dummy:
    bx lr

// enable IRQ interrupts
.globl _enable_irq
_enable_irq:
    mrs r0,cpsr
    bic r0,r0,#0x80
    msr cpsr_c,r0
    bx lr

// disable IRQ interrupts
.globl _disable_irq
_disable_irq:
    mrs r0,cpsr
    orr r0,r0,#0x80
    msr cpsr_c,r0
    bx lr

// IRQ interrupt handler
irq:
    // push the registers onto the stack (IRQ mode assumes all registers are available to use)
    push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
    // branch to the actual code written in C.
    bl c_irq_handler
    // pop back the registers
    pop  {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
    // special return to return to supervisor mode after an interrupt
    eret
