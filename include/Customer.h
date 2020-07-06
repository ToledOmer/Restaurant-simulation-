//
// Created by yuvalman@wincs.cs.bgu.ac.il on 11/5/18.
//

#ifndef ASSIGNMENT1_CUSTOMER_H
#define ASSIGNMENT1_CUSTOMER_H


#include <vector>
#include <string>
#include "Dish.h"

class Customer{
public:
    Customer(std::string c_name, int c_id);
    virtual ~Customer();
    virtual std::vector<int> order(const std::vector<Dish> &menu)=0;
    virtual std::string toString() const = 0;
    virtual Customer* clone()=0;
    std::string getName() const;
    int getId() const;
private:
    const std::string name;
    const int id;
};


class VegetarianCustomer : public Customer {
public:
    VegetarianCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    Customer* clone();
private:
};


class CheapCustomer : public Customer {
public:
    CheapCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    bool hasOrderedBefore();
    Customer* clone();


private:
    bool hasOrdered;
};


class SpicyCustomer : public Customer {
public:
    SpicyCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    bool hasOrderedBefore();
    Customer* clone();

private:
    bool hasOrdered;
};


class AlchoholicCustomer : public Customer {
public:
    AlchoholicCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    bool hasOrderedBefore(int dishId);
    Customer* clone();
private:
    std::vector<int> hasOrdered;
};


#endif