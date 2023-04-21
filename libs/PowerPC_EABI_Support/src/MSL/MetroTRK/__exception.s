.include "macros.inc"

.section .init, "ax"  # 0x80004744 - 0x80006678

.obj gTRKInterruptVectorTable, global
.string "Metrowerks Target Resident Kernel for PowerPC"
.balign 4, 0
.fill 0xD0
.endobj gTRKInterruptVectorTable

#############################################
# Interrupt vector slot 0x0000 is reserved. #
#############################################

# Slot 0x0100: System Reset Exception
    b __TRKreset
.fill 0xFC

# Slot 0x0200: Machine Check Exception
/* 80004580 00000680  7C 51 43 A6 */	mtspr 0x111, r2
/* 80004584 00000684  7C 5A 02 A6 */	mfspr r2, 0x1a
/* 80004588 00000688  7C 00 17 AC */	icbi 0, r2
/* 8000458C 0000068C  7C 53 02 A6 */	mfdar r2
/* 80004590 00000690  7C 00 13 AC */	dcbi 0, r2
/* 80004594 00000694  7C 51 42 A6 */	mfspr r2, 0x111
/* 80004598 00000698  7C 51 43 A6 */	mtspr 0x111, r2
/* 8000459C 0000069C  7C 72 43 A6 */	mtspr 0x112, r3
/* 800045A0 000006A0  7C 93 43 A6 */	mtspr 0x113, r4
/* 800045A4 000006A4  7C 5A 02 A6 */	mfspr r2, 0x1a
/* 800045A8 000006A8  7C 9B 02 A6 */	mfspr r4, 0x1b
/* 800045AC 000006AC  7C 60 00 A6 */	mfmsr r3
/* 800045B0 000006B0  60 63 00 30 */	ori r3, r3, 0x30
/* 800045B4 000006B4  7C 7B 03 A6 */	mtspr 0x1b, r3
/* 800045B8 000006B8  3C 60 80 2C */	lis r3, TRK_InterruptHandler@h
/* 800045BC 000006BC  60 63 F5 CC */	ori r3, r3, TRK_InterruptHandler@l
/* 800045C0 000006C0  7C 7A 03 A6 */	mtspr 0x1a, r3
/* 800045C4 000006C4  38 60 02 00 */	li r3, 0x200
/* 800045C8 000006C8  4C 00 00 64 */	rfi 
.fill 0xB4

# Slot 0x0300: DSI Exception
/* 80004680 00000780  7C 51 43 A6 */	mtspr 0x111, r2
/* 80004684 00000784  7C 72 43 A6 */	mtspr 0x112, r3
/* 80004688 00000788  7C 93 43 A6 */	mtspr 0x113, r4
/* 8000468C 0000078C  7C 5A 02 A6 */	mfspr r2, 0x1a
/* 80004690 00000790  7C 9B 02 A6 */	mfspr r4, 0x1b
/* 80004694 00000794  7C 60 00 A6 */	mfmsr r3
/* 80004698 00000798  60 63 00 30 */	ori r3, r3, 0x30
/* 8000469C 0000079C  7C 7B 03 A6 */	mtspr 0x1b, r3
/* 800046A0 000007A0  3C 60 80 2C */	lis r3, TRK_InterruptHandler@h
/* 800046A4 000007A4  60 63 F5 CC */	ori r3, r3, TRK_InterruptHandler@l
/* 800046A8 000007A8  7C 7A 03 A6 */	mtspr 0x1a, r3
/* 800046AC 000007AC  38 60 03 00 */	li r3, 0x300
/* 800046B0 000007B0  4C 00 00 64 */	rfi 
.fill 0xCC

# Slot 0x0400: ISI Exception
/* 80004780 00000880  7C 51 43 A6 */	mtspr 0x111, r2
/* 80004784 00000884  7C 72 43 A6 */	mtspr 0x112, r3
/* 80004788 00000888  7C 93 43 A6 */	mtspr 0x113, r4
/* 8000478C 0000088C  7C 5A 02 A6 */	mfspr r2, 0x1a
/* 80004790 00000890  7C 9B 02 A6 */	mfspr r4, 0x1b
/* 80004794 00000894  7C 60 00 A6 */	mfmsr r3
/* 80004798 00000898  60 63 00 30 */	ori r3, r3, 0x30
/* 8000479C 0000089C  7C 7B 03 A6 */	mtspr 0x1b, r3
/* 800047A0 000008A0  3C 60 80 2C */	lis r3, TRK_InterruptHandler@h
/* 800047A4 000008A4  60 63 F5 CC */	ori r3, r3, TRK_InterruptHandler@l
/* 800047A8 000008A8  7C 7A 03 A6 */	mtspr 0x1a, r3
/* 800047AC 000008AC  38 60 04 00 */	li r3, 0x400
/* 800047B0 000008B0  4C 00 00 64 */	rfi 
.fill 0xCC

# Slot 0x0500: External Interrupt Exception
/* 80004880 00000980  7C 51 43 A6 */	mtspr 0x111, r2
/* 80004884 00000984  7C 72 43 A6 */	mtspr 0x112, r3
/* 80004888 00000988  7C 93 43 A6 */	mtspr 0x113, r4
/* 8000488C 0000098C  7C 5A 02 A6 */	mfspr r2, 0x1a
/* 80004890 00000990  7C 9B 02 A6 */	mfspr r4, 0x1b
/* 80004894 00000994  7C 60 00 A6 */	mfmsr r3
/* 80004898 00000998  60 63 00 30 */	ori r3, r3, 0x30
/* 8000489C 0000099C  7C 7B 03 A6 */	mtspr 0x1b, r3
/* 800048A0 000009A0  3C 60 80 2C */	lis r3, TRK_InterruptHandler@h
/* 800048A4 000009A4  60 63 F5 CC */	ori r3, r3, TRK_InterruptHandler@l
/* 800048A8 000009A8  7C 7A 03 A6 */	mtspr 0x1a, r3
/* 800048AC 000009AC  38 60 05 00 */	li r3, 0x500
/* 800048B0 000009B0  4C 00 00 64 */	rfi 
.fill 0xCC

# Slot 0x0600: Alignment Exception
/* 80004980 00000A80  7C 51 43 A6 */	mtspr 0x111, r2
/* 80004984 00000A84  7C 72 43 A6 */	mtspr 0x112, r3
/* 80004988 00000A88  7C 93 43 A6 */	mtspr 0x113, r4
/* 8000498C 00000A8C  7C 5A 02 A6 */	mfspr r2, 0x1a
/* 80004990 00000A90  7C 9B 02 A6 */	mfspr r4, 0x1b
/* 80004994 00000A94  7C 60 00 A6 */	mfmsr r3
/* 80004998 00000A98  60 63 00 30 */	ori r3, r3, 0x30
/* 8000499C 00000A9C  7C 7B 03 A6 */	mtspr 0x1b, r3
/* 800049A0 00000AA0  3C 60 80 2C */	lis r3, TRK_InterruptHandler@h
/* 800049A4 00000AA4  60 63 F5 CC */	ori r3, r3, TRK_InterruptHandler@l
/* 800049A8 00000AA8  7C 7A 03 A6 */	mtspr 0x1a, r3
/* 800049AC 00000AAC  38 60 06 00 */	li r3, 0x600
/* 800049B0 00000AB0  4C 00 00 64 */	rfi 
.fill 0xCC

# Slot 0x0700: Program Exception
/* 80004A80 00000B80  7C 51 43 A6 */	mtspr 0x111, r2
/* 80004A84 00000B84  7C 72 43 A6 */	mtspr 0x112, r3
/* 80004A88 00000B88  7C 93 43 A6 */	mtspr 0x113, r4
/* 80004A8C 00000B8C  7C 5A 02 A6 */	mfspr r2, 0x1a
/* 80004A90 00000B90  7C 9B 02 A6 */	mfspr r4, 0x1b
/* 80004A94 00000B94  7C 60 00 A6 */	mfmsr r3
/* 80004A98 00000B98  60 63 00 30 */	ori r3, r3, 0x30
/* 80004A9C 00000B9C  7C 7B 03 A6 */	mtspr 0x1b, r3
/* 80004AA0 00000BA0  3C 60 80 2C */	lis r3, TRK_InterruptHandler@h
/* 80004AA4 00000BA4  60 63 F5 CC */	ori r3, r3, TRK_InterruptHandler@l
/* 80004AA8 00000BA8  7C 7A 03 A6 */	mtspr 0x1a, r3
/* 80004AAC 00000BAC  38 60 07 00 */	li r3, 0x700
/* 80004AB0 00000BB0  4C 00 00 64 */	rfi 
.fill 0xCC

# Slot 0x0800: Floating Point Unavailable Exception
/* 80004B80 00000C80  7C 51 43 A6 */	mtspr 0x111, r2
/* 80004B84 00000C84  7C 72 43 A6 */	mtspr 0x112, r3
/* 80004B88 00000C88  7C 93 43 A6 */	mtspr 0x113, r4
/* 80004B8C 00000C8C  7C 5A 02 A6 */	mfspr r2, 0x1a
/* 80004B90 00000C90  7C 9B 02 A6 */	mfspr r4, 0x1b
/* 80004B94 00000C94  7C 60 00 A6 */	mfmsr r3
/* 80004B98 00000C98  60 63 00 30 */	ori r3, r3, 0x30
/* 80004B9C 00000C9C  7C 7B 03 A6 */	mtspr 0x1b, r3
/* 80004BA0 00000CA0  3C 60 80 2C */	lis r3, TRK_InterruptHandler@h
/* 80004BA4 00000CA4  60 63 F5 CC */	ori r3, r3, TRK_InterruptHandler@l
/* 80004BA8 00000CA8  7C 7A 03 A6 */	mtspr 0x1a, r3
/* 80004BAC 00000CAC  38 60 08 00 */	li r3, 0x800
/* 80004BB0 00000CB0  4C 00 00 64 */	rfi 
.fill 0xCC

# Slot 0x0900: Decrementer Exception
/* 80004C80 00000D80  7C 51 43 A6 */	mtspr 0x111, r2
/* 80004C84 00000D84  7C 72 43 A6 */	mtspr 0x112, r3
/* 80004C88 00000D88  7C 93 43 A6 */	mtspr 0x113, r4
/* 80004C8C 00000D8C  7C 5A 02 A6 */	mfspr r2, 0x1a
/* 80004C90 00000D90  7C 9B 02 A6 */	mfspr r4, 0x1b
/* 80004C94 00000D94  7C 60 00 A6 */	mfmsr r3
/* 80004C98 00000D98  60 63 00 30 */	ori r3, r3, 0x30
/* 80004C9C 00000D9C  7C 7B 03 A6 */	mtspr 0x1b, r3
/* 80004CA0 00000DA0  3C 60 80 2C */	lis r3, TRK_InterruptHandler@h
/* 80004CA4 00000DA4  60 63 F5 CC */	ori r3, r3, TRK_InterruptHandler@l
/* 80004CA8 00000DA8  7C 7A 03 A6 */	mtspr 0x1a, r3
/* 80004CAC 00000DAC  38 60 09 00 */	li r3, 0x900
/* 80004CB0 00000DB0  4C 00 00 64 */	rfi 
.fill 0xCC

######################################################
# Interrupt vector slots 0x0A00 & 0x0B00 are reserved.
.fill 0x100 
.fill 0x100 
######################################################

# Slot 0x0C00: System Call Exception
/* 80004F80 00001080  7C 51 43 A6 */	mtspr 0x111, r2
/* 80004F84 00001084  7C 72 43 A6 */	mtspr 0x112, r3
/* 80004F88 00001088  7C 93 43 A6 */	mtspr 0x113, r4
/* 80004F8C 0000108C  7C 5A 02 A6 */	mfspr r2, 0x1a
/* 80004F90 00001090  7C 9B 02 A6 */	mfspr r4, 0x1b
/* 80004F94 00001094  7C 60 00 A6 */	mfmsr r3
/* 80004F98 00001098  60 63 00 30 */	ori r3, r3, 0x30
/* 80004F9C 0000109C  7C 7B 03 A6 */	mtspr 0x1b, r3
/* 80004FA0 000010A0  3C 60 80 2C */	lis r3, TRK_InterruptHandler@h
/* 80004FA4 000010A4  60 63 F5 CC */	ori r3, r3, TRK_InterruptHandler@l
/* 80004FA8 000010A8  7C 7A 03 A6 */	mtspr 0x1a, r3
/* 80004FAC 000010AC  38 60 0C 00 */	li r3, 0xc00
/* 80004FB0 000010B0  4C 00 00 64 */	rfi 
.fill 0xCC

# Slot 0x0D00: Trace Exception
/* 80005080 00001180  7C 51 43 A6 */	mtspr 0x111, r2
/* 80005084 00001184  7C 72 43 A6 */	mtspr 0x112, r3
/* 80005088 00001188  7C 93 43 A6 */	mtspr 0x113, r4
/* 8000508C 0000118C  7C 5A 02 A6 */	mfspr r2, 0x1a
/* 80005090 00001190  7C 9B 02 A6 */	mfspr r4, 0x1b
/* 80005094 00001194  7C 60 00 A6 */	mfmsr r3
/* 80005098 00001198  60 63 00 30 */	ori r3, r3, 0x30
/* 8000509C 0000119C  7C 7B 03 A6 */	mtspr 0x1b, r3
/* 800050A0 000011A0  3C 60 80 2C */	lis r3, TRK_InterruptHandler@h
/* 800050A4 000011A4  60 63 F5 CC */	ori r3, r3, TRK_InterruptHandler@l
/* 800050A8 000011A8  7C 7A 03 A6 */	mtspr 0x1a, r3
/* 800050AC 000011AC  38 60 0D 00 */	li r3, 0xd00
/* 800050B0 000011B0  4C 00 00 64 */	rfi 
.fill 0xCC

############################################################################
# Slot 0x0E00 is usually for the Floating Point Assist Exception Handler,  #
# however that exception is not implemented in the PPC 750CL architecture. #
############################################################################

# Slot 0x0F00: Performance Monitor Exception
/* 80005180 00001280  7C 51 43 A6 */	mtspr 0x111, r2
/* 80005184 00001284  7C 72 43 A6 */	mtspr 0x112, r3
/* 80005188 00001288  7C 93 43 A6 */	mtspr 0x113, r4
/* 8000518C 0000128C  7C 5A 02 A6 */	mfspr r2, 0x1a
/* 80005190 00001290  7C 9B 02 A6 */	mfspr r4, 0x1b
/* 80005194 00001294  7C 60 00 A6 */	mfmsr r3
/* 80005198 00001298  60 63 00 30 */	ori r3, r3, 0x30
/* 8000519C 0000129C  7C 7B 03 A6 */	mtspr 0x1b, r3
/* 800051A0 000012A0  3C 60 80 2C */	lis r3, TRK_InterruptHandler@h
/* 800051A4 000012A4  60 63 F5 CC */	ori r3, r3, TRK_InterruptHandler@l
/* 800051A8 000012A8  7C 7A 03 A6 */	mtspr 0x1a, r3
/* 800051AC 000012AC  38 60 0E 00 */	li r3, 0xe00
/* 800051B0 000012B0  4C 00 00 64 */	rfi 
.fill 0xCC

##################################################################################
# Interrupt vector slots 0x1000 through 0x1200 are not implemented in the 750CL. #
##################################################################################

# Slot 0x1300: Instruction Address Breakpoint Exception
/* 80005280 00001380  48 00 00 54 */	b .L_800052D4
.fill 0x1C
/* 800052A0 000013A0  7C 51 43 A6 */	mtspr 0x111, r2
/* 800052A4 000013A4  7C 72 43 A6 */	mtspr 0x112, r3
/* 800052A8 000013A8  7C 93 43 A6 */	mtspr 0x113, r4
/* 800052AC 000013AC  7C 5A 02 A6 */	mfspr r2, 0x1a
/* 800052B0 000013B0  7C 9B 02 A6 */	mfspr r4, 0x1b
/* 800052B4 000013B4  7C 60 00 A6 */	mfmsr r3
/* 800052B8 000013B8  60 63 00 30 */	ori r3, r3, 0x30
/* 800052BC 000013BC  7C 7B 03 A6 */	mtspr 0x1b, r3
/* 800052C0 000013C0  3C 60 80 2C */	lis r3, TRK_InterruptHandler@h
/* 800052C4 000013C4  60 63 F5 CC */	ori r3, r3, TRK_InterruptHandler@l
/* 800052C8 000013C8  7C 7A 03 A6 */	mtspr 0x1a, r3
/* 800052CC 000013CC  38 60 0F 20 */	li r3, 0xf20
/* 800052D0 000013D0  4C 00 00 64 */	rfi 
.L_800052D4:
/* 800052D4 000013D4  7C 51 43 A6 */	mtspr 0x111, r2
/* 800052D8 000013D8  7C 72 43 A6 */	mtspr 0x112, r3
/* 800052DC 000013DC  7C 93 43 A6 */	mtspr 0x113, r4
/* 800052E0 000013E0  7C 5A 02 A6 */	mfspr r2, 0x1a
/* 800052E4 000013E4  7C 9B 02 A6 */	mfspr r4, 0x1b
/* 800052E8 000013E8  7C 60 00 A6 */	mfmsr r3
/* 800052EC 000013EC  60 63 00 30 */	ori r3, r3, 0x30
/* 800052F0 000013F0  7C 7B 03 A6 */	mtspr 0x1b, r3
/* 800052F4 000013F4  3C 60 80 2C */	lis r3, TRK_InterruptHandler@h
/* 800052F8 000013F8  60 63 F5 CC */	ori r3, r3, TRK_InterruptHandler@l
/* 800052FC 000013FC  7C 7A 03 A6 */	mtspr 0x1a, r3
/* 80005300 00001400  38 60 0F 00 */	li r3, 0xf00
/* 80005304 00001404  4C 00 00 64 */	rfi 
.fill 0x78

# Slot 0x1400: System Management Interrupt Exception
/* 80005380 00001480  7C 51 43 A6 */	mtspr 0x111, r2
/* 80005384 00001484  7C 40 00 26 */	mfcr r2
/* 80005388 00001488  7C 52 43 A6 */	mtspr 0x112, r2
/* 8000538C 0000148C  7C 40 00 A6 */	mfmsr r2
/* 80005390 00001490  74 42 00 02 */	andis. r2, r2, 2
/* 80005394 00001494  41 82 00 1C */	beq .L_800053B0
/* 80005398 00001498  7C 40 00 A6 */	mfmsr r2
/* 8000539C 0000149C  6C 42 00 02 */	xoris r2, r2, 2
/* 800053A0 000014A0  7C 00 04 AC */	sync 0
/* 800053A4 000014A4  7C 40 01 24 */	mtmsr r2
/* 800053A8 000014A8  7C 00 04 AC */	sync 0
/* 800053AC 000014AC  7C 51 43 A6 */	mtspr 0x111, r2
.L_800053B0:
/* 800053B0 000014B0  7C 52 42 A6 */	mfspr r2, 0x112
/* 800053B4 000014B4  7C 4F F1 20 */	mtcrf 0xff, r2
/* 800053B8 000014B8  7C 51 42 A6 */	mfspr r2, 0x111
/* 800053BC 000014BC  7C 51 43 A6 */	mtspr 0x111, r2
/* 800053C0 000014C0  7C 72 43 A6 */	mtspr 0x112, r3
/* 800053C4 000014C4  7C 93 43 A6 */	mtspr 0x113, r4
/* 800053C8 000014C8  7C 5A 02 A6 */	mfspr r2, 0x1a
/* 800053CC 000014CC  7C 9B 02 A6 */	mfspr r4, 0x1b
/* 800053D0 000014D0  7C 60 00 A6 */	mfmsr r3
/* 800053D4 000014D4  60 63 00 30 */	ori r3, r3, 0x30
/* 800053D8 000014D8  7C 7B 03 A6 */	mtspr 0x1b, r3
/* 800053DC 000014DC  3C 60 80 2C */	lis r3, TRK_InterruptHandler@h
/* 800053E0 000014E0  60 63 F5 CC */	ori r3, r3, TRK_InterruptHandler@l
/* 800053E4 000014E4  7C 7A 03 A6 */	mtspr 0x1a, r3
/* 800053E8 000014E8  38 60 10 00 */	li r3, 0x1000
/* 800053EC 000014EC  4C 00 00 64 */	rfi 
.fill 0x90


##############################################################################
# Interrupt vector slots 0x1500 and 0x1600 are not implemented in the 750CL. #
##############################################################################

# Slot 0x1700: Thermal-Management Interrupt Exception
/* 80005480 00001580  7C 51 43 A6 */	mtspr 0x111, r2
/* 80005484 00001584  7C 40 00 26 */	mfcr r2
/* 80005488 00001588  7C 52 43 A6 */	mtspr 0x112, r2
/* 8000548C 0000158C  7C 40 00 A6 */	mfmsr r2
/* 80005490 00001590  74 42 00 02 */	andis. r2, r2, 2
/* 80005494 00001594  41 82 00 1C */	beq .L_800054B0
/* 80005498 00001598  7C 40 00 A6 */	mfmsr r2
/* 8000549C 0000159C  6C 42 00 02 */	xoris r2, r2, 2
/* 800054A0 000015A0  7C 00 04 AC */	sync 0
/* 800054A4 000015A4  7C 40 01 24 */	mtmsr r2
/* 800054A8 000015A8  7C 00 04 AC */	sync 0
/* 800054AC 000015AC  7C 51 43 A6 */	mtspr 0x111, r2
.L_800054B0:
/* 800054B0 000015B0  7C 52 42 A6 */	mfspr r2, 0x112
/* 800054B4 000015B4  7C 4F F1 20 */	mtcrf 0xff, r2
/* 800054B8 000015B8  7C 51 42 A6 */	mfspr r2, 0x111
/* 800054BC 000015BC  7C 51 43 A6 */	mtspr 0x111, r2
/* 800054C0 000015C0  7C 72 43 A6 */	mtspr 0x112, r3
/* 800054C4 000015C4  7C 93 43 A6 */	mtspr 0x113, r4
/* 800054C8 000015C8  7C 5A 02 A6 */	mfspr r2, 0x1a
/* 800054CC 000015CC  7C 9B 02 A6 */	mfspr r4, 0x1b
/* 800054D0 000015D0  7C 60 00 A6 */	mfmsr r3
/* 800054D4 000015D4  60 63 00 30 */	ori r3, r3, 0x30
/* 800054D8 000015D8  7C 7B 03 A6 */	mtspr 0x1b, r3
/* 800054DC 000015DC  3C 60 80 2C */	lis r3, TRK_InterruptHandler@h
/* 800054E0 000015E0  60 63 F5 CC */	ori r3, r3, TRK_InterruptHandler@l
/* 800054E4 000015E4  7C 7A 03 A6 */	mtspr 0x1a, r3
/* 800054E8 000015E8  38 60 11 00 */	li r3, 0x1100
/* 800054EC 000015EC  4C 00 00 64 */	rfi 
.fill 0x90

# Slot 0x1800(?)
/* 80005580 00001680  7C 51 43 A6 */	mtspr 0x111, r2
/* 80005584 00001684  7C 40 00 26 */	mfcr r2
/* 80005588 00001688  7C 52 43 A6 */	mtspr 0x112, r2
/* 8000558C 0000168C  7C 40 00 A6 */	mfmsr r2
/* 80005590 00001690  74 42 00 02 */	andis. r2, r2, 2
/* 80005594 00001694  41 82 00 1C */	beq .L_800055B0
/* 80005598 00001698  7C 40 00 A6 */	mfmsr r2
/* 8000559C 0000169C  6C 42 00 02 */	xoris r2, r2, 2
/* 800055A0 000016A0  7C 00 04 AC */	sync 0
/* 800055A4 000016A4  7C 40 01 24 */	mtmsr r2
/* 800055A8 000016A8  7C 00 04 AC */	sync 0
/* 800055AC 000016AC  7C 51 43 A6 */	mtspr 0x111, r2
.L_800055B0:
/* 800055B0 000016B0  7C 52 42 A6 */	mfspr r2, 0x112
/* 800055B4 000016B4  7C 4F F1 20 */	mtcrf 0xff, r2
/* 800055B8 000016B8  7C 51 42 A6 */	mfspr r2, 0x111
/* 800055BC 000016BC  7C 51 43 A6 */	mtspr 0x111, r2
/* 800055C0 000016C0  7C 72 43 A6 */	mtspr 0x112, r3
/* 800055C4 000016C4  7C 93 43 A6 */	mtspr 0x113, r4
/* 800055C8 000016C8  7C 5A 02 A6 */	mfspr r2, 0x1a
/* 800055CC 000016CC  7C 9B 02 A6 */	mfspr r4, 0x1b
/* 800055D0 000016D0  7C 60 00 A6 */	mfmsr r3
/* 800055D4 000016D4  60 63 00 30 */	ori r3, r3, 0x30
/* 800055D8 000016D8  7C 7B 03 A6 */	mtspr 0x1b, r3
/* 800055DC 000016DC  3C 60 80 2C */	lis r3, TRK_InterruptHandler@h
/* 800055E0 000016E0  60 63 F5 CC */	ori r3, r3, TRK_InterruptHandler@l
/* 800055E4 000016E4  7C 7A 03 A6 */	mtspr 0x1a, r3
/* 800055E8 000016E8  38 60 12 00 */	li r3, 0x1200
/* 800055EC 000016EC  4C 00 00 64 */	rfi 
.fill 0x90

# Slot 0x1900(?)
/* 80005680 00001780  7C 51 43 A6 */	mtspr 0x111, r2
/* 80005684 00001784  7C 72 43 A6 */	mtspr 0x112, r3
/* 80005688 00001788  7C 93 43 A6 */	mtspr 0x113, r4
/* 8000568C 0000178C  7C 5A 02 A6 */	mfspr r2, 0x1a
/* 80005690 00001790  7C 9B 02 A6 */	mfspr r4, 0x1b
/* 80005694 00001794  7C 60 00 A6 */	mfmsr r3
/* 80005698 00001798  60 63 00 30 */	ori r3, r3, 0x30
/* 8000569C 0000179C  7C 7B 03 A6 */	mtspr 0x1b, r3
/* 800056A0 000017A0  3C 60 80 2C */	lis r3, TRK_InterruptHandler@h
/* 800056A4 000017A4  60 63 F5 CC */	ori r3, r3, TRK_InterruptHandler@l
/* 800056A8 000017A8  7C 7A 03 A6 */	mtspr 0x1a, r3
/* 800056AC 000017AC  38 60 13 00 */	li r3, 0x1300
/* 800056B0 000017B0  4C 00 00 64 */	rfi 
.fill 0xCC

# Slot 0x1A00(?)
/* 80005780 00001880  7C 51 43 A6 */	mtspr 0x111, r2
/* 80005784 00001884  7C 72 43 A6 */	mtspr 0x112, r3
/* 80005788 00001888  7C 93 43 A6 */	mtspr 0x113, r4
/* 8000578C 0000188C  7C 5A 02 A6 */	mfspr r2, 0x1a
/* 80005790 00001890  7C 9B 02 A6 */	mfspr r4, 0x1b
/* 80005794 00001894  7C 60 00 A6 */	mfmsr r3
/* 80005798 00001898  60 63 00 30 */	ori r3, r3, 0x30
/* 8000579C 0000189C  7C 7B 03 A6 */	mtspr 0x1b, r3
/* 800057A0 000018A0  3C 60 80 2C */	lis r3, TRK_InterruptHandler@h
/* 800057A4 000018A4  60 63 F5 CC */	ori r3, r3, TRK_InterruptHandler@l
/* 800057A8 000018A8  7C 7A 03 A6 */	mtspr 0x1a, r3
/* 800057AC 000018AC  38 60 14 00 */	li r3, 0x1400
/* 800057B0 000018B0  4C 00 00 64 */	rfi 
.fill 0x1CC

# Slot 0x1B00(?)
/* 80005980 00001A80  7C 51 43 A6 */	mtspr 0x111, r2
/* 80005984 00001A84  7C 72 43 A6 */	mtspr 0x112, r3
/* 80005988 00001A88  7C 93 43 A6 */	mtspr 0x113, r4
/* 8000598C 00001A8C  7C 5A 02 A6 */	mfspr r2, 0x1a
/* 80005990 00001A90  7C 9B 02 A6 */	mfspr r4, 0x1b
/* 80005994 00001A94  7C 60 00 A6 */	mfmsr r3
/* 80005998 00001A98  60 63 00 30 */	ori r3, r3, 0x30
/* 8000599C 00001A9C  7C 7B 03 A6 */	mtspr 0x1b, r3
/* 800059A0 00001AA0  3C 60 80 2C */	lis r3, TRK_InterruptHandler@h
/* 800059A4 00001AA4  60 63 F5 CC */	ori r3, r3, TRK_InterruptHandler@l
/* 800059A8 00001AA8  7C 7A 03 A6 */	mtspr 0x1a, r3
/* 800059AC 00001AAC  38 60 16 00 */	li r3, 0x1600
/* 800059B0 00001AB0  4C 00 00 64 */	rfi 
.fill 0xCC

# Slot 0x1C00(?)
/* 80005A80 00001B80  7C 51 43 A6 */	mtspr 0x111, r2
/* 80005A84 00001B84  7C 72 43 A6 */	mtspr 0x112, r3
/* 80005A88 00001B88  7C 93 43 A6 */	mtspr 0x113, r4
/* 80005A8C 00001B8C  7C 5A 02 A6 */	mfspr r2, 0x1a
/* 80005A90 00001B90  7C 9B 02 A6 */	mfspr r4, 0x1b
/* 80005A94 00001B94  7C 60 00 A6 */	mfmsr r3
/* 80005A98 00001B98  60 63 00 30 */	ori r3, r3, 0x30
/* 80005A9C 00001B9C  7C 7B 03 A6 */	mtspr 0x1b, r3
/* 80005AA0 00001BA0  3C 60 80 2C */	lis r3, TRK_InterruptHandler@h
/* 80005AA4 00001BA4  60 63 F5 CC */	ori r3, r3, TRK_InterruptHandler@l
/* 80005AA8 00001BA8  7C 7A 03 A6 */	mtspr 0x1a, r3
/* 80005AAC 00001BAC  38 60 17 00 */	li r3, 0x1700
/* 80005AB0 00001BB0  4C 00 00 64 */	rfi 
.fill 0x4CC

# Slot 0x1D00(?)
/* 80005F80 00002080  7C 51 43 A6 */	mtspr 0x111, r2
/* 80005F84 00002084  7C 72 43 A6 */	mtspr 0x112, r3
/* 80005F88 00002088  7C 93 43 A6 */	mtspr 0x113, r4
/* 80005F8C 0000208C  7C 5A 02 A6 */	mfspr r2, 0x1a
/* 80005F90 00002090  7C 9B 02 A6 */	mfspr r4, 0x1b
/* 80005F94 00002094  7C 60 00 A6 */	mfmsr r3
/* 80005F98 00002098  60 63 00 30 */	ori r3, r3, 0x30
/* 80005F9C 0000209C  7C 7B 03 A6 */	mtspr 0x1b, r3
/* 80005FA0 000020A0  3C 60 80 2C */	lis r3, TRK_InterruptHandler@h
/* 80005FA4 000020A4  60 63 F5 CC */	ori r3, r3, TRK_InterruptHandler@l
/* 80005FA8 000020A8  7C 7A 03 A6 */	mtspr 0x1a, r3
/* 80005FAC 000020AC  38 60 1C 00 */	li r3, 0x1c00
/* 80005FB0 000020B0  4C 00 00 64 */	rfi 
.fill 0xCC

# Slot 0x1E00(?)
/* 80006080 00002180  7C 51 43 A6 */	mtspr 0x111, r2
/* 80006084 00002184  7C 72 43 A6 */	mtspr 0x112, r3
/* 80006088 00002188  7C 93 43 A6 */	mtspr 0x113, r4
/* 8000608C 0000218C  7C 5A 02 A6 */	mfspr r2, 0x1a
/* 80006090 00002190  7C 9B 02 A6 */	mfspr r4, 0x1b
/* 80006094 00002194  7C 60 00 A6 */	mfmsr r3
/* 80006098 00002198  60 63 00 30 */	ori r3, r3, 0x30
/* 8000609C 0000219C  7C 7B 03 A6 */	mtspr 0x1b, r3
/* 800060A0 000021A0  3C 60 80 2C */	lis r3, TRK_InterruptHandler@h
/* 800060A4 000021A4  60 63 F5 CC */	ori r3, r3, TRK_InterruptHandler@l
/* 800060A8 000021A8  7C 7A 03 A6 */	mtspr 0x1a, r3
/* 800060AC 000021AC  38 60 1D 00 */	li r3, 0x1d00
/* 800060B0 000021B0  4C 00 00 64 */	rfi 
.fill 0xCC

# Slot 0x1F00(?)
/* 80006180 00002280  7C 51 43 A6 */	mtspr 0x111, r2
/* 80006184 00002284  7C 72 43 A6 */	mtspr 0x112, r3
/* 80006188 00002288  7C 93 43 A6 */	mtspr 0x113, r4
/* 8000618C 0000228C  7C 5A 02 A6 */	mfspr r2, 0x1a
/* 80006190 00002290  7C 9B 02 A6 */	mfspr r4, 0x1b
/* 80006194 00002294  7C 60 00 A6 */	mfmsr r3
/* 80006198 00002298  60 63 00 30 */	ori r3, r3, 0x30
/* 8000619C 0000229C  7C 7B 03 A6 */	mtspr 0x1b, r3
/* 800061A0 000022A0  3C 60 80 2C */	lis r3, TRK_InterruptHandler@h
/* 800061A4 000022A4  60 63 F5 CC */	ori r3, r3, TRK_InterruptHandler@l
/* 800061A8 000022A8  7C 7A 03 A6 */	mtspr 0x1a, r3
/* 800061AC 000022AC  38 60 1E 00 */	li r3, 0x1e00
/* 800061B0 000022B0  4C 00 00 64 */	rfi 
.fill 0xCC

# Slot 0x2000(?)
/* 80006280 00002380  7C 51 43 A6 */	mtspr 0x111, r2
/* 80006284 00002384  7C 72 43 A6 */	mtspr 0x112, r3
/* 80006288 00002388  7C 93 43 A6 */	mtspr 0x113, r4
/* 8000628C 0000238C  7C 5A 02 A6 */	mfspr r2, 0x1a
/* 80006290 00002390  7C 9B 02 A6 */	mfspr r4, 0x1b
/* 80006294 00002394  7C 60 00 A6 */	mfmsr r3
/* 80006298 00002398  60 63 00 30 */	ori r3, r3, 0x30
/* 8000629C 0000239C  7C 7B 03 A6 */	mtspr 0x1b, r3
/* 800062A0 000023A0  3C 60 80 2C */	lis r3, TRK_InterruptHandler@h
/* 800062A4 000023A4  60 63 F5 CC */	ori r3, r3, TRK_InterruptHandler@l
/* 800062A8 000023A8  7C 7A 03 A6 */	mtspr 0x1a, r3
/* 800062AC 000023AC  38 60 1F 00 */	li r3, 0x1f00
/* 800062B0 000023B0  4C 00 00 64 */	rfi 
gTRKInterruptVectorTableEnd:
