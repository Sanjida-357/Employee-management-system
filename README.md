# Employee Management System

## Overview

The Employee Management System (EMS) is a C++-based project developed using Object-Oriented Programming concepts. It manages employee records, attendance, leave, payroll, and salary slip generation through a backend server and a web-based frontend dashboard.

The system demonstrates integration of OOP, file handling, and frontend-backend communication in a complete application.

---

## Objective

The objective of this project is to design and implement an Employee Management System using Object-Oriented Programming in C++. The system aims to efficiently manage employee records, attendance, leave, and payroll while demonstrating core OOP concepts and frontend-backend integration.

---

## Features

* Add employee records
* View all employees
* Search employee by ID
* Update employee details
* Delete employee records
* Track attendance and leave
* Calculate salary based on employee type
* Handle overtime, bonus, tax, insurance, pension, and leave deduction
* Generate salary slips
* Store employee data locally using a JSON file
* Download salary slips

---

## Payroll Logic

### Employee Types and Deductions

| Type      | Tax | Insurance | Pension | Overtime Rate |
| --------- | --- | --------- | ------- | ------------- |
| Permanent | 10% | 5%        | 8%      | 300 per hour  |
| Part-Time | 5%  | 0%        | 0%      | 200 per hour  |
| Contract  | 8%  | 0%        | 0%      | 200 per hour  |

---

### Leave Policy

* One paid leave is allowed per month
* Additional absences are treated as unpaid

**Formula:**

* Absent Days = Total Days - Days Present
* Unpaid Days = Absent Days - Paid Leave
* Leave Deduction = (Base Salary / Total Days) × Unpaid Days

---

### Salary Calculation

* Gross Salary = Base Salary + Bonus + Overtime Pay
* Net Salary = Gross Salary - (Tax + Insurance + Pension + Leave Deduction)

---

## OOP Concepts Used

This project demonstrates the following Object-Oriented Programming concepts:

1. Classes and Objects
2. Functions (inline, default arguments, overloading)
3. Constructors and Destructors
4. Inheritance
5. Encapsulation
6. Abstraction
7. Pointers and Virtual Functions
8. Polymorphism
9. File Handling
10. Templates
11. Exception Handling
12. String Manipulation

---

## Employee Types

* Permanent Employee
* Part-Time Employee
* Contract Employee

---

## Technologies Used

* C++
* cpp-httplib
* nlohmann/json
* HTML
* CSS
* JavaScript

---

## Project Structure

```
Employee-management-system/
│
├── backend/
│   ├── server.cpp
│   ├── employee.cpp
│   ├── feature.cpp
│   ├── httplib.h
│   └── json.hpp
│
├── frontend/
│   ├── index.html
│   ├── style.css
│   └── script.js
│
├── README.md
└── .gitignore
```

---

## How to Run

### Backend

```bash
cd backend
g++ server.cpp -std=c++17 -o server
./server
```

### Frontend

Open another terminal:

```bash
cd frontend
python3 -m http.server 5500
```

Open in browser:
http://localhost:5500
