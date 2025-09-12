#ifndef POKEMONVECTOR_HPP
#define POKEMONVECTOR_HPP

#include <iostream>
#include <cstddef>
#include "Pokemon.hpp"

class PokemonVector {
public:

    virtual void add(const Pokemon& p) = 0;
    virtual void removeAt(std::size_t index) = 0;
    virtual std::size_t size() const = 0;
    virtual const Pokemon& at(std::size_t index) const = 0;
    virtual Pokemon& at(std::size_t index) = 0;
    virtual void displayAll() const = 0;

    virtual ~PokemonVector() = default;
};

#endif 

