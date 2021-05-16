
#ifndef _FILTER_COEFS_H
#define _FILTER_COEFS_H


static const q15_t q15_hamming_512[] = {
		0x0a3d, 0x0a3e, 0x0a41, 0x0a47, 0x0a4f, 0x0a59, 0x0a66, 0x0a75,
		0x0a86, 0x0a99, 0x0aaf, 0x0ac7, 0x0ae1, 0x0afd, 0x0b1c, 0x0b3d,
		0x0b60, 0x0b85, 0x0bad, 0x0bd6, 0x0c02, 0x0c31, 0x0c61, 0x0c94,
		0x0cc8, 0x0cff, 0x0d39, 0x0d74, 0x0db1, 0x0df1, 0x0e33, 0x0e77,
		0x0ebd, 0x0f05, 0x0f4f, 0x0f9b, 0x0fea, 0x103a, 0x108c, 0x10e1,
		0x1137, 0x1190, 0x11eb, 0x1247, 0x12a5, 0x1306, 0x1368, 0x13cd,
		0x1433, 0x149b, 0x1505, 0x1571, 0x15de, 0x164e, 0x16bf, 0x1732,
		0x17a7, 0x181e, 0x1896, 0x1910, 0x198c, 0x1a0a, 0x1a89, 0x1b0a,
		0x1b8c, 0x1c10, 0x1c96, 0x1d1d, 0x1da6, 0x1e30, 0x1ebc, 0x1f49,
		0x1fd8, 0x2068, 0x20fa, 0x218d, 0x2221, 0x22b7, 0x234e, 0x23e6,
		0x2480, 0x251b, 0x25b7, 0x2654, 0x26f3, 0x2793, 0x2833, 0x28d5,
		0x2979, 0x2a1d, 0x2ac2, 0x2b68, 0x2c0f, 0x2cb8, 0x2d61, 0x2e0b,
		0x2eb6, 0x2f61, 0x300e, 0x30bc, 0x316a, 0x3219, 0x32c9, 0x3379,
		0x342a, 0x34dc, 0x358f, 0x3642, 0x36f5, 0x37a9, 0x385e, 0x3913,
		0x39c9, 0x3a7f, 0x3b35, 0x3bec, 0x3ca4, 0x3d5b, 0x3e13, 0x3ecb,
		0x3f84, 0x403c, 0x40f5, 0x41ae, 0x4267, 0x4320, 0x43d9, 0x4493,
		0x454c, 0x4605, 0x46bf, 0x4778, 0x4831, 0x48ea, 0x49a3, 0x4a5c,
		0x4b14, 0x4bcc, 0x4c84, 0x4d3c, 0x4df3, 0x4eaa, 0x4f61, 0x5017,
		0x50cd, 0x5183, 0x5238, 0x52ec, 0x53a0, 0x5453, 0x5506, 0x55b8,
		0x566a, 0x571b, 0x57cb, 0x587a, 0x5929, 0x59d7, 0x5a84, 0x5b30,
		0x5bdb, 0x5c86, 0x5d2f, 0x5dd8, 0x5e80, 0x5f26, 0x5fcc, 0x6071,
		0x6115, 0x61b7, 0x6259, 0x62f9, 0x6398, 0x6436, 0x64d3, 0x656e,
		0x6609, 0x66a2, 0x6739, 0x67d0, 0x6865, 0x68f8, 0x698b, 0x6a1c,
		0x6aab, 0x6b39, 0x6bc6, 0x6c51, 0x6cda, 0x6d62, 0x6de9, 0x6e6e,
		0x6ef1, 0x6f72, 0x6ff2, 0x7071, 0x70ed, 0x7168, 0x71e2, 0x7259,
		0x72cf, 0x7343, 0x73b5, 0x7426, 0x7494, 0x7501, 0x756c, 0x75d5,
		0x763c, 0x76a1, 0x7705, 0x7766, 0x77c5, 0x7823, 0x787e, 0x78d8,
		0x792f, 0x7985, 0x79d8, 0x7a2a, 0x7a79, 0x7ac7, 0x7b12, 0x7b5b,
		0x7ba2, 0x7be7, 0x7c2a, 0x7c6a, 0x7ca9, 0x7ce5, 0x7d20, 0x7d58,
		0x7d8e, 0x7dc1, 0x7df3, 0x7e22, 0x7e4f, 0x7e7a, 0x7ea3, 0x7ec9,
		0x7eee, 0x7f10, 0x7f2f, 0x7f4d, 0x7f68, 0x7f81, 0x7f98, 0x7fac,
		0x7fbe, 0x7fce, 0x7fdc, 0x7fe7, 0x7ff1, 0x7ff7, 0x7ffc, 0x7ffe,
		0x7ffe, 0x7ffc, 0x7ff7, 0x7ff1, 0x7fe7, 0x7fdc, 0x7fce, 0x7fbe,
		0x7fac, 0x7f98, 0x7f81, 0x7f68, 0x7f4d, 0x7f2f, 0x7f10, 0x7eee,
		0x7ec9, 0x7ea3, 0x7e7a, 0x7e4f, 0x7e22, 0x7df3, 0x7dc1, 0x7d8e,
		0x7d58, 0x7d20, 0x7ce5, 0x7ca9, 0x7c6a, 0x7c2a, 0x7be7, 0x7ba2,
		0x7b5b, 0x7b12, 0x7ac7, 0x7a79, 0x7a2a, 0x79d8, 0x7985, 0x792f,
		0x78d8, 0x787e, 0x7823, 0x77c5, 0x7766, 0x7705, 0x76a1, 0x763c,
		0x75d5, 0x756c, 0x7501, 0x7494, 0x7426, 0x73b5, 0x7343, 0x72cf,
		0x7259, 0x71e2, 0x7168, 0x70ed, 0x7071, 0x6ff2, 0x6f72, 0x6ef1,
		0x6e6e, 0x6de9, 0x6d62, 0x6cda, 0x6c51, 0x6bc6, 0x6b39, 0x6aab,
		0x6a1c, 0x698b, 0x68f8, 0x6865, 0x67d0, 0x6739, 0x66a2, 0x6609,
		0x656e, 0x64d3, 0x6436, 0x6398, 0x62f9, 0x6259, 0x61b7, 0x6115,
		0x6071, 0x5fcc, 0x5f26, 0x5e80, 0x5dd8, 0x5d2f, 0x5c86, 0x5bdb,
		0x5b30, 0x5a84, 0x59d7, 0x5929, 0x587a, 0x57cb, 0x571b, 0x566a,
		0x55b8, 0x5506, 0x5453, 0x53a0, 0x52ec, 0x5238, 0x5183, 0x50cd,
		0x5017, 0x4f61, 0x4eaa, 0x4df3, 0x4d3c, 0x4c84, 0x4bcc, 0x4b14,
		0x4a5c, 0x49a3, 0x48ea, 0x4831, 0x4778, 0x46bf, 0x4605, 0x454c,
		0x4493, 0x43d9, 0x4320, 0x4267, 0x41ae, 0x40f5, 0x403c, 0x3f84,
		0x3ecb, 0x3e13, 0x3d5b, 0x3ca4, 0x3bec, 0x3b35, 0x3a7f, 0x39c9,
		0x3913, 0x385e, 0x37a9, 0x36f5, 0x3642, 0x358f, 0x34dc, 0x342a,
		0x3379, 0x32c9, 0x3219, 0x316a, 0x30bc, 0x300e, 0x2f61, 0x2eb6,
		0x2e0b, 0x2d61, 0x2cb8, 0x2c0f, 0x2b68, 0x2ac2, 0x2a1d, 0x2979,
		0x28d5, 0x2833, 0x2793, 0x26f3, 0x2654, 0x25b7, 0x251b, 0x2480,
		0x23e6, 0x234e, 0x22b7, 0x2221, 0x218d, 0x20fa, 0x2068, 0x1fd8,
		0x1f49, 0x1ebc, 0x1e30, 0x1da6, 0x1d1d, 0x1c96, 0x1c10, 0x1b8c,
		0x1b0a, 0x1a89, 0x1a0a, 0x198c, 0x1910, 0x1896, 0x181e, 0x17a7,
		0x1732, 0x16bf, 0x164e, 0x15de, 0x1571, 0x1505, 0x149b, 0x1433,
		0x13cd, 0x1368, 0x1306, 0x12a5, 0x1247, 0x11eb, 0x1190, 0x1137,
		0x10e1, 0x108c, 0x103a, 0x0fea, 0x0f9b, 0x0f4f, 0x0f05, 0x0ebd,
		0x0e77, 0x0e33, 0x0df1, 0x0db1, 0x0d74, 0x0d39, 0x0cff, 0x0cc8,
		0x0c94, 0x0c61, 0x0c31, 0x0c02, 0x0bd6, 0x0bad, 0x0b85, 0x0b60,
		0x0b3d, 0x0b1c, 0x0afd, 0x0ae1, 0x0ac7, 0x0aaf, 0x0a99, 0x0a86,
		0x0a75, 0x0a66, 0x0a59, 0x0a4f, 0x0a47, 0x0a41, 0x0a3e, 0x0a3d
};


static const q31_t q31_hamming_512[] = {
		0x0a3d70a3, 0x0a3e9455, 0x0a41ff60, 0x0a47b1a3, 0x0a4faae4, 0x0a59ead4, 0x0a66710f, 0x0a753d17,
		0x0a864e5b, 0x0a99a432, 0x0aaf3ddb, 0x0ac71a80, 0x0ae13936, 0x0afd98fa, 0x0b1c38b2, 0x0b3d172e,
		0x0b60332a, 0x0b858b4a, 0x0bad1e1a, 0x0bd6ea14, 0x0c02ed99, 0x0c3126f5, 0x0c61945e, 0x0c9433f5,
		0x0cc903c3, 0x0d0001bd, 0x0d392bc3, 0x0d747f9e, 0x0db1fb02, 0x0df19b8e, 0x0e335ecc, 0x0e774230,
		0x0ebd4319, 0x0f055ed3, 0x0f4f9291, 0x0f9bdb76, 0x0fea368d, 0x103aa0cf, 0x108d171d, 0x10e19648,
		0x11381b09, 0x1190a208, 0x11eb27d8, 0x1247a8f7, 0x12a621d2, 0x13068ec0, 0x1368ec05, 0x13cd35d3,
		0x14336849, 0x149b7f72, 0x15057746, 0x15714bab, 0x15def876, 0x164e7967, 0x16bfca2e, 0x1732e667,
		0x17a7c99f, 0x181e6f50, 0x1896d2e1, 0x1910efa9, 0x198cc0f0, 0x1a0a41ea, 0x1a896dbb, 0x1b0a3f79,
		0x1b8cb225, 0x1c10c0b4, 0x1c966609, 0x1d1d9cf9, 0x1da66046, 0x1e30aaa7, 0x1ebc76c2, 0x1f49bf2c,
		0x1fd87e6e, 0x2068af02, 0x20fa4b53, 0x218d4dbf, 0x2221b094, 0x22b76e15, 0x234e8076, 0x23e6e1de,
		0x24808c67, 0x251b7a1f, 0x25b7a507, 0x26550713, 0x26f39a2c, 0x2793582e, 0x28343aec, 0x28d63c2b,
		0x297955a5, 0x2a1d810b, 0x2ac2b803, 0x2b68f426, 0x2c102f07, 0x2cb8622b, 0x2d618712, 0x2e0b972e,
		0x2eb68beb, 0x2f625eaa, 0x300f08c6, 0x30bc8390, 0x316ac850, 0x3219d049, 0x32c994b3, 0x337a0ec2,
		0x342b37a0, 0x34dd0873, 0x358f7a58, 0x36428668, 0x36f625b5, 0x37aa514a, 0x385f022f, 0x39143165,
		0x39c9d7e9, 0x3a7feeb4, 0x3b366eb9, 0x3bed50e7, 0x3ca48e2c, 0x3d5c1f6f, 0x3e13fd95, 0x3ecc2181,
		0x3f848413, 0x403d1e26, 0x40f5e897, 0x41aedc3e, 0x4267f1f3, 0x4321228c, 0x43da66de, 0x4493b7bd,
		0x454d0dfe, 0x46066273, 0x46bfadf1, 0x4778e94c, 0x48320d58, 0x48eb12eb, 0x49a3f2dc, 0x4a5ca603,
		0x4b15253a, 0x4bcd695d, 0x4c856b4a, 0x4d3d23e2, 0x4df48c09, 0x4eab9ca6, 0x4f624ea2, 0x50189aeb,
		0x50ce7a74, 0x5183e632, 0x5238d720, 0x52ed463c, 0x53a12c8c, 0x54548318, 0x550742f0, 0x55b96529,
		0x566ae2dd, 0x571bb52f, 0x57cbd545, 0x587b3c50, 0x5929e385, 0x59d7c421, 0x5a84d76a, 0x5b3116ad,
		0x5bdc7b40, 0x5c86fe80, 0x5d3099d3, 0x5dd946a9, 0x5e80fe7c, 0x5f27bacc, 0x5fcd7527, 0x60722722,
		0x6115ca5d, 0x61b85883, 0x6259cb49, 0x62fa1c6f, 0x639945c2, 0x64374118, 0x64d40854, 0x656f9565,
		0x6609e244, 0x66a2e8fa, 0x673aa39b, 0x67d10c46, 0x68661d2a, 0x68f9d081, 0x698c2095, 0x6a1d07bb,
		0x6aac8058, 0x6b3a84de, 0x6bc70fce, 0x6c521bb8, 0x6cdba33a, 0x6d63a101, 0x6dea0fca, 0x6e6eea60,
		0x6ef22ba1, 0x6f73ce76, 0x6ff3cddc, 0x707224de, 0x70eece98, 0x7169c639, 0x71e306fc, 0x725a8c30,
		0x72d05136, 0x7344517f, 0x73b6888c, 0x7426f1f4, 0x7495895b, 0x75024a79, 0x756d311a, 0x75d6391b,
		0x763d5e6a, 0x76a29d09, 0x7705f10e, 0x776756a0, 0x77c6c9fa, 0x7824476b, 0x787fcb54, 0x78d9522a,
		0x7930d877, 0x79865ad7, 0x79d9d5fb, 0x7a2b46a8, 0x7a7aa9b6, 0x7ac7fc14, 0x7b133ac3, 0x7b5c62d9,
		0x7ba37183, 0x7be863ff, 0x7c2b37a3, 0x7c6be9d8, 0x7caa781e, 0x7ce6e008, 0x7d211f41, 0x7d593386,
		0x7d8f1aad, 0x7dc2d29f, 0x7df4595c, 0x7e23acf9, 0x7e50cba2, 0x7e7bb396, 0x7ea4632e, 0x7ecad8d6,
		0x7eef1310, 0x7f111077, 0x7f30cfb8, 0x7f4e4f9a, 0x7f698ef9, 0x7f828cc6, 0x7f994809, 0x7fadbfe2,
		0x7fbff386, 0x7fcfe240, 0x7fdd8b72, 0x7fe8ee96, 0x7ff20b3a, 0x7ff8e104, 0x7ffd6fb0, 0x7fffb712,
		0x7fffb712, 0x7ffd6fb0, 0x7ff8e104, 0x7ff20b3a, 0x7fe8ee96, 0x7fdd8b72, 0x7fcfe240, 0x7fbff386,
		0x7fadbfe2, 0x7f994809, 0x7f828cc6, 0x7f698ef9, 0x7f4e4f9a, 0x7f30cfb8, 0x7f111077, 0x7eef1310,
		0x7ecad8d6, 0x7ea4632e, 0x7e7bb396, 0x7e50cba2, 0x7e23acf9, 0x7df4595c, 0x7dc2d29f, 0x7d8f1aad,
		0x7d593386, 0x7d211f41, 0x7ce6e008, 0x7caa781e, 0x7c6be9d8, 0x7c2b37a3, 0x7be863ff, 0x7ba37183,
		0x7b5c62d9, 0x7b133ac3, 0x7ac7fc14, 0x7a7aa9b6, 0x7a2b46a8, 0x79d9d5fb, 0x79865ad7, 0x7930d877,
		0x78d9522a, 0x787fcb54, 0x7824476b, 0x77c6c9fa, 0x776756a0, 0x7705f10e, 0x76a29d09, 0x763d5e6a,
		0x75d6391b, 0x756d311a, 0x75024a79, 0x7495895b, 0x7426f1f4, 0x73b6888c, 0x7344517f, 0x72d05136,
		0x725a8c30, 0x71e306fc, 0x7169c639, 0x70eece98, 0x707224de, 0x6ff3cddc, 0x6f73ce76, 0x6ef22ba1,
		0x6e6eea60, 0x6dea0fca, 0x6d63a101, 0x6cdba33a, 0x6c521bb8, 0x6bc70fce, 0x6b3a84de, 0x6aac8058,
		0x6a1d07bb, 0x698c2095, 0x68f9d081, 0x68661d2a, 0x67d10c46, 0x673aa39b, 0x66a2e8fa, 0x6609e244,
		0x656f9565, 0x64d40854, 0x64374118, 0x639945c2, 0x62fa1c6f, 0x6259cb49, 0x61b85883, 0x6115ca5d,
		0x60722722, 0x5fcd7527, 0x5f27bacc, 0x5e80fe7c, 0x5dd946a9, 0x5d3099d3, 0x5c86fe80, 0x5bdc7b40,
		0x5b3116ad, 0x5a84d76a, 0x59d7c421, 0x5929e385, 0x587b3c50, 0x57cbd545, 0x571bb52f, 0x566ae2dd,
		0x55b96529, 0x550742f0, 0x54548318, 0x53a12c8c, 0x52ed463c, 0x5238d720, 0x5183e632, 0x50ce7a74,
		0x50189aeb, 0x4f624ea2, 0x4eab9ca6, 0x4df48c09, 0x4d3d23e2, 0x4c856b4a, 0x4bcd695d, 0x4b15253a,
		0x4a5ca603, 0x49a3f2dc, 0x48eb12eb, 0x48320d58, 0x4778e94c, 0x46bfadf1, 0x46066273, 0x454d0dfe,
		0x4493b7bd, 0x43da66de, 0x4321228c, 0x4267f1f3, 0x41aedc3e, 0x40f5e897, 0x403d1e26, 0x3f848413,
		0x3ecc2181, 0x3e13fd95, 0x3d5c1f6f, 0x3ca48e2c, 0x3bed50e7, 0x3b366eb9, 0x3a7feeb4, 0x39c9d7e9,
		0x39143165, 0x385f022f, 0x37aa514a, 0x36f625b5, 0x36428668, 0x358f7a58, 0x34dd0873, 0x342b37a0,
		0x337a0ec2, 0x32c994b3, 0x3219d049, 0x316ac850, 0x30bc8390, 0x300f08c6, 0x2f625eaa, 0x2eb68beb,
		0x2e0b972e, 0x2d618712, 0x2cb8622b, 0x2c102f07, 0x2b68f426, 0x2ac2b803, 0x2a1d810b, 0x297955a5,
		0x28d63c2b, 0x28343aec, 0x2793582e, 0x26f39a2c, 0x26550713, 0x25b7a507, 0x251b7a1f, 0x24808c67,
		0x23e6e1de, 0x234e8076, 0x22b76e15, 0x2221b094, 0x218d4dbf, 0x20fa4b53, 0x2068af02, 0x1fd87e6e,
		0x1f49bf2c, 0x1ebc76c2, 0x1e30aaa7, 0x1da66046, 0x1d1d9cf9, 0x1c966609, 0x1c10c0b4, 0x1b8cb225,
		0x1b0a3f79, 0x1a896dbb, 0x1a0a41ea, 0x198cc0f0, 0x1910efa9, 0x1896d2e1, 0x181e6f50, 0x17a7c99f,
		0x1732e667, 0x16bfca2e, 0x164e7967, 0x15def876, 0x15714bab, 0x15057746, 0x149b7f72, 0x14336849,
		0x13cd35d3, 0x1368ec05, 0x13068ec0, 0x12a621d2, 0x1247a8f7, 0x11eb27d8, 0x1190a208, 0x11381b09,
		0x10e19648, 0x108d171d, 0x103aa0cf, 0x0fea368d, 0x0f9bdb76, 0x0f4f9291, 0x0f055ed3, 0x0ebd4319,
		0x0e774230, 0x0e335ecc, 0x0df19b8e, 0x0db1fb02, 0x0d747f9e, 0x0d392bc3, 0x0d0001bd, 0x0cc903c3,
		0x0c9433f5, 0x0c61945e, 0x0c3126f5, 0x0c02ed99, 0x0bd6ea14, 0x0bad1e1a, 0x0b858b4a, 0x0b60332a,
		0x0b3d172e, 0x0b1c38b2, 0x0afd98fa, 0x0ae13936, 0x0ac71a80, 0x0aaf3ddb, 0x0a99a432, 0x0a864e5b,
		0x0a753d17, 0x0a66710f, 0x0a59ead4, 0x0a4faae4, 0x0a47b1a3, 0x0a41ff60, 0x0a3e9455, 0x0a3d70a3
};


static const float32_t float_hamming_512[] = {
		0.08      , 0.08003477, 0.08013909, 0.08031292, 0.08055626,
		0.08086906, 0.08125127, 0.08170284, 0.0822237 , 0.08281376,
		0.08347295, 0.08420116, 0.08499828, 0.08586418, 0.08679875,
		0.08780184, 0.08887329, 0.09001294, 0.09122063, 0.09249617,
		0.09383936, 0.09525001, 0.09672789, 0.0982728 , 0.09988448,
		0.10156271, 0.10330722, 0.10511775, 0.10699403, 0.10893578,
		0.1109427 , 0.11301448, 0.11515082, 0.11735139, 0.11961586,
		0.12194389, 0.12433512, 0.12678919, 0.12930573, 0.13188437,
		0.13452471, 0.13722635, 0.13998888, 0.14281189, 0.14569495,
		0.14863762, 0.15163946, 0.15470002, 0.15781883, 0.16099542,
		0.16422931, 0.16752001, 0.17086702, 0.17426984, 0.17772796,
		0.18124085, 0.18480797, 0.1884288 , 0.19210278, 0.19582935,
		0.19960796, 0.20343803, 0.20731899, 0.21125024, 0.2152312 ,
		0.21926125, 0.2233398 , 0.22746622, 0.2316399 , 0.23586019,
		0.24012646, 0.24443807, 0.24879437, 0.25319469, 0.25763837,
		0.26212475, 0.26665313, 0.27122284, 0.27583319, 0.28048347,
		0.28517299, 0.28990103, 0.29466689, 0.29946984, 0.30430915,
		0.3091841 , 0.31409394, 0.31903793, 0.32401534, 0.32902539,
		0.33406735, 0.33914043, 0.34424389, 0.34937694, 0.35453881,
		0.35972872, 0.36494588, 0.37018951, 0.37545881, 0.38075299,
		0.38607124, 0.39141277, 0.39677676, 0.4021624 , 0.40756889,
		0.41299539, 0.4184411 , 0.42390518, 0.42938682, 0.43488518,
		0.44039943, 0.44592874, 0.45147227, 0.45702919, 0.46259865,
		0.46817981, 0.47377183, 0.47937386, 0.48498506, 0.49060458,
		0.49623157, 0.50186517, 0.50750453, 0.51314881, 0.51879715,
		0.5244487 , 0.5301026 , 0.53575799, 0.54141402, 0.54706984,
		0.55272459, 0.55837742, 0.56402747, 0.56967389, 0.57531582,
		0.58095241, 0.58658281, 0.59220616, 0.59782163, 0.60342835,
		0.60902548, 0.61461218, 0.6201876 , 0.62575089, 0.63130122,
		0.63683774, 0.64235963, 0.64786604, 0.65335614, 0.6588291 ,
		0.6642841 , 0.66972031, 0.67513691, 0.68053307, 0.68590799,
		0.69126085, 0.69659084, 0.70189716, 0.707179  , 0.71243557,
		0.71766606, 0.72286969, 0.72804568, 0.73319324, 0.73831159,
		0.74339995, 0.74845757, 0.75348367, 0.75847749, 0.76343829,
		0.7683653 , 0.77325778, 0.77811501, 0.78293623, 0.78772072,
		0.79246776, 0.79717663, 0.80184662, 0.80647702, 0.81106714,
		0.81561627, 0.82012373, 0.82458885, 0.82901093, 0.83338932,
		0.83772336, 0.84201238, 0.84625575, 0.85045281, 0.85460293,
		0.8587055 , 0.86275987, 0.86676546, 0.87072163, 0.87462781,
		0.8784834 , 0.88228781, 0.88604048, 0.88974082, 0.89338829,
		0.89698234, 0.90052241, 0.90400798, 0.90743851, 0.91081349,
		0.91413241, 0.91739477, 0.92060007, 0.92374783, 0.92683757,
		0.92986882, 0.93284114, 0.93575406, 0.93860715, 0.94139997,
		0.94413211, 0.94680315, 0.94941269, 0.95196032, 0.95444568,
		0.95686838, 0.95922805, 0.96152434, 0.9637569 , 0.9659254 ,
		0.9680295 , 0.97006889, 0.97204326, 0.97395231, 0.97579575,
		0.97757331, 0.97928471, 0.98092969, 0.98250802, 0.98401944,
		0.98546374, 0.98684068, 0.98815007, 0.98939171, 0.9905654 ,
		0.99167097, 0.99270826, 0.99367711, 0.99457736, 0.99540889,
		0.99617157, 0.99686528, 0.99748992, 0.99804539, 0.99853161,
		0.99894851, 0.99929602, 0.99957409, 0.99978268, 0.99992176,
		0.99999131, 0.99999131, 0.99992176, 0.99978268, 0.99957409,
		0.99929602, 0.99894851, 0.99853161, 0.99804539, 0.99748992,
		0.99686528, 0.99617157, 0.99540889, 0.99457736, 0.99367711,
		0.99270826, 0.99167097, 0.9905654 , 0.98939171, 0.98815007,
		0.98684068, 0.98546374, 0.98401944, 0.98250802, 0.98092969,
		0.97928471, 0.97757331, 0.97579575, 0.97395231, 0.97204326,
		0.97006889, 0.9680295 , 0.9659254 , 0.9637569 , 0.96152434,
		0.95922805, 0.95686838, 0.95444568, 0.95196032, 0.94941269,
		0.94680315, 0.94413211, 0.94139997, 0.93860715, 0.93575406,
		0.93284114, 0.92986882, 0.92683757, 0.92374783, 0.92060007,
		0.91739477, 0.91413241, 0.91081349, 0.90743851, 0.90400798,
		0.90052241, 0.89698234, 0.89338829, 0.88974082, 0.88604048,
		0.88228781, 0.8784834 , 0.87462781, 0.87072163, 0.86676546,
		0.86275987, 0.8587055 , 0.85460293, 0.85045281, 0.84625575,
		0.84201238, 0.83772336, 0.83338932, 0.82901093, 0.82458885,
		0.82012373, 0.81561627, 0.81106714, 0.80647702, 0.80184662,
		0.79717663, 0.79246776, 0.78772072, 0.78293623, 0.77811501,
		0.77325778, 0.7683653 , 0.76343829, 0.75847749, 0.75348367,
		0.74845757, 0.74339995, 0.73831159, 0.73319324, 0.72804568,
		0.72286969, 0.71766606, 0.71243557, 0.707179  , 0.70189716,
		0.69659084, 0.69126085, 0.68590799, 0.68053307, 0.67513691,
		0.66972031, 0.6642841 , 0.6588291 , 0.65335614, 0.64786604,
		0.64235963, 0.63683774, 0.63130122, 0.62575089, 0.6201876 ,
		0.61461218, 0.60902548, 0.60342835, 0.59782163, 0.59220616,
		0.58658281, 0.58095241, 0.57531582, 0.56967389, 0.56402747,
		0.55837742, 0.55272459, 0.54706984, 0.54141402, 0.53575799,
		0.5301026 , 0.5244487 , 0.51879715, 0.51314881, 0.50750453,
		0.50186517, 0.49623157, 0.49060458, 0.48498506, 0.47937386,
		0.47377183, 0.46817981, 0.46259865, 0.45702919, 0.45147227,
		0.44592874, 0.44039943, 0.43488518, 0.42938682, 0.42390518,
		0.4184411 , 0.41299539, 0.40756889, 0.4021624 , 0.39677676,
		0.39141277, 0.38607124, 0.38075299, 0.37545881, 0.37018951,
		0.36494588, 0.35972872, 0.35453881, 0.34937694, 0.34424389,
		0.33914043, 0.33406735, 0.32902539, 0.32401534, 0.31903793,
		0.31409394, 0.3091841 , 0.30430915, 0.29946984, 0.29466689,
		0.28990103, 0.28517299, 0.28048347, 0.27583319, 0.27122284,
		0.26665313, 0.26212475, 0.25763837, 0.25319469, 0.24879437,
		0.24443807, 0.24012646, 0.23586019, 0.2316399 , 0.22746622,
		0.2233398 , 0.21926125, 0.2152312 , 0.21125024, 0.20731899,
		0.20343803, 0.19960796, 0.19582935, 0.19210278, 0.1884288 ,
		0.18480797, 0.18124085, 0.17772796, 0.17426984, 0.17086702,
		0.16752001, 0.16422931, 0.16099542, 0.15781883, 0.15470002,
		0.15163946, 0.14863762, 0.14569495, 0.14281189, 0.13998888,
		0.13722635, 0.13452471, 0.13188437, 0.12930573, 0.12678919,
		0.12433512, 0.12194389, 0.11961586, 0.11735139, 0.11515082,
		0.11301448, 0.1109427 , 0.10893578, 0.10699403, 0.10511775,
		0.10330722, 0.10156271, 0.09988448, 0.0982728 , 0.09672789,
		0.09525001, 0.09383936, 0.09249617, 0.09122063, 0.09001294,
		0.08887329, 0.08780184, 0.08679875, 0.08586418, 0.08499828,
		0.08420116, 0.08347295, 0.08281376, 0.0822237 , 0.08170284,
		0.08125127, 0.08086906, 0.08055626, 0.08031292, 0.08013909,
		0.08003477, 0.08
};




static const q31_t q31_biquad[] =  { 0x3c8ac306, 0x86ea79fe, 0x3c8ac306, 0x78e5ac16, 0xc6baa00f};
static const float32_t float_biquad[] = {0.94596935, -1.89193869, 0.94596935, 1.88901808, -0.89485930};





#endif