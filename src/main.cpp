#include "Monitor.h"
#include <httplib.h>

#include <iterator>

#define PORT 1234 //change to whatever port you want, lel

int main(void) {
	using namespace httplib;
	using std::map;
	using std::string;

	Server svr;
	Monitor monitor;

	svr.Get("/init", [&](const Request& req, Response& res) {
		map<string, string> init = monitor.init();
		map<string, string>::iterator i;

		//TODO(?, cuz types): create function for json "encoding"
		string json_string = "{";
		for (i = init.begin(); i != init.end(); i++) {
			json_string.append("\"" + i->first + "\":" + "\"" + i->second + "\",");
		}
		json_string.pop_back();
		json_string.append("}");

		res.set_content(json_string, "application/json");
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