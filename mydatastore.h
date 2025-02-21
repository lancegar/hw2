#ifndef MYDATASTORE_H
#define MYDATASTORE_H

#include "datastore.h"
#include "product.h"
#include "user.h"
#include "util.h"
#include <map>
#include <vector>
#include <set>
#include <string>
#include <utility>

class MyDataStore : public DataStore {

    public:
        MyDataStore();
        ~MyDataStore();

        void addProduct(Product* p) override;
        void addUser(User* u) override;

        std::vector<Product*> search(std::vector<std::string>& terms, int type) override;

        void dump(std::ostream& ofile) override;

        void addToCart(const std::string& username, int hitIndex, const std::vector<Product*>& hits);
        void viewCart(const std::string& username) const;
        void buyCart(const std::string& username);

    private:
        std::map<std::string, User*> users_;
        std::vector<Product*> products_;
        std::map<std::string, std::set<Product*>> keywordMap_;
        std::map<std::string, std::vector<Product*>> carts_;

        void indexProduct(Product* p);
};

#endif
