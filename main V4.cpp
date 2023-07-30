/*
 * Author: Javier Paz
 * Created on July 25th, 2023 10:00 pm
 * Purpose: BlackJack V4
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

// Function to display the graphical representation of cards
void displayCard(int cardValue) {
    cout << " _____ " << endl;
    cout << "|     |" << endl;
    cout << "|  ";
    if (cardValue == 10)
        cout << "10";
    else
        cout << static_cast<char>('0' + cardValue);
    cout << "  |" << endl;
    cout << "|     |" << endl;
    cout << " ----- " << endl;
}

// Function to display the rank and suit of a card
void displayCardInfo(int cardValue) {
    cout << "Rank: ";
    if (cardValue >= 2 && cardValue <= 10)
        cout << cardValue;
    else if (cardValue == 11)
        cout << "Jack";
    else if (cardValue == 12)
        cout << "Queen";
    else if (cardValue == 13)
        cout << "King";
    else if (cardValue == 14)
        cout << "Ace";

    cout << ", Suit: ";
    int suit = rand() % 4;
    switch (suit) {
        case 0:
            cout << "Hearts";
            break;
        case 1:
            cout << "Diamonds";
            break;
        case 2:
            cout << "Clubs";
            break;
        case 3:
            cout << "Spades";
            break;
        default:
            break;
    }
    cout << endl;
}

// Function to display the leaderboard
void displayLeaderboard(string playerNames[], float playerBalances[], int numPlrs) {
    cout << "\n-------- Leaderboard --------" << endl;
    cout << "Player\t\tBalance" << endl;
    cout << "----------------------------" << endl;
    for (int i = 0; i < numPlrs; ++i) {
        cout << setw(10) << playerNames[i] << "\t$" << playerBalances[i] << endl;
    }
    cout << "----------------------------" << endl;
}

int main() {
    // Seed the random number generator
    srand(static_cast<unsigned int>(time(nullptr)));

    cout << "Welcome to Blackjack!" << endl;

    // Get the number of players from the user
    int numPlrs;
    do {
        cout << "Enter the number of players (1-8): ";
        cin >> numPlrs;
    } while (numPlrs < 1 || numPlrs > 8);

    // Player names, balances, and hands arrays
    const int MAX_PLAY = 8; // Moved the constant here to define the array size
    string playerNames[MAX_PLAY];
    float playerBalances[MAX_PLAY];
    const int HAND_SIZE = 2; // Moved the constant here to define the array size
    int playerHands[MAX_PLAY][HAND_SIZE];

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
            int splitHand[2] = {0};
            int splitPoints[2] = {0};
            int numSplits = 0;
            bool canSplit = false;

            for (int j = 0; j < HAND_SIZE; ++j) {
                // Deal initial cards to the player
                playerHands[i][j] = getRandom();
                plrPoints += playerHands[i][j];

                // Check if the player can split the hand
                if (j == 1 && playerHands[i][0] == playerHands[i][1]) {
                    canSplit = true;
                }
            }

            if (canSplit) {
                char splitChoice;
                cout << "Do you want to split your hand? (y/n): ";
                cin >> splitChoice;

                if (splitChoice == 'y' || splitChoice == 'Y') {
                    numSplits = 1;
                    splitHand[0] = playerHands[i][0];
                    splitHand[1] = getRandom();
                    plrPoints -= splitHand[0];
                    plrPoints += splitHand[1];

                    splitPoints[0] = splitHand[0];
                    splitPoints[1] = splitHand[1];
                }
            }

            for (int splitIndex = 0; splitIndex <= numSplits; ++splitIndex) {
                if (splitIndex == 1) {
                    cout << "\n" << playerNames[i] << "'s split hand:" << endl;
                    plrPoints = splitPoints[1];
                } else {
                    cout << "\n" << playerNames[i] << "'s turn:" << endl;
                }

                for (int j = 0; j < HAND_SIZE; ++j) {
                    if (splitIndex == 1 && j == 0) {
                        continue; // Skip displaying the first card of the second hand during the split
                    }

                    cout << "Card " << j + 1 << ":\n";
                    displayCard((splitIndex == 1) ? splitHand[j] : playerHands[i][j]);
                    displayCardInfo((splitIndex == 1) ? splitHand[j] : playerHands[i][j]);
                }

                cout << "Points: " << setw(7) << ((plrPoints > 21) ? "Bust!" : to_string(plrPoints)) << endl;

                // Ask the player to hit, stand, or double down until they stand or bust
                bool doubleDown = false; // Initialize doubleDown variable
                while (plrPoints < 21) {
                    char choice;
                    cout << "Do you want to hit (h), stand (s), or double down (d)? ";
                    cin >> choice;

                    switch (choice) {
                        case 'h':
                            // Deal another card to the player
                            playerHands[i][HAND_SIZE - 1] = getRandom();
                            plrPoints += playerHands[i][HAND_SIZE - 1];
                            for (int j = 0; j < HAND_SIZE; ++j) {
                                cout << "Card " << j + 1 << ":\n";
                                displayCard(playerHands[i][j]);
                                displayCardInfo(playerHands[i][j]);
                            }
                            cout << "Points: " << setw(7) << ((plrPoints > 21) ? "Bust!" : to_string(plrPoints)) << endl;
                            break;
                        case 's':
                            // Player stands
                            break;
                        case 'd':
                            // Double down
                            if (HAND_SIZE == 2) {
                                playerHands[i][1] = getRandom();
                                plrPoints += playerHands[i][1];
                                doubleDown = true;
                            } else {
                                cout << "You can only double down on your initial hand!" << endl;
                            }
                            for (int j = 0; j < HAND_SIZE; ++j) {
                                cout << "Card " << j + 1 << ":\n";
                                displayCard(playerHands[i][j]);
                                displayCardInfo(playerHands[i][j]);
                            }
                            cout << "Points: " << setw(7) << ((plrPoints > 21) ? "Bust!" : to_string(plrPoints)) << endl;
                            break;
                        default:
                            cout << "Invalid choice. Please try again." << endl;
                    }

                    if (choice == 's' || doubleDown) {
                        // Player stands or has double down
                        break;
                    }
                }

                if (plrPoints > 21) {
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
                    cout << "Card 1:\n";
                    displayCard(dlrHand[0]);
                    displayCardInfo(dlrHand[0]);
                    cout << "Points: " << setw(7) << dlrPoints << endl;

                    // Dealer hits until reaching the minimum points
                    const int DLR_MIN = 17; // Moved the constant here
                    while (dlrPoints < DLR_MIN) {
                        dlrHand[HAND_SIZE - 1] = getRandom();
                        dlrPoints += dlrHand[HAND_SIZE - 1];
                        cout << "Card " << HAND_SIZE << ":\n";
                        displayCard(dlrHand[HAND_SIZE - 1]);
                        displayCardInfo(dlrHand[HAND_SIZE - 1]);
                        cout << "Points: " << setw(7) << dlrPoints << endl;
                    }

                    float bet;
                    bool playerWins = false;

                    if (dlrPoints > 21 || plrPoints > dlrPoints) {
                        cout << "Congratulations! You win." << endl;
                        playerWins = true;
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

                    // Apply the double down feature on player's winnings
                    if (doubleDown && playerWins) {
                        bet *= 2;
                        cout << "Congratulations! Your winnings are doubled due to double down!" << endl;
                    }

                    // Update the player's balance based on the bet outcome
                    playerBalances[i] += (playerWins ? bet : -bet);
                    cout << "Current Balance: $" << playerBalances[i] << endl;
                }
            }
        }

        // Display the leaderboard
        displayLeaderboard(playerNames, playerBalances, numPlrs);

        // Ask if the players want to play again
        cout << "\nDo you want to play again (y/n)? ";
        cin >> playAgain;
    } while (playAgain == 'y' || playAgain == 'Y');

    cout << "Thanks for playing Blackjack! Goodbye!" << endl;
    return 0;
}
