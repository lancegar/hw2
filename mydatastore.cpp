#include "mydatastore.h"
#include <iostream>


MyDataStore::MyDataStore() {}

MyDataStore::~MyDataStore() {
    
    for(Product* p : products_) {
        delete p;
    }
    
    for(const std::pair<const std::string, User*>& entry : users_) {
        delete entry.second;
    }
}

void MyDataStore::addProduct(Product* p) {
    products_.push_back(p);
    indexProduct(p);  
}

void MyDataStore::addUser(User* u) {
    users_[u->getName()] = u;
}

void MyDataStore::indexProduct(Product* p) {
    std::set<std::string> keywords = p->keywords();

    for(std::set<std::string>::const_iterator keyword = keywords.begin(); keyword != keywords.end(); ++keyword) {
        keywordMap_[*keyword].insert(p);  
    }
}

std::vector<Product*> MyDataStore::search(std::vector<std::string>& terms, int type) {
    std::set<Product*> result;

    if(terms.empty()) return {};  
    
    if(keywordMap_.find(terms[0]) != keywordMap_.end()) {
        result = keywordMap_[terms[0]];
    }

    for(size_t i = 1; i < terms.size(); ++i) {
        if(keywordMap_.find(terms[i]) != keywordMap_.end()) {
            if(type == 0) {
                result = setIntersection(result, keywordMap_[terms[i]]);
            }
            
            else {
                result = setUnion(result, keywordMap_[terms[i]]);
            }
        } 
        
        else if(type == 0) {
            result.clear();
            break;
        }
    }

    return std::vector<Product*>(result.begin(), result.end());
}


void MyDataStore::addToCart(const std::string& username, int hitIndex, const std::vector<Product*>& hits) {
    if(users_.find(username) == users_.end()) {
        std::cout << "Invalid username\n";
        return;
    }

    if(hitIndex < 0 || hitIndex >= static_cast<int>(hits.size())) {
        std::cout << "Invalid request" << std::endl;
        return;
    }

    carts_[username].push_back(hits[hitIndex]);  
}


void MyDataStore::viewCart(const std::string& username) const {
    if(users_.find(username) == users_.end()) {
        std::cout << "Invalid username\n";
        return;
    }

    const std::vector<Product*>& cart = carts_.at(username);

    for(size_t i = 0; i < cart.size(); ++i) {
        std::cout << "Item " << (i + 1) << "\n" << cart[i]->displayString() << "\n";
    }
}


void MyDataStore::buyCart(const std::string& username) {
    if(users_.find(username) == users_.end()) {
        std::cout << "Invalid username\n";
        return;
    }

    std::vector<Product*>& cart = carts_[username];
    User* user = users_[username];
    std::vector<Product*> remainingCart;

    for(Product* p : cart) {
        if(p->getQty() > 0 && user->getBalance() >= p->getPrice()) {
            p->subtractQty(1);
            user->deductAmount(p->getPrice());
        } 
        
        else {
            remainingCart.push_back(p);  
        }
    }

    carts_[username] = remainingCart;  
}

void MyDataStore::dump(std::ostream& ofile) {
    ofile << "<products>\n";

    for(Product* p : products_) {
        p->dump(ofile);
    }

    ofile << "</products>\n<users>\n";

    for(const std::pair<std::string, User*>& u : users_) {
        u.second->dump(ofile);
    }

    ofile << "</users>\n";
}