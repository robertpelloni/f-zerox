#include "common.h"

/**
 * @brief Checks if the argument is within the valid track lookup range [0, 22).
 *
 * @param idx The index to check.
 * @return s32 1 if valid, 0 otherwise.
 */
s32 TrackLookup_ValidateIndex(s32 idx) {
    if ((idx >= 0) && (idx < 22)) {
        return 1;
    }
    return 0;
}

s32 func_8006D3F0(s32 arg0) {
    return TrackLookup_ValidateIndex(arg0);
}

/**
 * @brief Sets an entry in the track lookup table.
 *
 * @param idx Index into gTrackLookupTable (0 to 21).
 * @param val Value to assign.
 */
void TrackLookup_SetEntry(s32 idx, s32 val) {
    if (TrackLookup_ValidateIndex(idx)) {
        gTrackLookupTable[idx] = val;
    }
}

void func_8006D414(s32 arg0, s32 arg1, s32 arg2) {
    (void)arg2;
    TrackLookup_SetEntry(arg0, arg1);
}

/**
 * @brief Retrieves an entry from the track lookup table.
 *
 * @param idx Index into gTrackLookupTable (0 to 21).
 * @return s32 Entry value, or 0 if index is out of bounds.
 */
s32 TrackLookup_GetEntry(s32 idx) {
    if (TrackLookup_ValidateIndex(idx)) {
        return gTrackLookupTable[idx];
    }
    return 0;
}

s32 func_8006D448(s32 arg0) {
    return TrackLookup_GetEntry(arg0);
}


void func_8006DAAC(void) { /* TODO: SHIFTABLE - Implement */ }

void func_8006E478(void) { /* TODO: SHIFTABLE - Implement */ }

void func_8006EC7C(void) { /* TODO: SHIFTABLE - Implement */ }

void func_8006F3D8(void) { /* TODO: SHIFTABLE - Implement */ }

void func_8006F444(void) { /* TODO: SHIFTABLE - Implement */ }

void func_8006F478(void) { /* TODO: SHIFTABLE - Implement */ }

void func_8006F4E4(void) { /* TODO: SHIFTABLE - Implement */ }

void func_8006F514(void) { /* TODO: SHIFTABLE - Implement */ }

void func_8006F57C(void) { /* TODO: SHIFTABLE - Implement */ }

void func_8006F5E4(void) { /* TODO: SHIFTABLE - Implement */ }

void func_8006F68C(void) { /* TODO: SHIFTABLE - Implement */ }

void func_8006F6FC(void) { /* TODO: SHIFTABLE - Implement */ }

void func_8006F7A4(void) { /* TODO: SHIFTABLE - Implement */ }

void func_8006F89C(void) { /* TODO: SHIFTABLE - Implement */ }

void func_8006F998(void) { /* TODO: SHIFTABLE - Implement */ }

void func_8006FA94(void) { /* TODO: SHIFTABLE - Implement */ }

void func_8006FB90(void) { /* TODO: SHIFTABLE - Implement */ }

void func_8006FC8C(void) { /* TODO: SHIFTABLE - Implement */ }

void func_8006FD7C(void) { /* TODO: SHIFTABLE - Implement */ }

void func_8006FE90(void) { /* TODO: SHIFTABLE - Implement */ }

void func_8006FF90(void) { /* TODO: SHIFTABLE - Implement */ }

void func_8007049C(void) { /* TODO: SHIFTABLE - Implement */ }

void func_80070B5C(void) { /* TODO: SHIFTABLE - Implement */ }

void func_80071260(void) { /* TODO: SHIFTABLE - Implement */ }

void func_80071790(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800718D0(void) { /* TODO: SHIFTABLE - Implement */ }

void func_80071A58(void) { /* TODO: SHIFTABLE - Implement */ }

void func_80071BE4(void) { /* TODO: SHIFTABLE - Implement */ }

void func_8007243C(void) { /* TODO: SHIFTABLE - Implement */ }

void func_80072758(void) { /* TODO: SHIFTABLE - Implement */ }

void func_80072BB0(void) { /* TODO: SHIFTABLE - Implement */ }

/**
 * @brief Resets track state counters and registers.
 */
void Track_ResetState(void) {
    gTrackResetCount++;
}

void func_80072D00(void) {
    Track_ResetState();
}

/**
 * @brief Initializes track context identifier.
 *
 * @param contextId Identifier for the track context/segment to initialize.
 */
void Track_InitContext(s32 contextId) {
    gTrackContextId = contextId;
}

void func_80072ECC(s32 arg0) {
    Track_InitContext(arg0);
}

void func_800730A4(s32 arg0) { (void)arg0; /* TODO: SHIFTABLE - Implement */ }

void func_8007327C(s32 arg0) { (void)arg0; /* TODO: SHIFTABLE - Implement */ }

void func_80073510(s32 arg0) {
    func_80072D00();
    func_80072ECC(arg0);
    func_800730A4(arg0);
    func_8007327C(arg0);
}

void func_80073548(void) { /* TODO: SHIFTABLE - Implement */ }

void func_80073894(void) { /* TODO: SHIFTABLE - Implement */ }

void func_8007392C(s32 arg0) { (void)arg0; /* TODO: SHIFTABLE - Implement */ }

void func_80073A04(void) { /* TODO: SHIFTABLE - Implement */ }

void func_80073E28(void) { /* TODO: SHIFTABLE - Implement */ }

void func_80073ED0(void) { /* TODO: SHIFTABLE - Implement */ }

void func_80073FA0(void) { /* TODO: SHIFTABLE - Implement */ }

void func_8007402C(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800741DC(s32 arg0) {
    func_80073894();
    func_8007392C(arg0);
}

void func_80074204(void) { /* TODO: SHIFTABLE - Implement */ }

/**
 * @brief Resets the system config state fields unk2 and unk3 to 0.
 */
void SystemConfig_Reset(void) {
    gSystemConfigState.unk2 = 0;
    gSystemConfigState.unk3 = 0;
}

void func_800742D0(void) {
    func_80074204();
    SystemConfig_Reset();
}

void func_800742FC(void) { /* TODO: SHIFTABLE - Implement */ }

void func_80074428(void) { /* TODO: SHIFTABLE - Implement */ }

void func_80074594(void) { /* TODO: SHIFTABLE - Implement */ }

void func_80074634(void) { /* TODO: SHIFTABLE - Implement */ }

void func_80074744(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800747EC(void) { /* TODO: SHIFTABLE - Implement */ }

void func_80074844(void) { /* TODO: SHIFTABLE - Implement */ }

void func_80074A20(void) { /* TODO: SHIFTABLE - Implement */ }
