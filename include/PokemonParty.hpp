#ifndef POKEMONPARTY_HPP
#define POKEMONPARTY_HPP

#include "PokemonVector.hpp"
#include "Pokedex.hpp"
#include <vector>
#include <string>
#include <iostream>

class PokemonParty : public PokemonVector {
private:
    static const std::size_t MAX_PARTY_SIZE = 30; 
    std::vector<Pokemon> data;

public:
    void add(const Pokemon& p) override {
        if (data.size() == MAX_PARTY_SIZE){
            std::cout << "Votre partie a atteint sa limite (30 pokés)." << std::endl;
            return;
        } 
        data.push_back(p);
    }

    void removeAt(std::size_t index) override {
        if (index < data.size()) {
            data.erase(data.begin() + static_cast<long>(index));
        }
    }

    std::size_t size() const override { 
        return data.size(); 
    }

    const Pokemon& at(std::size_t index) const override {
         return data[index]; 
    }

    Pokemon& at(std::size_t index) override { 
        return data[index]; 
    }

    void displayAll() const override {
        std::cout << "--- Votre équipe (" << data.size() << "/" << MAX_PARTY_SIZE << ") ---\n";
        for (std::size_t i = 0; i < data.size(); ++i) {
            data[i].displayInfo();
        }
    }

    // helpers
    static std::size_t capacity() { return MAX_PARTY_SIZE; }
    bool isFull() const { return data.size() >= MAX_PARTY_SIZE; }

    // add pokes from pokedex (clones)
    bool addFromDexById(int id) {
        Pokemon tmp(0, "", 0, 0, 0, 0, 0);
        if (Pokedex::instance().cloneById(id, tmp)) {
            data.push_back(tmp);
            return true;
        }
        return false;
    }

    bool addFromDexByName(const std::string& name) {
        Pokemon tmp(0, "", 0, 0, 0, 0, 0);
        if (Pokedex::instance().cloneByName(name, tmp)) {
            data.push_back(tmp);
            return true;
        }
        return false;
    }
};

#endif 
