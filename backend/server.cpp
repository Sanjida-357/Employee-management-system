#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <memory>
#include <exception>

#include "httplib.h"
#include "json.hpp"
#include "employee.cpp"

using namespace std;
using json = nlohmann::json;

struct EmployeeRecord {
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

vector<EmployeeRecord> employees;

json employeeToJson(const EmployeeRecord& e) {
    return {
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
}

void loadEmployeesFromFile() {
    employees.clear();

    ifstream file("employees.json");
    if (!file.is_open()) return;

    try {
        json j;
        file >> j;

        if (!j.is_array()) return;

        for (const auto& item : j) {
            EmployeeRecord e;
            e.empID = item.value("empID", 0);
            e.name = item.value("name", "");
            e.department = item.value("department", "");
            e.employmentType = item.value("employmentType", "Permanent");
            e.daysPresent = item.value("daysPresent", 0);
            e.totalDays = item.value("totalDays", 0);
            e.leaveTaken = item.value("leaveTaken", 0);
            e.baseSalary = item.value("baseSalary", 0.0);
            e.overtimeHours = item.value("overtimeHours", 0.0);
            e.bonus = item.value("bonus", 0.0);

            employees.push_back(e);
        }
    } catch (...) {
        cout << "Warning: Could not read employees.json properly." << endl;
    }

    file.close();
}

void saveEmployeesToFile() {
    json j = json::array();

    for (const auto& e : employees) {
        j.push_back(employeeToJson(e));
    }

    ofstream file("employees.json");
    file << j.dump(4);
    file.close();
}

int findEmployeeIndexByID(int id) {
    for (int i = 0; i < static_cast<int>(employees.size()); i++) {
        if (employees[i].empID == id) return i;
    }
    return -1;
}

string trim(const string& s) {
    size_t start = s.find_first_not_of(" \t\n\r");
    size_t end = s.find_last_not_of(" \t\n\r");

    if (start == string::npos) return "";
    return s.substr(start, end - start + 1);
}

bool isValidEmploymentType(const string& type) {
    return type == "Permanent" || type == "Part-Time" || type == "Contract";
}

bool validateEmployeeData(const json& data, string& errorMessage) {
    vector<string> requiredFields = {
        "empID", "name", "department", "employmentType",
        "daysPresent", "totalDays", "leaveTaken",
        "baseSalary", "overtimeHours", "bonus"
    };

    for (const auto& field : requiredFields) {
        if (!data.contains(field)) {
            errorMessage = "Missing field: " + field;
            return false;
        }
    }

    try {
        int empID = data.at("empID").get<int>();
        string name = trim(data.at("name").get<string>());
        string department = trim(data.at("department").get<string>());
        string employmentType = data.at("employmentType").get<string>();
        int daysPresent = data.at("daysPresent").get<int>();
        int totalDays = data.at("totalDays").get<int>();
        int leaveTaken = data.at("leaveTaken").get<int>();
        double baseSalary = data.at("baseSalary").get<double>();
        double overtimeHours = data.at("overtimeHours").get<double>();
        double bonus = data.at("bonus").get<double>();

        if (empID <= 0) {
            errorMessage = "Employee ID must be positive.";
            return false;
        }

        if (name.empty()) {
            errorMessage = "Name cannot be empty.";
            return false;
        }

        if (department.empty()) {
            errorMessage = "Department cannot be empty.";
            return false;
        }

        if (!isValidEmploymentType(employmentType)) {
            errorMessage = "Employment type must be Permanent, Part-Time, or Contract.";
            return false;
        }

        if (totalDays <= 0) {
            errorMessage = "Total working days must be greater than 0.";
            return false;
        }

        if (daysPresent < 0 || leaveTaken < 0) {
            errorMessage = "Attendance and leave values cannot be negative.";
            return false;
        }

        if (daysPresent > totalDays) {
            errorMessage = "Days present cannot be greater than total days.";
            return false;
        }

        if (leaveTaken > totalDays) {
            errorMessage = "Leave taken cannot be greater than total days.";
            return false;
        }

        if (baseSalary < 0 || overtimeHours < 0 || bonus < 0) {
            errorMessage = "Salary, overtime, and bonus cannot be negative.";
            return false;
        }
    } catch (const exception&) {
        errorMessage = "Invalid data type in request body.";
        return false;
    }

    return true;
}

EmployeeRecord jsonToEmployeeRecord(const json& data) {
    EmployeeRecord e;
    e.empID = data.at("empID").get<int>();
    e.name = trim(data.at("name").get<string>());
    e.department = trim(data.at("department").get<string>());
    e.employmentType = data.at("employmentType").get<string>();
    e.daysPresent = data.at("daysPresent").get<int>();
    e.totalDays = data.at("totalDays").get<int>();
    e.leaveTaken = data.at("leaveTaken").get<int>();
    e.baseSalary = data.at("baseSalary").get<double>();
    e.overtimeHours = data.at("overtimeHours").get<double>();
    e.bonus = data.at("bonus").get<double>();
    return e;
}

double attendancePercentage(int daysPresent, int totalDays) {
    if (totalDays == 0) return 0.0;
    return (static_cast<double>(daysPresent) / totalDays) * 100.0;
}

double overtimeRateByType(const string& employmentType) {
    if (employmentType == "Permanent") {
        return 300.0;
    }
    return 200.0;
}

double overtimePay(const string& employmentType, double overtimeHours) {
    return overtimeHours * overtimeRateByType(employmentType);
}

double leaveDeduction(int daysPresent, int totalDays, int leaveTaken, double baseSalary) {
    const int paidLeaveLimit = 1;

    int absentDays = totalDays - daysPresent;

    int paidLeaveUsed = min(leaveTaken, paidLeaveLimit);

    int unpaidDays = absentDays - paidLeaveUsed;

    if (unpaidDays <= 0) {
        return 0.0;
    }

    double perDaySalary = baseSalary / totalDays;
    return unpaidDays * perDaySalary;
}

json calculateSalaryDetails(const EmployeeRecord& e) {
    unique_ptr<Employee> emp(createEmployeeObject(
        e.empID,
        e.name,
        e.department,
        e.employmentType,
        e.baseSalary
    ));

    double attend = attendancePercentage(e.daysPresent, e.totalDays);
    double leaveDed = leaveDeduction(e.daysPresent, e.totalDays, e.leaveTaken, e.baseSalary);
    double overtime = overtimePay(e.employmentType, e.overtimeHours);

    double basicPay = emp->calculateBasicPay();
    double gross = basicPay + overtime + e.bonus;
    double taxAmount = gross * emp->getTaxRate();
    double insuranceAmount = gross * emp->getInsuranceRate();
    double pensionAmount = gross * emp->getPensionRate();
    double net = gross - taxAmount - insuranceAmount - pensionAmount - leaveDed;

    return {
        {"attendancePercentage", attend},
        {"basicPay", basicPay},
        {"leaveDeduction", leaveDed},
        {"overtimePay", overtime},
        {"grossSalary", gross},
        {"tax", taxAmount},
        {"insurance", insuranceAmount},
        {"pension", pensionAmount},
        {"netSalary", net}
    };
}

void sendJson(httplib::Response& res, const json& body, int status = 200) {
    res.status = status;
    res.set_content(body.dump(4), "application/json");
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
        try {
            json data = json::parse(req.body);

            string errorMessage;
            if (!validateEmployeeData(data, errorMessage)) {
                sendJson(res, {{"message", errorMessage}}, 400);
                return;
            }

            EmployeeRecord e = jsonToEmployeeRecord(data);

            if (findEmployeeIndexByID(e.empID) != -1) {
                sendJson(res, {{"message", "Employee ID already exists."}}, 400);
                return;
            }

            employees.push_back(e);
            saveEmployeesToFile();

            sendJson(res, {
                {"message", "Employee saved successfully."},
                {"employee", employeeToJson(e)}
            });
        } catch (const exception& ex) {
            sendJson(res, {{"message", string("Invalid request: ") + ex.what()}}, 400);
        }
    });

    // View all employees
    svr.Get("/employees", [](const httplib::Request&, httplib::Response& res) {
        json j = json::array();

        for (const auto& e : employees) {
            j.push_back(employeeToJson(e));
        }

        sendJson(res, j);
    });

    // Search employee by ID
    svr.Get(R"(/employees/(\d+))", [](const httplib::Request& req, httplib::Response& res) {
        int id = stoi(req.matches[1]);
        int index = findEmployeeIndexByID(id);

        if (index == -1) {
            sendJson(res, {{"message", "Employee not found."}}, 404);
            return;
        }

        sendJson(res, employeeToJson(employees[index]));
    });

    // Update employee
    svr.Put(R"(/employees/(\d+))", [](const httplib::Request& req, httplib::Response& res) {
        try {
            int originalId = stoi(req.matches[1]);
            int index = findEmployeeIndexByID(originalId);

            if (index == -1) {
                sendJson(res, {{"message", "Employee not found."}}, 404);
                return;
            }

            json data = json::parse(req.body);

            string errorMessage;
            if (!validateEmployeeData(data, errorMessage)) {
                sendJson(res, {{"message", errorMessage}}, 400);
                return;
            }

            EmployeeRecord updated = jsonToEmployeeRecord(data);

            int otherIndex = findEmployeeIndexByID(updated.empID);
            if (otherIndex != -1 && otherIndex != index) {
                sendJson(res, {{"message", "Another employee already has this ID."}}, 400);
                return;
            }

            employees[index] = updated;
            saveEmployeesToFile();

            sendJson(res, {
                {"message", "Employee updated successfully."},
                {"employee", employeeToJson(updated)}
            });
        } catch (const exception& ex) {
            sendJson(res, {{"message", string("Invalid request: ") + ex.what()}}, 400);
        }
    });

    // Delete employee
    svr.Delete(R"(/employees/(\d+))", [](const httplib::Request& req, httplib::Response& res) {
        int id = stoi(req.matches[1]);
        int index = findEmployeeIndexByID(id);

        if (index == -1) {
            sendJson(res, {{"message", "Employee not found."}}, 404);
            return;
        }

        employees.erase(employees.begin() + index);
        saveEmployeesToFile();

        sendJson(res, {{"message", "Employee deleted successfully."}});
    });

    // Calculate payroll
    svr.Post("/calculate", [](const httplib::Request& req, httplib::Response& res) {
        try {
            json data = json::parse(req.body);

            string errorMessage;
            if (!validateEmployeeData(data, errorMessage)) {
                sendJson(res, {{"message", errorMessage}}, 400);
                return;
            }

            EmployeeRecord e = jsonToEmployeeRecord(data);
            json result = calculateSalaryDetails(e);

            sendJson(res, result);
        } catch (const InvalidInputException& ex) {
            sendJson(res, {{"message", ex.what()}}, 400);
        } catch (const exception& ex) {
            sendJson(res, {{"message", string("Invalid request: ") + ex.what()}}, 400);
        }
    });

    cout << "Server running at http://localhost:8080" << endl;
    svr.listen("0.0.0.0", 8080);

    return 0;
}