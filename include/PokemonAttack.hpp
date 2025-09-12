#ifndef POKEMONATTACK_HPP
#define POKEMONATTACK_HPP

#include "PokemonVector.hpp"
#include "PokemonParty.hpp"
#include <vector>
#include <iostream>

class PokemonAttack : public PokemonVector {
private:
    static const std::size_t MAX_TEAM_SIZE = 6; //limit to 6
    std::vector<Pokemon> data;

public:
    void add(const Pokemon& p) override {
        if (data.size() >= MAX_TEAM_SIZE) {
            std::cout << "Votre equipe est pleine (6 max).\n";
            return;
        }
        data.push_back(p);
    }

    // remove by index, ignore if out of bounds
    void removeAt(std::size_t index) override {
        if (index >= data.size()) return;
        data.erase(data.begin() + static_cast<long>(index));
    }

    std::size_t size() const override {
        return data.size();
    }

    // read
    const Pokemon& at(std::size_t index) const override {
        return data[index];
    }

    // write
    Pokemon& at(std::size_t index) override {
        return data[index];
    }

    // display all attack party
    void displayAll() const override {
        std::cout << "--- Ma équipe (" << data.size()
                  << "/" << MAX_TEAM_SIZE << ") ---\n";
        for (std::size_t i = 0; i < data.size(); ++i) {
            data[i].displayInfo();
        }
        std::cout << "-----------------------------------------\n";
    }

    // helpers (not used yet)

    static std::size_t capacity() { return MAX_TEAM_SIZE; }
    bool isFull()  const { return data.size() >= MAX_TEAM_SIZE; }
    bool isEmpty() const { return data.empty(); }
    void clear()         { data.clear(); }

    // move poke from party to attack; true if success, fralse if fail
    bool addFromParty(PokemonParty& party, std::size_t partyIndex) {
        if (isFull()) return false;
        if (partyIndex >= party.size()) return false;

        add(party.at(partyIndex));   
        party.removeAt(partyIndex);  
        return true;
    }

    // move many from party
    std::size_t addManyFromParty(PokemonParty& party, const std::vector<std::size_t>& indices) {
        if (indices.empty()) return 0;

        std::size_t added = 0;

        for (std::size_t k = indices.size(); k > 0; --k) {
            if (isFull()) break;

            std::size_t idx = indices[k - 1];
            if (idx >= party.size()) continue;  

            add(party.at(idx));
            party.removeAt(idx);
            ++added;

            if (isFull()) break;
        }
        return added;
    }

    // move from attack to party
    bool returnToParty(PokemonParty& party, std::size_t attackIndex) {
        if (attackIndex >= data.size()) return false;
        party.add(data[attackIndex]);
        removeAt(attackIndex);
        return true;
    }

    // move all from attack to party
    void returnAllToParty(PokemonParty& party) {
        for (std::size_t i = 0; i < data.size(); ++i) {
            party.add(data[i]);
        }
        data.clear();
    }
};

#endif 
