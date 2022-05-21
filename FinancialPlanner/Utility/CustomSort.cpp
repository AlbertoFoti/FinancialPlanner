#include "CustomSort.h"

Json::Value BubbleSortTransactions(Json::Value root)
{
	bool swapped = true;
	for (unsigned int i = 0; i != root["records"].size(); i++) {
		swapped = true;
		while (swapped) {
			swapped = false;
			for (unsigned int j = 0; j < root["records"][i]["data"].size() - 1; j++) {
				if (root["records"][i]["data"][j]["Day"] > root["records"][i]["data"][j + 1]["Day"]) {
					root = swapBubbleSortTransaction(root, i, j, j + 1);
					swapped = true;
				}
			}
		}
	}
	return root;
}

Json::Value swapBubbleSortTransaction(Json::Value root, int i, int j, int k) {
	Json::Value root2 = root["records"][i]["data"];
	Json::Value tmp = root2[j];
	root2[j] = root2[k];
	root2[k] = tmp;
	root["records"][i]["data"] = root2;

	return root;
}

Json::Value BubbleSortAccountDetails(Json::Value root)
{
	bool swapped = true;
	while (swapped) {
		swapped = false;
		for (unsigned int i = 0; i < root["records"].size() - 1; i++) {
			int monthXyear_i = root["records"][i]["Year"].asInt() * 12 + root["records"][i]["Month"].asInt();
			int monthXyear_next = root["records"][i + 1]["Year"].asInt() * 12 + root["records"][i + 1]["Month"].asInt();
			if (monthXyear_i > monthXyear_next) {
				root = swapBubbleSortAccountDetails(root, i, i + 1);
				swapped = true;
			}
		}
	};
	return root;
}

Json::Value swapBubbleSortAccountDetails(Json::Value root, int i, int j)
{
	Json::Value root2 = root["records"];
	Json::Value tmp = root2[i];
	root2[i] = root2[j];
	root2[j] = tmp;
	root["records"] = root2;

	return root;
}

Json::Value BubbleSortNetWorth(Json::Value root)
{
	bool swapped = true;
	while (swapped) {
		swapped = false;
		for (unsigned int i = 0; i < root["records"].size() - 1; i++) {
			int monthXyear_i = root["records"][i]["Year"].asInt() * 12 + root["records"][i]["Month"].asInt();
			int monthXyear_next = root["records"][i + 1]["Year"].asInt() * 12 + root["records"][i + 1]["Month"].asInt();
			if (monthXyear_i > monthXyear_next) {
				root = swapBubbleSortAccountDetails(root, i, i + 1);
				swapped = true;
			}
		}
	};
	return root;
}

Json::Value swapBubbleSortNetWorth(Json::Value root, int i, int j)
{
	Json::Value root2 = root["records"];
	Json::Value tmp = root2[i];
	root2[i] = root2[j];
	root2[j] = tmp;
	root["records"] = root2;

	return root;
}