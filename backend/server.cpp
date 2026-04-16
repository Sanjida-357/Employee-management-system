#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "httplib.h"
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

struct Employee {
    int empID;
    string name;
    string department;
    string employmentType;
    int daysPresent;
    int totalDays;
    int leaveTaken;
    double baseSalary;
    double overtimeHours;
    double bonus;
};

vector<Employee> employees;

void loadEmployeesFromFile() {
    employees.clear();
    ifstream file("employees.json");
    if (!file.is_open()) return;

    json j;
    file >> j;
    file.close();

    for (auto& item : j) {
        Employee e;
        e.empID = item["empID"];
        e.name = item["name"];
        e.department = item["department"];
        e.employmentType = item["employmentType"];
        e.daysPresent = item["daysPresent"];
        e.totalDays = item["totalDays"];
        e.leaveTaken = item["leaveTaken"];
        e.baseSalary = item["baseSalary"];
        e.overtimeHours = item["overtimeHours"];
        e.bonus = item["bonus"];
        employees.push_back(e);
    }
}

void saveEmployeesToFile() {
    json j = json::array();

    for (const auto& e : employees) {
        j.push_back({
            {"empID", e.empID},
            {"name", e.name},
            {"department", e.department},
            {"employmentType", e.employmentType},
            {"daysPresent", e.daysPresent},
            {"totalDays", e.totalDays},
            {"leaveTaken", e.leaveTaken},
            {"baseSalary", e.baseSalary},
            {"overtimeHours", e.overtimeHours},
            {"bonus", e.bonus}
        });
    }

    ofstream file("employees.json");
    file << j.dump(4);
    file.close();
}

int findEmployeeIndexByID(int id) {
    for (int i = 0; i < employees.size(); i++) {
        if (employees[i].empID == id) return i;
    }
    return -1;
}

double attendancePercentage(int daysPresent, int totalDays) {
    if (totalDays == 0) return 0;
    return ((double)daysPresent / totalDays) * 100.0;
}

double leaveDeduction(int leaveTaken) {
    int maxLeave = 5;
    if (leaveTaken > maxLeave) {
        return (leaveTaken - maxLeave) * 200.0;
    }
    return 0.0;
}

double overtimePay(double overtimeHours) {
    return overtimeHours * 200.0;
}

double tax(double grossSalary) {
    return grossSalary * 0.10;
}

double insurance(double grossSalary) {
    return grossSalary * 0.05;
}

double pension(double grossSalary) {
    return grossSalary * 0.03;
}

int main() {
    loadEmployeesFromFile();

    httplib::Server svr;

    svr.set_default_headers({
        {"Access-Control-Allow-Origin", "*"},
        {"Access-Control-Allow-Headers", "Content-Type"},
        {"Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS"}
    });

    svr.Options(R"(.*)", [](const httplib::Request&, httplib::Response& res) {
        res.status = 200;
    });

    // Save employee
    svr.Post("/employees", [](const httplib::Request& req, httplib::Response& res) {
        json data = json::parse(req.body);

        Employee e;
        e.empID = data["empID"];
        e.name = data["name"];
        e.department = data["department"];
        e.employmentType = data["employmentType"];
        e.daysPresent = data["daysPresent"];
        e.totalDays = data["totalDays"];
        e.leaveTaken = data["leaveTaken"];
        e.baseSalary = data["baseSalary"];
        e.overtimeHours = data["overtimeHours"];
        e.bonus = data["bonus"];

        if (findEmployeeIndexByID(e.empID) != -1) {
            res.status = 400;
            res.set_content(R"({"message":"Employee ID already exists."})", "application/json");
            return;
        }

        employees.push_back(e);
        saveEmployeesToFile();
        res.set_content(R"({"message":"Employee saved successfully."})", "application/json");
    });

    // View all employees
    svr.Get("/employees", [](const httplib::Request&, httplib::Response& res) {
        json j = json::array();

        for (const auto& e : employees) {
            j.push_back({
                {"empID", e.empID},
                {"name", e.name},
                {"department", e.department},
                {"employmentType", e.employmentType},
                {"daysPresent", e.daysPresent},
                {"totalDays", e.totalDays},
                {"leaveTaken", e.leaveTaken},
                {"baseSalary", e.baseSalary},
                {"overtimeHours", e.overtimeHours},
                {"bonus", e.bonus}
            });
        }

        res.set_content(j.dump(4), "application/json");
    });

    // Search employee by ID
    svr.Get(R"(/employees/(\d+))", [](const httplib::Request& req, httplib::Response& res) {
        int id = stoi(req.matches[1]);
        int index = findEmployeeIndexByID(id);

        if (index == -1) {
            res.status = 404;
            res.set_content(R"({"message":"Employee not found."})", "application/json");
            return;
        }

        Employee e = employees[index];
        json j = {
            {"empID", e.empID},
            {"name", e.name},
            {"department", e.department},
            {"employmentType", e.employmentType},
            {"daysPresent", e.daysPresent},
            {"totalDays", e.totalDays},
            {"leaveTaken", e.leaveTaken},
            {"baseSalary", e.baseSalary},
            {"overtimeHours", e.overtimeHours},
            {"bonus", e.bonus}
        };

        res.set_content(j.dump(4), "application/json");
    });

    // Update employee
    svr.Put(R"(/employees/(\d+))", [](const httplib::Request& req, httplib::Response& res) {
        int id = stoi(req.matches[1]);
        int index = findEmployeeIndexByID(id);

        if (index == -1) {
            res.status = 404;
            res.set_content(R"({"message":"Employee not found."})", "application/json");
            return;
        }

        json data = json::parse(req.body);

        employees[index].empID = data["empID"];
        employees[index].name = data["name"];
        employees[index].department = data["department"];
        employees[index].employmentType = data["employmentType"];
        employees[index].daysPresent = data["daysPresent"];
        employees[index].totalDays = data["totalDays"];
        employees[index].leaveTaken = data["leaveTaken"];
        employees[index].baseSalary = data["baseSalary"];
        employees[index].overtimeHours = data["overtimeHours"];
        employees[index].bonus = data["bonus"];

        saveEmployeesToFile();
        res.set_content(R"({"message":"Employee updated successfully."})", "application/json");
    });

    // Delete employee
    svr.Delete(R"(/employees/(\d+))", [](const httplib::Request& req, httplib::Response& res) {
        int id = stoi(req.matches[1]);
        int index = findEmployeeIndexByID(id);

        if (index == -1) {
            res.status = 404;
            res.set_content(R"({"message":"Employee not found."})", "application/json");
            return;
        }

        employees.erase(employees.begin() + index);
        saveEmployeesToFile();
        res.set_content(R"({"message":"Employee deleted successfully."})", "application/json");
    });

    // Generate salary slip
    svr.Post("/calculate", [](const httplib::Request& req, httplib::Response& res) {
        json data = json::parse(req.body);

        int daysPresent = data["daysPresent"];
        int totalDays = data["totalDays"];
        int leaveTaken = data["leaveTaken"];
        double baseSalary = data["baseSalary"];
        double overtimeHours = data["overtimeHours"];
        double bonus = data["bonus"];

        double attend = attendancePercentage(daysPresent, totalDays);
        double leaveDed = leaveDeduction(leaveTaken);
        double overtime = overtimePay(overtimeHours);
        double gross = baseSalary + overtime + bonus - leaveDed;
        double taxAmount = tax(gross);
        double insuranceAmount = insurance(gross);
        double pensionAmount = pension(gross);
        double net = gross - taxAmount - insuranceAmount - pensionAmount;

        json response = {
            {"attendancePercentage", attend},
            {"leaveDeduction", leaveDed},
            {"overtimePay", overtime},
            {"grossSalary", gross},
            {"tax", taxAmount},
            {"insurance", insuranceAmount},
            {"pension", pensionAmount},
            {"netSalary", net}
        };

        res.set_content(response.dump(4), "application/json");
    });

    cout << "Server running at http://localhost:8080" << endl;
    svr.listen("0.0.0.0", 8080);

    return 0;
}