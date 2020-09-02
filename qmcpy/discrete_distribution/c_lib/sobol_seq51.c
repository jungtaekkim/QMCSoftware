/*
Copyright Â© 2020 BRODA. Ltd All Rights Reserved.

I.M. SOBOL', V.I. TURCHANINOV, Yu.L. LEVITAN, B.V. SHUKHMAN
KELDYSH INSTITUTE OF APPLIED MATHEMATICS
RUSSIAN ACADEMY OF SCIENCES

QUASIRANDOM SEQUENCE GENERATORS 28.11.1991

------------------------------------------------------------------------

NOTE TO THE USER BY the NEA Data Bank:

    This quasi random number generator has been made available to
    you on condition that its identity is preserved when used
    in computer programs. If its use leads to scientific publication
    of results you should cite it in the references, in addition
    no commercial use should be made unless agreed upon with the
    main author (Prof. I.M. Sobol')

ABSTRACT

    POINTS BELONGING TO LP-TAU SEQUENCES UNIFORMLY DISTRIBUTED IN THE
    N-DIMENSIONAL UNIT CUBE ARE OFTEN USED IN NUMERICAL MATHEMATICS:
        - AS NODES FOR MULTIDIMENSIONAL INTEGRATION;
        - AS SEARCHING POINTS IN GLOBAL OPTIMIZATION;
        - AS TRIAL POINTS IN MULTI-CRITERIA DECISION MAKING;
        - AS QUASIRANDOM POINTS FOR QUASI-MONTECARLO ALGORITHMS;
        - ETC.

    THIS SUBROUTINE CONTAINS THE ALGORITHM FOR FAST GENERATION OF
    LP-TAU SEQUENCES THAT ARE SUITABLE FOR MULTI-PROCESSOR COMPUTATIONS.
    THE DIMENSIONS N.LE.51, THE NUMBER OF POINTS N.LT.2**30.
    THE PROGRAMMING LANGUAGE IS FORTRAN-77. THIS SUBROUTINE IS AVAILABLE
    ALSO IN THE MATLAB -LANGUAGE.
    
    THE REPORT DESCRIBING THE ALGORITHM CONTAINS THE DESCRIPTION OF THE
    ALGORITHM AND CERTAIN IMPORTANT PROPERTIES OF LP-TAU SEQUENCES AND
    THEIR GENERALIZATIONS ARE DISCUSSED.

REFERENCE:

    [1] I.M. SOBOL', V.I. TURCHANINOV, Yu.L. LEVITAN, B.V. SHUKHMAN
    KELDYSH INSTITUTE OF APPLIED MATHEMATICS
    RUSSIAN ACADEMY OF SCIENCES
    QUASIRANDOM SEQUENCE GENERATORS
    MOSCOW 1992, IPM ZAK. NO.30 (100 COPIES)

    [2] I. Sobolâ, D. Asotsky, A. Kreinin, S. Kucherenko. Construction
    and Comparison of High-Dimensional Sobolâ Generators, 2011, Wilmott 
    Journal, Nov, pp. 64-79 http://www.broda.co.uk/doc/HD_SobolGenerator.pdf
*/

#include <string.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXDIM 51
#define QP 30
#define SCALE 9.31322574615478516E-10

/* Direction numbers */
static int DIRECT[MAXDIM][QP] = {
    {536870912,	805306368,	939524096,	1006632960,	1040187392,	1056964608,	1065353216,	1069547520,	1071644672,	1072693248,	1073217536,	1073479680,	1073610752,	1073676288,	1073709056,	1073725440,	1073733632,	1073737728,	1073739776,	1073740800,	1073741312,	1073741568,	1073741696,	1073741760,	1073741792,	1073741808,	1073741816,	1073741820,	1073741822,	1073741823},	
    {536870912,	268435456,	939524096,	67108864,	637534208,	352321536,	1065353216,	4194304,	543162368,	273678336,	947388416,	71565312,	644218880,	357892096,	1073709056,	16384,	    536895488,	268455936,	939554816,	67126272,	637560320,	352343296,	1065385856,	4210752,	543187040,	273698896,	947419256,	71582788,	644245094,	357913941},	
    {536870912,	805306368,	134217728,	603979776,	1040187392,	50331648,	578813952,	826277888,	190840832,	644874240,	1070071808,	2359296,	538836992,	808648704,	137003008,	605372416,	1043685376,	52785152,	580732928,	829514752,	189496832,	644236032,	1073734272,	12608,	    536882016,	805312112,	134232008,	603989028,	1040195102,	50344755},	
    {536870912,	268435456,	671088640,	1006632960,	838860800,	419430400,	25165824,	624951296,	329252864,	753926144,	977797120,	891551744,	455475200,	2424832,	547782656,	275234816,	679944192,	1020628992,	854333440,	423777280,	27474432,	636735232,	331949184,	744788544,	981766752,	903123536,	460100200,	37500,	    537039474,	268538457},	
    {536870912,	805306368,	402653184,	335544320,	167772160,	956301312,	125829120,	616562688,	853540864,	495976448,	365428736,	158597120,	1062600704,	2555904,	545095680,	817971200,	417505280,	345018368,	172619776,	968297472,	129338880,	615860480,	842310784,	494377792,	357858144,	153504624,	1073685336,	120660,	    537089354,	805540473},	
    {536870912,	268435456,	402653184,	469762048,	234881024,	889192448,	964689920,	801112064,	132120576,	869269504,	702021632,	383516672,	139329536,	624230400,	26836992,	603963392,	301981696,	452972544,	511694848,	205511680,	908054016,	959444224,	799537536,	115601344,	810665952,	689632208,	398354904,	182195932,	642656334,	3487029},	
    {536870912,	805306368,	671088640,	872415232,	167772160,	620756992,	327155712,	952107008,	778043392,	355467264,	461897728,	1019478016,	205651968,	69271552,	34701312,	555335680,	832561152,	704442368,	936142848,	147076096,	641870336,	287380736,	965852032,	769248448,	369083488,	432848944,	1069834248,	213921796,	73402402,	3146769},	
    {536870912,	268435456,	134217728,	738197504,	1040187392,	117440512,	310378496,	406847488,	73400320,	57671680,	829947904,	947126272,	905052160,	456851456,	354385920,	721534976,	210542592,	255987712,	518199296,	926669824,	186375680,	1007410432,	369351808,	990895808,	613015520,	859888752,	686054696,	473570692,	664239174,	592038967},	
    {536870912,	268435456,	671088640,	469762048,	436207616,	956301312,	360710144,	398458880,	178257920,	816840704,	425197568,	621019136,	797048832,	1052966912,	925663232,	997801984,	167501824,	750759936,	593348608,	202300416,	302677504,	890187008,	662886016,	851706304,	86115232,	510853776,	108299224,	41763004,	1014620426,	729591963},	
    {536870912,	805306368,	402653184,	872415232,	33554432,	922746880,	360710144,	331350016,	522190848,	961544192,	634912768,	714866688,	452329472,	600637440,	656965632,	1028767744,	1071341568,	697692160,	225527808,	118395904,	763663872,	399480576,	86119808,	979326784,	845149216,	240805744,	273898840,	156352828,	500917234,	781578389},	
    {536870912,	268435456,	939524096,	1006632960,	570425344,	822083584,	159383552,	356515840,	639631360,	804257792,	437780480,	738459648,	973471744,	487260160,	327581696,	407060480,	229302272,	196677632,	900982784,	381332480,	103878144,	520092928,	865109888,	692061120,	606076960,	250610192,	731382552,	88343188,	1042677826,	66781679},	
    {536870912,	805306368,	134217728,	67108864,	1040187392,	218103808,	444596224,	46137344,	534773760,	495976448,	582483968,	265027584,	627703808,	794624000,	894795776,	375275520,	970661888,	764604416,	180279296,	1002738688,	325187072,	643311360,	299633792,	429015104,	1018241504,	852489168,	938170664,	698012780,	347788318,	113956361},	
    {536870912,	268435456,	671088640,	872415232,	503316480,	587202560,	947912704,	1010827264,	991952896,	347078656,	792199168,	468975616,	614072320,	386727936,	110067712,	256688128,	732176384,	485625856,	168904704,	743042048,	858254848,	816901376,	422735488,	217132864,	35245536,	139460144,	86812552,	666108868,	1069154738,	331153483},	
    {536870912,	805306368,	939524096,	469762048,	838860800,	385875968,	662700032,	1069547520,	333447168,	426770432,	315097088,	131858432,	803078144,	467599360,	1038057472,	1021165568,	576659456,	522022912,	62814208,	292551680,	922041856,	695310080,	181087360,	736067008,	635403744,	283082224,	677346808,	337608156,	373259762,	967802327},	
    {536870912,	268435456,	134217728,	469762048,	973078528,	452984832,	444596224,	734003200,	48234496,	1066401792,	611844096,	504627200,	637403136,	798031872,	209354752,	303349760,	402464768,	680620032,	754485248,	584567808,	261924352,	1019229440,	174252416,	55694400,	236633696,	222702928,	602208712,	1054329884,	362587674,	935343371},	
    {536870912,	805306368,	671088640,	469762048,	167772160,	218103808,	528482304,	113246208,	1059061760,	372244480,	667418624,	581173248,	835059712,	630652928,	596541440,	1022476288,	451584000,	362270720,	730691584,	284339200,	954978816,	77034240,	1041212544,	456152640,	407451232,	342181488,	652635752,	799472220,	239395914,	324351309},	
    {536870912,	805306368,	134217728,	1006632960,	234881024,	184549376,	578813952,	490733568,	761266176,	84934656,	166199296,	266600448,	949878784,	340983808,	42631168,	234143744,	368467968,	838103040,	1005996032,	183936000,	292822528,	733295872,	840196224,	631601984,	427671904,	922872432,	209797064,	641885244,	132283950,	609305915},	
    {536870912,	268435456,	939524096,	335544320,	436207616,	285212672,	578813952,	633339904,	673185792,	208666624,	513277952,	865861632,	662044672,	493944832,	471433216,	106708992,	928260096,	83972096,	411174912,	616762368,	849512960,	1035127552,	750314368,	787264192,	460141792,	187528656,	323968376,	392104980,	903121466,	818137857},	
    {536870912,	805306368,	939524096,	872415232,	771751936,	150994944,	75497472,	910163968,	220200960,	313524224,	187170816,	658243584,	767950848,	37945344,	52527104,	732971008,	933847040,	629133312,	249866240,	435221504,	210311680,	986582784,	391053440,	898599104,	116344544,	360637424,	384078968,	502284340,	445210638,	131059769},	
    {536870912,	268435456,	402653184,	603979776,	100663296,	956301312,	1065353216,	801112064,	396361728,	598736896,	1036517376,	546045952,	426901504,	264175616,	666861568,	733954048,	29319168,	46108672,	641669120,	159376384,	933232128,	332381952,	903306880,	478383808,	990246688,	814514736,	561178056,	1002405628,	968851198,	918519549},	
    {536870912,	805306368,	402653184,	201326592,	234881024,	352321536,	92274688,	893386752,	362807296,	487587840,	25690112,	521404416,	448659456,	263520256,	706707456,	789889024,	582934528,	867561472,	1008719872,	907322368,	691981824,	334852352,	742365312,	507774272,	1024892576,	301037840,	923399256,	249907140,	230153254,	827341719},	
    {536870912,	268435456,	134217728,	67108864,	905969664,	654311424,	511705088,	851443712,	815792128,	965738496,	201850880,	304873472,	23986176,	833421312,	674070528,	879575040,	772612096,	725422080,	749066240,	595696640,	155417088,	364956416,	236995200,	458270272,	883429408,	800872624,	996597176,	75586228,	935549622,	922770101},	
    {536870912,	805306368,	671088640,	335544320,	771751936,	553648128,	897581056,	801112064,	819986432,	1011875840,	443023360,	1060896768,	1016201216,	235077632,	824410112,	764657664,	330014720,	184504320,	860993536,	247497728,	627070464,	596672256,	42492800,	392995136,	150655392,	272595184,	942777416,	206587660,	308815630,	456972047},	
    {536870912,	268435456,	402653184,	67108864,	33554432,	352321536,	847249408,	918552576,	346030080,	296747008,	990380032,	163840000,	524943360,	464846848,	305496064,	762658816,	647323648,	751112192,	94177280,	553735168,	416795136,	800381696,	946000512,	872482496,	173476896,	158627600,	885167400,	565275348,	861891286,	363883221},	
    {536870912,	268435456,	671088640,	201326592,	838860800,	788529152,	662700032,	742391808,	39845888,	393216000,	63438848,	305922048,	525991936,	534839296,	136282112,	1010843648,	174071808,	191918080,	432564224,	826051584,	182081024,	480316672,	771692416,	124824768,	197666464,	238839088,	89791048,	1021426548,	496995094,	1057014661},	
    {536870912,	805306368,	402653184,	738197504,	973078528,	956301312,	931135488,	499122176,	752877568,	523239424,	211288064,	257163264,	618790912,	992411648,	847740928,	941080576,	1008689152,	306425856,	226355200,	564454400,	513433088,	574863104,	897405824,	1034161344,	1016740448,	927181136,	956122504,	425987996,	670740382,	900956063},	
    {536870912,	268435456,	402653184,	1006632960,	905969664,	587202560,	411041792,	62914560,	387973120,	720371712,	983040000,	50069504,	781058048,	10813440,	531464192,	827932672,	295919616,	507310080,	925784064,	446024704,	848658944,	647347968,	613803136,	362141632,	605376608,	710248688,	87393464,	598058580,	657311830,	580478293},	
    {536870912,	805306368,	671088640,	738197504,	33554432,	251658240,	713031680,	264241152,	643825664,	823132160,	1069023232,	773062656,	761659392,	367067136,	222920704,	98942976,	353869824,	30453760,	995657728,	214236160,	515770880,	702910208,	924258688,	1053833280,	970487008,	788854608,	987661336,	401940420,	573565382,	520383687},	
    {536870912,	268435456,	134217728,	603979776,	301989888,	218103808,	696254464,	708837376,	291504128,	128974848,	421003264,	59506688,	458096640,	116457472,	379289600,	1051738112,	177774592,	281858048,	967731200,	33661952,	627273216,	499965184,	808338304,	943810496,	1006881248,	1048376560,	993412952,	919427316,	248331478,	315470533},	
    {536870912,	805306368,	134217728,	335544320,	838860800,	721420288,	528482304,	717225984,	274726912,	407896064,	742916096,	779354112,	226361344,	115015680,	507740160,	624934912,	580198400,	604000256,	436226048,	251685888,	629169664,	364968704,	1038125440,	428920128,	598265632,	484709072,	827749496,	822049324,	1064650798,	227601711},	
    {536870912,	268435456,	939524096,	738197504,	100663296,	251658240,	209715200,	759169024,	568328192,	747634688,	487063552,	165937152,	950665216,	928710656,	11894784,	993280000,	381837312,	208662528,	913799168,	660327424,	414566912,	120862976,	683814272,	795835200,	1022861728,	85709008,	903211272,	115655868,	338312798,	169598765},	
    {536870912,	805306368,	939524096,	67108864,	234881024,	1056964608,	578813952,	499122176,	526385152,	850395136,	363331584,	587988992,	952500224,	619773952,	1053392896,	134070272,	638574592,	319557632,	549894144,	284244992,	147450368,	1023256320,	177186176,	835462848,	489055392,	1065469744,	33649816,	759299588,	669059078,	909227271},	
    {536870912,	805306368,	939524096,	1006632960,	1040187392,	520093696,	1065353216,	272629760,	673185792,	873463808,	973602816,	486801408,	516030464,	813760512,	129990656,	201375744,	637558784,	318779392,	964696064,	859868160,	698353152,	995114240,	766008960,	961843520,	216680608,	429237328,	594875176,	562394644,	1065190902,	796983815},	
    {536870912,	268435456,	939524096,	67108864,	637534208,	889192448,	1065353216,	423624704,	98566144,	504365056,	286785536,	160694272,	337510400,	1043398656,	557547520,	567558144,	679370752,	476139520,	843315200,	722502656,	244844032,	13951232,	704549248,	606198080,	371439072,	490778384,	65052056,	990942164,	379125622,	349496709},	
    {536870912,	805306368,	134217728,	603979776,	1040187392,	587202560,	578813952,	155189248,	396361728,	481296384,	171442176,	90439680,	496107520,	967770112,	666468352,	882163712,	504012800,	863719424,	445863936,	622844928,	226578944,	32520448,	194555008,	785652672,	53140576,	848024144,	822835240,	1003212268,	111897166,	393974911},	
    {536870912,	268435456,	671088640,	1006632960,	838860800,	956301312,	25165824,	239075328,	727711744,	686817280,	669515776,	423362560,	830865408,	912850944,	1061912576,	649084928,	747118592,	567062528,	1047422976,	324342784,	1020087808,	702127360,	306383744,	154954432,	965114400,	443114288,	625704888,	598750292,	520650422,	378320037},	
    {536870912,	805306368,	402653184,	335544320,	167772160,	419430400,	125829120,	205520896,	1042284544,	592445440,	110624768,	536608768,	944111616,	72155136,	576684032,	356564992,	429973504,	521179136,	548947968,	682374144,	886978048,	45176064,	496592512,	491617344,	98534112,	687907504,	1065374584,	675334852,	740300390,	777004343},	
    {536870912,	268435456,	402653184,	469762048,	234881024,	352321536,	964689920,	943718400,	106954752,	995098624,	383254528,	614203392,	636354560,	1009057792,	1041334272,	489439232,	1032609792,	712790016,	492066816,	400579584,	389697024,	444945664,	416020864,	297351232,	209692256,	474573648,	612232920,	675293340,	1046483950,	927999269},	
    {536870912,	805306368,	671088640,	872415232,	167772160,	83886080,	327155712,	373293056,	299892736,	498073600,	289931264,	56885248,	978452480,	668532736,	380469248,	637091840,	932667392,	610299904,	953870336,	405353472,	1007004160,	237860096,	655186816,	580735552,	264598496,	45706416,	536299720,	445665316,	66254898,	616377385},	
    {536870912,	268435456,	134217728,	738197504,	1040187392,	654311424,	310378496,	457179136,	912261120,	969932800,	352845824,	999030784,	921567232,	462356480,	114196480,	60637184,	583360512,	293687296,	1002653696,	605187072,	839646720,	152839936,	1032204928,	634587968,	321437280,	681465296,	1046858504,	903023756,	992513134,	345930959},	
    {536870912,	268435456,	671088640,	469762048,	436207616,	419430400,	360710144,	406847488,	44040192,	586153984,	878182400,	10747904,	533594112,	267321344,	133070848,	199278592,	969498624,	1023086592,	861440000,	840682496,	621924864,	530633984,	694773632,	185277760,	545303840,	805303216,	939518840,	872412692,	234871606,	989849787},	
    {536870912,	805306368,	402653184,	872415232,	33554432,	385875968,	360710144,	71303168,	895483904,	1039138816,	655884288,	764674048,	7471104,	795803648,	517046272,	815677440,	1056333824,	549089280,	385636352,	212956160,	549962240,	429135616,	577910144,	141505600,	324119904,	14567920,	160843032,	172640916,	610553330,	627400495},	
    {536870912,	268435456,	939524096,	1006632960,	570425344,	285212672,	159383552,	473956352,	333447168,	814743552,	836239360,	25952256,	678035456,	635764736,	129990656,	927678464,	660955136,	1065242624,	325253120,	157891584,	609583616,	267272448,	45797760,	417673280,	876587488,	369839504,	654656088,	1051615436,	450553866,	892675125},	
    {536870912,	805306368,	134217728,	67108864,	1040187392,	754974720,	444596224,	54525952,	551550976,	523239424,	765984768,	989069312,	471203840,	574160896,	790396928,	94519296,	247488512,	707751936,	187353088,	193201152,	735976960,	884808960,	910332544,	106907456,	778239712,	440402480,	744496936,	91229620,	566758134,	260314121},	
    {536870912,	268435456,	671088640,	872415232,	503316480,	50331648,	947912704,	624951296,	467664896,	294649856,	549978112,	352583680,	1065746432,	1003421696,	563773440,	419184640,	153509888,	363024384,	653584384,	437990400,	87342592,	933712640,	536014976,	395575616,	802033120,	797652624,	872197704,	94586692,	787800806,	1041964063},	
    {536870912,	805306368,	939524096,	469762048,	838860800,	922746880,	662700032,	104857600,	618659840,	305135616,	655884288,	799801344,	575275008,	181075968,	388333568,	933838848,	242180096,	16674816,	1008269312,	573578240,	1058542080,	61605632,	675946368,	566958656,	44406496,	115959088,	219111576,	344873452,	1071709866,	531367163},	
    {536870912,	268435456,	134217728,	469762048,	973078528,	989855744,	444596224,	1019215872,	606076928,	506462208,	85458944,	261357568,	996540416,	56688640,	661749760,	426655744,	205840384,	197136384,	748693504,	368053248,	163066368,	174335744,	987635840,	352651200,	665829024,	929784560,	829112632,	7029156,	971732606,	195776757},	
    {536870912,	805306368,	671088640,	469762048,	167772160,	754974720,	528482304,	901775360,	274726912,	11534336,	591921152,	873201664,	909246464,	388562944,	446791680,	911130624,	797499392,	1037561856,	1013016576,	580197376,	307997184,	564911360,	788223872,	415242688,	803213920,	91226544,	455614152,	683421900,	528102354,	237309785},	
    {536870912,	805306368,	134217728,	1006632960,	234881024,	721420288,	578813952,	213909504,	245366784,	449839104,	563609600,	40108032,	879362048,	190906368,	737378304,	171393024,	824565760,	195112960,	55814144,	937905152,	876471808,	302327552,	353993856,	26635200,	309742112,	205943056,	1064192952,	434258804,	790919122,	949187605},	
    {536870912,	268435456,	939524096,	335544320,	436207616,	822083584,	578813952,	281018368,	446693376,	619708416,	277348352,	769392640,	637927424,	657915904,	229998592,	561332224,	234348544,	372707328,	255170560,	565527552,	794280448,	650589440,	96313472,	1030317504,	735181344,	288358704,	313010856,	955002092,	917384366,	719002587},	
    {536870912,	805306368,	939524096,	872415232,	771751936,	687865856,	75497472,	851443712,	421527552,	479199232,	919076864,	254541824,	25821184,	474939392,	348225536,	471613440,	842326016,	992751616,	260708352,	89064448,	675386880,	450649344,	85248640,	247212992,	1036125600,	950217296,	820754920,	436424380,	656244162,	495745187}};

void sobol_seq51(int n, int d, int skip, double *x){
    int m, im, i, j, maskv[d], k;
    for(i=skip;i<(n+skip);i++){
        m = 0; 
        for(k=0;k<d;k++){maskv[k] = 0;}
        im = i^(i>>1);
        while((im!=0) && (m<QP)){
            if(im&1){
                for(j=0;j<d;j++){
                    maskv[j] = (maskv[j]^DIRECT[j][m]);}}
            im = (im>>1);
            m = m+1;}
        for(j=0;j<d;j++){
            x[(i-skip)*d+j] = maskv[j]*SCALE;}}}

/*
int main(){
    int n=8, d=2, skip=8;
    double *x = (double *) calloc(n*d, sizeof(double));
    sobol_seq51(n, d, skip, x);
    for(int i=0; i<n; i++){
        for(int j=0; j<d; j++){
            printf("%.3f\t",x[i*d+j]);}
        printf("\n");}
    return(0);}
*/