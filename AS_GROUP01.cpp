//Milestone 2
//Andrew Qin 132244195
//Harpreet Kaur 115028201
//Santiago Osejo 134962208
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <occi.h>
#include <iomanip>


using oracle::occi::Environment;
using oracle::occi::Connection;
using namespace oracle::occi;
using namespace std;



struct Employee
{
    int  employeeNumber;
    char lastName[50];
    char firstName[50];
    char extension[10];
    char email[100];
    char officecode[10];
    int  reportsTo[100];
    char jobTitle[50];
};



int menu(void) {
    const int max = 5, min = 0;
    int selection;
    bool flag = false;
    cout << endl << "*********************HR Menu*********************" << endl;
    cout << "1)    Find Employee" << endl;
    cout << "2)    Employees Report" << endl;
    cout << "3)    Add Employee" << endl;
    cout << "4)    Update Employee" << endl;
    cout << "5)    Remove Employee" << endl;
    cout << "0)    Exit" << endl;
    cout << "Select an option between " << min << " and " << max << ": ";
    while (!flag)
    {
        cin >> selection;
        if (!cin || selection < min || selection > max)
        {
            cout << "Wrong Choice. Try again: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        else flag = true;;
    }
    return selection;
}



int findEmployee(Connection* conn, int employeeNumber, struct Employee* emp) {
    int returnValue = 0;



    
        Statement* stmtJoin = nullptr;
        stmtJoin = conn->createStatement("SELECT EMPLOYEENUMBER,LASTNAME,FIRSTNAME,EXTENSION,EMAIL,OFFICECODE,REPORTSTO,JOBTITLE from EMPLOYEES");
        ResultSet* rsJoin = stmtJoin->executeQuery();

        while (rsJoin->next()) {    // if (a) result(s) exist(s), store data to structure
            if (employeeNumber == rsJoin->getInt(1)) {
                emp->employeeNumber = rsJoin->getInt(1);
                strcpy(emp->lastName, rsJoin->getString(2).c_str());
                strcpy(emp->firstName, rsJoin->getString(3).c_str());
                strcpy(emp->extension, rsJoin->getString(4).c_str());
                strcpy(emp->email, rsJoin->getString(5).c_str());
                strcpy(emp->officecode, rsJoin->getString(6).c_str());
                emp->reportsTo[0] = rsJoin->getInt(7);
                strcpy(emp->jobTitle, rsJoin->getString(8).c_str());

                returnValue = 1;
            }
        }
        conn->terminateStatement(stmtJoin);

    
    
    return returnValue;
}




void displayEmployee(Connection* conn, struct Employee emp) {
    int number;
    std::cout << "Enter an Employee Number: ";
    std::cin >> number;
    if (findEmployee(conn, number, &emp) == 0) {
        std::cout << "Employee " << number << " does not exist." << std::endl;
    }
    else
    {
        std::cout << std::endl << "-------------- Employee Information -------------" << std::endl;
        std::cout << "Employee Number: " << emp.employeeNumber << std::endl;
        std::cout << "Last Name: " << emp.lastName << std::endl;
        std::cout << "First Name: " << emp.firstName << std::endl;
        std::cout << "Extension: " << emp.extension << std::endl;
        std::cout << "Email: " << emp.email << std::endl;
        std::cout << "Office Code: " << emp.officecode << std::endl;
        std::cout << "Manager ID: " << emp.reportsTo[0] << std::endl;
        std::cout << "Job Title: " << emp.jobTitle << std::endl;
    };

}




void displayAllEmployees(Connection* conn) {
    Statement* stmtJoin = nullptr;
    stmtJoin = conn->createStatement("SELECT e.employeenumber, e.firstname || ' ' || e.lastname, e.email, o.phone, e.extension, e2.firstname || ' ' || e2.lastname \n"
        "FROM employees e \n"
        "JOIN offices o ON e.officecode = o.officecode \n"
        "LEFT JOIN employees e2 ON e.reportsto = e2.employeenumber \n"
        "order by e.employeenumber \n"
    );
    ResultSet* rsJoin = nullptr;
    rsJoin = stmtJoin->executeQuery();

    if (!rsJoin->next()) {
        // if the result set is empty
        cout << "There is no employees’ information to be displayed." << endl;
    }
    else {
        // if the result set in not empty
        std::cout << "-----   ----------------     ---------------------------------   --------------  --------- ------------" << std::endl;
        std::cout << "ID       Employee Name       Email                                   Phone       Extension Manager Name" << std::endl;
        std::cout << "-----   ----------------     ---------------------------------   --------------  --------- ------------" << std::endl;
        do {
            std::cout << rsJoin->getInt(1) << "    " << std::left << std::setw(15) << rsJoin->getString(2) << "     " << std::left << std::setw(33) << rsJoin->getString(3) << "   " << std::left << std::setw(9) << rsJoin->getString(4) << "  " << std::left << std::setw(9) << rsJoin->getString(5) << " " << std::left << std::setw(8) << rsJoin->getString(6) << std::endl;
        } while (rsJoin->next()); //if there is more rows, iterate
    }

    conn->terminateStatement(stmtJoin);
}

void getEmployee(struct Employee* emp) {
    strcpy(emp->officecode, "1");
    emp->reportsTo[0] = 1002;

    std::cout << "-------------- New Employee Information -------------" << std::endl;
    std::cout << "Employee Number: ";
    std::cin >> emp->employeeNumber;
    std::cout << "Last Name: ";
    std::cin >> emp->lastName;
    std::cout << "First Name: ";
    std::cin >> emp->firstName;
    std::cout << "Extension: ";
    std::cin >> emp->extension;
    std::cout << "Email: ";
    std::cin >> emp->email;
    std::cout << "Office Code: " << emp->officecode << std::endl;
    std::cout << "Manager ID: " << emp->reportsTo[0] << std::endl;
    std::cout << "Job Title: ";
    std::cin >> emp->jobTitle;
};

void insertEmployee(Connection* conn, struct Employee emp) {
    getEmployee(&emp);
    if (findEmployee(conn, emp.employeeNumber, &emp) == 0) {
           Statement* stmtJoin = nullptr;
           stmtJoin = conn->createStatement("INSERT INTO EMPLOYEES (EMPLOYEENUMBER,LASTNAME,FIRSTNAME,EXTENSION,EMAIL,OFFICECODE,REPORTSTO,JOBTITLE) \n"

               "VALUES (:1,:2,:3,:4,:5,:6,:7,:8)"
           );
           stmtJoin->setInt(1, emp.employeeNumber);
           stmtJoin->setString(2, emp.lastName);
           stmtJoin->setString(3, emp.firstName);
           stmtJoin->setString(4, emp.extension);
           stmtJoin->setString(5, emp.email);
           stmtJoin->setString(6, emp.officecode);
           stmtJoin->setInt(7, emp.reportsTo[0]);
           stmtJoin->setString(8, emp.jobTitle);

           stmtJoin->executeUpdate();

           conn->commit();
           conn->terminateStatement(stmtJoin);


           std::cout << "The new employee is added successfully." << std::endl;
    }
    else {
        std::cout << "An employee with the same employee number exists." << std::endl;
    }
};

void updateEmployee(Connection* conn, int employeeNumber) {
    Employee emp{};
    if (findEmployee(conn, employeeNumber, &emp) == 1) {
        
        std::cout << "Last Name: " << emp.lastName << std::endl;
        std::cout << "First Name: " << emp.firstName << std::endl;
        std::cout << "Extension: ";
        std::cin >> emp.extension;

        Statement* stmtJoin = nullptr;
        stmtJoin = conn->createStatement("UPDATE EMPLOYEES SET EXTENSION = :1 WHERE EMPLOYEENUMBER = :2");

        stmtJoin->setString(1, emp.extension);
        stmtJoin->setInt(2, emp.employeeNumber);
        

        stmtJoin->executeUpdate();


        conn->commit();
        conn->terminateStatement(stmtJoin);

        std::cout << "The employee's extension is updated successfully." << std::endl;



    }
    else {
        std::cout << "The employee with ID " << employeeNumber << " does not exist." << std::endl;
    }

};

void deleteEmployee(Connection* conn, int employeeNumber) {
    Employee emp{};
    if (findEmployee(conn, employeeNumber, &emp) == 1) {


        Statement* stmtJoin = nullptr;
        stmtJoin = conn->createStatement("DELETE FROM EMPLOYEES WHERE EMPLOYEENUMBER = :1");

        stmtJoin->setInt(1, emp.employeeNumber);

        stmtJoin->executeUpdate();


        conn->commit();
        conn->terminateStatement(stmtJoin);

        std::cout << "The employee with ID " << emp.employeeNumber << " is deleted successfully." << std::endl;



    }
    else {
        std::cout << "The employee with ID " << employeeNumber << " does not exist." << std::endl;
    }


};

int main(void)
{
    /* OCCI Variables */
    Environment* env = nullptr;
    Connection* conn = nullptr;



    /* Used Variables */
    bool exit = false;
    string user, pass;
    string  constr = "myoracle12c.senecacollege.ca:1521/oracle12c";
    Employee emp{};



    try {
        cout << "Enter Your Username: ";
        cin >> user;
        cout << "Enter Your Password: ";
        cin >> pass;
        env = Environment::createEnvironment(Environment::DEFAULT);
        conn = env->createConnection(user, pass, constr);
        cout << "Connection is Successful!" << endl;




        while (!exit)
        {

            switch (menu())
            {
            case 1:
                displayEmployee(conn, emp);
                break;
            case 2:
                displayAllEmployees(conn);
                break;
            case 3:
                insertEmployee(conn, emp);
                break;
            case 4:
                std::cout << "Employee Number: ";
                std::cin >> emp.employeeNumber;
                updateEmployee(conn, emp.employeeNumber);
                break;
            case 5:
                std::cout << "Employee Number: ";
                std::cin >> emp.employeeNumber;
                deleteEmployee(conn, emp.employeeNumber);
                break;
            case 0:
                exit = true;
                break;
            }
        }





        env->terminateConnection(conn);
        Environment::terminateEnvironment(env);
    }
    catch (SQLException& sqlExcp) {
        cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
    }
    return 0;
}
