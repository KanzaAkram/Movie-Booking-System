#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

class FileManager {
public:
    static bool writeToFile(const string& filename, const string& content) {
        ofstream file(filename, ios::app);
        if (file.is_open()) {
            file << content << endl;
            file.close();
            return true;
        } else {
            cout << "Error: Unable to open file '" << filename << "' for writing.\n";
            return false;
        }
    }

    static bool readFromFile(const string& filename, vector<string>& lines) {
        ifstream file(filename);
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                lines.push_back(line);
            }
            file.close();
            return true;
        } else {
            cout << "Error: Unable to open file '" << filename << "' for reading.\n";
            return false;
        }
    }

    static bool overwriteFile(const string& filename, const vector<string>& lines) {
        ofstream file(filename);
        if (file.is_open()) {
            for (const string& line : lines) {
                file << line << endl;
            }
            file.close();
            return true;
        } else {
            cout << "Error: Unable to open file '" << filename << "' for writing.\n";
            return false;
        }
    }
};

class Movie {
protected:
    string name;
    vector<string> weekdays;
    vector<string> timings;
    int availableSeats;
    map<string, bool> bookings; // Map to store bookings for each date, day, and time combination

public:
    Movie(const string& name, const vector<string>& weekdays,
          const vector<string>& timings, int availableSeats)
        : name(name), weekdays(weekdays), timings(timings), availableSeats(availableSeats) {
        if (this->availableSeats > 30) {
            this->availableSeats = 30;
        }
    }

    virtual ~Movie() {}

    const string& getName() const {
        return name;
    }

    const vector<string>& getWeekdays() const {
        return weekdays;
    }

    const vector<string>& getTimings() const {
        return timings;
    }

    int getAvailableSeats() const {
        return availableSeats;
    }

    virtual double getTicketPrice() const {
        return 10.0; // Default ticket price for a movie
    }

    void decreaseAvailableSeats(int numSeats) {
        availableSeats -= numSeats;
    }

    bool isBooked(const string& date, const string& weekday, const string& timing) const {
        string key = date + "-" + weekday + "-" + timing;
        auto it = bookings.find(key);
        return (it != bookings.end() && it->second);
    }

    void book(const string& date, const string& weekday, const string& timing) {
        string key = date + "-" + weekday + "-" + timing;
        bookings[key] = true;
    }
};

class BlockbusterMovie : public Movie {
public:
    BlockbusterMovie(const string& name, const vector<string>& weekdays,
                     const vector<string>& timings, int availableSeats)
        : Movie(name, weekdays, timings, availableSeats) {
        if (this->availableSeats > 30) {
            this->availableSeats = 30;
        }
    }

    double getTicketPrice() const override {
        return 15.0; // Blockbuster movie ticket price
    }
};

class RegularMovie : public Movie {
public:
    RegularMovie(const string& name, const vector<string>& weekdays,
                 const vector<string>& timings, int availableSeats)
        : Movie(name, weekdays, timings, availableSeats) {
        if (this->availableSeats > 30) {
            this->availableSeats = 30;
        }
    }

    double getTicketPrice() const override {
        return 10.0; // Regular movie ticket price
    }
};

class Person {
protected:
    string name;
    string phone;

public:
    Person(const string& name, const string& phone)
        : name(name), phone(phone) {}

    const string& getName() const {
        return name;
    }

    const string& getPhone() const {
        return phone;
    }
};

class Customer : public Person {
public:
    Customer(const string& name, const string& phone)
        : Person(name, phone) {}
};

class BookingSystem {
private:
    vector<Movie*> movies;
    vector<Customer> customers;

public:
    BookingSystem() {
        movies = {
            new RegularMovie("AVATAR", {"Saturday", "Sunday"}, {"1.00pm", "5.00pm", "8.00pm"}, 30),
            new RegularMovie("SPIDERMAN", {"Sunday", "Monday", "Tuesday", "Wednesday"}, {"1.30pm", "6.30pm", "9.30pm"}, 30),
            new BlockbusterMovie("IRONMAN", {"Tuesday", "Wednesday", "Thursday"}, {"2.00pm", "7.00pm", "10.00pm"}, 30)
        };
    }

    ~BookingSystem() {
        for (Movie* movie : movies) {
            delete movie;
        }
    }

    void displayMovies() const {
        cout << "Available Movies:\n";
        for (size_t i = 0; i < movies.size(); ++i) {
            const Movie* movie = movies[i];
            cout << i + 1 << ". " << movie->getName() << " (";
            for (const string& weekday : movie->getWeekdays()) {
                cout << weekday << ", ";
            }
            cout << ")" << " (" << movie->getAvailableSeats() << " seats available)" << endl;
        }
    }

    void displayMovieTimings(const Movie* movie) const {
        cout << "Available Timings for " << movie->getName() << ":\n";
        for (const string& timing : movie->getTimings()) {
            cout << "- " << timing << endl;
        }
    }

    void saveCustomerInfo(const string& name, const string& phone) {
        Customer customer(name, phone);
        customers.push_back(customer);
        string content = "Name: " + name + "\nPhone: " + phone + "\n-------------------------";
        FileManager::writeToFile("c1.txt", content);
    }

    void saveBookingDetails(const string& name, const string& phone, const string& movieName,
                            const string& weekday, const string& timing, int numSeats, double ticketPrice) {
        string content = "Name: " + name + "\nPhone: " + phone + "\nMovie: " + movieName +
            "\nWeekday: " + weekday + "\nTiming: " + timing +
            "\nSeats: " + to_string(numSeats) +
            "\nTicket Price: " + to_string(ticketPrice) +
            "\n-------------------------";
        FileManager::writeToFile("bookshow.txt", content);
    }

    void deleteBooking() {
        cout << "Delete Booking\n";

        // Get customer's phone number
        string phone;
        cout << "Enter your phone number: ";
        cin >> phone;

        // Delete booking from bookshow.txt
        vector<string> bookingLines;
        FileManager::readFromFile("bookshow.txt", bookingLines);
        bool bookingFound = false;

        if (!bookingLines.empty()) {
            vector<string> newBookingLines;
            for (size_t i = 0; i < bookingLines.size(); ++i) {
                if (bookingLines[i].find("Phone: " + phone) != string::npos) {
                    bookingFound = true;
                    // Skip the booking details and the date line
                    i += 8;
                } else {
                    newBookingLines.push_back(bookingLines[i]);
                }
            }
            if (bookingFound) {
                if (!FileManager::overwriteFile("bookshow.txt", newBookingLines)) {
                    cout << "Error: Unable to update booking file.\n";
                    return;
                }
            }
        } else {
            cout << "Error: No booking details found.\n";
            return;
        }

        if (!bookingFound) {
            cout << "Booking not found for the given phone number.\n";
            return;
        }

        // Increase available seats count
        for (Movie* movie : movies) {
            if (movie->getName() != "Deleted") {
                movie->decreaseAvailableSeats(-1); // Increase by 1 seat
            }
        }

        // Delete customer details from c1.txt
        vector<string> customerLines;
        FileManager::readFromFile("c1.txt", customerLines);
        bool customerFound = false;

        if (!customerLines.empty()) {
            vector<string> newCustomerLines;
            for (size_t i = 0; i < customerLines.size(); ++i) {
                if (customerLines[i].find("Phone: " + phone) != string::npos) {
                    customerFound = true;
                } else {
                    newCustomerLines.push_back(customerLines[i]);
                }
            }
            if (customerFound) {
                if (!FileManager::overwriteFile("c1.txt", newCustomerLines)) {
                    cout << "Error: Unable to update customer file.\n";
                    return;
                }
            }
        } else {
            cout << "Error: No customer details found.\n";
            return;
        }

        cout << "Booking deleted successfully.\n";
    }

    void bookTicket() {
        cout << "Book Ticket\n";

        // Get customer details
        string name, phone;
        cout << "Enter your name: ";
        cin.ignore();
        getline(cin, name);

        // Get and validate phone number
        bool validPhone = false;
do {
    cout << "Enter your phone number (11 digits): ";
    cin >> phone;

    // Check if the phone number contains only digits and has a length of 11
    if (phone.length() == 11 && std::all_of(phone.begin(), phone.end(), [](char c) { return std::isdigit(c); })) {
        validPhone = true;
    } else {
        cout << "Invalid phone number. Please enter 11 digits without any alphabets.\n";
    }
} while (!validPhone);


        // Save customer details
        saveCustomerInfo(name, phone);

        // Display movie options
        displayMovies();

        // Get movie choice
        int movieChoice;
        cout << "Enter the movie number: ";
        cin >> movieChoice;

        // Validate movie choice
        if (movieChoice < 1 || movieChoice > movies.size()) {
            cout << "Invalid movie choice.\n";
            return;
        }

        Movie* selectedMovie = movies[movieChoice - 1];

        // Display available weekdays for the selected movie
        cout << "Available Weekdays for " << selectedMovie->getName() << ":\n";
        for (const string& weekday : selectedMovie->getWeekdays()) {
            cout << "- " << weekday << endl;
        }

        // Get weekday choice
        string selectedWeekday;
        cout << "Enter the desired weekday: ";
        cin >> selectedWeekday;

        // Validate weekday choice
        bool validWeekday = false;
        for (const string& weekday : selectedMovie->getWeekdays()) {
            if (selectedWeekday == weekday) {
                validWeekday = true;
                break;
            }
        }

        if (!validWeekday) {
            cout << "Invalid weekday choice.\n";
            return;
        }

        // Display available timings for the selected movie and weekday
        displayMovieTimings(selectedMovie);

        // Get timing choice
        int timingChoice;
        cout << "Enter the timing number: ";
        cin >> timingChoice;

        // Validate timing choice
        if (timingChoice < 1 || timingChoice > selectedMovie->getTimings().size()) {
            cout << "Invalid timing choice.\n";
            return;
        }

        const string& selectedTiming = selectedMovie->getTimings()[timingChoice - 1];

        // Get date choice
        string selectedDate;
        cout << "Enter the desired date (DD/MM/YYYY): ";
        cin >> selectedDate;

        // Check if the selected movie, date, day, and time are booked
        if (selectedMovie->isBooked(selectedDate, selectedWeekday, selectedTiming)) {
            cout << "Sorry, the selected movie is already booked for the specified date, day, and time.\n";
            return;
        }

        // Book the movie for the selected date, day, and time
        selectedMovie->book(selectedDate, selectedWeekday, selectedTiming);

        // Check available seats
        if (selectedMovie->getAvailableSeats() <= 0) {
            cout << "Sorry, no seats available for the selected movie, weekday, and timing.\n";
            return;
        }

        // Get number of seats to book
        int numSeats;
        cout << "Enter the number of seats to book: ";
        cin >> numSeats;

        // Validate number of seats
        if (numSeats <= 0 || numSeats > selectedMovie->getAvailableSeats()) {
            cout << "Invalid number of seats.\n";
            return;
        }

        // Decrease available seats count
        selectedMovie->decreaseAvailableSeats(numSeats);

        // Get ticket price
        double ticketPrice = selectedMovie->getTicketPrice()*numSeats;

        // Save booking details
        saveBookingDetails(name, phone, selectedMovie->getName(), selectedWeekday, selectedTiming, numSeats, ticketPrice);

        cout << "Booking successful!\n";
    }

    void run() {
        int choice;
        do {
            cout << "Menu:\n";
            cout << "1. Book Ticket\n";
            cout << "2. View Available Shows\n";
            cout << "3. Delete Booking\n";
            cout << "4. Exit\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1:
                    bookTicket();
                    break;
                case 2:
                    displayMovies();
                    break;
                case 3:
                    deleteBooking();
                    break;
                case 4:
                    cout << "Goodbye!\n";
                    break;
                default:
                    cout << "Invalid choice. Try again.\n";
                    break;
            }

            cout << endl;
        } while (choice != 4);
    }
};

int main() {
    system("color 3d");
    BookingSystem bookingSystem;
    bookingSystem.run();
    return 0;
}






