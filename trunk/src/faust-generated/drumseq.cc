// generated from file '../src/faust/drumseq.dsp' by dsp2cc:
// Code generated with Faust (https://faust.grame.fr)


namespace drumseq {


Dsp::Dsp() {
}

Dsp::~Dsp() {
}

inline void Dsp::clear_state_f()
{
	for (int l0 = 0; l0 < 2; l0 = l0 + 1) iVec0[l0] = 0;
	for (int l1 = 0; l1 < 2; l1 = l1 + 1) fVec1[l1] = 0.0;
	for (int l2 = 0; l2 < 2; l2 = l2 + 1) fRec0[l2] = 0.0;
	for (int l3 = 0; l3 < 2; l3 = l3 + 1) fRec5[l3] = 0.0;
	for (int l4 = 0; l4 < 2; l4 = l4 + 1) fVec2[l4] = 0.0;
	for (int l5 = 0; l5 < 64; l5 = l5 + 1) fVec3[l5] = 0.0;
	for (int l6 = 0; l6 < 2; l6 = l6 + 1) fRec6[l6] = 0.0;
	for (int l7 = 0; l7 < 2; l7 = l7 + 1) fRec7[l7] = 0.0;
	for (int l8 = 0; l8 < 2; l8 = l8 + 1) fRec3[l8] = 0.0;
	for (int l9 = 0; l9 < 2; l9 = l9 + 1) fRec4[l9] = 0.0;
	for (int l10 = 0; l10 < 2; l10 = l10 + 1) fRec10[l10] = 0.0;
	for (int l11 = 0; l11 < 2; l11 = l11 + 1) fRec11[l11] = 0.0;
	for (int l12 = 0; l12 < 2; l12 = l12 + 1) fRec8[l12] = 0.0;
	for (int l13 = 0; l13 < 2; l13 = l13 + 1) fRec9[l13] = 0.0;
	for (int l14 = 0; l14 < 2; l14 = l14 + 1) fRec14[l14] = 0.0;
	for (int l15 = 0; l15 < 2; l15 = l15 + 1) fRec15[l15] = 0.0;
	for (int l16 = 0; l16 < 2; l16 = l16 + 1) fRec12[l16] = 0.0;
	for (int l17 = 0; l17 < 2; l17 = l17 + 1) fRec13[l17] = 0.0;
	for (int l18 = 0; l18 < 2; l18 = l18 + 1) fRec18[l18] = 0.0;
	for (int l19 = 0; l19 < 2; l19 = l19 + 1) fRec19[l19] = 0.0;
	for (int l20 = 0; l20 < 2; l20 = l20 + 1) fRec16[l20] = 0.0;
	for (int l21 = 0; l21 < 2; l21 = l21 + 1) fRec17[l21] = 0.0;
	for (int l22 = 0; l22 < 2; l22 = l22 + 1) fVec4[l22] = 0.0;
	for (int l23 = 0; l23 < 2; l23 = l23 + 1) fRec2[l23] = 0.0;
	for (int l24 = 0; l24 < 3; l24 = l24 + 1) fRec1[l24] = 0.0;
	for (int l25 = 0; l25 < 2; l25 = l25 + 1) fRec22[l25] = 0.0;
	for (int l26 = 0; l26 < 2; l26 = l26 + 1) iRec20[l26] = 0;
	for (int l27 = 0; l27 < 2; l27 = l27 + 1) fRec21[l27] = 0.0;
	for (int l28 = 0; l28 < 2; l28 = l28 + 1) fRec24[l28] = 0.0;
	for (int l29 = 0; l29 < 3; l29 = l29 + 1) fRec23[l29] = 0.0;
	for (int l30 = 0; l30 < 2; l30 = l30 + 1) iRec25[l30] = 0;
	for (int l31 = 0; l31 < 2; l31 = l31 + 1) fRec26[l31] = 0.0;
	for (int l32 = 0; l32 < 2; l32 = l32 + 1) fRec27[l32] = 0.0;
	for (int l33 = 0; l33 < 2; l33 = l33 + 1) fRec28[l33] = 0.0;
	for (int l34 = 0; l34 < 2; l34 = l34 + 1) fVec5[l34] = 0.0;
	for (int l35 = 0; l35 < 2; l35 = l35 + 1) fRec31[l35] = 0.0;
	for (int l36 = 0; l36 < 2; l36 = l36 + 1) iRec29[l36] = 0;
	for (int l37 = 0; l37 < 2; l37 = l37 + 1) fRec30[l37] = 0.0;
	for (int l38 = 0; l38 < 2; l38 = l38 + 1) fRec32[l38] = 0.0;
	for (int l39 = 0; l39 < 2; l39 = l39 + 1) fRec33[l39] = 0.0;
	for (int l40 = 0; l40 < 2; l40 = l40 + 1) fRec34[l40] = 0.0;
	for (int l41 = 0; l41 < 2; l41 = l41 + 1) fRec35[l41] = 0.0;
	for (int l42 = 0; l42 < 2; l42 = l42 + 1) fRec38[l42] = 0.0;
	for (int l43 = 0; l43 < 2; l43 = l43 + 1) fRec39[l43] = 0.0;
	for (int l44 = 0; l44 < 2; l44 = l44 + 1) fRec40[l44] = 0.0;
	for (int l45 = 0; l45 < 2; l45 = l45 + 1) fRec36[l45] = 0.0;
	for (int l46 = 0; l46 < 2; l46 = l46 + 1) fRec37[l46] = 0.0;
	for (int l47 = 0; l47 < 2; l47 = l47 + 1) iRec42[l47] = 0;
	for (int l48 = 0; l48 < 2; l48 = l48 + 1) fRec43[l48] = 0.0;
	for (int l49 = 0; l49 < 2; l49 = l49 + 1) fRec44[l49] = 0.0;
	for (int l50 = 0; l50 < 2; l50 = l50 + 1) fVec6[l50] = 0.0;
	for (int l51 = 0; l51 < 2; l51 = l51 + 1) fRec45[l51] = 0.0;
	for (int l52 = 0; l52 < 2; l52 = l52 + 1) iRec50[l52] = 0;
	for (int l53 = 0; l53 < 2; l53 = l53 + 1) fVec7[l53] = 0.0;
	for (int l54 = 0; l54 < 2; l54 = l54 + 1) fRec49[l54] = 0.0;
	for (int l55 = 0; l55 < 3; l55 = l55 + 1) fRec48[l55] = 0.0;
	for (int l56 = 0; l56 < 2; l56 = l56 + 1) fVec8[l56] = 0.0;
	for (int l57 = 0; l57 < 2; l57 = l57 + 1) fRec47[l57] = 0.0;
	for (int l58 = 0; l58 < 3; l58 = l58 + 1) fRec46[l58] = 0.0;
	for (int l59 = 0; l59 < 2; l59 = l59 + 1) fRec53[l59] = 0.0;
	for (int l60 = 0; l60 < 2; l60 = l60 + 1) iRec51[l60] = 0;
	for (int l61 = 0; l61 < 2; l61 = l61 + 1) fRec52[l61] = 0.0;
	for (int l62 = 0; l62 < 2; l62 = l62 + 1) fRec58[l62] = 0.0;
	for (int l63 = 0; l63 < 2; l63 = l63 + 1) fRec59[l63] = 0.0;
	for (int l64 = 0; l64 < 2; l64 = l64 + 1) fRec60[l64] = 0.0;
	for (int l65 = 0; l65 < 2; l65 = l65 + 1) fRec56[l65] = 0.0;
	for (int l66 = 0; l66 < 2; l66 = l66 + 1) fRec57[l66] = 0.0;
	for (int l67 = 0; l67 < 2; l67 = l67 + 1) fRec55[l67] = 0.0;
	for (int l68 = 0; l68 < 3; l68 = l68 + 1) fRec54[l68] = 0.0;
	for (int l69 = 0; l69 < 2; l69 = l69 + 1) fRec63[l69] = 0.0;
	for (int l70 = 0; l70 < 2; l70 = l70 + 1) iRec61[l70] = 0;
	for (int l71 = 0; l71 < 2; l71 = l71 + 1) fRec62[l71] = 0.0;
	for (int l72 = 0; l72 < 2; l72 = l72 + 1) fRec65[l72] = 0.0;
	for (int l73 = 0; l73 < 3; l73 = l73 + 1) fRec64[l73] = 0.0;
	for (int l74 = 0; l74 < 2; l74 = l74 + 1) fVec9[l74] = 0.0;
	for (int l75 = 0; l75 < 2; l75 = l75 + 1) fRec68[l75] = 0.0;
	for (int l76 = 0; l76 < 2; l76 = l76 + 1) iRec66[l76] = 0;
	for (int l77 = 0; l77 < 2; l77 = l77 + 1) fRec67[l77] = 0.0;
	for (int l78 = 0; l78 < 2; l78 = l78 + 1) fRec70[l78] = 0.0;
	for (int l79 = 0; l79 < 3; l79 = l79 + 1) fRec69[l79] = 0.0;
	for (int l80 = 0; l80 < 2; l80 = l80 + 1) iRec71[l80] = 0;
	for (int l81 = 0; l81 < 2; l81 = l81 + 1) fRec72[l81] = 0.0;
	for (int l82 = 0; l82 < 2; l82 = l82 + 1) fRec73[l82] = 0.0;
	for (int l83 = 0; l83 < 2; l83 = l83 + 1) fRec74[l83] = 0.0;
	for (int l84 = 0; l84 < 2; l84 = l84 + 1) iRec75[l84] = 0;
	for (int l85 = 0; l85 < 2; l85 = l85 + 1) fRec76[l85] = 0.0;
	for (int l86 = 0; l86 < 2; l86 = l86 + 1) fRec77[l86] = 0.0;
	for (int l87 = 0; l87 < 2; l87 = l87 + 1) fRec78[l87] = 0.0;
	for (int l88 = 0; l88 < 2; l88 = l88 + 1) iRec79[l88] = 0;
	for (int l89 = 0; l89 < 2; l89 = l89 + 1) fRec80[l89] = 0.0;
	for (int l90 = 0; l90 < 2; l90 = l90 + 1) fRec81[l90] = 0.0;
	for (int l91 = 0; l91 < 2; l91 = l91 + 1) fRec82[l91] = 0.0;
}

inline void Dsp::init(unsigned int sample_rate)
{
	fSampleRate = sample_rate;
	fConst0 = std::min<double>(1.92e+05, std::max<double>(1.0, double(fSampleRate)));
	fConst1 = 1.0 / double(int(0.01 * fConst0));
	fConst2 = 3.141592653589793 / fConst0;
	fConst3 = 1.6e+03 / fConst0;
	double fConst4 = std::max<double>(0.0, std::min<double>(2047.0, 0.0003125 * fConst0));
	iConst5 = int(fConst4);
	iConst6 = iConst5 + 1;
	double fConst7 = std::floor(fConst4);
	fConst8 = fConst4 - fConst7;
	fConst9 = fConst7 + (1.0 - fConst4);
	double fConst10 = 4.572399611740729 / fConst0;
	fConst11 = std::cos(fConst10);
	fConst12 = std::sin(fConst10);
	fConst13 = 0.00011370625 * fConst0;
	fConst14 = 6.283185307179586 / fConst0;
	double fConst15 = 20.327952464404543 / fConst0;
	fConst16 = std::cos(fConst15);
	fConst17 = std::sin(fConst15);
	fConst18 = 0.000505514705882353 * fConst0;
	double fConst19 = 326.7256359733385 / fConst0;
	fConst20 = std::cos(fConst19);
	fConst21 = std::sin(fConst19);
	fConst22 = 0.008125 * fConst0;
	double fConst23 = 1498.2980347889782 / fConst0;
	fConst24 = std::cos(fConst23);
	fConst25 = std::sin(fConst23);
	fConst26 = 0.037259615384615384 * fConst0;
	fConst27 = 1.0 / double(int(0.005 * fConst0));
	fConst28 = 1e+02 / fConst0;
	fConst29 = 1.0 - 1.0 / std::pow(1e+03, fConst28);
	fConst30 = 2e+02 / fConst0;
	fConst31 = 1.0 / double(int(0.1 * fConst0));
	fConst32 = 1.0 - 1.0 / std::pow(1e+03, 1e+01 / fConst0);
	fConst33 = 5.026548245743669 / fConst0;
	fConst34 = 5.340707511102648 / fConst0;
	fConst35 = 1.0 / fConst0;
	fConst36 = 1.0 - 1.0 / std::pow(1e+03, 2.5 / fConst0);
	fConst37 = 1.0 / double(int(0.03 * fConst0));
	double fConst38 = std::tan(1570.7963267948965 / fConst0);
	double fConst39 = mydsp_faustpower2_f(fConst38);
	fConst40 = 2.0 * (1.0 - 1.0 / fConst39);
	double fConst41 = 1.0 / fConst38;
	fConst42 = (fConst41 + -1.0000000000000004) / fConst38 + 1.0;
	double fConst43 = (fConst41 + 1.0000000000000004) / fConst38 + 1.0;
	fConst44 = 1.0 / fConst43;
	fConst45 = 1.0 - fConst41;
	fConst46 = 1.396983862573739e-11 / fConst38;
	fConst47 = 1.0 / (fConst41 + 1.0);
	fConst48 = 1.0 / (fConst39 * fConst43);
	fConst49 = 1.0 - 1.0 / std::pow(1e+03, fConst35);
	double fConst50 = std::tan(3141.592653589793 / fConst0);
	fConst51 = 2.0 * (1.0 - 1.0 / mydsp_faustpower2_f(fConst50));
	double fConst52 = 1.0 / fConst50;
	fConst53 = (fConst52 + -1.0000000000000004) / fConst50 + 1.0;
	fConst54 = 1.0 / ((fConst52 + 1.0000000000000004) / fConst50 + 1.0);
	fConst55 = 1.0 / double(int(0.02 * fConst0));
	double fConst56 = 31.41592653589793 / fConst0;
	fConst57 = std::cos(fConst56);
	fConst58 = std::sin(fConst56);
	fConst59 = 1.0 - fConst52;
	fConst60 = 1.0 / (fConst52 + 1.0);
	double fConst61 = std::tan(1643.0529578274618 / fConst0);
	double fConst62 = mydsp_faustpower2_f(fConst61);
	fConst63 = 2.0 * (1.0 - 1.0 / fConst62);
	double fConst64 = 1.0 / fConst61;
	fConst65 = (fConst64 + -1.0000000000000004) / fConst61 + 1.0;
	double fConst66 = (fConst64 + 1.0000000000000004) / fConst61 + 1.0;
	fConst67 = 1.0 / fConst66;
	fConst68 = 1.0 - fConst64;
	fConst69 = 4.656612875245797e-10 / fConst61;
	fConst70 = 1.0 / (fConst64 + 1.0);
	fConst71 = 1.0 / double(int(0.0005 * fConst0));
	fConst72 = 1.0 - 1.0 / std::pow(1e+03, 5.46448087431694 / fConst0);
	fConst73 = 1.0 - std::pow(1.0, 1e+03 / fConst0);
	fConst74 = 2e+03 / fConst0;
	fConst75 = 1.0 / (fConst62 * fConst66);
	double fConst76 = std::tan(22116.812281272145 / fConst0);
	fConst77 = 2.0 * (1.0 - 1.0 / mydsp_faustpower2_f(fConst76));
	double fConst78 = 1.0 / fConst76;
	fConst79 = (fConst78 + -1.0000000000000004) / fConst76 + 1.0;
	fConst80 = 1.0 / ((fConst78 + 1.0000000000000004) / fConst76 + 1.0);
	fConst81 = 1.0 - fConst78;
	fConst82 = 1.0 / (fConst78 + 1.0);
	fConst83 = 1.0 - 1.0 / std::pow(1e+03, 5.0 / fConst0);
	double fConst84 = 1162.3892818282234 / fConst0;
	fConst85 = std::cos(fConst84);
	fConst86 = std::sin(fConst84);
	fConst87 = 1.0 - 1.0 / std::pow(1e+03, 13.333333333333334 / fConst0);
	double fConst88 = 2073.4511513692632 / fConst0;
	fConst89 = std::cos(fConst88);
	fConst90 = std::sin(fConst88);
	fConst91 = 1.0 - 1.0 / std::pow(1e+03, 18.181818181818183 / fConst0);
	IOTA0 = 0;
	clear_state_f();
}

void always_inline Dsp::compute(int count, FAUSTFLOAT *input0, FAUSTFLOAT *output0)
{
#define fCheckbox0 (*fCheckbox0_)
#define fCheckbox1 (*fCheckbox1_)
#define fVslider1 (*fVslider1_)
#define fCheckbox2 (*fCheckbox2_)
#define fVslider2 (*fVslider2_)
#define fCheckbox3 (*fCheckbox3_)
#define fCheckbox4 (*fCheckbox4_)
	int iSlow0 = int(double(fCheckbox0));
	double fSlow1 = double(fCheckbox1);
	double fSlow2 = 0.0010000000000000009 * double(fVslider0);
	double fSlow3 = 0.0010000000000000009 * double(fVslider1);
	double fSlow4 = double(fCheckbox2);
	double fSlow5 = double(fVslider2);
	double fSlow6 = fConst14 * fSlow5;
	double fSlow7 = std::cos(fSlow6);
	double fSlow8 = std::sin(fSlow6);
	double fSlow9 = fConst33 * fSlow5;
	double fSlow10 = std::cos(fSlow9);
	double fSlow11 = std::sin(fSlow9);
	double fSlow12 = fConst34 * fSlow5;
	double fSlow13 = std::cos(fSlow12);
	double fSlow14 = std::sin(fSlow12);
	double fSlow15 = 0.6538461538461537 * fSlow5;
	double fSlow16 = std::max<double>(2.220446049250313e-16, std::fabs(0.9 * fSlow5));
	double fSlow17 = fConst35 * fSlow16;
	double fSlow18 = 1.0 - fConst0 / fSlow16;
	double fSlow19 = 0.0010000000000000009 * double(fVslider3);
	double fSlow20 = double(fCheckbox3);
	double fSlow21 = double(fCheckbox4);
	double fSlow22 = 0.0010000000000000009 * double(fVslider4);
	double fSlow23 = 0.0010000000000000009 * double(fVslider5);
	for (int i0 = 0; i0 < count; i0 = i0 + 1) {
		iVec0[0] = 1;
		fVec1[0] = fSlow1;
		double fTemp0 = double(fSlow1 > fVec1[1]);
		fRec0[0] = fTemp0 + fRec0[1] - fConst1 * double(fRec0[1] > 0.0);
		double fTemp1 = std::tan(fConst2 * (1.2e+04 - 3e+03 * fRec0[0]));
		double fTemp2 = 1.0 / fTemp1;
		double fTemp3 = (fTemp2 + 1.0000000000000004) / fTemp1 + 1.0;
		double fTemp4 = mydsp_faustpower2_f(fTemp1);
		int iTemp5 = 1 - iVec0[1];
		double fTemp6 = ((iTemp5) ? 0.0 : fConst3 + fRec5[1]);
		fRec5[0] = fTemp6 - std::floor(fTemp6);
		double fTemp7 = mydsp_faustpower2_f(2.0 * fRec5[0] + -1.0);
		fVec2[0] = fTemp7;
		double fTemp8 = double(iVec0[1]) * (fTemp7 - fVec2[1]);
		fVec3[IOTA0 & 63] = fTemp8;
		double fTemp9 = fTemp8 - fConst9 * fVec3[(IOTA0 - iConst5) & 63] - fConst8 * fVec3[(IOTA0 - iConst6) & 63];
		fRec6[0] = fConst12 * fRec7[1] + fConst11 * fRec6[1];
		double fTemp10 = double(iTemp5);
		fRec7[0] = fTemp10 + fConst11 * fRec7[1] - fConst12 * fRec6[1];
		double fTemp11 = fConst14 * (fConst13 * fRec6[0] * fTemp9 + 2094.4);
		double fTemp12 = std::cos(fTemp11);
		double fTemp13 = std::sin(fTemp11);
		fRec3[0] = fRec4[1] * fTemp13 + fRec3[1] * fTemp12;
		fRec4[0] = fTemp10 + fRec4[1] * fTemp12 - fTemp13 * fRec3[1];
		fRec10[0] = fConst17 * fRec11[1] + fConst16 * fRec10[1];
		fRec11[0] = fTemp10 + fConst16 * fRec11[1] - fConst17 * fRec10[1];
		double fTemp14 = fConst14 * (fConst18 * fRec10[0] * fTemp9 + 4944.0);
		double fTemp15 = std::cos(fTemp14);
		double fTemp16 = std::sin(fTemp14);
		fRec8[0] = fRec9[1] * fTemp16 + fRec8[1] * fTemp15;
		fRec9[0] = fTemp10 + fRec9[1] * fTemp15 - fTemp16 * fRec8[1];
		fRec14[0] = fConst21 * fRec15[1] + fConst20 * fRec14[1];
		fRec15[0] = fTemp10 + fConst20 * fRec15[1] - fConst21 * fRec14[1];
		double fTemp17 = fConst14 * (fConst22 * fRec14[0] * fTemp9 + 3847.9999999999995);
		double fTemp18 = std::cos(fTemp17);
		double fTemp19 = std::sin(fTemp17);
		fRec12[0] = fRec13[1] * fTemp19 + fRec12[1] * fTemp18;
		fRec13[0] = fTemp10 + fRec13[1] * fTemp18 - fTemp19 * fRec12[1];
		fRec18[0] = fConst25 * fRec19[1] + fConst24 * fRec18[1];
		fRec19[0] = fTemp10 + fConst24 * fRec19[1] - fConst25 * fRec18[1];
		double fTemp20 = fConst14 * (fConst26 * fRec18[0] * fTemp9 + 2144.0);
		double fTemp21 = std::cos(fTemp20);
		double fTemp22 = std::sin(fTemp20);
		fRec16[0] = fRec17[1] * fTemp22 + fRec16[1] * fTemp21;
		fRec17[0] = fTemp10 + fRec17[1] * fTemp21 - fTemp22 * fRec16[1];
		double fTemp23 = fRec16[0] + fRec12[0] + fRec8[0] + fRec3[0];
		fVec4[0] = fTemp23;
		fRec2[0] = -((fRec2[1] * (1.0 - fTemp2) - (fTemp23 - fVec4[1]) / fTemp1) / (fTemp2 + 1.0));
		fRec1[0] = fRec2[0] - (fRec1[2] * ((fTemp2 + -1.0000000000000004) / fTemp1 + 1.0) + 2.0 * fRec1[1] * (1.0 - 1.0 / fTemp4)) / fTemp3;
		fRec22[0] = fTemp0 + fRec22[1] - fConst27 * double(fRec22[1] > 0.0);
		int iTemp24 = fRec22[0] > 0.0;
		int iTemp25 = iTemp24 > 0;
		iRec20[0] = iTemp25 & (iRec20[1] | (fRec21[1] >= 1.0));
		int iTemp26 = iTemp24 <= 0;
		int iTemp27 = iTemp26 & (fRec21[1] > 0.0);
		fRec21[0] = (fConst30 * double(((iRec20[1] == 0) & iTemp25) & (fRec21[1] < 1.0)) + fRec21[1] * (1.0 - fConst29 * double(iTemp27))) * double((iTemp27 == 0) | (fRec21[1] >= 1e-06));
		fRec24[0] = fRec24[1] + fTemp0 - fConst31 * double(fRec24[1] > 0.0);
		double fTemp28 = 6e+03 * fRec24[0] + 9e+03;
		fRec23[0] = fTemp23 - (fConst14 * fRec23[1] * fTemp28 * std::cos(fConst14 * fTemp28) + fRec23[2] * mydsp_faustpower2_f(-(fConst2 * fTemp28)));
		iRec25[0] = iTemp25 & (iRec25[1] | (fRec26[1] >= 1.0));
		int iTemp29 = iTemp26 & (fRec26[1] > 0.0);
		fRec26[0] = (fConst30 * double(((iRec25[1] == 0) & iTemp25) & (fRec26[1] < 1.0)) + fRec26[1] * (1.0 - fConst32 * double(iTemp29))) * double((iTemp29 == 0) | (fRec26[1] >= 1e-06));
		fRec27[0] = fSlow2 + 0.999 * fRec27[1];
		fRec28[0] = fSlow3 + 0.999 * fRec28[1];
		fVec5[0] = fSlow4;
		fRec31[0] = fRec31[1] + double(fSlow4 > fVec5[1]) - fConst27 * double(fRec31[1] > 0.0);
		int iTemp30 = fRec31[0] > 0.0;
		int iTemp31 = iTemp30 > 0;
		iRec29[0] = iTemp31 & (iRec29[1] | (fRec30[1] >= 1.0));
		int iTemp32 = iTemp30 <= 0;
		int iTemp33 = iTemp32 & (fRec30[1] > 0.0);
		fRec30[0] = (fConst30 * double(((iRec29[1] == 0) & iTemp31) & (fRec30[1] < 1.0)) + fRec30[1] * (1.0 - fConst29 * double(iTemp33))) * double((iTemp33 == 0) | (fRec30[1] >= 1e-06));
		fRec32[0] = fSlow8 * fRec33[1] + fSlow7 * fRec32[1];
		fRec33[0] = fTemp10 + fSlow7 * fRec33[1] - fSlow8 * fRec32[1];
		fRec34[0] = fSlow11 * fRec35[1] + fSlow10 * fRec34[1];
		fRec35[0] = fTemp10 + fSlow10 * fRec35[1] - fSlow11 * fRec34[1];
		fRec38[0] = fSlow14 * fRec39[1] + fSlow13 * fRec38[1];
		fRec39[0] = fTemp10 + fSlow13 * fRec39[1] - fSlow14 * fRec38[1];
		double fTemp34 = fSlow17 + fRec40[1] + -1.0;
		int iTemp35 = fTemp34 < 0.0;
		double fTemp36 = fSlow17 + fRec40[1];
		fRec40[0] = ((iTemp35) ? fTemp36 : fTemp34);
		double fRec41 = ((iTemp35) ? fTemp36 : fSlow17 + fRec40[1] + fSlow18 * fTemp34);
		double fTemp37 = fConst14 * (2.0 * fRec41 + fSlow15 * fRec38[0] + -1.0);
		double fTemp38 = std::cos(fTemp37);
		double fTemp39 = std::sin(fTemp37);
		fRec36[0] = fRec37[1] * fTemp39 + fRec36[1] * fTemp38;
		fRec37[0] = fTemp10 + fRec37[1] * fTemp38 - fTemp39 * fRec36[1];
		iRec42[0] = iTemp31 & (iRec42[1] | (fRec43[1] >= 1.0));
		int iTemp40 = iTemp32 & (fRec43[1] > 0.0);
		fRec43[0] = (fConst30 * double(((iRec42[1] == 0) & iTemp31) & (fRec43[1] < 1.0)) + fRec43[1] * (1.0 - fConst36 * double(iTemp40))) * double((iTemp40 == 0) | (fRec43[1] >= 1e-06));
		fRec44[0] = fSlow19 + 0.999 * fRec44[1];
		fVec6[0] = fSlow20;
		double fTemp41 = double(fSlow20 > fVec6[1]);
		fRec45[0] = fTemp41 + fRec45[1] - fConst37 * double(fRec45[1] > 0.0);
		double fTemp42 = std::tan(fConst2 * (5.5e+03 * fRec45[0] + 5e+02));
		double fTemp43 = 1.0 / fTemp42;
		double fTemp44 = (fTemp43 + 1.0000000000000004) / fTemp42 + 1.0;
		iRec50[0] = 1103515245 * iRec50[1] + 12345;
		double fTemp45 = double(iRec50[0]);
		fVec7[0] = fTemp45;
		double fTemp46 = fTemp45 - fVec7[1];
		fRec49[0] = fConst47 * (fConst46 * fTemp46 - fConst45 * fRec49[1]);
		fRec48[0] = fRec49[0] - fConst44 * (fConst42 * fRec48[2] + fConst40 * fRec48[1]);
		double fTemp47 = fRec48[2] + (fRec48[0] - 2.0 * fRec48[1]);
		fVec8[0] = fTemp47;
		fRec47[0] = -((fRec47[1] * (1.0 - fTemp43) - fConst48 * (fTemp47 + fVec8[1])) / (fTemp43 + 1.0));
		fRec46[0] = fRec47[0] - (fRec46[2] * ((fTemp43 + -1.0000000000000004) / fTemp42 + 1.0) + 2.0 * fRec46[1] * (1.0 - 1.0 / mydsp_faustpower2_f(fTemp42))) / fTemp44;
		fRec53[0] = fTemp41 + fRec53[1] - fConst1 * double(fRec53[1] > 0.0);
		int iTemp48 = fRec53[0] > 0.0;
		int iTemp49 = iTemp48 > 0;
		iRec51[0] = iTemp49 & (iRec51[1] | (fRec52[1] >= 1.0));
		int iTemp50 = (iTemp48 <= 0) & (fRec52[1] > 0.0);
		fRec52[0] = (fConst28 * double(((iRec51[1] == 0) & iTemp49) & (fRec52[1] < 1.0)) + fRec52[1] * (1.0 - fConst49 * double(iTemp50))) * double((iTemp50 == 0) | (fRec52[1] >= 1e-06));
		fRec58[0] = fRec58[1] + fTemp41 - fConst55 * double(fRec58[1] > 0.0);
		fRec59[0] = fConst58 * fRec60[1] + fConst57 * fRec59[1];
		fRec60[0] = fTemp10 + fConst57 * fRec60[1] - fConst58 * fRec59[1];
		double fTemp51 = fConst14 * (19.23076923076923 * fRec59[0] + 5e+01 * (fRec58[0] + 1.0));
		double fTemp52 = std::cos(fTemp51);
		double fTemp53 = std::sin(fTemp51);
		fRec56[0] = fRec57[1] * fTemp53 + fRec56[1] * fTemp52;
		fRec57[0] = fTemp10 + fRec57[1] * fTemp52 - fTemp53 * fRec56[1];
		fRec55[0] = -(fConst60 * (fConst59 * fRec55[1] - (fRec56[0] + fRec56[1])));
		fRec54[0] = fRec55[0] - fConst54 * (fConst53 * fRec54[2] + fConst51 * fRec54[1]);
		fRec63[0] = fTemp41 + fRec63[1] - fConst27 * double(fRec63[1] > 0.0);
		int iTemp54 = fRec63[0] > 0.0;
		int iTemp55 = iTemp54 > 0;
		iRec61[0] = iTemp55 & (iRec61[1] | (fRec62[1] >= 1.0));
		int iTemp56 = (iTemp54 <= 0) & (fRec62[1] > 0.0);
		fRec62[0] = (fConst30 * double(((iRec61[1] == 0) & iTemp55) & (fRec62[1] < 1.0)) + fRec62[1] * (1.0 - fConst36 * double(iTemp56))) * double((iTemp56 == 0) | (fRec62[1] >= 1e-06));
		fRec65[0] = fConst70 * (fConst69 * fTemp46 - fConst68 * fRec65[1]);
		fRec64[0] = fRec65[0] - fConst67 * (fConst65 * fRec64[2] + fConst63 * fRec64[1]);
		fVec9[0] = fSlow21;
		fRec68[0] = fRec68[1] + double(fSlow21 > fVec9[1]) - fConst71 * double(fRec68[1] > 0.0);
		int iTemp57 = fRec68[0] > 0.0;
		int iTemp58 = iTemp57 > 0;
		iRec66[0] = iTemp58 & (iRec66[1] | (fRec67[1] >= 1.0));
		int iTemp59 = iTemp57 <= 0;
		int iTemp60 = iTemp59 & (fRec67[1] > 0.0);
		fRec67[0] = (fConst74 * double(((iRec66[1] == 0) & iTemp58) & (fRec67[1] < 1.0)) + fRec67[1] * (1.0 - fConst73 * double(iRec66[1] & (fRec67[1] > 1.0)) - fConst72 * double(iTemp60))) * double((iTemp60 == 0) | (fRec67[1] >= 1e-06));
		fRec70[0] = fConst82 * (4.656612875245797e-10 * (fTemp45 + fVec7[1]) - fConst81 * fRec70[1]);
		fRec69[0] = fRec70[0] - fConst80 * (fConst79 * fRec69[2] + fConst77 * fRec69[1]);
		iRec71[0] = iTemp58 & (iRec71[1] | (fRec72[1] >= 1.0));
		int iTemp61 = iTemp59 & (fRec72[1] > 0.0);
		fRec72[0] = (fConst74 * double(((iRec71[1] == 0) & iTemp58) & (fRec72[1] < 1.0)) + fRec72[1] * (1.0 - fConst73 * double(iRec71[1] & (fRec72[1] > 1.0)) - fConst83 * double(iTemp61))) * double((iTemp61 == 0) | (fRec72[1] >= 1e-06));
		fRec73[0] = fConst86 * fRec74[1] + fConst85 * fRec73[1];
		fRec74[0] = fTemp10 + fConst85 * fRec74[1] - fConst86 * fRec73[1];
		iRec75[0] = iTemp58 & (iRec75[1] | (fRec76[1] >= 1.0));
		int iTemp62 = iTemp59 & (fRec76[1] > 0.0);
		fRec76[0] = (fConst74 * double(((iRec75[1] == 0) & iTemp58) & (fRec76[1] < 1.0)) + fRec76[1] * (1.0 - fConst73 * double(iRec75[1] & (fRec76[1] > 1.0)) - fConst87 * double(iTemp62))) * double((iTemp62 == 0) | (fRec76[1] >= 1e-06));
		fRec77[0] = fConst90 * fRec78[1] + fConst89 * fRec77[1];
		fRec78[0] = fTemp10 + fConst89 * fRec78[1] - fConst90 * fRec77[1];
		iRec79[0] = iTemp58 & (iRec79[1] | (fRec80[1] >= 1.0));
		int iTemp63 = iTemp59 & (fRec80[1] > 0.0);
		fRec80[0] = (fConst74 * double(((iRec79[1] == 0) & iTemp58) & (fRec80[1] < 1.0)) + fRec80[1] * (1.0 - fConst73 * double(iRec79[1] & (fRec80[1] > 1.0)) - fConst91 * double(iTemp63))) * double((iTemp63 == 0) | (fRec80[1] >= 1e-06));
		fRec81[0] = fSlow22 + 0.999 * fRec81[1];
		fRec82[0] = fSlow23 + 0.999 * fRec82[1];
		double fTemp64 = std::pow(1e+01, 0.05 * fRec82[0]) * (std::pow(1e+01, 0.05 * fRec81[0]) * (fRec80[0] * (fRec77[0] + 0.25) + fRec76[0] * (fRec73[0] + 0.25) + 0.2 * (fConst80 * fRec72[0] * (fRec69[2] + fRec69[0] + 2.0 * fRec69[1]) + fConst75 * fRec67[0] * (fRec64[0] + fRec64[2] - 2.0 * fRec64[1]))) + 2.0 * ((fConst54 * fRec62[0] * (fRec54[2] + fRec54[0] + 2.0 * fRec54[1]) + fRec52[0] * (fRec46[2] + fRec46[0] + 2.0 * fRec46[1]) / fTemp44) * std::pow(1e+01, 0.05 * fRec44[0]) + (0.25 * fRec43[0] * (5.0 * fRec36[0] + 0.5 * (fRec34[0] + fRec32[0])) + 3.0 * fRec30[0]) * std::pow(1e+01, 0.05 * fRec28[0])) + std::pow(1e+01, 0.05 * fRec27[0]) * (fRec26[0] * (fRec23[0] - fRec23[2]) + std::sqrt(fRec21[0]) * (fRec1[0] + fRec1[2] - 2.0 * fRec1[1]) / (fTemp4 * fTemp3)));
		output0[i0] = FAUSTFLOAT(((iSlow0) ? fTemp64 : double(input0[i0]) + fTemp64));
		iVec0[1] = iVec0[0];
		fVec1[1] = fVec1[0];
		fRec0[1] = fRec0[0];
		fRec5[1] = fRec5[0];
		fVec2[1] = fVec2[0];
		IOTA0 = IOTA0 + 1;
		fRec6[1] = fRec6[0];
		fRec7[1] = fRec7[0];
		fRec3[1] = fRec3[0];
		fRec4[1] = fRec4[0];
		fRec10[1] = fRec10[0];
		fRec11[1] = fRec11[0];
		fRec8[1] = fRec8[0];
		fRec9[1] = fRec9[0];
		fRec14[1] = fRec14[0];
		fRec15[1] = fRec15[0];
		fRec12[1] = fRec12[0];
		fRec13[1] = fRec13[0];
		fRec18[1] = fRec18[0];
		fRec19[1] = fRec19[0];
		fRec16[1] = fRec16[0];
		fRec17[1] = fRec17[0];
		fVec4[1] = fVec4[0];
		fRec2[1] = fRec2[0];
		fRec1[2] = fRec1[1];
		fRec1[1] = fRec1[0];
		fRec22[1] = fRec22[0];
		iRec20[1] = iRec20[0];
		fRec21[1] = fRec21[0];
		fRec24[1] = fRec24[0];
		fRec23[2] = fRec23[1];
		fRec23[1] = fRec23[0];
		iRec25[1] = iRec25[0];
		fRec26[1] = fRec26[0];
		fRec27[1] = fRec27[0];
		fRec28[1] = fRec28[0];
		fVec5[1] = fVec5[0];
		fRec31[1] = fRec31[0];
		iRec29[1] = iRec29[0];
		fRec30[1] = fRec30[0];
		fRec32[1] = fRec32[0];
		fRec33[1] = fRec33[0];
		fRec34[1] = fRec34[0];
		fRec35[1] = fRec35[0];
		fRec38[1] = fRec38[0];
		fRec39[1] = fRec39[0];
		fRec40[1] = fRec40[0];
		fRec36[1] = fRec36[0];
		fRec37[1] = fRec37[0];
		iRec42[1] = iRec42[0];
		fRec43[1] = fRec43[0];
		fRec44[1] = fRec44[0];
		fVec6[1] = fVec6[0];
		fRec45[1] = fRec45[0];
		iRec50[1] = iRec50[0];
		fVec7[1] = fVec7[0];
		fRec49[1] = fRec49[0];
		fRec48[2] = fRec48[1];
		fRec48[1] = fRec48[0];
		fVec8[1] = fVec8[0];
		fRec47[1] = fRec47[0];
		fRec46[2] = fRec46[1];
		fRec46[1] = fRec46[0];
		fRec53[1] = fRec53[0];
		iRec51[1] = iRec51[0];
		fRec52[1] = fRec52[0];
		fRec58[1] = fRec58[0];
		fRec59[1] = fRec59[0];
		fRec60[1] = fRec60[0];
		fRec56[1] = fRec56[0];
		fRec57[1] = fRec57[0];
		fRec55[1] = fRec55[0];
		fRec54[2] = fRec54[1];
		fRec54[1] = fRec54[0];
		fRec63[1] = fRec63[0];
		iRec61[1] = iRec61[0];
		fRec62[1] = fRec62[0];
		fRec65[1] = fRec65[0];
		fRec64[2] = fRec64[1];
		fRec64[1] = fRec64[0];
		fVec9[1] = fVec9[0];
		fRec68[1] = fRec68[0];
		iRec66[1] = iRec66[0];
		fRec67[1] = fRec67[0];
		fRec70[1] = fRec70[0];
		fRec69[2] = fRec69[1];
		fRec69[1] = fRec69[0];
		iRec71[1] = iRec71[0];
		fRec72[1] = fRec72[0];
		fRec73[1] = fRec73[0];
		fRec74[1] = fRec74[0];
		iRec75[1] = iRec75[0];
		fRec76[1] = fRec76[0];
		fRec77[1] = fRec77[0];
		fRec78[1] = fRec78[0];
		iRec79[1] = iRec79[0];
		fRec80[1] = fRec80[0];
		fRec81[1] = fRec81[0];
		fRec82[1] = fRec82[0];
	}
#undef fCheckbox0
#undef fCheckbox1
#undef fVslider1
#undef fCheckbox2
#undef fVslider2
#undef fCheckbox3
#undef fCheckbox4
}

int Dsp::register_par(const ParamReg& reg)
{
	fCheckbox0_ = reg.registerFloatVar("seq.direct_out","","BA",N_("bypass the rack for direct output"),&fCheckbox0, 0.0, 0.0, 1.0, 1.0, 0);
	reg.registerFloatVar("seq.gain","","S",N_("Volume level in decibels"),&fVslider5, -2e+01, -6e+01, 4e+01, 0.1, 0);
	reg.registerFloatVar("seq.hat_closed.dsp.Gain","","S",N_("Volume level in decibels"),&fVslider0, -2e+01, -6e+01, 4e+01, 0.1, 0);
	fCheckbox1_ = reg.registerFloatVar("seq.hat_closed.dsp.gate","","BNA","",&fCheckbox1, 0.0, 0.0, 1.0, 1.0, 0);
	fCheckbox3_ = reg.registerFloatVar("seq.kick.dsp.gate","","BNA","",&fCheckbox3, 0.0, 0.0, 1.0, 1.0, 0);
	reg.registerFloatVar("seq.kick.dsp.Gain","","S",N_("Volume level in decibels"),&fVslider3, -2e+01, -6e+01, 4e+01, 0.1, 0);
	reg.registerFloatVar("seq.snare.dsp.Gain","","S",N_("Volume level in decibels"),&fVslider4, -2e+01, -6e+01, 4e+01, 0.1, 0);
	fCheckbox4_ = reg.registerFloatVar("seq.snare.dsp.gate","","BNA","",&fCheckbox4, 0.0, 0.0, 1.0, 1.0, 0);
	fVslider1_ = reg.registerFloatVar("seq.tom.dsp.Gainf","","SNA","",&fVslider1, -2e+01, -6e+01, 4e+01, 0.1, 0);
	fVslider2_ = reg.registerFloatVar("seq.tom.dsp.freq","","SNA","",&fVslider2, 9e+01, 9e+01, 1.5e+02, 1.0, 0);
	fCheckbox2_ = reg.registerFloatVar("seq.tom.dsp.gate","","BNA","",&fCheckbox2, 0.0, 0.0, 1.0, 1.0, 0);
	return 0;
}


} // end namespace drumseq
