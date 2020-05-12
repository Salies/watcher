#include "Monitor.h"
#include <httplib.h>

#include <iostream>
#include <iterator>

#define PORT 1234

int main(void) {
	using namespace httplib;
	using std::map;
	using std::string;

	Server svr;
	Monitor monitor;

	//TODO: turn this into a HTTP call aswell
	string init = monitor.init().begin()->second;
	std::cout << init << "\n";

	svr.Get("/getData", [](const Request& req, Response& res) {
		Monitor mon;
		map<string, int> m = mon.getData();
		map<string, int>::iterator i;
		string json_string = "{";
		for (i = m.begin(); i != m.end(); i++) {
			json_string.append("\"" + i->first + "\":" + "\"" + std::to_string(i->second) + "\",");
		}
		json_string.pop_back(); //last character will be comma, so remove it --- is it possible to detect the antepenultimate element in an iterator loop? nway think this is easier/less consuming
		json_string.append("}");

		res.set_content(json_string, "application/json");
	});

	svr.Get("/kill", [&](const Request& req, Response& res) {
		svr.stop();
	});

	svr.listen("localhost", PORT);

	return 0;
}