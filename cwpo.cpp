#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>


// Type definitions ###########################################################

using WingCountT = int;
using PriceT     = int;

struct MenuItem {
	WingCountT num_wings;
	PriceT price;
	};


// Main #######################################################################

int main(int argc, char* argv[]) {
	// Read inputs ========================================================

	// Maximum serving size to calculate
	if (argc == 1) {
		std::clog << "Please specify the maximum serving size to calculate!\n";
		return 0;
		}

	WingCountT max_num_wings = strtol(argv[1], nullptr, 10);

	// Menu
	std::vector<MenuItem> menu;
	std::clog << "Menu: \n";

	while (true) {
		MenuItem item;
		std::cin >> item.num_wings;
		std::cin >> item.price;

		if (!std::cin.good()) {
			if (!std::cin.eof()) {
				std::clog << "An error occured while parsing the menu, "
				             "please check the formatting "
				             "(only whitespaces and integers are allowed)!\n";
				return 0;
				}

			break;
			}

		std::clog << item.num_wings << " wings \t" << item.price << "\n";

		menu.push_back(std::move(item));
		}

	if (menu.size() == 0) {
		std::clog << "Error: The menu may not be empty!\n";
		return 0;
		}

	std::clog << "\n";


	// Initialize calculation data ========================================

	// Smallest serving size
	WingCountT min_num_wings = std::min_element(
			menu.begin(),
			menu.end(),
			[](const auto& a, const auto& b) {
				return a.num_wings < b.num_wings;
				}
			)
			->num_wings;

	// Highest possible price used when a price has not been calculated yet
	// or is not defined
	constexpr PriceT no_price = std::numeric_limits<PriceT>::max();

	// Minimum price for each serving size:
	// Initialized according to menu if the size is on the menu
	// and no_price otherwise.
	std::vector<PriceT> min_prices (max_num_wings + 1, no_price);

	for (auto& item : menu) {
		if (item.price < min_prices[item.num_wings]) {
			min_prices[item.num_wings] = item.price;
			}

		}

	// Optimal partition for each serving size:
	// List of menu items which result in the minimum price for that size.
	std::vector<std::string> partitions;
	partitions.reserve(min_prices.size());


	// Calculate minimum prices ===========================================

	// Print output header
	std::cout << "number of wings\tminimum price\toptimal partition\n";

	// For every serving size to calculate, ascending:
	for (WingCountT num_wings = min_num_wings;
			num_wings <= max_num_wings;
			++num_wings
			) {
		PriceT min_price          = min_prices[num_wings];
		WingCountT best_partition = 0;

		// For every way to partition this number of wings into two servings:
		for (int i = min_num_wings; i <= num_wings / 2; ++i) {
			// Get the minimum prices for both parts (already calculated).
			// A value of no_price means that this partition is impossible.
			PriceT p1 = min_prices[i];
			PriceT p2 = min_prices[num_wings - i];

			if (p1 == no_price or p2 == no_price) {
				continue;
				}

			// If this partition is possible, update the minimum price
			PriceT price = p1 + p2;

			if (price < min_price) {
				min_price      = price;
				best_partition = i;
				}

			}

		min_prices[num_wings] = min_price;

		// Store the best partition as string
		std::string partition;

		if (min_price == no_price) {
			partition = "-";
			}
		else if (best_partition == 0) {
			partition = std::to_string(num_wings);
			}
		else {
			auto& p1 = partitions[best_partition];
			auto& p2 = partitions[num_wings - best_partition];

			partition.reserve(p1.size() + p2.size() + 2);
			partition.append(p1)
			         .append(", ")
			         .append(p2);
			}

		partitions[num_wings] = partition;

		// Output serving size, minimum price and optimal partition
		std::cout << num_wings << "\t";

		if (min_price == no_price) {
			std::cout << "-\t";
			}
		else {
			std::cout << min_price << "\t";
			}

		std::cout << partition << "\n";
		}

	}
