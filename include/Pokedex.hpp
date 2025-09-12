#ifndef POKEDEX_HPP
#define POKEDEX_HPP

#include "PokemonVector.hpp"
#include <vector>
#include <string>

class Pokedex : public PokemonVector {
public:
    // Singleton "Meyers" simple: charger le csv
    static Pokedex& instance(const std::string& csvPath = "pokedex.csv",
                             bool includeMegas = false);

    // Clones (écriture dans le paramètre de sortie et retour bool si succes)
    bool cloneById(int id, Pokemon& out) const;
    bool cloneByName(const std::string& name, Pokemon& out) const;

    // blocage des nouveaux contructeurs
    Pokedex(const Pokedex&) = delete;
    Pokedex& operator=(const Pokedex&) = delete;

private:
    Pokedex(); // pas utilisé
    explicit Pokedex(const std::string& csvPath, bool includeMegas);

    void loadCsv(const std::string& csvPath, bool includeMegas);

    std::vector<Pokemon> data;

    // PokemonVector
    void add(const Pokemon& p) override;
    void removeAt(std::size_t index) override;
    std::size_t size() const override;
    const Pokemon& at(std::size_t index) const override;
    Pokemon& at(std::size_t index) override;
    void displayAll() const override;
};

#endif // POKEDEX_HPP
