# OmokGame : SystemProgramming(ELEC462)-2024 Team Project
## 📱 Demo Link

## 🔥 Project Overview
플레이어(server-client)들이 두는 수의 정보를 소켓 통신을 통해 주고 받으며 오목판의 상태를업데이트 및 출력하는 2인용 오목 게임 프로그램
## 🥺 Team 11
|<img src="https://avatars.githubusercontent.com/u/86557146?v=4" width="150" height="150"/>|<img src="https://avatars.githubusercontent.com/u/71973291?v=4" width="150" height="150"/>|<img src="https://avatars.githubusercontent.com/u/151692917?v=4" width="150" height="150"/>|
|:-:|:-:|:-:|:-:|
|이원준<br/>[@lwj0831](https://github.com/lwj0831)|심인<br/>[@dlsdud5962](https://github.com/dlsdud5962)|한준호<br/>[@junho3606](https://github.com/junho3606)|
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
## 🤔 사용자 플로우
* 메뉴얼 출력
서버, 클라이언트 모두, 메인 메뉴에서 메뉴얼 선택 시 게임 설명 메세지 출력
아무 키나 받아서 메뉴얼 출력 종료 후 메인 메뉴로 돌아감
* 오목 게임
<유저명 입력 창>
게임 시작 시, 플레이어들의 게임 결과 기록을 위해 서버와 클라이언트 각각 플레이어명 입력 후 F1키로 입력을 완료
Esc로 뒤로가기, 메인 메뉴로 돌아가기 가능
<본격적인 오목 게임 시작>
게임 시작 시 서버와, 클라이언트 소켓 연결 하여 데이터 주고받을 준비 완료
서버가 흑돌, 클라이언트가 백돌이고 흑돌인 서버가 먼저 턴을 시작
현재 턴에 해당하는 플레이어가 둘 오목의 위치를 표시하기 위해 깜빡깜빡거리는 하이라이팅 기능
하단에 서버의 턴인지 클라이언트의 턴인지 알려주는 메세지와 현재 몇번째 수를 두고 있는 지를 상시 표시
Enter키를 통해 오목을 두며 만약 이미 돌이 있는 경우에는 둘 수 없는 곳이라는 안내메세지가 하단에 출력, 해당하는 곳에 돌이 없으면 수를 둠
이때, 서버와 클라이언트가 통신하며 각 플레이어가 둔 수의 위치를 주거나 받아 오목판 정보 업데이트 및 출력
이렇게 게임이 진행되다 흑돌, 백돌 중 먼저 오목이 된 경우 스크린 창에 오목이 되어 게임이 끝났다는 메세지와 이긴 플레이어의 명을 3초동안 보여줌
동시에 오목 게임 시작 시 서버와 클라이언트에서 각각 입력받은 유저명으로 게임 종료 시 승패 결과를 score.txt 파일에 저장 및 업데이트 후 게임 프로세스 종료
오목 게임의 경우에도 시그널 핸들링을 통해 Ctrl+C or ESC를 누르면 메인 메뉴로 돌아가기 기능 구현
* 유저명에 따른 게임 결과 조회
승패결과를 알고 싶은 플레이어의 이름을 입력받음
score.txt파일을 읽어와 해당 플레이어의 이름을 찾고 존재하면 승패결과를 출력해주고 존재하지 않는경우 유저명을 찾을 수 없다는 메세지 출력



