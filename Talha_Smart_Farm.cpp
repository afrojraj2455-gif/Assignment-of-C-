#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <iomanip>

using namespace std;

// Abstract Class Advisory
class Advisory {
public:
    virtual void giveAdvice() const = 0;
    virtual ~Advisory() {}
};

// Base Class AgriculturalCrop
class AgriculturalCrop : public Advisory {
protected:
    string cropTitle;
    double plotArea;
    string plantingSeason;
    double baseYieldRate;
    static int instanceCounter;

public:
    // Constructor Overloading
    AgriculturalCrop() : cropTitle("Default"), plotArea(1.0), plantingSeason("Spring"), baseYieldRate(5.0) {
        instanceCounter++;
    }

    AgriculturalCrop(string title, double area, string season, double rate) {
        if (area <= 0 || rate < 0) {
            throw out_of_range("Range Error: Plot area must be positive and yield rate cannot be negative.");
        }
        cropTitle = title;
        plotArea = area;
        plantingSeason = season;
        baseYieldRate = rate;
        instanceCounter++;
    }

    virtual ~AgriculturalCrop() {
        instanceCounter--;
    }

    static int getInstanceCount() {
        return instanceCounter;
    }

    // Runtime Polymorphism
    virtual double calculateYield() const {
        return plotArea * baseYieldRate;
    }

    virtual void displayInfo() const {
        cout << left << setw(15) << cropTitle 
             << setw(10) << plotArea 
             << setw(12) << plantingSeason 
             << setw(15) << baseYieldRate 
             << setw(15) << calculateYield() << endl;
    }

    // Operator Overloading (== operator to check equal yields)
    bool operator==(const AgriculturalCrop& other) const {
        return this->calculateYield() == other.calculateYield();
    }

    virtual void serialize(ofstream& file) const {
        file << cropTitle << "," << plotArea << "," << plantingSeason << "," << baseYieldRate << ",";
    }

    string getTitle() const { return cropTitle; }
};

int AgriculturalCrop::instanceCounter = 0;

// Derived Class 1: CerealCrop
class CerealCrop : public AgriculturalCrop {
private:
    string grainType;
    double proteinContent; // percentage

public:
    CerealCrop() : AgriculturalCrop(), grainType("Wheat"), proteinContent(12.0) {}

    CerealCrop(string t, double a, string s, double r, string g, double p) : AgriculturalCrop(t, a, s, r) {
        if (p < 0 || p > 100) throw out_of_range("Range Error: Protein content must be between 0 and 100.");
        grainType = g;
        proteinContent = p;
    }

    double calculateYield() const override {
        return plotArea * baseYieldRate;
    }

    void displayInfo() const override {
        cout << "[Cereal]    ";
        AgriculturalCrop::displayInfo();
        cout << "            -> Grain: " << grainType << ", Protein: " << proteinContent << "%\n";
    }

    void giveAdvice() const override {
        cout << "            -> Advisory: Apply split nitrogen doses to boost protein content.\n";
    }

    void serialize(ofstream& file) const override {
        file << "CEREAL," << cropTitle << "," << plotArea << "," << plantingSeason << "," << baseYieldRate << "," << grainType << "," << proteinContent << "\n";
    }
};

// Derived Class 2: VegetableCrop
class VegetableCrop : public AgriculturalCrop {
private:
    string shelfLife;
    bool organicStatus;

public:
    VegetableCrop() : AgriculturalCrop(), shelfLife("Short"), organicStatus(true) {}

    VegetableCrop(string t, double a, string s, double r, string sl, bool org) : AgriculturalCrop(t, a, s, r) {
        shelfLife = sl;
        organicStatus = org;
    }

    double calculateYield() const override {
        return plotArea * baseYieldRate * 1.08; // Slight optimization factor
    }

    void displayInfo() const override {
        cout << "[Vegetable] ";
        AgriculturalCrop::displayInfo();
        cout << "            -> Shelf Life: " << shelfLife << ", Organic: " << (organicStatus ? "Yes" : "No") << "\n";
    }

    void giveAdvice() const override {
        cout << "            -> Advisory: Harvest early in the morning to preserve crispness and shelf life.\n";
    }

    void serialize(ofstream& file) const override {
        file << "VEGETABLE," << cropTitle << "," << plotArea << "," << plantingSeason << "," << baseYieldRate << "," << shelfLife << "," << organicStatus << "\n";
    }
};

int main() {
    vector<AgriculturalCrop*> registry;
    int selection;

    do {
        cout << "\n============================================\n";
        cout << "    FARM AUTOMATION SYSTEM (STUDENT 3)\n";
        cout << "============================================\n";
        cout << "1. Add Cereal Crop\n";
        cout << "2. Add Vegetable Crop\n";
        cout << "3. Display All Crops & Tips\n";
        cout << "4. Compare Two Crops Yields (== Operator)\n";
        cout << "5. Save Inventory to File\n";
        cout << "6. Check Active Instance Count (Static)\n";
        cout << "7. Exit\n";
        cout << "Make your selection: ";

        if (!(cin >> selection)) {
            cout << "Invalid choice entry.\n";
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        try {
            if (selection == 1) {
                string title, season, grain;
                double area, rate, protein;
                cout << "Crop Title: "; cin >> title;
                cout << "Plot Area: "; cin >> area;
                cout << "Season: "; cin >> season;
                cout << "Base Yield Rate: "; cin >> rate;
                cout << "Grain Type: "; cin >> grain;
                cout << "Protein Content (%): "; cin >> protein;

                registry.push_back(new CerealCrop(title, area, season, rate, grain, protein));
                cout << "Cereal crop added successfully!\n";
            }
            else if (selection == 2) {
                string title, season, shelf;
                double area, rate;
                char orgChar;
                cout << "Crop Title: "; cin >> title;
                cout << "Plot Area: "; cin >> area;
                cout << "Season: "; cin >> season;
                cout << "Base Yield Rate: "; cin >> rate;
                cout << "Shelf Life: "; cin >> shelf;
                cout << "Organic Status (y/n): "; cin >> orgChar;

                bool org = (orgChar == 'y' || orgChar == 'Y');
                registry.push_back(new VegetableCrop(title, area, season, rate, shelf, org));
                cout << "Vegetable crop added successfully!\n";
            }
            else if (selection == 3) {
                if (registry.empty()) {
                    cout << "Inventory is empty.\n";
                } else {
                    cout << "\n----------------------------------------------------------------------\n";
                    cout << left << setw(15) << "Category" << setw(15) << "Title" << setw(10) << "Area" << setw(12) << "Season" << setw(15) << "Rate" << setw(12) << "Total" << endl;
                    cout << "----------------------------------------------------------------------\n";
                    for (size_t i = 0; i < registry.size(); ++i) {
                        cout << i + 1 << ". ";
                        registry[i]->displayInfo();
                        registry[i]->giveAdvice();
                        cout << "----------------------------------------------------------------------\n";
                    }
                }
            }
            else if (registry.size() >= 2 && selection == 4) {
                int firstIdx, secondIdx;
                cout << "Enter first index (1-" << registry.size() << "): "; cin >> firstIdx;
                cout << "Enter second index (1-" << registry.size() << "): "; cin >> secondIdx;
                if (firstIdx >= 1 && firstIdx <= registry.size() && secondIdx >= 1 && secondIdx <= registry.size()) {
                    if (*registry[firstIdx - 1] == *registry[secondIdx - 1]) {
                        cout << "Both crops have EQUAL total yields.\n";
                    } else {
                        cout << "The crops have DIFFERENT total yields.\n";
                    }
                } else {
                    cout << "Out of bounds indices.\n";
                }
            }
            else if (selection == 5) {
                ofstream outStream("crop_inventory_3.txt");
                for (auto item : registry) {
                    item->serialize(outStream);
                }
                outStream.close();
                cout << "Inventory saved to 'crop_inventory_3.txt'.\n";
            }
            else if (selection == 6) {
                cout << "Total Active Instances: " << AgriculturalCrop::getInstanceCount() << endl;
            }
        } catch (const exception& ex) {
            cout << ex.what() << endl;
        }

    } while (selection != 7);

    for (auto item : registry) delete item;
    return 0;
}