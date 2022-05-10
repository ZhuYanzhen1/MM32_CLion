//
// Created by 16625 on 2022-04-10.
//

#ifndef MM32F3277_MATH_TEST_H_
#define MM32F3277_MATH_TEST_H_

#define INDEX_NUM   88  // main 最上方
#define FIRST_N     4385.7630000f
#define FIRST_E     39692.2030000f

float test_point[][4] = {
    {4363.8690000f, 39637.6450000f, -2.6728173f, 0.0092530f},
    {4363.7764000f, 39637.5981000f, -2.7345837f, 0.0092530f},
    {4363.6049600f, 39637.5241900f, -2.8679114f, 0.0009687f},
    {4363.3555640f, 39637.4541710f, -2.7569676f, 0.0109153f},
    {4363.0445076f, 39637.3282539f, -2.8143109f, 0.0001605f},
    {4362.6706568f, 39637.2013285f, -2.8800204f, 0.0016648f},
    {4362.2394912f, 39637.0858957f, -2.8316272f, 0.0050022f},
    {4361.7590420f, 39636.9320061f, -2.8437462f, 0.0009460f},
    {4361.2313378f, 39636.7700055f, -2.8856154f, 0.0013993f},
    {4360.6609041f, 39636.6207049f, -2.9239203f, 0.0015887f},
    {4360.0534136f, 39636.4863344f, -2.8638507f, 0.0047942f},
    {4359.4142723f, 39636.3041010f, -2.8863383f, 0.0004965f},
    {4358.7363451f, 39636.1271909f, -2.9153342f, 0.0015210f},
    {4358.0395105f, 39635.9667718f, -2.9459415f, 0.0014456f},
    {4357.3110595f, 39635.8223946f, -2.9716987f, 0.0012713f},
    {4356.5536535f, 39635.6924552f, -2.9910254f, 0.0011361f},
    {4355.7828882f, 39635.5755096f, -3.0079318f, 0.0010151f},
    {4355.0001994f, 39635.4702587f, -3.0247187f, 0.0009304f},
    {4354.1968794f, 39635.3759328f, -3.0395654f, 0.0007826f},
    {4353.3677915f, 39635.2910395f, -3.0506028f, 0.0007000f},
    {4352.5305123f, 39635.2146356f, -3.0615501f, 0.0006534f},
    {4351.6783611f, 39635.1462720f, -3.0697184f, 0.0005553f},
    {4350.8239250f, 39635.0847448f, -3.0787828f, 0.0005572f},
    {4349.9563325f, 39635.0301703f, -3.0857852f, 0.0004323f},
    {4349.0772992f, 39634.9810533f, -3.0919824f, 0.0003847f},
    {4348.1871693f, 39634.9368480f, -3.0969369f, 0.0003465f},
    {4347.2970524f, 39634.8970632f, -3.1021582f, 0.0003761f},
    {4346.4100472f, 39634.8620569f, -3.1063138f, 0.0002740f},
    {4345.5176424f, 39634.8305512f, -3.1111606f, 0.0003051f},
    {4344.6127782f, 39634.8029961f, -3.1140301f, 0.0002142f},
    {4343.7136004f, 39634.7781965f, -3.1168381f, 0.0001925f},
    {4342.8125403f, 39634.7558768f, -3.1199827f, 0.0002022f},
    {4341.9020863f, 39634.7361891f, -3.1224074f, 0.0001749f},
    {4340.9947777f, 39634.7187702f, -3.1248937f, 0.0001639f},
    {4340.0805999f, 39634.7034932f, -3.1272603f, 0.0001449f},
    {4339.1499399f, 39634.6901439f, -3.1286392f, 0.0001008f},
    {4338.2232459f, 39634.6781295f, -3.1299735f, 0.0000904f},
    {4337.2935213f, 39634.6673165f, -3.1321002f, 0.0001401f},
    {4336.3536692f, 39634.6583849f, -3.1329411f, 0.0000674f},
    {4335.4257023f, 39634.6503464f, -3.1338257f, 0.0000605f},
    {4334.4955321f, 39634.6431118f, -3.1346310f, 0.0000542f},
    {4333.5616788f, 39634.6366006f, -3.1353277f, 0.0000488f},
    {4332.6279110f, 39634.6307405f, -3.1368527f, 0.0001030f},
    {4331.6861199f, 39634.6262665f, -3.1381594f, 0.0000932f},
    {4330.7492079f, 39634.6230398f, -3.1385361f, 0.0000239f},
    {4329.8023871f, 39634.6201358f, -3.1388699f, 0.0000213f},
    {4328.8461484f, 39634.6175223f, -3.1395454f, 0.0000488f},
    {4327.8974335f, 39634.6155700f, -3.1397666f, 0.0000143f},
    {4326.9406902f, 39634.6138130f, -3.1403818f, 0.0000417f},
    {4325.9733212f, 39634.6126317f, -3.1405105f, 0.0000085f},
    {4325.0000891f, 39634.6115686f, -3.1406204f, 0.0000076f},
    {4324.0261801f, 39634.6106117f, -3.1407127f, 0.0000069f},
    {4323.0588621f, 39634.6097505f, -3.1415256f, 0.0000568f},
    {4322.0893759f, 39634.6096755f, -3.1415329f, 0.0000005f},
    {4321.1266383f, 39634.6096079f, 3.1408116f, 0.0000589f},
    {4320.1673745f, 39634.6103471f, 3.1408836f, 0.0000054f},
    {4319.2150370f, 39634.6110124f, 3.1401107f, 0.0000540f},
    {4318.2644333f, 39634.6124112f, 3.1402679f, 0.0000102f},
    {4317.3065900f, 39634.6136701f, 3.1403966f, 0.0000092f},
    {4316.3509310f, 39634.6148031f, 3.1405084f, 0.0000084f},
    {4315.4012379f, 39634.6158227f, 3.1406281f, 0.0000074f},
    {4314.4394141f, 39634.6167405f, 3.1394487f, 0.0000817f},
    {4313.4897727f, 39634.6187664f, 3.1396627f, 0.0000149f},
    {4312.5397954f, 39634.6205898f, 3.1398808f, 0.0000132f},
    {4311.5753159f, 39634.6222308f, 3.1391616f, 0.0000473f},
    {4310.6345843f, 39634.6245077f, 3.1394419f, 0.0000166f},
    {4309.6771259f, 39634.6265570f, 3.1393385f, 0.0000070f},
    {4308.7214133f, 39634.6287013f, 3.1391258f, 0.0000137f},
    {4307.7728720f, 39634.6310311f, 3.1389632f, 0.0000123f},
    {4306.8194848f, 39634.6335280f, 3.1392344f, 0.0000183f},
    {4305.8623363f, 39634.6357752f, 3.1394598f, 0.0000165f},
    {4304.9094027f, 39634.6377977f, 3.1396939f, 0.0000147f},
    {4303.9454624f, 39634.6396179f, 3.1394700f, 0.0000158f},
    {4302.9805162f, 39634.6416561f, 3.1396713f, 0.0000149f},
    {4302.0205645f, 39634.6434905f, 3.1398599f, 0.0000134f},
    {4301.0620081f, 39634.6451415f, 3.1400403f, 0.0000120f},
    {4300.0984073f, 39634.6466273f, 3.1393854f, 0.0000469f},
    {4299.1255665f, 39634.6487646f, 3.1395789f, 0.0000156f},
    {4298.1654099f, 39634.6506881f, 3.1381511f, 0.0001015f},
    {4297.1945689f, 39634.6540193f, 3.1384636f, 0.0000243f},
    {4296.2332120f, 39634.6570174f, 3.1383734f, 0.0000073f},
    {4295.2676908f, 39634.6601156f, 3.1387061f, 0.0000224f},
    {4294.2982217f, 39634.6629041f, 3.1389635f, 0.0000204f},
    {4293.3398996f, 39634.6654137f, 3.1392574f, 0.0000181f},
    {4292.3684096f, 39634.6676723f, 3.1391536f, 0.0000054f},
    {4291.4078686f, 39634.6700051f, 3.1389466f, 0.0000123f},
    {4290.4594818f, 39634.6725046f, 3.1387968f, 0.0000110f},
    {4289.5082336f, 39634.6751541f, 3.1386153f, 0.0000101f},
    {4288.5696102f, 39634.6779387f, 3.1386153f, 0.0000090f},
};

#endif // MM32F3277_MATH_TEST_H_
