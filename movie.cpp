#include "movie.h"
#include <sstream>

Movie::Movie(const std::string& category, const std::string& name, double price, 
						int qty, const std::string& genre, const std::string& rating)
				: Product(category, name, price, qty), genre_(genre), rating_(rating) {}

std::set<std::string> Movie::keywords() const {

	std::set<std::string> keys = parseStringToWords(name_);
	std::set<std::string> genreKeys = parseStringToWords(genre_);

	keys.insert(genreKeys.begin(), genreKeys.end());

	return keys;
}

std::string Movie::displayString() const {

	std::ostringstream os;

	os << name_ << "\nGenre: " << genre_ << " Rating: " << rating_ << "\n" << price_ << " " << qty_;

	return os.str();
}

void Movie::dump(std::ostream& os) const {

	os << category_ << "\n" << name_ << "\n" << price_ << "\n" << qty_ << "\n" << genre_ << "\n" << rating_ << std::endl;
}