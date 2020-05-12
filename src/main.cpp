#include "Monitor.h"
#include <httplib.h>

#include <iterator>
#include <iostream>


#define PORT 1234 //change to whatever port you want, lel

int main(void) {
	using namespace httplib;
	using std::map;
	using std::string;

	Server svr;
	Monitor monitor;

	bool already_initialized = false; 
	string init_res = "{";

	svr.Get("/init", [&](const Request& req, Response& res) {
		if (!already_initialized) {
			std::cout << "\nn foi inicializado, iniciando...\n";
			map<string, string> init = monitor.init();
			map<string, string>::iterator i;

			for (i = init.begin(); i != init.end(); i++) {
				init_res.append("\"" + i->first + "\":" + "\"" + i->second + "\",");
			}
			init_res.pop_back();
			init_res.append("}");

			already_initialized = true;
		}

		res.set_content(init_res, "application/json");
	});

	svr.Get("/getData", [&](const Request& req, Response& res) {
		map<string, int> m = monitor.getData();
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
		monitor.exit();
		svr.stop();
	});

	svr.listen("localhost", PORT);

	return 0;
}