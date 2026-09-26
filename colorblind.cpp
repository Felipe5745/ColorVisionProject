#include <iostream>
#include <string>
using namespace std;

double linearize(double channel) {
    double c = channel / 255.0;
    if (c <= 0.04045) {
        return c / 12.92;
    } else {
        return pow((c + 0.055) / 1.055, 2.4);
    }
}

// Calculates standard WCAG Relative Luminance
double calculateLuminance(int r, int g, int b) {
    double rLin = linearize(r);
    double gLin = linearize(g);
    double bLin = linearize(b);

    // Standard WCAG coefficients for RGB
    return 0.2126 * rLin + 0.7152 * gLin + 0.0722 * bLin;
}

// Calculates WCAG Contrast Ratio between two luminance values
double calculateContrastRatio(double l1, double l2) {
    double lighter = max(l1, l2);
    double darker = min(l1, l2);
    return (lighter + 0.05) / (darker + 0.05);
}

// Adjusts for colorblindness
double adjustLuminanceForColorBlindness(double luminance, const string& type) {
  
    if (type == "Protanopia") {
        return luminance * 0.85;
    } else if (type == "Deuteranopia") {
        return luminance * 0.90;
    } else if (type == "Tritanopia") {
        return luminance * 0.95;
    }
    return luminance;
}

// Helper to constrain RGB values between 0 and 255
int clampColor(int value) {
    return max(0, min(255, value));
}
int main() {
    int r1, g1, b1;
    int r2, g2, b2;
    string isColorBlind;
    string blindnessType = "";

    // 1. Prompt for first color
    cout << "Enter RGB values for the first color (0-255 separated by spaces): ";
    cin >> r1 >> g1 >> b1;

    // 2. Prompt for second color
    cout << "Enter RGB values for the second color (0-255 separated by spaces): ";
    cin >> r2 >> g2 >> b2;

    // 3. Ask about color blindness
    cout << "Do you have a form of color blindness? (yes/no): ";
    cin >> isColorBlind;

    if (isColorBlind == "yes" || isColorBlind == "Yes") {
        cout << "Enter your color blindness type (Protanopia / Deuteranopia / Tritanopia): ";
        cin >> blindnessType;
    }

    // 4. Calculate Luminance
    double l1 = calculateLuminance(r1, g1, b1);
    double l2 = calculateLuminance(r2, g2, b2);

    // 5. Adjust for color blindness
    if (!blindnessType.empty()) {
        l1 = adjustLuminanceForColorBlindness(l1, blindnessType);
        l2 = adjustLuminanceForColorBlindness(l2, blindnessType);
    }

    // 6. Calculate Contrast Ratio
    double ratio = calculateContrastRatio(l1, l2);

    cout << "\n-----------------------------------" << endl;
    cout << "Calculated Contrast Ratio: " << ratio << ":1" << endl;

    // 7. 
    if (ratio < 4.5) {
        cout << "Poor color contrast" << endl;
    }
     else {
        cout << "Is good color contrast" << endl;
    }

    return 0;
}
