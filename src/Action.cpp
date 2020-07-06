//
// Created by yuvalman@wincs.cs.bgu.ac.il on 11/11/18.
//


#include "../include/Action.h"
#include "../include/Restaurant.h"


extern Restaurant* backup;


//****maybe should init errorMsg without any string
BaseAction::BaseAction(): errorMsg("") ,status(PENDING) {

}

ActionStatus BaseAction::getStatus() const {
    return status;
}

void BaseAction::complete() {
    status = COMPLETED;

}

void BaseAction::error(std::string errorMsg) {
    status=ERROR;
    this->errorMsg=errorMsg;
    std::cout<<"Error: " + errorMsg<<std::endl;
}

std::string BaseAction::getErrorMsg() const {
    return errorMsg;
}

std::string BaseAction::actionStatusToString(ActionStatus status) const {
    std::string str = "";
    if (status==COMPLETED)
        str = "Completed";
    if (status==ERROR)
        str = "Error: " + getErrorMsg();
    if (status==PENDING)
        str = "Pending";
    return str;
}

BaseAction::~BaseAction() {

}

OpenTable::OpenTable(int id, std::vector<Customer *> &customersList): BaseAction(),
                                                                      tableId(id), customers(customersList), customersToString("")
{}
//destractor -
OpenTable::~OpenTable() {

}
//Open Table
void OpenTable::act(Restaurant &restaurant) {
    if (tableId >= restaurant.getNumOfTables() || restaurant.getTable(tableId)->isOpen()){ //checks if the the table exist or if its open
        for (size_t i = 0; i <customers.size() ; ++i) {
            customersToString.append(customers[i]->toString());
        }
        error("Table does not exist or is already open");
        for (size_t i = 0; i < customers.size(); ++i) {  //delete all the customer that doesn't get into the table
            delete (customers[i]);
            customers[i] = nullptr;}
    }

    else {
        restaurant.getTable(tableId)->openTable();
        for (size_t i = 0; i <customers.size() ; ++i) {
            restaurant.getTable(tableId)->addCustomer(customers[i]);
            customersToString.append(customers[i]->toString());
        }
        complete();
    }
}
//Fix to string
std::string OpenTable::toString() const {
    return "open " + std::to_string(tableId) + " " + getCustomersToString()  + actionStatusToString(getStatus());
}

// Get string of all customer(help for action logs)
std::string OpenTable::getCustomersToString() const {
    return customersToString;
}

BaseAction* OpenTable::clone() {
    return new OpenTable(*this);
}


Order::Order(int id): BaseAction(),tableId(id){

}


void Order::act(Restaurant &restaurant) {

     if (restaurant.getTable(tableId) == nullptr||tableId >= restaurant.getNumOfTables() || !(restaurant.getTable(tableId)->isOpen())) //check if the the table exist and if the table is close/open
        error("Table does not exist or is not open");
    else {
        std::vector<Dish> menu = restaurant.getMenu();
        Table *tablePtr = restaurant.getTable(tableId);
        tablePtr->order(menu);
        complete();
    }
}
//fix to string
std::string Order::toString() const {
    return "order " + std::to_string(tableId) + " " + actionStatusToString(getStatus());
}

BaseAction *Order::clone() {
    return new Order(*this);
}


void MoveCustomer::act(Restaurant &restaurant) {

    Table *srcTablePtr = restaurant.getTable(srcTable);
    Table *dstTablePtr = restaurant.getTable(dstTable);
    if (restaurant.getTable(srcTable) == nullptr || restaurant.getTable(dstTable) == nullptr)
        error("Cannot move customer");
    //check if tables one exist or closed
    else if (srcTable >= restaurant.getNumOfTables() || !(srcTablePtr->isOpen()))
        error("Cannot move customer");
    else if (dstTable >= restaurant.getNumOfTables() || !(dstTablePtr->isOpen()))
        error("Cannot move customer");
    // check if the dstTable has available seats for additional customers
    //****maybe should add <= and not only ==
    else if (dstTablePtr->getCapacity() == (int) dstTablePtr->getCustomers().size())
        error("Cannot move customer");
    //check if customer with the received id is in the srcTable
    else if (srcTablePtr->getCustomer(id) == nullptr)
        error("Cannot move customer");
    else {
        Customer *customer = srcTablePtr->getCustomer(id);
        //remove and add customer between tables
        srcTablePtr->removeCustomer(id);
        dstTablePtr->addCustomer(customer);
        //change bill in srcTable and dstTable(We can't delete const,
        // so we create **copy** of srcOrders clear the real one,
        // and add all the others order pairs)
        std::vector<OrderPair> srcOrders = srcTablePtr->getOrders();
        srcTablePtr->getOrders().clear();
        for (size_t i = 0; i < srcOrders.size(); ++i) {
            if (srcOrders[i].first == id) {
                dstTablePtr->getOrders().push_back(srcOrders[i]);
            }
            if (srcOrders[i].first != id) {
                srcTablePtr->getOrders().push_back(srcOrders[i]);
            }
        }
        if (srcTablePtr->getCustomers().size() == 0) { //close the table if its empty after the move order
            srcTablePtr->closeTable();
        }
        complete();
    }
}
MoveCustomer::MoveCustomer(int src, int dst, int customerId): BaseAction(),
                                                              srcTable(src), dstTable(dst), id(customerId) {

}

std::string MoveCustomer::toString() const {
    return "move " + std::to_string(srcTable) + " " + std::to_string(dstTable) + " " + std::to_string(id) + " " + actionStatusToString(getStatus());
}

BaseAction *MoveCustomer::clone() {
    return new MoveCustomer(*this);
}



PrintMenu::PrintMenu(): BaseAction() {

}

void PrintMenu::act(Restaurant &restaurant) {
    std::vector<Dish> menu = restaurant.getMenu();
    for (size_t i = 0; i < menu.size(); ++i) {
        std::cout<<menu[i].getName() + " " + dishTypeToString(menu[i].getType()) + " "
                   + std::to_string(menu[i].getPrice()) + "NIS"<<std::endl;
    }
    complete();
}
std::string PrintMenu::toString() const {
    return "menu " + actionStatusToString(getStatus());
}

std::string PrintMenu::dishTypeToString(DishType type) {  //turn the dish type into string
    std::string str = "";
    if (type==VEG)
        str = "VEG";
    if (type==SPC)
        str = "SPC";
    if (type==BVG)
        str = "BVG";
    if (type==ALC)
        str = "ALC";
    return str;
}

BaseAction *PrintMenu::clone() {
    return new PrintMenu(*this);
}

PrintActionsLog::PrintActionsLog():BaseAction() {

}
// print the actions logs
void PrintActionsLog::act(Restaurant &restaurant) {
    std::vector<BaseAction*> logs = restaurant.getActionsLog();
    for (size_t i = 0; i < logs.size() ; ++i) {
        std::cout << logs[i]->toString() << std::endl;  //printing each log in the vector of the logs
    }
    complete();
}

std::string PrintActionsLog::toString() const {
    return "log " + actionStatusToString(getStatus());
}

BaseAction *PrintActionsLog::clone() {
    return new PrintActionsLog(*this);
}

Close::Close( int id):BaseAction(),
                      tableId(id)
{}


void Close::act(Restaurant &restaurant) {
    if (restaurant.getTable(tableId) == nullptr ||tableId >= restaurant.getNumOfTables()||
        !restaurant.getTable(tableId)->isOpen()){
        //check if the table is open and if he points at nullptr
        error("Table does not exist or is not open");
    }
    else {
        std::cout << "Table " + std::to_string(tableId) + " was closed. Bill " +
                     std::to_string(restaurant.getTable(tableId)->getBill()) + "NIS" << std::endl;
        restaurant.getTable(tableId)->clear();
        complete();
    }
}

std::string Close::toString() const {
    return "close " + std::to_string(tableId) + " " + actionStatusToString(getStatus()) ;
}

BaseAction *Close::clone() {
    return new Close(*this);
}

CloseAll::CloseAll():BaseAction() {
}

void CloseAll::act(Restaurant &restaurant) {
    for (int i = 0; i <restaurant.getNumOfTables() ; ++i) {
        if ( restaurant.getTable(i) != nullptr &&
             restaurant.getTable(i)->isOpen()){  //checks if the table is not null and if the table is open
            std::cout << "Table " + std::to_string(i) + " was closed. Bill " + std::to_string(restaurant.getTable(i)->getBill()) + "NIS"<<std::endl;  //print that the table got closed and his bill
            restaurant.getTable(i)->closeTable();
        }
    }
    complete();
}

std::string CloseAll::toString() const {

    return "closeall " + actionStatusToString(getStatus()); //never used because i used the close() function instead
}

BaseAction *CloseAll::clone() {
    return new CloseAll(*this);
}

PrintTableStatus::PrintTableStatus(int id): BaseAction(),
                                            tableId(id) //init table id
{}

void PrintTableStatus::act(Restaurant &restaurant) {
//    if (restaurant.getTable(tableId) == nullptr){
//        error("Table does not exist");
//    }
    if(!restaurant.getTable(tableId)->isOpen()) //Table is closed
        std::cout << "Table "<< tableId << " status: closed\n";
    else { //Table is open
        std::cout << "Table " << tableId << " status: open\n";
        Table *temp = restaurant.getTable(tableId);
        std::vector<Dish> menu = restaurant.getMenu();
        std::cout << "Customers:\n"; //prints  "Customers:"
        for (size_t i = 0; i < temp->getCustomers().size(); ++i){
            std::cout << temp->getCustomers()[i]->getId() << " " << temp->getCustomers()[i]->getName() << "\n";
        }
        std::cout << "Orders:\n"; //prints  "Orders:"
        std::vector<OrderPair> orders = temp->getOrders();
        for (size_t l = 0; l <orders.size() ; ++l) {
            std::cout << orders[l].second.getName() << " " << orders[l].second.getPrice() << "NIS "
                      << orders[l].first << "\n"; //prints dish + dish price +customer id
        }
        std::cout<<"Current Bill: "<< temp->getBill()<<"NIS\n"; //prints the current bill
    }
    complete();
}

std::string PrintTableStatus::toString() const {
    return "status " + std::to_string(tableId) + " " + actionStatusToString(getStatus()) ;
}

BaseAction *PrintTableStatus::clone() {
    return new PrintTableStatus(*this);
}


RestoreResturant::RestoreResturant(): BaseAction() {

}

BaseAction *RestoreResturant::clone() {
    return new RestoreResturant(*this);
}

void RestoreResturant::act(Restaurant &restaurant) {
    if (backup == nullptr) {
        error("No backup available");
    }
    else{
        restaurant = *backup;
        complete();
    }

}

std::string RestoreResturant::toString() const {
    return "restore " + actionStatusToString(getStatus());
}


BackupRestaurant::BackupRestaurant(): BaseAction() {

}
BaseAction *BackupRestaurant::clone() {
    return new BackupRestaurant(*this);
}

std::string BackupRestaurant::toString() const {
    return "backup " + actionStatusToString(getStatus());
}

void BackupRestaurant::act(Restaurant &restaurant) {
    //delete every time the old backup
    if (backup != nullptr) {
        delete(backup);
        backup=nullptr;
    }
    backup = new Restaurant(restaurant);
    complete();
}

