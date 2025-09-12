#ifndef POKEMON_HPP
#define POKEMON_HPP

#include <iostream>
#include <string>

using std::string;

class Pokemon {
private:
    int id;
    string name;
    double maxHitPoint;
    double currHitPoint;
    double attack;
    double defense;
    double evolution;   

public:
    Pokemon(int id,
            const string& name,
            double maxHP,
            double currHP,
            double attack,
            double defense,
            double evolution = 0.0)  
        : id(id),
          name(name),
          maxHitPoint(maxHP),
          currHitPoint(currHP),
          attack(attack),
          defense(defense),
          evolution(evolution)
    {}

    Pokemon(const Pokemon& other)
        : id(other.id),
          name(other.name),
          maxHitPoint(other.maxHitPoint),
          currHitPoint(other.currHitPoint),
          attack(other.attack),
          defense(other.defense),
          evolution(other.evolution)
    {
        //std::cout << "Constructeur de copie appelé\n\n";
    }

    ~Pokemon() {
        //std::cout << "Destructeur appelé: " << name << " détruit\n\n";
    }

    // Getters
    int getId() const { return id; }
    const string& getName() const { return name; }
    double getMaxHitPoint() const { return maxHitPoint; }
    double getCurrHitPoint() const { return currHitPoint; }
    double getAttack() const { return attack; }
    double getDefense() const { return defense; }
    double getEvolution() const { return evolution; } 

    // Setters
    void setName(const string& n) { name = n; }
    void setMaxHitPoint(double maxhp) { maxHitPoint = maxhp; }
    void setCurrHitPoint(double currhp) { currHitPoint = currhp; }
    void setAttack(double atk) { attack = atk; }
    void setDefense(double def) { defense = def; }
    void setEvolution(double evol) { evolution = evol; }   

    void displayInfo() const {
        std::cout << "ID: " << id << '\n'
                  << "Nom: " << name << '\n'
                  << "HP maximum: " << maxHitPoint << '\n'
                  << "HP actuel: " << currHitPoint << '\n'
                  << "Attaque: " << attack << '\n'
                  << "Défense: " << defense << '\n'
                  << "Evolution: " << evolution << "\n\n";
    }

    void launch_attack(Pokemon& attacked) {
        std::cout << name << " a attaqué " << attacked.name << "!\n";
        if (attack > attacked.defense) {
            double dommage = attack - attacked.defense;
            attacked.currHitPoint -= dommage;   
            if (attacked.currHitPoint < 0) attacked.currHitPoint = 0;
            std::cout << attacked.name << " a perdu " << dommage << " HP!\n";
            if (attacked.currHitPoint <= 0) {
                std::cout << attacked.name << " s'est évanoui!\n";
            }
            std::cout << '\n';
        } else {
            std::cout << "L'attaque de " << name << " n'a pas été assez forte!\n\n";
        }
    }
};

#endif 
