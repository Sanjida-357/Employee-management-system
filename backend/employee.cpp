#include <iostream>
using namespace std;

class Employee {
protected:
    int empID;
    string name;

public:
    void inputBasicInfo() {
        cout << "Enter Employee ID: ";
        cin >> empID;
        cout << "Enter Name: ";
        cin.ignore();
        getline(cin, name);
    }

    void displayBasicInfo() {
        cout << "\nEmployee ID: " << empID;
        cout << "\nName: " << name << endl;
    }
};

class Attendance {
protected:
    int daysPresent, totalDays;

public:
    void inputAttendance() {
        cout << "Enter Days Present: ";
        cin >> daysPresent;
        cout << "Enter Total Working Days: ";
        cin >> totalDays;
    }

    double attendancePercentage() {
        return ((double)daysPresent / totalDays) * 100;
    }
};

class Leave {
protected:
    int leaveTaken;
    int maxLeave = 5;

public:
    void inputLeave() {
        cout << "Enter Leave Taken: ";
        cin >> leaveTaken;
    }

    int extraLeave() {
        if (leaveTaken > maxLeave)
            return leaveTaken - maxLeave;
        return 0;
    }
};

class Salary : public Employee, public Attendance, public Leave {
private:
    double baseSalary, overtimeHours, bonus;

public:
    void inputSalaryDetails() {
        cout << "Enter Base Salary: ";
        cin >> baseSalary;
        cout << "Enter Overtime Hours: ";
        cin >> overtimeHours;
        cout << "Enter Bonus: ";
        cin >> bonus;
    }

    double overtimePay() {
        return overtimeHours * 200;
    }

    double tax(double gross) {
        return gross * 0.1;
    }

    double insurance(double gross) {
        return gross * 0.05;
    }

    double pension(double gross) {
        return gross * 0.03;
    }

    double leaveDeduction() {
        return extraLeave() * 500;
    }

    void generateSalarySlip() {
        double ot = overtimePay();
        double gross = baseSalary + ot + bonus;

        double t = tax(gross);
        double ins = insurance(gross);
        double pen = pension(gross);
        double leaveDed = leaveDeduction();

        double net = gross - (t + ins + pen + leaveDed);

        displayBasicInfo();

        cout << "\n----- Salary Slip -----";
        cout << "\nBase Salary: " << baseSalary;
        cout << "\nOvertime Pay: " << ot;
        cout << "\nBonus: " << bonus;
        cout << "\nGross Salary: " << gross;

        cout << "\n\nDeductions:";
        cout << "\nTax: " << t;
        cout << "\nInsurance: " << ins;
        cout << "\nPension: " << pen;
        cout << "\nLeave Deduction: " << leaveDed;

        cout << "\n\nNet Salary: " << net;
        cout << "\nAttendance: " << attendancePercentage() << "%";
        cout << "\n----------------------\n";
    }
};

int main() {
    int choice;

    do {
        Salary emp;

        cout << "\n===== Employee Payment System =====";
        cout << "\n1. Add Employee & Generate Salary Slip";
        cout << "\n2. Exit";
        cout << "\nEnter choice: ";
        cin >> choice;

        if (choice == 1) {
            emp.inputBasicInfo();
            emp.inputAttendance();
            emp.inputLeave();
            emp.inputSalaryDetails();

            emp.generateSalarySlip();
        }

    } while (choice != 2);

    return 0;
}