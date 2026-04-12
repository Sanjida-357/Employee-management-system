const form = document.getElementById("salaryForm");
const result = document.getElementById("result");

form.addEventListener("submit", async function (e) {
    e.preventDefault();

    const data = {
        empID: parseInt(document.getElementById("empID").value),
        name: document.getElementById("name").value,
        daysPresent: parseInt(document.getElementById("daysPresent").value),
        totalDays: parseInt(document.getElementById("totalDays").value),
        leaveTaken: parseInt(document.getElementById("leaveTaken").value),
        baseSalary: parseFloat(document.getElementById("baseSalary").value),
        overtimeHours: parseFloat(document.getElementById("overtimeHours").value),
        bonus: parseFloat(document.getElementById("bonus").value)
    };

    try {
        const response = await fetch("http://localhost:8080/calculate", {
            method: "POST",
            headers: {
                "Content-Type": "application/json"
            },
            body: JSON.stringify(data)
        });

        const res = await response.json();

        result.innerHTML = `
            <p><strong>Employee ID:</strong> ${res.empID}</p>
            <p><strong>Name:</strong> ${res.name}</p>
            <p><strong>Overtime Pay:</strong> ${res.overtimePay.toFixed(2)}</p>
            <p><strong>Gross Salary:</strong> ${res.grossSalary.toFixed(2)}</p>
            <p><strong>Tax:</strong> ${res.tax.toFixed(2)}</p>
            <p><strong>Insurance:</strong> ${res.insurance.toFixed(2)}</p>
            <p><strong>Pension:</strong> ${res.pension.toFixed(2)}</p>
            <p><strong>Leave Deduction:</strong> ${res.leaveDeduction.toFixed(2)}</p>
            <p><strong>Attendance:</strong> ${res.attendance.toFixed(2)}%</p>
            <p><strong>Net Salary:</strong> ${res.netSalary.toFixed(2)}</p>
        `;
    } catch (err) {
        result.innerHTML = `<p>❌ Error connecting to backend</p>`;
    }
});