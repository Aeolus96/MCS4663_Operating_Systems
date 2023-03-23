/********************************
File Name: Vending_Machine.cpp
Part of: Project_Vending_Machine
Written by: Devson Butani
Last Edit: [18/06/2022]
*********************************/

#include "Definitions.h"

// Run main using Definitions.h
int main()
{
    // Initialize command manager with Service mode enabled
    bool exit = false;
    string str;
    vector<string> input;
    // Create vending machine object
    vending_machine machine;

    // Loop acts as a command manager console and switches between states including modes.
    // Allows easy cyclic implementation of the vending machine
    while (!exit)
    {
        // Take command input and push into a vector
        (service_mode) ? cout << "\n[SERVICE MODE] > " : cout << "\n[USER MODE] > ";
        cout << "Enter-Command: ";
        getline(cin, str);
        input = separate(str); // Separates typed commands and values into a vector

        // Convert first input command into an (int) key for switching states and modes
        int key = 0;
        key = command_map[anytoupper(input.at(0))];
        key = valid_key(key, service_mode);

        // Command manager:
        // Conveniently switch between states
        switch (key)
        {
        case 1: // HELP - displays commands that can be used in the current mode
            help_text(service_mode);
            break;
        case 2: // STATUS - read and display current inventory
            machine.display_current_inventory();
            break;
        case 3: // EXIT - exits the command manager and ends the program
            exit = true;
            break;
        case 4: // LOCK - checks password and changes to USER mode
            (compareCommand(input.at(1), passkey)) ? service_mode = false : service_mode = true;
            break;
        case 5: // UNLOCK - checks password and changes to SERVICE mode
            (compareCommand(input.at(1), passkey)) ? service_mode = true : service_mode = false;
            break;
        case 6: // VEND - vends one drink as requested in USER mode
            machine.vend_drink(input.at(1));
            break;
        // case 7 and 8 reserved for extended functionality. Not needed for current scope of operation.
        case 9: // COIN - manages coin input in both modes. Only one quantity allowed in USER mode
            (service_mode) ? machine.manage_coin(input.at(1), string_to_int(input.at(2))) : machine.insert_coin(input.at(1));
            break;
        case 10: // BILL - manages bill input in both modes. Only one quantity allowed in USER mode
            (service_mode) ? machine.manage_bill(input.at(1), string_to_int(input.at(2))) : machine.insert_bill(input.at(1));
            break;
        case 11: // DRINK - manages drink input in SERVICE mode
            machine.manage_drink(input.at(1), string_to_int(input.at(2)));

            break;
        case 12: // CUP - manages cup input in SERVICE mode
            machine.manage_drink("CUP", string_to_int(input.at(1)));
            break;

        default: // Command not recognized
            cout << "\n!! Command not recognized. Enter <HELP> for a list commands." << endl;
            break;
        }

        str.clear();   // Empty string contents for reuse
        input.clear(); // Empty vector contents for reuse
    }
    // End of program
    return 1;
}