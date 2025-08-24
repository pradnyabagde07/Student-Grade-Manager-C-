#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>   // for setw
using namespace std;

class Student {
private:
    int rollNo;
    string name;
    int marks[5];
    int total;
    float percentage;
    char grade;

public:
    // Constructor
    Student(int r = 0, string n = "") {
        this->rollNo = r;
        this->name = n;
        this->total = 0;
        this->percentage = 0.0;
        this->grade = 'F';
    }

    // Input student details
    void input() {
        cout << "Enter Roll Number: ";
        cin >> rollNo;
        cin.ignore();
        cout << "Enter Name: ";
        getline(cin, name);
        cout << "Enter marks for 5 subjects: ";
        total = 0;
        for (int i = 0; i < 5; i++) {
            cin >> marks[i];
            total += marks[i];
        }
        percentage = total / 5.0;
        calculateGrade();
    }

    // Calculate grade
    void calculateGrade() {
        if (percentage >= 90) grade = 'A';
        else if (percentage >= 80) grade = 'B';
        else if (percentage >= 70) grade = 'C';
        else if (percentage >= 60) grade = 'D';
        else grade = 'F';
    }

    // Display student in table row
    void displayRow() {
        cout << left << setw(10) << rollNo
             << setw(12) << name;
        for (int i = 0; i < 5; i++) {
            cout << setw(4) << marks[i];
        }
        cout << setw(8) << total
             << setw(6) << fixed << setprecision(1) << percentage
             << setw(6) << grade << endl;
    }

    // Save to file
    void save(ofstream &out) {
        out << rollNo << "," << name << ",";
        for (int i = 0; i < 5; i++) out << marks[i] << ",";
        out << total << "," << percentage << "," << grade << "\n";
    }

    // Load from file
    bool load(string line) {
        string temp;
        int index = 0;
        for (size_t i = 0; i < line.size(); i++) {
            if (line[i] == ',') index++;
            else {
                if (index == 0) rollNo = rollNo * 10 + (line[i] - '0');
                else if (index == 1) name += line[i];
                else if (index >= 2 && index <= 6) {
                    if (isdigit(line[i])) marks[index - 2] = marks[index - 2] * 10 + (line[i] - '0');
                }
                // total, percentage, grade recalculated instead of loading
            }
        }
        total = 0;
        for (int i = 0; i < 5; i++) total += marks[i];
        percentage = total / 5.0;
        calculateGrade();
        return true;
    }

    int getRollNo() { return rollNo; }
    string getName() { return name; }
    char getGrade() { return grade; }
    float getPercentage() { return percentage; }
};

// Linked list node
struct Node {
    Student data;
    Node *next;
    Node(Student s) : data(s), next(nullptr) {}
};

// Linked list class
class StudentList {
private:
    Node *head;
public:
    StudentList() { head = nullptr; }

    void addStudent() {
        Student s;
        s.input();
        Node *newNode = new Node(s);
        if (!head) head = newNode;
        else {
            Node *temp = head;
            while (temp->next) temp = temp->next;
            temp->next = newNode;
        }
    }

    void viewStudents() {
        if (!head) {
            cout << "No records found.\n";
            return;
        }

        cout << "-----------------------------------------------------------------------------\n";
        cout << left << setw(10) << "Roll No"
             << setw(12) << "Name"
             << "Marks(5)       "
             << setw(8) << "Total"
             << setw(6) << "%"
             << setw(6) << "Grade" << endl;
        cout << "-----------------------------------------------------------------------------\n";

        Node *temp = head;
        while (temp) {
            temp->data.displayRow();
            temp = temp->next;
        }
        cout << "-----------------------------------------------------------------------------\n";
    }

    void saveToFile(string filename) {
        ofstream out(filename);
        if (!out) {
            cout << "Error saving file.\n";
            return;
        }
        Node *temp = head;
        while (temp) {
            temp->data.save(out);
            temp = temp->next;
        }
        out.close();
        cout << "Data saved to " << filename << endl;
    }
};

int main() {
    StudentList list;
    int choice;

    do {
        cout << "\n--- Student Record & Grade Management System ---\n";
        cout << "1. Add Student\n";
        cout << "2. View All Students\n";
        cout << "3. Save to File\n";
        cout << "4. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: list.addStudent(); break;
            case 2: list.viewStudents(); break;
            case 3: list.saveToFile("students.txt"); break;
            case 4: cout << "Exiting...\n"; break;
            default: cout << "Invalid choice!\n";
        }
    } while (choice != 4);

    return 0;
}
