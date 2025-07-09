#include <iostream>
#include <vector>
#include <unordered_map>
#include <cstdlib>
#include <ctime>
#include <fstream>
using namespace std;

class Guesser {
public:
    int pickNumber() {
        int num;
        cout << "[Guesser] Enter a number (1 to 100): ";
        cin >> num;
        while (num < 1 || num > 100) {
            cout << "Invalid input. Try again (1-100): ";
            cin >> num;
        }
        return num;
    }
};

class Player {
public:
    int guess(int id) {
        int num;
        cout << "[Player " << id << "] Enter your guess (1 to 100): ";
        cin >> num;
        while (num < 1 || num > 100) {
            cout << "Invalid guess. Try again (1-100): ";
            cin >> num;
        }
        return num;
    }
};

class AIBot {
public:
    int makeSmartGuess() {
        // Random smart guess between 40–60
        int guess = rand() % 21 + 40;
        cout << "[AI Bot] guesses: " << guess << endl;
        return guess;
    }
};

class Umpire {
    int guessedNumber;
    unordered_map<int, int> leaderboard; // player_id → wins
    int aiWins = 0;

public:
    void playRound(int numPlayers, bool includeAI) {
        Guesser guesser;
        guessedNumber = guesser.pickNumber();

        vector<int> guesses(numPlayers);
        vector<int> winners;

        Player player;
        AIBot ai;

        // Player Guesses
        for (int i = 0; i < numPlayers; ++i) {
            guesses[i] = player.guess(i + 1);
            if (guesses[i] == guessedNumber) {
                winners.push_back(i + 1);
                leaderboard[i + 1]++;
            }
        }

        // AI Guess
        bool aiWon = false;
        int aiGuess = -1;
        if (includeAI) {
            aiGuess = ai.makeSmartGuess();
            if (aiGuess == guessedNumber) {
                aiWon = true;
                aiWins++;
            }
        }

        // Print round result
        cout << "\n[Umpire] Guesser chose: " << guessedNumber << endl;
        if (winners.empty() && !aiWon) {
            cout << "No players guessed correctly.\n";
        } else {
            if (!winners.empty()) {
                cout << "Winner(s): ";
                for (int id : winners)
                    cout << "Player " << id << " ";
                cout << endl;
            }
            if (aiWon) cout << "AI Bot also won this round!\n";
        }

        cout << "--------------------------------------\n";
    }

    void showLeaderboard(bool includeAI) {
        cout << "\n========= Leaderboard =========\n";
        for (auto& entry : leaderboard) {
            cout << "Player " << entry.first << ": " << entry.second << " win(s)\n";
        }
        if (includeAI) {
            cout << "AI Bot: " << aiWins << " win(s)\n";
        }
        cout << "===============================\n";
    }

    void saveLeaderboardToFile(bool includeAI) {
        ofstream file("leaderboard.txt");
        if (!file) {
            cout << "Error: Cannot write to file!\n";
            return;
        }

        file << "========= Leaderboard =========\n";
        for (auto& entry : leaderboard) {
            file << "Player " << entry.first << ": " << entry.second << " win(s)\n";
        }
        if (includeAI) {
            file << "AI Bot: " << aiWins << " win(s)\n";
        }
        file << "===============================\n";
        file.close();

        cout << "Leaderboard saved to 'leaderboard.txt'.\n";
    }
};

int main() {
    srand(time(0));
    int numPlayers, rounds;
    char useAI;

    cout << "Welcome to the Smart Number Guessing Game!\n";
    cout << "Enter number of human players: ";
    cin >> numPlayers;
    cout << "Enter number of rounds: ";
    cin >> rounds;
    cout << "Include AI Bot? (y/n): ";
    cin >> useAI;

    bool includeAI = (useAI == 'y' || useAI == 'Y');
    Umpire umpire;

    for (int i = 0; i < rounds; ++i) {
        cout << "\n===== Round " << i + 1 << " =====\n";
        umpire.playRound(numPlayers, includeAI);
    }

    umpire.showLeaderboard(includeAI);
    umpire.saveLeaderboardToFile(includeAI);

    cout << "Thanks for playing!\n";
    return 0;
}
