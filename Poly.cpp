#include "Poly.h"
#include <cmath>

// Default Constructor
Poly::Poly()
{

	head = new PolyNode(-1,0,NULL);
}


Poly::Poly(const std::vector<int>& deg, const std::vector<double>& coeff)
{
	head = new PolyNode(-1, 0, NULL);

	// error handling
	if (deg.empty() || coeff.empty()) {
		return;
	}

	PolyNode* p = new PolyNode(deg[0], coeff[0], NULL);

	head->next = p;

	// Loop through the vectors and add nodes for each term
	unsigned int i = 1; // start at index 1 since we already added the first term
	while (i < deg.size()) {
		p->next = new PolyNode(deg[i], coeff[i], NULL);
		// move to the next node
		p = p->next;
		// increment the index
		i++;
	}
}

// Destructor
Poly::~Poly() {
    PolyNode* temp = head;
    while (temp != NULL) {
        PolyNode* next = temp->next;
        delete temp;
        temp = next;
    }
}


void Poly::addMono(int i, double c) {
	PolyNode* prev = head;
	PolyNode* curr = prev->next;

	// Loop through the linked list until we find the correct position to insert the new term
	while (curr != NULL && curr->deg > i) {
		prev = curr;
		curr = curr->next;
	}

	// If a term with the same degree already exists, update its coefficient or remove it if the sum is zero
	if (curr != NULL && curr->deg == i) {
		curr->coeff += c;
		if (curr->coeff == 0) {
			prev->next = curr->next;
			delete curr;
		}
	}
	// If a term with the same degree does not exist, insert a new node in the correct position
	else {
		PolyNode* newNode = new PolyNode(i, c, curr);
		prev->next = newNode;
	}
}


void Poly::addPoly(const Poly& p) {
	PolyNode* p1 = head->next;  // Pointer to the first term of this polynomial
	PolyNode* p2 = p.head->next;  // Pointer to the first term of the other polynomial

	PolyNode* prev = head;  // Pointer to the previous node in this polynomial

	// Loop through both polynomials until we reach the end of one of them
	while (p1 != NULL && p2 != NULL) {
		if (p1->deg == p2->deg) {  // If the degrees match, add the coefficients
			p1->coeff += p2->coeff;
			prev = p1;
			p1 = p1->next;
			p2 = p2->next;
		}
		else if (p1->deg < p2->deg) {  // If the degree of this polynomial is less than the other polynomial, move to the next term of this polynomial
			prev = p1;
			p1 = p1->next;
		}
		else {  // If the degree of the other polynomial is less than this polynomial, insert a new node in this polynomial for the term in the other polynomial
			PolyNode* newNode = new PolyNode(p2->deg, p2->coeff, p1);
			prev->next = newNode;
			prev = newNode;
			p2 = p2->next;
		}
	}

	// If there are remaining terms in the other polynomial, add them to this polynomial
	while (p2 != NULL) {
		PolyNode* newNode = new PolyNode(p2->deg, p2->coeff, NULL);
		prev->next = newNode;
		prev = newNode;
		p2 = p2->next;
	}
}


void Poly::multiplyMono(int i, double c)
{
	// If c is zero, delete all nodes in the linked list
	if (c == 0) {
		PolyNode* temp = head->next;
		while (temp != NULL) {
			PolyNode* next = temp->next;
			delete temp;
			temp = next;
		}
		head->next = NULL;
	}

	// If the linked list is empty, do nothing
	else if (head->next == NULL) {
		return;
	}

	// Multiply each term in the polynomial by the given coefficient and degree
	else {
		PolyNode* current = head->next;
		while (current != NULL) {
			current->deg += i;
			current->coeff *= c;
			current = current->next;
		}
	}
}


void Poly::multiplyPoly(const Poly& p) {
	// Create a new polynomial to store the result of the multiplication
	Poly result;

	// Iterate through each monomial in the first polynomial
	for (PolyNode* it1 = head->next; it1 != nullptr; it1 = it1->next) {
		// Iterate through each monomial in the second polynomial
		for (PolyNode* it2 = p.head->next; it2 != nullptr; it2 = it2->next) {
			// Multiply the monomials and add them to the result polynomial
			double new_coeff = it1->coeff * it2->coeff;
			int new_deg = it1->deg + it2->deg;
			result.addMono(new_deg, new_coeff);
		}
	}

	// Replace the current polynomial with the result of the multiplication
	*this = result;
}



void Poly::duplicate(Poly& outputPoly) // Time complexity: O(n)
// The function duplicates the terms in this poly to outputPoly by creating new nodes and copying the data.
// The loop terminates when it has processed all the terms in this poly.
{
	PolyNode* opCurrent = outputPoly.head; // Current node in outputPoly

	// Loop through the terms in this poly
	for (PolyNode* pCurrent = this->head->next; pCurrent != NULL; pCurrent = pCurrent->next) {
		// Create a new node with the same degree and coefficient as the current node in this poly
		PolyNode* newNode = new PolyNode(pCurrent->deg, pCurrent->coeff, NULL);

		if (opCurrent->next == NULL) {
			// If outputPoly has no more nodes, set the new node as the next node in outputPoly
			opCurrent->next = newNode;
		}
		else {
			// If outputPoly has more nodes, insert the new node after the current node
			newNode->next = opCurrent->next;
			opCurrent->next = newNode;
		}

		// Move to the next node in outputPoly
		opCurrent = opCurrent->next;
	}
}

int Poly::getDegree() const // Time complexity: O(1)
// Retrieves the degree of the first non-head node in the linked list, which takes constant time regardless of the size of the polynomial.
{
	int degree;
	if (head->next == NULL) {
		degree = -1; // If the linked list is empty, set degree to -1
	}
	else {
		PolyNode* firstNode = head->next; // Set a pointer to the first non-head node
		while (firstNode->next != NULL) { // Traverse the linked list until the last node is reached
			firstNode = firstNode->next;
		}
		degree = firstNode->deg; // Retrieve the degree of the last node as the degree of the polynomial
	}
	return degree;
}

int Poly::getTermsNo() const // Time complexity: O(n)
// Counts the number of nodes in the linked list, which is directly proportional to the number of terms in the polynomial.
{
	int terms = 0;
	PolyNode* temp = head->next; // Set a pointer to the first non-head node
	while (temp != NULL) { // Traverse the linked list until the end is reached (NULL)
		terms++; // Increment the terms counter for each node visited
		temp = temp->next;
	}
	return terms; // Return the total number of terms in the polynomial
}


double Poly::evaluate(double x)
{
	// Iterate through nodes and add to polySum the result of coeff*x^(deg+1)
	double polySum = 0; // Initialize the sum of polynomial terms to 0
	PolyNode* node = head->next; // Start from the first non-head node

	while (node != NULL) { // Loop through each node in the linked list
		double term = (node->coeff) * pow(x, (node->deg + 1)); // Compute coeff*x^(deg+1)
		polySum += term; // Add the term to polySum
		node = node->next; // Move to the next node
	}

	return polySum; // Return the final evaluation of the polynomial
}

std::string Poly::toString()
{
	std::string polynomial; // Initialize an empty string to store the polynomial as a string
	PolyNode* node; // Create a pointer to traverse the linked list of polynomial terms
	double* tempCoeff = new double[getTermsNo()]; // Create a temporary array to store coefficients
	int* tempDeg = new int[getTermsNo()]; // Create a temporary array to store degrees

	int i = 0; // Initialize the index for temporary arrays

	for (node = head->next; node->next != NULL; node = node->next) {
		// Copy coefficients and degrees from polynomial terms to temporary arrays
		tempCoeff[i] = node->coeff;
		tempDeg[i] = node->deg;
		i++;
	}

	// Loop through the temporary arrays to construct the string representation of the polynomial
	polynomial.append("Degree: %d; ", getDegree()); // Add the degree of the polynomial to the string
	for (int j = 0; j < i; j++) {
		polynomial.append("Coefficient for Degree %d: %lf; ", tempDeg[j], tempCoeff[j]); // Add each coefficient and degree to the string
	}

	return polynomial; // Return the polynomial as a string
}