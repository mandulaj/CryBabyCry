
#ifndef _FILTER_COEFS_H
#define _FILTER_COEFS_H



static const float32_t float_hamming_512[] = {
			0.080000000000000020f,   0.080034772851092170f,   0.080139086147189730f,   0.080312924117550420f,
		    0.080556260480253110f,   0.080869058446171260f,   0.081251270724534920f,   0.081702839530080400f,
		    0.082223696591786740f,   0.082813763163197220f,   0.083472950034324760f,   0.084201157545139290f,
		    0.084998275600634940f,   0.085864183687475170f,   0.086798750892212120f,   0.087801835921079650f,
		    0.088873287121354390f,   0.090012942504284100f,   0.091220629769577730f,   0.092496166331455240f,
		    0.093839359346251480f,   0.095250005741572440f,   0.096727892246995850f,   0.098272795426315900f,
		    0.099884481711322910f,   0.101562707437116040f,   0.103307218878942120f,   0.105117752290554810f,
		    0.106994033944090350f,   0.108935780171450610f,   0.110942697407190320f,   0.113014482232899890f,
		    0.115150821423078420f,   0.117351391992488510f,   0.119615861244988080f,   0.121943886823828670f,
		    0.124335116763415630f,   0.126789189542520110f,   0.129305734138936420f,   0.131884370085575300f,
		    0.134524707527985620f,   0.137226347283294470f,   0.139988880900558940f,   0.142811890722517650f,
		    0.145694949948735050f,   0.148637622700127590f,   0.151639464084863510f,   0.154700020265623020f,
		    0.157818828528213610f,   0.160995417351525120f,   0.164229306478817890f,   0.167520006990330760f,
		    0.170867021377198950f,   0.174269843616671190f,   0.177727959248612070f,   0.181240845453283060f,
		    0.184807971130384440f,   0.188428796979351280f,   0.192102775580887230f,   0.195829351479728130f,
		    0.199607961268618020f,   0.203438033673489670f,   0.207318989639832310f,   0.211250242420237990f,
		    0.215231197663108500f,   0.219261253502513960f,   0.223339800649186460f,   0.227466222482636700f,
		    0.231639895144377660f,   0.235860187632244420f,   0.240126461895792230f,   0.244438072932761930f,
		    0.248794368886594120f,   0.253194691144982610f,   0.257638374439446040f,   0.262124746945908760f,
		    0.266653130386269640f,   0.271222840130950720f,   0.275833185302401580f,   0.280483468879552330f,
		    0.285172987803192950f,   0.289901033082265450f,   0.294666889901055270f,   0.299469837727259940f,
		    0.304309150420925160f,   0.309184096344226060f,   0.314093938472081450f,   0.319037934503581530f,
		    0.324015336974214590f,   0.329025393368871770f,   0.334067346235618730f,   0.339140433300210600f,
		    0.344243887581338730f,   0.349376937506586380f,   0.354538807029081140f,   0.359728715744821790f,
		    0.364945879010665000f,   0.370189508062952700f,   0.375458810136762300f,   0.380752988585761680f,
		    0.386071243002651390f,   0.391412769340175330f,   0.396776760032681470f,   0.402162404118215240f,
		    0.407568887361125180f,   0.412995392375164420f,   0.418441098747068700f,   0.423905183160591230f,
		    0.429386819520976960f,   0.434885179079856800f,   0.440399430560542820f,   0.445928740283706270f,
		    0.451472272293418300f,   0.457029188483535020f,   0.462598648724407590f,   0.468179810989898690f,
		    0.473771831484684770f,   0.479373864771826920f,   0.484985063900588990f,   0.490604580534485080f,
		    0.496231565079536420f,   0.501865166812718310f,   0.507504534010577820f,   0.513148814078002720f,
		    0.518797153677121980f,   0.524448698856319170f,   0.530102595179337890f,   0.535757987854461050f,
		    0.541414021863743660f,   0.547069842092279600f,   0.552724593457483810f,   0.558377421038368520f,
		    0.564027470204795600f,   0.569673886746685510f,   0.575315817003161700f,   0.580952407991612410f,
		    0.586582807536650370f,   0.592206164398949350f,   0.597821628403940820f,   0.603428350570347940f,
		    0.609025483238540220f,   0.614612180198688240f,   0.620187596818698280f,   0.625750890171909990f,
		    0.631301219164534740f,   0.636837744662818170f,   0.642359629619904670f,   0.647866039202388500f,
		    0.653356140916528490f,   0.658829104734109940f,   0.664284103217933300f,   0.669720311646911810f,
		    0.675136908140757660f,   0.680533073784239000f,   0.685907992750988790f,   0.691260852426846980f,
		    0.696590843532715940f,   0.701897160246912950f,   0.707179000326998990f,   0.712435565231067100f,
		    0.717666060238471020f,   0.722869694569975740f,   0.728045681507312860f,   0.733193238512121370f,
		    0.738311587344256840f,   0.743399954179450710f,   0.748457569726301620f,   0.753483669342582600f,
		    0.758477493150843230f,   0.763438286153293790f,   0.768365298345949910f,   0.773257784832023340f,
		    0.778115005934540080f,   0.782936227308169140f,   0.787720720050245630f,   0.792467760810970920f,
		    0.797176631902773320f,   0.801846621408812690f,   0.806477023290611770f,   0.811067137494800530f,
		    0.815616270058954100f,   0.820123733216510550f,   0.824588845500751620f,   0.829010931847831370f,
		    0.833389323698836670f,   0.837723359100863600f,   0.842012382807096230f,   0.846255746375870870f,
		    0.850452808268711280f,   0.854602933947320900f,   0.858705495969516170f,   0.862759874084087050f,
		    0.866765455324570720f,   0.870721634101923710f,   0.874627812296078330f,   0.878483399346370870f,
		    0.882287812340825980f,   0.886040476104284490f,   0.889740823285362750f,   0.893388294442228230f,
		    0.896982338127179090f,   0.900522410970015840f,   0.904007977760191480f,   0.907438511527727920f,
		    0.910813493622886550f,   0.914132413794581210f,   0.917394770267520920f,   0.920600069818071520f,
		    0.923747827848824480f,   0.926837568461861270f,   0.929868824530702300f,   0.932841137770930920f,
		    0.935754058809478710f,   0.938607147252565000f,   0.941399971752278850f,   0.944132110071791990f,
		    0.946803149149195940f,   0.949412685159951360f,   0.951960323577940030f,   0.954445679235112810f,
		    0.956868376379721110f,   0.959228048733125500f,   0.961524339545172250f,   0.963756901648128770f,
		    0.965925397509170950f,   0.968029499281413350f,   0.970068888853474980f,   0.972043257897573510f,
		    0.973952307916140620f,   0.975795750286950090f,   0.977573306306753540f,   0.979284707233417430f,
		    0.980929694326552190f,   0.982508018886630640f,   0.984019442292588090f,   0.985463736037898270f,
		    0.986840681765120520f,   0.988150071298912640f,   0.989391706677503650f,   0.990565400182623620f,
		    0.991670974367883320f,   0.992708262085602370f,   0.993677106512079190f,   0.994577361171300910f,
		    0.995408889957088320f,   0.996171567153673500f,   0.996865277454705880f,   0.997489915980685590f,
		    0.998045388294819260f,   0.998531610417298100f,   0.998948508837993690f,   0.999296020527572940f,
		    0.999574092947025820f,   0.999782684055609770f,   0.999921762317204750f,   0.999991306705082070f,
		    0.999991306705082070f,   0.999921762317204750f,   0.999782684055609770f,   0.999574092947025820f,
		    0.999296020527572940f,   0.998948508837993690f,   0.998531610417298100f,   0.998045388294819260f,
		    0.997489915980685590f,   0.996865277454705880f,   0.996171567153673500f,   0.995408889957088320f,
		    0.994577361171300910f,   0.993677106512079190f,   0.992708262085602370f,   0.991670974367883320f,
		    0.990565400182623620f,   0.989391706677503650f,   0.988150071298912640f,   0.986840681765120520f,
		    0.985463736037898270f,   0.984019442292588090f,   0.982508018886630640f,   0.980929694326552190f,
		    0.979284707233417430f,   0.977573306306753540f,   0.975795750286950090f,   0.973952307916140620f,
		    0.972043257897573510f,   0.970068888853474980f,   0.968029499281413350f,   0.965925397509170950f,
		    0.963756901648128770f,   0.961524339545172250f,   0.959228048733125500f,   0.956868376379721110f,
		    0.954445679235112810f,   0.951960323577940030f,   0.949412685159951360f,   0.946803149149195940f,
		    0.944132110071791990f,   0.941399971752278850f,   0.938607147252565000f,   0.935754058809478710f,
		    0.932841137770930920f,   0.929868824530702300f,   0.926837568461861270f,   0.923747827848824480f,
		    0.920600069818071520f,   0.917394770267520920f,   0.914132413794581210f,   0.910813493622886550f,
		    0.907438511527727920f,   0.904007977760191480f,   0.900522410970015840f,   0.896982338127179090f,
		    0.893388294442228230f,   0.889740823285362750f,   0.886040476104284490f,   0.882287812340825980f,
		    0.878483399346370870f,   0.874627812296078330f,   0.870721634101923710f,   0.866765455324570720f,
		    0.862759874084087050f,   0.858705495969516170f,   0.854602933947320900f,   0.850452808268711280f,
		    0.846255746375870870f,   0.842012382807096230f,   0.837723359100863600f,   0.833389323698836670f,
		    0.829010931847831370f,   0.824588845500751620f,   0.820123733216510550f,   0.815616270058954100f,
		    0.811067137494800530f,   0.806477023290611770f,   0.801846621408812690f,   0.797176631902773320f,
		    0.792467760810970920f,   0.787720720050245630f,   0.782936227308169140f,   0.778115005934540080f,
		    0.773257784832023340f,   0.768365298345949910f,   0.763438286153293790f,   0.758477493150843230f,
		    0.753483669342582600f,   0.748457569726301620f,   0.743399954179450710f,   0.738311587344256840f,
		    0.733193238512121370f,   0.728045681507312860f,   0.722869694569975740f,   0.717666060238471020f,
		    0.712435565231067100f,   0.707179000326998990f,   0.701897160246912950f,   0.696590843532715940f,
		    0.691260852426846980f,   0.685907992750988790f,   0.680533073784239000f,   0.675136908140757660f,
		    0.669720311646911810f,   0.664284103217933300f,   0.658829104734109940f,   0.653356140916528490f,
		    0.647866039202388500f,   0.642359629619904670f,   0.636837744662818170f,   0.631301219164534740f,
		    0.625750890171909990f,   0.620187596818698280f,   0.614612180198688240f,   0.609025483238540220f,
		    0.603428350570347940f,   0.597821628403940820f,   0.592206164398949350f,   0.586582807536650370f,
		    0.580952407991612410f,   0.575315817003161700f,   0.569673886746685510f,   0.564027470204795600f,
		    0.558377421038368520f,   0.552724593457483810f,   0.547069842092279600f,   0.541414021863743660f,
		    0.535757987854461050f,   0.530102595179337890f,   0.524448698856319170f,   0.518797153677121980f,
		    0.513148814078002720f,   0.507504534010577820f,   0.501865166812718310f,   0.496231565079536420f,
		    0.490604580534485080f,   0.484985063900588990f,   0.479373864771826920f,   0.473771831484684770f,
		    0.468179810989898690f,   0.462598648724407590f,   0.457029188483535020f,   0.451472272293418300f,
		    0.445928740283706270f,   0.440399430560542820f,   0.434885179079856800f,   0.429386819520976960f,
		    0.423905183160591230f,   0.418441098747068700f,   0.412995392375164420f,   0.407568887361125180f,
		    0.402162404118215240f,   0.396776760032681470f,   0.391412769340175330f,   0.386071243002651390f,
		    0.380752988585761680f,   0.375458810136762300f,   0.370189508062952700f,   0.364945879010665000f,
		    0.359728715744821790f,   0.354538807029081140f,   0.349376937506586380f,   0.344243887581338730f,
		    0.339140433300210600f,   0.334067346235618730f,   0.329025393368871770f,   0.324015336974214590f,
		    0.319037934503581530f,   0.314093938472081450f,   0.309184096344226060f,   0.304309150420925160f,
		    0.299469837727259940f,   0.294666889901055270f,   0.289901033082265450f,   0.285172987803192950f,
		    0.280483468879552330f,   0.275833185302401580f,   0.271222840130950720f,   0.266653130386269640f,
		    0.262124746945908760f,   0.257638374439446040f,   0.253194691144982610f,   0.248794368886594120f,
		    0.244438072932761930f,   0.240126461895792230f,   0.235860187632244420f,   0.231639895144377660f,
		    0.227466222482636700f,   0.223339800649186460f,   0.219261253502513960f,   0.215231197663108500f,
		    0.211250242420237990f,   0.207318989639832310f,   0.203438033673489670f,   0.199607961268618020f,
		    0.195829351479728130f,   0.192102775580887230f,   0.188428796979351280f,   0.184807971130384440f,
		    0.181240845453283060f,   0.177727959248612070f,   0.174269843616671190f,   0.170867021377198950f,
		    0.167520006990330760f,   0.164229306478817890f,   0.160995417351525120f,   0.157818828528213610f,
		    0.154700020265623020f,   0.151639464084863510f,   0.148637622700127590f,   0.145694949948735050f,
		    0.142811890722517650f,   0.139988880900558940f,   0.137226347283294470f,   0.134524707527985620f,
		    0.131884370085575300f,   0.129305734138936420f,   0.126789189542520110f,   0.124335116763415630f,
		    0.121943886823828670f,   0.119615861244988080f,   0.117351391992488510f,   0.115150821423078420f,
		    0.113014482232899890f,   0.110942697407190320f,   0.108935780171450610f,   0.106994033944090350f,
		    0.105117752290554810f,   0.103307218878942120f,   0.101562707437116040f,   0.099884481711322910f,
		    0.098272795426315900f,   0.096727892246995850f,   0.095250005741572440f,   0.093839359346251480f,
		    0.092496166331455240f,   0.091220629769577730f,   0.090012942504284100f,   0.088873287121354390f,
		    0.087801835921079650f,   0.086798750892212120f,   0.085864183687475170f,   0.084998275600634940f,
		    0.084201157545139290f,   0.083472950034324760f,   0.082813763163197220f,   0.082223696591786740f,
		    0.081702839530080400f,   0.081251270724534920f,   0.080869058446171260f,   0.080556260480253110f,
		    0.080312924117550420f,   0.080139086147189730f,   0.080034772851092170f,   0.080000000000000020f
};



static const float32_t dct_weights_64[128] = {
		   	2.000000000000000000f,   0.000000000000000000f,   1.999397637392408500f,  -0.049082457045824580f,
		    1.997590912410344810f,  -0.098135348654836030f,   1.994580913357380410f,  -0.147129127199334850f,
		    1.990369453344393860f,  -0.196034280659121210f,   1.984959069197419930f,  -0.244821350398432390f,
		    1.978353019929562030f,  -0.293460948910723500f,   1.970555284777882440f,  -0.341923777520602430f,
		    1.961570560806460860f,  -0.390180644032256500f,   1.951404260077057140f,  -0.438202480313739600f,
		    1.940062506389087950f,  -0.485960359806527740f,   1.927552131590879680f,  -0.533425514949796730f,
		    1.913880671464417650f,  -0.580569354508924660f,   1.899056361186073350f,  -0.627363480797783040f,
		    1.883088130366041610f,  -0.673779706784440100f,   1.865985597669477910f,  -0.719790073069976220f,
		    1.847759065022573480f,  -0.765366864730179560f,   1.828419511407061380f,  -0.810482628009979720f,
		    1.807978586246886680f,  -0.855110186860564170f,   1.786448602391030650f,  -0.899222659309213080f,
		    1.763842528696710100f,  -0.942793473651995280f,   1.740173982217422920f,  -0.985796384459568080f,
		    1.715457220000544240f,  -1.028205488386443320f,   1.689707130499414230f,  -1.069995239774194310f,
		    1.662939224605090470f,  -1.111140466039204360f,   1.635169626303167420f,  -1.151616382835690680f,
		    1.606415062961289890f,  -1.191398608984866710f,   1.576692855253212680f,  -1.230463181161253640f,
		    1.546020906725473990f,  -1.268786568327290980f,   1.514417693012969130f,  -1.306345685907553510f,
		    1.481902250709918210f,  -1.343117909694036660f,   1.448494165902934010f,  -1.379081089474133660f,
		    1.414213562373095150f,  -1.414213562373094920f,   1.379081089474133880f,  -1.448494165902933780f,
		    1.343117909694036660f,  -1.481902250709918210f,   1.306345685907553510f,  -1.514417693012968910f,
		    1.268786568327290980f,  -1.546020906725473990f,   1.230463181161253640f,  -1.576692855253212460f,
		    1.191398608984866940f,  -1.606415062961289660f,   1.151616382835690680f,  -1.635169626303167420f,
		    1.111140466039204580f,  -1.662939224605090470f,   1.069995239774194530f,  -1.689707130499414010f,
		    1.028205488386443320f,  -1.715457220000544240f,   0.985796384459568190f,  -1.740173982217422700f,
		    0.942793473651995620f,  -1.763842528696709880f,   0.899222659309213190f,  -1.786448602391030650f,
		    0.855110186860564390f,  -1.807978586246886680f,   0.810482628009979720f,  -1.828419511407061380f,
		    0.765366864730179670f,  -1.847759065022573480f,   0.719790073069976550f,  -1.865985597669477690f,
		    0.673779706784440100f,  -1.883088130366041610f,   0.627363480797783150f,  -1.899056361186073350f,
		    0.580569354508924660f,  -1.913880671464417870f,   0.533425514949796840f,  -1.927552131590879680f,
		    0.485960359806527960f,  -1.940062506389087950f,   0.438202480313739540f,  -1.951404260077057140f,
		    0.390180644032256660f,  -1.961570560806460860f,   0.341923777520602710f,  -1.970555284777882440f,
		    0.293460948910723500f,  -1.978353019929562030f,   0.244821350398432560f,  -1.984959069197419930f,
		    0.196034280659121540f,  -1.990369453344393640f,   0.147129127199334910f,  -1.994580913357380410f,
		    0.098135348654836250f,  -1.997590912410344810f,   0.049082457045824530f,  -1.999397637392408500f
};



static const q31_t q31_biquad[] =  { 0x3c8ac306, 0x86ea79fe, 0x3c8ac306, 0x78e5ac16, 0xc6baa00f};
static const float32_t float_biquad[] = {0.94596935, -1.89193869, 0.94596935, 1.88901808, -0.89485930};





#endif
