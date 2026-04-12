#include <iostream>
#include "httplib.h"
#include "json.hpp"
#include "employee.cpp"

using namespace std;
using json = nlohmann::json;

int main() {
    httplib::Server svr;

    svr.Options(R"(.*)", [](const httplib::Request &, httplib::Response &res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        res.status = 200;
    });

    svr.Post("/calculate", [](const httplib::Request& req, httplib::Response& res) {
        try {
            auto data = json::parse(req.body);

            Salary emp;
            emp.setBasicInfo(data["empID"], data["name"]);
            emp.setAttendance(data["daysPresent"], data["totalDays"]);
            emp.setLeave(data["leaveTaken"]);
            emp.setSalaryDetails(data["baseSalary"], data["overtimeHours"], data["bonus"]);

            json result = {
                {"empID", data["empID"]},
                {"name", data["name"]},
                {"attendance", emp.attendancePercentage()},
                {"overtimePay", emp.overtimePay()},
                {"grossSalary", emp.grossSalary()},
                {"tax", emp.tax()},
                {"insurance", emp.insurance()},
                {"pension", emp.pension()},
                {"leaveDeduction", emp.leaveDeduction()},
                {"netSalary", emp.netSalary()}
            };

            res.set_header("Access-Control-Allow-Origin", "*");
            res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
            res.set_header("Access-Control-Allow-Headers", "Content-Type");
            res.set_content(result.dump(4), "application/json");
        } catch (...) {
            res.status = 400;
            res.set_header("Access-Control-Allow-Origin", "*");
            res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
            res.set_header("Access-Control-Allow-Headers", "Content-Type");
            res.set_content("{\"error\":\"Invalid input\"}", "application/json");
        }
    });

    cout << "Server running at http://localhost:8080\n";
    svr.listen("0.0.0.0", 8080);
}