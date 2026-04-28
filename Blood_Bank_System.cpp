#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// Helper functions
bool isNumber(const char* str) {
	int i = 0;
	while (str[i] != '\0') {
		if (str[i] < '0' || str[i] > '9') {
			return false;
		}
		i++;
	}
	return true;
}


bool isValidDate(const char* date) {
    // Check format YYYY-MM-DD
    int len = 0;
    while (date[len] != '\0') len++;
    if (len != 10) return false;
    
    // Check dashes at positions 4 and 7
    if (date[4] != '-' || date[7] != '-') return false;
    
    // Extract year, month, day
    int year = 0, month = 0, day = 0;
    for (int i = 0; i < 4; i++) {
        if (date[i] < '0' || date[i] > '9') return false;
        year = year * 10 + (date[i] - '0');
    }
    for (int i = 5; i < 7; i++) {
        if (date[i] < '0' || date[i] > '9') return false;
        month = month * 10 + (date[i] - '0');
    }
    for (int i = 8; i < 10; i++) {
        if (date[i] < '0' || date[i] > '9') return false;
        day = day * 10 + (date[i] - '0');
    }
    
    // Validate ranges
    if (year < 1900 || year > 2100) return false;
    if (month < 1 || month > 12) return false;
    if (day < 1 || day > 31) return false;
    
    // Basic month-day validation
    if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) return false;
    if (month == 2) {
        bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        if (day > (isLeap ? 29 : 28)) return false;
    }
    
    return true;
}

bool isValidBloodGroup(const char* bg) {
    // Common blood groups
    const char* validGroups[] = {"A+", "A-", "B+", "B-", "AB+", "AB-", "O+", "O-"};
    
    for (int g = 0; g < 8; g++) {
        bool match = true;
        for (int i = 0; validGroups[g][i] != '\0' || bg[i] != '\0'; i++) {
            if (validGroups[g][i] != bg[i]) {
                match = false;
                break;
            }
        }
        if (match && bg[0] != '\0') return true;
    }
    return false;
}

int stringToNumber(const char* str) {
	int num = 0;
	int i = 0;
	while (str[i] != '\0') {
		num = num * 10 + (str[i] - '0');
		i++;
	}
	return num;
}

bool stringsEqual(const char* str1, const char* str2) {
	int i = 0;
	while (str1[i] != '\0' && str2[i] != '\0') {
		if (str1[i] != str2[i]) {
			return false;
		}
		i++;
	}
	return (str1[i] == '\0' && str2[i] == '\0');
}

int getLength(const string& str) {
	int len = 0;
	while (str[len] != '\0') {
		len++;
	}
	return len;
}

void getLine(char* str, int maxLen) {
	int i = 0;
	char ch;
	while (i < maxLen - 1 && (ch = cin.get()) != '\n') {
		str[i++] = ch;
	}
	str[i] = '\0';
}

void toProperCase(char* str) {
	if (str[0] >= 'a' && str[0] <= 'z') {
		str[0] = str[0] - 32;
	}
	for (int i = 1; str[i] != '\0'; i++) {
		if (str[i] >= 'A' && str[i] <= 'Z') {
			str[i] = str[i] + 32;
		}
	}
}

int getLastDonorID() {
	ifstream fin("donors.txt");
	if (!fin) return 1000;

	string line;
	string lastLine = "";
	while (getline(fin, line)) {
		if (getLength(line) > 0) {
			lastLine = line;
		}
	}
	fin.close();

	if (getLength(lastLine) == 0) return 1000;

	char donorIDStr[10];
	int i = 0, j = 0;
	while (i < getLength(lastLine) && lastLine[i] != ',') {
		donorIDStr[j++] = lastLine[i++];
	}
	donorIDStr[j] = '\0';

	int id = 0;
	for (int k = 0; donorIDStr[k] != '\0'; k++) {
		id = id * 10 + (donorIDStr[k] - '0');
	}
	return id;
}

// File handling functions
void updateFile(const char* filename, const char* tempFilename, const char* lineToSkip) {
	ifstream fin(filename);
	ofstream fout(tempFilename);
	string line;
	bool found = false;

	while (getline(fin, line)) {
		if (!stringsEqual(line.c_str(), lineToSkip)) {
			fout << line << "\n";
		}
		else {
			found = true;
		}
	}

	fin.close();
	fout.close();

	remove(filename);
	rename(tempFilename, filename);
}

void updateInventory(const char* bloodGroup, int units) {
	ifstream fin("blood_inventory.txt");
	ofstream fout("temp_blood.txt");
	string line;

	while (getline(fin, line)) {
		char bg[5], unitsStr[10];
		int i = 0, j = 0;

		// Parse blood group
		while (line[i] != ',' && line[i] != '\0') bg[j++] = line[i++];
		bg[j] = '\0'; i++; j = 0;

		// Parse units
		while (line[i] != '\0') unitsStr[j++] = line[i++];
		unitsStr[j] = '\0';

		if (stringsEqual(bg, bloodGroup)) {
			// Update units
			int currentUnits = stringToNumber(unitsStr);
			int newUnits = currentUnits + units;

			// Convert newUnits back to string manually
			char newUnitsStr[10];
			if (newUnits == 0) {
				newUnitsStr[0] = '0';
				newUnitsStr[1] = '\0';
			}
			else {
				int temp = newUnits;
				int numDigits = 0;
				while (temp != 0) {
					temp /= 10;
					numDigits++;
				}

				temp = newUnits;
				for (int k = numDigits - 1; k >= 0; k--) {
					newUnitsStr[k] = '0' + (temp % 10);
					temp /= 10;
				}
				newUnitsStr[numDigits] = '\0';
			}

			fout << bg << "," << newUnitsStr << "\n";
		}
		else {
			fout << line << "\n";
		}
	}

	fin.close();
	fout.close();

	// Replace original file
	remove("blood_inventory.txt");
	rename("temp_blood.txt", "blood_inventory.txt");
}

// Base User class
class User {
protected:
	string username;
	string password;
	string role;

public:
	User(string uname = "", string pass = "", string r = "")
		: username(uname), password(pass), role(r) {}

	virtual void showDashboard() = 0;
	string getRole() const { return role; }
	string getUsername() const { return username; }

	// Common functions that can be used by derived classes
	void changePassword() {
		char oldPass[30], newPass[30];
		cout << "\nChange Password\n";
		cout << "Enter current password: ";
		getLine(oldPass, 30);

		bool passMatch = true;
		for (int i = 0; oldPass[i] != '\0' || password[i] != '\0'; i++) {
			if (oldPass[i] != password[i]) {
				passMatch = false;
				break;
			}
		}

		if (!passMatch) {
			cout << "Incorrect old password. Password not changed.\n";
			return;
		}

		cout << "Enter new password: ";
		getLine(newPass, 30);

		ifstream fin("users.txt");
		ofstream fout("temp.txt");
		string line;
		bool updated = false;

		while (getline(fin, line)) {
			char fileUsername[30], filePassword[30], fileRole[15];
			int field = 0, i = 0, j = 0;

			for (i = 0; line[i] != '\0'; i++) {
				if (line[i] == ',') {
					switch (field) {
					case 0: fileUsername[j] = '\0'; break;
					case 1: filePassword[j] = '\0'; break;
					}
					field++; j = 0;
				}
				else {
					switch (field) {
					case 0: fileUsername[j++] = line[i]; break;
					case 1: filePassword[j++] = line[i]; break;
					case 2: fileRole[j++] = line[i]; break;
					}
				}
			}
			fileRole[j] = '\0';

			bool nameMatch = true, roleMatch = true;
			for (int k = 0; username[k] != '\0' || fileUsername[k] != '\0'; k++) {
				if (username[k] != fileUsername[k]) { nameMatch = false; break; }
			}
			for (int k = 0; role[k] != '\0' || fileRole[k] != '\0'; k++) {
				if (role[k] != fileRole[k]) { roleMatch = false; break; }
			}

			if (nameMatch && roleMatch) {
				fout << fileUsername << "," << newPass << "," << fileRole << "\n";
				updated = true;
				password = newPass;
			}
			else {
				fout << line << "\n";
			}
		}

		fin.close();
		fout.close();

		if (updated) {
			remove("users.txt");
			rename("temp.txt", "users.txt");
			cout << "Password updated successfully!\n";
		}
		else {
			remove("temp.txt");
			cout << "Failed to update password. (User not matched)\n";
		}
	}
};

// Admin class
class Admin : public User {
public:
	Admin(string uname, string pass) : User(uname, pass, "Admin") {}

	void showDashboard() override {
		int choice;
		do {
			cout << "\nAdmin Dashboard - " << username << "\n";
			cout << "1. Register New User (Donor/Staff)\n";
			cout << "2. View All Users\n";
			cout << "3. Delete User Account\n";
			cout << "4. View staff\n";
			cout << "5. Blood Inventory View\n";
			cout << "6. All Scheduled Appointments\n";
			cout << "7. Search Appointments by Donor ID\n";
			cout << "8. Delete Donor Record\n";
			cout << "9. Reset User Password\n";
			cout << "10. Delete Patient\n";
			cout << "11. View Donors\n";
			cout << "12. View Patients\n";
			cout << "0. Logout\n";
			cout << "Enter choice: ";

			char input[10];
			getLine(input, 10);

			if (!isNumber(input)) {
				cout << "Invalid input. Please enter a number.\n";
				continue;
			}

			choice = stringToNumber(input);

			switch (choice) {
			case 1: registerUser(); break;
			case 2: viewAllUsers(); break;
			case 3: deleteUser(); break;
			case 4: viewStaff(); break;
			case 5: viewInventory(); break;
			case 6: viewAllAppointments(); break;
			case 7: searchAppointments(); break;
			case 8: deleteDonor(); break;
			case 9: resetUserPassword(); break;
			case 10: deletePatient(); break;
			case 11: viewDonors(); break;
			case 12: viewPatients(); break;
			case 0: cout << "Logging out...\n"; break;
			default: cout << "Invalid choice.\n";
			}
		} while (choice != 0);
	}

private:
	void registerUser() {
		char newUsername[50], newPassword[50], newRole[10];

		cout << "\nRegister New User\n";
		cout << "Enter username: ";
		getLine(newUsername, 50);

		cout << "Enter password: ";
		getLine(newPassword, 50);

		cout << "Enter role (Donor/Staff): ";
		getLine(newRole, 10);
		toProperCase(newRole);

		bool validRole = false;
		if ((newRole[0] == 'D' && newRole[1] == 'o') || (newRole[0] == 'S' && newRole[1] == 't')) {
			validRole = true;
		}

		if (!validRole) {
			cout << "Invalid role. Only 'Donor' or 'Staff' allowed.\n";
			return;
		}

		ofstream fout("users.txt", ios::app);
		if (!fout) {
			cout << "Error: Cannot open users.txt\n";
			return;
		}

		fout << newUsername << "," << newPassword << "," << newRole << "\n";
		fout.close();

		cout << newRole << " registered successfully!\n";
	}

	void viewAllUsers() {
		cout << "\nAll Registered Users:\n";
		ifstream fin("users.txt");
		if (!fin) {
			cout << "Error: Cannot open users.txt\n";
			return;
		}

		string line;
		int count = 0;

		while (getline(fin, line)) {
			char username[50], password[50], role[20];
			int i = 0, j = 0;
			int len = getLength(line);

			while (i < len && line[i] != ',') {
				username[j++] = line[i++];
			}
			username[j] = '\0'; i++; j = 0;

			while (i < len && line[i] != ',') {
				password[j++] = line[i++];
			}
			password[j] = '\0'; i++; j = 0;

			while (i < len) {
				role[j++] = line[i++];
			}
			role[j] = '\0';

			cout << "----------------------------\n";
			cout << "Username: " << username << "\n";
			cout << "Password: " << password << "\n";
			cout << "Role    : " << role << "\n";
			count++;
		}

		if (count == 0) {
			cout << "No users found in the system.\n";
		}
		fin.close();
	}

	void deleteUser() {
		char deleteUsername[50];
		cout << "\nEnter username to delete (Staff/Donor only): ";
		getLine(deleteUsername, 50);

		ifstream fin("users.txt");
		ofstream temp("temp.txt");

		if (!fin || !temp) {
			cout << "Error: Cannot open files.\n";
			return;
		}

		string line;
		bool found = false;

		while (getline(fin, line)) {
			char username[50], password[50], role[20];
			int i = 0, j = 0;
			int len = getLength(line);

			while (i < len && line[i] != ',') {
				username[j++] = line[i++];
			}
			username[j] = '\0'; i++; j = 0;

			while (i < len && line[i] != ',') {
				password[j++] = line[i++];
			}
			password[j] = '\0'; i++; j = 0;

			while (i < len) {
				role[j++] = line[i++];
			}
			role[j] = '\0';

			bool sameUser = true;
			for (int k = 0; deleteUsername[k] != '\0' || username[k] != '\0'; k++) {
				if (deleteUsername[k] != username[k]) {
					sameUser = false;
					break;
				}
			}

			bool isAdmin = (role[0] == 'A' && role[1] == 'd' && role[2] == 'm');

			if (!sameUser || isAdmin) {
				temp << username << "," << password << "," << role << "\n";
			}
			else {
				found = true;
			}
		}
		fin.close();
		temp.close();

		ifstream readTemp("temp.txt");
		ofstream finalFile("users.txt");

		while (getline(readTemp, line)) {
			finalFile << line << "\n";
		}

		readTemp.close();
		finalFile.close();

		if (found) {
			cout << "User deleted successfully.\n";
		}
		else {
			cout << "User not found or cannot delete Admin.\n";
		}
	}

	void viewStaff() {
		cout << "\nAll Registered Staff Members\n";
		ifstream fin("users.txt");
		if (!fin) {
			cout << "Could not open users.txt\n";
			return;
		}

		string line;
		bool found = false;

		while (getline(fin, line)) {
			char username[20], password[20], role[10];
			int field = 0, i = 0, j = 0;

			for (int k = 0; k <= getLength(line); k++) {
				if (line[k] == ',' || line[k] == '\0') {
					switch (field) {
					case 0: username[j] = '\0'; j = 0; field++; break;
					case 1: password[j] = '\0'; j = 0; field++; break;
					case 2: role[j] = '\0'; j = 0; field++; break;
					}
				}
				else {
					switch (field) {
					case 0: username[j++] = line[k]; break;
					case 1: password[j++] = line[k]; break;
					case 2: role[j++] = line[k]; break;
					}
				}
			}

			bool isStaff = true;
			char staffWord[] = "Staff";
			for (int s = 0; staffWord[s] != '\0' || role[s] != '\0'; s++) {
				if (staffWord[s] != role[s]) {
					isStaff = false;
					break;
				}
			}

			if (isStaff) {
				found = true;
				cout << "\n------------------------\n";
				cout << "Username: " << username << "\n";
				cout << "Password: " << password << "\n";
			}
		}

		if (!found) {
			cout << "No staff members found.\n";
		}
		fin.close();
	}

	void viewInventory() {
		cout << "\nBlood Inventory Report (Admin View)\n";
		ifstream fin("blood_inventory.txt");
		if (!fin) {
			cout << "Could not open blood_inventory.txt\n";
			return;
		}

		string line;
		int count = 0;

		while (getline(fin, line)) {
			char bloodGroup[5], units[10];
			int i = 0, j = 0, len = getLength(line);

			while (i < len && line[i] != ',') {
				bloodGroup[j++] = line[i++];
			}
			bloodGroup[j] = '\0'; i++; j = 0;

			while (i < len) {
				units[j++] = line[i++];
			}
			units[j] = '\0';

			cout << "---------------------------\n";
			cout << "Blood Group : " << bloodGroup << "\n";
			cout << "Units       : " << units << "\n";
			count++;
		}

		if (count == 0) {
			cout << "Inventory is currently empty.\n";
		}
		fin.close();
	}

	void viewAllAppointments() {
		cout << "\nAll Scheduled Appointments (Admin View)\n";
		ifstream fin("appointments.txt");
		if (!fin) {
			cout << "Could not open appointments.txt\n";
			return;
		}

		string line;
		bool found = false;

		while (getline(fin, line)) {
			char appointmentID[10], donorID[10], date[15], time[10];
			int field = 0, i = 0, j = 0;

			for (int k = 0; k <= getLength(line); k++) {
				if (line[k] == ',' || line[k] == '\0') {
					switch (field) {
					case 0: appointmentID[j] = '\0'; j = 0; field++; break;
					case 1: donorID[j] = '\0'; j = 0; field++; break;
					case 2: date[j] = '\0'; j = 0; field++; break;
					case 3: time[j] = '\0'; j = 0; field++; break;
					}
				}
				else {
					switch (field) {
					case 0: appointmentID[j++] = line[k]; break;
					case 1: donorID[j++] = line[k]; break;
					case 2: date[j++] = line[k]; break;
					case 3: time[j++] = line[k]; break;
					}
				}
			}

			found = true;
			cout << "\n------------------------------\n";
			cout << "Appointment ID : " << appointmentID << "\n";
			cout << "Donor ID       : " << donorID << "\n";
			cout << "Date           : " << date << "\n";
			cout << "Time           : " << time << "\n";
		}

		if (!found) {
			cout << "No appointments found.\n";
		}
		fin.close();
	}

	void searchAppointments() {
		char searchDonorID[10];
		cout << "\nSearch Appointments by Donor ID (Admin View)\n";
		cout << "Enter Donor ID: ";
		getLine(searchDonorID, 10);

		ifstream fin("appointments.txt");
		if (!fin) {
			cout << "Could not open appointments.txt\n";
			return;
		}

		string line;
		bool found = false;

		while (getline(fin, line)) {
			char appointmentID[10], donorID[10], date[15], time[10];
			int field = 0, i = 0, j = 0;

			for (int k = 0; k <= getLength(line); k++) {
				if (line[k] == ',' || line[k] == '\0') {
					switch (field) {
					case 0: appointmentID[j] = '\0'; j = 0; field++; break;
					case 1: donorID[j] = '\0'; j = 0; field++; break;
					case 2: date[j] = '\0'; j = 0; field++; break;
					case 3: time[j] = '\0'; j = 0; field++; break;
					}
				}
				else {
					switch (field) {
					case 0: appointmentID[j++] = line[k]; break;
					case 1: donorID[j++] = line[k]; break;
					case 2: date[j++] = line[k]; break;
					case 3: time[j++] = line[k]; break;
					}
				}
			}

			bool match = true;
			for (int m = 0; searchDonorID[m] != '\0' || donorID[m] != '\0'; m++) {
				if (searchDonorID[m] != donorID[m]) {
					match = false;
					break;
				}
			}

			if (match) {
				found = true;
				cout << "\n------------------------------\n";
				cout << "Appointment ID : " << appointmentID << "\n";
				cout << "Donor ID       : " << donorID << "\n";
				cout << "Date           : " << date << "\n";
				cout << "Time           : " << time << "\n";
			}
		}

		if (!found) {
			cout << "No appointments found for Donor ID " << searchDonorID << "\n";
		}
		fin.close();
	}

	void deleteDonor() {
		char delID[20];
		cout << "\nDelete Donor\nEnter Donor ID to delete: ";
		getLine(delID, 20);

		ifstream fin("donors.txt");
		ofstream fout("temp.txt");
		string line;
		bool found = false;

		while (getline(fin, line)) {
			char currentID[20];
			int i = 0, j = 0;

			while (line[i] != ',' && line[i] != '\0') currentID[j++] = line[i++];
			currentID[j] = '\0';

			bool match = true;
			for (int k = 0; delID[k] != '\0' || currentID[k] != '\0'; k++) {
				if (delID[k] != currentID[k]) { match = false; break; }
			}

			if (!match) fout << line << "\n";
			else found = true;
		}

		fin.close();
		fout.close();
		remove("donors.txt");
		rename("temp.txt", "donors.txt");

		cout << (found ? "Donor deleted.\n" : "Donor not found.\n");
	}

	void resetUserPassword() {
		char targetUsername[30], newPassword[30];
		cout << "\nRESET USER PASSWORD\n";
		cout << "Enter username to reset: ";
		getLine(targetUsername, 30);
		cout << "Enter new password: ";
		getLine(newPassword, 30);

		ifstream fin("users.txt");
		ofstream fout("temp.txt");
		string line;
		bool found = false;

		while (getline(fin, line)) {
			char fileUsername[30], filePassword[30], fileRole[15];
			int field = 0, i = 0, j = 0;

			for (i = 0; line[i] != '\0'; i++) {
				if (line[i] == ',') {
					switch (field) {
					case 0: fileUsername[j] = '\0'; break;
					case 1: filePassword[j] = '\0'; break;
					}
					field++; j = 0;
				}
				else {
					switch (field) {
					case 0: fileUsername[j++] = line[i]; break;
					case 1: filePassword[j++] = line[i]; break;
					case 2: fileRole[j++] = line[i]; break;
					}
				}
			}
			fileRole[j] = '\0';

			bool match = true;
			for (int k = 0; targetUsername[k] != '\0' || fileUsername[k] != '\0'; k++) {
				if (targetUsername[k] != fileUsername[k]) {
					match = false;
					break;
				}
			}

			if (match) {
				fout << fileUsername << "," << newPassword << "," << fileRole << "\n";
				found = true;
			}
			else {
				fout << line << "\n";
			}
		}

		fin.close();
		fout.close();
		if (found) {
			remove("users.txt");
			rename("temp.txt", "users.txt");
			cout << "Password reset for user " << targetUsername << "\n";
		}
		else {
			remove("temp.txt");
			cout << "Username not found.\n";
		}
	}

	void deletePatient() {
		char delID[10];
		cout << "\nEnter Patient ID to delete: ";
		getLine(delID, 10);

		ifstream fin("patients.txt");
		ofstream fout("temp.txt");
		string line;
		bool found = false;

		while (getline(fin, line)) {
			char currentID[10];
			int i = 0, j = 0;

			while (line[i] != ',' && line[i] != '\0') {
				currentID[j++] = line[i++];
			}
			currentID[j] = '\0';

			if (stringsEqual(currentID, delID)) {
				found = true;
			}
			else {
				fout << line << "\n";
			}
		}

		fin.close();
		fout.close();

		if (found) {
			remove("patients.txt");
			rename("temp.txt", "patients.txt");
			cout << "Patient deleted successfully.\n";
		}
		else {
			remove("temp.txt");
			cout << "Patient not found.\n";
		}
	}

	void viewDonors() {
		cout << "\nList of All Donors:\n";
		ifstream fin("donors.txt");
		if (!fin) {
			cout << "Error: Cannot open donors.txt\n";
			return;
		}

		string line;
		int count = 0;

		while (getline(fin, line)) {
			char fields[6][50];
			int i = 0, f = 0, j = 0;
			int len = getLength(line);

			while (i <= len) {
				if (line[i] == ',' || line[i] == '\0') {
					fields[f][j] = '\0';
					f++; j = 0;
				}
				else {
					fields[f][j++] = line[i];
				}
				i++;
			}

			cout << "------------------------------\n";
			cout << "Donor ID   : " << fields[0] << "\n";
			cout << "Name       : " << fields[1] << "\n";
			cout << "Age        : " << fields[2] << "\n";
			cout << "Blood Group: " << fields[3] << "\n";
			cout << "Gender     : " << fields[4] << "\n";
			cout << "Phone      : " << fields[5] << "\n";
			count++;
		}

		if (count == 0) {
			cout << "No donor records found.\n";
		}
		fin.close();
	}

	void viewPatients() {
		cout << "\nPATIENT RECORDS\n";
		ifstream fin("patients.txt");
		if (!fin) {
			cout << "No patient records found.\n";
			return;
		}

		string line;
		while (getline(fin, line)) {
			char id[10], name[50], bg[5], units[10], condition[100], date[11], status[20];
			int i = 0, j = 0, field = 0;

			for (int k = 0; k <= getLength(line); k++) {
				if (line[k] == ',' || line[k] == '\0') {
					switch (field) {
					case 0: id[j] = '\0'; break;
					case 1: name[j] = '\0'; break;
					case 2: bg[j] = '\0'; break;
					case 3: units[j] = '\0'; break;
					case 4: condition[j] = '\0'; break;
					case 5: date[j] = '\0'; break;
					case 6: status[j] = '\0'; break;
					}
					field++; j = 0;
				}
				else {
					switch (field) {
					case 0: id[j++] = line[k]; break;
					case 1: name[j++] = line[k]; break;
					case 2: bg[j++] = line[k]; break;
					case 3: units[j++] = line[k]; break;
					case 4: condition[j++] = line[k]; break;
					case 5: date[j++] = line[k]; break;
					case 6: status[j++] = line[k]; break;
					}
				}
			}

			cout << "--------------------------------\n";
			cout << "ID: " << id << " | Name: " << name << "\n";
			cout << "Blood Group: " << bg << " | Units Needed: " << units << "\n";
			cout << "Condition: " << condition << "\n";
			cout << "Date: " << date << " | Status: " << status << "\n";
		}
		fin.close();
	}
};

// Staff class
class Staff : public User {
public:
	Staff(string uname, string pass) : User(uname, pass, "Staff") {}

	void showDashboard() override {
		int choice;
		do {
			cout << "\nStaff Dashboard - " << username << "\n";
			cout << "1. Add New Donor\n";
			cout << "2. View Donors\n";
			cout << "3. Manage Blood Inventory\n";
			cout << "4. Schedule Donation Appointment\n";
			cout << "5. See Report\n";
			cout << "6. Find Blood Group\n";
			cout << "7. View Schedule Appointment\n";
			cout << "8. Search Appointments by Donor ID\n";
			cout << "9. View My Info\n";
			cout << "10. Delete Appointment\n";
			cout << "11. Change My Password\n";
			cout << "12. Add Patient\n";
			cout << "13. View Patients\n";
			cout << "14. Fulfill Pending Requests\n";
			cout << "0. Logout\n";
			cout << "Enter choice: ";

			char input[10];
			getLine(input, 10);

			if (!isNumber(input)) {
				cout << "Invalid input. Please enter a number.\n";
				continue;
			}

			choice = stringToNumber(input);

			switch (choice) {
			case 1: addDonor(); break;
			case 2: viewDonors(); break;
			case 3: manageInventory(); break;
			case 4: scheduleAppointment(); break;
			case 5: viewReport(); break;
			case 6: findBloodGroup(); break;
			case 7: viewAppointments(); break;
			case 8: searchAppointments(); break;
			case 9: viewMyInfo(); break;
			case 10: deleteAppointment(); break;
			case 11: changePassword(); break;
			case 12: addPatient(); break;
			case 13: viewPatients(); break;
			case 14: fulfillPendingRequests(); break;
			case 0: cout << "Logging out...\n"; break;
			default: cout << "Invalid choice.\n";
			}
		} while (choice != 0);
	}

private:
	void addPatient() {
		char name[50], bloodGroup[5], unitsStr[10], medicalCondition[100], date[11];
		int units;

		cout << "\nADD NEW PATIENT\n";
		cout << "Enter Patient Name: ";
		getLine(name, 50);

		cout << "Enter Blood Group Needed (e.g., A+, B-): ";
		getLine(bloodGroup, 5);

		cout << "Enter Units Required: ";
		getLine(unitsStr, 10);
		if (!isNumber(unitsStr)) {
			cout << "Invalid units. Please enter a number.\n";
			return;
		}
		units = stringToNumber(unitsStr);

		cout << "Enter Medical Condition: ";
		getLine(medicalCondition, 100);

		cout << "Enter Date (YYYY-MM-DD): ";
		getLine(date, 11);

		// Generate patient ID
		int newID = 1000;
		ifstream fin("patients.txt");
		string line;
		while (getline(fin, line)) {
			char idStr[10];
			int i = 0, j = 0;
			while (line[i] != ',' && line[i] != '\0') {
				idStr[j++] = line[i++];
			}
			idStr[j] = '\0';
			int currentID = stringToNumber(idStr);
			if (currentID >= newID) {
				newID = currentID + 1;
			}
		}
		fin.close();

		// Save patient
		ofstream fout("patients.txt", ios::app);
		fout << newID << "," << name << "," << bloodGroup << "," << units << ","
			<< medicalCondition << "," << date << ",Pending\n";
		fout.close();

		cout << "Patient added successfully! ID: " << newID << "\n";
		fulfillRequest(newID);
	}

	void fulfillRequest(int patientID) {
		ifstream fin("patients.txt");
		ofstream fout("temp.txt");
		string line;
		bool found = false;
		char patientBloodGroup[5];
		int unitsNeeded = 0;

		while (getline(fin, line)) {
			char currentID[10], name[50], bg[5], unitsStr[10], condition[100], date[11], status[20];
			int i = 0, j = 0, field = 0;

			for (int k = 0; k <= getLength(line); k++) {
				if (line[k] == ',' || line[k] == '\0') {
					switch (field) {
					case 0: currentID[j] = '\0'; break;
					case 1: name[j] = '\0'; break;
					case 2: bg[j] = '\0'; break;
					case 3: unitsStr[j] = '\0'; break;
					case 4: condition[j] = '\0'; break;
					case 5: date[j] = '\0'; break;
					case 6: status[j] = '\0'; break;
					}
					field++; j = 0;
				}
				else {
					switch (field) {
					case 0: currentID[j++] = line[k]; break;
					case 1: name[j++] = line[k]; break;
					case 2: bg[j++] = line[k]; break;
					case 3: unitsStr[j++] = line[k]; break;
					case 4: condition[j++] = line[k]; break;
					case 5: date[j++] = line[k]; break;
					case 6: status[j++] = line[k]; break;
					}
				}
			}

			int id = stringToNumber(currentID);
			if (id == patientID) {
				found = true;
				int m = 0;
				while (bg[m] != '\0') {
					patientBloodGroup[m] = bg[m];
					m++;
				}
				patientBloodGroup[m] = '\0';
				unitsNeeded = stringToNumber(unitsStr);
				break;
			}
		}
		fin.close();

		if (!found) {
			cout << "Patient not found!\n";
			remove("temp.txt");
			return;
		}

		// Check inventory
		ifstream invFin("blood_inventory.txt");
		bool bloodAvailable = false;
		int currentUnits = 0;

		while (getline(invFin, line)) {
			char bg[5], unitsStr[10];
			int i = 0, j = 0;

			while (line[i] != ',' && line[i] != '\0') bg[j++] = line[i++];
			bg[j] = '\0'; i++; j = 0;

			while (line[i] != '\0') unitsStr[j++] = line[i++];
			unitsStr[j] = '\0';

			if (stringsEqual(bg, patientBloodGroup)) {
				currentUnits = stringToNumber(unitsStr);
				if (currentUnits >= unitsNeeded) {
					bloodAvailable = true;
				}
				break;
			}
		}
		invFin.close();

		// Update patient status
		fin.open("patients.txt");
		while (getline(fin, line)) {
			char currentID[10], name[50], bg[5], unitsStr[10], condition[100], date[11], status[20];
			int i = 0, j = 0, field = 0;

			for (int k = 0; k <= getLength(line); k++) {
				if (line[k] == ',' || line[k] == '\0') {
					switch (field) {
					case 0: currentID[j] = '\0'; break;
					case 1: name[j] = '\0'; break;
					case 2: bg[j] = '\0'; break;
					case 3: unitsStr[j] = '\0'; break;
					case 4: condition[j] = '\0'; break;
					case 5: date[j] = '\0'; break;
					case 6: status[j] = '\0'; break;
					}
					field++; j = 0;
				}
				else {
					switch (field) {
					case 0: currentID[j++] = line[k]; break;
					case 1: name[j++] = line[k]; break;
					case 2: bg[j++] = line[k]; break;
					case 3: unitsStr[j++] = line[k]; break;
					case 4: condition[j++] = line[k]; break;
					case 5: date[j++] = line[k]; break;
					case 6: status[j++] = line[k]; break;
					}
				}
			}

			int id = stringToNumber(currentID);
			if (id == patientID) {
				if (bloodAvailable) {
					fout << currentID << "," << name << "," << bg << "," << unitsStr << ","
						<< condition << "," << date << ",Fulfilled\n";
					updateInventory(bg, -unitsNeeded);
					cout << "Request fulfilled for Patient ID: " << patientID << "\n";
				}
				else {
					fout << line << "\n";
					cout << "Blood not available. Request remains pending.\n";
				}
			}
			else {
				fout << line << "\n";
			}
		}
		fin.close();
		fout.close();

		remove("patients.txt");
		rename("temp.txt", "patients.txt");
	}

	void viewPatients() {
		cout << "\nPATIENT RECORDS\n";
		ifstream fin("patients.txt");
		if (!fin) {
			cout << "No patient records found.\n";
			return;
		}

		string line;
		while (getline(fin, line)) {
			char id[10], name[50], bg[5], units[10], condition[100], date[11], status[20];
			int i = 0, j = 0, field = 0;

			for (int k = 0; k <= getLength(line); k++) {
				if (line[k] == ',' || line[k] == '\0') {
					switch (field) {
					case 0: id[j] = '\0'; break;
					case 1: name[j] = '\0'; break;
					case 2: bg[j] = '\0'; break;
					case 3: units[j] = '\0'; break;
					case 4: condition[j] = '\0'; break;
					case 5: date[j] = '\0'; break;
					case 6: status[j] = '\0'; break;
					}
					field++; j = 0;
				}
				else {
					switch (field) {
					case 0: id[j++] = line[k]; break;
					case 1: name[j++] = line[k]; break;
					case 2: bg[j++] = line[k]; break;
					case 3: units[j++] = line[k]; break;
					case 4: condition[j++] = line[k]; break;
					case 5: date[j++] = line[k]; break;
					case 6: status[j++] = line[k]; break;
					}
				}
			}

			cout << "--------------------------------\n";
			cout << "ID: " << id << " | Name: " << name << "\n";
			cout << "Blood Group: " << bg << " | Units Needed: " << units << "\n";
			cout << "Condition: " << condition << "\n";
			cout << "Date: " << date << " | Status: " << status << "\n";
		}
		fin.close();
	}

	void fulfillPendingRequests() {
		ifstream fin("patients.txt");
		ofstream fout("temp.txt");
		string line;

		while (getline(fin, line)) {
			char id[10], name[50], bg[5], unitsStr[10], condition[100], date[11], status[20];
			int i = 0, j = 0, field = 0;

			// Parse each field manually
			for (int k = 0; k <= getLength(line); k++) {
				if (line[k] == ',' || line[k] == '\0') {
					switch (field) {
					case 0: id[j] = '\0'; break;
					case 1: name[j] = '\0'; break;
					case 2: bg[j] = '\0'; break;
					case 3: unitsStr[j] = '\0'; break;
					case 4: condition[j] = '\0'; break;
					case 5: date[j] = '\0'; break;
					case 6: status[j] = '\0'; break;
					}
					field++; j = 0;
				}
				else {
					switch (field) {
					case 0: id[j++] = line[k]; break;
					case 1: name[j++] = line[k]; break;
					case 2: bg[j++] = line[k]; break;
					case 3: unitsStr[j++] = line[k]; break;
					case 4: condition[j++] = line[k]; break;
					case 5: date[j++] = line[k]; break;
					case 6: status[j++] = line[k]; break;
					}
				}
			}

			// Check if status is Pending
			if (stringsEqual(status, "Pending")) {
				int unitsNeeded = stringToNumber(unitsStr);
				bool bloodAvailable = false;

				// Check inventory for this blood group
				ifstream invFin("blood_inventory.txt");
				string invLine;
				while (getline(invFin, invLine)) {
					char invBG[5], invUnitsStr[10];
					int x = 0, y = 0;

					// Parse inventory line
					while (invLine[x] != ',' && invLine[x] != '\0') invBG[y++] = invLine[x++];
					invBG[y] = '\0'; x++; y = 0;

					while (invLine[x] != '\0') invUnitsStr[y++] = invLine[x++];
					invUnitsStr[y] = '\0';

					if (stringsEqual(invBG, bg)) {
						int currentUnits = stringToNumber(invUnitsStr);
						if (currentUnits >= unitsNeeded) {
							bloodAvailable = true;
						}
						break;
					}
				}
				invFin.close();

				if (bloodAvailable) {
					// Update status to Fulfilled
					fout << id << "," << name << "," << bg << "," << unitsStr << ","
						<< condition << "," << date << ",Fulfilled\n";

					// Update inventory
					updateInventory(bg, -unitsNeeded);
					cout << "Request fulfilled for Patient ID: " << id << "\n";
				}
				else {
					// Keep as Pending
					fout << line << "\n";
				}
			}
			else {
				// Not Pending, copy as-is
				fout << line << "\n";
			}
		}

		fin.close();
		fout.close();

		// Replace original file
		remove("patients.txt");
		rename("temp.txt", "patients.txt");
	}

	void addDonor() {
		char name[50], bloodGroup[5], gender[10], phone[20], ageStr[5];
		int age;

		cout << "\nAdd New Donor\n";
		cout << "Enter Name: ";
		getLine(name, 50);

		cout << "Enter Age: ";
		getLine(ageStr, 5);
		age = stringToNumber(ageStr);

		cout << "Enter Blood Group (A+/B+/O-/...): ";
		getLine(bloodGroup, 5);

		cout << "Enter Gender: ";
		getLine(gender, 10);

		cout << "Enter Phone Number: ";
		getLine(phone, 20);

		int newID = getLastDonorID() + 1;

		ofstream fout("donors.txt", ios::app);
		if (!fout) {
			cout << "Error: Cannot open donors.txt\n";
			return;
		}

		fout << newID << "," << name << "," << age << "," << bloodGroup << "," << gender << "," << phone << "\n";
		fout.close();

		cout << "Donor added with ID: " << newID << "\n";
	}

	void viewDonors() {
		cout << "\nList of All Donors:\n";
		ifstream fin("donors.txt");
		if (!fin) {
			cout << "Error: Cannot open donors.txt\n";
			return;
		}

		string line;
		int count = 0;

		while (getline(fin, line)) {
			char fields[6][50];
			int i = 0, f = 0, j = 0;
			int len = getLength(line);

			while (i <= len) {
				if (line[i] == ',' || line[i] == '\0') {
					fields[f][j] = '\0';
					f++; j = 0;
				}
				else {
					fields[f][j++] = line[i];
				}
				i++;
			}

			cout << "------------------------------\n";
			cout << "Donor ID   : " << fields[0] << "\n";
			cout << "Name       : " << fields[1] << "\n";
			cout << "Age        : " << fields[2] << "\n";
			cout << "Blood Group: " << fields[3] << "\n";
			cout << "Gender     : " << fields[4] << "\n";
			cout << "Phone      : " << fields[5] << "\n";
			count++;
		}

		if (count == 0) {
			cout << "No donor records found.\n";
		}
		fin.close();
	}

	void manageInventory() {
		char bloodGroup[5];
		cout << "\nManage Blood Inventory\n";
		cout << "Enter Blood Group (e.g., A+, B-, O+): ";
		cin.getline(bloodGroup, 5);

		// Check if blood group exists in inventory
		ifstream fin("blood_inventory.txt");
		bool groupExists = false;
		string line;

		while (getline(fin, line)) {
			char group[5];
			int i = 0, j = 0;
			int len = getLength(line);

			while (i < len && line[i] != ',') {
				group[j++] = line[i++];
			}
			group[j] = '\0';

			bool sameGroup = true;
			for (int k = 0; bloodGroup[k] != '\0' || group[k] != '\0'; k++) {
				if (bloodGroup[k] != group[k]) {
					sameGroup = false;
					break;
				}
			}

			if (sameGroup) {
				groupExists = true;
				break;
			}
		}
		fin.close();

		if (!groupExists) {
			cout << "This blood group doesn't exist in inventory. You can only ADD units.\n";
			char choice;
			cout << "Do you want to add this blood group? (Y/N): ";
			cin >> choice;
			cin.ignore();

			if (choice == 'Y' || choice == 'y') {
				char unitStr[10];
				int unitsToAdd;

				cout << "Enter number of units to add: ";
				cin.getline(unitStr, 10);

				unitsToAdd = stringToNumber(unitStr);

				ofstream fout("blood_inventory.txt", ios::app);
				fout << bloodGroup << "," << unitsToAdd << "\n";
				fout.close();
				cout << "New blood group added successfully with " << unitsToAdd << " units.\n";
			}
			return;
		}

		// If blood group exists, ask for operation
		char operation;
		cout << "This blood group exists in inventory. Do you want to (A)dd or (S)ubtract units? ";
		cin >> operation;
		cin.ignore();

		bool isAdd = (operation == 'A' || operation == 'a');
		bool isSub = (operation == 'S' || operation == 's');

		if (!isAdd && !isSub) {
			cout << "Invalid operation. Please enter A or S only.\n";
			return;
		}

		char unitStr[10];
		int unitsToChange;

		cout << "Enter number of units to " << (isAdd ? "add" : "subtract") << ": ";
		cin.getline(unitStr, 10);

		unitsToChange = stringToNumber(unitStr);

		fin.open("blood_inventory.txt");
		ofstream fout("temp_blood.txt");

		if (!fout) {
			cout << "Cannot write to temp_blood.txt\n";
			return;
		}

		bool found = false;
		while (getline(fin, line)) {
			char group[5], unitValueStr[10];
			int i = 0, j = 0, len = getLength(line);

			while (i < len && line[i] != ',') {
				group[j++] = line[i++];
			}
			group[j] = '\0'; i++; j = 0;

			while (i < len) {
				unitValueStr[j++] = line[i++];
			}
			unitValueStr[j] = '\0';

			bool sameGroup = true;
			for (int k = 0; bloodGroup[k] != '\0' || group[k] != '\0'; k++) {
				if (bloodGroup[k] != group[k]) {
					sameGroup = false;
					break;
				}
			}

			if (sameGroup) {
				found = true;
				int currentUnits = stringToNumber(unitValueStr);
				int updated = currentUnits;
				if (isAdd) {
					updated += unitsToChange;
				}
				else {
					if (unitsToChange > currentUnits) {
						cout << "Cannot subtract more units than available (" << currentUnits << "). Keeping value unchanged.\n";
						updated = currentUnits;
					}
					else {
						updated -= unitsToChange;
					}
				}

				fout << group << "," << updated << "\n";
			}
			else {
				fout << line << "\n";
			}
		}

		fin.close();
		fout.close();

		ifstream readTemp("temp_blood.txt");
		ofstream finalFile("blood_inventory.txt");

		while (getline(readTemp, line)) {
			finalFile << line << "\n";
		}

		readTemp.close();
		finalFile.close();

		cout << "Inventory updated successfully.\n";
	}

	void scheduleAppointment() {
		char donorID[10], date[15], time[10];

		cout << "\nSchedule Appointment\n";

		// View all donors first
		cout << "Available Donors:\n";
		ifstream donorFile("donors.txt");
		string donorLine;
		while (getline(donorFile, donorLine)) {
			char currentID[10], name[50];
			int i = 0, j = 0;

			// Extract ID
			while (donorLine[i] != ',' && donorLine[i] != '\0') {
				currentID[j++] = donorLine[i++];
			}
			currentID[j] = '\0';
			i++; j = 0;

			// Extract Name
			while (donorLine[i] != ',' && donorLine[i] != '\0') {
				name[j++] = donorLine[i++];
			}
			name[j] = '\0';

			cout << "ID: " << currentID << " | Name: " << name << "\n";
		}
		donorFile.close();

		cout << "Enter Donor ID: ";
		cin.getline(donorID, 10);

		// Verify donor exists
		bool donorExists = false;
		donorFile.open("donors.txt");
		while (getline(donorFile, donorLine)) {
			char currentID[10];
			int i = 0, j = 0;

			while (donorLine[i] != ',' && donorLine[i] != '\0') {
				currentID[j++] = donorLine[i++];
			}
			currentID[j] = '\0';

			bool match = true;
			for (int k = 0; donorID[k] != '\0' || currentID[k] != '\0'; k++) {
				if (donorID[k] != currentID[k]) {
					match = false;
					break;
				}
			}

			if (match) {
				donorExists = true;
				break;
			}
		}
		donorFile.close();

		if (!donorExists) {
			cout << "Error: Donor with ID " << donorID << " not found.\n";
			return;
		}

		cout << "Enter Date (YYYY-MM-DD): ";
		cin.getline(date, 15);

		cout << "Enter Time (HH:MM AM/PM): ";
		cin.getline(time, 10);

		// Generate appointment ID
		int newID = 1000;
		ifstream apptFile("appointments.txt");
		string apptLine;
		while (getline(apptFile, apptLine)) {
			char idStr[10];
			int i = 0, j = 0;

			while (apptLine[i] != ',' && apptLine[i] != '\0') {
				idStr[j++] = apptLine[i++];
			}
			idStr[j] = '\0';

			int currentID = stringToNumber(idStr);
			if (currentID >= newID) {
				newID = currentID + 1;
			}
		}
		apptFile.close();

		// Save appointment
		ofstream fout("appointments.txt", ios::app);
		fout << newID << "," << donorID << "," << date << "," << time << "\n";
		fout.close();

		cout << "Appointment scheduled successfully!\n";
		cout << "Appointment ID: " << newID << "\n";
		cout << "Donor ID: " << donorID << "\n";
		cout << "Date: " << date << "\n";
		cout << "Time: " << time << "\n";
	}

	void viewReport() {
		cout << "\nBlood Inventory Report:\n";
		ifstream fin("blood_inventory.txt");
		if (!fin) {
			cout << "Could not open blood_inventory.txt\n";
			return;
		}

		string line;
		int count = 0;

		while (getline(fin, line)) {
			char bloodGroup[5], units[10];
			int i = 0, j = 0, len = getLength(line);

			while (i < len && line[i] != ',') {
				bloodGroup[j++] = line[i++];
			}
			bloodGroup[j] = '\0'; i++; j = 0;

			while (i < len) {
				units[j++] = line[i++];
			}
			units[j] = '\0';

			cout << "---------------------------\n";
			cout << "Blood Group : " << bloodGroup << "\n";
			cout << "Units       : " << units << "\n";
			count++;
		}

		if (count == 0) {
			cout << "Inventory is currently empty.\n";
		}
		fin.close();
	}

	void findBloodGroup() {
		char searchGroup[5];
		cout << "\nSearch Donors by Blood Group\n";
		cout << "Enter Blood Group to search: ";
		getLine(searchGroup, 5);

		ifstream fin("donors.txt");
		if (!fin) {
			cout << "Cannot open donors.txt\n";
			return;
		}

		string line;
		bool found = false;

		while (getline(fin, line)) {
			char donorID[10], name[20], age[5], bloodGroup[5], phone[15];
			int field = 0, i = 0, j = 0;

			for (int k = 0; k <= getLength(line); k++) {
				if (line[k] == ',' || line[k] == '\0') {
					switch (field) {
					case 0: donorID[j] = '\0'; j = 0; field++; break;
					case 1: name[j] = '\0'; j = 0; field++; break;
					case 2: age[j] = '\0'; j = 0; field++; break;
					case 3: bloodGroup[j] = '\0'; j = 0; field++; break;
					case 4: phone[j] = '\0'; j = 0; field++; break;
					}
				}
				else {
					switch (field) {
					case 0: donorID[j++] = line[k]; break;
					case 1: name[j++] = line[k]; break;
					case 2: age[j++] = line[k]; break;
					case 3: bloodGroup[j++] = line[k]; break;
					case 4: phone[j++] = line[k]; break;
					}
				}
			}

			bool match = true;
			for (int b = 0; searchGroup[b] != '\0' || bloodGroup[b] != '\0'; b++) {
				if (searchGroup[b] != bloodGroup[b]) {
					match = false;
					break;
				}
			}

			if (match) {
				found = true;
				cout << "\n-------------------------------\n";
				cout << "Donor ID   : " << donorID << "\n";
				cout << "Name       : " << name << "\n";
				cout << "Age        : " << age << "\n";
				cout << "Blood Group: " << bloodGroup << "\n";
				cout << "Phone      : " << phone << "\n";
			}
		}

		if (!found) {
			cout << "No donors found with blood group " << searchGroup << "\n";
		}
		fin.close();
	}

	void viewAppointments() {
		cout << "\nAll Scheduled Appointments\n";
		ifstream fin("appointments.txt");
		if (!fin) {
			cout << "Could not open appointments.txt\n";
			return;
		}

		string line;
		bool found = false;

		while (getline(fin, line)) {
			char appointmentID[10], donorID[10], date[15], time[10];
			int field = 0, i = 0, j = 0;

			for (int k = 0; k <= getLength(line); k++) {
				if (line[k] == ',' || line[k] == '\0') {
					switch (field) {
					case 0: appointmentID[j] = '\0'; j = 0; field++; break;
					case 1: donorID[j] = '\0'; j = 0; field++; break;
					case 2: date[j] = '\0'; j = 0; field++; break;
					case 3: time[j] = '\0'; j = 0; field++; break;
					}
				}
				else {
					switch (field) {
					case 0: appointmentID[j++] = line[k]; break;
					case 1: donorID[j++] = line[k]; break;
					case 2: date[j++] = line[k]; break;
					case 3: time[j++] = line[k]; break;
					}
				}
			}

			found = true;
			cout << "\n------------------------------\n";
			cout << "Appointment ID : " << appointmentID << "\n";
			cout << "Donor ID       : " << donorID << "\n";
			cout << "Date           : " << date << "\n";
			cout << "Time           : " << time << "\n";
		}

		if (!found) {
			cout << "No appointments found.\n";
		}
		fin.close();
	}

	void searchAppointments() {
		char searchDonorID[10];
		cout << "\nSearch Appointments by Donor ID\n";
		cout << "Enter Donor ID: ";
		getLine(searchDonorID, 10);

		ifstream fin("appointments.txt");
		if (!fin) {
			cout << "Could not open appointments.txt\n";
			return;
		}

		string line;
		bool found = false;

		while (getline(fin, line)) {
			char appointmentID[10], donorID[10], date[15], time[10];
			int field = 0, i = 0, j = 0;

			for (int k = 0; k <= getLength(line); k++) {
				if (line[k] == ',' || line[k] == '\0') {
					switch (field) {
					case 0: appointmentID[j] = '\0'; j = 0; field++; break;
					case 1: donorID[j] = '\0'; j = 0; field++; break;
					case 2: date[j] = '\0'; j = 0; field++; break;
					case 3: time[j] = '\0'; j = 0; field++; break;
					}
				}
				else {
					switch (field) {
					case 0: appointmentID[j++] = line[k]; break;
					case 1: donorID[j++] = line[k]; break;
					case 2: date[j++] = line[k]; break;
					case 3: time[j++] = line[k]; break;
					}
				}
			}

			bool match = true;
			for (int m = 0; searchDonorID[m] != '\0' || donorID[m] != '\0'; m++) {
				if (searchDonorID[m] != donorID[m]) {
					match = false;
					break;
				}
			}

			if (match) {
				found = true;
				cout << "\n------------------------------\n";
				cout << "Appointment ID : " << appointmentID << "\n";
				cout << "Donor ID       : " << donorID << "\n";
				cout << "Date           : " << date << "\n";
				cout << "Time           : " << time << "\n";
			}
		}

		if (!found) {
			cout << "No appointments found for Donor ID " << searchDonorID << "\n";
		}
		fin.close();
	}

	void viewMyInfo() {
		cout << "\nSTAFF INFO\n";
		cout << "Username: " << username << "\n";
		cout << "Role    : " << role << "\n";
	}

	void deleteAppointment() {
		char delApptID[20];
		cout << "\nDelete Appointment\nEnter Appointment ID to delete: ";
		getLine(delApptID, 20);

		ifstream fin("appointments.txt");
		ofstream fout("temp.txt");
		string line;
		bool found = false;

		while (getline(fin, line)) {
			char currentID[20];
			int i = 0, j = 0;

			while (line[i] != ',' && line[i] != '\0') currentID[j++] = line[i++];
			currentID[j] = '\0';

			bool match = true;
			for (int k = 0; delApptID[k] != '\0' || currentID[k] != '\0'; k++) {
				if (delApptID[k] != currentID[k]) { match = false; break; }
			}

			if (!match) fout << line << "\n";
			else found = true;
		}

		fin.close();
		fout.close();
		remove("appointments.txt");
		rename("temp.txt", "appointments.txt");

		cout << (found ? "Appointment deleted.\n" : "Appointment not found.\n");
	}
};

// Donor class
class Donor : public User {
private:
	char donorID[20], name[50], bloodGroup[5], gender[10], phone[15], ageStr[5], lastDonationDate[11];
	int age;
	bool eligible;

	void loadDonorData() {
		ifstream file("donors.txt");
		if (!file) return;

		string line;
		while (getline(file, line)) {
			char currentID[20], tempName[50], tempAge[5], tempBG[5], tempGender[10], tempPhone[15];
			int field = 0, i = 0, j = 0;

			while (i <= getLength(line)) {
				if (line[i] == ',' || line[i] == '\0') {
					switch (field) {
					case 0: currentID[j] = '\0'; break;
					case 1: tempName[j] = '\0'; break;
					case 2: tempAge[j] = '\0'; break;
					case 3: tempBG[j] = '\0'; break;
					case 4: tempGender[j] = '\0'; break;
					case 5: tempPhone[j] = '\0'; break;
					}
					field++; j = 0;
				}
				else {
					switch (field) {
					case 0: currentID[j++] = line[i]; break;
					case 1: tempName[j++] = line[i]; break;
					case 2: tempAge[j++] = line[i]; break;
					case 3: tempBG[j++] = line[i]; break;
					case 4: tempGender[j++] = line[i]; break;
					case 5: tempPhone[j++] = line[i]; break;
					}
				}
				i++;
			}

			if (stringsEqual(currentID, donorID)) {
				// Copy data to member variables
				int k = 0;
				while (tempName[k] != '\0') { name[k] = tempName[k]; k++; }
				name[k] = '\0';

				k = 0;
				while (tempAge[k] != '\0') { ageStr[k] = tempAge[k]; k++; }
				ageStr[k] = '\0';
				age = stringToNumber(ageStr);

				k = 0;
				while (tempBG[k] != '\0') { bloodGroup[k] = tempBG[k]; k++; }
				bloodGroup[k] = '\0';

				k = 0;
				while (tempGender[k] != '\0') { gender[k] = tempGender[k]; k++; }
				gender[k] = '\0';

				k = 0;
				while (tempPhone[k] != '\0') { phone[k] = tempPhone[k]; k++; }
				phone[k] = '\0';

				break;
			}
		}
		file.close();
	}

public:
	Donor(const string& uname, const string& pass) : User(uname, pass, "Donor") {
		// Initialize donorID from username
		int i = 0;
		while (uname[i] != '\0' && i < 19) {
			donorID[i] = uname[i];
			i++;
		}
		donorID[i] = '\0';

		eligible = true;
		lastDonationDate[0] = '\0';
		name[0] = '\0';
		bloodGroup[0] = '\0';
		gender[0] = '\0';
		phone[0] = '\0';
		ageStr[0] = '\0';
		age = 0;
		loadDonorData();
	}

	void showDashboard() override {
		int choice;
		do {
			cout << "\n\nDONOR DASHBOARD\n";
			cout << "1. View Profile\n";
			cout << "2. View Appointments\n";
			cout << "3. Check Eligibility\n";
			cout << "4. Change My Password\n";
			cout << "0. Logout\n";
			cout << "Enter choice: ";

			char input[10];
			getLine(input, 10);

			if (!isNumber(input)) {
				cout << "Invalid input. Please enter a number.\n";
				continue;
			}

			choice = stringToNumber(input);

			switch (choice) {
			case 1: viewProfile(); break;
			case 2: viewAppointments(); break;
			case 3: checkEligibility(); break;
			case 4: changePassword(); break;
			case 0: cout << "Logging out...\n"; break;
			default: cout << "Invalid choice!\n";
			}
		} while (choice != 0);
	}

private:
	void viewProfile() {
		cout << "\nDONOR PROFILE\n";
		cout << "ID          : " << donorID << "\n";
		cout << "Name        : " << name << "\n";
		cout << "Age         : " << ageStr << "\n";
		cout << "Blood Group : " << bloodGroup << "\n";
		cout << "Gender      : " << gender << "\n";
		cout << "Phone       : " << phone << "\n";
		cout << "Eligible    : " << (eligible ? "Yes" : "No") << "\n";
		if (lastDonationDate[0] != '\0') {
			cout << "Last Donation: " << lastDonationDate << "\n";
		}
	}

	void viewAppointments() {
		ifstream file("appointments.txt");
		if (!file) {
			cout << "No appointments found.\n";
			return;
		}

		cout << "\nYOUR APPOINTMENTS\n";
		string line;
		bool found = false;

		while (getline(file, line)) {
			char apptID[10], apptDonorID[10], date[15], time[10];
			int field = 0, pos = 0;

			for (int i = 0; i <= getLength(line); i++) {
				if (line[i] == ',' || line[i] == '\0') {
					switch (field) {
					case 0: apptID[pos] = '\0'; break;
					case 1: apptDonorID[pos] = '\0'; break;
					case 2: date[pos] = '\0'; break;
					case 3: time[pos] = '\0'; break;
					}
					pos = 0;
					field++;
				}
				else {
					switch (field) {
					case 0: apptID[pos++] = line[i]; break;
					case 1: apptDonorID[pos++] = line[i]; break;
					case 2: date[pos++] = line[i]; break;
					case 3: time[pos++] = line[i]; break;
					}
				}
			}

			if (stringsEqual(apptDonorID, donorID)) {
				found = true;
				cout << "----------------------------\n";
				cout << "Appointment ID: " << apptID << "\n";
				cout << "Date          : " << date << "\n";
				cout << "Time          : " << time << "\n";
			}
		}
		if (!found) {
			cout << "No appointments found for your account.\n";
		}
		file.close();
	}

	void checkEligibility() {
		cout << "\nELIGIBILITY CHECK\n";
		eligible = (age >= 18 && age <= 65);
		cout << "You are " << (eligible ? "ELIGIBLE" : "NOT ELIGIBLE") << " to donate blood.\n";

		if (!eligible) {
			if (age < 18) cout << "Reason: Minimum age is 18.\n";
			else cout << "Reason: Maximum age is 65.\n";
		}
	}
};

User* login() {
	char inputUser[50], inputPass[50];
	cout << "Username: ";
	getLine(inputUser, 50);
	cout << "Password: ";
	getLine(inputPass, 50);

	ifstream fin("users.txt");
	if (!fin) {
		cout << "Error: Cannot open users.txt\n";
		return nullptr;
	}

	string line;
	while (getline(fin, line)) {
		char fileUser[50], filePass[50], fileRole[15];
		int i = 0, j = 0;
		int len = getLength(line);

		while (i < len && line[i] != ',') {
			fileUser[j++] = line[i++];
		}
		fileUser[j] = '\0'; i++; j = 0;

		while (i < len && line[i] != ',') {
			filePass[j++] = line[i++];
		}
		filePass[j] = '\0'; i++; j = 0;

		while (i < len) {
			fileRole[j++] = line[i++];
		}
		fileRole[j] = '\0';

		bool userMatch = stringsEqual(inputUser, fileUser);
		bool passMatch = stringsEqual(inputPass, filePass);

		if (userMatch && passMatch) {
			if (fileRole[0] == 'A' && fileRole[1] == 'd' && fileRole[2] == 'm') {
				return new Admin(inputUser, inputPass);
			}
			else if (fileRole[0] == 'S' && fileRole[1] == 't') {
				return new Staff(inputUser, inputPass);
			}
			else if (fileRole[0] == 'D' && fileRole[1] == 'o') {
				return new Donor(inputUser, inputPass);
			}
		}
	}

	cout << "Invalid username or password.\n";
	return nullptr;
}

int main() {
	int choice;
	User* loggedInUser = nullptr;
	char input[10];

	do {
		cout << "============================================\n";
		cout << "  HOSPITAL BLOOD BANK MANAGEMENT SYSTEM\n";
		cout << "============================================\n";
		cout << "1. Admin Login\n";
		cout << "2. Staff Login\n";
		cout << "3. Donor Login\n";
		cout << "4. Exit\n";
		cout << "Enter your choice: ";

		getLine(input, 10);

		if (!isNumber(input)) {
			cout << "Invalid input. Please enter a number.\n";
			continue;
		}

		choice = stringToNumber(input);

		switch (choice) {
		case 1:
		case 2:
		case 3: {
					cout << "\nPlease enter your credentials:\n";
					loggedInUser = login();

					if (loggedInUser != nullptr) {
						string selectedRole;
						if (choice == 1) selectedRole = "Admin";
						else if (choice == 2) selectedRole = "Staff";
						else selectedRole = "Donor";

						if (loggedInUser->getRole() != selectedRole) {
							cout << "\nAccess Denied. Please login as " << selectedRole << ".\n";
							delete loggedInUser;
							loggedInUser = nullptr;
						}
						else {
							cout << "\nLogin successful! Welcome, " << loggedInUser->getUsername() << "!\n";
							loggedInUser->showDashboard();
							delete loggedInUser;
							loggedInUser = nullptr;
						}
					}
					break;
		}
		case 4:
			cout << "\nThank you for using the Hospital Blood Bank Management System!\n";
			cout << "Exiting...\n";
			break;
		default:
			cout << "Invalid choice. Please try again.\n";
		}

		if (choice != 4) {
			cout << "\nPress Enter to continue...";
			cin.ignore();
		}
	} while (choice != 4);

	return 0;
}