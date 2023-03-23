/********************************
File Name: Definitions.h
Part of: Project_Vending_Machine
Written by: Devson Butani
Last Edit: [18/06/2022]
*********************************/

#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <map>
#include <cmath>
#include <iterator>
#include <algorithm>
#include <limits>
#include <vector>
#include <ranges>
#include <conio.h>

using namespace std;

//****Predefined Values****

// Pre-defined map of commands - these are constant and global
map<string, int> command_map = {
    {"HELP", 1},
    {"STATUS", 2},
    {"EXIT", 3},
    {"LOCK", 4},
    {"UNLOCK", 5},
    {"VEND", 6},
    {"ADD", 7},
    {"REMOVE", 8},
    {"COIN", 9},
    {"BILL", 10},
    {"DRINK", 11},
    {"CUP", 12}};
// Maps of denominations and their values - these are constant and global
map<string, double> coin = {
    {"NICKEL", 0.05},
    {"DIME", 0.10},
    {"QUARTER", 0.25}};
map<string, double> bill = {
    {"ONEDOLLAR", 1.00},
    {"FIVEDOLLAR", 5.00}};
// Note: No drink price map since price is same for all drinks in this implementation

// Service mode status indicator
bool service_mode = true;

// Service mode password
string passkey = "Password";

// Allowed keys for user/service modes
const int service_keys[] = {1, 2, 3, 4, 9, 10, 11, 12};
const int user_keys[] = {1, 3, 5, 6, 9, 10};

//****Functions****

// Check if given key is valid for the current user mode. Uses service/user_keys
int valid_key(int k, bool mode)
{
    if (mode)
    {
        // cout << "service mode" << endl;
        if (find(begin(service_keys), end(service_keys), k) != end(service_keys))
        {
            // cout << k << endl;
        }
        else
        {
            k = 0;
        }
    }
    else if (!mode)
    {
        // cout << "user mode" << endl;
        if (find(begin(user_keys), end(user_keys), k) != end(user_keys))
        {
            // cout << k << endl;
        }
        else
        {
            k = 0;
        }
    }
    return k;
}

// Text to display for HELP command. Uses the user/service mode as input
void help_text(bool service_mode)
{
    if (service_mode)
    {
        cout << "_____________________________________________________________________________________\n";
        cout << "\nCommands in SERVICE MODE are: \n\n";

        cout << "[Coin] <Denomination> <Quantity> \n";
        cout << "   Accepted denominations are <Nickel|Dime|Quarter> \n\n";
        cout << "   Example: <Coin dime 3> OR <COIN Nickel -7> \n\n";

        cout << "[Bill] <Denomination> <Quantity> \n";
        cout << "   Accepted denominations are <OneDollar|FiveDollar> \n";
        cout << "   Example: <bill OneDollar 2> <BILL FIVEDOLLAR -1>\n\n";

        cout << "[Drink] <Brand> <Quantity> - available brands are <Coke|Sprite|Pepsi|Fanta|Water>\n";
        cout << "   Example: <Drink Coke 5> OR <DRINK water -2> \n\n";

        cout << "[Cup] <Quantity> \n";
        cout << "[Lock] <ServiceKey> \n";
        cout << "[Status]\n";
        cout << "[Exit]\n";
        cout << "_____________________________________________________________________________________\n"
             << endl;
    }
    else
    {
        cout << "_____________________________________________________________________________________\n";
        cout << "\nCommands in USER MODE are: \n\n";

        cout << "[Coin] <Denomination> <Quantity> \n";
        cout << "   Accepted denominations are <Nickel|Dime|Quarter> \n\n";
        cout << "   Example: <Coin Dime> OR <COIN QUARTER> \n\n";

        cout << "[Bill] <Denomination> <Quantity> \n";
        cout << "   Accepted denominations are <OneDollar|FiveDollar> \n";
        cout << "   Example: <bill onedollar> OR <BILL FIVEDOLLAR>\n\n";

        cout << "[Vend] <Brand> - available brands are <Coke|Sprite|Pepsi|Fanta|Water>\n";
        cout << "   Example: <VEND Coke> OR <vend SPRITE> \n\n";

        cout << "[Unlock] <ServiceKey> - For customer service use only \n";
        cout << "[Exit] \n";
        cout << "_____________________________________________________________________________________\n"
             << endl;
    }
}

// Function for converting strings to uppercase.
string anytoupper(string s)
{
    // using transform() function and ::toupper in STL
    transform(s.begin(), s.end(), s.begin(), ::toupper);
    // Test cout << su << endl;
    return s;
}

// Function for comparing strings 1 to 1. Used for checking passwords
bool compareCommand(string s1, string s2)
{
    // using compare() function in STL
    if (s1 == "")
    {
        cout << "!! Incorrect input. Expected [Command] <key>\n";
        cout << "!! Use [LOCK|UNLOCK] <ServiceKey> - key is case sensitive" << endl;
        return false;
    }
    else if (s1.compare(s2) == 0)
    {
        // cout << "Password Accepted" << endl;
        system("cls"); // I know, I know...
        return true;
    }
    else
    {
        cout << "!! Incorrect Service Key" << endl;
        return false;
    }
}

// Separates input string into vector elements. " " space delimited
vector<string> separate(string buffer)
{
    vector<string> v;
    string delim = " "; // Space as delimiter
    // Initiate string pos locator and locate delimiter
    size_t start = 0U;
    size_t end = buffer.find(delim);
    while (end != std::string::npos)
    {
        // Write input into a vector
        v.push_back(buffer.substr(start, end - start));
        // Update string pos locator for next piece
        start = end + delim.length();
        end = buffer.find(delim, start);
    }
    v.push_back(buffer.substr(start));
    
    // Prevent out of range error
    for (size_t i = 0; i < 3; i++)
    {
        if (v.size() < 3)
        {
            v.push_back("");
        }
    }

    return v;
}

// Convert (string) to (int)
int string_to_int(string buffer)
{
    // Convert expected string numeral to int
    // If unexpected input, return 0
    int qt = 0;
    try
    {
        qt = stoi(buffer);
    }
    catch (invalid_argument const &e)
    {
        cout << "!! Bad input. Enter <HELP> for a list commands." << endl;
        return 0;
    }
    catch (out_of_range const &e)
    {
        cout << "!! Integer overflow. Enter <HELP> for a list commands." << endl;
        return 0;
    }
    return qt;
}

// Read File to map inventory items and quatitiy. Not for current scope
bool read_inventory_from_file()
{
    map<string, int> inventory_map;
    // Open and read file to load inventory into a map
    ifstream fin("./inventory.txt");

    // $$$>Error exit needs update<$$$
    if (!fin.is_open())
    {
        cerr << "Could not open the inventory file" << endl;
        return false;
    }

    // Find individual elements in string
    // Expected format: Item Qty
    //"Item 3" <string><space><int>
    string buffer;
    while (getline(fin, buffer))
    {
        string delim = " "; // Space as delimiter
        // Initiate string pos locator and locate delimiter
        size_t start = 0U;
        size_t end = buffer.find(delim);
        string line_item = buffer.substr(start, end - start);
        // Update string pos locator for next piece
        start = end + delim.length();
        end = buffer.find(delim, start);

        // Convert expected string numeral to int
        // If unexpected input, return false and close file
        int qt = 0;
        try
        {
            qt = stoi(buffer.substr(start));
        }
        catch (invalid_argument const &e)
        {
            cout << "Bad input from 'inventory.txt': std::invalid_argument thrown" << endl;
            fin.close();
            return false;
        }
        catch (out_of_range const &e)
        {
            cout << "Integer overflow in 'inventory.txt': std::out_of_range thrown" << endl;
            fin.close();
            return false;
        }

        // Save data into the inventory map
        inventory_map.insert(pair<string, int>(line_item, qt));
    }
    fin.close();
    // File closed and all input recorded

    // For verification only:
    // Print final inventory map created using iterator
    map<string, int>::iterator itr;
    for (itr = inventory_map.begin(); itr != inventory_map.end(); ++itr)
    {
        cout << itr->first << "\t" << itr->second << '\n';
    }
    cout << endl;
    return true;
}

//****Classes****

// Vending machine class
class vending_machine
{
private:
    double deposit_amount;
    double drink_price;
    // Inventory data management map
    map<string, int> inventory = {
        {"NICKEL", 0},
        {"DIME", 0},
        {"QUARTER", 0},
        {"ONEDOLLAR", 0},
        {"FIVEDOLLAR", 0},
        {"COKE", 0},
        {"PEPSI", 0},
        {"FANTA", 0},
        {"SPRITE", 0},
        {"WATER", 0},
        {"CUP", 0},
    };

public:
    // Default constructor sets deposit to 0 and unit price to 0.75
    vending_machine()
    {
        deposit_amount = 0;
        drink_price = 0.75;
    }

    // Deposit amount getter
    double get_deposit_amount()
    {
        return deposit_amount;
    }

    // Inventory getter
    map<string, int> get_inventory()
    {
        return inventory;
    }

    // Display Inventory
    void display_current_inventory()
    {
        cout << "-----------------------\n";
        cout << "Money in inventory:\n";
        cout << inventory["FIVEDOLLAR"] << " - FiveDollar\n";
        cout << inventory["ONEDOLLAR"] << " - OneDollar\n";
        cout << inventory["QUARTER"] << " - Quarter\n";
        cout << inventory["DIME"] << " - Dime\n";
        cout << inventory["NICKEL"] << " - Nickel\n";
        cout << "Amount Deposited = " << deposit_amount << "\n\n";

        cout << "Drinks in inventory:\n";
        cout << inventory["COKE"] << " - Coke\n";
        cout << inventory["PEPSI"] << " - Pepsi\n";
        cout << inventory["SPRITE"] << " - Sprite\n";
        cout << inventory["FANTA"] << " - Fanta\n";
        cout << inventory["WATER"] << " - Water\n";
        cout << "Cups available = " << inventory["CUP"] << "\n";
        cout << "-----------------------"
             << endl;
    }

    // For changing inventory values
    // Input string = denomination or brand. Bool check = verify if inside inventory
    bool manage_inventory(string input, int qty, bool check)
    {
        // Check if the brand entered is correct
        if (check)
        {
            int x = inventory[input] + qty;
            // Limit quantity lower bound to 0
            if (x < 0)
            {
                if (!service_mode)
                {
                    cout << "\n!! " << input << " - "
                         << "OUT OF STOCK" << endl;
                    return false;
                }
                inventory[input] = 0;
                cout << "\nRemoved all " << input << endl;
                return true;
            }
            else
            {
                inventory[input] = x;
            }
            return true;
        }
        else
        {
            cout << "\n!! Item not found" << endl;
            // Did not find item in inventory
            return false;
        }
    }

    // For coin deposit only
    void manage_coin(string denomination, int qty)
    {
        denomination = anytoupper(denomination);
        // Check if the denomination entered is correct
        if (manage_inventory(denomination, qty, coin[denomination]))
        {
            // Calculate deposit amount only in User Mode
            if (!service_mode)
            {
                deposit_amount = deposit_amount + (qty * coin[denomination]);
            }
        }
        else
        {
            cout << "!! Wrong Denomination for the command [COIN] \n";
            cout << "!! Use <Nickel|Dime|Quarter>" << endl;
        }
    }

    // For bill deposit only
    void manage_bill(string denomination, int qty)
    {
        denomination = anytoupper(denomination);
        // Check if the denomination entered is correct
        if (manage_inventory(denomination, qty, bill[denomination]))
        {
            // Calculate deposit amount only in User Mode
            if (!service_mode)
            {
                deposit_amount = deposit_amount + (qty * bill[denomination]);
            }
        }
        else
        {
            cout << "!! Wrong Denomination for the command [BILL] \n";
            cout << "!! Use <OneDollar|FiveDollar>" << endl;
        }
    }

    // For drink and cup only
    bool manage_drink(string brand, int qty)
    {
        brand = anytoupper(brand);
        // Check if the brand entered is correct
        if (manage_inventory(brand, qty, (inventory.find(brand) != inventory.end())))
        {
            // If dispensing CUP, not calc necessary
            if (brand != "CUP")
            {
                // Calculate deposit amount and refund only in User Mode
                if (!service_mode)
                {
                    // Check if enough deposit is present
                    if (deposit_amount >= drink_price)
                    {
                        deposit_amount = deposit_amount - drink_price;
                        // Check if refund not processed
                        if (!process_refund())
                        {
                            // Go to original input amount and return deposit as taken in
                            deposit_amount = deposit_amount + drink_price;
                            process_refund();
                            return false;
                        }
                    }
                    else
                    {
                        cout << "!! Drink costs $0.75\n";
                        cout << "Current Balance: $" << deposit_amount << endl;
                        return false;
                    }
                }
            }
        }
        else if (inventory.find(brand) != inventory.end())
        {
            cout << "!! Contact Customer Service to reload the machine" << endl;
            return false;
        }
        else
        {
            cout << "!! Brand not available ";
            cout << "Use <Coke|Pepsi|Fanta|Sprite|Water>" << endl;
            return false;
        }
        return true;
    }

    // Vending operations with quantity limitations. Use for USER mode only
    void vend_drink(string brand)
    {
        brand = anytoupper(brand);

        if (brand == "CUP")
        {
            cout << "!! Brand not available ";
            cout << "Use <Coke|Pepsi|Fanta|Sprite|Water>" << endl;
        }
        else
        {
            if (manage_drink("CUP", -1))
            {
                if (!manage_drink(brand, -1))
                {
                    service_mode = true;
                    manage_drink(brand, 1);
                    manage_drink("CUP", 1);
                    service_mode = false;
                }
                else
                {
                    cout << "\n"
                         << brand << " dispensed" << endl;
                }
            }
        }
    }
    void insert_coin(string denomination)
    {
        manage_coin(denomination, 1);
    }
    void insert_bill(string denomination)
    {
        manage_bill(denomination, 1);
    }

    // Calculates denomination and remaining amount. 0: Amount Remaining 1: Qty Needed
    // Only for internal use while pre-processing refund. Does not change inventory
    vector<int> qty_calc(int cent_input, string denomination, int unit_cent_value)
    {
        vector<int> buffer_vector;
        denomination = anytoupper(denomination);
        int qty_needed = cent_input / unit_cent_value;
        if (qty_needed != 0)
        {
            if (qty_needed > inventory[denomination])
            {
                // cout << "!! Not enough " << denomination << endl;
                qty_needed = inventory[denomination]; // Use all available in inventory
            }
            cent_input = cent_input - (unit_cent_value * qty_needed);

            // cout << qty_needed << " :" << denomination << "\n";
            // cout << "Amount remaining: " << cent_input << "\n";
        }
        buffer_vector.push_back(cent_input);
        buffer_vector.push_back(qty_needed);
        return buffer_vector;
    }

    // Returns true if change get processed. Returns how much change was processed
    bool process_refund()
    {
        // Bypass if no deposit found.
        if (deposit_amount == 0)
        {
            return true;
        }
        // Convert to cents for ease of calculation
        int cent = (int)(deposit_amount * 100);
        vector<int> buffer;

        // Denomination sequence inside vector: 5.00, 1.00, 0.25, 0.10, 0.05
        vector<int> qty;

        // Process all change quantities
        buffer = qty_calc(cent, "FiveDollar", 500);
        cent = buffer[0];
        qty.push_back(buffer[1]);

        buffer = qty_calc(cent, "OneDollar", 100);
        cent = buffer[0];
        qty.push_back(buffer[1]);

        buffer = qty_calc(cent, "Quarter", 25);
        cent = buffer[0];
        qty.push_back(buffer[1]);

        buffer = qty_calc(cent, "Dime", 10);
        cent = buffer[0];
        qty.push_back(buffer[1]);

        buffer = qty_calc(cent, "Nickel", 5);
        cent = buffer[0];
        qty.push_back(buffer[1]);

        // Check if there is enough change available to process amount
        if (cent != 0 || cent < 0)
        {
            cout << "\n!! Not enough change in inventory. Refund could not be processed.\n";
            cout << "!! Please Contact Customer Service to fill machine with change." << endl;
            qty.clear();
            return false;
        }

        // If enough change available, remove amount from inventory
        inventory["FIVEDOLLAR"] = inventory["FIVEDOLLAR"] - qty.at(0);
        inventory["ONEDOLLAR"] = inventory["ONEDOLLAR"] - qty.at(1);
        inventory["QUARTER"] = inventory["QUARTER"] - qty.at(2);
        inventory["DIME"] = inventory["DIME"] - qty.at(3);
        inventory["NICKEL"] = inventory["NICKEL"] - qty.at(4);

        // Display outcome for the customer
        // Denomination sequence in qty: 5.00, 1.00, 0.25, 0.10, 0.05
        cout << "\n$" << deposit_amount << " worth change dispensed:\n";
        (qty.at(0) != 0) ? cout << qty[0] << ": Five Dollar Bills" << endl : cout << "";
        (qty.at(1) != 0) ? cout << qty[1] << ": One Dollar Bills" << endl : cout << "";
        (qty.at(2) != 0) ? cout << qty[2] << ": Quarters" << endl : cout << "";
        (qty.at(3) != 0) ? cout << qty[3] << ": Dimes" << endl : cout << "";
        (qty.at(4) != 0) ? cout << qty[4] << ": Nickels" << endl : cout << "";

        // Clear deposit
        deposit_amount = 0;
        return true;
    }
};