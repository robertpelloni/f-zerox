#include "common.h"

void func_80068B20(void) {
    D_800DCE44 = -1;
    D_800DCE48 = 0x8000;

    if (D_800DCE60 != 0x20DE1529) {
        func_8008DB98();
        D_800DCE60 = 0x20DE1529;
        func_800A4BAC();
    } else {
        func_8008DA68();
        func_800A4B54();
    }

    func_80085510();
    func_800FC730();
    func_8007F500();
    func_80076848();
    func_8007D9D0();

    D_800CD16C = 1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/math_utils/func_80068BC0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/math_utils/func_80068DCC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/math_utils/func_80068F04.s")

void func_80069698(void) {
    s32 arg = 0; // Default or garbage value
    if (D_800CD044 != 3) {
        if (D_80106DA0 != 0) {
            typedef s32 (*FuncPtr)(void);
            FuncPtr target = (FuncPtr)D_800CD0FC[D_800DCE44 & 0x1F];
            arg = target();
        }
    }
    func_800FD184(arg);
}

#pragma GLOBAL_ASM("asm/nonmatchings/math_utils/func_80069700.s")

#pragma GLOBAL_ASM("asm/nonmatchings/math_utils/func_80069790.s")

#pragma GLOBAL_ASM("asm/nonmatchings/math_utils/func_80069820.s")

#pragma GLOBAL_ASM("asm/nonmatchings/math_utils/func_80069D44.s")

#pragma GLOBAL_ASM("asm/nonmatchings/math_utils/func_80069ED0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/math_utils/func_80069F5C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/math_utils/func_8006A00C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/math_utils/func_8006A3AC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/math_utils/func_8006A6E4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/math_utils/osSetTime.s")

void func_8006A904(s32 arg0, s32 arg1) {
    D_800CD178 = arg0;
    D_800CD17C = arg1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/math_utils/func_8006A918.s")

#pragma GLOBAL_ASM("asm/nonmatchings/math_utils/func_8006A978.s")

/**
 * @brief Rounds a floating point number to the nearest integer.
 *
 * @param arg0 Input float.
 * @return s32 Rounded integer.
 */
s32 func_8006A9E0(f32 arg0) {
    if (arg0 < 0.0f) {
        return (s32) (arg0 - 0.5f);
    }
    return (s32) (arg0 + 0.5f);
}

#pragma GLOBAL_ASM("asm/nonmatchings/math_utils/func_8006AA38.s")

#pragma GLOBAL_ASM("asm/nonmatchings/math_utils/func_8006AC10.s")

#pragma GLOBAL_ASM("asm/nonmatchings/math_utils/func_8006ADE4.s")

/**
 * @brief Sets two 3-byte vectors (likely RGB colors) in the struct to the same values.
 *
 * @param arg0 Pointer to the struct.
 * @param arg1 Value for index 0 and 4.
 * @param arg2 Value for index 1 and 5.
 * @param arg3 Value for index 2 and 6.
 */
void func_8006AFC8(struct UnkStruct_10* arg0, s32 arg1, s32 arg2, s32 arg3) {
    arg0->unk0 = arg0->unk4 = arg1;
    arg0->unk1 = arg0->unk5 = arg2;
    arg0->unk2 = arg0->unk6 = arg3;
}

/**
 * @brief Sets two 3-byte vectors in the struct to the same values. Identical to func_8006AFC8.
 */
void func_8006AFE4(struct UnkStruct_10* arg0, s32 arg1, s32 arg2, s32 arg3) {
    arg0->unk0 = arg0->unk4 = arg1;
    arg0->unk1 = arg0->unk5 = arg2;
    arg0->unk2 = arg0->unk6 = arg3;
}

/**
 * @brief Sets the last three bytes of the struct (offsets 0x8, 0x9, 0xA).
 *
 * @param arg0 Pointer to the struct.
 * @param arg1 Value for offset 0x8.
 * @param arg2 Value for offset 0x9.
 * @param arg3 Value for offset 0xA.
 */
void func_8006B000(struct UnkStruct_8* arg0, s32 arg1, s32 arg2, s32 arg3) {
    arg0->unk8 = arg1;
    arg0->unk9 = arg2;
    arg0->unkA = arg3;
}

#pragma GLOBAL_ASM("asm/nonmatchings/math_utils/func_8006B010.s")

#pragma GLOBAL_ASM("asm/nonmatchings/math_utils/func_8006B07C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/math_utils/func_8006B18C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/math_utils/func_8006B33C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/math_utils/func_8006B908.s")

#pragma GLOBAL_ASM("asm/nonmatchings/math_utils/func_8006BB80.s")

#pragma GLOBAL_ASM("asm/nonmatchings/math_utils/func_8006BBE8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/math_utils/func_8006BC84.s")

#pragma GLOBAL_ASM("asm/nonmatchings/math_utils/func_8006BFCC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/math_utils/func_8006C278.s")

#pragma GLOBAL_ASM("asm/nonmatchings/math_utils/func_8006C378.s")

#pragma GLOBAL_ASM("asm/nonmatchings/math_utils/func_8006C520.s")

#pragma GLOBAL_ASM("asm/nonmatchings/math_utils/func_8006CB0C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/math_utils/func_8006CC98.s")

#pragma GLOBAL_ASM("asm/nonmatchings/math_utils/func_8006D03C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/math_utils/func_8006D2E0.s")
