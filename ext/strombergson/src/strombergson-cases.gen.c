
//------------------------------------------------------------------------------
// TC1: All zero key and IV.
//------------------------------------------------------------------------------
ret += check_case(tc1_chacha8_keylen256_key,
                  tc1_chacha8_keylen256_iv,
                  tc1_chacha8_keylen256_ks0,
                  tc1_chacha8_keylen256_ks1,
                  8,
                  "TC1: All zero key and IV. - tc1_chacha8_keylen256");
ret += check_case(tc1_chacha12_keylen256_key,
                  tc1_chacha12_keylen256_iv,
                  tc1_chacha12_keylen256_ks0,
                  tc1_chacha12_keylen256_ks1,
                  12,
                  "TC1: All zero key and IV. - tc1_chacha12_keylen256");
ret += check_case(tc1_chacha20_keylen256_key,
                  tc1_chacha20_keylen256_iv,
                  tc1_chacha20_keylen256_ks0,
                  tc1_chacha20_keylen256_ks1,
                  20,
                  "TC1: All zero key and IV. - tc1_chacha20_keylen256");

//------------------------------------------------------------------------------
// TC2: Single bit in key set. All zero IV.
//------------------------------------------------------------------------------
ret += check_case(
    tc2_chacha8_keylen256_key,
    tc2_chacha8_keylen256_iv,
    tc2_chacha8_keylen256_ks0,
    tc2_chacha8_keylen256_ks1,
    8,
    "TC2: Single bit in key set. All zero IV. - tc2_chacha8_keylen256");
ret += check_case(
    tc2_chacha12_keylen256_key,
    tc2_chacha12_keylen256_iv,
    tc2_chacha12_keylen256_ks0,
    tc2_chacha12_keylen256_ks1,
    12,
    "TC2: Single bit in key set. All zero IV. - tc2_chacha12_keylen256");
ret += check_case(
    tc2_chacha20_keylen256_key,
    tc2_chacha20_keylen256_iv,
    tc2_chacha20_keylen256_ks0,
    tc2_chacha20_keylen256_ks1,
    20,
    "TC2: Single bit in key set. All zero IV. - tc2_chacha20_keylen256");

//------------------------------------------------------------------------------
// TC3: Single bit in IV set. All zero key.
//------------------------------------------------------------------------------
ret += check_case(
    tc3_chacha8_keylen256_key,
    tc3_chacha8_keylen256_iv,
    tc3_chacha8_keylen256_ks0,
    tc3_chacha8_keylen256_ks1,
    8,
    "TC3: Single bit in IV set. All zero key. - tc3_chacha8_keylen256");
ret += check_case(
    tc3_chacha12_keylen256_key,
    tc3_chacha12_keylen256_iv,
    tc3_chacha12_keylen256_ks0,
    tc3_chacha12_keylen256_ks1,
    12,
    "TC3: Single bit in IV set. All zero key. - tc3_chacha12_keylen256");
ret += check_case(
    tc3_chacha20_keylen256_key,
    tc3_chacha20_keylen256_iv,
    tc3_chacha20_keylen256_ks0,
    tc3_chacha20_keylen256_ks1,
    20,
    "TC3: Single bit in IV set. All zero key. - tc3_chacha20_keylen256");

//------------------------------------------------------------------------------
// TC4: All bits in key and IV are set.
//------------------------------------------------------------------------------
ret +=
    check_case(tc4_chacha8_keylen256_key,
               tc4_chacha8_keylen256_iv,
               tc4_chacha8_keylen256_ks0,
               tc4_chacha8_keylen256_ks1,
               8,
               "TC4: All bits in key and IV are set. - tc4_chacha8_keylen256");
ret +=
    check_case(tc4_chacha12_keylen256_key,
               tc4_chacha12_keylen256_iv,
               tc4_chacha12_keylen256_ks0,
               tc4_chacha12_keylen256_ks1,
               12,
               "TC4: All bits in key and IV are set. - tc4_chacha12_keylen256");
ret +=
    check_case(tc4_chacha20_keylen256_key,
               tc4_chacha20_keylen256_iv,
               tc4_chacha20_keylen256_ks0,
               tc4_chacha20_keylen256_ks1,
               20,
               "TC4: All bits in key and IV are set. - tc4_chacha20_keylen256");

//------------------------------------------------------------------------------
// TC5: Every even bit set in key and IV.
//------------------------------------------------------------------------------
ret += check_case(
    tc5_chacha8_keylen256_key,
    tc5_chacha8_keylen256_iv,
    tc5_chacha8_keylen256_ks0,
    tc5_chacha8_keylen256_ks1,
    8,
    "TC5: Every even bit set in key and IV. - tc5_chacha8_keylen256");
ret += check_case(
    tc5_chacha12_keylen256_key,
    tc5_chacha12_keylen256_iv,
    tc5_chacha12_keylen256_ks0,
    tc5_chacha12_keylen256_ks1,
    12,
    "TC5: Every even bit set in key and IV. - tc5_chacha12_keylen256");
ret += check_case(
    tc5_chacha20_keylen256_key,
    tc5_chacha20_keylen256_iv,
    tc5_chacha20_keylen256_ks0,
    tc5_chacha20_keylen256_ks1,
    20,
    "TC5: Every even bit set in key and IV. - tc5_chacha20_keylen256");

//------------------------------------------------------------------------------
// TC6: Every odd bit set in key and IV.
//------------------------------------------------------------------------------
ret +=
    check_case(tc6_chacha8_keylen256_key,
               tc6_chacha8_keylen256_iv,
               tc6_chacha8_keylen256_ks0,
               tc6_chacha8_keylen256_ks1,
               8,
               "TC6: Every odd bit set in key and IV. - tc6_chacha8_keylen256");
ret += check_case(
    tc6_chacha12_keylen256_key,
    tc6_chacha12_keylen256_iv,
    tc6_chacha12_keylen256_ks0,
    tc6_chacha12_keylen256_ks1,
    12,
    "TC6: Every odd bit set in key and IV. - tc6_chacha12_keylen256");
ret += check_case(
    tc6_chacha20_keylen256_key,
    tc6_chacha20_keylen256_iv,
    tc6_chacha20_keylen256_ks0,
    tc6_chacha20_keylen256_ks1,
    20,
    "TC6: Every odd bit set in key and IV. - tc6_chacha20_keylen256");

//------------------------------------------------------------------------------
// TC7: Sequence patterns in key and IV.
//------------------------------------------------------------------------------
ret +=
    check_case(tc7_chacha8_keylen256_key,
               tc7_chacha8_keylen256_iv,
               tc7_chacha8_keylen256_ks0,
               tc7_chacha8_keylen256_ks1,
               8,
               "TC7: Sequence patterns in key and IV. - tc7_chacha8_keylen256");
ret += check_case(
    tc7_chacha12_keylen256_key,
    tc7_chacha12_keylen256_iv,
    tc7_chacha12_keylen256_ks0,
    tc7_chacha12_keylen256_ks1,
    12,
    "TC7: Sequence patterns in key and IV. - tc7_chacha12_keylen256");
ret += check_case(
    tc7_chacha20_keylen256_key,
    tc7_chacha20_keylen256_iv,
    tc7_chacha20_keylen256_ks0,
    tc7_chacha20_keylen256_ks1,
    20,
    "TC7: Sequence patterns in key and IV. - tc7_chacha20_keylen256");

//------------------------------------------------------------------------------
// TC8: key: 'All your base are belong to us!, IV: 'IETF2013'
//------------------------------------------------------------------------------
ret += check_case(tc8_chacha8_keylen256_key,
                  tc8_chacha8_keylen256_iv,
                  tc8_chacha8_keylen256_ks0,
                  tc8_chacha8_keylen256_ks1,
                  8,
                  "TC8: key: 'All your base are belong to us!, IV: 'IETF2013' "
                  "- tc8_chacha8_keylen256");
ret += check_case(tc8_chacha12_keylen256_key,
                  tc8_chacha12_keylen256_iv,
                  tc8_chacha12_keylen256_ks0,
                  tc8_chacha12_keylen256_ks1,
                  12,
                  "TC8: key: 'All your base are belong to us!, IV: 'IETF2013' "
                  "- tc8_chacha12_keylen256");
ret += check_case(tc8_chacha20_keylen256_key,
                  tc8_chacha20_keylen256_iv,
                  tc8_chacha20_keylen256_ks0,
                  tc8_chacha20_keylen256_ks1,
                  20,
                  "TC8: key: 'All your base are belong to us!, IV: 'IETF2013' "
                  "- tc8_chacha20_keylen256");
