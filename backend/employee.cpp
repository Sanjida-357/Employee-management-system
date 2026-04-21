#ifndef EMPLOYEE_CPP
#define EMPLOYEE_CPP

#include <iostream>
#include <string>
#include <stdexcept>
using namespace std;

// ================= CUSTOM EXCEPTION =================
class InvalidInputException : public exception {
private:
    string message;

public:
    InvalidInputException(const string& msg) : message(msg) {}

    const char* what() const noexcept override {
        return message.c_str();
    }
};

// ================= TEMPLATE UTILITY FUNCTIONS =================
template <typename T>
T maxValue(T a, T b) {
    return (a > b) ? a : b;
}

template <typename T>
T minValue(T a, T b) {
    return (a < b) ? a : b;
}

// ================= ABSTRACT BASE CLASS =================
class Employee {
protected:
    int empID;
    string name;
    string department;
    string employeeType;
    double baseSalary;

public:
    Employee() {
        empID = 0;
        name = "";
        department = "";
        employeeType = "";
        baseSalary = 0.0;
    }

    Employee(int id, string n, string dept, string type, double salary) {
        if (id <= 0) throw InvalidInputException("Employee ID must be positive.");
        if (n.empty()) throw InvalidInputException("Name cannot be empty.");
        if (dept.empty()) throw InvalidInputException("Department cannot be empty.");
        if (salary < 0) throw InvalidInputException("Base salary cannot be negative.");

        empID = id;
        name = n;
        department = dept;
        employeeType = type;
        baseSalary = salary;
    }

    virtual ~Employee() {}

    void setEmpID(int id) {
        if (id <= 0) throw InvalidInputException("Employee ID must be positive.");
        empID = id;
    }

    void setName(string n) {
        if (n.empty()) throw InvalidInputException("Name cannot be empty.");
        name = n;
    }

    void setDepartment(string dept) {
        if (dept.empty()) throw InvalidInputException("Department cannot be empty.");
        department = dept;
    }

    void setEmployeeType(string type) {
        if (type.empty()) throw InvalidInputException("Employee type cannot be empty.");
        employeeType = type;
    }

    void setBaseSalary(double salary) {
        if (salary < 0) throw InvalidInputException("Base salary cannot be negative.");
        baseSalary = salary;
    }

    void setBaseSalary(int salary) {
        if (salary < 0) throw InvalidInputException("Base salary cannot be negative.");
        baseSalary = static_cast<double>(salary);
    }

    int getEmpID() const {
        return empID;
    }

    string getName() const {
        return name;
    }

    string getDepartment() const {
        return department;
    }

    string getEmployeeType() const {
        return employeeType;
    }

    double getBaseSalary() const {
        return baseSalary;
    }

    virtual double calculateBasicPay() const = 0;
    virtual double getTaxRate() const = 0;
    virtual double getInsuranceRate() const = 0;
    virtual double getPensionRate() const = 0;
};

// ================= DERIVED CLASSES =================
class PermanentEmployee : public Employee {
public:
    PermanentEmployee(int id, string n, string dept, double salary)
        : Employee(id, n, dept, "Permanent", salary) {}

    double calculateBasicPay() const override {
        return baseSalary;
    }

    double getTaxRate() const override {
        return 0.10;
    }

    double getInsuranceRate() const override {
        return 0.05;
    }

    double getPensionRate() const override {
        return 0.08;
    }
};

class PartTimeEmployee : public Employee {
public:
    PartTimeEmployee(int id, string n, string dept, double salary)
        : Employee(id, n, dept, "Part-Time", salary) {}

    double calculateBasicPay() const override {
        return baseSalary;
    }

    double getTaxRate() const override {
        return 0.05;
    }

    double getInsuranceRate() const override {
        return 0.0;
    }

    double getPensionRate() const override {
        return 0.0;
    }
};

class ContractEmployee : public Employee {
public:
    ContractEmployee(int id, string n, string dept, double salary)
        : Employee(id, n, dept, "Contract", salary) {}

    double calculateBasicPay() const override {
        return baseSalary;
    }

    double getTaxRate() const override {
        return 0.08;
    }

    double getInsuranceRate() const override {
        return 0.0;
    }

    double getPensionRate() const override {
        return 0.0;
    }
};

// ================= FACTORY FUNCTION =================
Employee* createEmployeeObject(int empID, string name, string department,
                               string employeeType, double baseSalary) {
    if (employeeType == "Permanent") {
        return new PermanentEmployee(empID, name, department, baseSalary);
    }
    else if (employeeType == "Part-Time") {
        return new PartTimeEmployee(empID, name, department, baseSalary);
    }
    else if (employeeType == "Contract") {
        return new ContractEmployee(empID, name, department, baseSalary);
    }
    else {
        throw InvalidInputException("Invalid employee type. Use Permanent, Part-Time, or Contract.");
    }
}

#endif