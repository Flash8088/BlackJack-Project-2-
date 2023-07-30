/*
 * Author: Javier Paz
 * Created on July 25th, 2023 10:00 pm
 * Purpose: BlackJack V1
 */

#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>
using namespace std;

// Function to get a random card value (2 to 11)
int getRandom() {
    return rand() % 10 + 2;
}

int main() {
    const int HAND_SIZE = 2;
    const int BLACKJACK = 21;
    const int DLR_MIN = 17;

    cout << "Welcome to Blackjack!" << endl;

    // Get the number of players from the user
    int numPlrs;
    do {
        cout << "Enter the number of players (1-4): ";
        cin >> numPlrs;
    } while (numPlrs < 1 || numPlrs > 4);

    // Player names, balances, and hands arrays
    string playerNames[4]; // Assuming the maximum number of players is 4
    float playerBalances[4]; // Assuming the maximum number of players is 4
    int playerHands[4][HAND_SIZE]; // Assuming the maximum number of players is 4

    // Seed the random number generator
    srand(static_cast<unsigned int>(time(nullptr)));

    // Game loop
    char playAgain;
    do {
        // Player names input and write to file
        for (int i = 0; i < numPlrs; ++i) {
            cin.ignore(); // To consume the newline character left in the stream after cin >> numPlrs;
            cout << "Enter the name of Player " << i + 1 << ": ";
            getline(cin, playerNames[i]);
            playerBalances[i] = 1000.0; // Initialize all players with $1000
        }

        // Initialize the round for each player
        for (int i = 0; i < numPlrs; ++i) {
            int plrPoints = 0;
            for (int j = 0; j < HAND_SIZE; ++j) {
                // Deal initial cards to the player
                playerHands[i][j] = getRandom();
                plrPoints += playerHands[i][j];
            }

            cout << "\n" << playerNames[i] << "'s turn:" << endl;
            for (int j = 0; j < HAND_SIZE; ++j) {
                cout << setw(7) << playerHands[i][j];
            }
            cout << endl;
            cout << "Points: " << setw(7) << ((plrPoints > BLACKJACK) ? "Bust!" : to_string(plrPoints)) << endl;

            // Ask the player to hit or stand until they stand or bust
            while (plrPoints < BLACKJACK) {
                char choice;
                cout << "Do you want to hit (h) or stand (s)? ";
                cin >> choice;

                switch (choice) {
                    case 'h':
                        // Deal another card to the player
                        playerHands[i][HAND_SIZE - 1] = getRandom();
                        plrPoints += playerHands[i][HAND_SIZE - 1];
                        for (int j = 0; j < HAND_SIZE; ++j) {
                            cout << setw(7) << playerHands[i][j];
                        }
                        cout << endl;
                        cout << "Points: " << setw(7) << ((plrPoints > BLACKJACK) ? "Bust!" : to_string(plrPoints)) << endl;
                        break;
                    case 's':
                        // Player stands
                        break;
                    default:
                        cout << "Invalid choice. Please try again." << endl;
                }

                if (choice == 's') {
                    // Player stands
                    break;
                }
            }

            if (plrPoints > BLACKJACK) {
                cout << "Bust! You lose." << endl;
                float bet;
                while (true) {
                    cout << "Enter your bet (0-100): ";
                    cin >> bet;
                    if (bet >= 0 && bet <= 100) {
                        break;
                    }
                    cout << "Invalid bet. Please try again." << endl;
                }
            } else {
                // Dealer's turn
                int dlrPoints = 0;
                int dlrHand[HAND_SIZE];
                dlrHand[0] = getRandom();
                dlrPoints += dlrHand[0];

                cout << "\nDealer's turn:" << endl;
                cout << setw(7) << dlrHand[0] << endl;
                cout << "Points: " << setw(7) << dlrPoints << endl;

                // Dealer hits until reaching the minimum points
                while (dlrPoints < DLR_MIN) {
                    dlrHand[HAND_SIZE - 1] = getRandom();
                    dlrPoints += dlrHand[HAND_SIZE - 1];
                    cout << setw(7) << dlrHand[HAND_SIZE - 1] << endl;
                    cout << "Points: " << setw(7) << dlrPoints << endl;
                }

                float bet;
                bool playerWins = false; // Added bool variable to track if the player wins

                if (dlrPoints > BLACKJACK || plrPoints > dlrPoints) {
                    cout << "Congratulations! You win." << endl;
                    playerWins = true; // Set playerWins to true
                    while (true) {
                        cout << "Enter your bet (0-100): ";
                        cin >> bet;
                        if (bet >= 0 && bet <= 100) {
                            break;
                        }
                        cout << "Invalid bet. Please try again." << endl;
                    }
                } else if (plrPoints == dlrPoints) {
                    cout << "It's a tie!" << endl;
                } else {
                    cout << "You lose." << endl;
                    while (true) {
                        cout << "Enter your bet (0-100): ";
                        cin >> bet;
                        if (bet >= 0 && bet <= 100) {
                            break;
                        }
                        cout << "Invalid bet. Please try again." << endl;
                    }
                }
            }
        }

        // Ask if the players want to play again
        cout << "\nDo you want to play again (y/n)? ";
        cin >> playAgain;
    } while (playAgain == 'y' || playAgain == 'Y');

    cout << "Thanks for playing Blackjack! Goodbye!" << endl;
    return 0;
}
