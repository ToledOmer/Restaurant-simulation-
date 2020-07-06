//
// Created by toled on 05/11/2018.
//

#ifndef SPL_DISH_H
#define SPL_DISH_H

#include <string>

enum DishType{
    VEG, SPC, BVG, ALC
};

class Dish{
public:
    Dish(int d_id, std::string d_name, int d_price, DishType d_type);
    int getId() const;
    std::string getName() const;
    int getPrice() const;
    DishType getType() const;
private:
    const int id;
    const std::string name;
    const int price;
    const DishType type;
};


#endif