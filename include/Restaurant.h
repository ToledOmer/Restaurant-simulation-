//
// Created by yuvalman@wincs.cs.bgu.ac.il on 11/4/18.
//

#ifndef RESTAURANT_H_
#define RESTAURANT_H_

#include <vector>
#include <string>
#include<fstream>
#include<sstream>
#include "Dish.h"
#include "Table.h"
#include "Action.h"


class Restaurant{



public:
    Restaurant();
    Restaurant(const std::string &configFilePath);
    virtual ~Restaurant();//Destructor
    void clear(); //Clear
    Restaurant(Restaurant &other);//Copy constructor
    Restaurant(Restaurant &&other);//Move constructor
    Restaurant& operator=(Restaurant &other); //Copy assignment
    Restaurant&operator=(Restaurant &&other);// Move assignment
    void start();
    int getNumOfTables() const;
    Table* getTable(int ind);
    DishType checkDishType(std::string str);
    const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<Dish>& getMenu();
    std::vector<Table*>& getTables();
    std::vector<std::string> splitLine(std::string line, char ch);

private:
    bool open;
    int numOfTables;
    std::vector<Table*> tables;
    std::vector<Dish> menu;
    std::vector<BaseAction*> actionsLog;
};

#endif