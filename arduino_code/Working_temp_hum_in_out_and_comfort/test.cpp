#include <iostream>
#include <cmath>

using namespace std;

// Function to calculate saturated vapor pressure
double FNPS(double TA) {
    return exp(16.6536 - 4030.183 / (TA + 235));
}

int main() {
    double CLO, MET, WME, TA, TR, VEL, RH, PA;
    double ICL, M, W, MW, FCL, HCF, TAA, TRA, TCLA, TCL, HL1, HL2, HL3, HL4, HL5, HL6, PMV, PPD;
    int N = 0;
    double XF, XN;
    double EPS = 0.00015;
    
    // Input from user
    cout << "Clothing (clo): ";
    cin >> CLO;
    cout << "Metabolic rate (met): ";
    cin >> MET;
    cout << "External work, normally around 0 (met): ";
    cin >> WME;
    cout << "Air Temperature (C): ";
    cin >> TA;
    cout << "Mean radiant temperature (C): ";
    cin >> TR;
    cout << "Relative air velocity (m/s): ";
    cin >> VEL;
    cout << "Relative humidity (%): ";
    cin >> RH;
    cout << "Water vapor pressure (Pa): ";
    cin >> PA;
    
    // Calculations
    ICL = 0.155 * CLO; // Thermal insulation of clothing in m2K/W
    M = MET * 58.15;   // Metabolic rate in W/m2
    W = WME * 58.15;   // External work in W/m2
    MW = M - W;        // Internal heat production
    if (ICL < 0.078) {
        FCL = 1 + 1.29 * ICL;
    } else {
        FCL = 1.05 + 0.645 * ICL;
    }
    HCF = 12.1 * sqrt(VEL);  // Heat transfer coefficient by forced convection
    TAA = TA + 273;          // Air temperature in Kelvin
    TRA = TR + 273;          // Mean radiant temperature in Kelvin

    // Iterative calculation for clothing surface temperature
    TCLA = TAA + (35.5 - TA) / (3.5 * (6.45 * ICL + 0.1));
    XN = TCLA / 100;
    XF = XN;

    // Loop to iterate until the solution converges
    do {
        double P1 = ICL * FCL;
        double P2 = P1 * 3.96;
        double P3 = P1 * 100;
        double P4 = P1 * TAA;
        double P5 = 308.7 - 0.028 * MW + P2 * pow((TRA / 100), 4);

        double HCN = 2.38 * pow(abs(100 * XF - TAA), 0.25);
        double HC = (HCF > HCN) ? HCF : HCN;

        XN = (P5 + P4 * HC - P2 * pow(XF, 4)) / (100 + P3 * HC);
        N++;
    } while (abs(XN - XF) > EPS && N < 150);

    // Final surface temperature of clothing
    TCL = 100 * XN - 273;

    // Heat loss components
    HL1 = 3.05 * 0.001 * (5733 - 6.99 * MW - PA);
    HL2 = (MW > 58.15) ? 0.42 * (MW - 58.15) : 0;
    HL3 = 1.7 * 0.00001 * M * (5867 - PA);
    HL4 = 0.0014 * M * (34 - TA);
    HL5 = 3.96 * FCL * (pow(XN, 4) - pow(TRA / 100, 4));
    HL6 = FCL * HC * (TCL - TA);

    // PMV and PPD calculations
    double TS = 0.303 * exp(-0.036 * M) + 0.028;
    PMV = TS * (MW - HL1 - HL2 - HL3 - HL4 - HL5 - HL6);
    PPD = 100 - 95 * exp(-0.03353 * pow(PMV, 4) - 0.2179 * pow(PMV, 2));

    // Output
    cout << "Predicted Mean Vote (PMV): " << PMV << endl;
    cout << "Predicted Percentage of Dissatisfied (PPD): " << PPD << "%" << endl;

    return 0;
}

