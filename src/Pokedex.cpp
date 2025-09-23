#include "Pokedex.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>

// Singleton 
Pokedex& Pokedex::instance(const std::string& csvPath, bool includeMegas) {
    static Pokedex dex(csvPath, includeMegas); 
    return dex;
}

Pokedex::Pokedex(const std::string& csvPath, bool includeMegas) {
    loadCsv(csvPath, includeMegas);
}

// helper: remove spaces 
static void trimSimple(std::string& s) {
    // left
    std::size_t i = 0;
    while (i < s.size() && std::isspace(static_cast<unsigned char>(s[i]))) i++;
    s.erase(0, i);
    // right
    if (!s.empty()) {
        std::size_t j = s.size() - 1;
        while (j < s.size() && std::isspace(static_cast<unsigned char>(s[j]))) {
            if (j == 0) { s.clear(); break; }
            j--;
        }
        if (!s.empty()) s.erase(j + 1);
    }
}

void Pokedex::loadCsv(const std::string& csvPath, bool includeMegas) {
    std::ifstream in(csvPath.c_str());
    if (!in.is_open()) {
        throw std::runtime_error("Le fichier CSV ne pu pas être ouvert: " + csvPath);
    }

    std::string line;
    bool firstLine = true;

    while (std::getline(in, line)) {
        if (line.empty()) continue;

        // divide with commas
        std::stringstream ss(line);
        std::string cell;
        std::string cols[13]; 
        int count = 0;
        while (std::getline(ss, cell, ',')) {
            if (count < 13) {
                trimSimple(cell);
                cols[count] = cell;
            }
            count++;
        }

        if (count < 13) {
            // invalid line
            continue;
        }

        // jump header
        if (firstLine) {
            firstLine = false;
            if (cols[0].empty() || (cols[0][0] < '0' || cols[0][0] > '9')) {
                continue; 
            }
        }

        // filter out Megas
        // name in cols[1]
        if (!includeMegas) {
            if (cols[1].find("Mega ") != std::string::npos) {
                continue;
            }
        }

        // converts fields that will be used
        try {
            int    id    = std::stoi(cols[0]); // "#"
            std::string name = cols[1];        // "Name"
            double hp    = std::stod(cols[5]); // "HP"
            double atk   = std::stod(cols[6]); // "Attack"
            double def   = std::stod(cols[7]); // "Defense"
            double evol  = std::stod(cols[11]); // "Generation" -> evolution

            // currHP = maxHP at start
            Pokemon p(id, name, hp, hp, atk, def, evol);
            data.push_back(p);
        } catch (...) {
            // skip if there is an error in the line
            continue;
        }
    }
}

bool Pokedex::cloneById(int id, Pokemon& out) const {
    for (std::size_t i = 0; i < data.size(); ++i) {
        if (data[i].getId() == id) {
            out = data[i]; 
            return true;
        }
    }
    return false;
}

bool Pokedex::cloneByName(const std::string& name, Pokemon& out) const {
    for (std::size_t i = 0; i < data.size(); ++i) {
        if (data[i].getName() == name) {
            out = data[i]; 
            return true;
        }
    }
    return false;
}

void Pokedex::add(const Pokemon& p) { data.push_back(p); }

void Pokedex::removeAt(std::size_t index) {
    if (index >= data.size()) return;
    data.erase(data.begin() + static_cast<long>(index));
}

std::size_t Pokedex::size() const { return data.size(); }

const Pokemon& Pokedex::at(std::size_t index) const {
    return data[index];
}

Pokemon& Pokedex::at(std::size_t index) {
    return data[index];
}

void Pokedex::displayAll() const {
    for (std::size_t i = 0; i < data.size(); ++i) {
        data[i].displayInfo();
    }
}
