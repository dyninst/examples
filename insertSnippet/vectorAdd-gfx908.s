
vectorAdd-gfx908:	file format elf64-amdgpu

Disassembly of section .text:

0000000000001000 <_Z15vectoradd_floatPfPKfS1_ii>:
	s_load_dwordx2 s[0:1], s[6:7], 0x18                        // 000000001000: C0060003 00000018
	s_load_dword s2, s[4:5], 0x4                               // 000000001008: C0020082 00000004
	s_waitcnt lgkmcnt(0)                                       // 000000001010: BF8CC07F
	s_lshr_b32 s3, s2, 16                                      // 000000001014: 8F039002
	s_mul_i32 s9, s9, s3                                       // 000000001018: 92090309
	v_add_u32_e32 v1, s9, v1                                   // 00000000101C: 68020209
	v_mul_lo_u32 v1, v1, s0                                    // 000000001020: D2850001 00000101
	s_and_b32 s2, s2, 0xffff                                   // 000000001028: 8602FF02 0000FFFF
	s_mul_i32 s8, s8, s2                                       // 000000001030: 92080208
	s_mul_i32 s0, s1, s0                                       // 000000001034: 92000001
	v_add3_u32 v0, s8, v0, v1                                  // 000000001038: D1FF0000 04060008
	v_cmp_gt_i32_e32 vcc, s0, v0                               // 000000001040: 7D880000
	s_and_saveexec_b64 s[0:1], vcc                             // 000000001044: BE80206A
	s_cbranch_execz BB0_2                                      // 000000001048: BF88001B
	s_load_dwordx2 s[0:1], s[6:7], 0x0                         // 00000000104C: C0060003 00000000
	s_load_dwordx2 s[2:3], s[6:7], 0x8                         // 000000001054: C0060083 00000008
	s_load_dwordx2 s[4:5], s[6:7], 0x10                        // 00000000105C: C0060103 00000010
	v_ashrrev_i32_e32 v1, 31, v0                               // 000000001064: 2202009F
	v_lshlrev_b64 v[0:1], 2, v[0:1]                            // 000000001068: D28F0000 00020082
	s_waitcnt lgkmcnt(0)                                       // 000000001070: BF8CC07F
	v_mov_b32_e32 v3, s1                                       // 000000001074: 7E060201
	v_add_co_u32_e32 v2, vcc, s0, v0                           // 000000001078: 32040000
	v_addc_co_u32_e32 v3, vcc, v3, v1, vcc                     // 00000000107C: 38060303
	v_mov_b32_e32 v5, s5                                       // 000000001080: 7E0A0205
	v_add_co_u32_e32 v4, vcc, s4, v0                           // 000000001084: 32080004
	v_addc_co_u32_e32 v5, vcc, v5, v1, vcc                     // 000000001088: 380A0305
	v_mov_b32_e32 v6, s3                                       // 00000000108C: 7E0C0203
	v_add_co_u32_e32 v0, vcc, s2, v0                           // 000000001090: 32000002
	v_addc_co_u32_e32 v1, vcc, v6, v1, vcc                     // 000000001094: 38020306
	global_load_dword v6, v[0:1], off                          // 000000001098: DC508000 067F0000
	global_load_dword v7, v[4:5], off                          // 0000000010A0: DC508000 077F0004
	s_waitcnt vmcnt(0)                                         // 0000000010A8: BF8C0F70
	v_add_f32_e32 v0, v6, v7                                   // 0000000010AC: 02000F06
	global_store_dword v[2:3], v0, off                         // 0000000010B0: DC708000 007F0002

00000000000010b8 <BB0_2>:
	s_endpgm                                                   // 0000000010B8: BF810000
