#define _CRT_SECURE_NO_DEPRECATE
#include<iostream>
#include<fstream>
#include<vector>
#include<string.h>
#include<stack>
#include<ctime>
#include <string>
using namespace std;

class contact
{
public:

	long ph;
	char name[20], add[20], email[30];
	time_t callTime;
	void create_contact()
	{
		cout << "Phone: ";
		cin >> ph;
		cout << "Name: ";
		getline(cin, name);
		cout << "Address: ";
		getline(cin, add);

		cout << "Email address: ";
		cin.ignore();
		cin >> email;

		cout << "\n";
	}

	void show_contact()
	{
		cout << endl << "Phone #: " << ph;
		cout << endl << "Name: " << name;
		cout << endl << "Address: " << add;
		cout << endl << "Email Address : " << email;
	}

	char* getName()
	{
		return name;
	}

	void makecall() {
		callTime = time(0);
	}
};

fstream fp;
contact cont;

void save_contact()
{
	fp.open("contactBook.txt", ios::out | ios::app);
	if (!fp.is_open()) {
		cout << "Error opening file for writing!" << endl;
		return; // Exit or handle the error as needed
	}

	cont.create_contact();
	fp.write((char*)&cont, sizeof(contact));
	fp.close();
	cout << endl << endl << "Contact Has Been Sucessfully Created...";

}

void show_all_contacts()
{
	cout << "\n\t\t================================\n\t\t\tLIST OF CONTACTS\n\t	\t================================\n";
	fp.open("contactBook.txt", ios::in);
	if (!fp.is_open()) {
		cout << "Error opening file for writing!" << endl;
		return; // Exit or handle the error as needed
	}

	while (fp.read((char*)&cont, sizeof(contact)))
	{
		cont.show_contact();
		cout << endl << "=================================================\n" << endl;
	}
	fp.close();
}

void display_contact(string nam)
{
	bool found;
	int ch;

	found = false;
	fp.open("contactBook.txt", ios::in);
	if (!fp.is_open()) {
		cout << "Error opening file for writing!" << endl;
		return; // Exit or handle the error as needed
	}

	while (fp.read((char*)&cont, sizeof(contact)))
	{
		if (strcmp(cont.name,nam.c_str())==0)
		{
			cont.show_contact();
			found = true;
		}
	}
	fp.close();
	if (found == false) {
		cout << "\n\nNo record found..." << endl;
	}
}


class contactNode {
public:
	contact cont;
	contactNode* next;
	contactNode* prev;

	contactNode(contact other) {
		cont.ph = other.ph;
		strcpy(cont.name, other.name);
		strcpy(cont.add, other.add); 
		strcpy(cont.email, other.email);
	}

	contactNode& operator=(const contact& other) {
		if (&cont != &other) {
			cont.ph = other.ph;
			strcpy(cont.name, other.name);
			strcpy(cont.add, other.add);
			strcpy(cont.email, other.email);
		}
		return *this;
	}
};


class contact_linkedList {

public:
	contactNode* head = NULL;

	contact_linkedList() {
		fp.open("contactBook.txt", ios::in);
		while (fp.read((char*)&cont, sizeof(contact)))
		{
			insertNode(cont);
		}
		fp.close();
	}

	void insertNode(contact d) {

		contactNode* newNode = new contactNode(d);

		if (head == NULL) {
			head = newNode;
			head->next = NULL;
			return;
		}

		contactNode* temp;
		temp = head;

		while (temp->next != NULL) {
			temp = temp->next;
		}

		temp->next = newNode;
	}

	void display() {
		contactNode* temp = head;
		while (temp != NULL) {
			temp->cont.show_contact();
			cout << endl;
			temp = temp->next;
		}
	}

	vector<contact> toArray() {
		vector<contact> array;
		contactNode* temp = head;
		while (temp != NULL) {
			array.push_back(temp->cont);
			temp = temp->next;
		}
		return array;
	}

	~contact_linkedList() {
		fp.open("contactBook.txt", ios::out | ios::trunc);
		contactNode* temp = head;
		while (temp != NULL) {
			fp.write((char*)&temp->cont, sizeof(contact));
			temp = temp->next;
		}

		fp.close();

	}
};

class contactTreeNode {
public:
	contact cont;
	contactTreeNode* right;
	contactTreeNode* left;

	contactTreeNode(contact other) {
		cont.ph = other.ph;
		strcpy(cont.name, other.name);
		strcpy(cont.add, other.add);
		strcpy(cont.email, other.email);
	}

};


class contact_tree {


	// Recursive function to build a balanced BST from an array
	contactTreeNode* sortedArrayToBST(vector<contact>& arr, int start, int end) {
		if (start > end) {
			return nullptr;
		}

		int mid = (start + end) / 2;
		contactTreeNode* newNode = new contactTreeNode(arr[mid]);

		newNode->left = sortedArrayToBST(arr, start, mid - 1);
		newNode->right = sortedArrayToBST(arr, mid + 1, end);

		return newNode;
	}


	// Recursive function to insert a contact into the BST
	contactTreeNode* insertRecursive(contactTreeNode* node, contact d) {
		if (node == nullptr) {
			return new contactTreeNode(d);
		}

		if (d.name < node->cont.name) {
			node->left = insertRecursive(node->left, d);
		}
		else if (d.name > node->cont.name) {
			node->right = insertRecursive(node->right, d);
		}

		return node;
	}


	// function to delelte contact on BST
	contactTreeNode* deleteRecursive(contactTreeNode* current, string value) {
		if (current == NULL) {
			cout << "Value Does Not Exist.\n\n";
			return current;
		}

		if (value > current->cont.getName()) {
			current->right = deleteRecursive(current->right, value);
		}
		else if (value < current->cont.getName()) {
			current->left = deleteRecursive(current->left, value);
		}
		else {
			if (current->left == NULL) {
				current = current->right;
				return current;
			}

			if (current->right == NULL) {
				current = current->left;
				return current;
			}

			contactTreeNode* temp = findMin(current->right);
			current->cont = temp->cont;
			current->right = deleteRecursive(current->right, temp->cont.name);
		}

		return current;
	}


	contactTreeNode* findMin(contactTreeNode* node) {
		while (node->left != nullptr) {
			node = node->left;
		}
		return node;
	}


	// Helper function to convert BST to linked list
	void bstToLinkedListHelper(contactTreeNode* node, contactNode*& head) {
		if (node == nullptr) {
			return;
		}

		
		bstToLinkedListHelper(node->left, head);

		// Create a new contactNode for the current node
		contactNode* newNode = new contactNode(node->cont);

		newNode->next = head;
		head = newNode;

		bstToLinkedListHelper(node->right, head);
	}


	// Recursive function to find a contact in the BST
	contactTreeNode* findRecursive(contactTreeNode* node, string d) {
		if (node == nullptr || strcmp(node->cont.name,d.c_str()) == 0) {
			return node;
		}

		if (d < node->cont.name) {
			return findRecursive(node->left, d);
		}
		else if (d > node->cont.name) {
			return findRecursive(node->right, d);
		}

	}


	//Recursive function for editing contact
	bool editRecursive(contactTreeNode* node, string name, contact& newCont) {
		if (node == NULL) {
			return 0;
		}

		if (strcmp(node->cont.name, name.c_str()) == 0) {
			node->cont = newCont;
			return 1;
		}
		else if (strcmp(node->cont.name, name.c_str()) < 0) {
			return editRecursive(node->left, name, newCont);
		}
		else if (strcmp(node->cont.name, name.c_str()) > 0) {
			return editRecursive(node->right, name, newCont);
		}

	}


	// Recursive function for inorder traversal
	void inorderTraversal(contactTreeNode* node) {
		if (node != nullptr) {
			inorderTraversal(node->left);
			node->cont.show_contact();  
			cout << endl;
			inorderTraversal(node->right);
		}
	}


	
public:
	contactTreeNode* root;

	contact_tree() {}

	void listtoBST(contact_linkedList& linkedList) {
		vector<contact> array = linkedList.toArray();
		int size = array.size();
		root = sortedArrayToBST(array, 0, size - 1);
	}

	void insert(contact d) {
		root = insertRecursive(root, d);
	}

	void dlt(string d) {
		root = deleteRecursive(root, d);
	}

	void inorder() {
		inorderTraversal(root);
	}

	void bstToLinkedList(contact_linkedList& linkedList) {
		linkedList.head = NULL;

		bstToLinkedListHelper(root, linkedList.head);
	}

	contactTreeNode* findContact(string name) {
		return findRecursive(root, name);
	}

	bool editContact(string name, contact& newContact) {
		return editRecursive(root, name, newContact);
	}

	

};


class contactManager {

	contact_linkedList cl1;
	contact_tree ct1;

	stack<contact> historyStack;
public:
	contactManager() {
		ct1.listtoBST(cl1);
	}

	void insertContact() {
		/*contact d;
		d.create_contact();*/
		save_contact();
		ct1.insert(d);
	}

	void deleteContact(string d) {
		ct1.dlt(d);
	}

	void showContact() {
		ct1.inorder();
	}

	void exit() {
		ct1.bstToLinkedList(cl1);
	}

	void editContact(string name) {
		contact newContact;
		cout << "\n\nEnter details for editing: \n\n";
		newContact.create_contact();

		if (ct1.editContact(name, newContact)) {
			cout << "Contact edited successfully." << endl;
		}
		else {
			cout << "Contact not found. Unable to edit." << endl;
		}
	}

	void makecall()
	{

		string name;
		int choice;

		cout << "\nHow would you like to make the call?\n";
		cout << "1. Enter by Name\n";
		cout << "2. Enter by Phone Number\n";
		cout << "Enter your choice: ";
		cin >> choice;

		if (choice == 1) {
			cout << " \nEnter the name of the contact you want call: ";
			cin >> name;

			contactTreeNode* foundContact = ct1.findContact(name);
			if (foundContact != nullptr) {
				foundContact->cont.makecall();
				historyStack.push(foundContact->cont);
				cout << "Call made to " << name << " successfully." << endl;
			}
			else {
				cout << "Contact not found. Unable to make a call." << endl;
			}
		}
		else if (choice == 2) {
			int phoneNumber;
			cout << "\nEnter the phone number of the contact you want to call: ";
			cin >> phoneNumber;
		//	display_contact(phoneNumber);
			contactTreeNode* foundContact = ct1.findContact(to_string(phoneNumber));
			if (foundContact != nullptr) {
				foundContact->cont.makecall();
				historyStack.push(foundContact->cont);
				cout << "Call made to " << foundContact->cont.getName() << " successfully." << endl;
			}
			else {
				cout << "Contact not found. Unable to make a call." << endl;
			}
		}
		else {
			cout << "Invalid choice. Please enter 1 or 2." << endl;
		}
	}

	void call_log()
	{
		cout << "\n\n\t\t================================\n\t\t\tCALL HISTORY\n\t\t================================\n";

		if (historyStack.empty())
		{
			cout << "\n\tNo call records Found\t\n";
		}

		stack<contact> temp = historyStack;
		while (!temp.empty())
		{
			contact c;
			c = temp.top();
			c.show_contact();
			//cout << endl << "Call Time: " << ctime(&call.callTime);
			//cout << endl << "=================================================\n" << endl;
			temp.pop();

		}
		
	}
};



int main()
{
	contactManager cms;
	string name;
	string nam;

	cout << "\n\n";

	while (1)
	{
		int ch;
		cout << "\t\t **** Welcome to Contact Management System ****";
		cout << "\n\t\t\t\t\tMAIN MENU\n\n\t\t=====================\n\n\t\t[1] Add a new Contact\n\t\t[2] List all Contacts\n\t\t[3] Search for contact\n\t\t[4] Edit a Contact\n\t\t[5] Delete a Contact\n\t\t[6] Call\n\t\t[0] Exit\n\t\t=================\n\t\t";
		cout << "Enter the choice:";
		cin >> ch;

		switch (ch)
		{
		case 0:
			cout << "\n\n\t\tThank you for using CMS...\n\n";
			cms.exit();
			return 0;

		case 1:
			cms.insertContact();
			break;

		case 2:
			cms.showContact();
			break;

		case 3:
			
			cout << "\n\n\tName: ";
			cin.ignore();
			cin >> nam;
			display_contact(nam);
			break;

		case 4:
			cout << "\nEnter name of the contact you want to edit: ";
			cin.ignore();
			cin >> name;
			cms.editContact(name);
			break;

		case 5:
			cout << "\nEnter Name: ";
			cin.ignore();
			cin >> name;
			cms.deleteContact(name);
			break;

		case 6:
		
			cms.makecall();

		default:
			break;
		}

		int opt;
		cout << "\n\n\n..::Enter the Choice:\n\n\t[1] Main Menu\t\t[0] Exit\n";
		cin >> opt;

		switch (opt)
		{
		case 1:
			continue;

		case 0:
			cms.exit();
			return 0;
		}
	}
	return 0;
}