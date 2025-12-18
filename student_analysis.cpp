#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

/* =======================
   STUDENT STRUCTURE
   ======================= */
struct Student {
    int id;
    string name;
    string dept;
    int semester;
    float cgpa;
    int credits;
    int year;
};

/* =======================
   LINKED LIST NODE
   ======================= */
struct Node {
    Student data;
    Node* next;
};

/* =======================
   CREATE NEW NODE
   ======================= */
Node* createNode(Student s) {
    Node* newNode = new Node;
    newNode->data = s;
    newNode->next = NULL;
    return newNode;
}

/* =======================
   INSERT AT END
   ======================= */
void insertNode(Node*& head, Student s) {
    Node* newNode = createNode(s);

    if (head == NULL) {
        head = newNode;
        return;
    }

    Node* temp = head;
    while (temp->next != NULL)
        temp = temp->next;

    temp->next = newNode;
}

/* =======================
   COPY LINKED LIST
   ======================= */
Node* copyList(Node* head) {
    Node* newHead = NULL;
    Node* temp = head;

    while (temp != NULL) {
        insertNode(newHead, temp->data);
        temp = temp->next;
    }
    return newHead;
}

/* =======================
   BUBBLE SORT BY CGPA
   ======================= */
void bubbleSortCGPA(Node* head) {
    bool swapped;
    Node* ptr;

    do {
        swapped = false;
        ptr = head;

        while (ptr->next != NULL) {
            if (ptr->data.cgpa < ptr->next->data.cgpa) {
                swap(ptr->data, ptr->next->data);
                swapped = true;
            }
            ptr = ptr->next;
        }
    } while (swapped);
}

/* =======================
   INSERTION SORT BY YEAR
   ======================= */
Node* insertionSortByYear(Node* head) {
    Node* sorted = NULL;

    while (head != NULL) {
        Node* current = head;
        head = head->next;

        if (sorted == NULL || current->data.year < sorted->data.year) {
            current->next = sorted;
            sorted = current;
        } else {
            Node* temp = sorted;
            while (temp->next != NULL &&
                   temp->next->data.year <= current->data.year) {
                temp = temp->next;
            }
            current->next = temp->next;
            temp->next = current;
        }
    }
    return sorted;
}

/* =======================
   OUTPUT: RANKED BY CGPA
   ======================= */
void outputRankedByCGPA(Node* head) {
    ofstream fout("ranked_by_cgpa.txt");

    fout << "====================================================\n";
    fout << "        STUDENTS RANKED BY CGPA\n";
    fout << "====================================================\n";
    fout << "Rank | ID     | Name               | Dept | CGPA\n";
    fout << "----------------------------------------------------\n";

    int rank = 1;
    Node* temp = head;

    while (temp != NULL) {
        fout << setw(4) << rank++ << " | "
             << temp->data.id << " | "
             << setw(18) << left << temp->data.name << " | "
             << setw(4) << temp->data.dept << " | "
             << fixed << setprecision(2) << temp->data.cgpa << endl;

        temp = temp->next;
    }
    fout.close();
}

/* =======================
   OUTPUT: SORTED BY YEAR
   ======================= */
void outputSortedByYear(Node* head) {
    ofstream fout("sorted_by_enrollment.txt");

    int currentYear = -1;
    float sum = 0;
    int count = 0;

    Node* temp = head;

    while (temp != NULL) {
        if (temp->data.year != currentYear) {
            if (count > 0) {
                fout << "Average CGPA: "
                     << fixed << setprecision(2)
                     << (sum / count) << "\n\n";
            }

            currentYear = temp->data.year;
            fout << "Enrollment Year: " << currentYear << "\n";
            fout << "---------------------------------\n";
            sum = 0;
            count = 0;
        }

        fout << temp->data.id << " "
             << temp->data.name << " "
             << fixed << setprecision(2)
             << temp->data.cgpa << endl;

        sum += temp->data.cgpa;
        count++;

        temp = temp->next;
    }

    if (count > 0) {
        fout << "Average CGPA: "
             << fixed << setprecision(2)
             << (sum / count) << endl;
    }

    fout.close();
}

/* =======================
   FREE MEMORY
   ======================= */
void freeList(Node*& head) {
    while (head != NULL) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
}

/* =======================
   MAIN FUNCTION
   ======================= */
int main() {
    ifstream fin("students_data.txt");

    if (!fin) {
        cout << "Error opening input file!\n";
        return 1;
    }

    int total;
    fin >> total;

    Node* head = NULL;

    for (int i = 0; i < total; i++) {
        Student s;
        fin >> s.id;
        fin.ignore();
        getline(fin, s.name);
        fin >> s.dept >> s.semester >> s.cgpa >> s.credits >> s.year;
        insertNode(head, s);
    }

    fin.close();

    // Copy lists
    Node* listCGPA = copyList(head);
    Node* listYear = copyList(head);

    // Sorting
    bubbleSortCGPA(listCGPA);
    listYear = insertionSortByYear(listYear);

    // Outputs
    outputRankedByCGPA(listCGPA);
    outputSortedByYear(listYear);

    // Free memory
    freeList(head);
    freeList(listCGPA);
    freeList(listYear);

    return 0;
}
