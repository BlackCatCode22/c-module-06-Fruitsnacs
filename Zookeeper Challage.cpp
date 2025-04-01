// Zookeeper Challange
// Alex Lopez
// 3-31-25

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <iomanip>
using namespace std;

// Base class Animal
class Animal {
protected:
    string species, color, sex, origin, name, uniqueID;
    int age, weight;
    string birthDate;
public:
    Animal(string sp, int a, string s, string c, int w, string o)
        : species(sp), age(a), sex(s), color(c), weight(w), origin(o) {}
    virtual void assignName(const vector<string>& nameList, int index) {
        if (index < nameList.size()) {
            name = nameList[index];
        }
        else {
            name = "Unnamed";
        }
    }
    virtual void generateID(int num) {
        uniqueID = species.substr(0, 2) + (num < 10 ? "0" : "") + to_string(num);
    }
    virtual void printInfo(ofstream& out) {
        out << uniqueID << "; " << name << "; birth date " << birthDate
            << "; " << color << "; " << sex << "; " << weight
            << " pounds; from " << origin << "\n";
    }
};

// Derived classes
class Hyena : public Animal {
public:
    Hyena(int a, string s, string c, int w, string o)
        : Animal("Hyena", a, s, c, w, o) {}
};

class Lion : public Animal {
public:
    Lion(int a, string s, string c, int w, string o)
        : Animal("Lion", a, s, c, w, o) {}
};

class Tiger : public Animal {
public:
    Tiger(int a, string s, string c, int w, string o)
        : Animal("Tiger", a, s, c, w, o) {}
};

class Bear : public Animal {
public:
    Bear(int a, string s, string c, int w, string o)
        : Animal("Bear", a, s, c, w, o) {}
};

int main() {
    ifstream animalFile("arrivingAnimals.txt");
    ifstream nameFile("animalNames.txt");
    ofstream outputFile("zooPopulation.txt");

    if (!animalFile || !nameFile || !outputFile) {
        cerr << "Error opening files." << endl;
        return 1;
    }

    vector<string> hyenaNames, lionNames, tigerNames, bearNames;
    string line, word;
    map<string, vector<string>*> nameMap = {
        {"Hyena", &hyenaNames}, {"Lion", &lionNames},
        {"Tiger", &tigerNames}, {"Bear", &bearNames}
    };

    while (getline(nameFile, line)) {
        if (line.find("Names:") != string::npos) {
            string species = line.substr(0, line.find(" "));
            vector<string>* nameList = nameMap[species];
            while (getline(nameFile, line) && !line.empty()) {
                istringstream ss(line);
                while (getline(ss, word, ',')) {
                    nameList->push_back(word);
                }
            }
        }
    }

    vector<Animal*> zoo;
    map<string, int> speciesCount;

    while (getline(animalFile, line)) {
        istringstream ss(line);
        int age, weight;
        string sex, species, color, origin, birthSeason;
        ss >> age >> word >> word >> sex >> species >> word >> word >> birthSeason;
        getline(ss, color, ',');
        ss >> weight >> word;
        getline(ss, origin);

        cout << "Parsed: Age=" << age << ", Sex=" << sex << ", Species=" << species
            << ", Birth Season=" << birthSeason << ", Color=" << color
            << ", Weight=" << weight << ", Origin=" << origin << endl;

        Animal* animal = nullptr;

        if (species == "Hyena") animal = new Hyena(age, sex, color, weight, origin);
        else if (species == "Lion") animal = new Lion(age, sex, color, weight, origin);
        else if (species == "Tiger") animal = new Tiger(age, sex, color, weight, origin);
        else if (species == "Bear") animal = new Bear(age, sex, color, weight, origin);

        if (animal) {
            speciesCount[species]++;
            if (!nameMap[species]->empty() && speciesCount[species] - 1 < nameMap[species]->size()) {
                animal->assignName(*nameMap[species], speciesCount[species] - 1);
            }
            else {
                animal->assignName({ "Unnamed" }, 0);
            }
            animal->generateID(speciesCount[species]);
            zoo.push_back(animal);
        }
    }

    cout << "Zoo size: " << zoo.size() << endl;

    for (auto animal : zoo) {
        animal->printInfo(outputFile);
        delete animal;
    }

    outputFile.close();
    return 0;
}
