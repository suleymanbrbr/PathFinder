#include <iostream>
#include <fstream>
#include <cctype>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <chrono>
#include <ctime>

using namespace std;

int flight_id = -1;
int airline_id = 0;
string temp_from = "";

// Flight and airline structs initializes some values
struct flight{
  string from;
  string to;
  int hour;
  int min;
  int price;
  int ID;
  
  flight *next;
  flight *prev;
    
    flight (string begin, string end, int h, int m, int p, int id)
        : from(begin), to(end), hour(h), min(m), price(p), ID(id)
    {};
    
    flight (string begin, string end, int h, int m, int p, int id, flight * prev)
        : from(begin), to(end), hour(h), min(m), price(p), ID(id), prev(prev)
    {};
};

struct airline{
  string name;
  int ID;
  airline *next;
  flight *flights;

  airline (string n, int id)
       : name(n), ID(id)
  {};
};

vector<vector<flight>> possible_flights;  // The vector includes possible paths

// This compare_flight struct is useful when we want to sort a collection of flight objects in a specific order, sorting them by their departure times (hour and minute).
struct compare_flight
{
    inline bool operator() (const struct flight& first, const struct flight& last)
    {
        if (first.hour < last.hour) {
            return true;
        }
        else if (first.hour > last.hour) {
            return false;
        }
        if (first.min < last.min) {
            return true;
        }
        else if (first.min > last.min) {
            return false;
        }
        return false;
    }
};

// The read_files function is designed to read and organize flight information from multiple airline files, allowing manual changes if necessary.

pair<vector<string>, vector<vector<flight>>> read_files(bool input_done){
  
  vector<string> airlines;
  vector<vector<flight>> flights(100); //start with a previously sized vector or you get seg fault (for sure)
  
  while(not input_done){
    string airline_name;
    string filename;
    cout << "Enter the name of airline: ";
    cin >> airline_name;
    cout << "Enter the name of airline file: ";
    cin >> filename;
    ifstream input(filename);

    if(filename == "done"){
      input_done = true;
      return make_pair(airlines, flights);
    }
    
    while( input.fail() ){
        
      cout << "Could not open file: " << filename << endl;
      cout << "Enter the name of airline: ";
      cin >> airline_name;
      cout << "Enter the name of airline file: ";
      cin >> filename;
      input.open(filename);

      if(filename == "done"){
      input_done = true;
      return make_pair(airlines, flights);
      }
    }
    airlines.push_back(airline_name);
   
    cout << "Processing input file: " << filename <<endl;
    
      int line_no = 0;
      vector<string> lines;
      string line;
      int vector_location = airlines.size() - 1;
      
      while(getline(input, line)){
        lines.push_back(line);
        line_no += 1;
          
        if(line_no % 5 == 0){
          flight_id++;
          flight a_flight(lines[line_no-5], lines[line_no-4], stoi(lines[line_no-3]), stoi(lines[line_no-2]), stoi(lines[line_no-1]), flight_id);
          flights[vector_location].push_back(a_flight);
         
        }
      }
    }
    if(input_done){
      cout << "Input files are already read, however you can do manual changes.." << endl;
    }
    return make_pair(airlines, flights);
  }

// The make_linked_list_structure function appears to create a linked list data structure containing information about airlines and their corresponding flights
airline* make_linked_list_structure(vector<string> airlines, vector<vector<flight>> flights){
  
    //It starts by sorting the flights vector using the compare_flight() comparison function for each airline to ensure that flights within an airline are sorted based on their departure times.
    for (int j = 0; j < flights.size(); j++){
        if(flights[j].size() > 0)
        sort(flights[j].begin(), flights[j].end(), compare_flight());
    }

    airline * head = nullptr, * current_airline = nullptr;
    flight * f_head = nullptr, * ptr = nullptr;
  
    for (int i = 0; i<airlines.size(); i++){
       
        if ( i == 0){
            head = new airline (airlines[i], airline_id);
            current_airline = head;
            airline_id++;
        }
        else {
            current_airline->next = new airline (airlines[i], airline_id);
            current_airline = current_airline->next;
            airline_id++;
        }
        flight * previous = nullptr;
        
        for (int j = 0; j<flights[i].size(); j++){
            
            if ( j == 0){
                f_head = new flight (flights[i][j].from, flights[i][j].to, flights[i][j].hour, flights[i][j].min, flights[i][j].price, flights[i][j].ID, previous);
                ptr = f_head;
                previous = ptr;
                current_airline->flights = f_head;
            }
            else {
                ptr->next = new flight (flights[i][j].from, flights[i][j].to, flights[i][j].hour, flights[i][j].min, flights[i][j].price, flights[i][j].ID, previous);
                previous = ptr;
                ptr = ptr->next;
            }
        }
        ptr->next = nullptr;
    }
    current_airline->next = nullptr;
    return head;
}

// This cheap_path function finds and displays the cheapest flight route among the provided possible routes and provides information about the flights included in that route.
void cheap_path (const vector<vector<flight>> &possible_routes, string from, string to){
    
    int min_cost = INT_MAX, temp=0;
    for (int i = 0; i<possible_routes.size(); i++){
        int total_cost = 0;
        
        for (int j = 0; j<possible_routes[i].size(); j++)
            total_cost += possible_routes[i][j].price;
        
        if (min_cost>total_cost){
            min_cost = total_cost;
            temp = i;
        }
    }
    if (possible_flights.size() == 0)
        cout << "No path found between " << from << " and " << to << endl;
    else {
        
        for (int j = 0; j<possible_routes[temp].size(); j++){
            cout << "[" << possible_routes[temp][j].ID << "|" << possible_routes[temp][j].from << "->" << possible_routes[temp][j].to << "|" << possible_routes[temp][j].hour << ":" << possible_routes[temp][j].min << "|" << possible_routes[temp][j].price << "]";
        }
        cout << "  $TOTAL PRICE: " << min_cost;
    }
}

// The find_path function appears to be a recursive function that searches for flight routes from an initial location (from) to a destination (to) within a linked list of airlines and their associated flights. The function keeps track of the flights that make up a route and ensures that the route meets certain criteria, including a maximum number of allowed transfers.
void find_path(airline* head, string from, string to, int max_transfer, vector<flight> flights_routes){
    
    airline * ptr = head;
    
    while (ptr != nullptr) {
        
        string current_location;
        flight * current = ptr->flights;
        
        while (current != nullptr){
            
            if (current->from == from){
                
                current_location = current->to;
                flights_routes.push_back(*current);
                
                if (current_location == to && (flights_routes[0].from == temp_from) && (flights_routes.size()-1 <= max_transfer)) // when it finds a path, adds to possible_flights vector
                    possible_flights.push_back(flights_routes);
                    
                else if (flights_routes.size()-1 < max_transfer) // when it does not find a path, continue to search path
                    find_path(head, current_location, to, max_transfer, flights_routes);
                
                flights_routes.pop_back();
                }
            current = current->next;
        }
        ptr = ptr->next;
    }
}

// The pathfinder function acts as a user interface for finding the best price path from a given starting location to a destination while taking into account the maximum number of transfers allowed. It uses the find_path function to explore possible routes and the cheap_path function to find the cheapest route among the possibilities.
void pathfinder(airline* head){
    
    string current_location;
    string from, to;
    vector<flight> flights_routes;
    int max_transfer;
    
    cout << "Where are you now?" << endl;
    cin >> from;
    temp_from = from;
    cout << "Where do you want to go" << endl;
    cin >> to;
    cout << "Maximum number of transfers:" << endl;
    cin >> max_transfer;
    cout << "##Best price path##" << endl;
    find_path(head, from, to, max_transfer, flights_routes);
    cheap_path(possible_flights, from, to);
    possible_flights.clear();
}
 
// The delete_linked_list functions are intended for managing a hierarchical data structure where airlines have associated flights, and both need to be deleted properly to avoid memory leaks.
void delete_linked_list(airline* &head){
    
    airline * ptr;
    while (head) {
        
        flight * temp, * temporary;
        temp = head->flights;
        while (temp) {
            temporary = temp->next;
            delete temp;
            temp = temporary;
        }
        ptr = head->next;
        delete head;
        head = ptr;
    }
}

// The aim of the print_all function is to provide a way to display information about airlines and their associated flights.
void print_all(airline* head){
    
    if (head == nullptr){
        cout << "List is empty.." << endl;
       
    }
    else {
        airline * ptr = head;
        while (ptr) {
            cout << "###################################" << endl;
            cout << "### AIRLINE ID: " << ptr->ID  << " ###" << endl;
            cout << "NAME:" << ptr->name << endl <<  "FLIGHTS: ";
            flight * temp = ptr->flights;
            ptr = ptr->next;
            
            while (temp != nullptr)
            {
                cout << "#[" << temp->ID << "|" << temp->from << "->" << temp->to << "|" << temp->hour << ":" << temp->min << "|" << temp->price << "TRY]#";
                temp = temp->next;
            }
        }
    }
}

// The add_flight_with_input function is designed to add a new flight to a sorted list of airlines, where each airline can have multiple flights associated with it.
void add_flight_with_input(airline* &head){
    
    airline * temporary, * previous_airline = nullptr;
    temporary = head;
    string name, from, to;
    int hour, min, price;
    cout << "Adding manual entry:" << endl << "AIRLINE: ";
    cin >> name;
    cout << "FROM: ";
    cin >> from;
    cout << "TO: ";
    cin >> to;
    cout << "HOUR: ";
    cin >> hour;
    cout << "MIN: ";
    cin >> min;
    cout << "PRICE: ";
    cin >> price;
    cout << ">>" << endl;
    
    // If no matching airline is found (temporary becomes nullptr), it creates a new airline node with the provided name and airline_id, and appends it to the end of the list. It also creates a new flight node and sets it as the only flight for this new airline.
    while (temporary != nullptr && temporary->name != name) {
        previous_airline = temporary;
        temporary = temporary->next;
    }
    
    flight * new_flight;
    
    if (temporary == nullptr) {
        airline * new_airline = new airline (name, airline_id);
        airline_id++;
        previous_airline->next = new_airline;
        flight_id++;
        new_flight = new flight(from, to, hour, min, price, flight_id, nullptr);
        new_flight->next = nullptr;
        new_airline->flights = new_flight;
    }
    
    // If a matching airline is found (temporary is not nullptr), it proceeds to add the new flight to the existing airline's list of flights in a sorted order based on the departure time (hour and minute). It checks if the flight list is empty, if the new flight should be inserted at the beginning, or if it should be inserted in the middle or at the end of the list, maintaining the sorted order.
    else {
        flight * temp = temporary->flights, * ptr = temp;
        
        if (temp == nullptr)
        {
            flight_id++;
            new_flight = new flight(from, to, hour, min, price, flight_id, nullptr);
            new_flight->next = nullptr;
            temp = new_flight;
            airline_id++;
            
        }
        else if (temp != nullptr && ((temp->hour) > hour || (temp->hour == hour && temp->min > min))){
            flight_id++;
            new_flight = new flight(from, to, hour, min, price, flight_id, nullptr);
            new_flight->next = ptr;
            ptr->prev = new_flight;
            temp = new_flight;
        }
        
        else {
            
            while (ptr->next != nullptr && ((ptr->next->hour) < hour || (ptr->next->hour == hour && ptr->next->min < min))) {
                ptr = ptr->next;
            }
            flight_id++;
            new_flight = new flight(from, to, hour, min, price, flight_id, ptr);
            if (ptr->next == nullptr) {
                ptr->next = new_flight;
                new_flight->next = nullptr;
            }
            else{
                new_flight->next = ptr->next;
                ptr->next = new_flight;
                new_flight->next->prev = new_flight;
            }
        }
        temporary->flights = temp;
    }
    cout << "Flight ID " << flight_id << " is added to the list.." << endl;
}

// The remove_flight_with_input function is designed to remove a flight with a specified ID from a list of airlines, where each airline can have multiple flights associated with it.
void remove_flight_with_input(airline* &head){
    
    bool flag = false;
    int id;
    cout << "Flight id to remove: ";
    cin >> id;
    airline * temporary = head, * prev = head;
    
    // It enters a do-while loop to search for and remove the flight with the specified ID. The loop continues until the flight is found and removed (flag becomes true) or the end of the list is reached (temporary becomes nullptr).
    do {
        flight * temp, * ptr, * deleted;
        temp = temporary->flights;
        ptr = temp;
        if (temporary == nullptr)
            break;
        
        // If the temporary airline had no flights left after the removal, it checks the following cases and removes the airline itself
        if (temp != nullptr && id == temp->ID)
        {
            deleted = ptr;
            if (ptr->next == nullptr)
                temp = nullptr;
            
            else {
                ptr = ptr->next;
                ptr->prev = nullptr;
                temp = ptr;
            }
            delete deleted;
            temporary->flights = temp;
            flag= true;
            
            if (temp == nullptr){
                if (temporary->ID == 0 && temporary->next != nullptr)
                    head = temporary->next;
                
                else if (temporary->ID == 0 && temporary->next == nullptr)
                    head = nullptr;
                
                else if (temporary->ID == 1 )
                    head->next = temporary->next;
                   
                else
                    prev->next = temporary->next;
                
                delete temporary;
            }
        }
        // If the flight with the specified ID is not found in the current airline, it iterates through the list of flights within the airline to find and remove the flight in a similar way
        else
        {
            while (ptr->next != nullptr && ptr->next->ID != id){
                ptr = ptr->next;
            }
            
            if (ptr->next == nullptr){
                prev = temporary;
                temporary = temporary->next;
            }
            else {
                deleted = ptr->next;
                flag= true;
                
                if (ptr->next->next == nullptr)
                    ptr->next = nullptr;
                
                else {
                    flight * previous;
                    previous = ptr;
                    ptr = deleted->next;
                    previous->next = ptr;
                    ptr->prev = previous;
                }
                delete deleted;
            }
        }
    // After exiting the loop, it checks the flag to determine whether the flight was successfully removed. If the flight was removed, it prints a message indicating that the specified flight has been removed.
    } while (flag == false && temporary != nullptr);
    
    if (flag == true)
        cout << "Flight ID " << id << " is removed from the list.."<< endl;
    
    cout << ">>";
}

// The printMainMenu function is responsible for displaying a simple text-based menu with various options to the user.
void printMainMenu() {
  cout << endl;
  cout <<"I*****I"<<endl
       <<"I               0 - DELETE DATA                 I"<<endl
       <<"I               1 - READ FILES                  I"<<endl
       <<"I               2 - PRINT ALL FLIGHTS           I"<<endl
       <<"I               3 - ADD FLIGHT                  I"<<endl
       <<"I               4 - REMOVE FLIGHT               I"<<endl
       <<"I               5 - PATH FINDER                 I"<<endl
       <<"I               6 - EXIT                        I"<<endl
       <<"I*****I"<<endl
       <<">>";
  cout << endl;
}

// The processMainMenu function that serves as the main loop for a program or system managing airline and flight data. It displays a menu to the user, processes user input, and performs various actions based on the selected menu option.
void processMainMenu() {

  pair<vector<string>, vector<vector<flight>>> lines_flights;
  airline* head = nullptr;
  bool input_done = 0;
  char input;
  do{
      
    printMainMenu();
    cout << "Please enter your option " << endl;
    cin >> input;
    switch (input) {
    case '0':
      delete_linked_list(head);
      cout << "Data is destroyed.." << endl;
      input_done = false;
      break;
    case '1':
      if(not input_done){
        lines_flights = read_files(input_done);
          
        head = make_linked_list_structure(lines_flights.first, lines_flights.second);
    cout << "Files have read.." << endl;
      }
      else{
    cout << "Files have already read.." << endl;
      }
      input_done = true;
      break;
    case '2':
      print_all(head);
      break;
    case '3':
      add_flight_with_input(head);
      break;
    case '4':
      remove_flight_with_input(head);
      break;
    case '5':
      pathfinder(head);
      break;
    case '6':
      cout << "Exiting.." << endl;
      exit(0);
    default:
      cout << "Invalid option: please enter again" << endl;
    }
  } while(true);
}

int main(){
  
  processMainMenu();
  return 0;
}

// SULEYMAN BERBER

