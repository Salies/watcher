#include <httplib.h>
#include "Monitor.h"

#include <iterator>

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
		if (already_initialized) {
			return res.set_content(init_res, "application/json");
		}

		map<string, string> init = monitor.init();
		map<string, string>::iterator i;

		for (i = init.begin(); i != init.end(); i++) {
			init_res.append("\"" + i->first + "\":" + "\"" + i->second + "\",");
		}
		init_res.pop_back();
		init_res.append("}");

		already_initialized = true;

		res.set_content(init_res, "application/json");
	});

	svr.Get("/getData", [&](const Request& req, Response& res) {
		if (!already_initialized) {
			return res.set_content("{\"error\":\"Program not initialized\"}", "application/json");
		}

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

	svr.listen("127.0.0.1", PORT);

	return 0;
}