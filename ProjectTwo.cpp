//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Ambren Cavazos
// Version     : 1.0
// Description : Advising Assistance Program
//============================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

// Structure used to store course information
struct Course {
    string courseNumber;
    string courseTitle;
    vector<string> prerequisites;
};

// Node used in the binary search tree
struct Node {
    Course course;
    Node* left;
    Node* right;

    Node(Course aCourse) {
        course = aCourse;
        left = nullptr;
        right = nullptr;
    }
};

// Binary Search Tree useed to store and organize courses
class BinarySearchTree {
private:
    Node* root;

    void addNode(Node* node, Course course) {
        if (course.courseNumber < node->course.courseNumber) {
            if (node->left == nullptr) {
                node->left = new Node(course);
            }
            else {
                addNode(node->left, course);
            }
        }
        else {
            if (node->right == nullptr) {
                node->right = new Node(course);
            }
            else {
                addNode(node->right, course);
            }
        }
    }

    void inOrder(Node* node) {
        if (node != nullptr) {
            inOrder(node->left);
            cout << node->course.courseNumber << ", " << node->course.courseTitle << endl;
            inOrder(node->right);
        }
    }

public:
    BinarySearchTree() {0
        root = nullptr;
    }

    // Insert a course into the BST based on course number
    void Insert(Course course) {
        if (root == nullptr) {
            root = new Node(course);
        }
        else {
            addNode(root, course);
        }
    }

    // Search for a course using the course number
    Course Search(string courseNumber) {
        Node* current = root;

        while (current != nullptr) {
            if (current->course.courseNumber == courseNumber) {
                return current->course;
            }

            if (courseNumber < current->course.courseNumber) {
                current = current->left;
            }
            else {
                current = current->right;
            }
        }

        Course course;
        return course;
    }

    // Print courses in alphanumeric order
    void PrintCourseList() {
        inOrder(root);
    }
};

string toUpperCase(string text) {
    transform(text.begin(), text.end(), text.begin(), ::toupper);
    return text;
}

vector<string> splitLine(string line) {
    vector<string> tokens;
    string token;
    stringstream ss(line);

    while (getline(ss, token, ',')) {
        tokens.push_back(token);
    }

    return tokens;
}

// Read course data from the input file and load it into the BST
void loadCourses(string fileName, BinarySearchTree& courseTree) {
    ifstream file(fileName);

    if (!file.is_open()) {
        cout << "Error: Could not open file." << endl;
        return;
    }

    string line;

    while (getline(file, line)) {
        vector<string> tokens = splitLine(line);
        // Validate that each line contains at least a course number and title
        if (tokens.size() < 2) {
            cout << "Error: Invalid file format." << endl;
            continue;
        }

        Course course;
        course.courseNumber = toUpperCase(tokens[0]);
        course.courseTitle = tokens[1];
        // Store all prerequisite course numbers
        for (unsigned int i = 2; i < tokens.size(); ++i) {
            if (!tokens[i].empty()) {
                course.prerequisites.push_back(toUpperCase(tokens[i]));
            }
        }

        courseTree.Insert(course);
    }

    file.close();

    cout << "Courses loaded successfully." << endl;
}

// Display course title and prerequisites for a selected course
void printCourseInformation(BinarySearchTree& courseTree, string courseNumber) {
    courseNumber = toUpperCase(courseNumber);

    Course course = courseTree.Search(courseNumber);

    if (course.courseNumber.empty()) {
        cout << "Course not found." << endl;
        return;
    }

    cout << course.courseNumber << ", " << course.courseTitle << endl;

    cout << "Prerequisites: ";

    if (course.prerequisites.empty()) {
        cout << "None";
    }
    else {
        for (unsigned int i = 0; i < course.prerequisites.size(); ++i) {
            cout << course.prerequisites[i];

            if (i < course.prerequisites.size() - 1) {
                cout << ", ";
            }
        }
    }

    cout << endl;
}

int main() {
    BinarySearchTree courseTree;
    string fileName;
    string courseNumber;
    int choice = 0;

    cout << "Welcome to the course planner." << endl;

    // Main menu loop
    while (choice != 9) {
        cout << endl;
        cout << "1. Load Data Structure." << endl;
        cout << "2. Print Course List." << endl;
        cout << "3. Print Course." << endl;
        cout << "9. Exit" << endl;
        cout << "What would you like to do? ";
        cin >> choice;

        switch (choice) {
            // Load course data from file
        case 1:
            cout << "Enter file name: ";
            cin.ignore();
            getline(cin, fileName);
            loadCourses(fileName, courseTree);
            break;
            // Print all courses in alphanumeric order
        case 2:
            cout << "Here is a sample schedule:" << endl;
            cout << endl;
            courseTree.PrintCourseList();
            break;
            // Search for a specific course
        case 3:
            cout << "What course do you want to know about? ";
            cin >> courseNumber;
            printCourseInformation(courseTree, courseNumber);
            break;
            // Exit program
        case 9:
            cout << "Thank you for using the course planner!" << endl;
            break;

        default:
            cout << choice << " is not a valid option." << endl;
            break;
        }
    }

    return 0;
}