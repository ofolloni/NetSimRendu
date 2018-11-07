#include "network.h"
#include "random.h"
#include <iostream>

void Network::resize(const size_t &n) {
	if(!(n<=0)) {
		values.resize(n);
		RNG.normal(values);
	} else {
		values.clear();
		std::cerr << "Your vector cannot be smaller than 0" << std::endl;
	}		
}
	
bool Network::add_link(const size_t& a, const size_t& b){
	
	if(a>=values.size() or b>=values.size() or a==b) { return false; } //if a or b do not exist or if they are the same

	for(auto i : links) {
		if((i.first == a and i.second == b) or ( i.first == b and i.second == a)){ return false; }//if the link a - b or b - a already exists	
	}
	links.insert({a, b});
	links.insert({b, a});
	return true;
}
	
size_t Network::random_connect(const double& mean_deg){
	links.clear();
	std::vector<size_t> shuffled_values(0); //to get random other nodes
	for(size_t i=0; i < values.size(); ++i) {
		shuffled_values.push_back(i);
	}
	size_t count(0);
	for(size_t i=0; i < values.size(); ++i) {
		RNG.shuffle(shuffled_values);
		int poissonNB(RNG.poisson(mean_deg));
		if(poissonNB <= 0) { poissonNB = 0; }
		if(poissonNB >= shuffled_values.size()) { poissonNB = shuffled_values.size() - 1; } //if the poisson distribution returns a number larger than the number of nodes, initiate it to the total number of nodes - 1, since we do not want count the current node i
		for(int j=0; j < poissonNB; ++j){
			if(add_link(i, shuffled_values[j])) { ++count; }
		}
	}
	return count;
}

size_t Network::set_values(const std::vector<double>& new_vec){
	size_t set_vals(0);
	if(new_vec.size() <= values.size()) { set_vals = new_vec.size(); }
	else { set_vals = values.size(); }
	for(size_t i=0; i < set_vals; ++i) {
		values[i] = new_vec[i];
	}		
	return set_vals;
}

size_t Network::size() const { return values.size(); }
	
size_t Network::degree(const size_t &_n) const { return links.count(_n); }
	
double Network::value(const size_t &_n) const{
	if(_n < values.size()) { return values[_n]; }
	else { return 0; }
}

std::vector<double> Network::sorted_values() const{
	std::vector<double> sorted_vals(values);
	std::sort(sorted_vals.begin(), sorted_vals.end(), std::greater<double>());
	return sorted_vals;
}

std::vector<size_t> Network::neighbors(const size_t& _n) const{
	std::vector<size_t> neighbor;
	for(auto N : links) {
		if(_n == N.first) {
			neighbor.push_back(N.second);
		}
	}
	return neighbor;
}
	
	

