Overview :-
This project is a comprehensive command-line Banking Management System implemented in C (with supportive header files and a Makefile) designed to simulate the core functionality of bank operations, including user accounts, loans, transactions, feedback, and administrative tasks. 
It’s perfect for understanding how banks manage data, user interactions, and file-based persistence.

Features :- 
-> Account creation & management — Create, modify, and delete customer or employee accounts with personal details and authentication.
-> User roles — Distinct modules for Customers, Employees, Managers, and Admin, each with role-specific privileges.
-> Transactions & Loans — Support for creating, approving, and tracking loans; record transactions to reflect deposits, withdrawals and transfers.
-> Persistence via text-files — Data storage is handled via .txt files (accounts, users, loans, feedback, transactions) making the system simple and transparent.
-> Feedback System — Customers can submit feedback, and employees or managers can view/respond, helping simulate customer-service workflows.
-> Structured Code Base — Header files for each major module (account.h, loan.h, user.h, etc.), separation of concerns, and a Makefile to build the project easily.

Why This Project Matters?
-> Real-world simulation of banking operations gives you a practical overview of how banking software manages accounts, data integrity, and role-based access.
-> File-based persistence teaches how to handle data without requiring a full database setup — ideal for introductory systems.
-> C programming with modular design helps improve structured programming skills, understanding of pointers, file I/O, and memory handling.
-> Great for learning or demonstrating how an entire system (front-end commands + back-end storage + business logic) works end-to-end.

How to Use?
Admin: Manage system-wide configurations and view system status.
Manager: Oversee employees and approve major loans and accounts.
Employee: Assist customers with account issues, record feedback, process routine loans.
Customer: View account details, request loans, make transactions, provide feedback.
Each role has a menu interface for its workflows; follow on-screen prompts.
Text files update automatically as you make changes (so you can check raw data after operations).

Future Enhancements :-
-> Replace text-file storage with a SQL database (e.g., MySQL) for better scalability and concurrency.
-> Add unit tests to validate each module (accounts, loans, transactions).
-> Introduce GUI interface or web-frontend for easier user interaction.
-> Add logging and auditing for each transaction and system action.
-> Improve security features: encrypted passwords, secure sessions, and rollback for failed operations.

Contribution :-
-> Feel free to open issues or submit pull requests — whether for bug fixes, feature ideas, or refactoring.
-> Before contributing, make sure your code builds cleanly with make and that your changes include clear comments and documentation.
