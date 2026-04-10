const form = document.getElementById("employeeForm");
const table = document.getElementById("employeeTable");

// Load existing data
let employees = JSON.parse(localStorage.getItem("employees")) || [];

// Display employees
function displayEmployees() {
    table.innerHTML = "";

    employees.forEach((emp, index) => {
        let row = `
        <tr>
            <td>${emp.id}</td>
            <td>${emp.name}</td>
            <td>${emp.position}</td>
            <td>${emp.salary}</td>
            <td>${emp.attendance}</td>
            <td>${emp.leave}</td>
            <td>
                <button class="delete" onclick="deleteEmployee(${index})">Delete</button>
            </td>
        </tr>
        `;
        table.innerHTML += row;
    });
}

// Add employee
form.addEventListener("submit", function(e) {
    e.preventDefault();

    const employee = {
        id: document.getElementById("empId").value,
        name: document.getElementById("name").value,
        position: document.getElementById("position").value,
        salary: document.getElementById("salary").value,
        attendance: document.getElementById("attendance").value || 0,
        leave: document.getElementById("leave").value || 0
    };

    employees.push(employee);
    localStorage.setItem("employees", JSON.stringify(employees));

    form.reset();
    displayEmployees();
});

// Delete employee
function deleteEmployee(index) {
    employees.splice(index, 1);
    localStorage.setItem("employees", JSON.stringify(employees));
    displayEmployees();
}

// Initial display
displayEmployees();