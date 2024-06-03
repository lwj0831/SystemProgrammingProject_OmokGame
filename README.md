# OmokGame : SystemProgramming(ELEC462)-2024 Team Project
## 📱 Demo Link

## 🔥 Project Overview
플레이어(server-client)들이 두는 수의 정보를 소켓 통신을 통해 주고 받으며 오목판의 상태를업데이트 및 출력하는 2인용 오목 게임 프로그램
## 🥺 Team 11
|<img src="https://avatars.githubusercontent.com/u/151692917?v=4" width="150" height="150"/>|<img src="https://avatars.githubusercontent.com/u/83826500?v=4" width="150" height="150"/>|<img src="https://avatars.githubusercontent.com/u/145525872?v=4" width="150" height="150"/>|
|:-:|:-:|:-:|
|이원준<br/>[@lwj0831](https://github.com/lwj0831)|심인영<br/>[@dlsdud5962](https://github.com/dlsdud5962)|한준호<br/>[@junho3606](https://github.com/junho3606)| 
## 📚 Environment Setting
* OS : Ubuntu 20.04 LTS
* Language : C
* IDE : Visual Studio
* etc : VM virtual Box
## 📚 Execute Sequence
1. git clone "https://github.com/lwj0831/SystemProgrammingProject_OmokGame.git"
2. cd SystemProgrammingProject_OmokGame
3. make
4. ./server - (in server terminal)
5. ./client - (in client terminal)
## 🔎 Main Features
* 오목 게임 플레이
* 메뉴얼 출력
* 플레이어 승패 정보 조회
## 🤔 Instructions for play

### 메뉴얼 출력
아무 키나 누르면 메뉴얼 출력 종료 후 메인 메뉴로 돌아가기 기능

### 오목 게임

#### 유저명 입력 창
게임 시작 전, 서버와 클라이언트 각각 플레이어명 입력 후 F1키로 입력을 완료  
Esc로 뒤로가기, 메인 메뉴로 돌아가기 가능

#### 본격적인 오목 게임 시작
Enter키를 통해 오목을 두며 만약 이미 돌이 있는 경우에는 둘 수 없는 곳이라는 안내메세지가 하단에 출력, 해당하는 곳에 돌이 없으면 수를 둠
오목 게임의 경우에도 시그널 핸들링을 통해 Ctrl+C or ESC를 누르면 메인 메뉴로 돌아가기 기능 구현

### 유저명에 따른 게임 결과 조회
승패결과를 알고 싶은 플레이어의 이름을 입력 후 Enter



