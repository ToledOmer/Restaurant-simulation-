//
// Created by yuvalman@wincs.cs.bgu.ac.il on 11/4/18.
//

#include "../include/Table.h"


Table::Table(int t_capacity) : capacity(t_capacity), open(false),customersList(),orderList()


{}

int Table::getCapacity() const {
    return capacity;
}

void Table::addCustomer(Customer* customer){
    customersList.push_back(customer);
}
void Table::removeCustomer(int id){
    std::vector<Customer*> customers ;
    for (size_t i = 0; i < customersList.size() ; ++i) {
        if(customersList[i] != getCustomer(id)) {
            customers.push_back(customersList[i]);
        }
    }
        customersList = customers;
    }


Customer* Table:: getCustomer(int id){
    for (size_t i = 0; i < customersList.size() ; ++i)
        if(customersList[i]->getId() == id)
            return customersList[i];
    return nullptr; //Customers hasn't found

}

std::vector<Customer*>& Table::getCustomers() {
    return customersList;
}
std::vector<OrderPair>& Table::getOrders() {
    return orderList ;
}



void Table::order(const std::vector<Dish> &menu) {
    for (size_t i = 0; i <customersList.size() ; ++i) {
        std::vector<int> customerOrder = customersList[i]->order(menu);
        for (size_t j = 0; j <customerOrder.size() ; ++j) {
            std::cout<<customersList[i]->getName() + " ordered " + menu[customerOrder[j]].getName()<<std::endl;
            orderList.push_back(OrderPair(customersList[i]->getId(),menu[customerOrder[j]]));
        }
    }
}

void Table::openTable(){
    open = true;

}
void Table::closeTable(){
    open = false;
}
int Table::getBill(){
    int bill = 0;
    for (size_t i = 0; i <orderList.size() ; ++i) {
        bill = bill + orderList[i].second.getPrice();
    }
    return bill;
}

bool Table::isOpen(){
    return open;
}

// Destructor
Table::~Table() { clear(); }
//clear
void Table::clear(){
    for (size_t i = 0; i < customersList.size(); ++i) {
        delete (customersList[i]);
        customersList[i] = nullptr;
    }
    customersList.clear();
    closeTable();
    orderList.clear();
}
// Copy Constructor
Table::Table(Table &other):
capacity(other.capacity), open(other.open),customersList(),orderList() {
    //dish is const, so we we have to go over all the vector and add order(instead of calling orders=getOrders())
    for (size_t j = 0; j < other.getOrders().size(); ++j) {
        OrderPair order=other.getOrders()[j];
        orderList.push_back(order);
    }
    for (size_t i = 0; i <other.getCustomers().size() ; ++i) {
        addCustomer(other.getCustomers()[i]->clone());
    } // copy customers pointers
}

// Move Constructor
Table::Table(Table &&other): capacity(other.capacity), open(other.open),
customersList(other.customersList),orderList()  {
    other.open= false;
    //dish is const, so we wals have to go over all the vector and add order(instead of calling orders=getOrders())
    for (size_t j = 0; j < other.getOrders().size(); ++j) {
        OrderPair order=other.getOrders()[j];
        orderList.push_back(order);
    }
    for (size_t i = 0; i <other.getCustomers().size() ; ++i) {
        addCustomer(other.getCustomers()[i]->clone());
    } // copy customers pointers
    other.clear();
}
// Copy Assignment
Table &Table::operator=( Table &other) {
    if(this != &other){
        this->clear();
        open = (other.open);
        capacity = (other.capacity);
        for (size_t i = 0; i < getCustomers().size() ; ++i) {
            customersList.push_back(other.getCustomers()[i]->clone());
        }
        for (size_t j = 0; j < other.getOrders().size(); ++j) {
            OrderPair order=other.getOrders()[j];
            orderList.push_back(order);
        }
        other.clear();
    }
    return *this;
}
// Move Assignment
Table &Table::operator=(Table &&other) {
    if (this != &other) {
        clear();
        open = (other.open);
        capacity = (other.capacity);
        for (size_t i = 0; i < getCustomers().size() ; ++i) {
            customersList.push_back(other.getCustomers()[i]->clone());
        }
        for (size_t j = 0; j < other.getOrders().size(); ++j) {
            OrderPair order=other.getOrders()[j];
            orderList.push_back(order);
        }
        other.clear();
    }
    return *this;
}





