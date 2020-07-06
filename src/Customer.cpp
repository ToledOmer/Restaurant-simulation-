//
// Created by yuvalman@wincs.cs.bgu.ac.il on 11/5/18.
//
//


#include "../include/Customer.h"

//constructor
Customer::Customer(std::string c_name, int c_id):name(c_name),id(c_id) {

}

std::string Customer::getName() const {
    return name;
}

int Customer::getId() const {
    return id;
}

Customer::~Customer() {

}


VegetarianCustomer::VegetarianCustomer(std::string name, int id) : Customer(name, id) {

}
//order veg with small id, and most expensive bvg
std::vector<int> VegetarianCustomer::order(const std::vector<Dish> &menu) {
    std::vector<int> selectedDishesId;
    int vegMinId = -1;
    int bvgMaxPrice = -1;
    int bvgMinId = -1;

    for (size_t i = 0; i < menu.size(); ++i) {
        Dish dish = menu[i];
        //check veg min id
        if (dish.getType() == VEG){
            if (vegMinId == -1){
                vegMinId = dish.getId();
            }
        }
        //check most expensive bvg
        if (dish.getType() == BVG){
            if (bvgMaxPrice == -1){
                bvgMaxPrice = dish.getPrice();
                bvgMinId = dish.getId();
            }
            if (dish.getPrice() > bvgMaxPrice){
                bvgMaxPrice = dish.getPrice();
                bvgMinId = dish.getId();
            }
            //if price is equal, decide by small id
            if (dish.getPrice() == bvgMaxPrice){
                if (dish.getId() < bvgMinId)
                    bvgMinId = dish.getId();
            }
        }
    }
    //check if customer order something
    if (bvgMinId!=-1 || vegMinId!=-1){
        selectedDishesId.push_back(vegMinId);
        selectedDishesId.push_back(bvgMinId);
    }
    return selectedDishesId;
}

std::string VegetarianCustomer::toString() const {
    return getName() + ",veg ";
}

Customer *VegetarianCustomer::clone() {
    return new VegetarianCustomer(*this);
}

CheapCustomer::CheapCustomer(std::string name, int id) : Customer(name, id), hasOrdered(false) {

}


//order the cheapsest dish only once
std::vector<int> CheapCustomer::order(const std::vector<Dish> &menu) {
    std::vector<int> selectedDishesId;
    //cheap customer has to order only once
    if (!hasOrderedBefore()){
        int cheapId = -1;
        int cheapPrice = -1;
        for (size_t i = 0; i < menu.size(); ++i) {
            Dish dish = menu[i];
            if (cheapPrice == -1){
                cheapPrice = dish.getPrice();
                cheapId = dish.getId();
            }
            if (dish.getPrice() < cheapPrice){
                cheapPrice = dish.getPrice();
                cheapId = dish.getId();
            }
            if (dish.getPrice() == cheapPrice){
                if (dish.getId() < cheapId)
                    cheapId = dish.getId();
            }
        }
        if (cheapId!=-1){
            selectedDishesId.push_back(cheapId);
            hasOrdered = true;
        }
    }
    return selectedDishesId;
}

bool CheapCustomer::hasOrderedBefore() {
    return hasOrdered;
}

std::string CheapCustomer::toString() const {
    return getName() + ",chp ";
}
Customer *CheapCustomer::clone() {
    return new CheapCustomer(*this);
}
//may be BUG because of adding if(!hasOrdered) in the start
SpicyCustomer::SpicyCustomer(std::string name, int id) : Customer(name, id),hasOrdered(false) {

}
//order the most expensive spc in the menu(if he didn't ordered before), else order cheapest bvg
std::vector<int> SpicyCustomer::order(const std::vector<Dish> &menu) {
    std::vector<int> selectedDishesId;
    //order the most expensive spc in the menu(if he didn't ordered before)
    if (!hasOrderedBefore()){
        int spcMaxPrice = -1;
        int spcMinId = -1;
        for (size_t i = 0; i < menu.size(); ++i) {
            Dish dish = menu[i];
            //check most expensive spc
            if (dish.getType() == SPC){
                if (spcMaxPrice == -1){
                    spcMaxPrice = dish.getPrice();
                    spcMinId = dish.getId();
                }
                if (dish.getPrice() > spcMaxPrice){
                    spcMaxPrice = dish.getPrice();
                    spcMinId = dish.getId();
                }
                //if price is equal, decide by small id
                if (dish.getPrice() == spcMaxPrice){
                    if (dish.getId() < spcMinId)
                        spcMinId = dish.getId();
                }
            }
        }
        //check if customer order something
        if (spcMinId!=-1){
            selectedDishesId.push_back(spcMinId);
            hasOrdered = true;

        }
    }
    //order the cheapest bvg(he can order the same bvg)
    else {
        int cheapId = -1;
        int cheapPrice = -1;
        for (size_t i = 0; i < menu.size(); ++i) {
            Dish dish = menu[i];
            if (dish.getType() == BVG){
                if (cheapPrice == -1){
                    cheapPrice = dish.getPrice();
                    cheapId = dish.getId();
                }
                if (dish.getPrice() < cheapPrice){
                    cheapPrice = dish.getPrice();
                    cheapId = dish.getId();
                }
                if (dish.getPrice() == cheapPrice){
                    if (dish.getId() < cheapId)
                        cheapId = dish.getId();
                }
            }
        }
        //check if customer order something
        if (cheapId!=-1)
            selectedDishesId.push_back(cheapId);
    }
    return selectedDishesId;
}

bool SpicyCustomer::hasOrderedBefore() {
    return hasOrdered;
}

std::string SpicyCustomer::toString() const {
    return getName() + ",spc ";

}

Customer *SpicyCustomer::clone() {
    return new SpicyCustomer(*this);
}

AlchoholicCustomer::AlchoholicCustomer(std::string name, int id):
Customer(name, id),hasOrdered() {

}

//order the cheapest alc. every time he order the next cheapest alc
std::vector<int> AlchoholicCustomer::order(const std::vector<Dish> &menu) {
    std::vector<int> selectedDishesId;
    int cheapId = -1;
    int cheapPrice = -1;
    for (size_t i = 0; i < menu.size(); ++i) {
        Dish dish = menu[i];
        if (dish.getType() == ALC){
            if (!hasOrderedBefore(dish.getId())){
                if (cheapPrice == -1){
                    cheapPrice = dish.getPrice();
                    cheapId = dish.getId();
                }
                if (dish.getPrice() < cheapPrice){
                    cheapPrice = dish.getPrice();
                    cheapId = dish.getId();
                }
                if (dish.getPrice() == cheapPrice){
                    if (dish.getId() < cheapId)
                        cheapId = dish.getId();
                }
            }
        }
    }
    //check if customer order something and add it to the vectors: selectedDishesId and hasOrdered
    if (cheapId!=-1){
        selectedDishesId.push_back(cheapId);
        hasOrdered.push_back(cheapId);
    }
    return selectedDishesId;
}
//check if the dish id is in hasOrdered vector
bool AlchoholicCustomer::hasOrderedBefore(int dishId) {
    for (size_t i = 0; i <hasOrdered.size() ; ++i) {
        if (hasOrdered[i]==dishId)
            return true;
    }
    return false;
}

std::string AlchoholicCustomer::toString() const {
    return getName() + ",alc ";
}
Customer *AlchoholicCustomer::clone() {
    return new AlchoholicCustomer(*this);
}
