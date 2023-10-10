#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

const int MAX_LEADERS = 5;

struct Player {
    char name[50];
    int guesses;
};

//This method prints all the current leaders in the game
void printLeaders(struct Player leaders[], int numLeaders) {
    printf("Here are the current leaders:\n");
    for (int i = 0; i < numLeaders; i++) {
    	//The for loops prints the name of the player as well as the number of guesses that player took to guess the square
        printf("%s made %d guesses\n", leaders[i].name, leaders[i].guesses);
    }
}

//This method stores the leaderboard information in a file "leaders.txt". It takes an array of Player structures and the number of leaders as input.
void storeLeaders(struct Player leaders[], int numLeaders) {
    FILE *file = fopen("leaders.txt", "w"); //Attempt to open file "leaders.txt" for writing ("w" mode).
    if (file == NULL) { //Null here is used t check whether the file exists or not
        printf("Unable to store leaders to file.\n");
        return; //Return without performing further actions if the file couldn't be opened.
    }

    for (int i = 0; i < numLeaders; i++) { //Iterate through the leaderboard entries and write each player's name and number of guesses in the file.
        fprintf(file, "%s %d\n", leaders[i].name, leaders[i].guesses); //Use fprintf to write the name and guesses to the file in the format "name guesses\n".
    }

    fclose(file); //File closed after entering all the data
}

//This method loads all the leaders from the file
void loadLeaders(struct Player leaders[], int *numLeaders) {
    FILE *file = fopen("leaders.txt", "r");
    if (file == NULL) {
        *numLeaders = 0; //Number of leaders is set to 0 if the file is null/does not exist
        return;
    }

    *numLeaders = 0;
    while (*numLeaders < MAX_LEADERS && fscanf(file, "%s %d", leaders[*numLeaders].name, &leaders[*numLeaders].guesses) == 2) {//The name of the leader and the corresponding number of guesses are obtained from the file using this method
        (*numLeaders)++; //The count of the number of leaders keeps incrementing
    }

    fclose(file);
}

int main() {
    char input;
    struct Player leaders[MAX_LEADERS];
    int numLeaders = 0;
    int welcomeDisplayed = 0; //Check if "Welcome!" has been displayed

    loadLeaders(leaders, &numLeaders);
    srand(time(NULL)); //Time complexity of the randomly generated decimals is null

    while (1) {
        if (!welcomeDisplayed) {
            printf("Welcome! Press 'q' to quit or any other key to continue:\n");
            welcomeDisplayed = 1; //Indicate that "Welcome!" has been displayed
        } else {
            printf("Press 'q' to quit or any other key to continue:\n");
        }

        //Read a character input from the user, including any whitespace characters.
        scanf(" %c", &input);
        if (input == 'q') {
            storeLeaders(leaders, numLeaders); //If 'q' is entered, call the function to store leaders to a file.
            printf("Bye Bye!\n");
            break; //Exit the program loop
        }

        struct Player currentPlayer;
        printf("Please enter your name to start: ");
        scanf("%s", currentPlayer.name);
        currentPlayer.guesses = 0;

        double target = rand() % 91 + 10; //Random target number generated between 10 and 100
        double guess;
        double sqroot = sqrt(target); //Square root of target

        //Display the target and initial guess message once
        printf("%.8lf is the square root of what number? Guess a value between 10 and 100: ", sqroot);

        while (1) {
            scanf("%lf", &guess); //Prompt user to enter their guess
            currentPlayer.guesses++;//Increment the number of guesses made by the current player

            if (guess < target) { //Check if player's guess is less than target
                printf("Too low, guess again: ");
            }
            else if (guess > target) { //Check if player's guess is more than target
                printf("Too high, guess again: ");
            }
            else { //Check if player's guess is equal to target
                printf("You got it, baby!\n"); //Congratulate player for the correct guess
                printf("You made %d guesses.\n", currentPlayer.guesses); //Inform player in how many tries they guessed the target

                //Print the leaderboard in asscending order of number of guesses with least number of guesses at first position and highest number of guesses in the last position
                int i;
                for(i = 0; i < numLeaders; i++){
                	if(currentPlayer.guesses < leaders[i].guesses) { //Comparing number of guesses made by the current player with the guesses of leaders.
                		break;
                	}
                }
                for(int j = numLeaders; j > i; j--) {
                	leaders[j] = leaders[j-1]; //Move each leader down by one spot to make space for the current player if needed.
                }
                leaders[i] = currentPlayer; //Insert the current player into the leaderboard at the correct position.
                if (numLeaders < MAX_LEADERS) { //Check if there is still space in the leaderboard to add the current player.
                    numLeaders++; //increment the number of leaders

                //Display updated leaderboard
                printLeaders(leaders, numLeaders);
                break;
                }
            }
        }
    }

    return 0;
}
