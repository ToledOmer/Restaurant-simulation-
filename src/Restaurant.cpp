// Created by yuvalman@wincs.cs.bgu.ac.il on 11/4/18.
//


#include "../include/Restaurant.h"


//constructor
Restaurant::Restaurant():
open(false),numOfTables(0),tables(),menu(),actionsLog()
{}
Restaurant::Restaurant(const std::string &configFilePath):
open(false),numOfTables(0),tables(),menu(),actionsLog()
{
    std::ifstream inFile(configFilePath);
    std::string line;
    //fileCount go over the 3 stages in the file(num of tables, num of customer in tables, menu)
    int fileCount = 0;
    int dishId = 0;
    while (std::getline(inFile,line)){
        if (!(line=="\r"||line[0]=='#'||line=="")){
            //set num of tables
            if (fileCount==0){
                numOfTables=stoi(line);
            }
            //create tables pointers vector
            if (fileCount==1){
                std::vector<std::string> tablesLine = splitLine(line,',');
                for (size_t i = 0; i <tablesLine.size() ; ++i) {
                    Table* tablePtr = new Table(stoi(tablesLine[i]));
                    tables.push_back(tablePtr);
                }
            }
            //create Menu vector
            if (fileCount>=2){
                std::vector<std::string> dishLine = splitLine(line,',');
                std::string dishName = dishLine[0];
                DishType dishType = checkDishType(dishLine[1]);
                int dishPrice = stoi(dishLine[2]);
                Dish dish = Dish(dishId, dishName,dishPrice,dishType);
                menu.push_back(dish);
                dishId++;
            }
            fileCount++;
        }
    }
}


//Destructor
Restaurant::~Restaurant() {
  clear();
}

//Clear
void Restaurant::clear(){
    for (size_t i = 0; i <tables.size() ; ++i) {
        delete(tables[i]);
        tables[i]= nullptr;
    }
    this->tables.clear();
    for (size_t j = 0; j < actionsLog.size(); ++j) {
        delete(actionsLog[j]);
        actionsLog[j]=nullptr;
    }
    this->actionsLog.clear();
    this->open = false;
    this->numOfTables = 0;
}

//Copy constructor

Restaurant::Restaurant(Restaurant &other):
open(other.open), numOfTables(other.getNumOfTables()),tables(),menu(),actionsLog()
{
    for (size_t i = 0; i <other.getTables().size() ; ++i) {
        tables.push_back(new Table(*other.tables[i]));
    }
    for (size_t j = 0; j < other.getActionsLog().size(); ++j) {
        actionsLog.push_back((other.getActionsLog()[j]->clone()));
    }
    for (size_t k = 0; k <other.getMenu().size() ; ++k) {
        menu.push_back(other.getMenu()[k]);
    }
}

//Move constructor

Restaurant::Restaurant(Restaurant &&other):
open(other.open), numOfTables(other.getNumOfTables()),tables(),menu(),actionsLog() {
    other.open = false;
    other.numOfTables=0;
    for (size_t i = 0; i <other.getTables().size() ; ++i) {
        tables.push_back(new Table (*other.tables[i]));
    }
    other.tables.clear();
    for (size_t j = 0; j < other.getActionsLog().size(); ++j) {
        actionsLog.push_back((other.getActionsLog()[j]->clone()));
    }
    other.actionsLog.clear();
    for (size_t k = 0; k <other.getMenu().size() ; ++k) {
        menu.push_back(other.getMenu()[k]);
        //don't need delete because Dish is place in stack
    }
    other.getMenu().clear();
    other.clear();
}


//copy assignment

Restaurant& Restaurant::operator=(Restaurant &other) {
    //will lose data ig not using if
    if (this != &other){
        this->clear();
        for (size_t i = 0; i <other.getTables().size() ; ++i) {
            tables.push_back(new Table(*other.tables[i]));
        }
        for (size_t j = 0; j < other.getActionsLog().size(); ++j) {
            actionsLog.push_back((other.getActionsLog()[j]->clone()));
        }
        for (size_t k = 0; k <other.getMenu().size() ; ++k) {
            menu.push_back(other.getMenu()[k]);
        }
        open = other.open;
        numOfTables = other.numOfTables;
    }
    return *this;
}

//move assignment

Restaurant &Restaurant::operator=(Restaurant &&other) {
    if (this != &other){
        this->clear();
        for (size_t i = 0; i <getTables().size() ; ++i) {
            tables.push_back(new Table(*other.tables[i]));
        }
        for (size_t j = 0; j < other.getActionsLog().size(); ++j) {
            actionsLog.push_back((other.getActionsLog()[j]->clone()));
        }
        for (size_t k = 0; k <other.getMenu().size() ; ++k) {
            menu.push_back(other.getMenu()[k]);
        }
        open = other.open;
        numOfTables = other.numOfTables;
        other.clear();
        other.getMenu().clear();
    }
    return *this;
}

DishType Restaurant::checkDishType(std::string str) {
    DishType dishType;
    if(str=="VEG")
        dishType = VEG;
    if (str=="SPC")
        dishType = SPC;
    if (str=="BVG")
        dishType = BVG;
    if (str=="ALC")
        dishType = ALC;
    return dishType;

}

int Restaurant::getNumOfTables() const {
    return numOfTables;
}

//split line by the char "ch"
std::vector<std::string> Restaurant::splitLine(std::string line, char ch) {
    std::stringstream streamLine(line);
    std::string segment;
    std::vector<std::string> lineVec;
    while(std::getline(streamLine, segment, ','))
    {
        lineVec.push_back(segment);
    }
    return lineVec;
}

std::vector<Dish> &Restaurant::getMenu() {
    return menu;
}

Table *Restaurant::getTable(int ind) {
    return tables[ind];
}

std::vector<Table *> &Restaurant::getTables() {
    return tables;
}
const std::vector<BaseAction *> &Restaurant::getActionsLog() const {
    return actionsLog;
}


void Restaurant::start() {
    std::cout << "Restaurant is now open!" << std::endl;
    int cusid = 0;
    std::string command;
    open = true;
    while (open) {  //while
        std::cin >> command; //receive the first word which command
        if (command == "open") {
            int tableid;  //init table id
            std::cin >> tableid; //receive table id
            std::vector<Customer *> customers;
            std::string customerstre;
            getline(std::cin,customerstre);
            customerstre = customerstre.substr(1);
            int pointer = customerstre.find(","); // init the ponter that helps sorting the customer and the stretegy
            while (pointer >0) {//
                std::size_t ps = customerstre.find(",");      // position of "," in customer
                std::string customer = customerstre.substr(0, ps); //get the customer name
                customerstre = customerstre.substr(ps+1);
                std::size_t re = customerstre.find(" ");      //postion of " "
                std::string strategy = customerstre.substr(0, re); //get the strategy
                customerstre = customerstre.substr(re + 1);  //receive the next customers
                if (strategy == "veg") { //checks if the customers is vegen
                    customers.push_back(new VegetarianCustomer(customer, cusid));
                    cusid++;
                    pointer = customerstre.find(","); //update the pointer

                }
                if (strategy == "chp") { //checks if the customers is cheap
                    customers.push_back(new CheapCustomer(customer, cusid));
                    cusid++;
                    pointer = customerstre.find(",");//update the pointer

                }
                if (strategy == "spc") {  //checks if the customers is "spicy"
                    customers.push_back(new SpicyCustomer(customer, cusid));
                    cusid++;
                    pointer = customerstre.find(",");//update the pointer

                }
                if (strategy == "alc") {   //checks if the customers is alcoholic
                    customers.push_back(new AlchoholicCustomer(customer, cusid));
                    cusid++;
                    pointer = customerstre.find(",");//update the pointer
                }
            }
            OpenTable *actopen = new OpenTable(tableid, customers);
            actopen->act(*this);
            actionsLog.push_back(actopen);
        }
        if (command == "order") {  //order command
            int tableid;
            std::cin >> tableid; //receive table id
            Order *actorder = new Order(tableid);
            actorder->act(*this);
            actionsLog.push_back(actorder);

        }
        if (command == "move") {
            int origin;  //init origin
            int destination; //init destination
            int customerid; //init customer id
            std::cin >> origin;     //receive origin
            std::cin >> destination; //receive destination
            std::cin >> customerid; //receive customer id

            MoveCustomer *actmove = new MoveCustomer(origin, destination, customerid);
            actmove->act(*this);
            actionsLog.push_back(actmove);
        }
        if (command == "close") { //close command
            int tableid;
            std::cin >> tableid; //receive table id
            Close *actclose = new Close(tableid); //use "*"?
            actclose->act(*this);
            actionsLog.push_back(actclose);

        }
        if (command == "closeall") { //close all command
            CloseAll *clsall = new CloseAll();
            clsall->act(*this);
            actionsLog.push_back(clsall);
           open = false;

        }
        if (command == "menu") {  //Print menu command
            PrintMenu *printm = new PrintMenu;
            printm->act(*this);
            actionsLog.push_back(printm);
        }
        if (command == "status") {  //Print status command
            int tableid;
            std::cin >> tableid; //receive table id
            PrintTableStatus *stat = new PrintTableStatus(tableid);
            stat->act(*this);
            actionsLog.push_back(stat);
        }
        if (command == "log") { //Print log action command
            PrintActionsLog *pal = new PrintActionsLog;
            pal->act(*this);
            actionsLog.push_back(pal);
        }
        if (command == "backup") {  //Backup command
            BackupRestaurant *backup = new BackupRestaurant;
            backup->act(*this);
            actionsLog.push_back(backup);
        }
        if (command == "restore") {  //restore command
            RestoreResturant *restore = new RestoreResturant;
            restore->act(*this);
            actionsLog.push_back(restore);
        }

    }
}

