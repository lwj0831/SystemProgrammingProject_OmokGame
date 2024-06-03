#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ncurses.h>
#include<form.h>
#include<ctype.h>
#include<signal.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>

#define PORT 12345
#define BUFFER_SIZE 100

typedef struct {
	char name[30];
	char win[30];
	char lose[30];
} user; // to save user info to file

int displayAndSelectMenu(void);
int inputUserName(void);
void playOmok(void);
int gameEnd(void);
void manual(void);
int finduser(user*, int, char*);
void showUserInfo(user*, int);

user user_arr[50];
int len = 0; // length of user_arr(number of users)
char* board[18][18];
user user1; // 전역변수로 선언
user user2; // 전역변수로 선언

int main(void) {
	// "score.txt" -> user_arr[]
	FILE* fp = NULL;
	fp = fopen("score.txt", "r");
	if (fp == NULL) {
		fp = fopen("score.txt", "w");
		fclose(fp);
		fp = fopen("score.txt", "r");
	}
	char ch;
	char word[30];
	int k = 0, i = 0;
	while (fscanf(fp, "%c", &ch) != EOF) {
		if (ch == ',' || ch == '\n') // delimiter
		{
			word[i] = '\0';
			if (k == 0) { // username
				strcpy(user_arr[len].name, word);
			} else if (k == 1) // win
				strcpy(user_arr[len].win, word);
			else if (k == 2) { // lose
				strcpy(user_arr[len++].lose, word); // update len
				k = -1;
			}
			k++;
			i = 0;
		} else word[i++] = ch;
	}
	// main menu
	int index1;
	int index2;
	int result;
	while (1) {
		int menu_num = displayAndSelectMenu();
		// press Esc
		if (menu_num == -1) break;
		// choose game start menu
		else if (menu_num == 0) {
			result = inputUserName();
			if (result == 27) // press esc
				continue;
			if ((index1 = finduser(user_arr, len, user1.name)) == -1) { // if user1 is not existing user_arr
				strcpy(user1.win, "0");
				strcpy(user1.lose, "0");
			} else { // if user is existing
				strcpy(user1.win, user_arr[index1].win);
				strcpy(user1.lose, user_arr[index1].lose);
			}
			if ((index2 = finduser(user_arr, len, user2.name)) == -1) { // if user2 is not existing user_arr
				strcpy(user2.win, "0");
				strcpy(user2.lose, "0");
			} else { // if user is existing user
				strcpy(user2.win, user_arr[index2].win);
				strcpy(user2.lose, user_arr[index2].lose);
			}
			playOmok();
			user_arr[len++] = user1; // add user1
			user_arr[len++] = user2; // add user2
			FILE* fp2 = NULL;
			if ((fp2 = fopen("score.txt", "w")) == NULL) {
				perror("file open(write mode) is failed\n");
				exit(1);
			}
			for (int i = 0; i < len; i++) {
				fprintf(fp2, "%s,%s,%s\n", user_arr[i].name, user_arr[i].win, user_arr[i].lose);
			}
			fclose(fp2);
		}
		// choose manual menu
		else if (menu_num == 1) {
			manual();
		}
		// choose user info menu
		else if (menu_num == 2) {
			showUserInfo(user_arr, len);
			continue;
		} else break;
	}
	return 0;
}

int displayAndSelectMenu(void) {
	initscr();
	clear();
	cbreak();
	noecho();

	int yMax, xMax;
	getmaxyx(stdscr, yMax, xMax); // get screen size

	WINDOW* window = newwin((yMax / 2), (xMax / 3), (yMax / 4), (xMax / 3));
	box(window, 0, 0);

	// setting for ignore signals except SIGINT(Ctrl+C)
	sigset_t sigs;
	sigfillset(&sigs);
	sigdelset(&sigs, SIGINT);
	sigprocmask(SIG_BLOCK, &sigs, NULL);

	mvwprintw(window, 2, 7, "Main Menu - Choose Menu option!");
	mvwprintw(window, 12, 3, "(Press Esc or Ctrl+C to quit program)");
	wattron(window, A_BOLD); // bold effect
	mvwprintw(window, 0, 15, "Omok");
	wattroff(window, A_BOLD);
	refresh();
	wrefresh(window);
	keypad(window, true); // for use keypad

	char* menus[3] = { "Game start", "Manual", "Search User Info" }; // menu names
	int input_key; // user input key
	int current_pos = 0; // corresponding number of a selected menu
	while (1) {
		for (int i = 0; i < 3; i++) {
			if (i == current_pos) wattron(window, A_REVERSE); // highlight : swaps the foreground and background colors
			mvwprintw(window, i + 6, 7, menus[i]);
			wattroff(window, A_REVERSE);
		}
		input_key = wgetch(window);
		switch (input_key) {
			case KEY_UP:
				current_pos--;
				if (current_pos == -1) current_pos = 2; // press key_up when current position is top of the menus
				break;
			case KEY_DOWN:
				current_pos++;
				if (current_pos == 3) current_pos = 0; // press key_down when current position is bottom of the menus
				break;
			case 27: // press Esc Key to quit main menu
				endwin();
				return -1;
			case 10:
				endwin();
				return current_pos; // return corresponding number of selected menu
		}
	}
}

int finduser(user user_arr[], int len, char* findUsername) { // find user in user_arr and return index of user
	for (int i = 0; i < len; i++) {
		if (strcmp(user_arr[i].name, findUsername) == 0) {
			return i;
		}
	}
	return -1; // failed
}

void manual(void) {
	initscr();
	clear();
	cbreak();
	noecho();

	int yMax, xMax;
	getmaxyx(stdscr, yMax, xMax);

	WINDOW* window = newwin(yMax - 12, xMax - 12, 5, 5);
	box(window, 0, 0);
	mvwprintw(window, 0, 27, "<MANUAL> (Press Any key if you want to go back)");
	mvwprintw(window, 2, 2, "Omok game for two players");
	mvwprintw(window, 4, 2, "1) Game start - Input User1, User2 name and start game!");
	mvwprintw(window, 5, 2, "User1 is a black stone, and user2 is a white stone,");
	mvwprintw(window, 6, 2, "and if there are 5 Go stones in a row, horizontally, vertically, and diagonally the game is won");
	mvwprintw(window, 8, 2, "2) Search User Info - Enter the name you want to find in the field,");
	mvwprintw(window, 9, 2, "then you will show number of wins, loses of that user");
	refresh();
	wrefresh(window);

	wgetch(window); // Press any key to quit
	endwin();
}

void showUserInfo(user user_arr[], int len) {
	initscr();
	clear();
	echo();
	cbreak();
	int xMax, yMax;
	getmaxyx(stdscr, yMax, xMax);

	WINDOW* window = newwin((yMax / 2), (xMax / 2), (yMax / 4), (xMax / 4));
	box(window, 0, 0);
	mvwprintw(window, 2, 4, "name:");
	refresh();
	wrefresh(window);
	keypad(stdscr, TRUE); // use keypad

	char findUsername[20];
    mvwgetnstr(window, 2, 10, findUsername, sizeof(findUsername) - 1);
	mvwprintw(window, 2, 10, "%s", findUsername);
	refresh();
	wrefresh(window);
	int count = 0;
	int isFind = 0;
	for (int i = 0; i < len; i++) {
		if (strstr(user_arr[i].name, findUsername) != NULL) {
			mvwprintw(window, 4 + count, 4, "name: %s win : %s  / lose : %s", user_arr[i].name, user_arr[i].win, user_arr[i].lose);
			count++;
			isFind = 1;
			break;
		}
	}
	if (!isFind) {
		mvwprintw(window, 4, 4, "Cannot find user who's username including %s", findUsername);
		mvwprintw(window, 5, 4, "Please check it and Try again!");
	}
	refresh();
	wrefresh(window);
	getchar(); // press any key to quit
	endwin();
}

int inputUserName(void) {
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);

	FIELD* field[2]; // field for input user names
	FORM* form; // making field
	field[0] = new_field(1, 10, 10, 30, 0, 0);
	field[1] = NULL;

	set_field_back(field[0], A_UNDERLINE); // show underline
	field_opts_off(field[0], O_AUTOSKIP); // set AutoSkip

	form = new_form(field);
	post_form(form);
	refresh();

	mvprintw(1, 1, "Press F1 when you're done typing Your name!"); // conditions for quit
	mvprintw(2, 1, "Press ESC if you want to go back!!");
	mvprintw(10, 25, "name:");
	refresh();

	int ch;
	while (1) {
		while ((ch = getch()) != KEY_F(1)) {
			switch (ch) {
				case KEY_BACKSPACE:
					form_driver(form, REQ_DEL_PREV); // remove
					break;
				case 27: // ESC
					endwin();
					return 27;
				default:
					form_driver(form, ch); // input string
					break;
			}
		}
		// Press F1
        form_driver(form, REQ_NEXT_FIELD);
        form_driver(form, REQ_PREV_FIELD);

        char *temp_name = field_buffer(field[0], 0);
        
        // Remove leading and trailing spaces
        size_t len = strlen(temp_name);
        while (len > 0 && isspace(temp_name[len - 1])) temp_name[--len] = '\0';
        while (*temp_name && isspace(*temp_name)) ++temp_name;

        // Copy the cleaned name to user1.name
        strncpy(user1.name, temp_name, sizeof(user1.name) - 1);
        user1.name[sizeof(user1.name) - 1] = '\0';

        if (strlen(user1.name) == 0) {
            mvprintw(16, 20, "Please enter a valid name without leading/trailing spaces.");
            refresh();
            continue;
        }

		unpost_form(form);
		free_form(form);
		free_field(field[0]);
		endwin();
		return 0; // 정상 종료
	}
}

void playOmok(void) {
	initscr();
	clear();
	cbreak();
	noecho();

	int xMax, yMax;
	signal(SIGINT, SIG_IGN);
	getmaxyx(stdscr, yMax, xMax);
	WINDOW* window = newwin(yMax, xMax, yMax, xMax);
	box(window, 0, 0);
	refresh();
	wrefresh(window);

	keypad(stdscr, TRUE); // use keypad

	// 클라이언트와의 연결
	int server_fd, new_socket;
	struct sockaddr_in address;
	int addrlen = sizeof(address);
	char buffer[BUFFER_SIZE] = { 0 };

	// 소켓 파일 디스크립터 생성
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// 주소 구조체 설정
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	// 소켓을 포트에 바인딩
	if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
		perror("bind failed");
		close(server_fd);
		exit(EXIT_FAILURE);
	}

	// 클라이언트의 연결을 기다림
	if (listen(server_fd, 3) < 0) {
		perror("listen failed");
		close(server_fd);
		exit(EXIT_FAILURE);
	}
	if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
		perror("accept failed");
		close(server_fd);
		exit(EXIT_FAILURE);
	}

	// 서버의 유저 이름 전송
	send(new_socket, user1.name, strlen(user1.name), 0);
	// 클라이언트의 유저 이름 수신
	read(new_socket, user2.name, sizeof(user2.name));

	int count = 0;
	int col, row;
	int xpoint = 3;
	for (int i = 0; i < 18; i++) {
		for (int j = 0; j < 18; j++)
			board[i][j] = "+"; // initialization
	}

	mvprintw(1, 3, "Let's Play Omok Game! - (Press Esc to go back)");
	while (1) {
		mvprintw(22, 53, "%d time", count); // number of times users put the go stone
		for (int i = 0; i < 18; i++) {
			xpoint = 3; // adjusting the x-axis spacing
			// mvprintw(3 + i, 3, "|");
			// mvprintw(3 + i, 58, "|");
			for (int j = 0; j < 18; j++) {
				mvprintw(3 + i, xpoint, "-%s-", board[i][j]);
				xpoint += 3; // print omok
			}
		}
		if (count % 2 == 0) // print which user's turn
			mvprintw(22, 3, "%s's turn", user1.name);
		else
			mvprintw(22, 3, "%s's turn", user2.name);
		move(LINES - 1, COLS - 1);
		refresh();
		wrefresh(window);

		int win = gameEnd();
		if (win == 1) { // user1 win
			wattron(window, A_BOLD);
			mvprintw(2, 17, "Game End - %s win !!!", user1.name); // show the result for 3secs
			refresh();
			wrefresh(window);
			wattroff(window, A_BOLD);
			sleep(3);
			endwin();
			int winCount = atoi(user1.win);
			int loseCount = atoi(user2.lose);
			winCount++;
			loseCount++;
			sprintf(user1.win, "%d", winCount);
			sprintf(user2.lose, "%d", loseCount);
			endwin();
			printf("%s win !!!\n", user1.name); // show the result in terminal prompt
			close(new_socket);
			close(server_fd);
			return;
		} else if (win == 2) { // user2 win
			wattron(window, A_BOLD);
			mvprintw(2, 17, "Game End - %s win !!!", user2.name); // show the result for 3secs
			refresh();
			wrefresh(window);
			wattroff(window, A_BOLD);
			sleep(3);
			int winCount = atoi(user2.win);
			int loseCount = atoi(user1.lose);
			winCount++;
			loseCount++;
			sprintf(user2.win, "%d", winCount);
			sprintf(user1.lose, "%d", loseCount);
			endwin();
			printf("%s win !!!\n", user2.name); // show the result in terminal prompt
			close(new_socket);
			close(server_fd);
			return;
		}

		int key; // implement plates as one-dimensional arrays for effect
		int current_row = 0, current_col = 0;

		// 백돌 차례
		if (count % 2 == 1) {
			// 클라이언트로부터 row, col 정보 받음
			// atoi로 current_row, current_col에 파싱
			int bytes_read = read(new_socket, buffer, BUFFER_SIZE);
			buffer[bytes_read] = '\0'; // Null-terminate the received message
			if (strcmp(buffer, "terminate") == 0) {
				endwin();
				close(new_socket);
				printf("Game terminated by server.\n");
				return;
			}
			char* token = strtok(buffer, ",");
			if (token != NULL) {
				current_row = atoi(token);
				token = strtok(NULL, ",");
				if (token != NULL) {
					current_col = atoi(token);
				}
			}
			board[current_row][current_col] = "O";
			count++;
		} else {
			while (1) {
				for (int i = 0; i < 18; i++) {
					for (int j = 0; j < 18; j++) {
						if (i == current_row && j == current_col) attron(A_REVERSE);
						mvprintw(3 + i, 3 + j * 3 + 1, board[i][j]);
						if (i == current_row && j == current_col) attroff(A_REVERSE);
					}
				}
				key = getch();
				switch (key) {
					case KEY_UP: // when press key up --> up floor
						if (current_row == 0) current_row = 17;
						else current_row -= 1;
						break;
					case KEY_DOWN: // when press key down --> down floor
						if (current_row == 17) current_row = 0;
						else current_row += 1;
						break;
					case KEY_LEFT: // when press key left --> left element
						if (current_col == 0) current_col = 17;
						else current_col -= 1;
						break;
					case KEY_RIGHT: // when press key right--> right element
						if (current_col == 17) current_col = 0;
						else current_col += 1;
						break;
				}

				if (key == 10) { // Press Enter key, put the go stone
					if (strcmp(board[current_row][current_col], "+") != 0) { // if there's already a rock here
						mvprintw(23, 3, "you cannot put it here\n");
						continue;
					} else mvprintw(23, 3, "                           ");

					board[current_row][current_col] = "@";
					count++;

					// 클라이언트로 흑돌 위치 보냄
					char temp[200];
					sprintf(temp, "%d,%d", current_row, current_col);
					send(new_socket, temp, strlen(temp), 0);
					break;
				}

				if (key == 27) { // Press Esc Key
                    send(new_socket, "terminate", strlen("terminate"), 0);
                    endwin();
                    close(new_socket);
                    close(server_fd);
					return;
				}
			}
		}
	}
}

int gameEnd(void) { // function to tell the winner of loser of a game
	for (int i = 0; i < 18; i++) { // examine all elements
		for (int j = 0; j < 18; j++) {
			// five Go stones in row
			if ((j < 14) && strcmp(board[i][j], "@") == 0 &&
				strcmp(board[i][j + 1], "@") == 0 &&
				strcmp(board[i][j + 2], "@") == 0 &&
				strcmp(board[i][j + 3], "@") == 0 &&
				strcmp(board[i][j + 4], "@") == 0)
				return 1;
			// five Go stones in column
			else if ((i < 14) && strcmp(board[i][j], "@") == 0 &&
				strcmp(board[i + 1][j], "@") == 0 &&
				strcmp(board[i + 2][j], "@") == 0 &&
				strcmp(board[i + 3][j], "@") == 0 &&
				strcmp(board[i + 4][j], "@") == 0)
				return 1;
			// five Go stones a diagonal line
			else if ((j < 14) && (i < 14) && strcmp(board[i][j], "@") == 0 &&
				strcmp(board[i + 1][j + 1], "@") == 0 &&
				strcmp(board[i + 2][j + 2], "@") == 0 &&
				strcmp(board[i + 3][j + 3], "@") == 0 &&
				strcmp(board[i + 4][j + 4], "@") == 0)
				return 1;
			// five Go stones a diagonal line
			else if ((i > 4) && strcmp(board[i][j], "@") == 0 &&
				strcmp(board[i - 1][j + 1], "@") == 0 &&
				strcmp(board[i - 2][j + 2], "@") == 0 &&
				strcmp(board[i - 3][j + 3], "@") == 0 &&
				strcmp(board[i - 4][j + 4], "@") == 0)
				return 1;
			// five Go stones in row
			else if ((j < 14) && strcmp(board[i][j], "O") == 0 &&
				strcmp(board[i][j + 1], "O") == 0 &&
				strcmp(board[i][j + 2], "O") == 0 &&
				strcmp(board[i][j + 3], "O") == 0 &&
				strcmp(board[i][j + 4], "O") == 0)
				return 2;
			// five Go stones in col
			else if ((i < 14) && strcmp(board[i][j], "O") == 0 &&
				strcmp(board[i + 1][j], "O") == 0 &&
				strcmp(board[i + 2][j], "O") == 0 &&
				strcmp(board[i + 3][j], "O") == 0 &&
				strcmp(board[i + 4][j], "O") == 0)
				return 2;
			// five Go stones in a diagonal line
			else if ((i < 14) && (j < 14) && strcmp(board[i][j], "O") == 0 &&
				strcmp(board[i + 1][j + 1], "O") == 0 &&
				strcmp(board[i + 2][j + 2], "O") == 0 &&
				strcmp(board[i + 3][j + 3], "O") == 0 &&
				strcmp(board[i + 4][j + 4], "O") == 0)
				return 2;
			// five Go stones in a diagonal line
			else if ((i > 4) && (strcmp(board[i][j], "O")) == 0 &&
				strcmp(board[i - 1][j + 1], "O") == 0 &&
				strcmp(board[i - 2][j + 2], "O") == 0 &&
				strcmp(board[i - 3][j + 3], "O") == 0 &&
				strcmp(board[i - 4][j + 4], "O") == 0)
				return 2;
		}
	}
	return 0; // there is no 5 go stone in one line, so game continues
}
